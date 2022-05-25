#version 330 core

in vec3 vertex;
in vec3 normal;

in vec3 matamb;
in vec3 matdiff;
in vec3 matspec;
in float matshin;

uniform mat4 proj;
uniform mat4 view;
uniform mat4 TG;

out vec4 vertexF;
out vec3 normalF;

out vec3 matambF;
out vec3 matdiffF;
out vec3 matspecF;
out float matshinF;

void main()
{	
  // Passar la posició del vèrtex a SCO
  vertexF = (view * TG * vec4(vertex, 1.0));

  // Passar el vector normal a SCO
  mat3 NormalMatrix = (inverse(transpose(mat3(view * TG))));
  normalF = NormalMatrix * normal;

  matambF = matamb;
  matdiffF = matdiff;
  matspecF = matspec;
  matshinF = matshin;

  gl_Position = proj * view * TG * vec4 (vertex, 1.0);
}
