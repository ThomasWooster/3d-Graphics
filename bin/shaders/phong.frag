// classic phong fragment shader
#version 410

in vec4 vPosition;
in vec3 vNormal;

uniform vec3 Ia; // ambiant light colour

uniform vec3 Id; // diffuse light colour
uniform vec3 Is; // specular light colour
uniform vec3 lightDirection;

uniform vec3 Ka; // ambient material colour
uniform vec3 Kd; // diffuse material colour
uniform vec3 Ks; // specular material colour
uniform float specularPower; // material specular power

uniform vec3 cameraPosition; 

out vec4 FragColour;

void main()
{
    // enshure the normal and light direction are normalized
    vec3 N = normalize(vNormal);
    vec3 L = normalize(lightDirection);
   // L = vec3(0,-1,0);

    // calculate lambert term (negate light direction)
    float lambertTerm = max( 0, min( 1, dot( N, -L ) ) );

    // calculate view vector and reflection vector
    vec3 V = normalize(cameraPosition - vPosition.xyz);
    vec3 R = reflect( L, N );

    // calculate specular term
    float specularTerm = pow( max( 0, dot( R, V ) ), specularPower );

    // calculate each colour properly
    vec3 ambient = Ia * Ka;
    vec3 diffuse = Id * Kd * lambertTerm;
    vec3 specular = Is * Ks * specularTerm;

    // out put final colour

    FragColour = vec4( ambient + diffuse + specular, 1 );

}