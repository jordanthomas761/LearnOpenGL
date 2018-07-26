//
//  Exercise 3.cpp
//  Exercise 3
//
//  Created by Jordan Thomas on 1/19/18.
//  Copyright © 2018 Jordan Thomas. All rights reserved.
//


#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow*);

const char *vertexShaderSource = "#version 330 core\n"
"layout (location = 0) in vec3 aPos;\n"
"void main()\n"
"{\n"
"   gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
"}\0";

const char *orangeFragmentShaderSource = "#version 330 core\n"
"out vec4 FragColor;\n"
"void main()\n"
"{\n"
"   FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);\n"
"}\0";

const char *yellowFragmentShaderSource = "#version 330 core\n"
"out vec4 FragColor;\n"
"void main()\n"
"{\n"
"   FragColor = vec4(1.0f, 1.0f, 0.0f, 1.0f);\n"
"}\0";


int main(int argc, const char * argv[]) {
    if(!glfwInit()){
        std::cout << "GLFW did not initalize" << std::endl;
        return -1;
    }
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    
    GLFWwindow* window = glfwCreateWindow(800, 600, "LearnOpenGL", NULL, NULL);
    if (window == NULL) {
        std::cout << "Filed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    
    

    
    if(!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)){
        std::cout << "Filed to create GLFW window" << std::endl;
        return -1;
    }
    glViewport(0, 0, 800, 600);
    
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    // print out some info about the graphics drivers
    std::cout << "OpenGL version: " << glGetString(GL_VERSION) << std::endl;
    std::cout << "GLSL version: " << glGetString(GL_SHADING_LANGUAGE_VERSION) << std::endl;
    std::cout << "Vendor: " << glGetString(GL_VENDOR) << std::endl;
    std::cout << "Renderer: " << glGetString(GL_RENDERER) << std::endl;
    
    
    //Vertex Shader Creation and Compile
    unsigned int vertexShader;
    vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
    glCompileShader(vertexShader);
    
    //Vertex Shader Compile Error
    int success;
    char infolog[512];
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
    if(!success){
        glGetShaderInfoLog(vertexShader, 512, NULL, infolog);
        std::cout << "ERROR::SHADER::VERTEX::COMILATION_FAILED\n" << infolog << std::endl;
    }
    
    //Fragment Shader Creation and Compile
    unsigned int orangeFragmentShader, yellowFragmentShader;
    orangeFragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(orangeFragmentShader, 1, &orangeFragmentShaderSource, NULL);
    glCompileShader(orangeFragmentShader);
    yellowFragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(yellowFragmentShader, 1, &yellowFragmentShaderSource, NULL);
    glCompileShader(yellowFragmentShader);
    
    //Fragment Shader Compile Error
    glGetShaderiv(orangeFragmentShader, GL_COMPILE_STATUS, &success);
    if(!success){
        glGetShaderInfoLog(orangeFragmentShader, 512, NULL, infolog);
        std::cout << "ERROR::SHADER::Frament::COMILATION_FAILED\n" << infolog << std::endl;
    }
    glGetShaderiv(yellowFragmentShader, GL_COMPILE_STATUS, &success);
    if(!success){
        glGetShaderInfoLog(yellowFragmentShader, 512, NULL, infolog);
        std::cout << "ERROR::SHADER::Frament::COMILATION_FAILED\n" << infolog << std::endl;
    }
    
    unsigned int shaderProgramOrange, shaderProgramYellow;
    shaderProgramOrange = glCreateProgram();
    shaderProgramYellow = glCreateProgram();
    
    // initialize Orange shader program
    glAttachShader(shaderProgramOrange, vertexShader);
    glAttachShader(shaderProgramOrange, orangeFragmentShader);
    glLinkProgram(shaderProgramOrange);
    
    // Program Linking error
    glGetProgramiv(shaderProgramOrange, GL_LINK_STATUS, &success);
    if(!success){
        glGetShaderInfoLog(shaderProgramOrange, 512, NULL, infolog);
        std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infolog << std::endl;
    }
    
    // initialize yellow shader program
    glAttachShader(shaderProgramYellow, vertexShader);
    glAttachShader(shaderProgramYellow, yellowFragmentShader);
    glLinkProgram(shaderProgramYellow);
    
    // Program Linking error
    glGetProgramiv(shaderProgramYellow, GL_LINK_STATUS, &success);
    if(!success){
        glGetShaderInfoLog(shaderProgramYellow, 512, NULL, infolog);
        std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infolog << std::endl;
    }

    
    float firstTriangle[] = {
        -0.5f, 0.5f, 0.0f,   // top
        -1.0f, -0.5f, 0.0f, //bottom left
        0.0f, -0.5f, 0.0f,   // bottom right
    };
    
    float secondTriangle[] = {
        0.5f, 0.5f, 0.0f,   // top
        0.0f, -0.5f, 0.0f, //bottom left
        1.0f, -0.5f, 0.0f   // bottom right
    };
    
    glDeleteShader(vertexShader);
    glDeleteShader(orangeFragmentShader);
    
    // Create VAO, and  VBO first triangle
    unsigned int VBO[2], VAO[2];
    glGenBuffers(2, VBO);
    glGenVertexArrays(2, VAO);
    
    glBindVertexArray(VAO[0]);
    
    glBindBuffer(GL_ARRAY_BUFFER, VBO[0]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(firstTriangle), firstTriangle, GL_STATIC_DRAW);
    
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    
    // Create VAO, and  VBO first triangle
    
    glBindVertexArray(VAO[1]);
    
    glBindBuffer(GL_ARRAY_BUFFER, VBO[1]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(secondTriangle), secondTriangle, GL_STATIC_DRAW);
    
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    
    
    
    
    // Render Loop
    while(!glfwWindowShouldClose(window)){
        // input
        processInput(window);
        
        // render commands here
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
        
        glUseProgram(shaderProgramOrange);
        glBindVertexArray(VAO[0]);
        glDrawArrays(GL_TRIANGLES, 0, 3);
        glUseProgram(shaderProgramYellow);
        glBindVertexArray(VAO[1]);
        glDrawArrays(GL_TRIANGLES, 0, 3);
        glBindVertexArray(0);
        
        // check and call events and swap the buffers
        glfwSwapBuffers(window);
        glfwWaitEvents();
    }
    
    glfwTerminate();
    return 0;
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height){
    glViewport(0, 0, width, height);
}

void processInput(GLFWwindow* window){
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
        glfwSetWindowShouldClose(window, true);
    }
    if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS){
        GLint data;
        glGetIntegerv(GL_POLYGON_MODE, &data);
        
        if( data == GL_FILL){
            glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
        } else {
            glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
        }
    }
}

