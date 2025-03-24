/******************************************************************************
 * This fragment shader is exactly the same as in the previous demo. ;)
 *
 * Happy hacking! - eric
 *****************************************************************************/

#version 330 core

// in vec4 shaderColor;
// in vec2 shaderTexCoord;
// uniform sampler2D shaderTexture;
// out vec4 fragmentColor;

// void main()
// {
//     fragmentColor = shaderColor * texture(shaderTexture, shaderTexCoord);
// }    


in vec4 shaderColor;
in vec2 shaderTexCoord;
in mat3 shaderTBN;
in vec3 shaderPointLightPosition;
in vec3 shaderSpotLightPosition;
in vec3 shaderSpotLightDirection;
in vec3 shaderPosition;
in vec4 shaderLightSpacePosition;

uniform sampler2D diffuseMap;
uniform sampler2D normalMap;
uniform sampler2D specularMap;
uniform sampler2D shadowMap;

uniform vec3 pointLightColor;
uniform vec3 pointLightAttenuation;

uniform vec3 spotLightColor;
uniform float spotLightInnerCutoff;
uniform float spotLightOuterCutoff;
uniform vec3 spotLightAttenuation;

uniform bool useNormals;
uniform vec3 eyePosition;

out vec4 fragmentColor;

struct lightInstance {
    vec3 position;
    vec3 lightToFragDirection;  
    vec3 direction;
    vec3 color;
    float innerCutoff;
    float outerCutoff;

    float ambientIntensity;
    float specularPower;
    float specularIntensity;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
	
    vec3 attenuation;

}; 

bool inShadow()
{
    // perform perspective division and rescale to the [0, 1] range to get the coordinates into the depth texture
    vec3 position = shaderLightSpacePosition.xyz / shaderLightSpacePosition.w;
    position = position * 0.5f + 0.5f;

    // if the position is outside the light-space frustum, do NOT put the
    // fragment in shadow, to prevent the scene from becoming dark "by default"
    // (note that if you have a spot light, you might want to do the opposite --
    // that is, everything outside the spot light's cone SHOULD be dark by default)
    if (position.x < 0.0f || position.x > 1.0f
        || position.y < 0.0f || position.y > 1.0f
        || position.z < 0.0f || position.z > 1.0f)
    {
        return false;
    }

    // access the shadow map at this position
    float shadowMapZ = texture(shadowMap, position.xy).r;

    // add a bias to prevent shadow acne
    float bias = 0.0001f;
    shadowMapZ += bias;

    // if the depth stored in the texture is less than the current fragment's depth, we are in shadow
    return shadowMapZ < position.z;
}
///////////////////////////////////////////////////////////////////////////////

void main()
{
    // Setting the texture
    vec3 textureDiffuse = vec3(texture(diffuseMap, shaderTexCoord));
    vec3 textureSpecular = vec3(texture(specularMap, shaderTexCoord));
    // look up the normal from the normal map, then reorient it with the current model transform via the TBN matrix
    vec3 textureNormal;
    if(useNormals) {
        textureNormal = vec3(texture(normalMap, shaderTexCoord));
    }
    else {
        textureNormal = vec3(0.5, 0.5, 1.0);
    }
    textureNormal = normalize(textureNormal * 2.0f - 1.0f);  // convert range from [0, 1] to [-1, 1]
    vec3 normalDir = normalize(shaderTBN * textureNormal);

    
    //Poitn Light calculation
    lightInstance pointLight;

    pointLight.position = shaderPointLightPosition;
    pointLight.color = pointLightColor;
    pointLight.attenuation = pointLightAttenuation;
    
    pointLight.lightToFragDirection = normalize(pointLight.position - shaderPosition);
    pointLight.diffuse = max(dot(normalDir, pointLight.lightToFragDirection), 0.0f) * pointLight.color * textureDiffuse;

    // Specular lighting
    pointLight.specularPower = 64.0f;
    pointLight.specularIntensity = 1.0f;
    vec3 viewDir = normalize(eyePosition-shaderPosition);
    vec3 reflectDir = reflect(-pointLight.lightToFragDirection, normalDir);
    pointLight.specular = pow(max(dot(reflectDir, viewDir), 0), pointLight.specularPower) * pointLight.color * pointLight.specularIntensity * textureSpecular;



    // Spot Light calculation
    lightInstance spotLight;

    spotLight.position = shaderSpotLightPosition;
    spotLight.color = spotLightColor;
    spotLight.direction = normalize(shaderSpotLightDirection);
    spotLight.innerCutoff = spotLightInnerCutoff;
    spotLight.outerCutoff = spotLightOuterCutoff;
    spotLight.attenuation = spotLightAttenuation;

    float theta = dot(spotLight.direction, normalize(shaderPosition - spotLight.position));
    float epsilon = spotLight.innerCutoff - spotLight.outerCutoff;
    float intensity = clamp((theta - spotLight.outerCutoff) / epsilon, 0.0f, 1.0f);
    
    spotLight.lightToFragDirection = normalize(spotLight.position - shaderPosition);
    spotLight.diffuse = max(dot(normalDir, spotLight.lightToFragDirection), 0.0f) * spotLight.color * intensity * textureDiffuse;

    // Specular lighting
    spotLight.specularPower = 64.0f;
    spotLight.specularIntensity = 0.3f;
    viewDir = normalize(eyePosition-shaderPosition);
    reflectDir = reflect(-spotLight.lightToFragDirection, normalDir);
    spotLight.specular = pow(max(dot(reflectDir, viewDir), 0), spotLight.specularPower) * spotLight.color * spotLight.specularIntensity * intensity * textureSpecular;

    
    // Final fragment color
    vec3 ambient = 0.1f * textureDiffuse;
    float d = length(shaderPosition - pointLight.position);
    float pointLightTotalAttenuation = 1.0 / (pointLight.attenuation.x + pointLight.attenuation.y * d + pointLight.attenuation.z * (d * d));
    d = length(shaderPosition - spotLight.position);
    float spotLightTotalAttenuation = 1.0 / (spotLight.attenuation.x + spotLight.attenuation.y * d + spotLight.attenuation.z * (d * d));
    
    if (inShadow())
        spotLight.diffuse = spotLight.specular = vec3(0.0f, 0.0f, 0.0f);

    vec3 light = ambient;
    light += (pointLight.diffuse + pointLight.specular) * pointLightTotalAttenuation;
    light += (spotLight.diffuse + spotLight.specular) * spotLightTotalAttenuation;
    fragmentColor = vec4(light, 1.0);
}
