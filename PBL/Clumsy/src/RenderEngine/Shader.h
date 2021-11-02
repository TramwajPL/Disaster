#pragma once
#include <iostream>
#include <glm/glm.hpp>
#include <glad/glad.h>
#include <glm\ext\matrix_clip_space.hpp>
#include <glm\ext\matrix_transform.hpp>



namespace Clumsy 
{
    class DirectionalLight;
    class Shader
    {
    public:
        unsigned int ID;
        Shader(const char* vertexPath, const char* fragmentPath);
		Shader() {}
        void use();
        void setBool(const std::string& name, bool value) const;
        void setInt(const std::string& name, int value) const;
        void SetInteger(const GLchar* name, GLint value, GLboolean useShader);
        void setFloat(const std::string& name, float value) const;
        void setMat4(const std::string& name, const glm::mat4& mat) const;
		void setVec3(const std::string& name, const glm::vec3& value) const;
		void setVec3(const std::string& name, float x, float y, float z) const;
		void setDirectional(const std::string& uniformName, DirectionalLight* directionalLight) const;

        
        void SetDirectionalLight(float intensity, glm::vec3 viewPosition, glm::vec3 lightPos, glm::mat4 lightSpaceMatrix);
       
        void SetSpotLight(glm::vec3 position, glm::vec3 direction, glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 specular);
        
        void SetPointLight(std::string number ,glm::vec3 position, glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 specular);

		void setVec2(const std::string& name, const glm::vec2& value) const;
		void setVec2(const std::string& name, float x, float y) const;
		void setVec4(const std::string& name, const glm::vec4& value) const;
		void setVec4(const std::string& name, float x, float y, float z, float w);

    private:
        void checkCompileErrors(unsigned int shader, std::string type);
    };
}