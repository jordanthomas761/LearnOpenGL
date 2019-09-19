#include <iostream>
#include <cmath>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "shader.h"
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "camera.h"

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void processInput(GLFWwindow*);

int SCR_WIDTH = 800;
int SCR_HEIGHT = 600;

Camera camera(glm::vec3(0.0f, 0.0f, 3.0f));
float lastX = 0, lastY = 0;
float deltaTime = 0.0f;	// time between current frame and last frame
float lastFrame = 0.0f;
bool firstMouse = true;

glm::vec3 lightPos(1.2f, 0.3f, 2.0f);

float vertices[] = {
	// positions          	// normals           	// texture coords
	-0.5f, -0.5f, -0.5f,  	0.0f,  0.0f, -1.0f,  	0.0f, 0.0f,
	0.5f, -0.5f, -0.5f,  	0.0f,  0.0f, -1.0f,  	1.0f, 0.0f,
	0.5f,  0.5f, -0.5f,  	0.0f,  0.0f, -1.0f,  	1.0f, 1.0f,
	0.5f,  0.5f, -0.5f,  	0.0f,  0.0f, -1.0f,  	1.0f, 1.0f,
	-0.5f,  0.5f, -0.5f,  	0.0f,  0.0f, -1.0f,  	0.0f, 1.0f,
	-0.5f, -0.5f, -0.5f,  	0.0f,  0.0f, -1.0f,  	0.0f, 0.0f,
	
	-0.5f, -0.5f,  0.5f,  	0.0f,  0.0f, 1.0f,   	0.0f, 0.0f,
	0.5f, -0.5f,  0.5f,  	0.0f,  0.0f, 1.0f,   	1.0f, 0.0f,
	0.5f,  0.5f,  0.5f,  	0.0f,  0.0f, 1.0f,   	1.0f, 1.0f,
	0.5f,  0.5f,  0.5f,  	0.0f,  0.0f, 1.0f,   	1.0f, 1.0f,
	-0.5f,  0.5f,  0.5f,  	0.0f,  0.0f, 1.0f,   	0.0f, 1.0f,
	-0.5f, -0.5f,  0.5f,  	0.0f,  0.0f, 1.0f,   	0.0f, 0.0f,
	
	-0.5f,  0.5f,  0.5f, 	-1.0f,  0.0f,  0.0f,  	1.0f, 0.0f,
	-0.5f,  0.5f, -0.5f, 	-1.0f,  0.0f,  0.0f,  	1.0f, 1.0f,
	-0.5f, -0.5f, -0.5f, 	-1.0f,  0.0f,  0.0f,  	0.0f, 1.0f,
	-0.5f, -0.5f, -0.5f, 	-1.0f,  0.0f,  0.0f,  	0.0f, 1.0f,
	-0.5f, -0.5f,  0.5f, 	-1.0f,  0.0f,  0.0f,  	0.0f, 0.0f,
	-0.5f,  0.5f,  0.5f, 	-1.0f,  0.0f,  0.0f,  	1.0f, 0.0f,
	
	0.5f,  0.5f,  0.5f,  	1.0f,  0.0f,  0.0f,  	1.0f, 0.0f,
	0.5f,  0.5f, -0.5f,  	1.0f,  0.0f,  0.0f,  	1.0f, 1.0f,
	0.5f, -0.5f, -0.5f,  	1.0f,  0.0f,  0.0f,  	0.0f, 1.0f,
	0.5f, -0.5f, -0.5f,  	1.0f,  0.0f,  0.0f,  	0.0f, 1.0f,
	0.5f, -0.5f,  0.5f,  	1.0f,  0.0f,  0.0f,  	0.0f, 0.0f,
	0.5f,  0.5f,  0.5f,  	1.0f,  0.0f,  0.0f,  	1.0f, 0.0f,
	
	-0.5f, -0.5f, -0.5f,  	0.0f, -1.0f,  0.0f,  	0.0f, 1.0f,
	0.5f, -0.5f, -0.5f,  	0.0f, -1.0f,  0.0f,  	1.0f, 1.0f,
	0.5f, -0.5f,  0.5f,  	0.0f, -1.0f,  0.0f,  	1.0f, 0.0f,
	0.5f, -0.5f,  0.5f,  	0.0f, -1.0f,  0.0f,  	1.0f, 0.0f,
	-0.5f, -0.5f,  0.5f,  	0.0f, -1.0f,  0.0f,  	0.0f, 0.0f,
	-0.5f, -0.5f, -0.5f,  	0.0f, -1.0f,  0.0f,  	0.0f, 1.0f,
	
	-0.5f,  0.5f, -0.5f,  	0.0f,  1.0f,  0.0f,  	0.0f, 1.0f,
	0.5f,  0.5f, -0.5f,  	0.0f,  1.0f,  0.0f,  	1.0f, 1.0f,
	0.5f,  0.5f,  0.5f,  	0.0f,  1.0f,  0.0f,  	1.0f, 0.0f,
	0.5f,  0.5f,  0.5f,  	0.0f,  1.0f,  0.0f,  	1.0f, 0.0f,
	-0.5f,  0.5f,  0.5f,  	0.0f,  1.0f,  0.0f,  	0.0f, 0.0f,
	-0.5f,  0.5f, -0.5f,  	0.0f,  1.0f,  0.0f,  	0.0f, 1.0f
};

int main(int argc, const char * argv[]) {
	
	if(!glfwInit()){
		std::cout <<"GLFW did not initalize" << std::endl;
		return -1;
	}
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
	
	GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "LearnOpenGL: LightMaps", NULL, NULL);
	if (window == NULL) {
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);
	
	if(!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)){
		std::cout << "Filed to load GLAD" << std::endl;
		return -1;
	}
	
	glfwGetFramebufferSize(window, &SCR_WIDTH, &SCR_HEIGHT);
	glViewport(0, 0, SCR_WIDTH, SCR_HEIGHT);
	
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
	glfwSetCursorPosCallback(window, mouse_callback);
	glfwSetScrollCallback(window, scroll_callback);
	
	// tell GLFW to capture our mouse
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	
	
	Shader lightingShader("VertexShader.vsh", "FragmentShader.fsh");
	Shader lampShader("VertexShader.vsh", "Lamp.fsh");
	
	glEnable(GL_DEPTH_TEST);
	
	unsigned int cubeVAO, VBO;
	glGenVertexArrays(1, &cubeVAO);
	glGenBuffers(1, &VBO);
	
	glBindVertexArray(cubeVAO);
	
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	
	//position attribute
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3*sizeof(float)));
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6*sizeof(float)));
	glEnableVertexAttribArray(2);
	
	unsigned int lightVAO;
	glGenVertexArrays(1, &lightVAO);
	glBindVertexArray(lightVAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	
	//load texture
	unsigned int texture1;
	glGenTextures(1, &texture1);
	glBindTexture(GL_TEXTURE_2D, texture1);
	// Set the texture and filtering options
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	
	int width, height, nrChannels;
	unsigned char *data = stbi_load("container2.png", &width, &height, &nrChannels, 0);
	if (data){
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
	} else {
		std::cout << "Failed to load texture" << std::endl;
	}
	
	stbi_image_free(data);
	
	unsigned int specularMap;
	glGenTextures(1, &specularMap);
	glBindTexture(GL_TEXTURE_2D, specularMap);
	// Set the texture and filtering options
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	
	data = stbi_load("container2_specular.png", &width, &height, &nrChannels, 0);
	if (data){
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
	} else {
		std::cout << "Failed to load texture" << std::endl;
	}
	
	stbi_image_free(data);
	
	
	while(!glfwWindowShouldClose(window)){
		// Frame update
		float currentFrame = glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;
		
		// input
		processInput(window);
		
		lightingShader.use();
		lightingShader.setVector3("objectColor", 1.0f, 1.0f, 1.0f);
		
		
		lightingShader.setVector3("lightPos", lightPos);
		
		
		
		
		lightingShader.setVector3("light.ambient", 0.1f, 0.1f, 0.1f);
		lightingShader.setVector3("light.diffuse", 0.5f, 0.5f, 0.5f);
		lightingShader.setVector3("light.specular", 1.0f, 1.0f, 1.0f);
		
		lightingShader.setInt("material.diffuse", 0);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, texture1);
		lightingShader.setInt("material.specular", 1);
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, specularMap);
		lightingShader.setFloat("material.shininess", 64.0f);
		
		
		glm::mat4 projection;
		projection = glm::perspective(glm::radians(45.0f), (float)SCR_WIDTH/(float)SCR_HEIGHT, 0.1f, 100.0f);
		lightingShader.setMat4("projection", projection);
		
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		
		
		lightPos.x = sin(glfwGetTime()) * 2.0f;
		//lightPos.y = sin(glfwGetTime() / 2.0f) * 1.0f;
		lightPos.z = cos(glfwGetTime()) * 2.0f;
		
		glm::mat4 view;
		view =  camera.GetViewMatrix();
		lightingShader.setMat4("view", view);
		
		glm::mat4 model;
		//model = glm::rotate(model, (float)glfwGetTime() * glm::radians(45.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		lightingShader.setMat4("model", model);
		
		
		glBindVertexArray(cubeVAO);
		glDrawArrays(GL_TRIANGLES, 0, 36);
		
		
		lampShader.use();
		model = glm::mat4();
		model = glm::translate(model, lightPos);
		model = glm::scale(model, glm::vec3(0.2f));
		
		lampShader.setMat4("projection", projection);
		lampShader.setMat4("view", view);
		lampShader.setMat4("model", model);
		
		glBindVertexArray(lightVAO);
		glDrawArrays(GL_TRIANGLES, 0, 36);
		
		
		
		// check and call events and swap the buffers
		glfwSwapBuffers(window);
		glfwPollEvents();
	}
	
	glDeleteVertexArrays(1, &cubeVAO);
	glDeleteVertexArrays(1, &lightVAO);
	glDeleteBuffers(1, &VBO);
	
	glfwTerminate();
	return 0;
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height){
	glViewport(0, 0, width, height);
}

void processInput(GLFWwindow *window)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);
	
	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
		camera.ProcessKeyboard(FORWARD, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
		camera.ProcessKeyboard(BACKWARD, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
		camera.ProcessKeyboard(LEFT, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
		camera.ProcessKeyboard(RIGHT, deltaTime);
}

void mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
	if (firstMouse)
	{
		lastX = xpos;
		lastY = ypos;
		firstMouse = false;
	}
	
	float xoffset = xpos - lastX;
	float yoffset = lastY - ypos; // reversed since y-coordinates go from bottom to top
	
	lastX = xpos;
	lastY = ypos;
	
	camera.ProcessMouseMovement(xoffset, yoffset);
}

void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
	camera.ProcessMouseScroll(yoffset);
}

