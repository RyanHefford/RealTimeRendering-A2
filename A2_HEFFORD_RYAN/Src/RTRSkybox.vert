#version 460

layout (location = 0) in vec3 a_Position;

uniform mat4 u_ModelMatrix;
uniform mat4 u_ViewMatrix;
uniform mat4 u_ProjectionMatrix;

out VertexData {
    vec3 TexCoord;
} vs_out;

void main() {
    vs_out.TexCoord = vec3(u_ModelMatrix * vec4(a_Position, 1.0));
    vec4 pos = u_ProjectionMatrix * u_ViewMatrix * u_ModelMatrix * vec4(a_Position, 1.0);
    gl_Position = pos.xyww;
}