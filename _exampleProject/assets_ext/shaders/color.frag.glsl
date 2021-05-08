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
  gl_FragColor = vec4(1.0, 1.0, 0.0, 1.0);
}
