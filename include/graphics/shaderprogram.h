#ifndef SHADERPROGRAM_H_INCLUDED
#define SHADERPROGRAM_H_INCLUDED

#include <string>
#include <fstream>
#include <functional>
#include "graphicglobals.h"

#include "shader.h"
#include "matrix.h"
#include "3d/light.h"

using std::string;
using std::ifstream;
namespace solstice {
    enum ShaderMatrix {MATRIX_VIEW = 0, MATRIX_MODEL, MATRIX_PROJECTION};
    enum ShaderProgramType {SHADER_DEFAULT = 0, SHADER_SILHOUETTE, SHADER_CIRCLE, SHADER_CUSTOM};
    enum ShaderUniform {UNIFORM_MAT_PROJECTION, UNIFORM_MAT_MODEL, UNIFORM_MAT_VIEW, UNIFORM_MAT_TEXTRANSFORM, UNIFORM_COLOR, UNIFORM_SAMPLER, UNIFORM_USESTEXTURE};

    class ShaderProgram {
        private:
            static int sDefaultShaderID;
            solstice::Shader mShaders[4];
            GLuint mProgram;
            bool mCompiled;
            int mShaderProgramIDi;
            vector<GLint> mUniformLocations, mUniformLocCache;
            string mShaderProgramIDs;
            ShaderProgramType mType;
        protected:
            void Initialize();
            void LoadShader(const char* filename, ShaderType type);
            void GetDefaultUniformLocations();
        public:
            ShaderProgram();
            ShaderProgram(string vertexShader, string fragmentShader, string geometryShader = "", string tesselationShader = "");
            ShaderProgram(string vertexShader, string fragmentShader, bool link);
            ShaderProgram(const ShaderProgram& sh);
            ShaderProgram(ShaderProgramType sh);
            ShaderProgram& operator=(const ShaderProgram& sh);

            void UseProgram();
            void Bind() {UseProgram();}
            void Unbind() {glUseProgram(0);}

            void Initialize(string vertexShader, string fragmentShader, string geometryShader = "", string tesselationShader = "");
            void LoadAndAttachShaders(string vertexShader, string fragmentShader, string geometryShader = "", string tesselationShader = "");
            void LinkProgram();

            GLuint GetProgram();
            Shader& GetShader(ShaderType t);
            bool Compiled();
            void ClearShaders();
            void Clear();

            void SetProgramID(string id);
            void SetProgramID(int i, string id = "");
            void SetShaderType(ShaderProgramType type);
            string GetProgramID();
            int GetProgramIDInt();
            ShaderProgramType GetShaderType();

            int GetUniformLocation(ShaderUniform uniform);
            int GetUniformLocation(string s);

            int SetUniform1f(string s, float f);
            int SetUniform2f(string s, float f1, float f2);
            int SetUniform3f(string s, float f1, float f2, float f3);
            int SetUniform4f(string s, float f1, float f2, float f3, float f4);
            int SetUniform1i(string s, int i);
            int SetUniform2i(string s, int i, int i2);
            int SetUniform3i(string s, int i, int i2, int i3);
            int SetUniform4i(string s, int i, int i2, int i3, int i4);
            int SetUniform1ui(string s, GLuint i);
            int SetUniform2ui(string s, GLuint i, GLuint i2);
            int SetUniform3ui(string s, GLuint i, GLuint i2, GLuint i3);
            int SetUniform4ui(string s, GLuint i, GLuint i2, GLuint i3, GLuint i4);
            int SetUniform1fv(string s, GLsizei count, const GLfloat* value);
            int SetUniform2fv(string s, GLsizei count, const GLfloat* value);
            int SetUniform3fv(string s, GLsizei count, const GLfloat* value);
            int SetUniform4fv(string s, GLsizei count, const GLfloat* value);
            int SetUniform1iv(string s, GLsizei count, const GLint* value);
            int SetUniform2iv(string s, GLsizei count, const GLint* value);
            int SetUniform3iv(string s, GLsizei count, const GLint* value);
            int SetUniform4iv(string s, GLsizei count, const GLint* value);
            int SetUniform1uiv(string s, GLsizei count, const GLuint* value);
            int SetUniform2uiv(string s, GLsizei count, const GLuint* value);
            int SetUniform3uiv(string s, GLsizei count, const GLuint* value);
            int SetUniform4uiv(string s, GLsizei count, const GLuint* value);
            int SetUniformMatrix2fv(string s, GLsizei count, GLboolean transpose, const GLfloat* value);
            int SetUniformMatrix3fv(string s, GLsizei count, GLboolean transpose, const GLfloat* value);
            int SetUniformMatrix4fv(string s, GLsizei count, GLboolean transpose, const GLfloat* value);
            int SetUniformMatrix2x3fv(string s, GLsizei count, GLboolean transpose, const GLfloat* value);
            int SetUniformMatrix3x2fv(string s, GLsizei count, GLboolean transpose, const GLfloat* value);
            int SetUniformMatrix2x4fv(string s, GLsizei count, GLboolean transpose, const GLfloat* value);
            int SetUniformMatrix3x4fv(string s, GLsizei count, GLboolean transpose, const GLfloat* value);
            int SetUniformMatrix4x3fv(string s, GLsizei count, GLboolean transpose, const GLfloat* value);
            int SetUniform2f(string s, glm::vec2 v);
            int SetUniform3f(string s, glm::vec3 v);
            int SetUniform4f(string s, glm::vec4 v);
            int SetUniform2i(string s, glm::ivec2 v);
            int SetUniform3i(string s, glm::ivec3 v);
            int SetUniform4i(string s, glm::ivec4 v);

            int SetSubroutine(GLenum shaderType, string s);
            int GetSubroutineIndex(GLenum shaderType, string s);
            void SetUniformLight(string s, Light& light);
            void SetUniformLights(string s, std::vector<Light>& lights);
            std::vector<GLint>& GetUniformLocCache() {return mUniformLocCache;}

            std::function<void(void)> SetShaderUniforms;
    };

    ShaderProgram GenerateGenericShader(ShaderProgramType s);
};
#endif // SHADERPROGRAM_H_INCLUDED
