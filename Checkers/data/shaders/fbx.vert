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
uniform mat4 world;

const int MAX_BONES = 128;
uniform mat4 bones[MAX_BONES];

void main() 
{
	fragPosition = Position;
	fragNormal = Normal;
	fragTangent = Tangent;
	fragBiTangent = vec4(cross(Normal.xyz, Tangent.xyz), 0);
	fragTexCoord = TexCoord;

	// cast the indices to integer's so they can index an array
	ivec4 index = ivec4(Indices);
	// sample bones and blend up to 4
	vec4 P = bones[ index.x ] * Position * Weights.x;
	P += bones[ index.y ] * Position * Weights.y;
	P += bones[ index.z ] * Position * Weights.z;
	P += bones[ index.w ] * Position * Weights.w;

	gl_Position = projectionView * world * Position; 
}