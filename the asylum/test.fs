#version 330 core

// Input varyings from vertex shader
in vec3 shaderPosition;
in vec3 shaderNormal;
in vec2 shaderTexCoord;
in vec4 shaderColor; // Received color
in mat3 TBN;
in vec4 fragPosLightSpace; // For shadows

// <<< ADDED: Input velocity
in vec2 v_Velocity;

// Output fragment data
layout (location = 0) out vec4 fragmentColor; // Output to color attachment 0
layout (location = 1) out vec2 velocityOutput; // <<< ADDED: Output to velocity attachment 1

// Textures
uniform sampler2D diffuseMap;
uniform sampler2D normalMap;
uniform sampler2D specularMap;
uniform sampler2D shadowMap; // For shadows

// Uniforms
uniform vec3 eyePosition;
uniform bool useNormals;
uniform bool enableShadows;
uniform float shadowSharpness;

// Point Light
uniform vec3 pointLightPos;
uniform vec3 pointLightColor;
uniform vec3 pointLightAttenuation; // x=constant, y=linear, z=quadratic

// Spot Light
uniform vec3 spotLightPos;
uniform vec3 spotLightColor;
uniform vec3 spotLightDir;
uniform vec3 spotLightAttenuation; // x=constant, y=linear, z=quadratic
uniform float spotLightInnerCutoff; // cosine of angle
uniform float spotLightOuterCutoff; // cosine of angle

// Shadow calculation function (simplified)
float calculateShadow(vec4 fragPosLightSpace, vec3 normal, vec3 lightDir)
{
    // Perspective divide
    vec3 projCoords = fragPosLightSpace.xyz / fragPosLightSpace.w;
    // Transform to [0,1] range
    projCoords = projCoords * 0.5 + 0.5;

    // Get closest depth from shadow map
    float closestDepth = texture(shadowMap, projCoords.xy).r;
    // Get current depth
    float currentDepth = projCoords.z;

    // Check if fragment is outside the light's frustum (for perspective shadows)
    if(projCoords.z > 1.0) return 0.0;

    // Bias to prevent shadow acne
    float bias = max(0.005 * (1.0 - dot(normal, lightDir)), 0.0005);

    // PCF (Percentage Closer Filtering) for softer shadows
    float shadow = 0.0;
    vec2 texelSize = 1.0 / textureSize(shadowMap, 0);
    int pcfRange = int(shadowSharpness); // Use sharpness to control range (adjust multiplier as needed)
    float count = 0.0;

    for(int x = -pcfRange; x <= pcfRange; ++x)
    {
        for(int y = -pcfRange; y <= pcfRange; ++y)
        {
            float pcfDepth = texture(shadowMap, projCoords.xy + vec2(x, y) * texelSize).r;
            shadow += (currentDepth - bias) > pcfDepth ? 1.0 : 0.0; // 1.0 if in shadow
            count += 1.0;
        }
    }
    shadow /= count; // Average shadow contribution

    return 1.0 - shadow; // Return 1.0 if not in shadow, 0.0 if in shadow
}


// Lighting calculation function (example)
vec3 calculateLighting(vec3 normal, vec3 fragPos, vec3 viewDir, vec3 diffuseColor, vec3 specularColor) {
    vec3 totalLight = vec3(0.0); // Start with ambient (or add later)

    // --- Point Light ---
    vec3 pointLightDir = normalize(pointLightPos - fragPos);
    float pointDist = length(pointLightPos - fragPos);
    float pointAttenuation = 1.0 / (pointLightAttenuation.x + pointLightAttenuation.y * pointDist + pointLightAttenuation.z * (pointDist * pointDist));

    // Diffuse
    float pointDiff = max(dot(normal, pointLightDir), 0.0);
    vec3 pointDiffuse = pointLightColor * pointDiff * diffuseColor;

    // Specular
    vec3 pointReflectDir = reflect(-pointLightDir, normal);
    float pointSpec = pow(max(dot(viewDir, pointReflectDir), 0.0), 32.0); // Shininess = 32
    vec3 pointSpecular = pointLightColor * pointSpec * specularColor;

    totalLight += pointAttenuation * (pointDiffuse + pointSpecular);


    // --- Spot Light ---
    vec3 spotLightDirNorm = normalize(spotLightPos - fragPos); // Direction from frag to light
    float spotDist = length(spotLightPos - fragPos);
    float spotAttenuation = 1.0 / (spotLightAttenuation.x + spotLightAttenuation.y * spotDist + spotLightAttenuation.z * (spotDist * spotDist));

    float theta = dot(spotLightDirNorm, normalize(-spotLightDir)); // Angle between light->frag and light's direction
    float epsilon = spotLightInnerCutoff - spotLightOuterCutoff;
    float intensity = clamp((theta - spotLightOuterCutoff) / epsilon, 0.0, 1.0);

    // Diffuse
    float spotDiff = max(dot(normal, spotLightDirNorm), 0.0);
    vec3 spotDiffuse = spotLightColor * spotDiff * diffuseColor;

    // Specular
    vec3 spotReflectDir = reflect(-spotLightDirNorm, normal);
    float spotSpec = pow(max(dot(viewDir, spotReflectDir), 0.0), 32.0);
    vec3 spotSpecular = spotLightColor * spotSpec * specularColor;

    // Shadow calculation (only for spotlight in this example)
    float shadow = 1.0;
    if (enableShadows) {
        shadow = calculateShadow(fragPosLightSpace, normal, spotLightDirNorm);
    }

    totalLight += spotAttenuation * intensity * shadow * (spotDiffuse + spotSpecular);

    // Add a small ambient light component
    vec3 ambient = 0.1 * diffuseColor;
    totalLight += ambient;

    return totalLight;
}

void main()
{
    // Sample diffuse color from texture
    vec4 texColor = texture(diffuseMap, shaderTexCoord);
    // Discard transparent fragments (optional, based on your assets)
    // if(texColor.a < 0.1) discard;

    // Get normal
    vec3 normal;
    if (useNormals) {
        // Sample normal from normal map and transform to world space
        normal = texture(normalMap, shaderTexCoord).rgb;
        normal = normalize(normal * 2.0 - 1.0); // Transform from [0,1] to [-1,1] range
        normal = normalize(TBN * normal); // Transform from tangent to world space
    } else {
        normal = normalize(shaderNormal); // Use interpolated vertex normal
    }

    // Get specular intensity from specular map
    vec3 specularColor = texture(specularMap, shaderTexCoord).rgb;

    // Calculate view direction
    vec3 viewDir = normalize(eyePosition - shaderPosition);

    // Calculate final lighting
    vec3 lighting = calculateLighting(normal, shaderPosition, viewDir, texColor.rgb, specularColor);

    // Final color is texture color modulated by lighting
    fragmentColor = vec4(lighting, texColor.a); // Preserve alpha

    // <<< ADDED: Output the calculated velocity
    velocityOutput = v_Velocity;
}