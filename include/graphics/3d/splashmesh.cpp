#include "splashmesh.h"

namespace solstice {
SplashMesh::SplashMesh() {
    mSpread = 0.005f;
    mDamping = 0.04f;
    mSpringConstant = 0.025f;
    mSize = 10;
    mNumIter = 1;
    mFrontHeight = 0.0f;
    mXSprings = 0;
    mZSprings = 0;
}
SplashMesh::SplashMesh(float size, size_t xcount, size_t zcount) {
    Init(size, 0.0f, xcount, zcount);
}
SplashMesh::SplashMesh(float size, float frontHeight, size_t xcount, size_t zcount) {
    Init(size, frontHeight, xcount, zcount);
}
void SplashMesh::Init(float size, size_t xcount, size_t zcount) {
    Init(size, 0.0f, xcount, zcount);
}
void SplashMesh::Init(float size, float frontHeight, size_t xcount, size_t zcount) {
    mSpread = 0.005f;
    mDamping = 0.04f;
    mSpringConstant = 0.0125f;
    mNumIter = 1;
    mSize = size;
    mXSprings = xcount;
    mZSprings = zcount;
    mFrontHeight = frontHeight;
    mHeights.resize(mZSprings * mXSprings);
    for(size_t z = 0; z < mZSprings; z++) {
        for(size_t x = 0; x < mXSprings; x++) {
            Vector2f pos = Vector2f(x * size, -size);
            Spring spring(pos, size, size);
            spring.minHeight = -100;
            spring.maxHeight = 100;
            mSprings.push_back(spring);
        }
    }
}
void SplashMesh::Update2() {
    float mYSprings = mZSprings;
    for(int k = 0; k < 1; k++) {
        for(size_t i = 0; i < mXSprings; i++) {
            for(size_t j = 0; j < mYSprings; j++) {
                Spring& spring = mSprings[i + j * mYSprings];
                if(i > 0) {
                    spring.Pull(mSprings[i - 1 + j * mYSprings], mSpread);
                    if(j > 0) {
                        spring.Pull(mSprings[i - 1 + (j - 1) * mYSprings], mSpread);
                    }
                    if(j < mYSprings - 1) {
                        spring.Pull(mSprings[i - 1 + (j + 1) * mYSprings], mSpread);
                    }
                }
                if(i < mXSprings - 1) {
                    spring.Pull(mSprings[i + 1 + j * mYSprings], mSpread);
                    if(j > 0) {
                        spring.Pull(mSprings[i + 1 + (j - 1) * mYSprings], mSpread);
                    }
                    if(j < mYSprings - 1) {
                        spring.Pull(mSprings[i + 1 + (j + 1) * mYSprings], mSpread);
                    }
                }
                if(j > 0) {
                    spring.Pull(mSprings[i + (j - 1) * mYSprings], mSpread);
                }
                if(j < mYSprings - 1) {
                    spring.Pull(mSprings[i + (j + 1) * mYSprings], mSpread);
                }
            }
        }
    }
    for(auto& spring : mSprings) {
        spring.Update2(mSpringConstant, mDamping);
    }
    UpdateMesh();
}
void SplashMesh::Update(float t, float dt) {
    if(mSprings.empty()) {
       return;
    }
    for(auto& spring : mSprings) {
        spring.Update(t, dt, mSpringConstant, mDamping);
    }
    std::vector<WaterDeltas> deltas(mZSprings * mXSprings);
    for(int k = 0; k < mNumIter; k++) {
        for(size_t i = 0; i < mXSprings; i++) {
            for(size_t j = 0; j < mZSprings; j++) {
                Spring& spring = mSprings[i + j * mXSprings];
                if(i > 0) {
                    Spring& spr2 = mSprings[i - 1 + j * mXSprings];
                    deltas[i + j * mXSprings].left = spr2.HeightDifference(spring, mSpread);
                    spr2.speed+= deltas[i + j * mXSprings].left;
                    if(j > 0) {
                        Spring& spr2 = mSprings[i - 1 + (j - 1) * mXSprings];
                        deltas[i + j * mXSprings].botLeft = spr2.HeightDifference(spring , mSpread) * 0.5;
                        spr2.speed+=deltas[i + j * mXSprings].botLeft;
                    }
                    if(j < mZSprings - 1) {
                        Spring& spr2 = mSprings[i - 1 + (j + 1) * mXSprings];
                        deltas[i + j * mXSprings].topLeft = spr2.HeightDifference(spring, mSpread) * 0.5;
                        spr2.speed+= deltas[i + j * mXSprings].topLeft;
                    }
                }
                if(i < mXSprings - 1) {
                    Spring& spr2 = mSprings[i + 1 + j * mXSprings];
                    deltas[i + j * mXSprings].right = spr2.HeightDifference(spring, mSpread);
                    spr2.speed+= deltas[i + j * mXSprings].right;
                    if(j > 0) {
                        Spring& spr2 = mSprings[i + 1 + (j - 1) * mXSprings];
                        deltas[i + j * mXSprings].botRight = spr2.HeightDifference(spring, mSpread) * 0.5;
                        spr2.speed+=deltas[i + j * mXSprings].botRight;
                    }
                    if(j < mZSprings - 1) {
                        Spring& spr2 = mSprings[i + 1 + (j + 1) * mXSprings];
                        deltas[i + j * mXSprings].botLeft = spr2.HeightDifference(spring, mSpread) * 0.5;
                        spr2.speed+= deltas[i + j * mXSprings].botLeft;
                    }
                }
                if(j > 0) {
                    Spring& spr2 = mSprings[i + (j - 1) * mXSprings];
                    deltas[i + j * mXSprings].bottom = spr2.HeightDifference(spring, mSpread);
                    spr2.speed+= deltas[i + j * mXSprings].bottom;
                }
                if(j < mZSprings - 1) {
                    Spring& spr2 = mSprings[i + (j + 1) * mXSprings];
                    deltas[i + j * mXSprings].top = spr2.HeightDifference(spring, mSpread);
                    spr2.speed+= deltas[i + j * mXSprings].top;
                }
            }
        }
        for(size_t i = 0; i < mXSprings; i++) {
            for(size_t j = 0; j < mZSprings; j++) {
                if(i > 0) {
                    Spring& spr2 = mSprings[i - 1 + j * mXSprings];
                    spr2.height+= deltas[i + j * mXSprings].left;
                    if(j > 0) {
                        Spring& spr2 = mSprings[i - 1 + (j - 1) * mXSprings];
                        spr2.height+=deltas[i + j * mXSprings].botLeft;
                    }
                    if(j < mZSprings - 1) {
                        Spring& spr2 = mSprings[i - 1 + (j + 1) * mXSprings];
                        spr2.height+= deltas[i + j * mXSprings].topLeft;
                    }
                }
                if(i < mXSprings - 1) {
                    Spring& spr2 = mSprings[i + 1 + j * mXSprings];
                    spr2.height+= deltas[i + j * mXSprings].right;
                    if(j > 0) {
                        Spring& spr2 = mSprings[i + 1 + (j - 1) * mXSprings];
                        spr2.height+=deltas[i + j * mXSprings].botRight;
                    }
                    if(j < mZSprings - 1) {
                        Spring& spr2 = mSprings[i + 1 + (j + 1) * mXSprings];
                        spr2.height+= deltas[i + j * mXSprings].botLeft;
                    }
                }
                if(j > 0) {
                    Spring& spr2 = mSprings[i + (j - 1) * mXSprings];
                    spr2.height+= deltas[i + j * mXSprings].bottom;
                }
                if(j < mZSprings - 1) {
                    Spring& spr2 = mSprings[i + (j + 1) * mXSprings];
                    spr2.height+= deltas[i + j * mXSprings].top;
                }
                mHeights[i + j * mXSprings] = mSprings[i + j * mXSprings].height;
            }
        }
    }
    UpdateMesh();
}
void SplashMesh::Splash(int i, int j, float speed) {
    if(mSprings.empty() || i > mXSprings || j > mZSprings) {
        return;
    }
    mSprings[i + j * mXSprings].speed = speed;
}
void SplashMesh::SplashRange(size_t x, size_t z, float speed) {
    if(x < 0 || x >= mXSprings || z < 0 || z >= mZSprings) {
        return;
    }
    mSprings[x + z * mXSprings].speed = speed;
    mSprings[x + 1 + z * mXSprings].speed = speed / 2;
    mSprings[x - 1 + z * mXSprings].speed = speed / 2;

    mSprings[x + (z + 1) * mXSprings].speed = speed / 2;
    mSprings[x + (z - 1) * mXSprings].speed = speed / 2;
    mSprings[x + 1 + (z + 1) * mXSprings].speed = speed / 2;

    mSprings[x + 1 + (z - 1)* mXSprings].speed = speed / 2;
    mSprings[x - 1 + (z + 1) * mXSprings].speed = speed / 2;
    mSprings[x - 1 + (z - 1) * mXSprings].speed = speed / 2;
}
void SplashMesh::SplashRange(int xStart, int xEnd, int zStart, int zEnd, float speed) {
    if(xStart == xEnd && zStart == zEnd) {
        SplashRange(xStart, zStart, speed);
    }
    else {
        int z = zEnd;
        for(int x = xStart; x <= xEnd; x++) {
            SplashRange(x, zStart, speed);
        }
    }
}
void SplashMesh::Draw(SceneRenderer& scene) {
    for(size_t i = 0; i < mZSprings; i++) {
        for(size_t j = 0; j < mXSprings; j++) {
            Vector2f pos = mSprings[i + j * mXSprings].position;
            pos.y+= mSprings[i + j * mXSprings].height;
            pos.y+= i * mSize;
            Rectanglef rect(pos, mSize / 4, mSize / 4, DRAW_CENTER);
            float alpha = (float)i / (float)(mSprings.size() / mZSprings);
            scene.DrawRect(rect, Color(alpha, 0.5f, 1.0f, 0.5f));
        }
    }
}
void SplashMesh::CreateMesh() {
    mVertices.resize(mXSprings * mZSprings);
    mNormals.resize(mXSprings * mZSprings);
    mColors.resize(mXSprings * mZSprings);
    mTexCoords.resize(mXSprings * mZSprings);
    mHeights.resize(mXSprings * mZSprings);

    mIndices.clear();
    float width = mZSprings * mSize * xScale * 2;
    float depthStep = width / mZSprings;
    glm::vec3 pos(-width / 2, 0, -width / 2);
    for(size_t z = 0; z < mZSprings; z++) {
        for(size_t x = 0; x < mXSprings; x++) {
            Spring* spring = &mSprings[x + z * mXSprings];
            Vertex3f vt;
            vt.position.x = spring->position.x * xScale;
            vt.position.y = (spring->position.y + spring->height) * yScale;
            vt.position.z = z * -depthStep;
            vt.texCoord.x = (float)x  / float(mXSprings) * 6;
            vt.texCoord.y = (float)z  / float(mZSprings) * 2;
            vt.normal.x = 0.0f;
            vt.normal.y = 1.0f;
            vt.normal.z = 0.0f;
            vt.color.alpha = 1.0f;
            //mVertices.push_back(vt.position);
            mVertices[x + z * mXSprings] = vt.position;
            mNormals[x + z * mXSprings] = vt.normal;
            mColors[x + z * mXSprings] = vt.color;
            mTexCoords[x + z * mXSprings] = vt.texCoord;
            if(x < mXSprings - 1 && z < mZSprings - 1) {
                mIndices.push_back(x + z * mXSprings);
                mIndices.push_back(x + 1 + z * mXSprings);
                mIndices.push_back(x + 1 + (z + 1) * mXSprings);

                mIndices.push_back(x + z * mXSprings);
                mIndices.push_back(x + (z  + 1) * mXSprings);
                mIndices.push_back(x + 1 + (z  + 1) * mXSprings);
            }
        }
    }
    size_t indexCount = mVertices.size();
    if(mFrontHeight > 0.0f) {
        for(size_t i = 0; i < mXSprings; i++) {
            Spring* spring = &mSprings[i];
            Vertex3f vt;
            vt.position.x = spring->position.x * xScale;
            vt.position.y = -mFrontHeight;
            vt.position.z = 0.0f;
            vt.texCoord.x = (float)i  / float(mXSprings - 1) * 6;
            vt.texCoord.y = 1.0f;
            vt.normal.x = 0.0f;
            vt.normal.y = 0.0f;
            vt.normal.z = 1.0f;
            vt.color.alpha = 0.5f;
            mVertices.push_back(vt.position);
            mColors.push_back(vt.color);
            mNormals.push_back(vt.normal);
            mTexCoords.push_back(vt.texCoord);
            if(i < mXSprings - 1) {
                mIndices.push_back(i);
                mIndices.push_back(i + 1);
                mIndices.push_back(indexCount + i);

                mIndices.push_back(indexCount + i);
                mIndices.push_back(indexCount + i + 1);
                mIndices.push_back(i + 1);
            }
        }
        Vertex3f botLeft;
        Vertex3f botRight;
        botLeft.position.x = mSprings[0].position.x * xScale;
        botLeft.position.y = -bottom;
        botLeft.texCoord.x = 0.0f;
        botLeft.texCoord.y = 5.0f;
        botLeft.normal.z = 1.0f;
        botLeft.color.alpha = 0.5f;

        botRight.position.x = mSprings[mXSprings - 1].position.x * xScale;
        botRight.position.y = -bottom;
        botRight.texCoord.x = 6;
        botRight.texCoord.y = 5.0f;
        botRight.normal.z = 1.0f;
        botRight.color.alpha = 0.5f;

        mVertices.push_back(botLeft.position);
        mVertices.push_back(botRight.position);
        mColors.push_back(botLeft.color);
        mNormals.push_back(botLeft.normal);
        mTexCoords.push_back(botLeft.texCoord);
        mColors.push_back(botRight.color);
        mNormals.push_back(botRight.normal);
        mTexCoords.push_back(botRight.texCoord);

        mIndices.push_back(mVertices.size() - 2);
        mIndices.push_back(mVertices.size() - 1);
        mIndices.push_back(indexCount);

        mIndices.push_back(mVertices.size() - 1);
        mIndices.push_back(indexCount + mXSprings - 1);
        mIndices.push_back(indexCount);
    }
    SetupMesh();
}
float SplashMesh::GetSpringHeight(size_t i) {
    return (mSprings[i].position.y + mSprings[i].height) * yScale;
}
float SplashMesh::GetSpringHeight(size_t x, size_t z) {
    return GetSpringHeight(x + z * mXSprings);
}
Rectanglef SplashMesh::GetHitBox(size_t x, size_t z) {
    Spring& spring = GetSpring(x, z);
    return Rectanglef(spring.position, spring.width, spring.height);
}
void SplashMesh::UpdateMesh() {
    for(size_t z = 0; z < mZSprings; z++) {
        for(size_t x = 0; x < mXSprings; x++) {
            glm::vec3& normal = mNormals[x + z * mXSprings];
            int i = x + z * mXSprings;
            mVertices[x + z * mXSprings].y = GetSpringHeight(x, z);
            normal = glm::vec3(0, 0, 0);
            if(x < mXSprings - 1 && z < mZSprings - 1) {
                normal+= GetNormal(i,
                                   x + 1 + z * mXSprings,
                                   x + (z + 1) * mXSprings);
                /*
                if(x > 0) {
                    normal+= GetNormal(i,
                                   x - 1 + z * mXSprings,
                                   x + (z + 1) * mXSprings);
                }
                if(z > 0) {
                    normal+= GetNormal(i,
                                   x + 1 + z * mXSprings,
                                   x + (z - 1) * mXSprings);
                }
                */
            }
            if(z > 0 && x > 0) {
                normal+= GetNormal(i,
                                   x - 1 + z * mXSprings,
                                   x + (z - 1) * mXSprings);
            }
            if((x == 0 || z == 0) || (z == mZSprings  -1 || x == mXSprings - 1)) {
                normal+= glm::vec3(0, 1, 0);
            }
        }
    }

    glBindBuffer(GL_ARRAY_BUFFER, mVertexBuffer.vbo);
    GLvoid* p = glMapBuffer(GL_ARRAY_BUFFER, GL_WRITE_ONLY);
    memcpy(p, &mVertices[0], sizeof(glm::vec3) * mVertices.size());
    glUnmapBuffer(GL_ARRAY_BUFFER);

    glBindBuffer(GL_ARRAY_BUFFER, mNormalBuff);
    p = glMapBuffer(GL_ARRAY_BUFFER, GL_WRITE_ONLY);
    memcpy(p, &mNormals[0], sizeof(glm::vec3) * mNormals.size());
    glUnmapBuffer(GL_ARRAY_BUFFER);

}
void SplashMesh::SetupMesh() {
    glGenVertexArrays(1, &mVertexBuffer.vao);
    glGenBuffers(1, &mVertexBuffer.vbo);
    glGenBuffers(1, &mVertexBuffer.ebo);

    glGenBuffers(1, &mNormalBuff);
    glGenBuffers(1, &mColBuff);
    glGenBuffers(1, &mTexBuff);
    glGenBuffers(1, &mHeightBuffer);

    glBindVertexArray(mVertexBuffer.vao);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mVertexBuffer.ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, mIndices.size() * sizeof(GLuint), &mIndices[0], GL_STATIC_DRAW);

    glBindBuffer(GL_ARRAY_BUFFER, mVertexBuffer.vbo);
    glBufferData(GL_ARRAY_BUFFER, mVertices.size() * sizeof(glm::vec3), &mVertices[0], GL_DYNAMIC_DRAW);

    glBindBuffer(GL_ARRAY_BUFFER, mColBuff);
    glBufferData(GL_ARRAY_BUFFER, mColors.size() * sizeof(glm::vec4), &mColors[0], GL_DYNAMIC_DRAW);

    glBindBuffer(GL_ARRAY_BUFFER, mNormalBuff);
    glBufferData(GL_ARRAY_BUFFER, mNormals.size() * sizeof(glm::vec3), &mNormals[0], GL_DYNAMIC_DRAW);

    glBindBuffer(GL_ARRAY_BUFFER, mTexBuff);
    glBufferData(GL_ARRAY_BUFFER, mVertices.size() * sizeof(glm::vec2), &mTexCoords[0], GL_DYNAMIC_DRAW);

    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);
    glEnableVertexAttribArray(2);
    glEnableVertexAttribArray(3);

    glBindBuffer(GL_ARRAY_BUFFER, mVertexBuffer.vbo);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(glm::vec3), 0);
    glBindBuffer(GL_ARRAY_BUFFER, mColBuff);
    glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, sizeof(glm::vec4), 0);
    glBindBuffer(GL_ARRAY_BUFFER, mNormalBuff);
    glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(glm::vec3), 0);
    glBindBuffer(GL_ARRAY_BUFFER, mTexBuff);
    glVertexAttribPointer(3, 2, GL_FLOAT, GL_FALSE, sizeof(glm::vec2), 0);

    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}
glm::vec3& SplashMesh::GetVertex(size_t x, size_t z) {return mVertices[x + z * mXSprings];}
Spring& SplashMesh::GetSpring(size_t x, size_t z) {
    return mSprings[x + z * mXSprings];
}
glm::vec3 SplashMesh::GetNormal(size_t i, size_t j, size_t k) {
    glm::vec3 v1 = mVertices[i];
    glm::vec3 v2 = mVertices[j];
    glm::vec3 v3 = mVertices[k];
    v2.y = GetSpringHeight(j);
    v3.y = GetSpringHeight(k);
    glm::vec3 norm = glm::cross(v3 - v1, v2 - v1);
    if(norm.y < 0)
        return -norm;
    return norm;
}
void SplashMesh::CalcNormals() {
    for(size_t i = 0; i < mIndices.size(); i+=3) {
        glm::vec3& current = mVertices[mIndices[i]];
        glm::vec3& curNormal = mNormals[mIndices[i]];
        glm::vec3& next = mVertices[mIndices[i + 1]];
        glm::vec3& next2 = mVertices[mIndices[i + 2]];
        glm::vec3 edge1 = current - next;
        glm::vec3 edge2 = current - next2;
        glm::vec3 norm = glm::cross(edge1, edge2);
        if(norm.y < 0) {
            norm = -norm;
        }
        curNormal+= norm;
    }
}
void SplashMesh::Draw(ShaderProgram& p) {
    glm::mat4 trans = mTransform.GetTransform() * mInverseTransform;
    p.SetUniformMatrix4fv("model", 1, GL_FALSE, glm::value_ptr(trans));
    DrawVao();
}
void SplashMesh::Draw(ShaderProgram& p, TextureArray& textures) {
    glm::mat4 trans = mTransform.GetTransform() * mInverseTransform;
    p.SetUniformMatrix4fv("model", 1, GL_FALSE, glm::value_ptr(trans));
    DrawVao();
}
void SplashMesh::DrawVao() {
    glBindVertexArray(mVertexBuffer.vao);
    glDrawElements(GL_TRIANGLES, mIndices.size(), GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
}
}
