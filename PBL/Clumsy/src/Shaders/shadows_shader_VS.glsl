#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoords;
layout (location = 3) in ivec4 bone_ids;     // INT pointer
layout (location = 4) in vec4 weights;
layout (location = 5) in mat4 instanceMatrix;

out vec3 FragPos;
out vec3 Normal;
out vec2 TexCoords;
out vec4 FragPosLightSpace;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
uniform bool hasBones;
uniform bool instanced;

uniform mat4 lightSpaceMatrix;

const int MAX_BONES = 100;
uniform mat4 bones[MAX_BONES];

void main()
{
    mat4 bone_transform = bones[bone_ids[0]] * weights[0];
		bone_transform += bones[bone_ids[1]] * weights[1];
		bone_transform += bones[bone_ids[2]] * weights[2];
		bone_transform += bones[bone_ids[3]] * weights[3];
	
   
	vec4 boned_position = bone_transform * vec4(aPos, 1.0); // transformed by bones
    Normal = mat3(transpose(inverse(model))) * aNormal;  
    TexCoords = aTexCoords;
    
	if(hasBones)
	{
		FragPos = vec3(model * boned_position);
		gl_Position = projection * view * model * boned_position;
		FragPosLightSpace = lightSpaceMatrix * vec4(FragPos, 1.0);
	}
	else if(instanced == false) 
	{
		FragPos = vec3(model * vec4(aPos, 1.0));
		gl_Position = projection * view * model * vec4(FragPos, 1.0);
		FragPosLightSpace = lightSpaceMatrix * vec4(FragPos, 1.0);
	}
	else
	{
		FragPos = vec3(instanceMatrix * vec4(aPos, 1.0));
		gl_Position = projection * view * instanceMatrix * vec4(aPos, 1.0);
		FragPosLightSpace = lightSpaceMatrix * vec4(FragPos, 1.0);
	}
}