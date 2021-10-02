#version 460
out vec4 f_FragColor;

in VertexData {
    vec3 TexCoord;
} fs_in;

uniform samplerCube u_SkyboxTextureId;

void main() 
{
    f_FragColor = texture(u_SkyboxTextureId, fs_in.TexCoord);
}