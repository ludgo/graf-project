#version 330
// A texture is expected as program attribute
uniform sampler2D Texture;

// Direction of light
uniform vec3 LightDirection1;
uniform vec3 LightDirection2;

// (optional) Transparency
uniform float Transparency;

// (optional) Texture offset
uniform vec2 TextureOffset;

// The vertex shader will feed this input
in vec2 texCoord;

// Wordspace normal passed from vertex shader
in vec4 normal;

in vec3 position;

// The final color
out vec4 FragmentColor;

void main() {
  // Compute diffuse lighting
  float diffuse1 = max(dot(normal, vec4(normalize(LightDirection1), 1.0f)), 0.0f);
  //float diffuse2 = max(dot(normal, vec4(normalize(LightDirection2), 1.0f)), 0.0f);
  //float diffuse = max(diffuse1, diffuse2);

  vec3 L = LightDirection1*10;
  vec3 V = -position;
  vec3 H = normalize(L + V);
  float specular = pow(max(dot(normal, vec4(normalize(H), 1.0f)), 0.0f), 50.0f);

  // Lookup the color in Texture on coordinates given by texCoord
  // NOTE: Texture coordinate is inverted vertically for compatibility with OBJ
  FragmentColor = texture(Texture, vec2(texCoord.x, 1.0 - texCoord.y) + TextureOffset) * diffuse1;
  FragmentColor += vec4(1, 1, 1, 1) * specular;
  FragmentColor.a = Transparency;
}
