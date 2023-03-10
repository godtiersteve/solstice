#include "texarray.h"
namespace solstice {
    TextureArray::TextureArray(const char* path, string texRef, TexLoadType loadType) {
        switch(loadType) {
            case TEX_IMAGE:
                LoadImage(path, texRef);
                break;
            case TEX_ARCHIVE:
                //LoadArchive(path, texRef);
                break;
            case TEX_FOLDER:
                //LoadDirectory(path, texRef);
                break;
            case TEX_CYTEX:
                //LoadCyTex(path, texRef);
                break;
            default:
                break;
        }
    }
    TextureArray::TextureArray(GLubyte* pdata, unsigned int h, unsigned int w, string texRef) {
        LoadPixelData(pdata, h, w, texRef);
    }
    TextureArray::~TextureArray() {
        clear();
    }
    Texture& TextureArray::operator[](unsigned short i) {
        if(i < mTextures.size()) {
            return mTextures[i];
        }
        else
            return mTextures[0];
    }
    Texture& TextureArray::operator[](string ref) {
        for(unsigned int i = 0; i < mTextures.size(); i++) {
            if(mTextures[i].GetNameRef() == ref)
                return mTextures[i];
        }
        return mTextures[0];
    }
    Texture& TextureArray::GetTexture(unsigned short i) {
        if(i < mTextures.size()) {
            return mTextures[i];
        }
        else
            return mTextures[0];
    }
    Texture& TextureArray::GetTexture(string ref) {
        for(unsigned int i = 0; i < mTextures.size(); i++) {
            if(mTextures[i].GetNameRef() == ref)
                return mTextures[i];
        }
        return mTextures[0];
    }
    int TextureArray::LoadImg(string filename, string texRef) {
        if(texRef.empty()) {
            size_t start = filename.find_last_of("\\");
            if(start == string::npos) {
                start = filename.find_last_of("/");
            }
            if(start == string::npos) {
                start = 0;
            }
            size_t end = filename.find_last_of(".");
            texRef = filename.substr(start, end);
        }
        for(size_t i = 0; i < mTextures.size(); i++) {
            if(mTextures[i].GetNameRef() == texRef) {
                return i;
            }
        }
        printf("Loading texture: %s (ref: %s)\n", filename.c_str(), texRef.c_str());
        mTextures.push_back(Texture((mPath + filename).c_str(), texRef));
        return mTextures.size() - 1;
    }
    void TextureArray::LoadImage(string filename, string texRef) {
        if(texRef.empty()) {
            size_t start = filename.find_last_of("\\");
            if(start == string::npos) {
                start = filename.find_last_of("/");
            }
            size_t end = filename.find_last_of(".");
            texRef = filename.substr(start, end);
        }
        for(auto& texture : mTextures) {
            if(texture.GetNameRef() == texRef) {
                return;
            }
        }
        mTextures.push_back(Texture());
        mTextures.back().LoadImage((mPath + filename).c_str());
        mTextures.back().SetNameRef(texRef);
        //mTextures.push_back(Texture((mPath + filename).c_str(), texRef));
    }
    void TextureArray::LoadPixelData(GLubyte* pdata, unsigned int h, unsigned int w, string texRef) {
        mTextures.push_back(Texture(pdata, h, w, texRef));
    }
    void TextureArray::LoadImages(const char* prefix, const char* ext, unsigned short amt, unsigned short digits, string ref) {
        int numSize = digits + strlen(ext) + 40;
        char* num = new char[numSize];
        for(int i = 0; i < amt; i++) {
            SetStrNum(num, i, digits);
            int nameSize = strlen(prefix) + strlen(num) + 40;
            char fname[nameSize];
            strcpy(fname, prefix);
            strcat(fname, num);
            strcat(fname, ext);
            string n = fname;
            string nameRef = "";
            if(!ref.empty()) {
                nameRef = ref + num;
            }
            mTextures.push_back(Texture(fname, nameRef));
            strcpy(num, "");
            strcpy(fname, "");
        }
        delete[] num;
        num = NULL;
    }
    void TextureArray::Unbind(GLenum unit) {
        glBindTexture(unit, 0);
    }
    bool TextureArray::BindTexture(string id, GLenum unit) {
        return BindTexture(GetTextureID(id), unit);
    }
    bool TextureArray::BindTexture(int id, GLenum unit) {
        if(id < 0 || id >= mTextures.size()) {
            return false;
        }
        //printf("%i: %s\n", id, GetTexture(id).GetNameRef().c_str());
        //std::cout << GetTexture(id).GetNameRef() << std::endl;
        mTextures[id].Bind();
        return true;
    }
    void TextureArray::BindAll(size_t amt) {
        if(amt) {
            for(size_t i = 0; i < amt; i++) {
                mTextures[i].Bind(GL_TEXTURE0 + amt);
            }
        }
        else {
            for(size_t i = 0; i < mTextures.size(); i++) {
                mTextures[i].Bind(GL_TEXTURE0 + i);
            }
        }
    }
    void TextureArray::BindAll(size_t start, size_t end) {
        for(size_t i = start; i < end; i++) {
            mTextures[i].Bind(GL_TEXTURE0 + i - start);
        }
    }
    void TextureArray::DeleteTexture(unsigned int i) {
        if(i < mTextures.size()) {
            mTextures.erase(mTextures.begin() + i);
        }
    }
    void TextureArray::DeleteTexture(string texRef) {
        for(unsigned int i = 0; i < mTextures.size(); i++) {
            if(mTextures[i].GetNameRef() == texRef) {
                DeleteTexture(i);
                return;
            }
        }
    }
    void TextureArray::RemoveRange(unsigned short start, unsigned short end) {
        mTextures.erase(mTextures.begin() + start, mTextures.begin() + end);
    }
    unsigned int TextureArray::size() {return mTextures.size();}
    void TextureArray::pop() {
        mTextures.pop_back();
    }
    void TextureArray::clear() {
        if(mTextures.size() > 0) {
            for(unsigned int i = 0; i < mTextures.size(); i++) {
                mTextures[i].Clean();
            }
        }
        mTextures.clear();
    }
    void TextureArray::push(Texture tex) {mTextures.push_back(tex);}
    Texture& TextureArray::front() {return mTextures.front();}
    Texture& TextureArray::back() {return mTextures.back();}
    int TextureArray::GetTextureID(string texRef) {
        for(size_t i = 0; i < mTextures.size(); i++) {
            if(mTextures[i].GetNameRef() == texRef) {
                return i;
            }
        }
        return -1;
    }
    void TextureArray::ClearNameRefs() {
        for(unsigned int i = 0; i < mTextures.size(); i++) {
            mTextures[i].SetNameRef("");
        }
    }
};

