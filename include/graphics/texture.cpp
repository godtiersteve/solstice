#include "texture.h"
namespace solstice {
    Texture::Texture():
    mTexture(-1), mPixelData(NULL), mYAdjust(1.0f), mXAdjust(1.0f), mHeight(0), mWidth(0)
    {
        mType = GL_TEXTURE_2D;
        mLastUsedArrayID = 0;
        mPixelData = NULL;
    }
    Texture::Texture(const char* filename, string nameRef) {
        mPixelData = NULL;
        LoadImage(filename);
        if(nameRef.empty()) {
            string s = filename;
            size_t pos = s.find_last_of('/');
            if(pos == std::string::npos) {
                pos = 0;
            }
            else {
                pos++;
            }
            mNameRef = s.substr(pos, s.size() - 4);
        }
        else {
            mNameRef = nameRef;
        }
        mLastUsedArrayID = 0;
        mPixelData = NULL;
    }
    Texture::Texture(GLubyte* pdata, int h, int w, string nameRef) {
        mPixelData = NULL;
        LoadPixelData(pdata, h, w);
        mNameRef = nameRef;
        mLastUsedArrayID = 0;
        mPixelData = NULL;
    }

    void Texture::Clean() {
        if(mTexture > -1) {
            glDeleteTextures(1, &mTexture);
            mTexture = -1;
        }
        if(mPixelData) {
            SDL_FreeSurface(mPixelData);
        }
        mLastUsedArrayID = 0;
        mNameRef.clear();
    }
    void    Texture::SetNameRef(string ref)     {mNameRef = ref;}
    short   Texture::GetHeight()                {return mHeight;}
    short   Texture::GetWidth()                 {return mWidth;}
    float   Texture::GetXAdjust()               {return mXAdjust;}
    float   Texture::GetYAdjust()               {return mYAdjust;}
    string  Texture::GetNameRef()               {return mNameRef;}
    GLuint&  Texture::GetTexture()               {return mTexture;}
    void Texture::FreePixelData() {
        SDL_FreeSurface(mPixelData);
    }
    void Texture::Bind() {
        glBindTexture(mType, mTexture);
    }
    void Texture::Bind(GLenum textureUnit) {
        if(glIsTexture(mTexture)) {
            //glActiveTexture(textureUnit);
            glBindTexture(mType, mTexture);
        }
    }
    void Texture::Unbind() {glBindTexture(GL_TEXTURE_2D, 0);}
    void Texture::Rebind(const char* filename) {
        glDeleteTextures(1, &mTexture);
        LoadImage(filename);
    }
    void Texture::Delete() {
        std::cout << "Deleting text\n";
        if(glIsTexture(mTexture)) {
            glDeleteTextures(1, &mTexture);
            mTexture = 0;
        }
    }
    void Texture::ClearSurface() {
        if(mPixelData) {
            SDL_FreeSurface(mPixelData);
            mPixelData = NULL;
        }
    }

    void Texture::LoadPixelData(GLubyte* pdata, int h, int w) {
        glGenTextures(1, &mTexture);
        glBindTexture(GL_TEXTURE_2D, mTexture);

        glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        glTexImage2D(GL_TEXTURE_2D,
                     0,
                     GL_RGBA,
                     w,
                     h,
                     0,
                     GL_LUMINANCE_ALPHA,
                     GL_UNSIGNED_BYTE,
                     pdata);
        mHeight = h;
        mWidth = w;
        mXAdjust = 1.0f / w;
        mYAdjust = 1.0f / h;
        mType = GL_TEXTURE_2D;
    }
    void Texture::LoadPixelDataArrays(GLubyte* pdata, int h, int w, int count) {
        glGenTextures(1, &mTexture);
        glBindTexture(GL_TEXTURE_2D_ARRAY, mTexture);

        glTexStorage3D(GL_TEXTURE_2D_ARRAY,
                       1,
                       GL_RGBA,
                       w, h,
                       count);
        glTexSubImage3D(GL_TEXTURE_2D_ARRAY,
                        0, 0, 0, 0,
                        w, h, 1,
                        GL_RGBA,
                        GL_UNSIGNED_BYTE,
                        pdata);

        glTexParameteri(GL_TEXTURE_2D_ARRAY,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D_ARRAY,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D_ARRAY,GL_TEXTURE_WRAP_S,GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D_ARRAY,GL_TEXTURE_WRAP_T,GL_CLAMP_TO_EDGE);
        mType = GL_TEXTURE_2D;
    }
    void Texture::LoadImage(const char* filename, GLenum tileType, GLenum minFilter, GLenum maxFilter) {
        GetSurface(filename);
        if(!mPixelData) {
            std::cout << "Could not load " << filename << std::endl;
            return;
        }
        else {
            glGenTextures(1, &mTexture);
            glBindTexture(GL_TEXTURE_2D, mTexture);
            mHeight = mPixelData->h;
            mWidth = mPixelData->w;

            glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, tileType);
            glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, tileType);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, minFilter);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, maxFilter);

            glTexImage2D(GL_TEXTURE_2D,
                         0,
                         GL_RGBA,
                         mWidth, mHeight,
                         0,
                         GL_RGBA,
                         GL_UNSIGNED_BYTE,
                         mPixelData->pixels);
            mXAdjust = 1.0f / mWidth;
            mYAdjust = 1.0f / mHeight;
            SDL_FreeSurface(mPixelData);
            mPixelData = NULL;
            mType = GL_TEXTURE_2D;
            printf("Loaded image: %s\n", filename);
        }
    }
    void Texture::CreateArrayTexture(int width, int height, int layers, int wrapType) {
        if(!glIsTexture(mTexture)) {
            glGenTextures(1, &mTexture);
        }
        //glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D_ARRAY, mTexture);
        glTexStorage3D(GL_TEXTURE_2D_ARRAY, 1, GL_RGBA8, width, height, layers);

        glTexParameteri(GL_TEXTURE_2D_ARRAY,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D_ARRAY,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_WRAP_S, wrapType);
        glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_WRAP_T, wrapType);
        mLayerRefs.resize(layers);
        glBindTexture(GL_TEXTURE_2D_ARRAY, 0);
        mHeight = height;
        mWidth = width;
        mDepth = layers;
        mType = GL_TEXTURE_2D_ARRAY;
    }
    void Texture::LoadToArray(const char* filename, const char* ref) {
        GetSurface(filename);
        if(!mPixelData || mPixelData->h != mHeight || mPixelData->w != mWidth) {
            return;
        }
        //glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D_ARRAY, mTexture);
        glTexSubImage3D(GL_TEXTURE_2D_ARRAY,
                        0,
                        0, 0,
                        mLastUsedArrayID,
                        mWidth,
                        mHeight,
                        1,
                        GL_RGBA,
                        GL_UNSIGNED_BYTE,
                        mPixelData->pixels);
        glBindTexture(GL_TEXTURE_2D_ARRAY, 0);
        mLayerRefs[mLastUsedArrayID++] = ref;
        SDL_FreeSurface(mPixelData);
    }
    void Texture::GetSurface(const char* file) {
        mPixelData = IMG_Load(file);
        if(!mPixelData) {
            std::cout << "Could not load " << file << std::endl;
            mPixelData = NULL;
            return;
        }
        else {
            int bpp;
            Uint32 rmask, gmask, bmask, amask;
            SDL_PixelFormatEnumToMasks(SDL_PIXELFORMAT_ABGR8888,
                                      &bpp,
                                      &rmask, &gmask, &bmask, &amask);
            SDL_Surface *rgbSurface = SDL_CreateRGBSurface(0,
                                                           mPixelData->w, mPixelData->h,
                                                           bpp,
                                                           rmask,
                                                           gmask,
                                                           bmask,
                                                           amask);
            SDL_SetSurfaceAlphaMod(mPixelData, 0xFF);
            SDL_SetSurfaceBlendMode(mPixelData, SDL_BLENDMODE_NONE);

            SDL_BlitSurface(mPixelData, NULL, rgbSurface, NULL);
            SDL_FreeSurface(mPixelData);
            mPixelData = rgbSurface;
        }
    }
    bool Texture::Empty() {
        return glIsTexture(mTexture) == GL_FALSE;
    }
    int Texture::GetArrayID(string ref) {
        for(size_t i = 0; i < mLastUsedArrayID; i++) {
            if(ref == mLayerRefs[i]) {
                return i;
            }
        }
        return -1;
    }
    Vector2<float> Texture::ConvertPoint(Vector2<float> pt) {
        return Vector2<float>(pt.x * mXAdjust, pt.y * mYAdjust);
    }
    Rectangle<float> Texture::ConvertRect(Rectangle<float> rect) {
        return Rectangle<float>(ConvertPoint(rect.GetBotLeft()),
                                             rect.GetHeight() * mYAdjust,
                                             rect.GetWidth() * mXAdjust);
    }
};
