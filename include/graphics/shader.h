#ifndef SHADER_H_INCLUDED
#define SHADER_H_INCLUDED

#include <string>
#include <fstream>
#include <iostream>
#include "GL/glew.h"
#include "SDL2/sdl.h"
#include "GL/gl.h"
#include "GL/glext.h"
#include "GL/glu.h"

using std::string;
using std::ifstream;

namespace solstice {
    enum ShaderType {SHADER_VERTEX = 0, SHADER_FRAGMENT, SHADER_GEOMETRY, SHADER_TESSELATION, SHADER_NONE};
    GLenum ShaderTypeToGLEnum(ShaderType s);
    class Shader {
        private:
            string mText;
            string mShaderID;
            ShaderType mType;
            GLuint mShader;
        public:
            Shader();
            Shader(string filename, ShaderType type, bool fromText = false);
            void SetText(string t);
            void SetType(ShaderType t);
            void SetShaderID(string id);
            string GetShaderID(string id);
            int GetShader();
            ShaderType GetType();
            string GetShaderText();
            void LoadShader();
            void LoadShader(string filename, bool fromText = false);
            void LoadShader(string filename, ShaderType type, bool fromText = false);
            void Attach(GLuint program);
            void UseNullProgram();
            void Clear();

    };
};

#endif // SHADER_H_INCLUDED
