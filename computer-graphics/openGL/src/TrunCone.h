#pragma once

//berechne alle Koordinaten für einen Kegelstumpf
void generateTrunCone(float, float);

//binde Textur und zeichne Kegelstumpf
void drawTrunCone(GLuint);

//berechne Normalen
void calcTrunConeNormals(float*, int, float*);