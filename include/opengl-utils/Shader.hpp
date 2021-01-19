#ifndef GLSL_SHADER_H
#define GLSL_SHADER_H

#include <glad/glad.h>

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>

class Shader
{
	public:
		unsigned int ID;

		//constructor
		Shader(char const * vertexPath, char const * fragmentPath);

		//bind shaderprogram
		void use();

		//functions required to manipulate shader uniforms
		void setBool(std::string const &name, bool value) const;
		void setInt(const std::string &name, int value) const;
		void setFloat(const std::string &name, float value) const;
	private:
		bool compileShader(unsigned int shader, char const * name);
};

#endif
