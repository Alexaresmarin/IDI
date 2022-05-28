#version 330 core

in vec3 vertex;
in vec3 normal;

in vec3  matamb;
in vec3  matdiff;
in vec3  matspec;
in float matshin;

uniform mat4 TG;
uniform mat4 Proj;
uniform mat4 View;

out vec3  fmatamb;
out vec3  fmatdiff;
out vec3  fmatspec;
out float fmatshin;
out vec3 vertexF;
out vec3 normalF;


void main()
{	
    // Passem les dades al fragment shader
    fmatamb  = matamb;
    fmatdiff = matdiff;
    fmatspec = matspec;
    fmatshin = matshin;
    // Passar la posició del vèrtex a SCO
    vertexF = (View * TG * vec4(vertex, 1.0)).xyz;

    // Passar el vector normal a SCO
    mat3 NormalMatrix = (inverse(transpose(mat3(View * TG))));
    normalF = NormalMatrix * normal;

    gl_Position = Proj * View * TG * vec4 (vertex, 1.0);
}
