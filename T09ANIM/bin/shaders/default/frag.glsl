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
uniform int IsFL;
layout(binding = 0) uniform sampler2D Tex;


#if 0

vec3 Shade( vec3 P, vec3 N, vec3 V, vec3 R, vec3 L, vec3 LColor, float F )
{
  vec3 color = vec3(0);
 
  // Ka
  color += Ka;// * DrawColor.rgb;
 
  // Kd
  vec3 Diff = Kd;
  if (IsTexture0)
    Diff.rgb = texture(Tex, DrawTexCoord).rgb;
  color += F * LColor * Diff * max(0, dot(N, L)); 
  // Ks
  color += F * LColor * Ks * max(0, pow(dot(R, L), Ph));
  return color;
} 

void main( void )
{
  vec3 N = normalize(DrawNormal); 
  vec3 LPos = CamLoc; /*vec3(cos(Time * 0) + 50, 10, sin(Time * 0) * 8)*/
  vec3 LDir = RndCamDir;/*normalize(LPos - DrawPos);*/
  vec3 D = normalize(vec3(0, 0, 0) - LPos);//vec3(0, -1, 0);
  float F = 1, a = radians(200), b = radians(180);
  vec3 L = normalize(LPos - DrawPos);
  float Spot = dot(normalize(RndCamDir), -L);
 
 
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
 
    // Ks
  vec3 V = normalize(DrawPos - CamLoc);
  vec3 R = reflect(V, N);

  color = Shade(DrawPos, N, V, R, L, vec3(1, 1, 1), F);
  color += Shade(DrawPos, N, V, R, normalize(vec3(1, 1, 1)), vec3(1, 1, 0), 1);
  OutColor = vec4((color * 2), 1);
}
#endif

void main( void )
{
  vec3 N = normalize(DrawNormal);          
  vec3 L = normalize(CamLoc - DrawPos);
  vec3 V = normalize(CamLoc - DrawPos);
  vec3 R = reflect(-L, N);
  float Spec = pow(max(dot(R, V), 0.0), Ph);
     
  vec3 DiffColor = Kd;

  float Spot = max(dot(RndCamDir, -L), 0);
  float SpK;
  
  if (Spot > 0.9)
    SpK = 1;
  else if (Spot > 0.8)
    SpK = (Spot - 0.8) / 0.1;
  else
    SpK = 0;
     
  if (IsTexture0)
    DiffColor = texture(Tex, DrawTexCoord).rgb;
                                         
  float Dist = length(CamLoc - DrawPos);
                                        
  float F = max(1.0 - Dist / 40.0, 0.0);
                           
  float Diff = max(dot(N, L), 0.0);
     
  vec3 Color = DiffColor * 0.1;
 
 Color += DiffColor * Diff * F * SpK * 2.5 * IsFL;
  Color += Ks * Spec * F * SpK * IsFL;

  OutColor = vec4(Color, 1.0);
}