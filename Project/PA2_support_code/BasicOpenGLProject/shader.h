#pragma once

#include <GL/glew.h>
#include <GL/freeglut.h>
#include <string>
#include <glm/glm.hpp>
#include <glm/ext.hpp>

class Shader
{
public:
	Shader();
	Shader( std::string shaderPath, GLenum shaderType );
	~Shader();

public:
	void Create( std::string shaderPath, GLenum shaderType );
	void Delete();
	void Load();
	void setVec2(const std::string& name, float x, float y) const
	{
		glUniform2f(glGetUniformLocation(ID, name.c_str()), x, y);
	}

public:
	int GetStatus( GLenum ) const;
	int GetDeleteStatus() const;
	int GetCompileStatus() const;

	std::string GetInfoLog() const;
	std::string GetSource() const;

	GLuint      GetID()   const { return ID;   }
	GLenum      GetType() const { return Type; }
	std::string GetPath() const { return Path; }

private:
	GLuint ID;
	GLenum Type;
	std::string Path;
};
