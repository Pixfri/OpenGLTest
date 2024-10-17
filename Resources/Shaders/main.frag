#version 330 core

layout (location = 0) out vec4 FragColor;

in vec3 color;
in vec2 texCoord;

uniform sampler2D texture0;
uniform sampler2D texture1;

void main() {
    FragColor = mix(texture(texture0, texCoord), texture(texture1, texCoord), 0.2);
}