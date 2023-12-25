#include "MyShader.h"

MyShader::MyShader(const char* FilePath, GLenum Type)
{
	state = true;

	ID = glCreateShader(Type);

	int infoLogLength;

	int available = 0;
	std::string Code;
	std::ifstream CodeStream(FilePath, std::ios::in);

	GLint result = GL_FALSE;

	//read in Shader Code linewise
	if (CodeStream.is_open()) {
		
		available = 1;
		std::string line = "";
		while (std::getline(CodeStream, line)) {
			Code += "\n" + line;
		}
		CodeStream.close();
	}
	else {
		printf("Shader %s konnte nicht geladen werden\n", FilePath);
		state = false;
	}

	if (available) {
		
		printf("Shader %s wird kompiliert\n", FilePath);
		
		//Pointer to Code and Length
		char const* CodePointer = Code.c_str();
		const GLint CodeLength = Code.size();
		 
		//Define Shader Code Source and its Length -> Compile
		glShaderSource(ID, 1, &CodePointer, &CodeLength);
		glCompileShader(ID);
		
		//print status 
		glGetShaderiv(ID, GL_COMPILE_STATUS, &result);
		if (result == GL_FALSE) {
			printf("Shader %s konnte nicht kompiliert werden\n", FilePath);
			state = false;
		}

		glGetShaderiv(ID, GL_INFO_LOG_LENGTH, &infoLogLength);
		if (infoLogLength > 0) {
			std::vector<char> ErrorMsg(infoLogLength);
			glGetShaderInfoLog(ID, infoLogLength, NULL, &ErrorMsg[0]);
			printf("%s\n", &ErrorMsg[0]);
		}
	}
	else { state = false; }
}

int MyShader::getID()
{
	return ID;
}

bool MyShader::getState()
{
	return state;
}

void MyShader::del()
{
	glDeleteShader(ID);
}
