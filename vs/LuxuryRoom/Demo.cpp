#include "Demo.h"

Demo::Demo() {

}
Demo::~Demo() {
}
void Demo::Init() {
	// build and compile our shader program
	// ------------------------------------
	shaderProgram = BuildShader("vertexShader.vert", "fragmentShader.frag", nullptr);

	BuildColoredCube();
	Buildwindow();
	Buildtidur();
	Buildpapantidur();
	Buildpapantidur2();
	Buildalastv();
	Buildbuffet();
	Buildkacalemari();
	Buildkacalemari2();
	Buildtv();
	Buildwall();
	Buildtop();

	BuildColoredPlane();
	InitCamera();
}
void Demo::DeInit() {
	// optional: de-allocate all resources once they've outlived their purpose:
	// ------------------------------------------------------------------------
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
	glDeleteBuffers(1, &EBO);

	glDeleteVertexArrays(1, &VAO2);
	glDeleteBuffers(1, &VBO2);
	glDeleteBuffers(1, &EBO2);

	glDeleteVertexArrays(1, &kacalemariVAO);
	glDeleteBuffers(1, &kacalemariVBO);
	glDeleteBuffers(1, &kacalemariEBO);

	glDeleteVertexArrays(1, &kacalemari2VAO);
	glDeleteBuffers(1, &kacalemari2VBO);
	glDeleteBuffers(1, &kacalemari2EBO);

	glDeleteVertexArrays(1, &tidurVAO);
	glDeleteBuffers(1, &tidurVBO);
	glDeleteBuffers(1, &tidurEBO);

	glDeleteVertexArrays(1, &jendelaVAO);
	glDeleteBuffers(1, &jendelaVBO);
	glDeleteBuffers(1, &jendelaEBO);
}
// process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
// ---------------------------------------------------------------------------------------------------------
void Demo::ProcessInput(GLFWwindow *window) {
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
		glfwSetWindowShouldClose(window, true);
	}
	
	// zoom camera
	// -----------
	if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_RIGHT) == GLFW_PRESS) {
		if (fovy < 90) {
			fovy += 0.0001f;
		}
	}

	if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS) {
		if (fovy > 0) {
			fovy -= 0.0001f;
		}
	}

	// update camera movement 
	// -------------
	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
		MoveCamera(CAMERA_SPEED);
	}
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
		MoveCamera(-CAMERA_SPEED);
	}

	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
		StrafeCamera(-CAMERA_SPEED);
	}

	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
		StrafeCamera(CAMERA_SPEED);
	}

	// update camera rotation
	// ----------------------
	double mouseX, mouseY;
	double midX = screenWidth/2;
	double midY = screenHeight/2;
	float angleY = 0.0f;
	float angleZ = 0.0f;

	// Get mouse position
	glfwGetCursorPos(window, &mouseX, &mouseY);
	if ((mouseX == midX) && (mouseY == midY)) {
		return;
	}

	// Set mouse position
	glfwSetCursorPos(window, midX, midY);

	// Get the direction from the mouse cursor, set a resonable maneuvering speed
	angleY = (float)((midX - mouseX)) / 1000;
	angleZ = (float)((midY - mouseY)) / 1000;

	// The higher the value is the faster the camera looks around.
	viewCamY += angleZ * 2;

	// limit the rotation around the x-axis
	if ((viewCamY - posCamY) > 8) {
		viewCamY = posCamY + 8;
	}
	if ((viewCamY - posCamY) < -8) {
		viewCamY = posCamY - 8;
	}
	RotateCamera(-angleY);


	

}
void Demo::Update(double deltaTime) {
	
}
void Demo::Render() {
	glViewport(0, 0, this->screenWidth, this->screenHeight);

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

	glEnable(GL_DEPTH_TEST);

	// Pass perspective projection matrix
	glm::mat4 projection = glm::perspective(fovy, (GLfloat)this->screenWidth / (GLfloat)this->screenHeight, 0.1f, 100.0f);
	GLint projLoc = glGetUniformLocation(this->shaderProgram, "projection");
	glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projection));

	// LookAt camera (position, target/direction, up)
	glm::mat4 view = glm::lookAt(glm::vec3(posCamX, posCamY, posCamZ), glm::vec3(viewCamX, viewCamY, viewCamZ), glm::vec3(upCamX, upCamY, upCamZ));
	GLint viewLoc = glGetUniformLocation(this->shaderProgram, "view");
	glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));

	// set lighting attribute
	GLint lightPosLoc = glGetUniformLocation(this->shaderProgram, "lightPos");
	glUniform3f(lightPosLoc, 0, 1, 0);
	GLint viewPosLoc = glGetUniformLocation(this->shaderProgram, "viewPos");
	glUniform3f(viewPosLoc, 0, 2, 3);
	GLint lightColorLoc = glGetUniformLocation(this->shaderProgram, "lightColor");
	glUniform3f(lightColorLoc, 1.0f, 1.0f, 1.0f);

	DrawColoredCube();
	Drawwindow();
	Drawalastv();
	Drawtidur();
	Drawkacalemari();
	Drawkacalemari2();
	Drawpapantidur();
	Drawpapantidur2();
	Drawalastv();
	Drawac();
	Drawtv();
	Drawwall();
	Drawtop();
	DrawColoredPlane();

	glDisable(GL_DEPTH_TEST);
}

void Demo::BuildColoredCube() {
	// load image into texture memory
	// ------------------------------
	// Load and create a texture 
	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	int width, height;
	unsigned char* image = SOIL_load_image("woodbed.png", &width, &height, 0, SOIL_LOAD_RGBA);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image);
	SOIL_free_image_data(image);
	glBindTexture(GL_TEXTURE_2D, 0);

	// set up vertex data (and buffer(s)) and configure vertex attributes
	// ------------------------------------------------------------------
	float vertices[] = {
		// format position, tex coords, normal
		// front
		-2.5,  -0.5, -2.0, 0, 0, 0.0f,  0.0f,  1.0f, // 0
		 0.0,  -0.5, -2.0, 1, 0, 0.0f,  0.0f,  1.0f, // 1
		 0.0,   2.5, -2.0, 1, 1, 0.0f,  0.0f,  1.0f, // 2
		-2.5,   2.5, -2.0, 0, 1, 0.0f,  0.0f,  1.0f, // 3

		// right
		 0.0,  -0.5, -2.0, 0, 0, 1.0f,  0.0f,  0.0f, // 4
		 0.0,  -0.5, -2.9, 1, 0, 1.0f,  0.0f,  0.0f, // 5
		 0.0,   2.5, -2.9, 1, 1, 1.0f,  0.0f,  0.0f, // 6
		 0.0,   2.5, -2.0, 0, 1, 1.0f,  0.0f,  0.0f, // 7

		// back
		-2.5,  -0.5, -2.9, 0, 0,  0.0f,  0.0f,  -1.0f, // 8 
		 0.0,  -0.5, -2.9, 1, 0,  0.0f,  0.0f,  -1.0f, // 9
		 0.0,   2.5, -2.9, 1, 1,  0.0f,  0.0f,  -1.0f, // 10
		-2.5,   2.5, -2.9, 0, 1,  0.0f,  0.0f,  -1.0f, // 11

		// left   4
		-2.5,  -0.5, -2.9, 0, 0, -1.0f,  0.0f,  0.0f, // 12
		-2.5,  -0.5, -2.0, 1, 0, -1.0f,  0.0f,  0.0f, // 13
		-2.5,   2.5, -2.0, 1, 1, -1.0f,  0.0f,  0.0f, // 14
		-2.5,   2.5, -2.9, 0, 1, -1.0f,  0.0f,  0.0f, // 15 

		// upper4				  
		-2.5,   2.5, -2.0, 0, 0,  0.0f,  1.0f,  0.0f, // 16
		 0.0,   2.5, -2.0, 1, 0,  0.0f,  1.0f,  0.0f, // 17
		 0.0,   2.5, -2.9, 1, 1,  0.0f,  1.0f,  0.0f, // 18
		-2.5,   2.5, -2.9, 0, 1,  0.0f,  1.0f,  0.0f, // 19

		// bottom
		-2.5,  -0.5, -2.0, 0, 0, 0.0f,  -1.0f,  0.0f, // 20
		 0.0,  -0.5, -2.0, 1, 0, 0.0f,  -1.0f,  0.0f, // 21
		 0.0,  -0.5, -2.9, 1, 1, 0.0f,  -1.0f,  0.0f, // 22
		-2.5,  -0.5, -2.9, 0, 1, 0.0f,  -1.0f,  0.0f, // 23  
	};



	unsigned int indices[] = {
		0,  1,  2,  0,  2,  3,   // front
		4,  5,  6,  4,  6, 7,   // right
		8,  9,  10, 8,  10, 11,  // back
		12, 14, 13, 12, 15, 14,  // left
		16, 18, 17, 16, 19, 18,  // upper
		20, 22, 21, 20, 23, 22   // bottom
	};

	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);
	// bind the Vertex Array Object first, then bind and set vertex buffer(s), and then configure vertex attributes(s).
	glBindVertexArray(VAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	// define position pointer layout 0
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)(0 * sizeof(GLfloat)));
	glEnableVertexAttribArray(0);

	// define texcoord pointer layout 1
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
	glEnableVertexAttribArray(1);

	// define normal pointer layout 2
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)(5 * sizeof(GLfloat)));
	glEnableVertexAttribArray(2);

	// note that this is allowed, the call to glVertexAttribPointer registered VBO as the vertex attribute's bound vertex buffer object so afterwards we can safely unbind
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	// You can unbind the VAO afterwards so other VAO calls won't accidentally modify this VAO, but this rarely happens. Modifying other
	// VAOs requires a call to glBindVertexArray anyways so we generally don't unbind VAOs (nor VBOs) when it's not directly necessary.
	glBindVertexArray(0);

	// remember: do NOT unbind the EBO while a VAO is active as the bound element buffer object IS stored in the VAO; keep the EBO bound.
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

}
void Demo::Buildwindow() {
	glGenTextures(1, &jendelaTexture);
	glBindTexture(GL_TEXTURE_2D, jendelaTexture);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	int width, height;
	unsigned char* image = SOIL_load_image("window.png", &width, &height, 0, SOIL_LOAD_RGBA);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image);
	SOIL_free_image_data(image);
	glBindTexture(GL_TEXTURE_2D, 0);
	float vertices[] = {
		// format position, tex coords
		// front
		-0.5,  0.3,  3.1, 0, 0, 0.0f,  0.0f,  1.0f, // 0
		 0.5,  0.3,  3.1, 1, 0, 0.0f,  0.0f,  1.0f, // 1
		 0.5,  1.5,  3.1, 1, 1, 0.0f,  0.0f,  1.0f, // 2
		-0.5,  1.5,  3.1, 0, 1, 0.0f,  0.0f,  1.0f, // 3

		// righ
		 0.5,  0.3,  3.1, 0, 0, 1.0f,  0.0f,  0.0f, // 4
		 0.5,  0.3,  2.9, 1, 0, 1.0f,  0.0f,  0.0f, // 5
		 0.5,  1.5,  2.9, 1, 1, 1.0f,  0.0f,  0.0f, // 6
		 0.5,  1.5,  3.1, 0, 1, 1.0f,  0.0f,  0.0f, // 7

		// back
		-0.5,  0.3,  2.9, 0, 0,  0.0f,  0.0f,  -1.0f, // 8 
		 0.5,  0.3,  2.9, 1, 0,  0.0f,  0.0f,  -1.0f, // 9
		 0.5,  1.5,  2.9, 1, 1,  0.0f,  0.0f,  -1.0f, // 10
		-0.5,  1.5,  2.9, 0, 1,  0.0f,  0.0f,  -1.0f, // 11

		// left  4
		-0.5,  0.3,  2.9, 0, 0, -1.0f,  0.0f,  0.0f, // 12
		-0.5,  0.3,  3.1, 1, 0, -1.0f,  0.0f,  0.0f, // 13
		-0.5,  1.5,  3.1, 1, 1, -1.0f,  0.0f,  0.0f, // 14
		-0.5,  1.5,  2.9, 0, 1, -1.0f,  0.0f,  0.0f, // 15

		// uppe4				  
		-0.5,  1.5,  3.1, 0, 0,  0.0f,  1.0f,  0.0f, // 16
		 0.5,  1.5,  3.1, 1, 0,  0.0f,  1.0f,  0.0f, // 17
		 0.5,  1.5,  2.9, 1, 1,  0.0f,  1.0f,  0.0f, // 18
		-0.5,  1.5,  2.9, 0, 1,  0.0f,  1.0f,  0.0f, // 19

		// bottm
		-0.5,  0.3,  3.1, 0, 0, 0.0f,  -1.0f,  0.0f, // 20
		 0.5,  0.3,  3.1, 1, 0, 0.0f,  -1.0f,  0.0f, // 21
		 0.5,  0.3,  2.9, 1, 1, 0.0f,  -1.0f,  0.0f, // 22
		-0.5,  0.3,  2.9, 0, 1, 0.0f,  -1.0f,  0.0f, // 23   
	};



	unsigned int indices[] = {
		0,  1,  2,  0,  2,  3,   // front
		4,  5,  6,  4,  6, 7,   // right
		8,  9,  10, 8,  10, 11,  // back
		12, 14, 13, 12, 15, 14,  // left
		16, 18, 17, 16, 19, 18,  // upper
		20, 22, 21, 20, 23, 22   // bottom
	};

	glGenVertexArrays(1, &jendelaVAO);
	glGenBuffers(1, &jendelaVBO);
	glGenBuffers(1, &jendelaEBO);
	// bind the Vertex Array Object first, then bind and set vertex buffer(s), and then configure vertex attributes(s).
	glBindVertexArray(jendelaVAO);

	glBindBuffer(GL_ARRAY_BUFFER, jendelaVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, jendelaEBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	// define position pointer layout 0
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)(0 * sizeof(GLfloat)));
	glEnableVertexAttribArray(0);

	// define texcoord pointer layout 1
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
	glEnableVertexAttribArray(1);

	// define normal pointer layout 2
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)(5 * sizeof(GLfloat)));
	glEnableVertexAttribArray(2);

	// note that this is allowed, the call to glVertexAttribPointer registered VBO as the vertex attribute's bound vertex buffer object so afterwards we can safely unbind
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	// You can unbind the VAO afterwards so other VAO calls won't accidentally modify this VAO, but this rarely happens. Modifying other
	// VAOs requires a call to glBindVertexArray anyways so we generally don't unbind VAOs (nor VBOs) when it's not directly necessary.
	glBindVertexArray(0);

	// remember: do NOT unbind the EBO while a VAO is active as the bound element buffer object IS stored in the VAO; keep the EBO bound.
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}
void Demo::Buildpapantidur()
{
	glGenTextures(1, &papantidurTexture);
	glBindTexture(GL_TEXTURE_2D, papantidurTexture);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	int width, height;
	unsigned char* image = SOIL_load_image("woodbed.png", &width, &height, 0, SOIL_LOAD_RGBA);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image);
	SOIL_free_image_data(image);
	glBindTexture(GL_TEXTURE_2D, 0);
	float vertices[] = {
		// format position, tex coords
		// front
		 0.9,  -0.5,  -2.7, 0, 0,0.0f,  0.0f,  1.0f, // 0
		 3.0,  -0.5,  -2.7, 1, 0,0.0f,  0.0f,  1.0f, // 1
		 3.0,   0.5,  -2.7, 1, 1,0.0f,  0.0f,  1.0f, // 2
		 0.9,   0.5,  -2.7, 0, 1,0.0f,  0.0f,  1.0f, // 3

		// right
		 3.0,  -0.5, -2.7, 0, 0, 1.0f,  0.0f,  0.0f, // 4
		 3.0,  -0.5, -2.9, 1, 0, 1.0f,  0.0f,  0.0f, // 5
		 3.0,   0.5, -2.9, 1, 1, 1.0f,  0.0f,  0.0f, // 6
		 3.0,   0.5, -2.7, 0, 1, 1.0f,  0.0f,  0.0f, // 7

		// back
		 0.9,  -0.5, -2.9, 0, 0,  0.0f,  0.0f,  -1.0f, // 8 
		 3.0,  -0.5, -2.9, 1, 0,  0.0f,  0.0f,  -1.0f, // 9
		 3.0,   0.5, -2.9, 1, 1,  0.0f,  0.0f,  -1.0f, // 10
		 0.9,   0.5, -2.9, 0, 1,  0.0f,  0.0f,  -1.0f, // 11

		// left   4
		 0.9,  -0.5, -2.9, 0, 0, -1.0f,  0.0f,  0.0f, // 12
		 0.9,  -0.5, -2.7, 1, 0, -1.0f,  0.0f,  0.0f, // 13
		 0.9,   0.5, -2.7, 1, 1, -1.0f,  0.0f,  0.0f, // 14
		 0.9,   0.5, -2.9, 0, 1, -1.0f,  0.0f,  0.0f, // 15

		// upper4				  
		 0.9,   0.5, -2.7, 0, 0,  0.0f,  1.0f,  0.0f, // 16
		 3.0,   0.5, -2.7, 1, 0,  0.0f,  1.0f,  0.0f, // 17
		 3.0,   0.5, -2.9, 1, 1,  0.0f,  1.0f,  0.0f, // 18
		 0.9,   0.5, -2.9, 0, 1,  0.0f,  1.0f,  0.0f, // 19

		// bottom
		 0.9,  -0.5, -2.7, 0, 0, 0.0f,  -1.0f,  0.0f, // 20
		 3.0,  -0.5, -2.7, 1, 0, 0.0f,  -1.0f,  0.0f, // 21
		 3.0,  -0.5, -2.9, 1, 1, 0.0f,  -1.0f,  0.0f, // 22
		 0.9,  -0.5, -2.9, 0, 1, 0.0f,  -1.0f,  0.0f, // 23   
	};



	unsigned int indices[] = {
		0,  1,  2,  0,  2,  3,   // front
		4,  5,  6,  4,  6, 7,   // right
		8,  9,  10, 8,  10, 11,  // back
		12, 14, 13, 12, 15, 14,  // left
		16, 18, 17, 16, 19, 18,  // upper
		20, 22, 21, 20, 23, 22   // bottom
	};

	glGenVertexArrays(1, &papantidurVAO);
	glGenBuffers(1, &papantidurVBO);
	glGenBuffers(1, &papantidurEBO);
	// bind the Vertex Array Object first, then bind and set vertex buffer(s), and then configure vertex attributes(s).
	glBindVertexArray(papantidurVAO);

	glBindBuffer(GL_ARRAY_BUFFER, papantidurVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, papantidurEBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	// define position pointer layout 0
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)(0 * sizeof(GLfloat)));
	glEnableVertexAttribArray(0);

	// define texcoord pointer layout 1
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
	glEnableVertexAttribArray(1);

	// define normal pointer layout 2
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)(5 * sizeof(GLfloat)));
	glEnableVertexAttribArray(2);

	// note that this is allowed, the call to glVertexAttribPointer registered VBO as the vertex attribute's bound vertex buffer object so afterwards we can safely unbind
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	// You can unbind the VAO afterwards so other VAO calls won't accidentally modify this VAO, but this rarely happens. Modifying other
	// VAOs requires a call to glBindVertexArray anyways so we generally don't unbind VAOs (nor VBOs) when it's not directly necessary.
	glBindVertexArray(0);

	// remember: do NOT unbind the EBO while a VAO is active as the bound element buffer object IS stored in the VAO; keep the EBO bound.
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}
void Demo::Buildpapantidur2() {
	glGenTextures(1, &papantidur2Texture);
	glBindTexture(GL_TEXTURE_2D, papantidur2Texture);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	int width, height;
	unsigned char* image = SOIL_load_image("woodbed.png", &width, &height, 0, SOIL_LOAD_RGBA);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image);
	SOIL_free_image_data(image);
	glBindTexture(GL_TEXTURE_2D, 0);
	float vertices[] = {
		// format position, tex coords
		// front
		 0.9,  -0.5,  0.7, 0, 0, 0.0f,  0.0f,  1.0f, // 0
		 3.0,  -0.5,  0.7, 1, 0, 0.0f,  0.0f,  1.0f, // 1
		 3.0,   0.2,  0.7, 1, 1, 0.0f,  0.0f,  1.0f, // 2
		 0.9,   0.2,  0.7, 0, 1, 0.0f,  0.0f,  1.0f, // 3

		// right
		 3.0,  -0.5,  0.7, 0, 0, 1.0f,  0.0f,  0.0f, // 4
		 3.0,  -0.5,  0.5, 1, 0, 1.0f,  0.0f,  0.0f, // 5
		 3.0,   0.2,  0.5, 1, 1, 1.0f,  0.0f,  0.0f, // 6
		 3.0,   0.2,  0.7, 0, 1, 1.0f,  0.0f,  0.0f, // 7

		// back
		 0.9,  -0.5,  0.5, 0, 0,  0.0f,  0.0f,  -1.0f, // 8 
		 3.0,  -0.5,  0.5, 1, 0,  0.0f,  0.0f,  -1.0f, // 9
		 3.0,   0.2,  0.5, 1, 1,  0.0f,  0.0f,  -1.0f, // 10
		 0.9,   0.2,  0.5, 0, 1,  0.0f,  0.0f,  -1.0f, // 11

		// left   4
		 0.9,  -0.5,  0.5, 0, 0, -1.0f,  0.0f,  0.0f, // 12
		 0.9,  -0.5,  0.7, 1, 0, -1.0f,  0.0f,  0.0f, // 13
		 0.9,   0.2,  0.7, 1, 1, -1.0f,  0.0f,  0.0f, // 14
		 0.9,   0.2,  0.5, 0, 1, -1.0f,  0.0f,  0.0f, // 15

		// upper4				  
		 0.9,   0.2,  0.7, 0, 0,  0.0f,  1.0f,  0.0f, // 16
		 3.0,   0.2,  0.7, 1, 0,  0.0f,  1.0f,  0.0f, // 17
		 3.0,   0.2,  0.5, 1, 1,  0.0f,  1.0f,  0.0f, // 18
		 0.9,   0.2,  0.5, 0, 1,  0.0f,  1.0f,  0.0f, // 19

		// bottom
		 0.9,  -0.5,  0.7, 0, 0, 0.0f,  -1.0f,  0.0f, // 20
		 3.0,  -0.5,  0.7, 1, 0, 0.0f,  -1.0f,  0.0f, // 21
		 3.0,  -0.5,  0.5, 1, 1, 0.0f,  -1.0f,  0.0f, // 22
		 0.9,  -0.5,  0.5, 0, 1, 0.0f,  -1.0f,  0.0f, // 23    
	};



	unsigned int indices[] = {
		0,  1,  2,  0,  2,  3,   // front
		4,  5,  6,  4,  6, 7,   // right
		8,  9,  10, 8,  10, 11,  // back
		12, 14, 13, 12, 15, 14,  // left
		16, 18, 17, 16, 19, 18,  // upper
		20, 22, 21, 20, 23, 22   // bottom
	};

	glGenVertexArrays(1, &papantidur2VAO);
	glGenBuffers(1, &papantidur2VBO);
	glGenBuffers(1, &papantidur2EBO);
	// bind the Vertex Array Object first, then bind and set vertex buffer(s), and then configure vertex attributes(s).
	glBindVertexArray(papantidur2VAO);

	glBindBuffer(GL_ARRAY_BUFFER, papantidur2VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, papantidur2EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	// define position pointer layout 0
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)(0 * sizeof(GLfloat)));
	glEnableVertexAttribArray(0);

	// define texcoord pointer layout 1
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
	glEnableVertexAttribArray(1);

	// define normal pointer layout 2
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)(5 * sizeof(GLfloat)));
	glEnableVertexAttribArray(2);

	// note that this is allowed, the call to glVertexAttribPointer registered VBO as the vertex attribute's bound vertex buffer object so afterwards we can safely unbind
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	// You can unbind the VAO afterwards so other VAO calls won't accidentally modify this VAO, but this rarely happens. Modifying other
	// VAOs requires a call to glBindVertexArray anyways so we generally don't unbind VAOs (nor VBOs) when it's not directly necessary.
	glBindVertexArray(0);

	// remember: do NOT unbind the EBO while a VAO is active as the bound element buffer object IS stored in the VAO; keep the EBO bound.
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}
void Demo::Buildtidur()
{
	glGenTextures(1, &tidurTexture);
	glBindTexture(GL_TEXTURE_2D, tidurTexture);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	int width, height;
	unsigned char* image = SOIL_load_image("bedreal.png", &width, &height, 0, SOIL_LOAD_RGBA);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image);
	SOIL_free_image_data(image);
	glBindTexture(GL_TEXTURE_2D, 0);
	float vertices[] = {
		// format position, tex coords
		// front
		 1.0,  -0.5,  0.5, 0, 0, 0.0f,  0.0f,  1.0f, // 0
		 2.9,  -0.5,  0.5, 0, 0, 0.0f,  0.0f,  1.0f, // 1
		 2.9,   0.1,  0.5, 0, 0, 0.0f,  0.0f,  1.0f, // 2
		 1.0,   0.1,  0.5, 0, 0, 0.0f,  0.0f,  1.0f, // 3

		// right
		 2.9,  -0.5,  0.5, 0, 0, 1.0f,  0.0f,  0.0f, // 4
		 2.9,  -0.5, -2.7, 1, 0, 1.0f,  0.0f,  0.0f, // 5
		 2.9,   0.1, -2.7, 1, 1, 1.0f,  0.0f,  0.0f, // 6
		 2.9,   0.1,  0.5, 0, 1, 1.0f,  0.0f,  0.0f, // 7

		// back
		 1.0,  -0.5, -2.7, 0, 0,  0.0f,  0.0f,  -1.0f, // 8 
		 2.9,  -0.5, -2.7, 1, 0,  0.0f,  0.0f,  -1.0f, // 9
		 2.9,   0.1, -2.7, 1, 1,  0.0f,  0.0f,  -1.0f, // 10
		 1.0,   0.1, -2.7, 0, 1,  0.0f,  0.0f,  -1.0f, // 11

		// left   4
		 1.0,  -0.5, -2.7, 0, 1, -1.0f,  0.0f,  0.0f, // 12
		 1.0,  -0.5,  0.5, 0, 0, -1.0f,  0.0f,  0.0f, // 13
		 1.0,   0.1,  0.5, 1, 0, -1.0f,  0.0f,  0.0f, // 14
		 1.0,   0.1, -2.7, 1, 1, -1.0f,  0.0f,  0.0f, // 15

		// upper4				  
		 1.0,   0.1,  0.5, 0, 0,  0.0f,  1.0f,  0.0f, // 16
		 2.9,   0.1,  0.5, 1, 0,  0.0f,  1.0f,  0.0f, // 17
		 2.9,   0.1, -2.7, 1, 1,  0.0f,  1.0f,  0.0f, // 18
		 1.0,   0.1, -2.7, 0, 1,  0.0f,  1.0f,  0.0f, // 19

		// bottom
		 1.0,  -0.5,  0.5, 0, 0, 0.0f,  -1.0f,  0.0f, // 20
		 2.9,  -0.5,  0.5, 1, 0, 0.0f,  -1.0f,  0.0f, // 21
		 2.9,  -0.5, -2.7, 1, 1, 0.0f,  -1.0f,  0.0f, // 22
		 1.0,  -0.5, -2.7, 0, 1, 0.0f,  -1.0f,  0.0f, // 23   
	};



	unsigned int indices[] = {
		0,  1,  2,  0,  2,  3,   // front
		4,  5,  6,  4,  6, 7,   // right
		8,  9,  10, 8,  10, 11,  // back
		12, 14, 13, 12, 15, 14,  // left
		16, 18, 17, 16, 19, 18,  // upper
		20, 22, 21, 20, 23, 22   // bottom
	};

	glGenVertexArrays(1, &tidurVAO);
	glGenBuffers(1, &tidurVBO);
	glGenBuffers(1, &tidurEBO);
	// bind the Vertex Array Object first, then bind and set vertex buffer(s), and then configure vertex attributes(s).
	glBindVertexArray(tidurVAO);

	glBindBuffer(GL_ARRAY_BUFFER, tidurVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, tidurEBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	// define position pointer layout 0
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)(0 * sizeof(GLfloat)));
	glEnableVertexAttribArray(0);

	// define texcoord pointer layout 1
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
	glEnableVertexAttribArray(1);

	// define normal pointer layout 2
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)(5 * sizeof(GLfloat)));
	glEnableVertexAttribArray(2);

	// note that this is allowed, the call to glVertexAttribPointer registered VBO as the vertex attribute's bound vertex buffer object so afterwards we can safely unbind
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	// You can unbind the VAO afterwards so other VAO calls won't accidentally modify this VAO, but this rarely happens. Modifying other
	// VAOs requires a call to glBindVertexArray anyways so we generally don't unbind VAOs (nor VBOs) when it's not directly necessary.
	glBindVertexArray(0);

	// remember: do NOT unbind the EBO while a VAO is active as the bound element buffer object IS stored in the VAO; keep the EBO bound.
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

}
void Demo::Buildbuffet()
{
	glGenTextures(1, &acTexture);
	glBindTexture(GL_TEXTURE_2D, acTexture);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	int width, height;
	unsigned char* image = SOIL_load_image("buffet.png", &width, &height, 0, SOIL_LOAD_RGBA);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image);
	SOIL_free_image_data(image);
	glBindTexture(GL_TEXTURE_2D, 0);
	float vertices[] = {
		// format position, tex coords
		// front
		-2.9,  -0.5,  2.0, 0, 0, 0.0f,  0.0f,  1.0f, // 0
		-2.0,  -0.5,  2.0, 1, 0, 0.0f,  0.0f,  1.0f, // 1
		-2.0,   0.2,  2.0, 1, 1, 0.0f,  0.0f,  1.0f, // 2
		-2.9,   0.2,  2.0, 0, 1, 0.0f,  0.0f,  1.0f, // 3

		// right
		-2.0,  -0.5,  2.0, 0, 0, 1.0f,  0.0f,  0.0f, // 4
		-2.0,  -0.5,  0.0, 1, 0, 1.0f,  0.0f,  0.0f, // 5
		-2.0,   0.2,  0.0, 1, 1, 1.0f,  0.0f,  0.0f, // 6
		-2.0,   0.2,  2.0, 0, 1, 1.0f,  0.0f,  0.0f, // 7

		// back
		-2.9,  -0.5,  0.0, 0, 0,  0.0f,  0.0f,  -1.0f, // 8 
		-2.0,  -0.5,  0.0, 1, 0,  0.0f,  0.0f,  -1.0f, // 9
		-2.0,   0.2,  0.0, 1, 1,  0.0f,  0.0f,  -1.0f, // 10
		-2.9,   0.2,  0.0, 0, 1,  0.0f,  0.0f,  -1.0f, // 11

		// left   4
		-2.9,  -0.5,  0.0, 0, 0, -1.0f,  0.0f,  0.0f, // 12
		-2.9,  -0.5,  2.0, 1, 0, -1.0f,  0.0f,  0.0f, // 13
		-2.9,   0.2,  2.0, 1, 1, -1.0f,  0.0f,  0.0f, // 14
		-2.9,   0.2,  0.0, 0, 1, -1.0f,  0.0f,  0.0f, // 15  

		// upper4				  
		-2.9,   0.2,  2.0, 0, 1,  0.0f,  1.0f,  0.0f, // 16
		-2.0,   0.2,  2.0, 0, 0,  0.0f,  1.0f,  0.0f, // 17
		-2.0,   0.2,  0.0, 1, 0,  0.0f,  1.0f,  0.0f, // 18
		-2.9,   0.2,  0.0, 1, 1,  0.0f,  1.0f,  0.0f, // 19

		// bottom
		-2.9,  -0.5,  2.0, 0, 0, 0.0f,  -1.0f,  0.0f, // 20
		-2.0,  -0.5,  2.0, 1, 0, 0.0f,  -1.0f,  0.0f, // 21
		-2.0,  -0.5,  0.0, 1, 1, 0.0f,  -1.0f,  0.0f, // 22
		-2.9,  -0.5,  0.0, 0, 1, 0.0f,  -1.0f,  0.0f, // 23    
	};



	unsigned int indices[] = {
		0,  1,  2,  0,  2,  3,   // front
		4,  5,  6,  4,  6, 7,   // right
		8,  9,  10, 8,  10, 11,  // back
		12, 14, 13, 12, 15, 14,  // left
		16, 18, 17, 16, 19, 18,  // upper
		20, 22, 21, 20, 23, 22   // bottom
	};

	glGenVertexArrays(1, &acVAO);
	glGenBuffers(1, &acVBO);
	glGenBuffers(1, &acEBO);
	// bind the Vertex Array Object first, then bind and set vertex buffer(s), and then configure vertex attributes(s).
	glBindVertexArray(acVAO);

	glBindBuffer(GL_ARRAY_BUFFER, acVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, acEBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	// define position pointer layout 0
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)(0 * sizeof(GLfloat)));
	glEnableVertexAttribArray(0);

	// define texcoord pointer layout 1
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
	glEnableVertexAttribArray(1);

	// define normal pointer layout 2
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)(5 * sizeof(GLfloat)));
	glEnableVertexAttribArray(2);

	// note that this is allowed, the call to glVertexAttribPointer registered VBO as the vertex attribute's bound vertex buffer object so afterwards we can safely unbind
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	// You can unbind the VAO afterwards so other VAO calls won't accidentally modify this VAO, but this rarely happens. Modifying other
	// VAOs requires a call to glBindVertexArray anyways so we generally don't unbind VAOs (nor VBOs) when it's not directly necessary.
	glBindVertexArray(0);

	// remember: do NOT unbind the EBO while a VAO is active as the bound element buffer object IS stored in the VAO; keep the EBO bound.
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}
void Demo::Buildwall() {
	glGenTextures(1, &wallTexture);
	glBindTexture(GL_TEXTURE_2D, wallTexture);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	int width, height;
	unsigned char* image = SOIL_load_image("wall2.png", &width, &height, 0, SOIL_LOAD_RGBA);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image);
	SOIL_free_image_data(image);
	glBindTexture(GL_TEXTURE_2D, 0);
	float vertices[] = {
		// format position, tex coords
		// front
		-3.0,-0.5,  3.0, 0, 0, 0.0f,  0.0f,  0.0f, // 0
		 3.0,-0.5,  3.0, 1, 0, 0.0f,  0.0f,  0.0f, // 1
		 3.0, 3.5,  3.0, 1, 1, 0.0f,  0.0f,  0.0f, // 2
		-3.0, 3.5,  3.0, 0, 1, 0.0f,  0.0f,  0.0f, // 3

		 3.0,-0.5,  3.0, 0, 0, 0.0f,  0.0f,  0.0f, // 0
		 3.0,-0.5, -3.0, 1, 0, 0.0f,  0.0f,  0.0f, // 1
		 3.0, 3.5, -3.0, 1, 1, 0.0f,  0.0f,  0.0f, // 2
		 3.0, 3.5,  3.0, 0, 1, 0.0f,  0.0f,  0.0f, // 3
							   
		-3.0,-0.5, -3.0, 0, 0, 0.0f,  0.0f,  0.0f, // 0
		 3.0,-0.5, -3.0, 1, 0, 0.0f,  0.0f,  0.0f, // 1
		 3.0, 3.5, -3.0, 1, 1, 0.0f,  0.0f,  0.0f, // 2  
		-3.0, 3.5, -3.0, 0, 1, 0.0f,  0.0f,  0.0f, // 3 
								
		-3.0,-0.5, -3.0, 0, 0, 0.0f,  0.0f,  0.0f, // 0  
		-3.0,-0.5,  3.0, 1, 0, 0.0f,  0.0f,  0.0f, // 1
		-3.0, 3.5,  3.0, 1, 1, 0.0f,  0.0f,  0.0f, // 2
		-3.0, 3.5, -3.0, 0, 1, 0.0f,  0.0f,  0.0f, // 3
							   
							   
							      
		
		
		  
	};



	unsigned int indices[] = {
		0,  1,  2,  0,  2,  3,   // front
		4,  5,  6,  4,  6, 7,   // right
		8,  9,  10, 8,  10, 11,  // back
		12, 14, 13, 12, 15, 14,  // left
	};

	glGenVertexArrays(1, &wallVAO);
	glGenBuffers(1, &wallVBO);
	glGenBuffers(1, &wallEBO);
	// bind the Vertex Array Object first, then bind and set vertex buffer(s), and then configure vertex attributes(s).
	glBindVertexArray(wallVAO);

	glBindBuffer(GL_ARRAY_BUFFER, wallVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, wallEBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	// define position pointer layout 0
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)(0 * sizeof(GLfloat)));
	glEnableVertexAttribArray(0);

	// define texcoord pointer layout 1
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
	glEnableVertexAttribArray(1);

	// define normal pointer layout 2
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)(5 * sizeof(GLfloat)));
	glEnableVertexAttribArray(2);

	// note that this is allowed, the call to glVertexAttribPointer registered VBO as the vertex attribute's bound vertex buffer object so afterwards we can safely unbind
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	// You can unbind the VAO afterwards so other VAO calls won't accidentally modify this VAO, but this rarely happens. Modifying other
	// VAOs requires a call to glBindVertexArray anyways so we generally don't unbind VAOs (nor VBOs) when it's not directly necessary.
	glBindVertexArray(0);

	// remember: do NOT unbind the EBO while a VAO is active as the bound element buffer object IS stored in the VAO; keep the EBO bound.
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}
void Demo::Buildtop() {
	glGenTextures(1, &topTexture);
	glBindTexture(GL_TEXTURE_2D, topTexture);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	int width, height;
	unsigned char* image = SOIL_load_image("wall.png", &width, &height, 0, SOIL_LOAD_RGBA);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image);
	SOIL_free_image_data(image);
	glBindTexture(GL_TEXTURE_2D, 0);
	float vertices[] = {
	-3.0, 3.5,  3.0, 0, 0,  0.0f,  0.0f,  1.0f, // 0
	3.0, 3.5, 3.0, 1, 0,    0.0f,  0.0f,  1.0f, // 1
	3.0, 3.5, -3.0, 1, 1,   0.0f,  0.0f,  1.0f, // 2
	-3.0, 3.5, -3.0, 0, 1,  0.0f,  0.0f,  1.0f, // 3
	};
	unsigned int indices[] = {
		0,  1,  2,  0,  2,  3,   // front
	};

	glGenVertexArrays(1, &topVAO);
	glGenBuffers(1, &topVBO);
	glGenBuffers(1, &topEBO);
	// bind the Vertex Array Object first, then bind and set vertex buffer(s), and then configure vertex attributes(s).
	glBindVertexArray(topVAO);

	glBindBuffer(GL_ARRAY_BUFFER, topVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, topEBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	// define position pointer layout 0
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)(0 * sizeof(GLfloat)));
	glEnableVertexAttribArray(0);

	// define texcoord pointer layout 1
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
	glEnableVertexAttribArray(1);

	// define normal pointer layout 2
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)(5 * sizeof(GLfloat)));
	glEnableVertexAttribArray(2);

	// note that this is allowed, the call to glVertexAttribPointer registered VBO as the vertex attribute's bound vertex buffer object so afterwards we can safely unbind
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	// You can unbind the VAO afterwards so other VAO calls won't accidentally modify this VAO, but this rarely happens. Modifying other
	// VAOs requires a call to glBindVertexArray anyways so we generally don't unbind VAOs (nor VBOs) when it's not directly necessary.
	glBindVertexArray(0);

	// remember: do NOT unbind the EBO while a VAO is active as the bound element buffer object IS stored in the VAO; keep the EBO bound.
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}
void Demo::Buildalastv()
{
	glGenTextures(1, &alastvTexture);
	glBindTexture(GL_TEXTURE_2D, alastvTexture);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	int width, height;
	unsigned char* image = SOIL_load_image("woodbed.png", &width, &height, 0, SOIL_LOAD_RGBA);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image);
	SOIL_free_image_data(image);
	glBindTexture(GL_TEXTURE_2D, 0);
	float vertices[] = {
		// format position, tex coords
		// front
		-2.6,   0.2,  1.1, 0, 0, 0.0f,  0.0f,  1.0f, // 0
		-2.4,   0.2,  1.1, 1, 0, 0.0f,  0.0f,  1.0f, // 1
		-2.4,   0.3,  1.1, 1, 1, 0.0f,  0.0f,  1.0f, // 2
		-2.6,   0.3,  1.1, 0, 1, 0.0f,  0.0f,  1.0f, // 3

		// right
		-2.4,   0.2,  1.1, 0, 0, 1.0f,  0.0f,  0.0f, // 4
		-2.4,   0.2,  0.9, 1, 0, 1.0f,  0.0f,  0.0f, // 5
		-2.4,   0.3,  0.9, 1, 1, 1.0f,  0.0f,  0.0f, // 6
		-2.4,   0.3,  1.1, 0, 1, 1.0f,  0.0f,  0.0f, // 7

		// back
		-2.6,   0.2,  0.9, 0, 0,  0.0f,  0.0f,  -1.0f, // 8 
		-2.4,   0.2,  0.9, 1, 0,  0.0f,  0.0f,  -1.0f, // 9
		-2.4,   0.3,  0.9, 1, 1,  0.0f,  0.0f,  -1.0f, // 10
		-2.6,   0.3,  0.9, 0, 1,  0.0f,  0.0f,  -1.0f, // 11

		// left   4
		-2.6,   0.2,  0.9, 0, 0, -1.0f,  0.0f,  0.0f, // 12
		-2.6,   0.2,  1.1, 1, 0, -1.0f,  0.0f,  0.0f, // 13
		-2.6,   0.3,  1.1, 1, 1, -1.0f,  0.0f,  0.0f, // 14
		-2.6,   0.3,  0.9, 0, 1, -1.0f,  0.0f,  0.0f, // 15 

		// upper4				  
		-2.6,   0.3,  1.1, 0, 0,  0.0f,  1.0f,  0.0f, // 16
		-2.4,   0.3,  1.1, 1, 0,  0.0f,  1.0f,  0.0f, // 17
		-2.4,   0.3,  0.9, 1, 1,  0.0f,  1.0f,  0.0f, // 18
		-2.6,   0.3,  0.9, 0, 1,  0.0f,  1.0f,  0.0f, // 19

		// bottom
		-2.6,   0.2,  1.1, 0, 0, 0.0f,  -1.0f,  0.0f, // 20
		-2.4,   0.2,  1.1, 1, 0, 0.0f,  -1.0f,  0.0f, // 21
		-2.4,   0.2,  0.9, 1, 1, 0.0f,  -1.0f,  0.0f, // 22
		-2.6,   0.2,  0.9, 0, 1, 0.0f,  -1.0f,  0.0f, // 23 
	};



	unsigned int indices[] = {
		0,  1,  2,  0,  2,  3,   // front
		4,  5,  6,  4,  6, 7,   // right
		8,  9,  10, 8,  10, 11,  // back
		12, 14, 13, 12, 15, 14,  // left
		16, 18, 17, 16, 19, 18,  // upper
		20, 22, 21, 20, 23, 22   // bottom
	};

	glGenVertexArrays(1, &alastvVAO);
	glGenBuffers(1, &alastvVBO);
	glGenBuffers(1, &alastvEBO);
	// bind the Vertex Array Object first, then bind and set vertex buffer(s), and then configure vertex attributes(s).
	glBindVertexArray(alastvVAO);

	glBindBuffer(GL_ARRAY_BUFFER, alastvVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, alastvEBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	// define position pointer layout 0
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)(0 * sizeof(GLfloat)));
	glEnableVertexAttribArray(0);

	// define texcoord pointer layout 1
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
	glEnableVertexAttribArray(1);

	// define normal pointer layout 2
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)(5 * sizeof(GLfloat)));
	glEnableVertexAttribArray(2);

	// note that this is allowed, the call to glVertexAttribPointer registered VBO as the vertex attribute's bound vertex buffer object so afterwards we can safely unbind
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	// You can unbind the VAO afterwards so other VAO calls won't accidentally modify this VAO, but this rarely happens. Modifying other
	// VAOs requires a call to glBindVertexArray anyways so we generally don't unbind VAOs (nor VBOs) when it's not directly necessary.
	glBindVertexArray(0);

	// remember: do NOT unbind the EBO while a VAO is active as the bound element buffer object IS stored in the VAO; keep the EBO bound.
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}
void Demo::Buildtv() {
	glGenTextures(1, &tvTexture);
	glBindTexture(GL_TEXTURE_2D, tvTexture);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	int width, height;
	unsigned char* image = SOIL_load_image("net.png", &width, &height, 0, SOIL_LOAD_RGBA);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image);
	SOIL_free_image_data(image);
	glBindTexture(GL_TEXTURE_2D, 0);
	float vertices[] = {
		// format position, tex coords
		// front
		-2.5,   0.3,  2.0, 0, 0, 0.0f,  0.0f,  1.0f, // 0
		-2.4,   0.3,  2.0, 0, 0, 0.0f,  0.0f,  1.0f, // 1
		-2.4,   1.5,  2.0, 0, 0, 0.0f,  0.0f,  1.0f, // 2
		-2.5,   1.5,  2.0, 0, 0, 0.0f,  0.0f,  1.0f, // 3

		// right
		-2.4,   0.3,  2.0, 0, 0, 1.0f,  0.0f,  0.0f, // 4
		-2.4,   0.3,  0.0, 1, 0, 1.0f,  0.0f,  0.0f, // 5
		-2.4,   1.5,  0.0, 1, 1, 1.0f,  0.0f,  0.0f, // 6
		-2.4,   1.5,  2.0, 0, 1, 1.0f,  0.0f,  0.0f, // 7

		// back
		-2.5,   0.3,  0.0, 0, 0,  0.0f,  0.0f,  -1.0f, // 8 
		-2.4,   0.3,  0.0, 0, 0,  0.0f,  0.0f,  -1.0f, // 9
		-2.4,   1.5,  0.0, 0, 0,  0.0f,  0.0f,  -1.0f, // 10
		-2.5,   1.5,  0.0, 0, 0,  0.0f,  0.0f,  -1.0f, // 11

		// left   4
		-2.5,   0.3,  0.0, 0, 0, -1.0f,  0.0f,  0.0f, // 12
		-2.5,   0.3,  2.0, 0, 0, -1.0f,  0.0f,  0.0f, // 13
		-2.5,   1.5,  2.0, 0, 0, -1.0f,  0.0f,  0.0f, // 14
		-2.5,   1.5,  0.0, 0, 0, -1.0f,  0.0f,  0.0f, // 15 

		// upper4				  
		-2.5,   1.5,  2.0, 0, 0,  0.0f,  1.0f,  0.0f, // 16
		-2.4,   1.5,  2.0, 0, 0,  0.0f,  1.0f,  0.0f, // 17
		-2.4,   1.5,  0.0, 0, 0,  0.0f,  1.0f,  0.0f, // 18
		-2.5,   1.5,  0.0, 0, 0,  0.0f,  1.0f,  0.0f, // 19

		// bottom
		-2.5,   0.3,  2.0, 0, 0, 0.0f,  -1.0f,  0.0f, // 20
		-2.4,   0.3,  2.0, 0, 0, 0.0f,  -1.0f,  0.0f, // 21
		-2.4,   0.3,  0.0, 0, 0, 0.0f,  -1.0f,  0.0f, // 22
		-2.5,   0.3,  0.0, 0, 0, 0.0f,  -1.0f,  0.0f, // 23  
	};



	unsigned int indices[] = {
		0,  1,  2,  0,  2,  3,   // front
		4,  5,  6,  4,  6, 7,   // right
		8,  9,  10, 8,  10, 11,  // back
		12, 14, 13, 12, 15, 14,  // left
		16, 18, 17, 16, 19, 18,  // upper
		20, 22, 21, 20, 23, 22   // bottom
	};

	glGenVertexArrays(1, &tvVAO);
	glGenBuffers(1, &tvVBO);
	glGenBuffers(1, &tvEBO);
	// bind the Vertex Array Object first, then bind and set vertex buffer(s), and then configure vertex attributes(s).
	glBindVertexArray(tvVAO);

	glBindBuffer(GL_ARRAY_BUFFER, tvVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, tvEBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	// define position pointer layout 0
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)(0 * sizeof(GLfloat)));
	glEnableVertexAttribArray(0);

	// define texcoord pointer layout 1
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
	glEnableVertexAttribArray(1);

	// define normal pointer layout 2
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)(5 * sizeof(GLfloat)));
	glEnableVertexAttribArray(2);

	// note that this is allowed, the call to glVertexAttribPointer registered VBO as the vertex attribute's bound vertex buffer object so afterwards we can safely unbind
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	// You can unbind the VAO afterwards so other VAO calls won't accidentally modify this VAO, but this rarely happens. Modifying other
	// VAOs requires a call to glBindVertexArray anyways so we generally don't unbind VAOs (nor VBOs) when it's not directly necessary.
	glBindVertexArray(0);

	// remember: do NOT unbind the EBO while a VAO is active as the bound element buffer object IS stored in the VAO; keep the EBO bound.
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}
void Demo::Buildkacalemari2()
{
	// load image into texture memory
	// ------------------------------
	// Load and create a texture 
	glGenTextures(1, &kacalemari2Texture);
	glBindTexture(GL_TEXTURE_2D, kacalemari2Texture);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	int width, height;
	unsigned char* image = SOIL_load_image("mirror.png", &width, &height, 0, SOIL_LOAD_RGBA);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image);
	SOIL_free_image_data(image);
	glBindTexture(GL_TEXTURE_2D, 0);

	float vertices[] = {
		// format position, tex coords
		// front
		
		//kacalemari2
		// format position, tex coords
		// front
		-1.2,  -0.5, -1.9, 0, 0, 0.0f,  0.0f,  1.0f, // 0
		 0.0,  -0.5, -1.9, 1, 0, 0.0f,  0.0f,  1.0f, // 1
		 0.0,   2.5, -1.9, 1, 1, 0.0f,  0.0f,  1.0f, // 2
		-1.2,   2.5, -1.9, 0, 1, 0.0f,  0.0f,  1.0f, // 3

		// right
		 0.0,  -0.5, -1.9, 0, 0, 1.0f,  0.0f,  0.0f, // 4
		 0.0,  -0.5, -2.0, 1, 0, 1.0f,  0.0f,  0.0f, // 5
		 0.0,   2.5, -2.0, 1, 1, 1.0f,  0.0f,  0.0f, // 6
		 0.0,   2.5, -1.9, 0, 1, 1.0f,  0.0f,  0.0f, // 7

		// back
		-1.2,  -0.5, -2.0, 0, 0,  0.0f,  0.0f,  -1.0f, // 8 
		 0.0,  -0.5, -2.0, 1, 0,  0.0f,  0.0f,  -1.0f, // 9
		 0.0,   2.5, -2.0, 1, 1,  0.0f,  0.0f,  -1.0f, // 10
		-1.2,   2.5, -2.0, 0, 1,  0.0f,  0.0f,  -1.0f, // 11

		// left   4
		-1.2,  -0.5, -2.0, 0, 0, -1.0f,  0.0f,  0.0f, // 12
		-1.2,  -0.5, -1.9, 1, 0, -1.0f,  0.0f,  0.0f, // 13
		-1.2,   2.5, -1.9, 1, 1, -1.0f,  0.0f,  0.0f, // 14
		-1.2,   2.5, -2.0, 0, 1, -1.0f,  0.0f,  0.0f, // 15 

		// upper4				  
		-1.2,   2.5, -1.9, 0, 0,  0.0f,  1.0f,  0.0f, // 16
		 0.0,   2.5, -1.9, 1, 0,  0.0f,  1.0f,  0.0f, // 17
		 0.0,   2.5, -2.0, 1, 1,  0.0f,  1.0f,  0.0f, // 18
		-1.2,   2.5, -2.0, 0, 1,  0.0f,  1.0f,  0.0f, // 19

		// bottom
		-1.2,  -0.5, -1.9, 0, 0, 0.0f,  -1.0f,  0.0f, // 20
		 0.0,  -0.5, -1.9, 1, 0, 0.0f,  -1.0f,  0.0f, // 21
		 0.0,  -0.5, -2.0, 1, 1, 0.0f,  -1.0f,  0.0f, // 22
		-1.2,  -0.5, -2.0, 0, 1, 0.0f,  -1.0f,  0.0f, // 23   
	};



	unsigned int indices[] = {
		0,  1,  2,  0,  2,  3,   // front
		4,  5,  6,  4,  6, 7,   // right
		8,  9,  10, 8,  10, 11,  // back
		12, 14, 13, 12, 15, 14,  // left
		16, 18, 17, 16, 19, 18,  // upper
		20, 22, 21, 20, 23, 22,   // bottom

	};

	glGenVertexArrays(1, &kacalemari2VAO);
	glGenBuffers(1, &kacalemari2VBO);
	glGenBuffers(1, &kacalemari2EBO);
	// bind the Vertex Array Object first, then bind and set vertex buffer(s), and then configure vertex attributes(s).
	glBindVertexArray(kacalemari2VAO);

	glBindBuffer(GL_ARRAY_BUFFER, kacalemari2VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, kacalemari2EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	// define position pointer layout 0
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)(0 * sizeof(GLfloat)));
	glEnableVertexAttribArray(0);

	// define texcoord pointer layout 1
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
	glEnableVertexAttribArray(1);

	// define normal pointer layout 2
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)(5 * sizeof(GLfloat)));
	glEnableVertexAttribArray(2);

	// note that this is allowed, the call to glVertexAttribPointer registered VBO as the vertex attribute's bound vertex buffer object so afterwards we can safely unbind
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	// You can unbind the VAO afterwards so other VAO calls won't accidentally modify this VAO, but this rarely happens. Modifying other
	// VAOs requires a call to glBindVertexArray anyways so we generally don't unbind VAOs (nor VBOs) when it's not directly necessary.
	glBindVertexArray(0);

	// remember: do NOT unbind the EBO while a VAO is active as the bound element buffer object IS stored in the VAO; keep the EBO bound.
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}
void Demo::Buildkacalemari()
{
	// load image into texture memory
	// ------------------------------
	// Load and create a texture 
	glGenTextures(1, &kacalemariTexture);
	glBindTexture(GL_TEXTURE_2D, kacalemariTexture);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	int width, height;
	unsigned char* image = SOIL_load_image("mirror.png", &width, &height, 0, SOIL_LOAD_RGBA);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image);
	SOIL_free_image_data(image);
	glBindTexture(GL_TEXTURE_2D, 0);

	float vertices[] = {
		// format position, tex coords
		// front
		-2.5,  -0.5, -1.8, 0, 0, 0.0f,  0.0f,  1.0f, // 0
		-1.2,  -0.5, -1.8, 1, 0, 0.0f,  0.0f,  1.0f, // 1
		-1.2,   2.5, -1.8, 1, 1, 0.0f,  0.0f,  1.0f, // 2
		-2.5,   2.5, -1.8, 0, 1, 0.0f,  0.0f,  1.0f, // 3

		// right
		-1.2,  -0.5, -1.8, 0, 0, 1.0f,  0.0f,  0.0f, // 4
		-1.2,  -0.5, -2.0, 1, 0, 1.0f,  0.0f,  0.0f, // 5
		-1.2,   2.5, -2.0, 1, 1, 1.0f,  0.0f,  0.0f, // 6
		-1.2,   2.5, -1.8, 0, 1, 1.0f,  0.0f,  0.0f, // 7

		// back
		-2.5,  -0.5, -2.0, 0, 0,  0.0f,  0.0f,  -1.0f, // 8 
		-1.2,  -0.5, -2.0, 1, 0,  0.0f,  0.0f,  -1.0f, // 9
		-1.2,   2.5, -2.0, 1, 1,  0.0f,  0.0f,  -1.0f, // 10
		-2.5,   2.5, -2.0, 0, 1,  0.0f,  0.0f,  -1.0f, // 11

		// left   4
		-2.5,  -0.5, -2.0, 0, 0, -1.0f,  0.0f,  0.0f, // 12
		-2.5,  -0.5, -1.8, 1, 0, -1.0f,  0.0f,  0.0f, // 13
		-2.5,   2.5, -1.8, 1, 1, -1.0f,  0.0f,  0.0f, // 14
		-2.5,   2.5, -2.0, 0, 1, -1.0f,  0.0f,  0.0f, // 15 

		// upper4				  
		-2.5,   2.5, -1.8, 0, 0,  0.0f,  1.0f,  0.0f, // 16
		-1.2,   2.5, -1.8, 1, 0,  0.0f,  1.0f,  0.0f, // 17
		-1.2,   2.5, -2.0, 1, 1,  0.0f,  1.0f,  0.0f, // 18
		-2.5,   2.5, -2.0, 0, 1,  0.0f,  1.0f,  0.0f, // 19

		// bottom
		-2.5,  -0.5, -1.8, 0, 0, 0.0f,  -1.0f,  0.0f, // 20
		-1.2,  -0.5, -1.8, 1, 0, 0.0f,  -1.0f,  0.0f, // 21
		-1.2,  -0.5, -2.0, 1, 1, 0.0f,  -1.0f,  0.0f, // 22
		-2.5,  -0.5, -2.0, 0, 1, 0.0f,  -1.0f,  0.0f, // 23   

		
	};



	unsigned int indices[] = {
		0,  1,  2,  0,  2,  3,   // front
		4,  5,  6,  4,  6, 7,   // right
		8,  9,  10, 8,  10, 11,  // back
		12, 14, 13, 12, 15, 14,  // left
		16, 18, 17, 16, 19, 18,  // upper
		20, 22, 21, 20, 23, 22,   // bottom

	};

	glGenVertexArrays(1, &kacalemariVAO);
	glGenBuffers(1, &kacalemariVBO);
	glGenBuffers(1, &kacalemariEBO);
	// bind the Vertex Array Object first, then bind and set vertex buffer(s), and then configure vertex attributes(s).
	glBindVertexArray(kacalemariVAO);

	glBindBuffer(GL_ARRAY_BUFFER, kacalemariVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, kacalemariEBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	// define position pointer layout 0
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)(0 * sizeof(GLfloat)));
	glEnableVertexAttribArray(0);

	// define texcoord pointer layout 1
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
	glEnableVertexAttribArray(1);

	// define normal pointer layout 2
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)(5 * sizeof(GLfloat)));
	glEnableVertexAttribArray(2);

	// note that this is allowed, the call to glVertexAttribPointer registered VBO as the vertex attribute's bound vertex buffer object so afterwards we can safely unbind
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	// You can unbind the VAO afterwards so other VAO calls won't accidentally modify this VAO, but this rarely happens. Modifying other
	// VAOs requires a call to glBindVertexArray anyways so we generally don't unbind VAOs (nor VBOs) when it's not directly necessary.
	glBindVertexArray(0);

	// remember: do NOT unbind the EBO while a VAO is active as the bound element buffer object IS stored in the VAO; keep the EBO bound.
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}
void Demo::BuildColoredPlane()
{
	// Load and create a texture 
	glGenTextures(1, &texture2);
	glBindTexture(GL_TEXTURE_2D, texture2);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	int width, height;
	unsigned char* image = SOIL_load_image("wood.png", &width, &height, 0, SOIL_LOAD_RGBA);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image);
	glGenerateMipmap(GL_TEXTURE_2D);
	SOIL_free_image_data(image);
	glBindTexture(GL_TEXTURE_2D, 0);

	// Build geometry
	GLfloat vertices[] = {
		// format position, tex coords
		// bottom
		-50.0,-0.5, -50.0,  0,  0,  0.0f,  1.0f,  0.0f,
		 50.0,-0.5, -50.0, 50,  0, 0.0f,  1.0f,  0.0f,
		 50.0,-0.5,  50.0, 50, 50, 0.0f,  1.0f,  0.0f,
		-50.0,-0.5,  50.0,  0, 50,  0.0f,  1.0f,  0.0f,


	};

	GLuint indices[] = { 0,  2,  1,  0,  3,  2 };

	glGenVertexArrays(1, &VAO2);
	glGenBuffers(1, &VBO2);
	glGenBuffers(1, &EBO2);

	glBindVertexArray(VAO2);

	glBindBuffer(GL_ARRAY_BUFFER, VBO2);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO2);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	// Position attribute
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), 0);
	glEnableVertexAttribArray(0);
	// TexCoord attribute
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
	glEnableVertexAttribArray(1);
	// Normal attribute
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)(5 * sizeof(GLfloat)));
	glEnableVertexAttribArray(2);

	glBindVertexArray(0); // Unbind VAO
}

void Demo::DrawColoredCube()
{
	glUseProgram(shaderProgram);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, texture);
	glUniform1i(glGetUniformLocation(shaderProgram, "material.diffuse"), 0);

	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, texture);
	glUniform1i(glGetUniformLocation(shaderProgram, "material.specular"), 1);

	GLint shininessMatLoc = glGetUniformLocation(shaderProgram, "material.shininess");
	glUniform1f(shininessMatLoc, 10.0f);

	glBindVertexArray(VAO); // seeing as we only have a single VAO there's no need to bind it every time, but we'll do so to keep things a bit more organized


	glm::mat4 model;
	model = glm::translate(model, glm::vec3(0, 1, 0));

	GLint modelLoc = glGetUniformLocation(this->shaderProgram, "model");
	glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));

	glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);

	glBindTexture(GL_TEXTURE_2D, 0);
	glBindVertexArray(0);
}
void Demo::Drawwindow() {
	glUseProgram(shaderProgram);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, jendelaTexture);
	glUniform1i(glGetUniformLocation(shaderProgram, "material.diffuse"), 0);

	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, jendelaTexture);
	glUniform1i(glGetUniformLocation(shaderProgram, "material.specular"), 1);

	GLint shininessMatLoc = glGetUniformLocation(shaderProgram, "material.shininess");
	glUniform1f(shininessMatLoc, 10.0f);

	glBindVertexArray(jendelaVAO); // seeing as we only have a single VAO there's no need to bind it every time, but we'll do so to keep things a bit more organized


	glm::mat4 model;
	model = glm::translate(model, glm::vec3(0, 1, 0));

	GLint modelLoc = glGetUniformLocation(this->shaderProgram, "model");
	glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));

	glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);

	glBindTexture(GL_TEXTURE_2D, 0);
	glBindVertexArray(0);
}
void Demo::Drawac()
{
	glUseProgram(shaderProgram);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, acTexture);
	glUniform1i(glGetUniformLocation(shaderProgram, "material.diffuse"), 0);

	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, acTexture);
	glUniform1i(glGetUniformLocation(shaderProgram, "material.specular"), 1);

	GLint shininessMatLoc = glGetUniformLocation(shaderProgram, "material.shininess");
	glUniform1f(shininessMatLoc, 10.0f);

	glBindVertexArray(acVAO); // seeing as we only have a single VAO there's no need to bind it every time, but we'll do so to keep things a bit more organized


	glm::mat4 model;
	model = glm::translate(model, glm::vec3(0, 1, 0));

	GLint modelLoc = glGetUniformLocation(this->shaderProgram, "model");
	glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));

	glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);

	glBindTexture(GL_TEXTURE_2D, 0);
	glBindVertexArray(0);
}
void Demo::Drawtv() {
	glUseProgram(shaderProgram);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, tvTexture);
	glUniform1i(glGetUniformLocation(shaderProgram, "material.diffuse"), 0);

	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, tvTexture);
	glUniform1i(glGetUniformLocation(shaderProgram, "material.specular"), 1);

	GLint shininessMatLoc = glGetUniformLocation(shaderProgram, "material.shininess");
	glUniform1f(shininessMatLoc, 10.0f);

	glBindVertexArray(tvVAO); // seeing as we only have a single VAO there's no need to bind it every time, but we'll do so to keep things a bit more organized


	glm::mat4 model;
	model = glm::translate(model, glm::vec3(0, 1, 0));

	GLint modelLoc = glGetUniformLocation(this->shaderProgram, "model");
	glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));

	glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);

	glBindTexture(GL_TEXTURE_2D, 0);
	glBindVertexArray(0);
}
void Demo::Drawpapantidur()
{
	glUseProgram(shaderProgram);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, papantidurTexture);
	glUniform1i(glGetUniformLocation(shaderProgram, "material.diffuse"), 0);

	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, papantidurTexture);
	glUniform1i(glGetUniformLocation(shaderProgram, "material.specular"), 1);

	GLint shininessMatLoc = glGetUniformLocation(shaderProgram, "material.shininess");
	glUniform1f(shininessMatLoc, 10.0f);

	glBindVertexArray(papantidurVAO); // seeing as we only have a single VAO there's no need to bind it every time, but we'll do so to keep things a bit more organized


	glm::mat4 model;
	model = glm::translate(model, glm::vec3(0, 1, 0));

	GLint modelLoc = glGetUniformLocation(this->shaderProgram, "model");
	glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));

	glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);

	glBindTexture(GL_TEXTURE_2D, 0);
	glBindVertexArray(0);
}
void Demo::Drawpapantidur2()
{
	glUseProgram(shaderProgram);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, papantidur2Texture);
	glUniform1i(glGetUniformLocation(shaderProgram, "material.diffuse"), 0);

	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, papantidur2Texture);
	glUniform1i(glGetUniformLocation(shaderProgram, "material.specular"), 1);

	GLint shininessMatLoc = glGetUniformLocation(shaderProgram, "material.shininess");
	glUniform1f(shininessMatLoc, 10.0f);

	glBindVertexArray(papantidur2VAO); // seeing as we only have a single VAO there's no need to bind it every time, but we'll do so to keep things a bit more organized


	glm::mat4 model;
	model = glm::translate(model, glm::vec3(0, 1, 0));

	GLint modelLoc = glGetUniformLocation(this->shaderProgram, "model");
	glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));

	glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);

	glBindTexture(GL_TEXTURE_2D, 0);
	glBindVertexArray(0);
}
void Demo::Drawtidur()
{
	glUseProgram(shaderProgram);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, tidurTexture);
	glUniform1i(glGetUniformLocation(shaderProgram, "material.diffuse"), 0);

	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, tidurTexture);
	glUniform1i(glGetUniformLocation(shaderProgram, "material.specular"), 1);

	GLint shininessMatLoc = glGetUniformLocation(shaderProgram, "material.shininess");
	glUniform1f(shininessMatLoc, 10.0f);

	glBindVertexArray(tidurVAO); // seeing as we only have a single VAO there's no need to bind it every time, but we'll do so to keep things a bit more organized


	glm::mat4 model;
	model = glm::translate(model, glm::vec3(0, 1, 0));

	GLint modelLoc = glGetUniformLocation(this->shaderProgram, "model");
	glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));

	glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);

	glBindTexture(GL_TEXTURE_2D, 0);
	glBindVertexArray(0);
}
void Demo::Drawkacalemari2() {
	glUseProgram(shaderProgram);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, kacalemari2Texture);
	glUniform1i(glGetUniformLocation(shaderProgram, "material.diffuse"), 0);

	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, kacalemari2Texture);
	glUniform1i(glGetUniformLocation(shaderProgram, "material.specular"), 1);

	GLint shininessMatLoc = glGetUniformLocation(shaderProgram, "material.shininess");
	glUniform1f(shininessMatLoc, 10.0f);

	glBindVertexArray(kacalemari2VAO); // seeing as we only have a single VAO there's no need to bind it every time, but we'll do so to keep things a bit more organized


	glm::mat4 model;
	model = glm::translate(model, glm::vec3(0, 1, 0));

	GLint modelLoc = glGetUniformLocation(this->shaderProgram, "model");
	glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));

	glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);

	glBindTexture(GL_TEXTURE_2D, 0);
	glBindVertexArray(0);
}
void Demo::Drawkacalemari() {
	glUseProgram(shaderProgram);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, kacalemariTexture);
	glUniform1i(glGetUniformLocation(shaderProgram, "material.diffuse"), 0);

	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, kacalemariTexture);
	glUniform1i(glGetUniformLocation(shaderProgram, "material.specular"), 1);

	GLint shininessMatLoc = glGetUniformLocation(shaderProgram, "material.shininess");
	glUniform1f(shininessMatLoc, 10.0f);

	glBindVertexArray(kacalemariVAO); // seeing as we only have a single VAO there's no need to bind it every time, but we'll do so to keep things a bit more organized


	glm::mat4 model;
	model = glm::translate(model, glm::vec3(0, 1, 0));

	GLint modelLoc = glGetUniformLocation(this->shaderProgram, "model");
	glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));

	glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);

	glBindTexture(GL_TEXTURE_2D, 0);
	glBindVertexArray(0);
}
void Demo::Drawalastv()
{

	glUseProgram(shaderProgram);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, alastvTexture);
	glUniform1i(glGetUniformLocation(shaderProgram, "material.diffuse"), 0);

	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, alastvTexture);
	glUniform1i(glGetUniformLocation(shaderProgram, "material.specular"), 1);

	GLint shininessMatLoc = glGetUniformLocation(shaderProgram, "material.shininess");
	glUniform1f(shininessMatLoc, 10.0f);

	glBindVertexArray(alastvVAO); // seeing as we only have a single VAO there's no need to bind it every time, but we'll do so to keep things a bit more organized


	glm::mat4 model;
	model = glm::translate(model, glm::vec3(0, 1, 0));

	GLint modelLoc = glGetUniformLocation(this->shaderProgram, "model");
	glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));

	glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);

	glBindTexture(GL_TEXTURE_2D, 0);
	glBindVertexArray(0);
}
void Demo::Drawwall() {
	glUseProgram(shaderProgram);

	glUseProgram(shaderProgram);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, wallTexture);
	glUniform1i(glGetUniformLocation(shaderProgram, "material.diffuse"), 0);

	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, wallTexture);
	glUniform1i(glGetUniformLocation(shaderProgram, "material.specular"), 1);

	GLint shininessMatLoc = glGetUniformLocation(shaderProgram, "material.shininess");
	glUniform1f(shininessMatLoc, 10.0f);

	glBindVertexArray(wallVAO); // seeing as we only have a single VAO there's no need to bind it every time, but we'll do so to keep things a bit more organized


	glm::mat4 model;
	model = glm::translate(model, glm::vec3(0, 1, 0));

	GLint modelLoc = glGetUniformLocation(this->shaderProgram, "model");
	glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));

	glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);

	glBindTexture(GL_TEXTURE_2D, 0);
	glBindVertexArray(0);
}
void Demo::Drawtop() {
	glUseProgram(shaderProgram);

	glUseProgram(shaderProgram);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, topTexture);
	glUniform1i(glGetUniformLocation(shaderProgram, "material.diffuse"), 0);

	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, topTexture);
	glUniform1i(glGetUniformLocation(shaderProgram, "material.specular"), 1);

	GLint shininessMatLoc = glGetUniformLocation(shaderProgram, "material.shininess");
	glUniform1f(shininessMatLoc, 10.0f);

	glBindVertexArray(topVAO); // seeing as we only have a single VAO there's no need to bind it every time, but we'll do so to keep things a bit more organized


	glm::mat4 model;
	model = glm::translate(model, glm::vec3(0, 1, 0));

	GLint modelLoc = glGetUniformLocation(this->shaderProgram, "model");
	glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));

	glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);

	glBindTexture(GL_TEXTURE_2D, 0);
	glBindVertexArray(0);
}
void Demo::DrawColoredPlane()
{
	glUseProgram(shaderProgram);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, texture2);
	glUniform1i(glGetUniformLocation(shaderProgram, "material.diffuse"), 0);

	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, texture2);
	glUniform1i(glGetUniformLocation(shaderProgram, "material.specular"), 1);

	GLint shininessMatLoc = glGetUniformLocation(shaderProgram, "material.shininess");
	glUniform1f(shininessMatLoc, 10.0f);

	glBindVertexArray(VAO2); // seeing as we only have a single VAO there's no need to bind it every time, but we'll do so to keep things a bit more organized


	glm::mat4 model;
	model = glm::translate(model, glm::vec3(0, 1, 0));

	GLint modelLoc = glGetUniformLocation(this->shaderProgram, "model");
	glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));

	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

	glBindTexture(GL_TEXTURE_2D, 0);
	glBindVertexArray(0);
}

void Demo::InitCamera()
{
	posCamX = 0.0f;
	posCamY = 2.0f;
	posCamZ = 8.0f;
	viewCamX = 0.0f;
	viewCamY = 1.0f;
	viewCamZ = 0.0f;
	upCamX = 0.0f;
	upCamY = 1.0f;
	upCamZ = 0.0f;
	CAMERA_SPEED = 0.001f;
	fovy = 45.0f;
	glfwSetInputMode(this->window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
}
void Demo::MoveCamera(float speed)
{
	float x = viewCamX - posCamX;
	float z = viewCamZ - posCamZ;
	// forward positive cameraspeed and backward negative -cameraspeed.
	posCamX = posCamX + x * speed;
	posCamZ = posCamZ + z * speed;
	viewCamX = viewCamX + x * speed;
	viewCamZ = viewCamZ + z * speed;
}
void Demo::StrafeCamera(float speed)
{
	float x = viewCamX - posCamX;
	float z = viewCamZ - posCamZ;
	float orthoX = -z;
	float orthoZ = x;

	// left positive cameraspeed and right negative -cameraspeed.
	posCamX = posCamX + orthoX * speed;
	posCamZ = posCamZ + orthoZ * speed;
	viewCamX = viewCamX + orthoX * speed;
	viewCamZ = viewCamZ + orthoZ * speed;
}
void Demo::RotateCamera(float speed)
{
	float x = viewCamX - posCamX;
	float z = viewCamZ - posCamZ;
	viewCamZ = (float)(posCamZ + glm::sin(speed) * x + glm::cos(speed) * z);
	viewCamX = (float)(posCamX + glm::cos(speed) * x - glm::sin(speed) * z);
}
int main(int argc, char** argv) {
	RenderEngine &app = Demo();
	app.Start("Luxury Room", 800, 600, false, false);
}