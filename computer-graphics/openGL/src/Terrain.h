#pragma once

//erzeuge alle Koordinaten f�r das Terrain
void generateTerrain();

//binde Textur und zeichne Terrain
void drawTerrain(GLuint);

//berechne Normalen
void calcTerrainNormals(float*, int, float*);