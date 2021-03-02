#version 100

precision mediump float;
attribute vec4 position;
attribute vec4 color;
attribute vec2 texCoord;
uniform mat4 modelViewMat;

varying vec4 vcolor;
varying vec2 UV;

void main()
{
  gl_Position = modelViewMat * position;
  vcolor = color;
  UV = texCoord;
}
