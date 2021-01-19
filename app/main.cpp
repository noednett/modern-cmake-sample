#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <cmath>
#include <opengl-utils/Shader.hpp>

void framebuffer_size_callback(GLFWwindow* window, const int width, const int height);
void processInput(GLFWwindow* window);
float colorValueManipulator(float const * const timeValue, float const * const offset);

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

	glViewport(0,0, 800, 500);

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
	
	Shader shader ("../res/shaders/vColorCycleShader.gl", "../res/shaders/fColorCycleShader.gl");

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

		shader.use();

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
