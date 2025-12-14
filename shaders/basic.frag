#version 450

// Input from vertex shader
layout(location = 0) in vec3 fragColor;
layout(location = 1) in vec2 fragTexCoord;

// Output color
layout(location = 0) out vec4 outColor;

// Texture sampler (optional, for textured rendering)
layout(binding = 0) uniform sampler2D texSampler;

void main() {
    // For now, output vertex color
    // When texturing is enabled, multiply by texture sample:
    // outColor = texture(texSampler, fragTexCoord) * vec4(fragColor, 1.0);

    outColor = vec4(fragColor, 1.0);
}
