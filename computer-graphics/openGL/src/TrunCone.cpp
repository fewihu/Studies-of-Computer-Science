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
#include "MyShader.h"
#include "MyShaderLoader.h"
#include "MyTextureLoader.h"
#include "Terrain.h"
#include "Data.h"
#include "TrunCone.h"

void generateTrunCone(float r1, float r2){

    int circSegments = 10;
    GLfloat vertices[300];
    GLfloat normals[60];

    float degOffset = ((2 * glm::pi<float>()) / (float)circSegments);  //Offset zum letzten Vertex

    float x1 = 0;      float x2 = 0;
    float y1 = 0;       float y2 = 0;
    float z1 = -0.78;   float z2 = -0.2;

    for (int i = 0; i < circSegments; i++) {

        float deg = degOffset * i;

        glm::vec2 unten1 = glm::vec2(x1 + glm::sin(deg) * r1, y1 + glm::cos(deg) * r1);
        glm::vec2 unten2 = glm::vec2(x1 + glm::sin(deg + degOffset) * r1, y1 + glm::cos(deg + degOffset) * r1);

        glm::vec2 oben1  = glm::vec2(x2 + glm::sin(deg) * r2, y2 + glm::cos(deg) * r2);
        glm::vec2 oben2  = glm::vec2(x2 + glm::sin(deg + degOffset) * r2, y2 + glm::cos(deg + degOffset) * r2);

        int stride = 30 * i;

        //DEBUG
        /*printf("%f %f %f\n", unten1.x, unten1.y, z1); printf("%f %f %f\n", unten2.x, unten2.y, z1);
        printf("%f %f %f\n", oben1.x, oben1.y, z2);     printf("---------------------------------\n");
        printf("%f %f %f\n", oben1.x, oben1.y, z2);     printf("%f %f %f\n", oben2.x, oben2.y, z2);
        printf("%f %f %f\n", unten2.x, unten2.y, z1);   printf("---------------------------------\n");*/

        vertices[0 + stride] = unten1.x; vertices[5 + stride] = unten2.x; vertices[10 + stride] = oben1.x;
        vertices[1 + stride] = unten1.y; vertices[6 + stride] = unten2.y; vertices[11 + stride] = oben1.y;
        vertices[2 + stride] = z1;       vertices[7 + stride] = z1;       vertices[12 + stride] = z2;
        //Textur    
        vertices[3 + stride] = i*0.1;    vertices[8 + stride] = (i+1)*0.1;vertices[13 + stride] = i*0.1;
        vertices[4 + stride] = 0;        vertices[9 + stride] = 0;        vertices[14 + stride] = 1;

        vertices[15 + stride] = oben1.x;  vertices[20 + stride] = oben2.x;  vertices[25 + stride] = unten2.x;
        vertices[16 + stride] = oben1.y;  vertices[21 + stride] = oben2.y;  vertices[26 + stride] = unten2.y;
        vertices[17 + stride] = z2;       vertices[22 + stride] = z2;       vertices[27 + stride] = z1;
        //Textur    
        vertices[18 + stride] = i*0.1;    vertices[23 + stride] = (i+1)*0.1;vertices[28 + stride] = (i+1)*0.1;
        vertices[19 + stride] = 1;        vertices[24 + stride] = 1;        vertices[29 + stride] = 0;
    
        int stride2 = i * 6;

        glm::vec3 v0 = glm::vec3(unten2.x, unten2.y, z1);
        glm::vec3 v1 = glm::vec3(unten1.x, unten1.y, z1);
        glm::vec3 v2 = glm::vec3(oben1.x, oben1.y, z2);
        glm::vec3 n = glm::normalize(glm::cross((v1 - v0), (v2 - v0)));
        normals[stride2 + 0] = -n.x; normals[stride2 + 1] = -n.y; normals[stride2 + 2] = n.z;
        
        v0 = glm::vec3(oben1.x, oben1.y, z2);
        v1 = glm::vec3(oben2.x, oben2.y, z2);
        v2 = glm::vec3(unten2.x, unten2.y, z1);
        n = glm::normalize(glm::cross((v1 - v0), (v2 - v0)));
        normals[stride2 + 3] = -n.x; normals[stride2 + 4] = -n.y; normals[stride2 + 5] = n.z;
    }
    
    glBindVertexArray(VAO[1]);
    glBindBuffer(GL_ARRAY_BUFFER, Buffer[1]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices) + sizeof(normals), vertices, GL_STATIC_DRAW);
    glBufferSubData(GL_ARRAY_BUFFER, sizeof(vertices), sizeof(normals), normals);
    //setze Attributpointer (Position,0 / Texturkoordinaten,1 / Normalen,2)
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
    glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 0, (void*)sizeof(vertices));

    glEnableVertexAttribArray(0); glEnableVertexAttribArray(1); glEnableVertexAttribArray(2);
}

void drawTrunCone(GLuint texID) {
   
    //glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, texID);
    glBindVertexArray(VAO[1]);
    glVertexAttrib3f(GLuint(1), 1.0f, 1.0f, 1.0f);
    glDrawArrays(GL_TRIANGLES, 0, 60);
}

void calcTrunConeNormals(float* array, int size, float* normals) {

    int l = 0;
    for (; l < 26; l++) {
        int o = l * 3;
        *(normals + o + 0) = 1;
        *(normals + o + 1) = 1;
        *(normals + o + 2) = 1;
    }

    for (int i = l; i < 86; i++) {
        int u = l * 3;
        *(normals + u + 0) = 0.5;
        *(normals + u + 1) = 0.5;
        *(normals + u + 2) = 0;
    }

}