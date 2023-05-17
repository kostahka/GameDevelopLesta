#version 300 es
precision mediump float;

layout (location = 0) in vec3 v_position;
layout (location = 1) in vec2 v_text_coord;
out vec2 f_tex_coord;

uniform mat3 v_trans_matrix;
uniform float time;

void main(){
    f_tex_coord = v_text_coord;
    vec3 new_position = vec3(v_position.x + 0.03 * sin(time + sin(v_position.x) + cos(v_position.y)),
                             v_position.y + 0.03 * cos(time + cos(v_position.x) + cos(v_position.y)), 1.0);
    gl_Position = vec4((v_trans_matrix * new_position).xy, 0.0, 1.0);
}
