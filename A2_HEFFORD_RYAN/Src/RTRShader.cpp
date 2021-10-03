//-----------------------------------------------------------------------------
// RTRShader.cpp
//-----------------------------------------------------------------------------
// RMIT University, COSC1226: Real-Time Rendering and 3D Game Programming
//-----------------------------------------------------------------------------
#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <iostream>
#include "RTRShader.h"
#include <fstream>
#include <sstream>

static char *load_shader_file(const char *filename)
{
    FILE *fp = fopen(filename, "r");
    if (fp == nullptr) return nullptr;
    fseek(fp, 0L, SEEK_END);
    size_t size = ftell(fp);
    fseek(fp, 0L, SEEK_SET);
    char *buf = new char[size + 1];
    fread(buf, 1, size, fp);
    buf[size] = 0;
    fclose(fp);
    return buf;
}

int RTRShader::Load(const char *vert_path, const char *frag_path, const char *geom_path)
{
	//getting source files
	std::string vertCode;
	std::string fragCode;
	std::string geomCode;
	std::ifstream vsFile;
	std::ifstream fsFile;
	std::ifstream gsFile;
	//error handling
	vsFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
	fsFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
	gsFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
	try
	{
		//open files
		vsFile.open(vert_path);
		fsFile.open(frag_path);
		if (geom_path != nullptr) { gsFile.open(geom_path); };
		std::stringstream vsStream, fsStream, gsStream;

		//read into streams
		vsStream << vsFile.rdbuf();
		fsStream << fsFile.rdbuf();
		if (geom_path != nullptr) { gsStream << gsFile.rdbuf(); };
		//close files
		vsFile.close();
		fsFile.close();
		if (geom_path != nullptr) { gsFile.close(); };

		//Convert stream into string
		vertCode = vsStream.str();
		fragCode = fsStream.str();
		if (geom_path != nullptr) { geomCode = gsStream.str(); };
	}
	catch (std::ifstream::failure e)
	{
		std::cout << "ERROR::SHADER::FILE_NOT_SUCCESFULLY_READ" << std::endl;
	}
	const char* vShaderCode = vertCode.c_str();
	const char* fShaderCode = fragCode.c_str();
	const char* gShaderCode = geomCode.c_str();

	//compile shaders
	unsigned int vertex, fragment, geometry;
	int success;
	char infoLog[512];
	//vertex Shader
	vertex = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertex, 1, &vShaderCode, nullptr);
	glCompileShader(vertex);

	//print errors if need
	glGetShaderiv(vertex, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(vertex, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" <<
			infoLog << std::endl;
	};



	//same for fs
	fragment = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragment, 1, &fShaderCode, nullptr);
	glCompileShader(fragment);

	glGetShaderiv(fragment, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(fragment, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" <<
			infoLog << std::endl;
	};

	if (gShaderCode != nullptr) {
		//and geom shader
		geometry = glCreateShader(GL_GEOMETRY_SHADER);
		glShaderSource(geometry, 1, &gShaderCode, nullptr);
		glCompileShader(geometry);

		glGetShaderiv(geometry, GL_COMPILE_STATUS, &success);
		if (!success)
		{
			glGetShaderInfoLog(geometry, 512, NULL, infoLog);
			std::cout << "ERROR::SHADER::GEOMETRY::COMPILATION_FAILED\n" <<
				infoLog << std::endl;
		};
	}

	//shader Program
	m_Id = glCreateProgram();
	glAttachShader(m_Id, vertex);
	if (geom_path != nullptr) { glAttachShader(m_Id, geometry); };
	glAttachShader(m_Id, fragment);
	glLinkProgram(m_Id);


	//linking errors
	glGetProgramiv(m_Id, GL_LINK_STATUS, &success);
	if (!success)
	{
		glGetProgramInfoLog(m_Id, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" <<
			infoLog << std::endl;
	}


	//delete all shaders
	glDeleteShader(vertex);
	glDeleteShader(fragment);
	glDeleteShader(geometry);

	return success == -1;
}
