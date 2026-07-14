#version 300 es
precision mediump float;

in float CornerShade;
uniform vec3 PARTCOLOR;
out vec4 FragColor;

void main()
{
    FragColor = vec4(PARTCOLOR * CornerShade, 1.0);
}
