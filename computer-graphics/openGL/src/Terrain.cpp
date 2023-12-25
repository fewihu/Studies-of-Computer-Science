#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <fstream>
#include <vector>
#include <glm.hpp>
#include <gtx/transform.hpp>
#include <glew.h>
#include <freeglut.h>
#include <FreeImage.h>
#include <iostream>
#include "Terrain.h"
#include "Data.h"


#define TER_OFF 0.5
#define TER_NOR 10.5

//Terrainkoordinaten
GLfloat terrain[] = {
    (-10.0f + TER_OFF) / TER_NOR,   (-10.0f + TER_OFF) / TER_NOR,   (-9.75f + TER_OFF) / TER_NOR,
    (0.0f), (0.0f),
    (0.0f + TER_OFF) / TER_NOR,     (-10.0f + TER_OFF) / TER_NOR,   (-9.0f + TER_OFF) / TER_NOR,
    (1.0f), (0.0f),
    (-10.0f + TER_OFF) / TER_NOR,   (0.0f + TER_OFF) / TER_NOR,     (-9.0f + TER_OFF) / TER_NOR,
    (1.0f), (1.0f),
    //==============================================================================
    (-10.0f + TER_OFF) / TER_NOR,   (0.0f + TER_OFF) / TER_NOR, (-9.0f + TER_OFF) / TER_NOR,
    (0.0f), (0.0f),
    (0.0f + TER_OFF) / TER_NOR,   (-10.0f + TER_OFF) / TER_NOR, (-9.0f + TER_OFF) / TER_NOR,
    (1.0f), (0.0f),
    (0.0f + TER_OFF) / TER_NOR,   (0.0f + TER_OFF) / TER_NOR, (-8.5f + TER_OFF) / TER_NOR,
    (1.0f), (1.0f),
    //==============================================================================
    (0.0f + TER_OFF) / TER_NOR, (0.0f + TER_OFF) / TER_NOR, (-8.5f + TER_OFF) / TER_NOR,
    (1.0f), (0.0f),
    (0.0f + TER_OFF) / TER_NOR, (-10.0f + TER_OFF) / TER_NOR, (-9.0f + TER_OFF) / TER_NOR,
    (0.0f), (0.0f),
    (10.0f + TER_OFF) / TER_NOR, (0.0f + TER_OFF) / TER_NOR, (-9.0f + TER_OFF) / TER_NOR,
    (0.5f), (1.0f),
    //==============================================================================
    //kleiner Hügel
    (0.0f + TER_OFF) / TER_NOR, (-10.0f + TER_OFF) / TER_NOR, (-9.0f + TER_OFF) / TER_NOR,
    (0.0f), (0.0f),
    (5.0f + TER_OFF) / TER_NOR, (-10.0f + TER_OFF) / TER_NOR, (-8.5f + TER_OFF) / TER_NOR,
    (0.5f), (0.0f),
    (5.0f + TER_OFF) / TER_NOR, (-5.0f + TER_OFF) / TER_NOR, (-9.0f + TER_OFF) / TER_NOR,
    (0.5f), (0.5f),
    //
    (5.0f + TER_OFF) / TER_NOR, (-5.0f + TER_OFF) / TER_NOR, (-9.0f + TER_OFF) / TER_NOR,
    (0.0f), (0.0f),
    (10.0f + TER_OFF) / TER_NOR, (-5.0f + TER_OFF) / TER_NOR, (-8.5f + TER_OFF) / TER_NOR,
    (0.5f), (0.0f),
    (10.0f + TER_OFF) / TER_NOR, (0.0f + TER_OFF) / TER_NOR, (-9.0f + TER_OFF) / TER_NOR,
    (0.5f), (0.5f),
    //
    (5.0f + TER_OFF) / TER_NOR, (-5.0f + TER_OFF) / TER_NOR, (-9.0f + TER_OFF) / TER_NOR,
    (0.5f), (0.0f),
    (5.0f + TER_OFF) / TER_NOR, (-10.0f + TER_OFF) / TER_NOR, (-8.5f + TER_OFF) / TER_NOR,
    (0.0f), (0.0f),
    (10.0f + TER_OFF) / TER_NOR, (-10.0f + TER_OFF) / TER_NOR, (-7.75f + TER_OFF) / TER_NOR,
    (0.5f), (0.5f),
    //
    (10.0f + TER_OFF) / TER_NOR, (-5.0f + TER_OFF) / TER_NOR, (-8.5f + TER_OFF) / TER_NOR,
    (0.0f), (0.0f),
    (5.0f + TER_OFF) / TER_NOR, (-5.0f + TER_OFF) / TER_NOR, (-9.0f + TER_OFF) / TER_NOR,
    (0.5f), (0.0f),
    (10.0f + TER_OFF) / TER_NOR, (-10.0f + TER_OFF) / TER_NOR, (-7.75f + TER_OFF) / TER_NOR,
    (0.5f), (0.5f),
    //
    //==============================================================================
    (-10.0f + TER_OFF) / TER_NOR, (0.0f + TER_OFF) / TER_NOR, (-9.0f + TER_OFF) / TER_NOR,
    (0.0f), (0.0f),
    (0.0f + TER_OFF) / TER_NOR, (0.0f + TER_OFF) / TER_NOR, (-8.5f + TER_OFF) / TER_NOR,
    (1.0f), (0.0f),
    (0.0f + TER_OFF) / TER_NOR, (10.0f + TER_OFF) / TER_NOR, (-9.0f + TER_OFF) / TER_NOR,
    (1.0f), (1.0f),
    //==============================================================================
    (-10.f + TER_OFF) / TER_NOR, (10.0f + TER_OFF) / TER_NOR, (-9.0f + TER_OFF) / TER_NOR,
    (0.0f), (0.0f),
    (-10.0f + TER_OFF) / TER_NOR, (0.0f + TER_OFF) / TER_NOR, (-9.0f + TER_OFF) / TER_NOR,
    (1.0f), (0.0f),
    (0.0f + TER_OFF) / TER_NOR, (10.0f + TER_OFF) / TER_NOR, (-9.0f + TER_OFF) / TER_NOR,
    (1.0f), (1.0f),
    //==============================================================================
    (0.0f + TER_OFF) / TER_NOR, (0.0f + TER_OFF) / TER_NOR, (-8.5f + TER_OFF) / TER_NOR,
    (0.0f), (0.0f),
    (10.0f + TER_OFF) / TER_NOR, (0.0f + TER_OFF) / TER_NOR, (-9.0f + TER_OFF) / TER_NOR,
    (1.0f), (0.0f),
    (0.0f + TER_OFF) / TER_NOR, (10.0f + TER_OFF) / TER_NOR, (-9.0f + TER_OFF) / TER_NOR,
    (1.0f), (1.0f),
    //==============================================================================
    (0.0f + TER_OFF) / TER_NOR, (10.0f + TER_OFF) / TER_NOR, (-9.0f + TER_OFF) / TER_NOR,
    (0.0f), (0.0f),
    (10.0f + TER_OFF) / TER_NOR, (0.0f + TER_OFF) / TER_NOR,(-9.0f + TER_OFF) / TER_NOR,
    (1.0f), (0.0f),
    (10.0f + TER_OFF) / TER_NOR, (10.0f + TER_OFF) / TER_NOR,(-8.75f + TER_OFF) / TER_NOR,
    (1.0f), (1.0f),
};

void generateTerrain() {

    //Normalen berechnen
    float* normals = (float*)malloc(sizeof(float) * 99);
    calcTerrainNormals(&terrain[0], 11, normals);
    
    //Vertexe einfügen
    glBindVertexArray(VAO[0]);
    glBindBuffer(GL_ARRAY_BUFFER, Buffer[0]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(terrain) + (99 * sizeof(float)), terrain, GL_STATIC_DRAW);
    glBufferSubData(GL_ARRAY_BUFFER, sizeof(terrain), 99 * sizeof(float), normals);
    //setze Attributpointer (Position,0)
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
    //setze Attributpointer (Texturkoordinate, 1)
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
    //setze Attributpointer (Normalen, 2)
    glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 0, (void*)sizeof(terrain));
    
    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);
    glEnableVertexAttribArray(2);
}

void calcTerrainNormals(float* array, int size, float* normals) {

    for (int i = 0; i < size; i ++) {
 
        int o = i * 15;

        glm::vec3 v0 = glm::vec3(*(array + o + 0), *(array + o + 1), *(array + o + 2));
        glm::vec3 v1 = glm::vec3(*(array + o + 5), *(array + o + 6), *(array + o + 7));
        glm::vec3 v2 = glm::vec3(*(array + o +10), *(array + o +11), *(array + o +12));

        glm::vec3 n = glm::normalize(glm::cross((v1 - v0), (v2 - v0)));

        //DEBUG
        //printf("%f %f %f\n", v0.x, v0.y, v0.z);
        //printf("%f %f %f\n", v1.x, v1.y, v1.z);
        //printf("%f %f %f\n", v2.x, v2.y, v2.z);
        //printf("%f %f %f\n", n.x, n.y, n.z );
        //printf("------------------------------\n");

        int u = i * 9;
        for (int j = 0; j < 9; j += 3) {
            //printf("%d %d %d \n", u + j + 0, u + j + 1, u + j + 2);
            *(normals + u + j + 0) = n.x;
            *(normals + u + j + 1) = n.y;
            *(normals + u + j + 2) = n.z;
        }
    }
}

void drawTerrain(GLuint texID) {
    //Texturen zulassen
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, texID);
    glBindVertexArray(VAO[0]);
    glVertexAttrib3f(GLuint(1), 1.0f, 1.0f, 1.0f);
    glDrawArrays(GL_TRIANGLES, 0, 33);
}