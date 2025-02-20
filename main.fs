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
uniform float spotLightCutoff;

uniform vec3 eyePosition;

out vec4 fragmentColor;

struct lightInstance {
    vec3 position;
    vec3 direction;  
    vec3 color;
    float cutoff;

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
    
    pointLight.direction = normalize(pointLight.position - shaderPosition);
    pointLight.diffuse = max(dot(normalDir, pointLight.direction), 0.0f) * pointLight.color;

    // Specular lighting
    pointLight.specularPower = 64.0f;
    pointLight.specularIntensity = 0.3f;
    vec3 viewDir = normalize(eyePosition-shaderPosition);
    vec3 reflectDir = reflect(-pointLight.direction, normalDir);
    pointLight.specular = pow(max(dot(reflectDir, viewDir), 0), pointLight.specularPower) * pointLight.color * pointLight.specularIntensity;



    // Spot Light calculation
    lightInstance spotLight;

    spotLight.position = shaderSpotLightPosition;
    spotLight.color = spotLightColor;
    spotLight.direction = normalize(shaderSpotLightDirection);
    spotLight.cutoff = 0;

    spotLight.constant = 0.5f;
    spotLight.linear = 0.014f;
    spotLight.quadratic = 0.00014f;

    vec3 lightToFragment = normalize(shaderPosition - spotLight.position); // Changed direction calculation
    float theta = dot(lightToFragment, spotLight.direction); // Remove the negative
    
    if (theta > spotLight.cutoff) { // Changed comparison operator
        spotLight.ambientIntensity = 0.15f;
        spotLight.ambient = spotLight.color * spotLight.ambientIntensity;

        spotLight.direction = normalize(spotLight.position - shaderPosition);
        spotLight.diffuse = max(dot(normalDir, spotLight.direction), 0.0f) * spotLight.color;

        spotLight.specularPower = 64.0f;
        spotLight.specularIntensity = 0.3f;
        vec3 spotViewDir = normalize(eyePosition - shaderPosition);
        vec3 spotReflectDir = reflect(-spotLight.direction, normalDir);
        spotLight.specular = pow(max(dot(spotReflectDir, spotViewDir), 0), spotLight.specularPower) * spotLight.color * spotLight.specularIntensity;
    } else {
        spotLight.ambient = vec3(0.0f);
        spotLight.diffuse = vec3(0.0f);
        spotLight.specular = vec3(0.0f);
    }


    pointLight.ambient = vec3(0.0f);
    pointLight.diffuse = vec3(0.0f);
    pointLight.specular = vec3(0.0f);
    // Final fragment color
    float d = length(shaderPosition - pointLight.position);
    float pointLightAttenuation = 1.0 / (pointLight.constant + pointLight.linear * d + pointLight.quadratic * (d * d));
    float spotLightAttenuation = 1.0 / (spotLight.constant + spotLight.linear * d + spotLight.quadratic * (d * d));
    
    vec3 light = (pointLight.ambient + pointLight.diffuse + pointLight.specular) * pointLightAttenuation + (spotLight.ambient + spotLight.diffuse + spotLight.specular) * spotLightAttenuation;
    fragmentColor = vec4(light, 1.0) * vec4(textureDiffuse, 1.0);
    
}
