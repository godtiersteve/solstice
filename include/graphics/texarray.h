#ifndef TEXARRAY_H_INCLUDED
#define TEXARRAY_H_INCLUDED
#include "texture.h"
#include "utility/sol_str.h"
#include <vector>
#include <string>
using std::vector;
using std::string;

namespace solstice {
    class TextureArray {
    private:
        vector<Texture> mTextures;
        string mPath;
    public:
        TextureArray() {}
        TextureArray(const char* path, string texRef = "", TexLoadType loadType = TEX_IMAGE);
        TextureArray(GLubyte* pdata, unsigned int h, unsigned int w, string texRef = "");
        ~TextureArray();

        Texture& operator[](unsigned short i);
        Texture& operator[](string ref);
        Texture& GetTexture(unsigned short i);
        Texture& GetTexture(string ref);

        void LoadImage(string filename, string texRef = "");
        int LoadImg(string filename, string texRef = "");
        void LoadPixelData(GLubyte* pdata, unsigned int h, unsigned int w, string texRef = "");
        void LoadImages(const char* prefix, const char* ext, unsigned short amt, unsigned short digits, string texRef = "");
        void SetPath(string path) {mPath = path;}
        void AddTexture(Texture t) {mTextures.push_back(t);}

        void DeleteTexture(unsigned int i);
        void DeleteTexture(string texRef);
        void RemoveRange(unsigned short start, unsigned short end);
        int GetTextureID(string texRef);
        string GetPath() {return mPath;}
        bool BindTexture(int id, GLenum unit = GL_TEXTURE0);
        bool BindTexture(string id, GLenum unit = GL_TEXTURE0);
        void BindAll(size_t amt = 0);
        void BindAll(size_t start, size_t end);
        void Unbind(GLenum unit = GL_TEXTURE0);
        vector<Texture> GetTextures()                              {return mTextures;}

        unsigned int size();
        void pop();
        void clear();
        void push(Texture tex);
        void ClearNameRefs();
        Texture& front();
        Texture& back();
    };
};
#endif // TEXARRAY_H_INCLUDED
