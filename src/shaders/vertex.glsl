#version 460 core
layout (location = 0) in vec2 vPos;

uniform float uResolution;

void main() {
    gl_Position = vec4(2 * vPos / uResolution - 1.0, 0.0, 1.0);
}