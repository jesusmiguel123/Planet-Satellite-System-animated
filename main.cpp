#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <SOIL2/SOIL2.h>
#include <string>
#include <cmath>
#include <iostream>
#include <fstream>
#include <random>
#include <glm/gtc/type_ptr.hpp> // glm::value_ptr
#include <glm/gtc/matrix_transform.hpp> // glm::translate, glm::rotate, glm::scale, glm::perspective
#include "Sphere.h"
#include "Utils.h"
using namespace std;

float toRadians(float degrees) { return (degrees * 2.0f * 3.14159f) / 360.0f; }

#define numVAOs 1
#define numVBOs 7

float ang = 35.0f;
float rad = 8.0f;
float randf[100];
float rande[100];
float randv[100];
float randa[100];
int numMet;

float cameraX, cameraY, cameraZ;
float lLocX, lLocY, lLocZ;
float mLocX, mLocY, mLocZ;
float tLocX, tLocY, tLocZ;
float lightLocX, lightLocY, lightLocZ;
GLuint renderingProgram, renderingProgramCubeMap, renderingPrograml, renderingProgramm;
GLuint vaot[numVAOs];
GLuint vbot[numVBOs];

GLuint vaol[numVAOs];
GLuint vbol[numVBOs];

GLuint vaom[numVAOs];
GLuint vbom[numVBOs];

GLuint skyboxTexture;
float rotAmt = 0.0f;
float rotAmt1 = 0.0f;
float rotAmt2[100];

GLuint tierraNormalMap;
GLuint tierraTexture;

GLuint moonNormalMap;
GLuint moonTexture;

GLuint meteorNormalMap;
GLuint meteorTexture;

// variable allocation for display
GLuint vLoc, mvLoc, projLoc, nLoc;
GLuint globalAmbLoc, ambLoc, diffLoc, specLoc, posLoc, mambLoc, mdiffLoc, mspecLoc, mshiLoc;
GLuint globalAmbLocl, ambLocl, diffLocl, specLocl, posLocl, mambLocl, mdiffLocl, mspecLocl, mshiLocl;
GLuint globalAmbLocm, ambLocm, diffLocm, specLocm, posLocm, mambLocm, mdiffLocm, mspecLocm, mshiLocm;

int width, height;
float aspect;
glm::mat4 pMat, vMat, mMatt, mMatl, mMatm, mvMatt, mvMatm, mvMatl, invTrMat;
glm::vec3 currentLightPos;
float lightPos[3];

// white light
float globalAmbient[4] = { 0.3f, 0.3f, 0.3f, 1.0f };
float lightAmbient[4] = { 0.0f, 0.0f, 0.0f, 1.0f };
float lightDiffuse[4] = { 1.0f, 1.0f, 1.0f, 1.0f };
float lightSpecular[4] = { 1.0f, 1.0f, 1.0f, 1.0f };

float matAmb[4] = { 0.1923f, 0.1923f, 0.5923f, 1 };
float matDif[4] = { 0.5075f, 0.5075f, 0.7075f, 1 };
float matSpe[4] = { 0.5083f, 0.5083f, 0.7083f, 1 };
float matShi = 51.2f;


// silver material
float* matAmbl = Utils::silverAmbient();
float* matDifl = Utils::silverDiffuse();
float* matSpel = Utils::silverSpecular();
float matShil = Utils::silverShininess();

float* matAmbm = Utils::silverAmbient();
float* matDifm = Utils::silverDiffuse();
float* matSpem = Utils::silverSpecular();
float matShim = Utils::silverShininess();

Sphere mySpheret(48);
int numSphereVerticest, numSphereIndicest;

Sphere mySphere(48);
int numSphereVertices, numSphereIndices;

Sphere mySpherem(48);
int numSphereVerticesm, numSphereIndicesm;

void setupVertices(void) {
	float cubeVertexPositions[108] =
	{	-1.0f,  1.0f, -1.0f, -1.0f, -1.0f, -1.0f, 1.0f, -1.0f, -1.0f,
		1.0f, -1.0f, -1.0f, 1.0f,  1.0f, -1.0f, -1.0f,  1.0f, -1.0f,
		1.0f, -1.0f, -1.0f, 1.0f, -1.0f,  1.0f, 1.0f,  1.0f, -1.0f,
		1.0f, -1.0f,  1.0f, 1.0f,  1.0f,  1.0f, 1.0f,  1.0f, -1.0f,
		1.0f, -1.0f,  1.0f, -1.0f, -1.0f,  1.0f, 1.0f,  1.0f,  1.0f,
		-1.0f, -1.0f,  1.0f, -1.0f,  1.0f,  1.0f, 1.0f,  1.0f,  1.0f,
		-1.0f, -1.0f,  1.0f, -1.0f, -1.0f, -1.0f, -1.0f,  1.0f,  1.0f,
		-1.0f, -1.0f, -1.0f, -1.0f,  1.0f, -1.0f, -1.0f,  1.0f,  1.0f,
		-1.0f, -1.0f,  1.0f,  1.0f, -1.0f,  1.0f,  1.0f, -1.0f, -1.0f,
		1.0f, -1.0f, -1.0f, -1.0f, -1.0f, -1.0f, -1.0f, -1.0f,  1.0f,
		-1.0f,  1.0f, -1.0f, 1.0f,  1.0f, -1.0f, 1.0f,  1.0f,  1.0f,
		1.0f,  1.0f,  1.0f, -1.0f,  1.0f,  1.0f, -1.0f,  1.0f, -1.0f
	};
	float cubeTextureCoord[72] =
	{	1.00f, 0.6666666f, 1.00f, 0.3333333f, 0.75f, 0.3333333f,	// back face lower right
		0.75f, 0.3333333f, 0.75f, 0.6666666f, 1.00f, 0.6666666f,	// back face upper left
		0.75f, 0.3333333f, 0.50f, 0.3333333f, 0.75f, 0.6666666f,	// right face lower right
		0.50f, 0.3333333f, 0.50f, 0.6666666f, 0.75f, 0.6666666f,	// right face upper left
		0.50f, 0.3333333f, 0.25f, 0.3333333f, 0.50f, 0.6666666f,	// front face lower right
		0.25f, 0.3333333f, 0.25f, 0.6666666f, 0.50f, 0.6666666f,	// front face upper left
		0.25f, 0.3333333f, 0.00f, 0.3333333f, 0.25f, 0.6666666f,	// left face lower right
		0.00f, 0.3333333f, 0.00f, 0.6666666f, 0.25f, 0.6666666f,	// left face upper left
		0.25f, 0.3333333f, 0.50f, 0.3333333f, 0.50f, 0.0000000f,	// bottom face upper right
		0.50f, 0.0000000f, 0.25f, 0.0000000f, 0.25f, 0.3333333f,	// bottom face lower left
		0.25f, 1.0000000f, 0.50f, 1.0000000f, 0.50f, 0.6666666f,	// top face upper right
		0.50f, 0.6666666f, 0.25f, 0.6666666f, 0.25f, 1.0000000f		// top face lower left
	};

	numSphereVerticest = mySpheret.getNumIndices();

	std::vector<int> indt = mySpheret.getIndices();
	std::vector<glm::vec3> vertt = mySpheret.getVertices();
	std::vector<glm::vec2> text = mySpheret.getTexCoords();
	std::vector<glm::vec3> normt = mySpheret.getNormals();
	std::vector<glm::vec3> tangt = mySpheret.getTangents();

	std::vector<float> pvaluest;
	std::vector<float> tvaluest;
	std::vector<float> nvaluest;
	std::vector<float> tanvaluest;

	for (int i = 0; i < mySpheret.getNumIndices(); i++) {
		pvaluest.push_back((vertt[indt[i]]).x);
		pvaluest.push_back((vertt[indt[i]]).y);
		pvaluest.push_back((vertt[indt[i]]).z);
		tvaluest.push_back((text[indt[i]]).s);
		tvaluest.push_back((text[indt[i]]).t);
		nvaluest.push_back((normt[indt[i]]).x);
		nvaluest.push_back((normt[indt[i]]).y);
		nvaluest.push_back((normt[indt[i]]).z);
		tanvaluest.push_back((tangt[indt[i]]).x);
		tanvaluest.push_back((tangt[indt[i]]).y);
		tanvaluest.push_back((tangt[indt[i]]).z);
	}

	glGenVertexArrays(1, vaot);
	glBindVertexArray(vaot[0]);
	glGenBuffers(numVBOs, vbot);

	glBindBuffer(GL_ARRAY_BUFFER, vbot[0]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(cubeVertexPositions), cubeVertexPositions, GL_STATIC_DRAW);

	glBindBuffer(GL_ARRAY_BUFFER, vbot[1]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(cubeTextureCoord) * 4, cubeTextureCoord, GL_STATIC_DRAW);

	glBindBuffer(GL_ARRAY_BUFFER, vbot[2]);
	glBufferData(GL_ARRAY_BUFFER, pvaluest.size() * 4, &pvaluest[0], GL_STATIC_DRAW);

	glBindBuffer(GL_ARRAY_BUFFER, vbot[3]);
	glBufferData(GL_ARRAY_BUFFER, tvaluest.size() * 4, &tvaluest[0], GL_STATIC_DRAW);
	
	glBindBuffer(GL_ARRAY_BUFFER, vbot[4]);
	glBufferData(GL_ARRAY_BUFFER, nvaluest.size() * 4, &nvaluest[0], GL_STATIC_DRAW);

	glBindBuffer(GL_ARRAY_BUFFER, vbot[5]);
	glBufferData(GL_ARRAY_BUFFER, tanvaluest.size() * 4, &tanvaluest[0], GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vbot[6]);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indt.size() * 4, &indt[0], GL_STATIC_DRAW);
	
	///////////////////////////////////////////////////////////////////////////////////
	numSphereVertices = mySphere.getNumIndices();

	std::vector<int> indl = mySphere.getIndices();
	std::vector<glm::vec3> vertl = mySphere.getVertices();
	std::vector<glm::vec2> texl = mySphere.getTexCoords();
	std::vector<glm::vec3> norml = mySphere.getNormals();
	std::vector<glm::vec3> tangl = mySphere.getTangents();

	std::vector<float> pvaluesl;
	std::vector<float> tvaluesl;
	std::vector<float> nvaluesl;
	std::vector<float> tanvaluesl;

	for (int i = 0; i < mySphere.getNumIndices(); i++) {
		pvaluesl.push_back((vertl[indl[i]]).x);
		pvaluesl.push_back((vertl[indl[i]]).y);
		pvaluesl.push_back((vertl[indl[i]]).z);
		tvaluesl.push_back((texl[indl[i]]).s);
		tvaluesl.push_back((texl[indl[i]]).t);
		nvaluesl.push_back((norml[indl[i]]).x);
		nvaluesl.push_back((norml[indl[i]]).y);
		nvaluesl.push_back((norml[indl[i]]).z);
		tanvaluesl.push_back((tangl[indl[i]]).x);
		tanvaluesl.push_back((tangl[indl[i]]).y);
		tanvaluesl.push_back((tangl[indl[i]]).z);
	}

	glGenVertexArrays(1, vaol);
	glBindVertexArray(vaol[0]);
	glGenBuffers(numVBOs, vbol);

	glBindBuffer(GL_ARRAY_BUFFER, vbol[0]);
	glBufferData(GL_ARRAY_BUFFER, pvaluesl.size() * 4, &pvaluesl[0], GL_STATIC_DRAW);

	glBindBuffer(GL_ARRAY_BUFFER, vbol[1]);
	glBufferData(GL_ARRAY_BUFFER, tvaluesl.size() * 4, &tvaluesl[0], GL_STATIC_DRAW);

	glBindBuffer(GL_ARRAY_BUFFER, vbol[2]);
	glBufferData(GL_ARRAY_BUFFER, nvaluesl.size() * 4, &nvaluesl[0], GL_STATIC_DRAW);

	glBindBuffer(GL_ARRAY_BUFFER, vbol[3]);
	glBufferData(GL_ARRAY_BUFFER, tanvaluesl.size() * 4, &tanvaluesl[0], GL_STATIC_DRAW);
	
	///////////////////////////////////////////////////////////////////////////////////
	numSphereVerticesm = mySpherem.getNumIndices();

	std::vector<int> indm = mySpherem.getIndices();
	std::vector<glm::vec3> vertm = mySpherem.getVertices();
	std::vector<glm::vec2> texm = mySpherem.getTexCoords();
	std::vector<glm::vec3> normm = mySpherem.getNormals();
	std::vector<glm::vec3> tangm = mySpherem.getTangents();

	std::vector<float> pvaluesm;
	std::vector<float> tvaluesm;
	std::vector<float> nvaluesm;
	std::vector<float> tanvaluesm;

	for (int i = 0; i < mySpherem.getNumIndices(); i++) {
		pvaluesm.push_back((vertm[indm[i]]).x);
		pvaluesm.push_back((vertm[indm[i]]).y);
		pvaluesm.push_back((vertm[indm[i]]).z);
		tvaluesm.push_back((texm[indm[i]]).s);
		tvaluesm.push_back((texm[indm[i]]).t);
		nvaluesm.push_back((normm[indm[i]]).x);
		nvaluesm.push_back((normm[indm[i]]).y);
		nvaluesm.push_back((normm[indm[i]]).z);
		tanvaluesm.push_back((tangm[indm[i]]).x);
		tanvaluesm.push_back((tangm[indm[i]]).y);
		tanvaluesm.push_back((tangm[indm[i]]).z);
	}

	glGenVertexArrays(1, vaom);
	glBindVertexArray(vaom[0]);
	glGenBuffers(numVBOs, vbom);

	glBindBuffer(GL_ARRAY_BUFFER, vbom[0]);
	glBufferData(GL_ARRAY_BUFFER, pvaluesm.size() * 4, &pvaluesm[0], GL_STATIC_DRAW);

	glBindBuffer(GL_ARRAY_BUFFER, vbom[1]);
	glBufferData(GL_ARRAY_BUFFER, tvaluesm.size() * 4, &tvaluesm[0], GL_STATIC_DRAW);

	glBindBuffer(GL_ARRAY_BUFFER, vbom[2]);
	glBufferData(GL_ARRAY_BUFFER, nvaluesm.size() * 4, &nvaluesm[0], GL_STATIC_DRAW);

	glBindBuffer(GL_ARRAY_BUFFER, vbom[3]);
	glBufferData(GL_ARRAY_BUFFER, tanvaluesm.size() * 4, &tanvaluesm[0], GL_STATIC_DRAW);
}


void installLights(glm::mat4 v_matrix) {
	glm::vec3 transformed = glm::vec3(v_matrix * glm::vec4(currentLightPos, 1.0));
	lightPos[0] = transformed.x;
	lightPos[1] = transformed.y;
	lightPos[2] = transformed.z;

	// get the locations of the light and material fields in the shader
	globalAmbLoc = glGetUniformLocation(renderingProgram, "globalAmbient");
	ambLoc = glGetUniformLocation(renderingProgram, "light.ambient");
	diffLoc = glGetUniformLocation(renderingProgram, "light.diffuse");
	specLoc = glGetUniformLocation(renderingProgram, "light.specular");
	posLoc = glGetUniformLocation(renderingProgram, "light.position");
	mambLoc = glGetUniformLocation(renderingProgram, "material.ambient");
	mdiffLoc = glGetUniformLocation(renderingProgram, "material.diffuse");
	mspecLoc = glGetUniformLocation(renderingProgram, "material.specular");
	mshiLoc = glGetUniformLocation(renderingProgram, "material.shininess");

	//  set the uniform light and material values in the shader
	glProgramUniform4fv(renderingProgram, globalAmbLoc, 1, globalAmbient);
	glProgramUniform4fv(renderingProgram, ambLoc, 1, lightAmbient);
	glProgramUniform4fv(renderingProgram, diffLoc, 1, lightDiffuse);
	glProgramUniform4fv(renderingProgram, specLoc, 1, lightSpecular);
	glProgramUniform3fv(renderingProgram, posLoc, 1, lightPos);
	glProgramUniform4fv(renderingProgram, mambLoc, 1, matAmb);
	glProgramUniform4fv(renderingProgram, mdiffLoc, 1, matDif);
	glProgramUniform4fv(renderingProgram, mspecLoc, 1, matSpe);
	glProgramUniform1f(renderingProgram, mshiLoc, matShi);
	
	////////////////////////////////////////////////////////////////////////////
	
	globalAmbLocl = glGetUniformLocation(renderingPrograml, "globalAmbient");
	ambLocl = glGetUniformLocation(renderingPrograml, "light.ambient");
	diffLocl = glGetUniformLocation(renderingPrograml, "light.diffuse");
	specLocl = glGetUniformLocation(renderingPrograml, "light.specular");
	posLocl = glGetUniformLocation(renderingPrograml, "light.position");
	mambLocl = glGetUniformLocation(renderingPrograml, "material.ambient");
	mdiffLocl = glGetUniformLocation(renderingPrograml, "material.diffuse");
	mspecLocl = glGetUniformLocation(renderingPrograml, "material.specular");
	mshiLocl = glGetUniformLocation(renderingPrograml, "material.shininess");

	//  set the uniform light and material values in the shader
	glProgramUniform4fv(renderingPrograml, globalAmbLocl, 1, globalAmbient);
	glProgramUniform4fv(renderingPrograml, ambLocl, 1, lightAmbient);
	glProgramUniform4fv(renderingPrograml, diffLocl, 1, lightDiffuse);
	glProgramUniform4fv(renderingPrograml, specLocl, 1, lightSpecular);
	glProgramUniform3fv(renderingPrograml, posLocl, 1, lightPos);
	glProgramUniform4fv(renderingPrograml, mambLocl, 1, matAmbl);
	glProgramUniform4fv(renderingPrograml, mdiffLocl, 1, matDifl);
	glProgramUniform4fv(renderingPrograml, mspecLocl, 1, matSpel);
	glProgramUniform1f(renderingPrograml, mshiLocl, matShil);
	
	////////////////////////////////////////////////////////////////////////////
	
	globalAmbLocm = glGetUniformLocation(renderingProgramm, "globalAmbient");
	ambLocm = glGetUniformLocation(renderingProgramm, "light.ambient");
	diffLocm = glGetUniformLocation(renderingProgramm, "light.diffuse");
	specLocm = glGetUniformLocation(renderingProgramm, "light.specular");
	posLocm = glGetUniformLocation(renderingProgramm, "light.position");
	mambLocm = glGetUniformLocation(renderingProgramm, "material.ambient");
	mdiffLocm = glGetUniformLocation(renderingProgramm, "material.diffuse");
	mspecLocm = glGetUniformLocation(renderingProgramm, "material.specular");
	mshiLocm = glGetUniformLocation(renderingProgramm, "material.shininess");

	//  set the uniform light and material values in the shader
	glProgramUniform4fv(renderingProgramm, globalAmbLocm, 1, globalAmbient);
	glProgramUniform4fv(renderingProgramm, ambLocm, 1, lightAmbient);
	glProgramUniform4fv(renderingProgramm, diffLocm, 1, lightDiffuse);
	glProgramUniform4fv(renderingProgramm, specLocm, 1, lightSpecular);
	glProgramUniform3fv(renderingProgramm, posLocm, 1, lightPos);
	glProgramUniform4fv(renderingProgramm, mambLocm, 1, matAmbm);
	glProgramUniform4fv(renderingProgramm, mdiffLocm, 1, matDifm);
	glProgramUniform4fv(renderingProgramm, mspecLocm, 1, matSpem);
	glProgramUniform1f(renderingProgramm, mshiLocm, matShim);
}


void init(GLFWwindow* window) {
	renderingProgram = Utils::createShaderProgram("vertShader.glsl", "fragShader.glsl");
	renderingPrograml = Utils::createShaderProgram("vertShader.glsl", "fragShader.glsl");
	renderingProgramm = Utils::createShaderProgram("vertShader.glsl", "fragShader.glsl");
	renderingProgramCubeMap = Utils::createShaderProgram("vertCShader.glsl", "fragCShader.glsl");

	tLocX = 0.0f; tLocY = 0.0f; tLocZ = -4.0f;
	lLocX = 0.0f; lLocY = 0.0f; lLocZ = -4.0f;
	mLocX = 0.0f; mLocY = 0.0f; mLocZ = -4.0f;
	cameraX = 0.0f; cameraY = 0.0f; cameraZ = -4.0f;
	lightLocX = 6.0f; lightLocY = -0.4f; lightLocZ = 2.5f;

	glfwGetFramebufferSize(window, &width, &height);
	aspect = (float)width / (float)height;
	pMat = glm::perspective(1.0472f, aspect, 0.1f, 1000.0f);

	setupVertices();

	skyboxTexture = Utils::loadTexture("space.jpg");
	moonTexture = Utils::loadTexture("moon.jpg");
	moonNormalMap = Utils::loadTexture("moonNORMAL.jpg");
	meteorTexture = Utils::loadTexture("meteor.jpg");
	meteorNormalMap = Utils::loadTexture("meteorNORMAL.jpg");
	tierraTexture = Utils::loadTexture("tierra.jpg");
	tierraNormalMap = Utils::loadTexture("tierraNORMAL.jpg");
}

void display(GLFWwindow* window, double currentTime) {
	glClear(GL_DEPTH_BUFFER_BIT);
	glClearColor(0.0, 0.0, 0.0, 1.0);
	glClear(GL_COLOR_BUFFER_BIT);

	vMat = glm::translate(glm::mat4(1.0f), glm::vec3(-cameraX, -cameraY, -cameraZ));
	
	glm::mat4 v = glm::mat4(1.0f);
	
	v = glm::lookAt(glm::vec3(sin(ang)*rad, 0.0f, cos(ang)*rad), 
					    glm::vec3(0.0f        , 0.0f, 0.0f), 
						 glm::vec3(0.0f        , 0.1f, 0.0f));
	
	vMat = v*vMat;
	
	glUseProgram(renderingProgramCubeMap);

	vLoc = glGetUniformLocation(renderingProgramCubeMap, "v_matrix");
	glUniformMatrix4fv(vLoc, 1, GL_FALSE, glm::value_ptr(vMat));

	projLoc = glGetUniformLocation(renderingProgramCubeMap, "p_matrix");
	glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(pMat));

	glBindBuffer(GL_ARRAY_BUFFER, vbot[0]);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(0);
	
	glBindBuffer(GL_ARRAY_BUFFER, vbot[1]);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(1);
	
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, skyboxTexture);

	glEnable(GL_CULL_FACE);
	glFrontFace(GL_CCW);	// cube is CW, but we are viewing the inside
	glDisable(GL_DEPTH_TEST);
	glDrawArrays(GL_TRIANGLES, 0, 36);
	glEnable(GL_DEPTH_TEST);

	// draw scene (in this case it is just a torus)

	glUseProgram(renderingProgram);

	mvLoc = glGetUniformLocation(renderingProgram, "mv_matrix");
	projLoc = glGetUniformLocation(renderingProgram, "proj_matrix");
	nLoc = glGetUniformLocation(renderingProgram, "norm_matrix");	

	mMatt = glm::translate(glm::mat4(1.0f), glm::vec3(tLocX, tLocY, tLocZ));
	mMatt = glm::scale(mMatt, glm::vec3(1.0f, 1.0f, 1.0f));
	mMatt = glm::rotate(mMatt, toRadians(15.0f), glm::vec3(1.0f, 0.0f, 0.0f));
	mMatt = glm::rotate(mMatt, rotAmt, glm::vec3(0.0f, 1.0f, 0.0f));
	rotAmt += 0.002f;
		
	mvMatt = vMat * mMatt;

	invTrMat = glm::transpose(glm::inverse(mvMatt));

	currentLightPos = glm::vec3(lightLocX, lightLocY, lightLocZ);
	installLights(vMat);

	glUniformMatrix4fv(mvLoc, 1, GL_FALSE, glm::value_ptr(mvMatt));
	glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(pMat));
	glUniformMatrix4fv(nLoc, 1, GL_FALSE, glm::value_ptr(invTrMat));

	glBindBuffer(GL_ARRAY_BUFFER, vbot[2]);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(0);

	glBindBuffer(GL_ARRAY_BUFFER, vbot[3]);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(1);

	glBindBuffer(GL_ARRAY_BUFFER, vbot[4]);
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(2);

	glBindBuffer(GL_ARRAY_BUFFER, vbot[5]);
	glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(3);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, tierraNormalMap);

	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, tierraTexture);

	glClear(GL_DEPTH_BUFFER_BIT);
	glDisable(GL_CULL_FACE);
	glFrontFace(GL_CCW);
	glDisable(GL_LEQUAL);
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);
	
	glDrawArrays(GL_TRIANGLES, 0, numSphereVerticest);
	
	/////////////////////////////////////////////////////////////////////////////////////////////
	
	glUseProgram(renderingPrograml);

	mvLoc = glGetUniformLocation(renderingPrograml, "mv_matrix");
	projLoc = glGetUniformLocation(renderingPrograml, "proj_matrix");
	nLoc = glGetUniformLocation(renderingPrograml, "norm_matrix");
	
	mMatl = glm::translate(glm::mat4(1.0f), glm::vec3(lLocX, lLocY, lLocZ));
	mMatl = glm::scale(mMatl, glm::vec3(0.3f, 0.3f, 0.3f));
	mMatl = glm::rotate(mMatl, toRadians(15.0f), glm::vec3(1.0f, 0.0f, 0.0f));
	mMatl = glm::translate(mMatl, glm::vec3(sin(rotAmt1)*12.0f, 0.0f, cos(rotAmt1)*12.0f));
	rotAmt1 += 0.02f;
	mvMatl = vMat * mMatl;
	invTrMat = glm::transpose(glm::inverse(mvMatl));

	currentLightPos = glm::vec3(lightLocX, lightLocY, lightLocZ);
	installLights(vMat);

	glUniformMatrix4fv(mvLoc, 1, GL_FALSE, glm::value_ptr(mvMatl));
	glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(pMat));
	glUniformMatrix4fv(nLoc, 1, GL_FALSE, glm::value_ptr(invTrMat));

	glBindBuffer(GL_ARRAY_BUFFER, vbol[0]);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(0);

	glBindBuffer(GL_ARRAY_BUFFER, vbol[1]);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(1);

	glBindBuffer(GL_ARRAY_BUFFER, vbol[2]);
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(2);

	glBindBuffer(GL_ARRAY_BUFFER, vbol[3]);
	glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(3);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, moonNormalMap);

	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, moonTexture);

	glDisable(GL_CULL_FACE);
	glFrontFace(GL_CCW);
	glDisable(GL_LEQUAL);
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);

	glDrawArrays(GL_TRIANGLES, 0, numSphereVertices);
	
	
	
	
	/////////////////////////////////////////////////////////////////////////////////////////////
	
	glUseProgram(renderingProgramm);

	mvLoc = glGetUniformLocation(renderingProgramm, "mv_matrix");
	projLoc = glGetUniformLocation(renderingProgramm, "proj_matrix");
	nLoc = glGetUniformLocation(renderingProgramm, "norm_matrix");
	
	for(int s = 0; s < numMet; s++){
		mMatm = glm::translate(glm::mat4(1.0f), glm::vec3(mLocX, mLocY, mLocZ));
		mMatm = glm::scale(mMatm, glm::vec3(rande[s], rande[s], rande[s]));
		mMatm = glm::rotate(mMatm, toRadians(randa[s]), glm::vec3(1.0f, 0.0f, 0.0f));
		mMatm = glm::translate(mMatm, glm::vec3(sin(rotAmt2[s])*randf[s], 0.0f, cos(rotAmt2[s])*randf[s]));
		rotAmt2[s] += randv[s];
		mvMatm = vMat * mMatm;
		invTrMat = glm::transpose(glm::inverse(mvMatm));

		currentLightPos = glm::vec3(lightLocX, lightLocY, lightLocZ);
		installLights(vMat);

		glUniformMatrix4fv(mvLoc, 1, GL_FALSE, glm::value_ptr(mvMatm));
		glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(pMat));
		glUniformMatrix4fv(nLoc, 1, GL_FALSE, glm::value_ptr(invTrMat));

		glBindBuffer(GL_ARRAY_BUFFER, vbom[0]);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
		glEnableVertexAttribArray(0);

		glBindBuffer(GL_ARRAY_BUFFER, vbom[1]);
		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, 0);
		glEnableVertexAttribArray(1);

		glBindBuffer(GL_ARRAY_BUFFER, vbom[2]);
		glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 0, 0);
		glEnableVertexAttribArray(2);

		glBindBuffer(GL_ARRAY_BUFFER, vbom[3]);
		glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, 0, 0);
		glEnableVertexAttribArray(3);

		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, meteorNormalMap);

		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, meteorTexture);

		glDisable(GL_CULL_FACE);
		glFrontFace(GL_CCW);
		glDisable(GL_LEQUAL);
		glEnable(GL_DEPTH_TEST);
		glDepthFunc(GL_LEQUAL);

		glDrawArrays(GL_TRIANGLES, 0, numSphereVerticesm);
	}
}

void window_size_callback(GLFWwindow* win, int newWidth, int newHeight) {
	aspect = (float)newWidth / (float)newHeight;
	glViewport(0, 0, newWidth, newHeight);
	pMat = glm::perspective(1.0472f, aspect, 0.1f, 1000.0f);
}

int main(void) {
	if (!glfwInit()) { exit(EXIT_FAILURE); }
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	GLFWwindow* window = glfwCreateWindow(800, 800, "Proyecto de Computación Gráfica", NULL, NULL);
	glfwMakeContextCurrent(window);
	if (glewInit() != GLEW_OK) { exit(EXIT_FAILURE); }
	glfwSwapInterval(1);

	glfwSetWindowSizeCallback(window, window_size_callback);

	init(window);
	
	numMet = 7;
	
	random_device rd;
	default_random_engine eng(rd());
	uniform_real_distribution<> distr(6, 40);
	
	random_device rd1;
	default_random_engine eng1(rd1());
	uniform_real_distribution<> distr1(1, 10);
	
	random_device rd2;
	default_random_engine eng2(rd2());
	uniform_real_distribution<> distr2(1, 5);
	
	random_device rd3;
	default_random_engine eng3(rd3());
	uniform_real_distribution<> distr3(0, 60);
	
	for(int w = 0; w < numMet; w++){
		rotAmt2[w] = 0.0f;
		randf[w] = distr(eng);
		rande[w] = distr1(eng1)/10.0f;
		randv[w] = distr2(eng2)/1000.0f;
		randa[w] = distr3(eng3);
		if(11.5 < randf[w] && randf[w] < 12.5){
			randf[w] = randf[w] + 2.0f;
		}
	}


	while (!glfwWindowShouldClose(window)) {
		if(glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS){
			ang = ang + 0.01f;
		}
		if(glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS){
			ang = ang - 0.01f;
		}
		if(glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS){
			if(rad > 1.2f){
				rad = rad - 0.1f;
			}
		}
		if(glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS){
			rad = rad + 0.1f;
		}
		display(window, glfwGetTime());
		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	glfwDestroyWindow(window);
	glfwTerminate();
	exit(EXIT_SUCCESS);
}
