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
#include "MyShader.h"

class MyShaderLoader
{
public:

	//link Shaders together to create program
	MyShaderLoader(GLuint VSID, GLuint FSID, GLuint GSID);

	GLuint getID();

	bool getState();

private:

	GLuint ID;

	bool state;

};

