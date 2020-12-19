#pragma once
#include "RenderEngine.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/vector_angle.hpp>
#include <SOIL/SOIL.h>

class Demo :
	public RenderEngine
{
public:
	Demo();
	~Demo();
private:
	GLuint shaderProgram, VBO, VAO, EBO, texture, VBO2, VAO2, EBO2, texture2;
	GLuint kacalemariVBO, kacalemariVAO, kacalemariEBO, kacalemariTexture;
	GLuint kacalemari2VBO, kacalemari2VAO, kacalemari2EBO, kacalemari2Texture;
	GLuint tidurVBO, tidurVAO, tidurEBO, tidurTexture;
	GLuint jendelaVBO, jendelaVAO, jendelaEBO, jendelaTexture;
	GLuint papantidurVBO, papantidurVAO, papantidurEBO, papantidurTexture;
	GLuint papantidur2VBO, papantidur2VAO, papantidur2EBO, papantidur2Texture;
	GLuint tvVBO, tvVAO, tvEBO, tvTexture;
	GLuint acVBO, acVAO, acEBO, acTexture;
	GLuint wallVBO, wallVAO, wallEBO, wallTexture;
	GLuint topVBO, topVAO, topEBO, topTexture;
	GLuint alastvVBO, alastvVAO, alastvEBO, alastvTexture;
	float viewCamX, viewCamY, viewCamZ, upCamX, upCamY, upCamZ, posCamX, posCamY, posCamZ, CAMERA_SPEED, fovy;
	virtual void Init();
	virtual void DeInit();
	virtual void Update(double deltaTime);
	virtual void Render();
	void Buildwindow();
	void Buildpapantidur();
	void Buildpapantidur2();
	void Buildtidur();
	virtual void ProcessInput(GLFWwindow *window);
	void Buildbuffet();
	void Buildwall();
	void Buildtop();
	void BuildColoredCube();
	void Buildkacalemari2();
	void Buildkacalemari();
	void Drawkacalemari();
	void BuildColoredPlane();
	void Drawkacalemari2();
	void Buildalastv();
	void Buildtv();
	void Drawalastv();
	void Drawwall();
	void Drawtop();
	void Drawwindow();
	void DrawColoredCube();
	void Drawac();
	void Drawtv();
	void Drawpapantidur();
	void Drawpapantidur2();
	void Drawtidur();
	void DrawColoredPlane();
	void MoveCamera(float speed);
	void StrafeCamera(float speed);
	void RotateCamera(float speed);
	void InitCamera();
};

