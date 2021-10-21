//-----------------------------------------------------------------------------
// RTRDefault.frag
//-----------------------------------------------------------------------------
// RMIT University, COSC1226: Real-Time Rendering and 3D Game Programming
//-----------------------------------------------------------------------------
#version 460
out vec4 f_FragColor;

in VertexData {
    vec3 FragPos;
    vec3 Normal;
    vec2 TexCoord;
} fs_in;

struct RTRCamera {
    vec3 Position;
    vec3 Front;
    vec3 Up;
    vec3 Right;
};

#define RTR_MAX_LIGHTS 10

#define RTRDirectionalLight    0
#define RTRPointLight          1
#define RTRSpotLight           2

#define RTRBasicMaterial       0
#define RTRTexturedMaterial    1
#define RTRReflectiveMaterial  2
#define RTRGlassMaterial       3

struct RTRLight {
    int Type;
    vec3 Ambient;
    vec3 Diffuse;
    vec3 Specular;
    vec3 Direction;
    vec3 Position;
    float Constant;
    float Linear;
    float Quadratic;
    float Cutoff;
    float OuterCutoff;
};

struct RTRMaterial
{
    int Type;               // Used by material type
    vec3 Ambient;           // Basic, Textured, Reflective, Glass
    vec3 Diffuse;           // Basic, Textured, Reflective, Glass
    vec3 Specular;          // Basic, Textured, Reflective, Glass
    float Shininess;        // Basic, Textured, Reflective, Glass
    sampler2D DiffuseMap;   // Textured, Reflective, Glass
    sampler2D SpecularMap;  // Textured, Reflective, Glass
    sampler2D ReflectionMap;// Reflective, Glass
    float RefractiveIndex;  // Glass
};

uniform mat4 u_ModelMatrix;
uniform mat4 u_ViewMatrix;
uniform mat4 u_ProjectionMatrix;

uniform int         u_NumLights;
uniform RTRLight    u_Lights[RTR_MAX_LIGHTS];
uniform RTRMaterial u_ObjectMaterial;
uniform RTRCamera   u_Camera;

uniform float u_CurTime;

uniform samplerCube u_SkyBox;

void main() 
{
    //vec3 N = normalize(cross(dFdx(fs_in.FragPos), dFdy(fs_in.FragPos)));
    vec3 N = normalize(transpose(inverse(mat3(u_ModelMatrix))) * fs_in.Normal);
    
    vec3 final_color = vec3(0.0, 0.0, 0.0);
    for (int cur_light=0; cur_light<u_NumLights; cur_light++) {
        float attenuation = 1.0;

        // calc ambient
        vec3 ambient = u_Lights[cur_light].Ambient * u_ObjectMaterial.Ambient;
    
        // calc diffuse
        vec3 L;
        if (u_Lights[cur_light].Type == RTRDirectionalLight) {
            L = -u_Lights[cur_light].Direction;
        }
        else {
            L = u_Lights[cur_light].Position - fs_in.FragPos;
            float Ld = length(L);
            attenuation = min(
                1.0 / (
                  (u_Lights[cur_light].Constant) +
                  (u_Lights[cur_light].Linear * Ld) +
                  (u_Lights[cur_light].Quadratic * Ld * Ld)
                ), 1);
        }
        L = normalize(L);
        float d = max(dot(N, L), 0.0);
        vec3 diffuse;
        if(u_ObjectMaterial.Type == RTRBasicMaterial){
            diffuse = u_Lights[cur_light].Diffuse * diffuse * d;
        }else{
            diffuse = u_Lights[cur_light].Diffuse * vec3(texture(u_ObjectMaterial.DiffuseMap, fs_in.TexCoord)) * d;
        }
    
        // calc specular
        vec3 V = normalize(u_Camera.Position - fs_in.FragPos);
        // Phong
        //vec3 R = reflect(-L, N);  
        //float s = pow(max(dot(R, V), 0.0), u_ObjectMaterial.Shininess);
        // Blinn-Phong
        vec3 H = normalize(L + V);
        float s = pow(max(dot(N, H), 0.0), u_ObjectMaterial.Shininess);
        vec3 specular;
        if(u_ObjectMaterial.Type == RTRBasicMaterial){
            specular = u_Lights[cur_light].Specular * u_ObjectMaterial.Specular * s;
        }else{
            specular = u_Lights[cur_light].Specular * vec3(texture(u_ObjectMaterial.SpecularMap, fs_in.TexCoord)) * s;
        }

        //Check if reflections need to be calculated
        if(u_ObjectMaterial.Type >= RTRReflectiveMaterial){
            vec3 reflection;

            vec3 I = normalize(fs_in.FragPos - u_Camera.Position);
            vec3 R = reflect(I, normalize(fs_in.Normal));

            //check if glass material
            if(u_ObjectMaterial.Type == RTRGlassMaterial){
                float glass_ratio = 1 / u_ObjectMaterial.RefractiveIndex;
                R = refract(I, normalize(fs_in.Normal), 1 / glass_ratio);
            }

            reflection = vec3(texture(u_SkyBox, R) * texture(u_ObjectMaterial.ReflectionMap, fs_in.TexCoord));
            //reflection = texture(u_SkyBox, R).rgb;
            //final_color += reflection;
            final_color += (ambient + attenuation*(diffuse + specular + reflection));
        }else{
            final_color += (ambient + attenuation*(diffuse + specular));
        }

    }
    
    f_FragColor = vec4(final_color, 1.0);
    //f_FragColor = texture(u_MetalTexture, fs_in.TexCoord);
    //f_FragColor = vec4(fs_in.TexCoord.xy, 0, 1.0);
    
}

