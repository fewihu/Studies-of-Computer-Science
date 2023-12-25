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
#include "Cone.h"
#include "Data.h"

float M_PI = glm::pi<float>();

void generateCone(float height, float radius) {
	
	GLfloat vertices[numVertices * 6][6];

	//Spitze und Mittelpunkt
	float z = -0.5f + height;
	for (int i = 0; i < numVertices * 6; i += 3) {
		
		if (i == numVertices * 3) z = -0.5f;
		
		vertices[i][0] = 0.0f;
		vertices[i][1] = 0.0f;
		vertices[i][2] = z;
	}

	for (int deg = 0; deg < 360; deg += 360 / numVertices) {
		
		int i = deg / (360 / numVertices) * 3 + 1;
		
		float x = sin((float)deg * M_PI / 180.0) * radius;
		float y = cos((float)deg * M_PI / 180.0) * radius;
		
		vertices[numVertices * 3 + i][0] = vertices[i][0] = x;
		vertices[numVertices * 3 + i][1] = vertices[i][1] = y;
		vertices[numVertices * 3 + i][2] = vertices[i][2] = -0.5f;
		
		if (i - 2 >= 0) i = i - 2;
		else i = numVertices * 3 - 1;
		
		vertices[numVertices * 3 + i][0] = vertices[i][0] = x;
		vertices[numVertices * 3 + i][1] = vertices[i][1] = y;
		vertices[numVertices * 3 + i][2] = vertices[i][2] = -0.5f;
	}

	float coneTexVertices[numVertices * 6][2];
	calcConeTexCoords(height, radius, coneTexVertices);

	float sign = 1.0f;
	//Normalvektoren
	for (int i = 0; i < numVertices * 6; i += 3) {
		
		if (i == numVertices * 3) sign = -1.0f;
		
		glm::vec3 normalVector = cross(glm::vec3(vertices[i + 2][0], vertices[i + 2][1], vertices[i + 2][2]) - glm::vec3(vertices[i][0], vertices[i][1], vertices[i][2]),
			glm::vec3(vertices[i + 1][0], vertices[i + 1][1], vertices[i + 1][2]) - glm::vec3(vertices[i][0], vertices[i][1], vertices[i][2]));
		
		for (int j = i; j < i + 3; j++) {
			vertices[j][3] = normalVector[0];
			vertices[j][4] = normalVector[1];
			vertices[j][5] = sign * normalVector[2]; //Grundfläche
		}
	}

	//Vertexe einfügen
	glBindVertexArray(VAO[2]);
	glBindBuffer(GL_ARRAY_BUFFER, Buffer[2]);	
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices) + sizeof(coneTexVertices), vertices, GL_STATIC_DRAW);
	glBufferSubData(GL_ARRAY_BUFFER, sizeof(vertices), sizeof(coneTexVertices), coneTexVertices);
	
	//setze Attributpointer (Position, 0)
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), NULL);
	glEnableVertexAttribArray(0);
	
	//setze Attributpointer (Texturkoordinate, 1)
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, (void*)sizeof(vertices));
	glEnableVertexAttribArray(1);

	//setze Attributpointer (Normalen, 2)
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(2);
}

void drawCone(int texID) {
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, texID);
	glBindVertexArray(VAO[2]);
	glDrawArrays(GL_TRIANGLES, 0, numVertices * 6);
}

void calcConeTexCoords(float h, float r, float vertices[][2]) {

	int n = numVertices;

	float alpha = 2 * M_PI / (float)n;
	float s = sqrt(r * r + h * h);
	float a = sqrt((1 - cos(alpha)) * 2 * r * r);
	float gamma = acos(1 - (a * a / (2 * s * s)));

	float factor = 1 / (2 * s);
	r *= factor;

	for (int i = 0; i <= n; i++) { //Mantelfäche
		
		float deg1 = gamma * (float)i;
		float x1 = 0.5 + sin(deg1) * 0.5f;
		float y1 = 0.5 + cos(deg1) * 0.5f;

		float deg2 = alpha * (float)i;
		float x2 = 0.5 + sin(deg2) * r;
		float y2 = 0.5 + cos(deg2) * r;

		if (i * 3 < n * 3) {
			vertices[i * 3 + 1][0] = x1;
			vertices[i * 3 + 1][1] = y1;
			// Mittelpunkt
			vertices[i * 3][0] = 0.5;
			vertices[i * 3][1] = 0.5;

			vertices[i * 3 + 3 * n + 1][0] = x2;
			vertices[i * 3 + 3 * n + 1][1] = y2;
			//Mittelpunkt
			vertices[i * 3 + 3 * n][0] = 0.5;
			vertices[i * 3 + 3 * n][1] = 0.5;
		}
		if (i * 3 >= 0) {
			vertices[i * 3 - 1][0] = x1;
			vertices[i * 3 - 1][1] = y1;

			vertices[i * 3 + 3 * n - 1][0] = x2;
			vertices[i * 3 + 3 * n - 1][1] = y2;
		}
	}
}