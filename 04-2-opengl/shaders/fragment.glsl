#version 300 es
precision mediump float;
in vec4 v_f_color;
uniform float time;
out vec4 FragColor;

void main(){
    vec2 coord = gl_FragCoord.xy;
    vec2 center = vec2(320.0, 240.0);
    float dist = length(center - coord);
    float circlesOut = cos(dist/7.0 - time * 6.0);
    circlesOut *= 5.0;
    float circlesIn = cos(dist/7.0 + time * 6.0);
    circlesIn *= 5.0;
    circlesIn = clamp(circlesIn,0.0,1.0);
    circlesOut = clamp(circlesOut,0.0,1.0);
    float edge = clamp(dist-100.0,0.0,1.0);
    circlesOut *= edge;
    circlesIn *= 1.0-edge;
    float c = circlesOut + circlesIn;
	FragColor = vec4(c,c,c,0.0);
}
