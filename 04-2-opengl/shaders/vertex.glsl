#version 300 es
precision mediump float;

layout (location = 0) in vec3 v_position;
layout (location = 1) in vec4 v_color;
out vec4 v_f_color;

void main(){
    v_f_color = v_color;
    gl_Position = vec4(v_position, 1.0);
}
