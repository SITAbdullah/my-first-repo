#version 300 es
layout(location = 0) in vec3 VertexPosition;
layout(location = 1) in float Shade;

uniform mat4 MODELVIEWPROJECTIONMATRIX;

out float CornerShade;

void main()
{
    gl_Position = MODELVIEWPROJECTIONMATRIX * vec4(VertexPosition, 1.0);
    CornerShade = Shade;
}
