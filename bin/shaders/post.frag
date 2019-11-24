 // post-prossesing fragment shader
#version 410

in vec2 vTexCoord;

uniform sampler2D colourTarget;

out vec4 FragColour;

// just out put the colour unchanged
vec4 Default(vec2 texCoord) {
	return texture( colourTarget, texCoord );
}

// simplebox blur
vec4 BoxBlur(vec2 texCoord)
{
    vec2 texel = 1.0f / textureSize(colourTarget, 0);

    // 9_tap box kernel
    vec4 colour = texture(colourTarget, texCoord);
    colour += texture(colourTarget, texCoord + texel * vec2(-1, 1));
    colour += texture(colourTarget, texCoord + texel * vec2(-1, 0));
    colour += texture(colourTarget, texCoord + texel * vec2(-1, -1));
    colour += texture(colourTarget, texCoord + texel * vec2(0, 1));
    colour += texture(colourTarget, texCoord + texel * vec2(0, -1));
    colour += texture(colourTarget, texCoord + texel * vec2(1, 1));
    colour += texture(colourTarget, texCoord + texel * vec2(1, 0));
    colour += texture(colourTarget, texCoord + texel * vec2(1, -1));

    return colour / 9;
}

// distortion stuff
vec4 Distort(vec2 texCoord)
{
    vec2 mid = vec2(0.5f);

    float distanceFromCenter = distance(texCoord, mid);
    vec2 normalizedCoord = normalize(texCoord - mid);
    float bias = distanceFromCenter + sin(distanceFromCenter * 15) * 0.05f;

    vec2 newCoord = mid + bias * normalizedCoord;

    return texture(colourTarget, newCoord);
}



/////////////////
void main() 
{
	// calculate texil size
	vec2 texSize = textureSize( colourTarget, 0 );
	vec2 texelSize = 1.0f / texSize;

	// adjust texture coordinate
	vec2 scale = (texSize - texelSize) / texSize;
	vec2 texCoord = vTexCoord / scale + texelSize * 0.5f;

	// sample post effect
	FragColour = Default(texCoord);
}