//
//  main.cpp
//  OpenGL Shadows
//
//  Created by CGIS on 05/12/16.
//  Copyright � 2016 CGIS. All rights reserved.
//

#define GLEW_STATIC

#include <iostream>
#include "glm/glm.hpp"//core glm functionality
#include "glm/gtc/matrix_transform.hpp"//glm extension for generating common transformation matrices
#include "glm/gtc/matrix_inverse.hpp"
#include "glm/gtc/type_ptr.hpp"
#include "GLEW/glew.h"
#include "GLFW/glfw3.h"
#include <string>
#include "Shader.hpp"
#include "Camera.hpp"
#include "SkyBox.hpp"
#define TINYOBJLOADER_IMPLEMENTATION

#include "Model3D.hpp"
#include "Mesh.hpp"

int glWindowWidth = 640;
int glWindowHeight = 480;
int retina_width, retina_height;
GLFWwindow* glWindow = NULL;

const GLuint SHADOW_WIDTH = 2048, SHADOW_HEIGHT = 2048;

glm::mat4 model;
GLuint modelLoc;
glm::mat4 view;
GLuint viewLoc;
glm::mat4 projection;
GLuint projectionLoc;
glm::mat3 normalMatrix;
GLuint normalMatrixLoc;
glm::mat3 lightDirMatrix;
GLuint lightDirMatrixLoc;

glm::vec3 lightDir;
GLuint lightDirLoc;
glm::vec3 lightColor;
GLuint lightColorLoc;

gps::Camera myCamera(glm::vec3(0.0f, 1.0f, 2.5f), glm::vec3(0.0f, 0.0f, 0.0f));
GLfloat cameraSpeed = 0.2f;

bool pressedKeys[1024];
GLfloat angle;
GLfloat lightAngle;
GLfloat positionY;
GLfloat positionX;

gps::Model3D ground;
gps::Model3D lightCube;
gps::Shader myCustomShader;
gps::Shader lightShader;
gps::Shader depthMapShader;
gps::Model3D horn;
gps::Model3D idk;
gps::Model3D otherroofs;
gps::Model3D outdoor;
gps::Model3D outdoor2;
gps::Model3D roofs;
gps::Model3D windows;
gps::Model3D ball;
gps::Model3D balloonup;
gps::Model3D balloondown;
gps::Model3D base;
gps::Model3D books;
gps::Model3D corners;
gps::Model3D fencein;
gps::Model3D fenceout;
gps::Model3D field;
gps::Model3D floordown;
gps::Model3D floorup;
gps::Model3D goals;
gps::Model3D roof;
gps::Model3D stairs;
gps::Model3D treedown;
gps::Model3D treeup;
gps::Model3D walls;
gps::Model3D walls2;
gps::Model3D comoda;
gps::Model3D tv;
gps::Model3D couch;



GLuint shadowMapFBO;
GLuint depthMapTexture;

gps::SkyBox mySkyBox;
gps::Shader skyboxShader;

std::vector<const GLchar*> faces;

GLenum glCheckError_(const char *file, int line)
{
	GLenum errorCode;
	while ((errorCode = glGetError()) != GL_NO_ERROR)
	{
		std::string error;
		switch (errorCode)
		{
		case GL_INVALID_ENUM:                  error = "INVALID_ENUM"; break;
		case GL_INVALID_VALUE:                 error = "INVALID_VALUE"; break;
		case GL_INVALID_OPERATION:             error = "INVALID_OPERATION"; break;
		case GL_STACK_OVERFLOW:                error = "STACK_OVERFLOW"; break;
		case GL_STACK_UNDERFLOW:               error = "STACK_UNDERFLOW"; break;
		case GL_OUT_OF_MEMORY:                 error = "OUT_OF_MEMORY"; break;
		case GL_INVALID_FRAMEBUFFER_OPERATION: error = "INVALID_FRAMEBUFFER_OPERATION"; break;
		}
		std::cout << error << " | " << file << " (" << line << ")" << std::endl;
	}
	return errorCode;
}
#define glCheckError() glCheckError_(__FILE__, __LINE__)

void windowResizeCallback(GLFWwindow* window, int width, int height)
{
	fprintf(stdout, "window resized to width: %d , and height: %d\n", width, height);
	//TODO
	//for RETINA display
	glfwGetFramebufferSize(glWindow, &retina_width, &retina_height);

	myCustomShader.useShaderProgram();

	//set projection matrix
	glm::mat4 projection = glm::perspective(glm::radians(45.0f), (float)retina_width / (float)retina_height, 0.1f, 1000.0f);
	//send matrix data to shader
	GLint projLoc = glGetUniformLocation(myCustomShader.shaderProgram, "projection");
	glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projection));

	lightShader.useShaderProgram();

	glUniformMatrix4fv(glGetUniformLocation(lightShader.shaderProgram, "projection"), 1, GL_FALSE, glm::value_ptr(projection));

	//set Viewport transform
	glViewport(0, 0, retina_width, retina_height);
}

void keyboardCallback(GLFWwindow* window, int key, int scancode, int action, int mode)
{
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
		glfwSetWindowShouldClose(window, GL_TRUE);

	if (key >= 0 && key < 1024)
	{
		if (action == GLFW_PRESS)
			pressedKeys[key] = true;
		else if (action == GLFW_RELEASE)
			pressedKeys[key] = false;
	}
}

void mouseCallback(GLFWwindow* window, double xpos, double ypos)
{
	myCamera.rotate(float(glWindowWidth / 2 - xpos), float(glWindowHeight / 2 - ypos));
}

void processMovement()
{
	if (pressedKeys[GLFW_KEY_Q]) {
		angle += 0.3f;
		if (angle > 360.0f)
			angle -= 360.0f;
	}

	if (pressedKeys[GLFW_KEY_LEFT]) {
		positionY += 0.3f;
		if (positionY >= 12.7) {
			positionY = 0;
			positionX = 0;
		}
	}
	if (pressedKeys[GLFW_KEY_RIGHT]) {
		positionY -= 0.3f;
		if (positionY <= -12.7) {
			positionY = 0;
			positionX = 0;
		}
	}
	if (pressedKeys[GLFW_KEY_DOWN]) {
		positionX += 0.3f;
		if (positionX >= 8.5) {
			positionX = 0;
			positionY = 0;
		}
	}
	if (pressedKeys[GLFW_KEY_UP]) {
		positionX -= 0.3f;
		if (positionX <= -8.5) {
			positionX = 0;
			positionY = 0;
		}
	}

	if (pressedKeys[GLFW_KEY_E]) {
		angle -= 0.3f;
		if (angle < 0.0f)
			angle += 360.0f;
	}

	if (pressedKeys[GLFW_KEY_W]) {
		myCamera.move(gps::MOVE_FORWARD, cameraSpeed);
	}

	if (pressedKeys[GLFW_KEY_S]) {
		myCamera.move(gps::MOVE_BACKWARD, cameraSpeed);
	}

	if (pressedKeys[GLFW_KEY_A]) {
		myCamera.move(gps::MOVE_LEFT, cameraSpeed);
	}

	if (pressedKeys[GLFW_KEY_D]) {
		myCamera.move(gps::MOVE_RIGHT, cameraSpeed);
	}
	
	if (pressedKeys[GLFW_KEY_J]) {

		lightAngle += 1.3f;
		if (lightAngle > 360.0f)
			lightAngle -= 360.0f;
		glm::vec3 lightDirTr = glm::vec3(glm::rotate(glm::mat4(1.0f), glm::radians(lightAngle), glm::vec3(0.0f, 1.0f, 0.0f)) * glm::vec4(lightDir, 1.0f));
		myCustomShader.useShaderProgram();
		glUniform3fv(lightDirLoc, 1, glm::value_ptr(lightDirTr));
	}

	if (pressedKeys[GLFW_KEY_L]) {
		lightAngle -= 1.3f;
		if (lightAngle < 0.0f)
			lightAngle += 360.0f;
		glm::vec3 lightDirTr = glm::vec3(glm::rotate(glm::mat4(1.0f), glm::radians(lightAngle), glm::vec3(0.0f, 1.0f, 0.0f)) * glm::vec4(lightDir, 1.0f));
		myCustomShader.useShaderProgram();
		glUniform3fv(lightDirLoc, 1, glm::value_ptr(lightDirTr));
	}
	
	if (pressedKeys[GLFW_KEY_Z])
	{
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	}
	if (pressedKeys[GLFW_KEY_X])
	{
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	}
	if (pressedKeys[GLFW_KEY_C])
	{
		glPolygonMode(GL_FRONT_AND_BACK, GL_POINT);
	}
}

bool initOpenGLWindow()
{
	if (!glfwInit()) {
		fprintf(stderr, "ERROR: could not start GLFW3\n");
		return false;
	}

	//for Mac OS X
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	glWindow = glfwCreateWindow(glWindowWidth, glWindowHeight, "OpenGL Shader Example", NULL, NULL);
	if (!glWindow) {
		fprintf(stderr, "ERROR: could not open window with GLFW3\n");
		glfwTerminate();
		return false;
	}

	glfwSetWindowSizeCallback(glWindow, windowResizeCallback);
	glfwMakeContextCurrent(glWindow);

	glfwWindowHint(GLFW_SAMPLES, 4);

	// start GLEW extension handler
	glewExperimental = GL_TRUE;
	glewInit();

	// get version info
	const GLubyte* renderer = glGetString(GL_RENDERER); // get renderer string
	const GLubyte* version = glGetString(GL_VERSION); // version as a string
	printf("Renderer: %s\n", renderer);
	printf("OpenGL version supported %s\n", version);

	//for RETINA display
	glfwGetFramebufferSize(glWindow, &retina_width, &retina_height);

	glfwSetKeyCallback(glWindow, keyboardCallback);
	glfwSetCursorPosCallback(glWindow, mouseCallback);
	//glfwSetInputMode(glWindow, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	return true;
}

void initOpenGLState()
{
	glClearColor(0.3f, 0.3f, 0.3f, 1.0f);
	glViewport(0, 0, retina_width, retina_height);

	glEnable(GL_DEPTH_TEST); // enable depth-testing
	glDepthFunc(GL_LESS); // depth-testing interprets a smaller value as "closer"
						  //glEnable(GL_CULL_FACE); // cull face
	glCullFace(GL_BACK); // cull back face
	glFrontFace(GL_CCW); // GL_CCW for counter clock-wise
}

void initFBOs()
{
	//generate FBO ID
	glGenFramebuffers(1, &shadowMapFBO);

	//create depth texture for FBO
	glGenTextures(1, &depthMapTexture);
	glBindTexture(GL_TEXTURE_2D, depthMapTexture);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT,
		SHADOW_WIDTH, SHADOW_HEIGHT, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

	//attach texture to FBO
	glBindFramebuffer(GL_FRAMEBUFFER, shadowMapFBO);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, depthMapTexture, 0);
	glDrawBuffer(GL_NONE);
	glReadBuffer(GL_NONE);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

glm::mat4 computeLightSpaceTrMatrix()
{
	const GLfloat near_plane = 1.0f, far_plane = 10.0f;
	glm::mat4 lightProjection = glm::ortho(-20.0f, 20.0f, -20.0f, 20.0f, near_plane, far_plane);

	glm::vec3 lightDirTr = glm::vec3(glm::rotate(glm::mat4(1.0f), glm::radians(lightAngle), glm::vec3(0.0f, 1.0f, 0.0f)) * glm::vec4(lightDir, 1.0f));
	glm::mat4 lightView = glm::lookAt(lightDirTr, myCamera.getCameraTarget(), glm::vec3(0.0f, 1.0f, 0.0f));

	return lightProjection * lightView;
}

void initModels()
{
	lightCube = gps::Model3D("objects/cube/cube.obj", "objects/cube/");
	horn = gps::Model3D("objects/newGround/mesh0.obj", "objects/newGround/");
	idk = gps::Model3D("objects/newGround/mesh1.obj", "objects/newGround/");
	otherroofs = gps::Model3D("objects/newGround/mesh02.obj", "objects/newGround/");
	outdoor = gps::Model3D("objects/newGround/mesh3.obj", "objects/newGround/");
	outdoor2 = gps::Model3D("objects/newGround/mesh4.obj", "objects/newGround/");
	roofs = gps::Model3D("objects/newGround/mesh5.obj", "objects/newGround/");
	windows = gps::Model3D("objects/newGround/mesh6.obj", "objects/newGround/");
	ball = gps::Model3D("objects/newGround/ball.obj", "objects/newGround/");
	balloonup = gps::Model3D("objects/newGround/baloonup.obj", "objects/newGround/");
	balloondown = gps::Model3D("objects/newGround/baloondown.obj", "objects/newGround/");
	books = gps::Model3D("objects/newGround/books.obj", "objects/newGround/");
	fencein = gps::Model3D("objects/newGround/fencein.obj", "objects/newGround/");
	fenceout = gps::Model3D("objects/newGround/fenceout.obj", "objects/newGround/");
	field = gps::Model3D("objects/newGround/field.obj", "objects/newGround/");
	floordown = gps::Model3D("objects/newGround/floordown.obj", "objects/newGround/");
	floorup = gps::Model3D("objects/newGround/floorup.obj", "objects/newGround/");
	goals = gps::Model3D("objects/newGround/goals.obj", "objects/newGround/");
	ground = gps::Model3D("objects/newGround/ground.obj", "objects/newGround/");
	stairs = gps::Model3D("objects/newGround/stairs.obj", "objects/newGround/");
	treedown = gps::Model3D("objects/newGround/treedown.obj", "objects/newGround/");
	treeup = gps::Model3D("objects/newGround/treeup.obj", "objects/newGround/");
	comoda = gps::Model3D("objects/newGround/comoda.obj", "objects/newGround/");
	tv = gps::Model3D("objects/newGround/tv.obj", "objects/newGround/");
	couch = gps::Model3D("objects/newGround/couch.obj", "objects/newGround/");


	faces.push_back("textures/skybox/green_rt.tga");
	faces.push_back("textures/skybox/green_lf.tga");
	faces.push_back("textures/skybox/green_up.tga");
	faces.push_back("textures/skybox/green_dn.tga");
	faces.push_back("textures/skybox/green_bk.tga");
	faces.push_back("textures/skybox/green_ft.tga");
}

void initShaders()
{
	myCustomShader.loadShader("shaders/shaderStart.vert", "shaders/shaderStart.frag");
	lightShader.loadShader("shaders/lightCube.vert", "shaders/lightCube.frag");
	depthMapShader.loadShader("shaders/simpleDepthMap.vert", "shaders/simpleDepthMap.frag");


}

void initUniforms()
{
	myCustomShader.useShaderProgram();

	modelLoc = glGetUniformLocation(myCustomShader.shaderProgram, "model");

	viewLoc = glGetUniformLocation(myCustomShader.shaderProgram, "view");

	normalMatrixLoc = glGetUniformLocation(myCustomShader.shaderProgram, "normalMatrix");

	lightDirMatrixLoc = glGetUniformLocation(myCustomShader.shaderProgram, "lightDirMatrix");

	projection = glm::perspective(glm::radians(45.0f), (float)retina_width / (float)retina_height, 0.1f, 1000.0f);
	projectionLoc = glGetUniformLocation(myCustomShader.shaderProgram, "projection");
	glUniformMatrix4fv(projectionLoc, 1, GL_FALSE, glm::value_ptr(projection));

	//set the light direction (direction towards the light)
	lightDir = glm::vec3(3.0f, 3.2f, 2.5f);
	lightDirLoc = glGetUniformLocation(myCustomShader.shaderProgram, "lightDir");
	glUniform3fv(lightDirLoc, 1, glm::value_ptr(lightDir));

	//set light color
	lightColor = glm::vec3(1.0f, 1.0f, 1.0f); //white light
	lightColorLoc = glGetUniformLocation(myCustomShader.shaderProgram, "lightColor");
	glUniform3fv(lightColorLoc, 1, glm::value_ptr(lightColor));

	lightShader.useShaderProgram();
	glUniformMatrix4fv(glGetUniformLocation(lightShader.shaderProgram, "projection"), 1, GL_FALSE, glm::value_ptr(projection));

	mySkyBox.Load(faces);
	skyboxShader.loadShader("shaders/skyboxShader.vert", "shaders/skyboxShader.frag");
	skyboxShader.useShaderProgram();
	view = myCamera.getViewMatrix();
	glUniformMatrix4fv(glGetUniformLocation(skyboxShader.shaderProgram, "view"), 1, GL_FALSE, glm::value_ptr(view));

	projection = glm::perspective(glm::radians(45.0f), (float)retina_width / (float)retina_height, 0.1f, 1000.0f);
	glUniformMatrix4fv(glGetUniformLocation(skyboxShader.shaderProgram, "projection"), 1, GL_FALSE, glm::value_ptr(projection));
}

void renderScene()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	processMovement();

	//render the scene to the depth buffer (first pass)

	depthMapShader.useShaderProgram();

	glUniformMatrix4fv(glGetUniformLocation(depthMapShader.shaderProgram, "lightSpaceTrMatrix"),
		1,
		GL_FALSE,
		glm::value_ptr(computeLightSpaceTrMatrix()));

	glViewport(0, 0, SHADOW_WIDTH, SHADOW_HEIGHT);
	glBindFramebuffer(GL_FRAMEBUFFER, shadowMapFBO);
	glClear(GL_DEPTH_BUFFER_BIT);

	//create model matrix for nanosuit
	model = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, -1.0f, 0.0f));
	model = glm::scale(model, glm::vec3(2.0f, 2.0f, 2.0f));
	//send model matrix to shader
	glUniformMatrix4fv(glGetUniformLocation(depthMapShader.shaderProgram, "model"),
		1,
		GL_FALSE,
		glm::value_ptr(model));
	treedown.Draw(depthMapShader);
	treeup.Draw(depthMapShader);


	//create model matrix for ground
	model = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, -1.0f, 0.0f));
	model = glm::scale(model, glm::vec3(2.0f, 2.0f, 2.0f));
	//send model matrix to shader
	glUniformMatrix4fv(glGetUniformLocation(depthMapShader.shaderProgram, "model"),
		1,
		GL_FALSE,
		glm::value_ptr(model));
	ground.Draw(depthMapShader);
	

	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	//render the scene (second pass)

	myCustomShader.useShaderProgram();

	//send lightSpace matrix to shader
	glUniformMatrix4fv(glGetUniformLocation(myCustomShader.shaderProgram, "lightSpaceTrMatrix"),
		1,
		GL_FALSE,
		glm::value_ptr(computeLightSpaceTrMatrix()));

	//send view matrix to shader
	view = myCamera.getViewMatrix();
	glUniformMatrix4fv(glGetUniformLocation(myCustomShader.shaderProgram, "view"),
		1,
		GL_FALSE,
		glm::value_ptr(view));

	//compute light direction transformation matrix
	lightDirMatrix = glm::mat3(glm::inverseTranspose(view));
	//send lightDir matrix data to shader
	glUniformMatrix3fv(lightDirMatrixLoc, 1, GL_FALSE, glm::value_ptr(lightDirMatrix));

	glViewport(0, 0, retina_width, retina_height);
	myCustomShader.useShaderProgram();

	//bind the depth map
	glActiveTexture(GL_TEXTURE3);
	glBindTexture(GL_TEXTURE_2D, depthMapTexture);
	glUniform1i(glGetUniformLocation(myCustomShader.shaderProgram, "shadowMap"), 3);

	//create model matrix for nanosuit
	model = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, -2.0f, 0.0f));
	model = glm::scale(model, glm::vec3(2.0f, 2.0f, 2.0f));
	//send model matrix data to shader	
	glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));

	//compute normal matrix
	normalMatrix = glm::mat3(glm::inverseTranspose(view*model));
	//send normal matrix data to shader
	glUniformMatrix3fv(normalMatrixLoc, 1, GL_FALSE, glm::value_ptr(normalMatrix));
	treedown.Draw(myCustomShader);
	treeup.Draw(myCustomShader);

	//create model matrix for ground
	model = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, -1.0f, 0.0f));
	//send model matrix data to shader
	glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));

	//create normal matrix
	normalMatrix = glm::mat3(glm::inverseTranspose(view*model));
	//send normal matrix data to shader
	glUniformMatrix3fv(normalMatrixLoc, 1, GL_FALSE, glm::value_ptr(normalMatrix));

	ground.Draw(myCustomShader);
	books.Draw(myCustomShader);
	fencein.Draw(myCustomShader);
	fenceout.Draw(myCustomShader);
	field.Draw(myCustomShader);
	floordown.Draw(myCustomShader);
	floorup.Draw(myCustomShader);
	goals.Draw(myCustomShader);
	ground.Draw(myCustomShader);
	roof.Draw(myCustomShader);
	stairs.Draw(myCustomShader);
	walls.Draw(myCustomShader);
	walls2.Draw(myCustomShader);
	comoda.Draw(myCustomShader);
	tv.Draw(myCustomShader);
	couch.Draw(myCustomShader);

	//create model matrix for ground
	model = glm::translate(glm::mat4(1.0f), glm::vec3(12.485f, -1.0f, 0.0f));
	//send model matrix data to shader
	glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));

	//create normal matrix
	normalMatrix = glm::mat3(glm::inverseTranspose(view*model));
	//send normal matrix data to shader
	glUniformMatrix3fv(normalMatrixLoc, 1, GL_FALSE, glm::value_ptr(normalMatrix));

	horn.Draw(myCustomShader);
	idk.Draw(myCustomShader);
	otherroofs.Draw(myCustomShader);
	outdoor.Draw(myCustomShader);
	outdoor2.Draw(myCustomShader);
	roofs.Draw(myCustomShader);
	windows.Draw(myCustomShader);

	model = glm::mat4(1.0f);
	model = glm::translate(model, glm::vec3(positionX, -1.0f, positionY));

	glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));

	ball.Draw(myCustomShader);

	model = glm::mat4(1.0f);
	model = glm::rotate(glm::mat4(9.0f), glm::radians(angle), glm::vec3(0, 1, 0));
	glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
	//balloon.Draw(myCustomShader);
	//balloon2.Draw(myCustomShader);
	balloonup.Draw(myCustomShader);
	balloondown.Draw(myCustomShader);

	//draw a white cube around the light

	lightShader.useShaderProgram();

	glUniformMatrix4fv(glGetUniformLocation(lightShader.shaderProgram, "view"), 1, GL_FALSE, glm::value_ptr(view));

	model = glm::rotate(glm::mat4(1.0f), glm::radians(lightAngle), glm::vec3(0.0f, 1.0f, 0.0f));
	model = glm::translate(model, lightDir);
	model = glm::scale(model, glm::vec3(0.05f, 0.05f, 0.05f));
	glUniformMatrix4fv(glGetUniformLocation(lightShader.shaderProgram, "model"), 1, GL_FALSE, glm::value_ptr(model));

	lightCube.Draw(lightShader);

	mySkyBox.Draw(skyboxShader, view, projection);
}

int main(int argc, const char * argv[]) {

	initOpenGLWindow();
	initOpenGLState();
	initFBOs();
	initModels();
	initShaders();
	initUniforms();
	glCheckError();
	while (!glfwWindowShouldClose(glWindow)) {
		angle += 0.1;
		if (angle >= 360)
			angle = 0;

		renderScene();
		lightAngle = 250.0;
		glfwPollEvents();
		glfwSwapBuffers(glWindow);
	}


	//close GL context and any other GLFW resources
	glfwTerminate();

	return 0;
}
