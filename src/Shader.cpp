#include <opengl-utils/Shader.hpp> //dunno how this include will look like

	unsigned int ID;

Shader::Shader(char const * vertexPath, char const * fragmentPath) 
	{
		
		//READ SHADER FROM PATH

		std::string vertexSource, fragmentSource;
		std::fstream vertexFile, fragmentFile;

		//ensure that ifstream objects can throw exceptions
		vertexFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
		fragmentFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);

		try
		{
			//opening files
			vertexFile.open(vertexPath);
			fragmentFile.open(fragmentPath);

			//initializing inStreams (stringstream)
			std::stringstream vertexInStream, fragmentInStream;

			//reading
			vertexInStream << vertexFile.rdbuf();
			fragmentInStream << fragmentFile.rdbuf();

			//closing files
			vertexFile.close();
			fragmentFile.close();

			//convert instream to std::string
			vertexSource = vertexInStream.str();
			fragmentSource = fragmentInStream.str();
	
		}
		catch(std::ifstream::failure e)
		{
			std::cout << "ERROR::SHADER::FILE_NOT_SUCCESSFULLY_READ" << std::endl;
		}

		//converting to c compatible char arrays
		const char * vertexShaderSource = vertexSource.c_str();
		const char * fragmentShaderSource = fragmentSource.c_str();

		//INITIALIZING SHADER PROGRAM

		//initializing shaders from source
		unsigned int vertexShaderID, fragmentShaderID;
	
		vertexShaderID = glCreateShader(GL_VERTEX_SHADER);
		fragmentShaderID = glCreateShader(GL_FRAGMENT_SHADER);

		glShaderSource(vertexShaderID, 1, &vertexShaderSource, NULL);
		glShaderSource(fragmentShaderID, 1, &fragmentShaderSource, NULL);

		//compiling shaders
		compileShader(vertexShaderID, "VERTEX");
		compileShader(fragmentShaderID, "FRAGMENT");

		//initializing program

		ID = glCreateProgram();

		glAttachShader(ID, vertexShaderID);
		glAttachShader(ID, fragmentShaderID);

		glLinkProgram(ID);

		int success;
		char infoLog[512];

		glGetProgramiv(ID, GL_LINK_STATUS, &success);
		if(!success)
		{
			glGetProgramInfoLog(ID,512,NULL,infoLog);
			std::cout << "ERROR::SHADER::PROGRAM_LINK::FAILED\n" << infoLog << std::endl;
		}
	
		//we now dont need those programs anymore, so we delete them
		glDeleteShader(vertexShaderID);
		glDeleteShader(fragmentShaderID);
	}

bool Shader::compileShader(unsigned int shader, char const * shaderName = "DEFAULT")
{
	glCompileShader(shader);

	int success;
	char infoLog[512];
	glGetShaderiv(shader, GL_COMPILE_STATUS, &success);

	if(!success)
	{
		glGetShaderInfoLog(shader, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::" << shaderName << "::COMPILATION::FAILED\n" << infoLog << std::endl;
		return false;
	}

	return true;
}

void Shader::use()
{
	glUseProgram(ID);
}

void Shader::setBool(std::string const &name, bool value) const
{
	glUniform1i(glGetUniformLocation(ID, name.c_str()), (int) value);
}

void Shader::setInt(std::string const &name, int value) const
{
	glUniform1i(glGetUniformLocation(ID, name.c_str()), value);
}

void Shader::setFloat(std::string const &name, float value) const
{
	glUniform1f(glGetUniformLocation(ID, name.c_str()), value);
}
