#version 410

in vec4 fragPosition;
in vec4 fragNormal;
in vec2 fragTexCoord;
in vec4 fragTangent;
in vec4 fragBiTangent;

out vec4 outColor;

uniform vec3 lightDir;
uniform vec3 lightColor;
uniform vec3 ambientLight;
uniform vec3 materialColor;
uniform float specularPower;
uniform vec3 eyePos;

uniform sampler2D diffTexture;
uniform sampler2D normTexture;

void main()
{
	//vec3 ambient = materialColor * ambientLight;
	
	//vec3 N = normalize(fragNormal.xyz);
	//vec3 L = normalize(lightDir);

	//float d = max(0.0f, dot(N, -L));
	//vec3 diffuse = vec3(d) * lightColor * materialColor;

	mat3 TBN = mat3(normalize(fragTangent), normalize(fragBiTangent), normalize(fragNormal));

	vec3 sampledNormal = texture(normTexture, fragTexCoord).xyz;
	vec3 adjustedNormal = sampledNormal * 2 - 1;
	
	vec3 materialColor = texture(diffTexture, fragTexCoord).xyz;
	
	vec3 ambient = materialColor * ambientLight;
	
	vec3 N = normalize(TBN * adjustedNormal);
	vec3 L = normalize(lightDir);

	float d = max(0.0f, dot(N, -L));
	vec3 diffuse = vec3(d) * lightColor * materialColor;
	
	vec3 E = normalize(eyePos - fragPosition.xyz);
	vec3 R = reflect(L, N);
	float s = max(0, dot(R, E));
	s = pow(s, specularPower);
	vec3 specular = vec3(s) * lightColor * materialColor;

	outColor = texture(diffTexture, fragTexCoord) * vec4(ambient + diffuse + specular, 1);
}