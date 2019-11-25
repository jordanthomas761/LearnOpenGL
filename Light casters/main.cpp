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
unsigned int loadTexture(const char *path);

int SCR_WIDTH = 800;
int SCR_HEIGHT = 600;

Camera camera(glm::vec3(0.0f, 0.0f, 3.0f));
float lastX = 0, lastY = 0;
float deltaTime = 0.0f;	// time between current frame and last frame
float lastFrame = 0.0f;
bool firstMouse = true;

glm::vec3 lightPos(-0.2f, -1.0f, -0.3f);

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

glm::vec3 cubePositions[] = {
	glm::vec3( 0.0f,  0.0f,  0.0f),
	glm::vec3( 2.0f,  5.0f, -15.0f),
	glm::vec3(-1.5f, -2.2f, -2.5f),
	glm::vec3(-3.8f, -2.0f, -12.3f),
	glm::vec3( 2.4f, -0.4f, -3.5f),
	glm::vec3(-1.7f,  3.0f, -7.5f),
	glm::vec3( 1.3f, -2.0f, -2.5f),
	glm::vec3( 1.5f,  2.0f, -2.5f),
	glm::vec3( 1.5f,  0.2f, -1.5f),
	glm::vec3(-1.3f,  1.0f, -1.5f)
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
	unsigned int diffuseMap = loadTexture("container2.png");
	unsigned int specularMap = loadTexture("container2_specular.png");
	
	lightingShader.use();
	lightingShader.setInt("material.diffuse", 0);
	lightingShader.setInt("material.specular", 1);
	
	
	while(!glfwWindowShouldClose(window)){
		// Frame update
		float currentFrame = glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;
		
		// input
		processInput(window);
		
		
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		
		lightingShader.use();
		
		
		lightingShader.setVector3("light.direction",  -0.2f, -1.0f, -0.3f);
		lightingShader.setVector3("viewPos", camera.Position);
		
		
		
		
		lightingShader.setVector3("light.ambient", 0.1f, 0.1f, 0.1f);
		lightingShader.setVector3("light.diffuse", 0.5f, 0.5f, 0.5f);
		lightingShader.setVector3("light.specular", 1.0f, 1.0f, 1.0f);
		
		lightingShader.setFloat("material.shininess", 64.0f);
		
		
		glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH/(float)SCR_HEIGHT, 0.1f, 100.0f);
		glm::mat4 view =  camera.GetViewMatrix();
		lightingShader.setMat4("projection", projection);
		lightingShader.setMat4("view", view);
		
		
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, diffuseMap);
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, specularMap);
		
		glBindVertexArray(cubeVAO);
		for(unsigned int i = 0; i < 10; i++)
		{
			glm::mat4 model = glm::mat4(1.0f);
			model = glm::translate(model, cubePositions[i]);
			float angle = 20.0f * i;
			model = glm::rotate(model, glm::radians(angle), glm::vec3(1.0f, 0.3f, 0.5f));
			lightingShader.setMat4("model", model);

			glDrawArrays(GL_TRIANGLES, 0, 36);
		}
		
		
		
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

unsigned int loadTexture(char const * path)
{
    unsigned int textureID;
    glGenTextures(1, &textureID);

    int width, height, nrComponents;
    unsigned char *data = stbi_load(path, &width, &height, &nrComponents, 0);
    if (data)
    {
        GLenum format;
        if (nrComponents == 1)
            format = GL_RED;
        else if (nrComponents == 3)
            format = GL_RGB;
        else if (nrComponents == 4)
            format = GL_RGBA;

        glBindTexture(GL_TEXTURE_2D, textureID);
        glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        stbi_image_free(data);
    }
    else
    {
        std::cout << "Texture failed to load at path: " << path << std::endl;
        stbi_image_free(data);
    }

    return textureID;
}
