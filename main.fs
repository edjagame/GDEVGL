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

uniform sampler2D diffuseMap;
uniform sampler2D normalMap;

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

void main()
{
    // Setting the texture
    vec3 textureDiffuse = vec3(texture(diffuseMap, shaderTexCoord));

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
    pointLight.ambientIntensity = 0.15f;
    pointLight.ambient = pointLight.color * pointLight.ambientIntensity;
    
    pointLight.lightToFragDirection = normalize(pointLight.position - shaderPosition);
    pointLight.diffuse = max(dot(normalDir, pointLight.lightToFragDirection), 0.0f) * pointLight.color;

    // Specular lighting
    pointLight.specularPower = 64.0f;
    pointLight.specularIntensity = 0.3f;
    vec3 viewDir = normalize(eyePosition-shaderPosition);
    vec3 reflectDir = reflect(-pointLight.lightToFragDirection, normalDir);
    pointLight.specular = pow(max(dot(reflectDir, viewDir), 0), pointLight.specularPower) * pointLight.color * pointLight.specularIntensity;



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
    
    spotLight.ambient = vec3(0.0);
    spotLight.lightToFragDirection = normalize(spotLight.position - shaderPosition);
    spotLight.diffuse = max(dot(normalDir, spotLight.lightToFragDirection), 0.0f) * spotLight.color * intensity;

    // Specular lighting
    spotLight.specularPower = 64.0f;
    spotLight.specularIntensity = 0.3f;
    viewDir = normalize(eyePosition-shaderPosition);
    reflectDir = reflect(-spotLight.lightToFragDirection, normalDir);
    spotLight.specular = pow(max(dot(reflectDir, viewDir), 0), spotLight.specularPower) * spotLight.color * spotLight.specularIntensity * intensity;


    // Final fragment color
    float d = length(shaderPosition - pointLight.position);
    float pointLightTotalAttenuation = 1.0 / (pointLight.attenuation.x + pointLight.attenuation.y * d + pointLight.attenuation.z * (d * d));
    d = length(shaderPosition - spotLight.position);
    float spotLightTotalAttenuation = 1.0 / (spotLight.attenuation.x + spotLight.attenuation.y * d + spotLight.attenuation.z * (d * d));
    
    
    vec3 light = (pointLight.ambient + pointLight.diffuse + pointLight.specular) * pointLightTotalAttenuation + (spotLight.ambient + spotLight.diffuse + spotLight.specular) * spotLightTotalAttenuation;
    fragmentColor = vec4(light, 1.0) * vec4(textureDiffuse, 1.0);
    
}
