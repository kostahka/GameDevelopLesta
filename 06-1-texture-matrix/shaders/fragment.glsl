#version 300 es
precision mediump float;
in vec2 f_tex_coord;

out vec4 f_color;

uniform sampler2D lava_texture;

void main(){
	f_color = texture2D(lava_texture, f_tex_coord);
}
