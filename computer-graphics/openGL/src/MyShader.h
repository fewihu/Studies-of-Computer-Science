#pragma once
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <vector>
#include <glm.hpp>
#include <gtx/transform.hpp>
#include <glew.h>
#include <freeglut.h>
#include <FreeImage.h>

class MyShader
{
public:
	
	// constructor reads and builds the shader
	MyShader(const char* FilePath, GLenum Type);
	
	// use/activate the shader
	int getID();

	bool getState();

	void del();

private:

	// the program ID
	GLuint ID;

	//ready or not
	bool state;
};

