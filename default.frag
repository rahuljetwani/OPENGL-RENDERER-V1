#version 330 core


out vec4 FragColor;             // Outputs colors in RGBA


in vec3 crntPos;                // Imports the current position from the Vertex Shader
in vec3 normal;                 // Imports the normal from the Vertex Shader
in vec3 color;                  // Inputs the color from the Vertex Shader
in vec2 texCoord;               // Inputs the texture coordinates from the Vertex Shader


uniform sampler2D diffuse0;         // Gets the Texture Unit from the main function
uniform sampler2D specular0;         // Gets the Texture Unit from the main function
uniform vec4 lightColor;        // Gets the color of the light from the main function
uniform vec3 lightPos;          // Gets the position of the light from the main function
uniform vec3 camPos;            // Gets the position of the camera from the main function

vec4 directionLight()
{
    // ambient lighting
    float ambient = 0.20f;  
    
    // diffuse lighting
    vec3 norm = normalize(normal);
    vec3 lightDirection = normalize(vec3(1.0f, 1.0f, 0.0f));
    float diffuse  = max(dot(norm, lightDirection), 0.0f);
    
    // specular lighting
    float specularLight = 0.50f;
    vec3 viewDirection  = normalize(camPos - crntPos);
    vec3 reflectionDirection = reflect(-lightDirection, norm);
    float specAmount = pow(max(dot(viewDirection, reflectionDirection), 0.0f), 20);
    float specular = specAmount * specularLight;

    return (texture(diffuse0, texCoord) * (diffuse + ambient) + texture(specular0, texCoord).r * specular) * lightColor;
}

vec4 pointLight()
{
    vec3 lightVec = lightPos - crntPos;
    float dist = length(lightVec);
    float a = 0.08;
    float b = 0.04;
    float inten = 1.0f / (a * dist * dist + b * dist + 1.0f);

    // ambient lighting
    float ambient = 0.20f;  
    
    // diffuse lighting
    vec3 norm = normalize(normal);
    vec3 lightDirection = normalize(lightVec);
    float diffuse  = max(dot(norm, lightDirection), 0.0f);
    
    // specular lighting
    float specularLight = 0.50f;
    vec3 viewDirection  = normalize(camPos - crntPos);
    vec3 reflectionDirection = reflect(-lightDirection, norm);
    float specAmount = pow(max(dot(viewDirection, reflectionDirection), 0.0f), 20);
    float specular = specAmount * specularLight;

    return (texture(diffuse0, texCoord) * lightColor * (diffuse + inten + ambient) + texture(specular0, texCoord).r * specular * inten) * lightColor;
}

vec4 spotLight()
{
    float innerCone = 0.95f;
    float outerCone = 0.90f;
    
    // ambient lighting
    float ambient = 0.10f;  
    
    // diffuse lighting
    vec3 norm = normalize(normal);
    vec3 lightDirection = normalize(lightPos - crntPos);
    float diffuse  = max(dot(norm, lightDirection), 0.0f);
    
    // specular lighting
    float specularLight = 0.50f;
    vec3 viewDirection  = normalize(camPos - crntPos);
    vec3 reflectionDirection = reflect(-lightDirection, norm);
    float specAmount = pow(max(dot(viewDirection, reflectionDirection), 0.0f), 20);
    float specular = specAmount * specularLight;

    float angle = dot(vec3(0.0f, -1.0f, 0.0f), -lightDirection);
    float inten = clamp((angle - outerCone) / (innerCone - outerCone), 0.0f, 1.f);

    return (texture(diffuse0, texCoord) * lightColor * (diffuse + inten + ambient) + texture(specular0, texCoord).r * specular * inten) * lightColor;
}

void main()
{
    
    FragColor = spotLight();
}