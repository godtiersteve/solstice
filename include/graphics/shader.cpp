#include "shader.h"

namespace solstice {
    Shader::Shader() {
        mText = "";
        mType = SHADER_NONE;
        mShader = 0;
    }
    Shader::Shader(string filename, ShaderType type, bool fromText) {
        LoadShader(filename, type, fromText);
    }
    void Shader::Clear() {
        if(mShader) {
            glDeleteShader(mShader);
        }
        mShader = 0;
        mText = "";
        mType = SHADER_NONE;
    }

    void Shader::SetShaderID(string id)     {mShaderID = id;}
    void Shader::SetText(string t)          {mText = t;}
    void Shader::SetType(ShaderType t)      {mType = t;}
    int Shader::GetShader()                 {return mShader;}
    ShaderType Shader::GetType()            {return mType;}
    string Shader::GetShaderText()          {return mText;}
    string Shader::GetShaderID(string id)   {return mShaderID;}
    void Shader::LoadShader() {
        LoadShader(mText, mType, true);
    }
    void Shader::LoadShader(string filename, bool fromText) {
        LoadShader(filename, mType, fromText);
    }
    void Shader::LoadShader(string filename, ShaderType type, bool fromText) {
        if(glIsShader(mShader)) {
            glDeleteShader(mShader);
        }
        if(!fromText) {
            ifstream in(filename.c_str());
            if(!in) {
                return;
            }
            mText = "";
            string currentLine;
            while(getline(in, currentLine)) {
                mText += currentLine + '\n';
            }
        }
        else if(!filename.empty()) {
            mText = filename;
        }
        else {
            return;
        }
        mType = type;
        mShader = glCreateShader(ShaderTypeToGLEnum(mType));
        const char* fileContents2 = mText.c_str();
        glShaderSource(mShader, 1, &fileContents2, NULL);
        glCompileShader(mShader);

        GLint compiled;
        glGetShaderiv(mShader, GL_COMPILE_STATUS, &compiled);

        if(!compiled) {
            GLsizei len;
            glGetShaderiv(mShader, GL_INFO_LOG_LENGTH, &len );

            GLchar* log = new GLchar[len+1];
            glGetShaderInfoLog(mShader, len, &len, log);
            std::cout << log << std::endl;
            delete[] log;
        }
    }

    void Shader::Attach(GLuint program) {
        if(mShader) {
            glAttachShader(program, mShader);
        }
    }

    void Shader::UseNullProgram() {
        glUseProgram(0);
    }

    GLenum ShaderTypeToGLEnum(ShaderType s) {
        switch(s) {
            case SHADER_VERTEX:
                return GL_VERTEX_SHADER;
                break;
            case SHADER_FRAGMENT:
                return GL_FRAGMENT_SHADER;
                break;
            case SHADER_GEOMETRY:
                return GL_GEOMETRY_SHADER;
                break;
            case SHADER_TESSELATION:
                return GL_TESS_CONTROL_SHADER;
                break;
            default:
                return GL_NONE;
                break;
        }
    }
};
