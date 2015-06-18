#version 410

in vec4 fragPosition;
in vec4 fragNormal;
in vec2 fragTexCoord;

out vec4 outColor;

uniform vec3 lightDir;
uniform vec3 lightColor;
uniform vec3 ambientLight;
uniform vec3 materialColor;
uniform float specularPower;
uniform vec3 eyePos;

uniform sampler2D diffText;


void main()
{
	vec3 ambient = materialColor * ambientLight;
	
	vec3 N = normalize(fragNormal.xyz);
	vec3 L = normalize(lightDir);

	float d = max(0.0f, dot(N, -L));
	vec3 diffuse = vec3(d) * lightColor * materialColor;

	vec3 E = normalize(eyePos - fragPosition.xyz);
	vec3 R = reflect(L, N);
	float s = max(0, dot(R, E));
	s = pow(s, specularPower);
	vec3 specular = vec3(s) * lightColor * materialColor;
	
	outColor = texture(diffText, fragTexCoord) * vec4(ambient + diffuse + specular, 1);
}