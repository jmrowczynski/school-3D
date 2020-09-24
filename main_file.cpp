#define GLM_FORCE_RADIANS
#define GLM_FORCE_SWIZZLE

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <stdlib.h>
#include <stdio.h>
#include <iostream>
#include "constants.h"
#include "lodepng.h"
#include "shaderprogram.h"
#include "OBJ_Loader.h"
#include "globals.h"
#include "model.h"

Model locker("models/locker/", "locker.obj");
Model chair("models/chair/", "school chair.obj");
Model schoolFloor("models/floor/", "floor.obj");
Model schoolWall("models/wall/", "wall.obj");
Model schoolCeiling("models/ceiling/", "ceiling.obj");
Model person("models/person/", "Neck_Mech_Walker_by_3DHaupt-(Wavefront OBJ).obj");
Model rubbishBin("models/bin/", "bin.obj");
Model skeleton("models/skeleton/", "Skeleton.obj");

using namespace std;

void error_callback(int error, const char* description) {
	fputs(description, stderr);
}


void windowResizeCallback(GLFWwindow* window,int width,int height) {
    if (height==0) return;
    aspectRatio=(float)width/(float)height;
    glViewport(0,0,width,height);
}

void initOpenGLProgram(GLFWwindow* window) {
	glClearColor(1,1,1,1);
	glEnable(GL_DEPTH_TEST);
	glfwSetWindowSizeCallback(window,windowResizeCallback);

	sp=new ShaderProgram("v_simplest.glsl",NULL,"f_simplest.glsl");

	locker.load();
	chair.load();
	schoolFloor.load();
	schoolWall.load();
	schoolCeiling.load();
	person.load();
	rubbishBin.load();
	skeleton.load();
}


void freeOpenGLProgram(GLFWwindow* window) {

	locker.freeModel();
	chair.freeModel();
	schoolFloor.freeModel();
	schoolWall.freeModel();
	schoolCeiling.freeModel();
	person.freeModel();
	rubbishBin.freeModel();
	skeleton.freeModel();
    delete sp;
}

glm::mat4 drawFloorAndCeiling() {

	glm::mat4 M=glm::mat4(1.0f);
	glm::mat4 Mceiling=glm::mat4(1.0f);

	int xDimension = 16;
	float schoolHeight = 20.0f;
	float fragmentWidth = 10.0f;
	
	auto drawFloorAndCeiling1 = [&]() { 
        for (int i = 0; i < xDimension; i++) {
			M=glm::translate(M, glm::vec3(fragmentWidth, 0.0f, 0.0f));
			glUniformMatrix4fv(sp->u("M"),1,false,glm::value_ptr(M));
			schoolFloor.draw();
			Mceiling=glm::translate(M, glm::vec3(0.0f, schoolHeight, 0.0f));
			glUniformMatrix4fv(sp->u("M"),1,false,glm::value_ptr(Mceiling));
			schoolCeiling.draw();
		}

		M=glm::translate(M, glm::vec3(0.0f, 0.0f, fragmentWidth));
		glUniformMatrix4fv(sp->u("M"),1,false,glm::value_ptr(M));
		schoolFloor.draw();
		Mceiling=glm::translate(M, glm::vec3(0.0f, schoolHeight, 0.0f));
		glUniformMatrix4fv(sp->u("M"),1,false,glm::value_ptr(Mceiling));
		schoolCeiling.draw();
    };

	auto drawFloorAndCeiling2 = [&]() { 
        for (int i = 0; i < xDimension; i++) {
			M=glm::translate(M, glm::vec3(-fragmentWidth, 0.0f, 0.0f));
			glUniformMatrix4fv(sp->u("M"),1,false,glm::value_ptr(M));
			schoolFloor.draw();
			Mceiling=glm::translate(M, glm::vec3(0.0f, schoolHeight, 0.0f));
			glUniformMatrix4fv(sp->u("M"),1,false,glm::value_ptr(Mceiling));
			schoolCeiling.draw();
		}

		M=glm::translate(M, glm::vec3(0.0f, 0.0f, fragmentWidth));
		glUniformMatrix4fv(sp->u("M"),1,false,glm::value_ptr(M));
		schoolFloor.draw();
		Mceiling=glm::translate(M, glm::vec3(0.0f, schoolHeight, 0.0f));
		glUniformMatrix4fv(sp->u("M"),1,false,glm::value_ptr(Mceiling));
		schoolCeiling.draw();
    };

	M=glm::translate(M, glm::vec3(0.0f, 0.0f, -39.0f));
	glUniformMatrix4fv(sp->u("M"),1,false,glm::value_ptr(M));
	schoolFloor.draw();
	Mceiling=glm::translate(M, glm::vec3(0.0f, schoolHeight, 0.0f));
	glUniformMatrix4fv(sp->u("M"),1,false,glm::value_ptr(Mceiling));
	schoolCeiling.draw();

	drawFloorAndCeiling1();
	drawFloorAndCeiling2();
	drawFloorAndCeiling1();
	drawFloorAndCeiling2();
	drawFloorAndCeiling1();
	drawFloorAndCeiling2();
	drawFloorAndCeiling1();
	drawFloorAndCeiling2();
	drawFloorAndCeiling1();
	drawFloorAndCeiling2();
	drawFloorAndCeiling1();

	return M;

}

void drawWalls() {
	glm::mat4 M=glm::mat4(1.0f);
	glm::mat4 M2 = M;

	float wallSegmentWidth = 10.0f;
	float hallwayWidth = 20.0f;

	M=glm::scale(M, glm::vec3(1.0f, 2.0f, 1.0f));
	M=glm::translate(M, glm::vec3(0.0f, 5.0f, 0.0f));
	M=glm::rotate(M,glm::radians(90.0f),glm::vec3(1.0f,0.0f,0.0f));
	M2 = M;
	glUniformMatrix4fv(sp->u("M"),1,false,glm::value_ptr(M));
	schoolWall.draw();
	M2=glm::translate(M, glm::vec3(0.0f, hallwayWidth, 0.0f));
	glUniformMatrix4fv(sp->u("M"),1,false,glm::value_ptr(M2));
	schoolWall.draw();

	for (int i = 0; i < 16; i++) {
		if (i == 3) {
			M=glm::translate(M, glm::vec3(2*wallSegmentWidth-2.0f, 0.0f, 0.0f));
			glUniformMatrix4fv(sp->u("M"),1,false,glm::value_ptr(M));
			schoolWall.draw();
		} else {
			M=glm::translate(M, glm::vec3(wallSegmentWidth, 0.0f, 0.0f));
			glUniformMatrix4fv(sp->u("M"),1,false,glm::value_ptr(M));
			schoolWall.draw();
			M2=glm::translate(M, glm::vec3(0.0f, hallwayWidth, 0.0f));
			glUniformMatrix4fv(sp->u("M"),1,false,glm::value_ptr(M2));
			schoolWall.draw();
		}
	}

}

glm::mat4 drawClass(glm::mat4 M) {

	glm::mat4 rubbishBinM = glm::mat4(1.0f);
	glm::mat4 skeletonM = glm::mat4(1.0f);

	glUniformMatrix4fv(sp->u("M"),1,false,glm::value_ptr(M));
	chair.draw();

	M=glm::translate(M, glm::vec3(8.0f, 0.0f, 0.0f));
	glUniformMatrix4fv(sp->u("M"),1,false,glm::value_ptr(M));
	chair.draw();

	M=glm::translate(M, glm::vec3(8.0f, 0.0f, 0.0f));
	glUniformMatrix4fv(sp->u("M"),1,false,glm::value_ptr(M));
	chair.draw();

	M=glm::translate(M, glm::vec3(8.0f, 0.0f, 0.0f));
	glUniformMatrix4fv(sp->u("M"),1,false,glm::value_ptr(M));
	chair.draw();

	rubbishBinM = glm::translate(M, glm::vec3(8.0f, -2.0f, 14.0f));
	skeletonM = glm::rotate(rubbishBinM, glm::radians(180.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	rubbishBinM = glm::scale(rubbishBinM, glm::vec3(0.2f, 0.2f, 0.2f));
	glUniformMatrix4fv(sp->u("M"),1,false,glm::value_ptr(rubbishBinM));
	rubbishBin.draw();

	skeletonM = glm::translate(skeletonM, glm::vec3(8.0f, 2.6f, 5.0f));
	skeletonM = glm::scale(skeletonM, glm::vec3(2.5f, 3.0f, 3.0f));
	glUniformMatrix4fv(sp->u("M"),1,false,glm::value_ptr(skeletonM));
	skeleton.draw();
	///////
	M=glm::translate(M, glm::vec3(0.0f, 0.0f, -10.0f));
	glUniformMatrix4fv(sp->u("M"),1,false,glm::value_ptr(M));
	chair.draw();

	M=glm::translate(M, glm::vec3(-8.0f, 0.0f, 0.0f));
	glUniformMatrix4fv(sp->u("M"),1,false,glm::value_ptr(M));
	chair.draw();

	M=glm::translate(M, glm::vec3(-8.0f, 0.0f,0.0f));
	glUniformMatrix4fv(sp->u("M"),1,false,glm::value_ptr(M));
	chair.draw();

	M=glm::translate(M, glm::vec3(-8.0f, 0.0f,0.0f));
	glUniformMatrix4fv(sp->u("M"),1,false,glm::value_ptr(M));
	chair.draw();
	////////////
	M=glm::translate(M, glm::vec3(0.0f, 0.0f, -10.0f));
	glUniformMatrix4fv(sp->u("M"),1,false,glm::value_ptr(M));
	chair.draw();

	M=glm::translate(M, glm::vec3(8.0f, 0.0f, 0.0f));
	glUniformMatrix4fv(sp->u("M"),1,false,glm::value_ptr(M));
	chair.draw();
	
	M=glm::translate(M, glm::vec3(8.0f, 0.0f, 0.0f));
	glUniformMatrix4fv(sp->u("M"),1,false,glm::value_ptr(M));
	chair.draw();
	
	M=glm::translate(M, glm::vec3(8.0f, 0.0f, 0.0f));
	glUniformMatrix4fv(sp->u("M"),1,false,glm::value_ptr(M));
	chair.draw();

	return M;
}

void drawClasses() {

	glm::mat4 M=glm::mat4(1.0f);
	glm::mat4 M2=glm::mat4(1.0f);
	glm::mat4 M3=glm::mat4(1.0f);
	glm::mat4 M4=glm::mat4(1.0f);

	M=glm::translate(M, glm::vec3(13.0f, 2.0f, -15.0f));

	//first class -> on the left
	M = drawClass(M);
	M2 = glm::rotate(M,glm::radians(180.0f),glm::vec3(0.0f,1.0f,0.0f));
	M2 = glm::translate(M2, glm::vec3(0.0f, 0.0f, -70.0f));
	// second class -> on the right
	drawClass(M2);
	// third class
	M3 = glm::translate(M, glm::vec3(25.0f, 0.0f, 20.0f));
	drawClass(M3);
	//fourth class
	M4 = glm::translate(M2, glm::vec3(-50.0f, 0.0f, 0.0f));
	drawClass(M4);

}

void drawExternalWalls(glm::mat4 M) {

	M = glm::translate(M, glm::vec3(0.0f, 10.0f, -5.0f));
	M = glm::rotate(M,glm::radians(90.0f),glm::vec3(1.0f,0.0f,0.0f));
	M = glm::scale(M, glm::vec3(1.0f, 1.0f, 2.0f));
	glUniformMatrix4fv(sp->u("M"),1,false,glm::value_ptr(M));
	schoolWall.draw();

	

	for (int i = 0; i < 16; i++) {
		M = glm::translate(M, glm::vec3(-10.0f, 0.0f, 0.0f));
		glUniformMatrix4fv(sp->u("M"),1,false,glm::value_ptr(M));
		schoolWall.draw();
	}

	M = glm::rotate(M,glm::radians(90.0f),glm::vec3(0.0f,0.0f,1.0f));
	M = glm::translate(M, glm::vec3(5.0f, 5.0f, 0.0f));
	glUniformMatrix4fv(sp->u("M"),1,false,glm::value_ptr(M));
	schoolWall.draw();

	for (int i = 0; i < 11; i++) {
		M = glm::translate(M, glm::vec3(-10.0f, 0.0f, 0.0f));
		glUniformMatrix4fv(sp->u("M"),1,false,glm::value_ptr(M));
		schoolWall.draw();
	}

	M = glm::rotate(M,glm::radians(90.0f),glm::vec3(0.0f,0.0f,1.0f));
	M = glm::translate(M, glm::vec3(-5.0f, 5.0f, 0.0f));
	glUniformMatrix4fv(sp->u("M"),1,false,glm::value_ptr(M));
	schoolWall.draw();

	for (int i = 0; i < 16; i++) {
		M = glm::translate(M, glm::vec3(-10.0f, 0.0f, 0.0f));
		glUniformMatrix4fv(sp->u("M"),1,false,glm::value_ptr(M));
		schoolWall.draw();
	}

	M = glm::rotate(M,glm::radians(90.0f),glm::vec3(0.0f,0.0f,1.0f));
	M = glm::translate(M, glm::vec3(-5.0f, 5.0f, 0.0f));
	glUniformMatrix4fv(sp->u("M"),1,false,glm::value_ptr(M));
	schoolWall.draw();

	for (int i = 0; i < 11; i++) {
		M = glm::translate(M, glm::vec3(-10.0f, 0.0f, 0.0f));
		glUniformMatrix4fv(sp->u("M"),1,false,glm::value_ptr(M));
		schoolWall.draw();
	}
} 


//Procedura rysująca zawartość sceny
void drawScene(GLFWwindow* window) {
	//************Tutaj umieszczaj kod rysujący obraz******************l
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glm::mat4 V=glm::lookAt(cameraPos,cameraPos + cameraFront,cameraUp); //Wylicz macierz widoku

    glm::mat4 P=glm::perspective(90.0f*PI/180.0f, aspectRatio, 0.01f, 200.0f); //Wylicz macierz rzutowania

    glm::mat4 M=glm::mat4(1.0f);

    sp->use();//Aktywacja programu cieniującego
    //Przeslij parametry programu cieniującego do karty graficznej
	glUniformMatrix4fv(sp->u("P"),1,false,glm::value_ptr(P));
    glUniformMatrix4fv(sp->u("V"),1,false,glm::value_ptr(V));
	
	M = drawFloorAndCeiling();
	drawExternalWalls(M);
	drawWalls();
	drawClasses();

    glfwSwapBuffers(window);
}

void processInput(GLFWwindow* window) {

	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
		glfwSetWindowShouldClose(window, true);
	}

	float cameraSpeed = 12 * deltaTime;
	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
		cameraPos += cameraSpeed * cameraFront;
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
		cameraPos -= cameraSpeed * cameraFront;
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
		cameraPos -= glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
	cameraPos += glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
}

void mouse_callback(GLFWwindow* window, double xpos, double ypos) {
	if (firstMouse) {
		lastX = xpos;
		lastY = ypos;
		firstMouse = false;
	}

	float xoffset = xpos - lastX;
	float yoffset = ypos - lastY;
	lastX = xpos;
	lastY = ypos;

	float sensitivity = 0.1f;
	xoffset *= sensitivity;
	yoffset *= sensitivity;

	yaw += xoffset;
	pitch -= yoffset;

	if (pitch > 89.0f) pitch = 89.0f;
	if (pitch < -89.0f) pitch = -89.0f;

	glm::vec3 front;
	front.x = 10*(cos(glm::radians(yaw)) * cos(glm::radians(pitch)));
	front.y = 0.0f;
	// front.y = sin(glm::radians(pitch));
	front.z = 10*(sin(glm::radians(yaw)) * cos(glm::radians(pitch)));
	cameraFront = glm::normalize(front);
}


int main(void)
{
	GLFWwindow* window;

	glfwSetErrorCallback(error_callback);

	if (!glfwInit()) {
		fprintf(stderr, "Nie można zainicjować GLFW.\n");
		exit(EXIT_FAILURE);
	}

	// window = glfwCreateWindow(1280, 720, "School", glfwGetPrimaryMonitor(), NULL);  
	window = glfwCreateWindow(1280, 720, "OpenGL", NULL, NULL);

	if (!window)
	{
		fprintf(stderr, "Nie można utworzyć okna.\n");
		glfwTerminate();
		exit(EXIT_FAILURE);
	}

	glfwSetCursorPosCallback(window, mouse_callback);

	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	glfwMakeContextCurrent(window);
	glfwSwapInterval(1);
	if (glewInit() != GLEW_OK) {
		fprintf(stderr, "Nie można zainicjować GLEW.\n");
		exit(EXIT_FAILURE);
	}

	initOpenGLProgram(window);


	while (!glfwWindowShouldClose(window))
	{
		float currentFrame = glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;

		processInput(window);

		drawScene(window);
		glfwPollEvents();
	}

	freeOpenGLProgram(window);

	glfwDestroyWindow(window);
	glfwTerminate();
	exit(EXIT_SUCCESS);
}

