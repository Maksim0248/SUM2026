/**/
#version 430
 
layout(location = 0) out vec4 OutColor;
 
in vec4 DrawColor;
in vec3 DrawNormal;
in vec3 DrawPos;
in vec2 DrawTexCoord;
 
uniform float Time;
 
uniform vec3 Ka;
uniform vec3 Kd;
uniform vec3 Ks;
uniform float Ph;
uniform float Trans;
uniform vec3 CamLoc;
uniform vec3 RndCamDir;
uniform bool IsTexture0;

layout(binding = 0) uniform sampler2D Tex;

vec3 Shade( vec3 P, vec3 N, vec3 V, vec3 R, vec3 L, vec3 LColor, float F )
{
  vec3 color = vec3(0);
 
  // Ka
  color += Ka;// * DrawColor.rgb;
 
  // Kd
  vec3 Diff = Kd;
  if (IsTexture0)
    Diff.rgb = texture(Tex, DrawTexCoord).rgb;
  color += F * LColor * Diff * max(0, dot(N, L));// * DrawColor.rgb;
 
  // Ks
  color += F * LColor * Ks * max(0, pow(dot(R, L), Ph));
  return color;
}

void main( void )
{
  vec3 N = normalize(DrawNormal); 
  vec3 LPos = CamLoc; /*vec3(cos(Time * 0) + 50, 10, sin(Time * 0) * 8)*/
  vec3 L = RndCamDir;/*normalize(LPos - DrawPos);*/
  vec3 D = normalize(vec3(0, 0, 0) - LPos);//vec3(0, -1, 0);
  float F = 1, a = radians(200), b = radians(180);
 
 
  vec3 color = vec3(1, 0, 0);
  //vec3
  //  Ka = vec3(0.1),
  //  Kd = vec3(0.8),
  //  Ks = vec3(0.5);
  //float Ph = 90;
 
  //Ka = vec3(0.2295, 0.08825, 0.0275);
  //Kd = vec3(0.5508, 0.2118, 0.066);
  //Ks = vec3(0.580594, 0.223257, 0.0695701);
  //Ph = 51.2;
 
  float ld = dot(-L, D), start = cos(a), end = cos(b);
  if (ld < start)
    F = 0;
  else if (ld < end)
    F = 1 - (ld - end) / (start - end);
  else
    F = 1;
 
  color = vec3(0);
  // Ka
  color += Ka;// * DrawColor.rgb;
 
  // Kd
  color += F * Kd * max(0, dot(N, L));// * DrawColor.rgb;
 
  // Ks
  vec3 V = normalize(DrawPos - CamLoc);
  vec3 R = reflect(V, N);
  color += F * Ks * max(0, pow(dot(R, L), Ph));
 
  color = Shade(DrawPos, N, V, R, L, vec3(1, 1, 1), F);
  color += Shade(DrawPos, N, V, R, normalize(vec3(1, 1, 1)), vec3(0.30, 0.18, 0.08), 1);
  OutColor = vec4(color, 1);
}