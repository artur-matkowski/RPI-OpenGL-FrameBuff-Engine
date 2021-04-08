#version 100
precision mediump float;
attribute vec4 position;
attribute vec4 color;
attribute vec2 texCoord;
uniform vec3 offset;

varying vec4 vcolor;
varying vec2 UV;

void main()
{
  gl_Position = position + vec4(offset, 0.0);
  vcolor = color;
  UV = texCoord;
}
