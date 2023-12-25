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
#include "Cone.h"

const int numTextures = 3;
GLuint texture[numTextures];

GLint heigth = 150, width = 150;
GLuint program;
const int ArrayBuffer = 3;
const int NumBuffer = 3;
GLuint Buffer[NumBuffer];
const int NumVAO = 3;
GLuint VAO[NumVAO];

//Rotation durch Nutzer
float xRot;
float yRot;

void reshape(int w, int h) {

	width = w;
	heigth = h;
	glViewport(0, 0, w, h);
}

void init(void) {

    xRot = 0.0f;
    yRot = 0.0f;

	printf("\n%s", (char*)glGetString(GL_RENDERER));
	printf("\n%s", (char*)glGetString(GL_VERSION));
	printf("\n%s\n", (char*)glGetString(GL_SHADING_LANGUAGE_VERSION));

	MyShader vs = MyShader("C:\\Users\\49173\\Documents\\CG1\\Beleg\\Shader\\Dreiecke.vs", GL_VERTEX_SHADER);
	MyShader fs = MyShader("C:\\Users\\49173\\Documents\\CG1\\Beleg\\Shader\\Dreiecke.fs", GL_FRAGMENT_SHADER);

	if (vs.getState() && fs.getState()) {

		MyShaderLoader sl = MyShaderLoader(vs.getID(), fs.getID(), 0);
		if (sl.getState()) {
			program = sl.getID();
			vs.del(); fs.del();
		}
	}
    
    glUseProgram(program);

    //Buffer und Vertexe erzeugen und hochladen
    glGenBuffers(NumBuffer,Buffer);
    glGenVertexArrays(NumVAO, VAO);
    generateTerrain(); generateTrunCone(0.13f, 0.09f); generateCone();

    //Textur (Terrain)
    glGenTextures(1,&texture[0]);
    glBindTexture(GL_TEXTURE_2D,texture[0]);
    //setze Filtering Parameter für Textur
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR); glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    //lade Textur
    MyTextureLoader tl = MyTextureLoader();
    if(tl.load("C:\\Users\\49173\\Documents\\CG1\\Beleg\\img\\bsp.png")){

        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, tl.getW(), tl.getH(), 0, GL_RGBA, GL_UNSIGNED_BYTE, tl.getBits());
        glGenerateMipmap(GL_TEXTURE_2D);
        printf("%s geladen\n", "C:\\Users\\49173\\Documents\\CG1\\Beleg\\img\\bsp.png");
    }
    else printf("%s konnte nicht geladen werden\n", "C:\\Users\\49173\\Documents\\CG1\\Beleg\\img\\bsp.png");

    //Textur (Holz)
    glGenTextures(1, &texture[1]);
    glBindTexture(GL_TEXTURE_2D, texture[1]);
    //setze wrapping Parameter für Textur 
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    //setze Filtering Parameter für Textur
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR); glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    //lade Textur
    if (tl.load("C:\\Users\\49173\\Documents\\CG1\\Beleg\\img\\Holz.png")) {

        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, tl.getW(), tl.getH(), 0, GL_RGBA, GL_UNSIGNED_BYTE, tl.getBits());
        glGenerateMipmap(GL_TEXTURE_2D);
        printf("%s geladen\n", "C:\\Users\\49173\\Documents\\CG1\\Beleg\\img\\Holz.png");
    }
    else printf("%s konnte nicht geladen werden\n", "C:\\Users\\49173\\Documents\\CG1\\Beleg\\img\\Holz.png");

    //Textur (Blätter)
    glGenTextures(1, &texture[2]);
    glBindTexture(GL_TEXTURE_2D, texture[2]);
    //setze wrapping Parameter für Textur 
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    //setze Filtering Parameter für Textur
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR); glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    //lade Textur
    if (tl.load("C:\\Users\\49173\\Documents\\CG1\\Beleg\\img\\leaves.png")) {

        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, tl.getW(), tl.getH(), 0, GL_RGBA, GL_UNSIGNED_BYTE, tl.getBits());
        glGenerateMipmap(GL_TEXTURE_2D);
        printf("%s geladen\n", "C:\\Users\\49173\\Documents\\CG1\\Beleg\\img\\leaves.png");
    }
    else printf("%s konnte nicht geladen werden\n", "C:\\Users\\49173\\Documents\\CG1\\Beleg\\img\\leaves.png");

    //Texture zulassen
    glEnable(GL_TEXTURE_2D);

    //3D Projektion
    glm::mat4 modl = glm::mat4(1.0f);
    modl = glm::rotate(modl, glm::radians(-70.0f), glm::vec3(1.0f, 0.0f, 0.0f));

    glm::mat4 view = glm::mat4(1.0f);   //Position
    view = glm::translate(view, glm::vec3(0.5f, 0.5f, -2.0f));

    glm::mat4 proj = glm::perspective(glm::radians(45.0f), 500.0f / 500.0f, 0.1f, 100.0f);

    int modlLoc = glGetUniformLocation(program, "model");
    glUniformMatrix4fv(modlLoc, 1, GL_FALSE, &modl[0][0]);

    int viewLoc = glGetUniformLocation(program, "view");
    glUniformMatrix4fv(viewLoc, 1, GL_FALSE, &view[0][0]);

    int projLoc = glGetUniformLocation(program, "projection");
    glUniformMatrix4fv(projLoc, 1, GL_FALSE, &proj[0][0]);

    //Matrizen für Lichtberechnung
    glm::mat4 modlView = view * modl;
    glm::mat4 invModlView = glm::transpose(glm::inverse(modlView));

    int modlViewLoc = glGetUniformLocation(program, "modelView");
    glUniformMatrix4fv(modlViewLoc, 1.0f, GL_FALSE, &modlView[0][0]);

    int invModlViewLoc = glGetUniformLocation(program, "invModelView");
    glUniformMatrix4fv(invModlViewLoc, 1.0f, GL_FALSE, &invModlView[0][0]);
}

float grow = 0;

void display(void) {

    //Modelmatrix als Uniform in Shader bringen
    glm::mat4 modl = glm::mat4(1.0f);
    modl = glm::rotate(modl, glm::radians(yRot), glm::vec3(1.0f, 0.0f, 0.0f));
    modl = glm::rotate(modl, glm::radians(xRot), glm::vec3(0.0f, 0.0f, 1.0f));
    int modlLoc = glGetUniformLocation(program, "model");
    glUniformMatrix4fv(modlLoc, 1, GL_FALSE, &modl[0][0]);

    //setze Hintergrund
    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    //Tiefentest (vorne überdeckt hinten)
    glEnable(GL_DEPTH_TEST);
    glClear(GL_DEPTH_BUFFER_BIT);

    //zeichne
    generateCone(0.8 + grow, 0.3 + (grow/2));
    grow += 0.0005;
    drawTerrain(texture[0]); drawTrunCone(texture[1]); drawCone(texture[2]);
    glFlush();
}

void timer(int value) {
	glutPostRedisplay();
	glutTimerFunc(50, timer, 10);
}

void special(int specKey, int mouseX, int mouseY) {
    switch (specKey) {
        case GLUT_KEY_LEFT:     
            /*if (xRot > 360)*/ xRot -= 360; 
            xRot += 0.5; break;
        case GLUT_KEY_RIGHT:
            /*if (xRot < 0)*/ xRot += 360; 
            xRot -= 0.5; break;
        case GLUT_KEY_UP:
            /*if(yRot > - 90)*/ yRot -=0.5; break;
        case GLUT_KEY_DOWN:     
            /*if(yRot < 0)*/ yRot +=0.5; break;
    }
}

int main(int argc, char** argv){

	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGBA);
	glutInitWindowSize(500, 500);
	glutInitContextVersion(4, 2);  // (4,5) (3,3);
	glutInitContextProfile(GLUT_CORE_PROFILE);  //GLUT_COMPATIBILITY_PROFILE
	glutCreateWindow("Texture Spielerei");
	glewExperimental = GL_TRUE;
	if (glewInit()) printf("Error");
	init();
	glutReshapeFunc(reshape);
	glutDisplayFunc(display);
	glutTimerFunc(1750, timer, 0);
	glViewport(0,0,500,500);
    glutSpecialFunc(special);
	glutMainLoop();
}