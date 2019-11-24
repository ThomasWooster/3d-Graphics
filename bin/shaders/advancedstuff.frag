// a normal map fragment shader
#version 410

in vec2 vTexCoord;
in vec3 vNormal;
in vec3 vTangent;
in vec3 vBiTangent;
in vec4 vPosition;

out vec4 FragColour;

uniform sampler2D diffuseTexture;
uniform sampler2D specularTexture;
uniform sampler2D normalTexture;

uniform vec3 Ka; // matrial ambient
uniform vec3 Kd; // material diffuse
uniform vec3 Ks; // material specular
uniform float specularPower;

uniform vec3 Ia; // light ambiant
uniform vec3 Id; // light diffuse
uniform vec3 Is; // light specular
uniform vec3 lightDirection;

uniform vec4 cameraPosition;

uniform float roughness;
uniform float reflectionCoefficient;


float orenNayar(vec3 N, vec3 L, vec3 E)
{
    float Ndl = max( 0.0f, dot( N, L ) );
    float Nde = max( 0.0f, dot( N, E ) );

    float R2 = roughness * roughness;

    // Oren-Nayar Diffuse Term
    float A = 1.0f - 0.5f * R2 / (R2 + 0.33f);
    float B = 0.45f * R2 / (R2 + 0.09f);

    // CX = Max(0, cos(L, E))
    vec3 lightProjected = normalize( L - N * Ndl );
    vec3 viewProjected = normalize( E - N * Nde );
    float CX = max( 0.0f, dot( lightProjected, viewProjected ) );

    // DX = sin(alpha) * tan(beta)
    float alpha = sin( max( acos( Nde ), acos( Ndl ) ) );
    float beta = tan( min( acos( Nde ), acos( Ndl ) ) );
    float DX = alpha * beta;

    // calculate oren-nayar, replaces the p[hong lambern term
    float OrenNayar = Ndl * (A + B * CX * DX);

    return OrenNayar;
}

float cookTorrence(vec3 N, vec3 L, vec3 E)
{
    float Ndl = max( 0.0f, dot( N, L ) );
    float Nde = max( 0.0f, dot( N, E ) );
    vec3 H = normalize( L + E );
    float R2 = roughness * roughness;

    float NdH = max( 0.0f, dot( N, H ) );
    float NdH2 = NdH * NdH;
    float e = 2.71828182845904523536028747135f;
    float pi = 3.1415926535897932384626433832f;

    // Beckman's Distribution function D
    float exponent = -(1 - NdH2) / (NdH2 * R2);
    float D = pow(e, exponent) / (R2 * NdH2 * NdH2);

    // freshnell term F
    float F = reflectionCoefficient + (1 - reflectionCoefficient) * pow( 1 - Nde, 5 );

    // geometric attenuation factor G
    float X = 2.0f * NdH / dot( E, H );
    float G = min(1, min(X * Ndl, X* Nde));

    // calculate th3e cook torrence
    float CookTorrance = max( (D * G * F) / (Nde * pi), 0.0f );

    return CookTorrance;

}

void main()
{
    vec3 N = normalize(vNormal);
    vec3 T = normalize(vTangent);
    vec3 B = normalize(vBiTangent);
    vec3 L = normalize(lightDirection);

    mat3 TBN = mat3(T, B, N);


    vec3 texDiffuse = texture( diffuseTexture, vTexCoord ).rgb;
    vec3 texSpecular = texture( specularTexture, vTexCoord ).rgb;
    vec3 texNormal = texture( normalTexture, vTexCoord ).rgb;

    N = TBN * (texNormal * 2 - 1);

    // calculate the view vector and reflection vector
    vec3 V = normalize(cameraPosition.xyz - vPosition.xyz);
    vec3 R = reflect( L, N );

    // calculate lambert term
    float lambertTerm = orenNayar(N, L, V);

    // calculate the specular term
    float specularTerm = cookTorrence(N, L, V);

    // calculate each light property
    vec3 ambient = Ia * Ka;
    vec3 diffuse = Id * Kd * texDiffuse * lambertTerm;
    vec3 specular = Is * Ks * texSpecular * specularTerm;

    FragColour = vec4(ambient + specular + diffuse, 1);
}