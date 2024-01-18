#version 330 core
layout (location = 0) out vec4 FragColor;
layout (location = 1) out vec4 BrightColor;

struct Light {
    vec3 Position;
    vec3 Color;
    // vec3 direction;
    // float cutOff;
    // float outerCutOff;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;

    float constant;
    float linear;
    float quadratic;
};

# define MAX_POINT_LIGHTS 4

in vec3 Normal;  
in vec3 FragPos;  
in vec2 TexCoords;

uniform vec3 viewPos;
uniform sampler2D diffuseTexture;
uniform Light lights[4];
uniform vec3 Color;
uniform float shininess;

void main()
{
    // vec3 color = texture(diffuseTexture, TexCoords).rgb;
    vec3 color = Color;
    vec3 normal = normalize(Normal);
    // ambient
    vec3 ambient = 0.0 * color;
    // lighting
    vec3 lighting = vec3(0.0);
    vec3 viewDir = normalize(viewPos - FragPos);
    for(int i = 0; i < 4; i++)
    {
        // ambient 
        ambient = lights[i].ambient * color;
        // diffuse
        vec3 lightDir = normalize(lights[i].Position - FragPos);
        float diff = max(dot(lightDir, normal), 0.0);
        vec3 diffuse = lights[i].Color * diff * color;    
        // specular
        vec3 viewDir = normalize(viewPos - FragPos);
        vec3 reflectDir = reflect(-lightDir, normal);  
        float spec = pow(max(dot(viewDir, reflectDir), 0.0), shininess);
        vec3 specular = lights[i].specular * spec * color; 
        // attenuation (use quadratic as we have gamma correction)
        float distance = length(FragPos - lights[i].Position);
        vec3 result = ambient + diffuse + specular;
        result *= 1.0 / (distance * distance);
        lighting += result;
                
    }
    vec3 result = ambient + lighting;

     // check whether result is higher than some threshold, if so, output as bloom threshold color
    float brightness = dot(result, vec3(0.2126, 0.7152, 0.0722));
    if(brightness > 1.0)
        BrightColor = vec4(result, 1.0);
    else
        BrightColor = vec4(0.0, 0.0, 0.0, 1.0);
    // result = vec3(1.0f, 1.0f, 1.0f);
    FragColor = vec4(result, 1.0);
} 