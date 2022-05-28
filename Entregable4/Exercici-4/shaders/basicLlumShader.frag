#version 330 core

in vec3  fmatamb;
in vec3  fmatdiff;
in vec3  fmatspec;
in float fmatshin;
in vec3  vertexF;
in vec3  normalF;

const vec3 llumAmbient = vec3(0.2, 0.2, 0.2);

uniform mat4 TG;
uniform mat4 Proj;
uniform mat4 View;

uniform mat4 TGaux;

uniform vec3 colFocusCam;

uniform vec3 posFocusPat;
uniform vec3 colFocusPat;

uniform vec3 posFocusEsc;
uniform vec3 colFocusEsc;

out vec4 FragColor;


vec3 Ambient() {
  return llumAmbient*fmatamb;
}

vec3 Difus (vec3 NormSCO, vec3 L, vec3 colFocus)
{
  // Fixeu-vos que SOLS es retorna el terme de Lambert!
  // S'assumeix que els vectors que es reben com a paràmetres estan normalitzats
  vec3 colRes = vec3(0);
  if (dot (L, NormSCO) > 0)
    colRes = colFocus * fmatdiff * dot (L, NormSCO);
  return (colRes);
}

vec3 Especular (vec3 NormSCO, vec3 L, vec3 vertSCO, vec3 colFocus)
{
  // Fixeu-vos que SOLS es retorna el terme especular!
  // Assumim que els vectors estan normalitzats
  vec3 colRes = vec3 (0);
  // Si la llum ve de darrera o el material és mate no fem res
  if ((dot(NormSCO,L) < 0) || (fmatshin == 0))
    return colRes;  // no hi ha component especular

  vec3 R = reflect(-L, NormSCO); // equival a: 2.0*dot(NormSCO,L)*NormSCO - L;
  vec3 V = normalize(-vertSCO); // perquè la càmera està a (0,0,0) en SCO

  if (dot(R, V) < 0)
    return colRes;  // no hi ha component especular

  float shine = pow(max(0.0, dot(R, V)), fmatshin);
  return (colRes + fmatspec * colFocus * shine);
}

void main()
{   
    vec3 NM = normalize(normalF);
    vec3 fcolor;

    //ESCENA
    vec3 posFocusEscSCO = (View * vec4(posFocusEsc, 1.0)).xyz;
    vec3 LEsc = normalize(posFocusEscSCO - vertexF.xyz);
    fcolor = Ambient() + Difus(NM, LEsc, colFocusEsc) +Especular(NM , LEsc, vertexF, colFocusEsc);

    //PATRICIO
    vec3 posFocusPatSCO = (View * TGaux * vec4(posFocusPat, 1.0)).xyz;
    vec3 LPat = normalize(posFocusPatSCO - vertexF.xyz);
    fcolor += Difus(NM, LPat, colFocusPat) +Especular(NM , LPat, vertexF, colFocusPat);

    //CAMERA
    vec3 posFocusCamera = vec3(0, 0, 0);
    vec3 LCamera = normalize(posFocusCamera - vertexF.xyz);
    fcolor += Difus(NM, LCamera, colFocusCam) +Especular(NM , LCamera, vertexF, colFocusCam);

    FragColor = vec4(fcolor, 1);
}
