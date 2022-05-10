#version 330 core
layout (location = 0) in vec3 position;
layout (location = 1) in vec3 normal;
layout (location = 2) in vec2 texCoords;

const float amplitude = 0.09;
const float frequency = 2.0;
const float PI = 3.14159;
out vec2 TexCoords;
out vec3 Normal;
out vec3 FragPos;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
uniform float time;

void main()
{

  float distance = length(position);
  float effect = amplitude*sin(-PI*distance*frequency+time);
  gl_Position = projection*view*model*vec4(position.x + effect ,position.y + effect , position.z + effect ,1);
  TexCoords=vec2(texCoords.x + effect ,texCoords.y + effect);

}