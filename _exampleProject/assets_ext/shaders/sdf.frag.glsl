#version 300 es
uniform sampler2D texUnit;
uniform lowp vec4 bgColor;
uniform lowp vec4 fgColor;
precision lowp float;
in mediump vec2 vTexCoord;
out lowp vec4 FragColor;
//uniform float pxRange; // set to distance field's pixel range
const float pxRange = 1.0; // set to distance field's pixel range
float screenPxRange() 
{
    vec2 unitRange = vec2(pxRange)/vec2(textureSize(texUnit, 0));
    vec2 screenTexSize = vec2(1.0)/fwidth(vTexCoord);
    return max(0.5*dot(unitRange, screenTexSize), 1.0);
}
float median(float r, float g, float b) {
    return max(min(r, g), min(max(r, g), b));
}
void main() 
{
    vec3 msd = texture(texUnit, vTexCoord).rgb;
    float sd = median(msd.r, msd.g, msd.b);
    float screenPxDistance = screenPxRange()*(sd - 0.5);
    float opacity = clamp(screenPxDistance + 0.5, 0.0, 1.0);
    FragColor = mix(bgColor, fgColor, opacity);
  	//FragColor.rgb = vec3(1.0);
    //FragColor.rgba = vTexCoord.yyyy;
}