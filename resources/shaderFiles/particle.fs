#version 330 core
out vec4 FragColor;

struct Light {
    vec3 position;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;

    float constant;
    float linear;
    float quadratic;
};

in vec3 FragPos;  
in vec3 Normal;  
in vec2 TexCoord;

uniform vec4 particleColor;

uniform sampler2D particleTexture;
uniform vec3 viewPos;
uniform Light light;

uniform int hasLight;

void main()
{
    if (hasLight == 1){
	// ambient
    vec3 ambient = light.ambient;
  	
    // diffuse 
    vec3 norm = normalize(Normal);
    vec3 lightDir = normalize(light.position - FragPos);
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = diff * light.diffuse;
    
    // specular
    vec3 viewDir = normalize(viewPos - FragPos);
    vec3 reflectDir = reflect(-lightDir, norm);  
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32);
    vec3 specular = spec * light.specular;
    float distance    = length(light.position - FragPos);
    float attenuation = 1.0 / (light.constant + light.linear * distance +  light.quadratic * (distance * distance));

    vec3 result = ambient + diffuse + specular;
    //result *= attenuation;
    FragColor = vec4(result, 1.0);
    // FragColor = texture(particleTexture, TexCoord) * particleColor;
    }
    else {
	FragColor = texture(particleTexture, TexCoord) * particleColor;
}
    
} 