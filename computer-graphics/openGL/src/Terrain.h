#pragma once

//erzeuge alle Koordinaten für das Terrain
void generateTerrain();

//binde Textur und zeichne Terrain
void drawTerrain(GLuint);

//berechne Normalen
void calcTerrainNormals(float*, int, float*);