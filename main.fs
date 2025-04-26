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


// From Vertex Shader
in vec4 shaderColor;
in vec2 shaderTexCoord;
in mat3 shaderTBN;
in vec3 shaderPointLightPosition;
in vec3 shaderSpotLightPosition;
in vec3 shaderSpotLightDirection;
in vec3 shaderPosition;
in vec4 shaderLightSpacePosition;

in vec4 prevPos;
in vec4 currPos;
// Map Uniforms
uniform sampler2D diffuseMap;
uniform sampler2D normalMap;
uniform sampler2D specularMap;
uniform sampler2D shadowMap;
uniform sampler2D celShadingMap;

uniform vec2 windowSize;

//Light Uniforms
uniform bool flashlightMode;
uniform vec3 pointLightColor;
uniform vec3 pointLightAttenuation;
uniform vec3 spotLightColor;
uniform vec3 spotLightDirection;
uniform float spotLightInnerCutoff;
uniform float spotLightOuterCutoff;
uniform vec3 spotLightAttenuation;

// Normal Uniforms
uniform bool useNormals;

// Shadow Uniforms
uniform float shadowSharpness;
uniform bool enableShadows;
// Camera Uniforms
uniform vec3 eyePosition;

//other
uniform bool useCelShading;

layout(location = 0) out vec4 fragmentColor;
layout(location = 1) out vec2 velocity;

struct lightInstance {
    vec3 position;
    vec3 lightDir;  
    vec3 direction;
    vec3 color;
    vec3 attenuation;

    float distance;
    float attenuationFactor;
    // Spot Light Specific
    float innerCutoff;
    float outerCutoff;

    float specularPower;
    float specularIntensity;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;

}; 

int kernelWidth = 5;

vec2 texelSize = 1.0 / textureSize(shadowMap, 0);
vec2 averageBlockerDistance(vec3 position) {

	int blockers = 0;
    float avgBlockerDepth = 0.0f;
    for (int i = -kernelWidth/2; i <= kernelWidth/2; i++) {
        for (int j = -kernelWidth/2; j <= kernelWidth/2; j++) {
            vec2 offset = vec2(float(i), float(j)) * texelSize * 2;
            float shadowDepth = texture(shadowMap, position.xy + offset).r;
            if (shadowDepth < position.z) {
                avgBlockerDepth += shadowDepth;
                blockers++;
            }
        }
    }
    if(blockers > 0) {
        avgBlockerDepth /= blockers;
    }
    
    return vec2(avgBlockerDepth, blockers);
}

float percentShadow()
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
        return 0.0f;
    }

    vec2 avgBlockerDepth = averageBlockerDistance(position);
    float receiverDepth = position.z;
    float lightSize = 500.f;

    float bias = 0.0001f;
    float percentShadow = 0.0;
    int halfWidth = kernelWidth / 2;
    int totalSamples = 0;
    // if there are no blockers, no shadow, fully lit, 0.0f
    if (avgBlockerDepth.y < 1.0) {
        return 0.0f;
    } else {
        float penumbraSize = (receiverDepth - avgBlockerDepth.x) * shadowSharpness / (avgBlockerDepth.x);
        for(int x = -halfWidth; x <= halfWidth; x++) {
            for(int y = -halfWidth; y <= halfWidth; y++) {
                vec2 offset = vec2(x, y) * texelSize * penumbraSize;
                float shadowDepth = texture(shadowMap, position.xy + offset).r;

                if (shadowDepth + bias < position.z) {
                    percentShadow += 1.0f;
                } 
                totalSamples++;
            }
        }
    }

    percentShadow /= totalSamples;
    // if the depth stored in the texture is less than the current fragment's depth, we are in shadow
    return percentShadow;
}

float simplePercentShadow() {
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
        return 0.0f;
    }

    float shadowDepth = texture(shadowMap, position.xy).r;
    float bias = 0.0001f;
    
    if (shadowDepth + bias < position.z) {
        return 1.0f;
    } else {
        return 0.0f;
    }
}
///////////////////////////////////////////////////////////////////////////////

float diffuseAngle(lightInstance light, vec3 normalDir, vec3 textureDiffuse) {
    float nDotL = clamp(dot(normalDir, light.lightDir), 0.0f, 1.0f);
    float celShading = texture(celShadingMap, vec2(nDotL, 0.0)).x;
    if (useCelShading) {
        return celShading;
    } else {
        return nDotL;
    }
}


void pointLightCalculations(inout lightInstance pointLight, vec3 normalDir, vec3 viewDir, vec3 textureDiffuse, vec3 textureSpecular) {
    pointLight.position = shaderPointLightPosition;
    pointLight.color = pointLightColor;
    pointLight.attenuation = pointLightAttenuation;
    pointLight.lightDir = normalize(pointLight.position - shaderPosition);
    pointLight.specularPower = 64.0f;
    pointLight.specularIntensity = 1.0f;
    float diffuseAngle = diffuseAngle(pointLight, normalDir, textureDiffuse);
    
    pointLight.diffuse = diffuseAngle * pointLight.color * textureDiffuse;
    pointLight.specular = pow(max(dot(reflect(-pointLight.lightDir, normalDir), viewDir), 0), pointLight.specularPower) * pointLight.color * pointLight.specularIntensity * textureSpecular;

    pointLight.distance = length(shaderPosition - pointLight.position);
    pointLight.attenuationFactor = 1.0 / (pointLight.attenuation.x + pointLight.attenuation.y * pointLight.distance + pointLight.attenuation.z * (pointLight.distance * pointLight.distance));
}

void spotLightCalculations(inout lightInstance spotLight, vec3 normalDir, vec3 viewDir, vec3 textureDiffuse, vec3 textureSpecular) {
    spotLight.position = shaderSpotLightPosition;
    spotLight.color = spotLightColor;
    spotLight.lightDir = normalize(spotLight.position - shaderPosition);
    spotLight.attenuation = spotLightAttenuation;
    
    spotLight.direction = normalize(shaderSpotLightDirection);
    spotLight.innerCutoff = spotLightInnerCutoff;
    spotLight.outerCutoff = spotLightOuterCutoff;
    spotLight.specularPower = 64.0f;
    spotLight.specularIntensity = 0.3f;

    float theta = dot(spotLight.direction, -spotLight.lightDir);
    float epsilon = spotLight.innerCutoff - spotLight.outerCutoff;
    float intensity = clamp((theta - spotLight.outerCutoff) / epsilon, 0.0f, 1.0f);
    
    float diffuseAngle = diffuseAngle(spotLight, normalDir, textureDiffuse);
    spotLight.diffuse = diffuseAngle * spotLight.color * intensity * textureDiffuse;
    spotLight.specular = pow(max(dot(reflect(-spotLight.lightDir, normalDir), viewDir), 0), spotLight.specularPower) * spotLight.color * spotLight.specularIntensity * intensity * textureSpecular;

    spotLight.distance = length(shaderPosition - spotLight.position);
    spotLight.attenuationFactor = 1.0 / (spotLight.attenuation.x + spotLight.attenuation.y * spotLight.distance + spotLight.attenuation.z * (spotLight.distance * spotLight.distance));
    spotLight.attenuationFactor = 1;
}

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
    vec3 viewDir = normalize(shaderPosition);
    //vec3 viewDir = normalize(eyePosition-shaderPosition);
    
    //Point Light calculation
    lightInstance pointLight;
    pointLightCalculations(pointLight, normalDir, viewDir, textureDiffuse, textureSpecular);

    // Spot Light calculation
    lightInstance spotLight;
    spotLightCalculations(spotLight, normalDir, viewDir, textureDiffuse, textureSpecular);

    // Final fragment color
    vec3 ambient = 0.2f * textureDiffuse;
    if (enableShadows) {
        spotLight.diffuse *= 1.0 - percentShadow();
        spotLight.specular *= 1.0 - percentShadow();
    }
    vec3 light = ambient;
    light += (pointLight.diffuse + pointLight.specular) * pointLight.attenuationFactor;
    light += (spotLight.diffuse + spotLight.specular) * spotLight.attenuationFactor;

    fragmentColor = vec4(light, 1.0f);

    
    float e = 0.0001;
    if (currPos.w == 0.0 || prevPos.w == 0.0) {
        velocity = vec2(0.0, 0.0);
    } else {
        vec2 currScreenPos = currPos.xy / currPos.w;
        vec2 prevScreenPos = prevPos.xy / prevPos.w;
        velocity = (currScreenPos - prevScreenPos) * 0.5 + 0.5;
    }
}
