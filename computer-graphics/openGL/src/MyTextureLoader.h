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

class MyTextureLoader
{
public:

	// constructor 
	MyTextureLoader();

	// lade Texture liefert bits
	bool load(const char*);

	// liefere Daten
	BYTE* getBits();

	// liefere Höhe
	int getH();

	// liefere Breite
	int getW();

	//Speicher bereinigen
	void del();

private:

	BYTE* bits;
	BYTE* pixels;

	FIBITMAP* dib;
};

