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
uniform vec3 pointLightPos;

uniform vec3 spotLightColor;
uniform float spotLightInnerCutoff;
uniform float spotLightOuterCutoff;

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
	
    float constant;
    float linear;
    float quadratic;

}; 

void main()
{
    // Setting the texture
    vec3 textureDiffuse = vec3(texture(diffuseMap, shaderTexCoord));

    // look up the normal from the normal map, then reorient it with the current model transform via the TBN matrix
    vec3 textureNormal = vec3(texture(normalMap, shaderTexCoord));
    textureNormal = normalize(textureNormal * 2.0f - 1.0f);  // convert range from [0, 1] to [-1, 1]
    vec3 normalDir = normalize(shaderTBN * textureNormal);

    
    //Poitn Light calculation
    lightInstance pointLight;

    pointLight.position = shaderPointLightPosition;
    pointLight.color = pointLightColor;

    pointLight.constant = 0.5f;
    pointLight.linear = 0.014f;
    pointLight.quadratic = 0.00014f;


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

    spotLight.constant = 0.3f;
    spotLight.linear = 0.0014f;
    spotLight.quadratic = 0.0000014f;

    
    float theta = dot(spotLight.direction, normalize(shaderPosition - spotLight.position));
    float epsilon = spotLight.innerCutoff - spotLight.outerCutoff;
    float intensity = clamp((theta - spotLight.outerCutoff) / epsilon, 0.0f, 1.0f);
    
    spotLight.ambient = vec3(0.0);
    spotLight.lightToFragDirection = normalize(spotLight.position - shaderPosition);
    spotLight.diffuse = max(dot(normalDir, spotLight.lightToFragDirection), 0.0f) * spotLight.color * intensity;

    // Specular lighting
    spotLight.specularPower = 64.0f;
    spotLight.specularIntensity = 0.3f;
    reflectDir = reflect(-spotLight.lightToFragDirection, normalDir);
    spotLight.specular = pow(max(dot(reflectDir, viewDir), 0), spotLight.specularPower) * spotLight.color * spotLight.specularIntensity;


    // Final fragment color
    float d = length(shaderPosition - pointLight.position);
    float pointLightAttenuation = 1.0 / (pointLight.constant + pointLight.linear * d + pointLight.quadratic * (d * d));
    d = length(shaderPosition - spotLight.position);
    float spotLightAttenuation = 1.0 / (spotLight.constant + spotLight.linear * d + spotLight.quadratic * (d * d));
    
    vec3 light = (pointLight.ambient + pointLight.diffuse + pointLight.specular) * pointLightAttenuation + (spotLight.ambient + spotLight.diffuse + spotLight.specular) ;//* spotLightAttenuation;
    fragmentColor = vec4(light, 1.0) * vec4(textureDiffuse, 1.0);
    
}
