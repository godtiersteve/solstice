#include "shaderprogram.h"

namespace solstice {
    int ShaderProgram::sDefaultShaderID = 0;
    ShaderProgram::ShaderProgram() {
        mCompiled = false;
        SetShaderUniforms = NULL;
        mProgram = 0;
        mType = SHADER_DEFAULT;
    }
    ShaderProgram::ShaderProgram(string vertexShader, string fragmentShader, string geometryShader, string tesselationShader) {
        mCompiled = false;
        SetShaderUniforms = NULL;
        mType = SHADER_DEFAULT;
        Initialize(vertexShader, fragmentShader, geometryShader, tesselationShader);
        mShaderProgramIDi = sDefaultShaderID++;
    }
    ShaderProgram::ShaderProgram(ShaderProgramType sh) {
        mCompiled = false;
        SetShaderUniforms = NULL;
        mType = SHADER_DEFAULT;
        *this = GenerateGenericShader(sh);
        mShaderProgramIDi = sDefaultShaderID++;
    }
    ShaderProgram::ShaderProgram(string vertexShader, string fragmentShader, bool link) {
        if(link) {
            Initialize(vertexShader, fragmentShader);
        }
        else {
            LoadAndAttachShaders(vertexShader, fragmentShader);
        }
    }
    ShaderProgram::ShaderProgram(const ShaderProgram& sh) {
        *this = sh;
    }
    ShaderProgram& ShaderProgram::operator=(const ShaderProgram& sh) {
        if(this != &sh) {
            mShaders[SHADER_VERTEX] = sh.mShaders[SHADER_VERTEX];
            mShaders[SHADER_FRAGMENT] = sh.mShaders[SHADER_FRAGMENT];
            mShaders[SHADER_GEOMETRY] = sh.mShaders[SHADER_GEOMETRY];
            mShaders[SHADER_TESSELATION] = sh.mShaders[SHADER_TESSELATION];
            mProgram = sh.mProgram;
            mShaderProgramIDi = sh.mShaderProgramIDi;
            mShaderProgramIDs = sh.mShaderProgramIDs;
            mUniformLocations = sh.mUniformLocations;
            mType = sh.mType;
            mCompiled = sh.mCompiled;
        }
        return *this;
    }
    void ShaderProgram::Initialize(string vertexShader, string fragmentShader, string geometryShader, string tesselationShader) {
        LoadAndAttachShaders(vertexShader, fragmentShader, geometryShader, tesselationShader);
        LinkProgram();
    }
    void ShaderProgram::Initialize() {
        mProgram = glCreateProgram();
        for(int i = 0; i < 4; i++) {
            mShaders[i].Attach(mProgram);
        }
        glLinkProgram(mProgram);
        GLint linked;
        glGetProgramiv(mProgram, GL_LINK_STATUS, &linked);
        if(linked == GL_TRUE) {
            mCompiled = true;
            GetDefaultUniformLocations();
        }
        else {
            GLsizei len;
            glGetProgramiv(mProgram, GL_INFO_LOG_LENGTH, &len);
            GLchar* str = new GLchar[len + 1];
            glGetProgramInfoLog(mProgram, len, &len, str);
            std::cout << str << std::endl;
        }
    }
    void ShaderProgram::LoadAndAttachShaders(string vertexShader, string fragmentShader, string geometryShader, string tesselationShader) {
        mCompiled = false;
        ClearShaders();
        LoadShader(vertexShader.c_str(), SHADER_VERTEX);
        LoadShader(fragmentShader.c_str(), SHADER_FRAGMENT);
        if(!geometryShader.empty()) {
            LoadShader(geometryShader.c_str(), SHADER_GEOMETRY);
        }
        if(!tesselationShader.empty()) {
            LoadShader(tesselationShader.c_str(),  SHADER_TESSELATION);
        }
        SetShaderUniforms = NULL;
        mProgram = glCreateProgram();
        for(int i = 0; i < 4; i++) {
            mShaders[i].Attach(mProgram);
        }
    }
    void ShaderProgram::LinkProgram() {
        glLinkProgram(mProgram);
        GLint linked;
        glGetProgramiv(mProgram, GL_LINK_STATUS, &linked);
        if(linked == GL_TRUE) {
            mCompiled = true;
            GetDefaultUniformLocations();
        }
        else {
            GLsizei len;
            glGetProgramiv(mProgram, GL_INFO_LOG_LENGTH, &len);
            GLchar* str = new GLchar[len + 1];
            glGetProgramInfoLog(mProgram, len, &len, str);
            std::cout << str << std::endl;
        }
    }
    void ShaderProgram::GetDefaultUniformLocations() {
        mUniformLocations.clear();
        for(int i = 0; i < 8; i++) {
            mUniformLocations.push_back(-1);
        }
        mUniformLocations[UNIFORM_MAT_PROJECTION] = glGetUniformLocation(mProgram, "projection");
        mUniformLocations[UNIFORM_MAT_MODEL] = glGetUniformLocation(mProgram, "model");
        mUniformLocations[UNIFORM_MAT_VIEW] = glGetUniformLocation(mProgram, "view");
        mUniformLocations[UNIFORM_MAT_TEXTRANSFORM] = glGetUniformLocation(mProgram, "tex_mat");
        mUniformLocations[UNIFORM_COLOR] = glGetUniformLocation(mProgram, "color");
        mUniformLocations[UNIFORM_SAMPLER] = glGetUniformLocation(mProgram, "sampler");
        mUniformLocations[UNIFORM_USESTEXTURE] = glGetUniformLocation(mProgram, "usesTexture");
    }

    void ShaderProgram::Clear() {
        for(int i = 0; i < 4; i++) {
                mShaders[i].Clear();
        }
        glDeleteProgram(mProgram);
        mCompiled = false;
        SetShaderUniforms = NULL;
    }
    void ShaderProgram::ClearShaders() {
        for(int i = 0; i < 4; i++) {
            mShaders[i].Clear();
        }
    }
    void ShaderProgram::UseProgram() {
        glUseProgram(mProgram);
    }

    void ShaderProgram::LoadShader(const char* filename, ShaderType type) {
        mShaders[type].LoadShader(filename, type);
    }

    GLuint ShaderProgram::GetProgram()                          {return mProgram;}
    Shader& ShaderProgram::GetShader(ShaderType t)              {return mShaders[t];}
    bool ShaderProgram::Compiled()                              {return mCompiled;}
    ShaderProgramType ShaderProgram::GetShaderType()            {return mType;}
    void ShaderProgram::SetShaderType(ShaderProgramType type)   {mType = type;}

    void ShaderProgram::SetProgramID(string id)             {mShaderProgramIDs = id;}
    void ShaderProgram::SetProgramID(int i, string id) {
        mShaderProgramIDi = i;
        if(mShaderProgramIDs.empty() || !id.empty()) {
            mShaderProgramIDs = id;
        }
    }

    string ShaderProgram::GetProgramID()                            {return mShaderProgramIDs;}
    int ShaderProgram::GetProgramIDInt()                            {return mShaderProgramIDi;}
    int ShaderProgram::GetUniformLocation(ShaderUniform uniform)    {
        return mUniformLocations[uniform];
    }
    int ShaderProgram::GetUniformLocation(string s) {
        return glGetUniformLocation(mProgram, s.c_str());
    }
    int ShaderProgram::SetUniform1f(string s, float f) {
        int loc = GetUniformLocation(s);
        glUniform1f(loc, f);
        return loc;
    }
    int ShaderProgram::SetUniform2f(string s, float f1, float f2) {
        int loc = GetUniformLocation(s);
		glUniform2f(loc, f1, f2);
		return loc;
    }
    int ShaderProgram::SetUniform3f(string s, float f1, float f2, float f3) {
        int loc = GetUniformLocation(s);
		glUniform3f(loc, f1, f2, f3);
		return loc;
    }
    int ShaderProgram::SetUniform4f(string s, float f1, float f2, float f3, float f4) {
        int loc = GetUniformLocation(s);
		glUniform4f(loc, f1, f2, f3, f4);
		return loc;
    }
    int ShaderProgram::SetUniform2f(string s, glm::vec2 v) {
        int loc = GetUniformLocation(s);
        glUniform2f(loc, v.x, v.y);
        return loc;
    }
    int ShaderProgram::SetUniform3f(string s, glm::vec3 v) {
        int loc = GetUniformLocation(s);
        glUniform3f(loc, v.x, v.y, v.z);
        return loc;
    }
    int ShaderProgram:: SetUniform4f(string s, glm::vec4 v) {
        int loc = GetUniformLocation(s);
        glUniform4f(loc, v.x, v.y, v.z, v.w);
        return loc;
    }
    int ShaderProgram::SetUniform1i(string s, int i) {
        int loc = GetUniformLocation(s);
		glUniform1i(loc, i);
		return loc;
    }
    int ShaderProgram::SetUniform2i(string s, int i, int i2) {
        int loc = GetUniformLocation(s);
		glUniform2i(loc, i, i2);
		return loc;
    }
    int ShaderProgram::SetUniform3i(string s, int i, int i2, int i3) {
        int loc = GetUniformLocation(s);
		glUniform3i(loc, i, i2, i3);
		return loc;
    }
    int ShaderProgram::SetUniform4i(string s, int i, int i2, int i3, int i4) {
        int loc = GetUniformLocation(s);
		glUniform4i(loc, i, i2, i3, i4);
		return loc;
    }
    int ShaderProgram::SetUniform2i(string s, glm::ivec2 v) {
        int loc = GetUniformLocation(s);
		glUniform2i(loc, v.x, v.y);
		return loc;
    }
    int ShaderProgram::SetUniform3i(string s, glm::ivec3 v) {
        int loc = GetUniformLocation(s);
		glUniform3i(loc, v.x, v.y, v.z);
		return loc;
    }
    int ShaderProgram::SetUniform4i(string s, glm::ivec4 v) {
        int loc = GetUniformLocation(s);
		glUniform4i(loc, v.x, v.y, v.z, v.w);
		return loc;
    }
    int ShaderProgram::SetUniform1ui(string s, GLuint i) {
        GLint loc = GetUniformLocation(s);
		glUniform1ui(loc, i);
		return loc;
    }
    int ShaderProgram::SetUniform2ui(string s, GLuint i, GLuint i2) {
        int loc = GetUniformLocation(s);
		glUniform2ui(loc, i, i2);
		return loc;
    }
    int ShaderProgram::SetUniform3ui(string s, GLuint i, GLuint i2, GLuint i3) {
        int loc = GetUniformLocation(s);
		glUniform3ui(loc, i, i2, i3);
		return loc;
    }
    int ShaderProgram::SetUniform4ui(string s, GLuint i, GLuint i2, GLuint i3, GLuint i4) {
        int loc = GetUniformLocation(s);
		glUniform4ui(loc, i, i2, i3, i4);
		return loc;
    }
    int ShaderProgram::SetUniform1fv(string s, GLsizei count, const GLfloat* value) {
		int loc = GetUniformLocation(s);
		glUniform1fv(loc, count, value);
		return loc;
	}
    int ShaderProgram::SetUniform2fv(string s, GLsizei count, const GLfloat* value) {
		int loc = GetUniformLocation(s);
		glUniform2fv(loc, count, value);
		return loc;
	}
    int ShaderProgram::SetUniform3fv(string s, GLsizei count, const GLfloat* value) {
		int loc = GetUniformLocation(s);
		glUniform3fv(loc, count, value);
		return loc;
	}
    int ShaderProgram::SetUniform4fv(string s, GLsizei count, const GLfloat* value) {
		int loc = GetUniformLocation(s);
		glUniform4fv(loc, count, value);
		return loc;
	}
    int ShaderProgram::SetUniform1iv(string s, GLsizei count, const GLint* value) {
		int loc = GetUniformLocation(s);
		glUniform1iv(loc, count, value);
		return loc;
	}
    int ShaderProgram::SetUniform2iv(string s, GLsizei count, const GLint* value) {
		int loc = GetUniformLocation(s);
		glUniform2iv(loc, count, value);
		return loc;
	}
    int ShaderProgram::SetUniform3iv(string s, GLsizei count, const GLint* value) {
		int loc = GetUniformLocation(s);
		glUniform3iv(loc, count, value);
		return loc;
	}
    int ShaderProgram::SetUniform4iv(string s, GLsizei count, const GLint* value) {
		int loc = GetUniformLocation(s);
		glUniform4iv(loc, count, value);
		return loc;
	}
    int ShaderProgram::SetUniform1uiv(string s, GLsizei count, const GLuint* value) {
		int loc = GetUniformLocation(s);
		glUniform1uiv(loc, count, value);
		return loc;
	}
    int ShaderProgram::SetUniform2uiv(string s, GLsizei count, const GLuint* value) {
		int loc = GetUniformLocation(s);
		glUniform2uiv(loc, count, value);
		return loc;
	}
    int ShaderProgram::SetUniform3uiv(string s, GLsizei count, const GLuint* value) {
		int loc = GetUniformLocation(s);
		glUniform3uiv(loc, count, value);
		return loc;
	}
    int ShaderProgram::SetUniform4uiv(string s, GLsizei count, const GLuint* value) {
		int loc = GetUniformLocation(s);
		glUniform4uiv(loc, count, value);
		return loc;
	}
    int ShaderProgram::SetUniformMatrix2fv(string s, GLsizei count, GLboolean transpose, const GLfloat* value) {
		int loc = GetUniformLocation(s);
		glUniformMatrix2fv(loc, count, transpose, value);
		return loc;
	}
    int ShaderProgram::SetUniformMatrix3fv(string s, GLsizei count, GLboolean transpose, const GLfloat* value) {
		int loc = GetUniformLocation(s);
		glUniformMatrix3fv(loc, count, transpose, value);
		return loc;
	}
    int ShaderProgram::SetUniformMatrix4fv(string s, GLsizei count, GLboolean transpose, const GLfloat* value) {
		int loc = GetUniformLocation(s);
		glUniformMatrix4fv(loc, count, transpose, value);
		return loc;
	}
    int ShaderProgram::SetUniformMatrix2x3fv(string s, GLsizei count, GLboolean transpose, const GLfloat* value) {
		int loc = GetUniformLocation(s);
		glUniformMatrix2x3fv(loc, count, transpose, value);
		return loc;
	}
    int ShaderProgram::SetUniformMatrix3x2fv(string s, GLsizei count, GLboolean transpose, const GLfloat* value) {
		int loc = GetUniformLocation(s);
		glUniformMatrix3x2fv(loc, count, transpose, value);
		return loc;
	}
    int ShaderProgram::SetUniformMatrix2x4fv(string s, GLsizei count, GLboolean transpose, const GLfloat* value) {
		int loc = GetUniformLocation(s);
		glUniformMatrix2x4fv(loc, count, transpose, value);
		return loc;
	}
    int ShaderProgram::SetUniformMatrix3x4fv(string s, GLsizei count, GLboolean transpose, const GLfloat* value) {
		int loc = GetUniformLocation(s);
		glUniformMatrix3x4fv(loc, count, transpose, value);
		return loc;
	}
    int ShaderProgram::SetUniformMatrix4x3fv(string s, GLsizei count, GLboolean transpose, const GLfloat* value) {
		int loc = GetUniformLocation(s);
		glUniformMatrix4x3fv(loc, count, transpose, value);
		return loc;
	}
	int ShaderProgram::SetSubroutine(GLenum shaderType, string s) {
        int loc = glGetSubroutineUniformLocation(mProgram, shaderType, s.c_str());
        return loc;
	}
	int ShaderProgram::GetSubroutineIndex(GLenum shaderType, string s) {
        return glGetSubroutineIndex(mProgram, shaderType, s.c_str());
	}
	void ShaderProgram::SetUniformLight(string s, Light& light) {
	    int ambient = GetUniformLocation(s + ".ambient");
	    int diffuse = GetUniformLocation(s + ".diffuse");
	    int specular = GetUniformLocation(s + ".specular");
	    int position = GetUniformLocation(s + ".position");
	    int direction = GetUniformLocation(s + ".direction");
	    int spot = GetUniformLocation(s + ".specExponent");
	    int spotCutoff = GetUniformLocation(s + ".spotCutoff");
	    int constant = GetUniformLocation(s + ".constant");
	    int linear = GetUniformLocation(s + ".linear");
	    int quadratic = GetUniformLocation(s + ".quadratic");

        glUniform3f(position, light.GetPosition().x, light.GetPosition().y, light.GetPosition().z);
        glUniform3f(ambient, light.GetAmbient().x, light.GetAmbient().y, light.GetAmbient().z);
        glUniform3f(diffuse, light.GetDiffuse().x, light.GetDiffuse().y, light.GetDiffuse().z);
        glUniform3f(specular, light.GetSpecular().x, light.GetSpecular().y, light.GetSpecular().z);
        glUniform1f(spot, light.GetSpecExponent());
        glUniform1f(spotCutoff , light.GetSpotCutoff());
        glUniform1f(spotCutoff , light.GetSpotCutoff());
        glUniform1f(constant , light.GetConstant());
        glUniform1f(linear , light.GetLinear());
        glUniform1f(quadratic, light.GetQuadratic());
        glUniform4f(direction, light.GetDirection().x, light.GetDirection().y, light.GetDirection().z, light.GetDirection().w);
	}
	void ShaderProgram::SetUniformLights(string s, std::vector<Light>& lights) {
        for(size_t i = 0; i < lights.size(); i++) {
            char index[2];
            sprintf(index, "%i", i);
            string uniformName = s + "[" + index + "]";
            SetUniformLight(uniformName, lights[i]);
        }
	}
    ShaderProgram GenerateGenericShader(ShaderProgramType s) {
        ShaderProgram sh;
        switch(s) {
        case SHADER_DEFAULT:
            sh.Initialize("shaders\\simple_vertex.glsl",
                          "shaders\\simple_fragment.glsl");
            sh.SetProgramID(s, "SHADER_DEFAULT");
            sh.SetShaderType(SHADER_DEFAULT);
            break;
        case SHADER_SILHOUETTE:
            sh.Initialize("shaders\\simple_vertex.glsl",
                          "shaders\\frag_silhouette.glsl");
            sh.SetProgramID(s, "SHADER_SILHOUETTE");
            sh.SetShaderType(SHADER_SILHOUETTE);
            break;
        case SHADER_CIRCLE:
            sh.Initialize("shaders\\circle_vertex.glsl",
                          "shaders\\frag_circle.glsl");
            sh.SetProgramID(s, "SHADER_SILHOUETTE");
            sh.SetShaderType(SHADER_CIRCLE);
            break;
        default:
            sh.SetShaderType(SHADER_CUSTOM);
            break;
        }
        return sh;
    }
};
