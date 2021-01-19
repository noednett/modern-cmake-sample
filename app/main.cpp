#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <cmath>
#include <opengl-utils/Shader.hpp>

void framebuffer_size_callback(GLFWwindow* window, const int width, const int height);
void processInput(GLFWwindow* window);
float colorValueManipulator(float const * const timeValue, float const * const offset);

void createShaderProgram(unsigned int * const);

int main()
{
	glfwInit();

	//configuring required version
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	GLFWwindow* window = glfwCreateWindow(500, 800, "ColoryCycleTriangle", NULL, NULL);

	if(window == NULL)
	{
		std::cout << "Failed to create window" << std::endl;
		glfwTerminate();
		return -1;
	}

	//creating the context
	glfwMakeContextCurrent(window);

	//GLAD
	if(!gladLoadGLLoader((GLADloadproc) glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		return -1;
	}

	glViewport(0,0, 500, 800);

	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
	
	//vertices
	float triangleLeft[] = {
		-0.75f, -0.5f, 0.0f, 1.0f, 0.0f, 0.0f,
		-0.25f, -0.5f, 0.0f, 0.0f, 1.0f, 0.0f,
		-0.5f, 0.5f, 0.0f , 0.0f, 0.0f, 1.0f
	};
	
	float triangleRight[] = {
		0.75f, -0.5f, 0.0f, 0.0f, 0.0f, 1.0f,
		0.25f, -0.5f, 0.0f, 0.0f, 1.0f, 0.0f,
		0.5f, 0.5f, 0.0f, 1.0f, 0.0f, 0.0f
	};

	//initializing two seperate VAOs and VBOs just for the sake of it
	unsigned int VAO[2], VBO[2];
	
	glGenVertexArrays(2, VAO);
	glGenBuffers(2, VBO);
	
	//Setting up VBOs
	//Setting up 0
	glBindBuffer(GL_ARRAY_BUFFER, VBO[0]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(triangleLeft), triangleLeft, GL_STATIC_DRAW);

	//Setting up VAOs
	//Setting up 0
	glBindVertexArray(VAO[0]);
	//position in 3d
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6*sizeof(float), (void*) 0); //location 0, position in 3d
	glEnableVertexAttribArray(0);
	//color attribute
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6*sizeof(float), (void*)(sizeof(float) * 3));
	glEnableVertexAttribArray(1);

	//Setting up 1
	glBindBuffer(GL_ARRAY_BUFFER, VBO[1]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(triangleRight), triangleRight, GL_STATIC_DRAW);

	//------------------- this shoudl be looped at some point ---------------------
	
	//Setting up 1
	glBindVertexArray(VAO[1]);
	//position in 3d
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6*sizeof(float), (void*) 0); //location 0, position in 3d
	glEnableVertexAttribArray(0);
	//color attribute
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6*sizeof(float), (void*)(sizeof(float) * 3));
	glEnableVertexAttribArray(1);
	//------------------- ----------------------------------- ---------------------
	
	unsigned int shaderProgram;
	createShaderProgram(&shaderProgram);

	while(!glfwWindowShouldClose(window))
	{
		float time = glfwGetTime();
		float offset = 0;
		float value = colorValueManipulator(&time, &offset);
		
		float newTriangleLeft[] = {
			-0.75f, -0.5f, 0.0f, value, 0.0f, 0.0f,
			-0.25f, -0.5f, 0.0f, 0.0f, value, 0.0f,
			-0.5f, 0.5f, 0.0f , 0.0f, 0.0f, value
		};
	
		float newTriangleRight[] = {
			0.75f, -0.5f, 0.0f, 0.0f, 0.0f, value,
			0.25f, -0.5f, 0.0f, 0.0f, value, 0.0f,
			0.5f, 0.5f, 0.0f, value, 0.0f, 0.0f
		};

		//initializing two seperate VAOs and VBOs just for the sake of it
	
		//Setting up VBOs
		//Setting up 0
		glBindBuffer(GL_ARRAY_BUFFER, VBO[0]);
		glBufferData(GL_ARRAY_BUFFER, sizeof(triangleLeft), newTriangleLeft, GL_DYNAMIC_DRAW);

		//Setting up VAOs
		//Setting up 0
		glBindVertexArray(VAO[0]);
		//position in 3d
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6*sizeof(float), (void*) 0); //location 0, position in 3d
		glEnableVertexAttribArray(0);
		//color attribute
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6*sizeof(float), (void*)(sizeof(float) * 3));
		glEnableVertexAttribArray(1);

		//Setting up 1
		glBindBuffer(GL_ARRAY_BUFFER, VBO[1]);
		glBufferData(GL_ARRAY_BUFFER, sizeof(triangleRight), newTriangleRight, GL_DYNAMIC_DRAW);

		//handling input
		processInput(window);

		glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		glUseProgram(shaderProgram);
		
		glBindVertexArray(VAO[0]);
		glBindBuffer(GL_ARRAY_BUFFER, VBO[0]);

		glDrawArrays(GL_TRIANGLES, 0, 3);

		glBindVertexArray(VAO[1]);
		glBindBuffer(GL_ARRAY_BUFFER, VBO[1]);

		glDrawArrays(GL_TRIANGLES, 0, 3);

		glfwPollEvents();
		glfwSwapBuffers(window);
	}

	glfwTerminate();

	return 0;
}


float colorValueManipulator(float const * const timeValue, float const * const offset)
{
	return (sin(*timeValue + *offset) + 1.0f) / 2.0f;
}

//creates a shaderProgram
void createShaderProgram(unsigned int * const shaderProgram)
{
//creating our vertex shader
	//the shader, very basic, not conduction any processing, just defining to take 3d vectors as input and casting the 4th value, used for perspective distortion, as 1 onto it.
	const char *vertexShaderSource = "#version 330 core\n"
    		"layout (location = 0) in vec3 aPos;\n"
		"layout (location = 1) in vec3 aColor;\n"
		"out vec3 color;"
    		"void main()\n"
    		"{\n"
    		"	gl_Position = vec4(aPos, 1.0);\n"
		"	color = aColor;\n"
    		"}\0";

	unsigned int vertexShader;
	vertexShader = glCreateShader(GL_VERTEX_SHADER);

	//attaching vertexShaderSource to our vertex shader object
	glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
	//compiling our shader
	glCompileShader(vertexShader);

	//checking for compilation error.
	int success;
	char infoLog[512];
	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);

	if(!success)
	{
		glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::VERTEX::COMPLIATION_FAILED\n" << infoLog << std::endl;
	}
	
	//creating our fragment shader, which assigns the pixlecolors to the pixles provided by the 'pixleShader'
	
	const char *fragmentShaderSource = "#version 330 core\n"
		"out vec4 FragColor;\n"
		"in vec3 color;\n"
		"void main()\n"
		"{\n"
		"FragColor = vec4(color, 1.0);\n"
		"}\0";
	
	unsigned int fragmentShader;
	fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);

	//attaching fragmentShaderSource to our vertex shader object
	glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
	//compiling our shader
	glCompileShader(fragmentShader);

	//checking for compilation error.
	glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);

	if(!success)
	{
		glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::FRAGMENT::COMPLIATION_FAILED\n" << infoLog << std::endl;
	}
	
	//voila, we have created the two required shaders.
	

	//creating the shaderProgram
	*shaderProgram = glCreateProgram();

	//attaching our shaders to the program
	glAttachShader(*shaderProgram, vertexShader);
	glAttachShader(*shaderProgram, fragmentShader);

	//linking? whatever that means...
	glLinkProgram(*shaderProgram);

	//error check for good measure:
	glGetProgramiv(*shaderProgram, GL_LINK_STATUS, &success);
	if(!success)
	{
		glGetProgramInfoLog(*shaderProgram,512,NULL,infoLog);
		std::cout << "ERROR::SHADER::PROGRAM_LINK::FAILED\n" << infoLog << std::endl;
	}
	
	//we now dont need those programs anymore, so we delete them
	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);
}

//framebuffer_size_callback
//Handels resizing of window
void framebuffer_size_callback(GLFWwindow* window, const int width, const int height)
{
	glViewport(0,0,width, height);
}

void processInput(GLFWwindow* window)
{
	if(glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window,true);
	
}
