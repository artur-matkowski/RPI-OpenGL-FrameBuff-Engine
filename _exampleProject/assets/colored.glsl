##VERT:
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
##FRAG:
#version 100

precision mediump float;
varying vec4 vcolor;
varying vec2 UV;
uniform sampler2D texUnit;
uniform float blend;

void main()
{
  //gl_FragColor = texture2D(texUnit, UV);
  //gl_FragColor = texture2D(texUnit, UV) * vcolor * (blend+1.0);
  gl_FragColor = vec4(1.0, 1.0, 1.0, 1.0) * (blend);
}
