#pragma once

constexpr int numVertices = 36;

//berechne Texturkoordinaten f�r Kegel
void calcConeTexCoords(float h, float r, float vertices[][2]);


//berechne alle Koordinaten f�r Kegel
void generateCone(float height = 1.0f, float radius = 0.5f);

//binde Textur und zeichne Kegel
void drawCone(int texID);

