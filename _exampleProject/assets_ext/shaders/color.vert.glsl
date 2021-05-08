#version 100

precision mediump float;
attribute vec4 position;
attribute vec4 normal;
attribute vec4 color0;
attribute vec2 uv0;
uniform mat4 modelViewMat;

varying vec4 vcolor;
varying vec2 UV;

void main()
{
  gl_Position = modelViewMat * position;
  vcolor = color0;
  UV = uv0;
}