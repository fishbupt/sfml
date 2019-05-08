#version 130

// Uniforms
uniform mat4 sf_ModelMatrix;
uniform mat4 sf_ViewMatrix;
uniform mat4 sf_ProjectionMatrix;
uniform mat4 sf_TextureMatrix;
uniform int sf_TextureEnabled;
uniform float wave_phase;
uniform vec2 wave_amplitude;

// Vertex attributes
in vec3 sf_Vertex;
in vec4 sf_Color;
in vec2 sf_MultiTexCoord0;

// Vertex shader outputs
out vec4 sf_FrontColor;
out vec2 sf_TexCoord0;

void main()
{
    // Vertex position
    vec4 vertex = vec4(sf_Vertex, 1.0);
    vertex.x += cos(sf_Vertex.y * 0.02 + wave_phase * 3.8) * wave_amplitude.x
              + sin(sf_Vertex.y * 0.02 + wave_phase * 6.3) * wave_amplitude.x * 0.3;
    vertex.y += sin(sf_Vertex.x * 0.02 + wave_phase * 2.4) * wave_amplitude.y
              + cos(sf_Vertex.x * 0.02 + wave_phase * 5.2) * wave_amplitude.y * 0.3;

    gl_Position = sf_ProjectionMatrix * sf_ViewMatrix * sf_ModelMatrix * vertex;

    // Vertex color
    sf_FrontColor = sf_Color;

    // Texture data
    if (sf_TextureEnabled == 1)
        sf_TexCoord0 = (sf_TextureMatrix * vec4(sf_MultiTexCoord0, 0.0, 1.0)).st;
}
