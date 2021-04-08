#version 100
precision mediump float;
varying vec4 vcolor;
varying vec2 UV;
uniform sampler2D texUnit;
uniform float blend;

void main()
{
  gl_FragColor = vec4(1.0) * (blend+1.0);
}
