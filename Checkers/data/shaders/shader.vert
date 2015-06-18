#version 410

layout (location = 0) in vec4 Position;
layout (location = 1) in vec4 Normal;
layout (location = 2) in vec4 Tangent;
layout (location = 3) in vec2 TexCoord;
layout (location = 4) in vec4 Weights;
layout (location = 5) in vec4 Indices;

out vec4 fragPosition;
out vec4 fragNormal;
out vec2 fragTexCoord;
out vec4 fragTangent;
out vec4 fragBiTangent;

uniform mat4 projectionView;

void main()
{
	fragPosition = Position;
	fragNormal = Normal;
	fragTexCoord = TexCoord;
	fragTangent = Tangent;
	fragBiTangent = vec4(cross(Normal.xyz, Tangent.xyz), 1);

	gl_Position = projectionView * Position;

}
