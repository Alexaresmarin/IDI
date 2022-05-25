#version 330 core

out vec4 FragColor;

in vec4 vertexF;
in vec3 normalF;

in vec3 matambF;
in vec3 matdiffF;
in vec3 matspecF;
in float matshinF;

uniform vec3 posFocus;
uniform mat4 view;
uniform vec3 colorFocus;

uniform vec3 llumAmbient;
uniform int fixe;

vec3 Ambient() {
    return llumAmbient * matambF;
}

vec3 Difus (vec3 NormSCO, vec3 L, vec3 colFocus) 
{
    // Fixeu-vos que SOLS es retorna el terme difús
    // S'assumeix que els vectors que es reben com a paràmetres estan normalitzats
    vec3 colRes = vec3(0);
    // Càlcul component difusa, si n'hi ha
    if (dot (L, NormSCO) > 0)
      colRes = colFocus * matdiffF * dot (L, NormSCO);
    return (colRes);
}

vec3 Especular (vec3 NormSCO, vec3 L, vec4 vertSCO, vec3 colFocus) 
{
    // Fixeu-vos que SOLS es retorna el terme especular!
    // Assumim que els vectors estan normalitzats
    vec3 colRes = vec3 (0);
    // Si la llum ve de darrera o el material és mate no fem res
    if ((dot(NormSCO,L) < 0) || (matshinF == 0))
      return colRes;  // no hi ha component especular

    // Calculem R i V
    vec3 R = reflect(-L, NormSCO); // equival a: 2.0*dot(NormSCO,L)*NormSCO - L;
    vec3 V = normalize(-vertSCO.xyz); // perquè la càmera està a (0,0,0) en SCO

    if (dot(R, V) < 0)
      return colRes;  // no hi ha component especular
    
    float shine = pow(max(0.0, dot(R, V)), matshinF);
    return (matspecF * colFocus * shine); 
}

void main()
{	
	vec3 posFocusSCO = posFocus;
	if (fixe == 1) {
		vec4 posFocusSCO4 = view * vec4(posFocus, 1.0);
		posFocusSCO = posFocusSCO4.xyz;
	}

	// Direccio llum
	vec3 L = normalize(posFocus - vertexF.xyz);

	vec3 NM = normalize(normalF);
	
	//Phong
    vec3 fcolor = Ambient() + Difus(NM, L, colorFocus) + Especular(NM , L, vertexF, colorFocus);
	    
	FragColor = vec4(fcolor,1);	
}
