#version 300 es

precision lowp float;

in vec4 position;
in vec2 uv0;
uniform highp mat4 modelViewMat;

out mediump vec2 vTexCoord;

void main()
{
    vTexCoord = uv0;
    gl_Position = modelViewMat * position;
}
