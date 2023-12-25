#include "MyShaderLoader.h"

MyShaderLoader::MyShaderLoader(GLuint VSID, GLuint FSID, GLuint GSID)
{
    state = true;
    int result = 0;
    int infoLogLength = 0;

    printf("Linking shaders\n");
    ID = glCreateProgram();

    if(VSID != 0) glAttachShader(ID, VSID);
    if(FSID != 0) glAttachShader(ID, FSID);
    if(GSID != 0) glAttachShader(ID, GSID);

    glLinkProgram(ID);
    glGetProgramiv(ID, GL_LINK_STATUS, &result);
    glGetProgramiv(ID, GL_INFO_LOG_LENGTH, &infoLogLength);

    if (result == GL_FALSE) {
        printf("Programm konnte nicht gelinkt werden\n");
        state = false;
    }

    if (infoLogLength > 0) {
        std::vector<char> progErrorMsg(infoLogLength);
        glGetProgramInfoLog(ID, infoLogLength, NULL, &progErrorMsg[0]);
        printf("%s\n", &progErrorMsg[0]);
    }

}

GLuint MyShaderLoader::getID()
{
    return ID;
}

bool MyShaderLoader::getState()
{
    return state;
}
