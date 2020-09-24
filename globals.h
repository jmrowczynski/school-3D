#ifndef GLOBALS
#define GLOBALS

#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <stdio.h>
#include "shaderprogram.h"
#include "OBJ_Loader.h"

using namespace std;

GLuint tex0;
GLuint tex1;

glm::vec3 cameraPos   = glm::vec3(0.0f, 11.0f, 0.0f);
glm::vec3 cameraFront = glm::vec3(0.0f,0.0f,0.0f);
glm::vec3 cameraUp    = glm::vec3(0.0f,1.0f,0.0f);
float yaw = -90.0f;
float pitch = 0.0f;

float deltaTime = 0.0f;
float lastFrame = 0.0f;

bool firstMouse = true;
float lastX = 800.0f / 2.0;
float lastY = 600.0f / 2.0;

float aspectRatio=1;

ShaderProgram *sp;

vector<vector<float>> vertices;
vector<vector<float>> normals;
vector<vector<float>> texCoords;

vector<objl::Mesh> meshes;

#endif