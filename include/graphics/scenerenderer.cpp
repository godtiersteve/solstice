#include "scenerenderer.h"

namespace solstice {
GLuint SceneRenderer::sIndexBuffer = 0;
SceneRenderer::SceneRenderer(Vector2f resolution, int vtBuffSize) {
    mVBO = mIBO = mVAO = mVAO2 = 0;
    Setup(vtBuffSize);
    mPerspective = false;
    SetResolution(resolution);
    SetOrtho();
    mUsedVertices = 0;
    mUsedIndices = 0;
    mUsedQuads = 0;
    mLastIndex = 0;
}
SceneRenderer::SceneRenderer(float resx, float resy, int vtBuffSize) {
    mVBO = mIBO = mVAO = mVAO2 = 0;
    Setup(vtBuffSize);
    mPerspective = false;
    SetResolution(Vector2f(resx, resy));
    SetOrtho();
    mUsedVertices = 0;
    mUsedIndices = 0;
    mUsedQuads = 0;
    mLastIndex = 0;
}
void SceneRenderer::SetResolution(Vector2f res) {
    mResolution = res;
    mNormalize.x = 2.0d / (double)res.x;
    mNormalize.y = 2.0d / (double)res.y;
}
void SceneRenderer::Setup(int vtBuffSize) {
    glGenBuffers(1, &mIBO);
    glGenBuffers(1, &mVBO);
    glGenVertexArrays(1, &mVAO);
    ReinitBuffers(vtBuffSize);
    CreateBuffers(vtBuffSize);
}
void SceneRenderer::ReinitBuffers(unsigned int vtBuffSize) {
    size_t vtBuff = vtBuffSize * 2;
    size_t idBuff = vtBuffSize * 3;
    mVerts.resize(vtBuff);
    mIndices.resize(idBuff);

    glBindVertexArray(mVAO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mIBO);
    glBindBuffer(GL_ARRAY_BUFFER, mVBO);

    glBufferData(GL_ELEMENT_ARRAY_BUFFER,
                 mIndices.size() * sizeof(GLuint),
                 nullptr,
                 GL_DYNAMIC_DRAW);

    glBufferData(GL_ARRAY_BUFFER,
                 mVerts.size() * sizeof(Vertex2f),
                 nullptr,
                 GL_DYNAMIC_DRAW);

    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);
    glEnableVertexAttribArray(2);
    glEnableVertexAttribArray(3);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex2f), NULL);
    glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex2f), (const GLvoid*)offsetof(Vertex2f, color));
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex2f), (const GLvoid*)offsetof(Vertex2f, texCoord));
    glVertexAttribIPointer(3, 1, GL_INT, sizeof(Vertex2f), (const GLvoid*)offsetof(Vertex2f, textureID));

    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}
void SceneRenderer::DrawRect(Rectanglef rect, Color c) {
    DrawRect(rect, -1000, c);
}
void SceneRenderer::DrawRect(solstice::Rectanglef rect, int textureID, Color c) {
    QuadData q(rect, textureID);
    DrawQuad(q, c);
}
void SceneRenderer::DrawRectOutline(Rectanglef rect, float size, Color c) {
    DrawLine(rect.GetTopLeft(), rect.GetTopRight(), size, c);
    DrawLine(rect.GetTopRight(), rect.GetBotRight(), size, c);
    DrawLine(rect.GetBotRight(), rect.GetBotLeft(), size, c);
    DrawLine(rect.GetBotLeft(), rect.GetTopLeft(), size, c);
}
void SceneRenderer::DrawQuad(QuadData q, Color c) {
    mPositions[mUsedQuads].x = q.GetBotLeft().x;
    mPositions[mUsedQuads].y = q.GetBotLeft().y;
    mPositions[mUsedQuads].z = q.GetWidth();
    mPositions[mUsedQuads].w = q.GetHeight();
    if(c == COL_EMPTY) {
        c = q.GetColor();
    }
    mColors[mUsedQuads].r = c.red;
    mColors[mUsedQuads].g = c.green;
    mColors[mUsedQuads].b = c.blue;
    mColors[mUsedQuads].a = c.alpha;
    Vector2f texCoord = q.GetTexCoord(0);
    Vector2f texCoord2 = q.GetTexCoord(2);
    mTextureOffsets[mUsedQuads].x = texCoord.x;
    mTextureOffsets[mUsedQuads].y = texCoord.y;
    mTextureOffsets[mUsedQuads].z = texCoord2.x - texCoord.x;
    mTextureOffsets[mUsedQuads].w = texCoord2.y - texCoord.y;
    int id = q.GetTextureID();
    mTextureIDs[mUsedQuads] = q.GetTextureID();
    mAngles[mUsedQuads] = q.GetAngle();
    mUsedQuads++;
    if(mUsedQuads > mMaxQuads) {
        ResizeBuffers(mMaxQuads * 2);
    }
}
void SceneRenderer::AddVertex(Vertex2f vt) {
    mVerts[mUsedVertices++] = vt;
    if(mUsedVertices >= mVerts.size()) {
        mVerts.resize(mVerts.size() * 2);
        mIndices.resize(mIndices.size() * 2);
        ReinitBuffers(mVerts.size());
    }
}
void SceneRenderer::DrawLine(Vector2f start, Vector2f end, float size, Color c) {
    float width = start.DistanceSqrt(end);
    if(end.x < start.x) {
        Vector2f temp = start;
        start = end;
        end = start;
    }
    Rectanglef rect(start, size, width, DRAW_LEFTCENTER);
    Vector2f dist = end - start;
    rect.SetAngle(atan2(dist.y, dist.x));
    DrawRect(rect, c);
}
void SceneRenderer::DrawEdge(Edgef e, float size, Color c) {
    DrawLine(e.v1, e.v2, size, c);
}
void SceneRenderer::DrawMultiQuad(Picture picture, Color c) {
    for(auto quad : picture.GetQuads()) {
        quad.AddXY(picture.GetOrigin());
        DrawQuad(quad, c);
    }
}
void SceneRenderer::AddQuadIndices() {
    int i = mLastIndex;
    mIndices[mUsedIndices++] = i;
    mIndices[mUsedIndices++] = i + 1;
    mIndices[mUsedIndices++] = i + 2;
    mIndices[mUsedIndices++] = i;
    mIndices[mUsedIndices++] = i + 3;
    mIndices[mUsedIndices++] = i + 2;
    mLastIndex+=4;
}
void SceneRenderer::DrawPoly(Polygonf p, Color c) {
    DrawPoly(p, -1000, 0, c);
}
void SceneRenderer::DrawPoly(Polygonf p, int texID, Vector2f offset, Color c) {
    DrawPoly(p, texID, offset, 1.0f, 1.0f, c);
}
void SceneRenderer::DrawPoly(Polygonf p, int texID, Vector2f offset, float height, float width, Color c) {
    if(p.GetNumVert() < 3) {
        return;
    }
    Vector2f pos = Vector2f(0);
    Vertex2f vert = (p.GetVertex(0) + pos);
    vert.textureID = texID;
    vert.color = c;
    vert.texCoord = offset + 0.5f;
    mVerts[mUsedVertices++] = vert;
    float x = -1, y = -1;
    if(texID >= 0) {
        x = offset.x + (cos(DegreesToRadians(90)) + 1) / 2.0f;
        y = offset.y + (sin(DegreesToRadians(90)) + 1) / 2.0f;
    }
    vert = (p.GetVertex(1) + pos);
    vert.texCoord = Vector2f(x + texID, y + texID);
    mVerts[mUsedVertices++] = vert;

    float startAngle = PI * 0.5;
    int firstIndex = mLastIndex;
    float angleStep = TWO_PI / (p.GetNumVert() - 2);
    for(size_t i = 1; i < p.GetNumVert(); i++) {
        if(texID >= 0) {
            x = offset.x + (cos(startAngle + (i - 1) * angleStep) + 0.5 * width) / 2.0;
            y = offset.y + (sin(startAngle + (i - 1) * angleStep) + 0.5 * height) / 2.0;
        }
        vert = (p.GetRight(i) + pos);
        vert.texCoord = Vector2f(x + texID, y + texID);
        mVerts[mUsedVertices++] = vert;
        mIndices[mUsedIndices++] = firstIndex;
        mIndices[mUsedIndices++] = firstIndex + i;
        mIndices[mUsedIndices++] = firstIndex + i + 1;
    }
    mLastIndex+= p.GetNumVert() + 1;

}
void SceneRenderer::DrawPolyVerts(Polygonf p, float size, Color c) {
    for(size_t i = 0; i < p.GetNumVert(); i++) {
        Rectanglef rect(p.GetVertex(i), size, size);
        Color c = COL_BLUE;
        c.alpha = 0.5f;
        c.red = (float)i / (float)p.GetNumVert();
        DrawRect(rect, -1, c);
    }
}
void SceneRenderer::DrawCircle(Circlef circ, int verts, Color col) {
    DrawCircle(circ, verts, -1000, col);
}
void SceneRenderer::DrawCircle(Circlef circ, int verts, int texId, Color col) {
    Polygonf p(circ.GetPosition(), verts, circ.GetRadius() * 2);
    DrawPoly(p, col);
}
void SceneRenderer::DrawRing(float innerRadius, float outerRadius, Color col) {

}
void SceneRenderer::DrawHitBox(HitBoxf& hbox, Color c) {
    for(auto hb : hbox.GetHitBoxes()) {
        DrawShape(hb.get(), c);
    }
}
void SceneRenderer::DrawShape(const IShapef* shape, Color c) {
    switch(shape->GetType()) {
        case SHAPE_RECT:
            //DrawRect(*(Rectanglef*)shape, c);
            DrawRectPoly(*(Rectanglef*)shape, c);
            break;
        default:
            DrawPoly(*(Polygonf*)shape, c);
            break;
    }
}
void SceneRenderer::DrawRectPoly(const Rectanglef& rect, Color c) {
    DrawQuadPoly(rect, -1, c);
}
void SceneRenderer::DrawQuadPoly(const Rectanglef& rect, int texID, Color c) {
    Vertex2f vert;
    vert.color = c;
    vert.textureID = texID;

    vert.texCoord = Vector2f(0, 1);
    mVerts[mUsedVertices] = vert;
    mVerts[mUsedVertices++] = rect.GetTopLeft();

    vert.texCoord = Vector2f(1, 1);
    mVerts[mUsedVertices] = vert;
    mVerts[mUsedVertices++] = rect.GetTopRight();

    vert.texCoord = Vector2f(1, 0);
    mVerts[mUsedVertices] = vert;
    mVerts[mUsedVertices++] = rect.GetBotRight();

    vert.texCoord = Vector2f(0, 0);
    mVerts[mUsedVertices] = vert;
    mVerts[mUsedVertices++] = rect.GetBotLeft();
    AddQuadIndices();
}
void SceneRenderer::DrawQuadPoly(const QuadData& rect, Color c) {
    mVerts[mUsedVertices++] = rect.GetVertex2f(VT_TOPLEFT);
    mVerts[mUsedVertices++] = rect.GetVertex2f(VT_TOPRIGHT);
    mVerts[mUsedVertices++] = rect.GetVertex2f(VT_BOTRIGHT);
    mVerts[mUsedVertices++] = rect.GetVertex2f(VT_BOTLEFT);
    AddQuadIndices();
}
void SceneRenderer::ResizeBuffers(size_t size){
    mMaxQuads = size;
    mPositions.resize(mMaxQuads);
    mColors.resize(mMaxQuads);
    mTextureOffsets.resize(mMaxQuads);
    mTextureIDs.resize(mMaxQuads);
    mAngles.resize(mMaxQuads);

    glBindBuffer(GL_ARRAY_BUFFER, mColorBuffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec4) * mMaxQuads, nullptr, GL_DYNAMIC_DRAW);

    glBindBuffer(GL_ARRAY_BUFFER, mTexCoordBuffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec4) * mMaxQuads, nullptr, GL_DYNAMIC_DRAW);

    glBindBuffer(GL_ARRAY_BUFFER, mPositionBuffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec4) * mMaxQuads, nullptr, GL_DYNAMIC_DRAW);

    glBindBuffer(GL_ARRAY_BUFFER, mTexIDBuff);
    glBufferData(GL_ARRAY_BUFFER, sizeof(int) * mMaxQuads, nullptr, GL_DYNAMIC_DRAW);

    glBindBuffer(GL_ARRAY_BUFFER, mAngleBuffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(float) * mMaxQuads, nullptr, GL_DYNAMIC_DRAW);
}
void SceneRenderer::CreateBuffers(size_t maxQuads) {
    mUsedQuads = 0;
    glGenVertexArrays(1, &mVAO2);
    glBindVertexArray(mVAO2);
    glGenBuffers(1, &mVertexBuffer);
    glGenBuffers(1, &mTextureBuffer);
    glGenBuffers(1, &mColorBuffer);
    glGenBuffers(1, &mTexCoordBuffer);
    glGenBuffers(1, &mPositionBuffer);
    glGenBuffers(1, &mAngleBuffer);
    glGenBuffers(1, &mTexIDBuff);

    glGenBuffers(1, &sIndexBuffer);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, sIndexBuffer);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(SOLSTICE_RENDER_TRIANGLES), SOLSTICE_RENDER_TRIANGLES, GL_STATIC_DRAW);

    ResizeBuffers(maxQuads);
    float verts[] = {0.0, 0.0,
                    0.0, 1.0,
                    1.0, 1.0,
                    1.0, 0.0};
    float texCoords[] = {0.0, 1.0,
                        0.0, 0.0,
                        1.0, 0.0,
                        1.0, 1.0};

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, sIndexBuffer);
    glBindBuffer(GL_ARRAY_BUFFER, mVertexBuffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(verts), verts, GL_STATIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, mTextureBuffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(texCoords), texCoords, GL_STATIC_DRAW);

    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);
    glEnableVertexAttribArray(2);
    glEnableVertexAttribArray(3);
    glEnableVertexAttribArray(4);
    glEnableVertexAttribArray(5);
    glEnableVertexAttribArray(6);

    glBindBuffer(GL_ARRAY_BUFFER, mVertexBuffer);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, NULL);

    glBindBuffer(GL_ARRAY_BUFFER, mTextureBuffer);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, NULL);

    glBindBuffer(GL_ARRAY_BUFFER, mColorBuffer);
    glVertexAttribPointer(2, 4, GL_FLOAT, GL_FALSE, 0, NULL);
    glVertexAttribDivisor(2, 1);

    glBindBuffer(GL_ARRAY_BUFFER, mTexCoordBuffer);
    glVertexAttribPointer(3, 4, GL_FLOAT, GL_FALSE, 0, NULL);
    glVertexAttribDivisor(3, 1);

    glBindBuffer(GL_ARRAY_BUFFER, mPositionBuffer);
    glVertexAttribPointer(4, 4, GL_FLOAT, GL_FALSE, 0, NULL);
    glVertexAttribDivisor(4, 1);

    glBindBuffer(GL_ARRAY_BUFFER, mTexIDBuff);
    glVertexAttribIPointer(5, 1, GL_INT, 0, NULL);
    glVertexAttribDivisor(5, 1);

    glBindBuffer(GL_ARRAY_BUFFER, mAngleBuffer);
    glVertexAttribPointer(6, 1, GL_FLOAT, GL_FALSE, 0, NULL);
    glVertexAttribDivisor(6, 1);

    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    glDisableVertexAttribArray(0);
    glDisableVertexAttribArray(1);
    glDisableVertexAttribArray(2);
    glDisableVertexAttribArray(3);
    glDisableVertexAttribArray(4);
    glDisableVertexAttribArray(5);
    glDisableVertexAttribArray(6);
}
void SceneRenderer::Buffer() {
    if(!mUsedVertices) {
        return;
    }
    glBindVertexArray(mVAO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mIBO);
    glBufferSubData(GL_ELEMENT_ARRAY_BUFFER,
                    0,
                    mUsedIndices * sizeof(GLuint),
                    &mIndices[0]);
    glBindBuffer(GL_ARRAY_BUFFER, mVBO);
    glBufferSubData(GL_ARRAY_BUFFER,
                 0,
                 mUsedVertices * sizeof(Vertex2f),
                 &mVerts[0]);
}
void SceneRenderer::ClearBuffers() {
    mUsedVertices = 0;
    mUsedIndices = 0;
    mLastIndex = 0;
    mUsedQuads = 0;
}
void SceneRenderer::Render(ShaderProgram& sh) {
    if(!mUsedQuads) {
        return;
    }
    Vector2f norm(1.0);
    if(mPerspective) {
        norm = mNormalize.x;
    }
    sh.SetUniform1i("textures", 0);
    sh.SetUniformMatrix4fv("mv", 1, GL_FALSE, glm::value_ptr(mTransform.GetTransform()));
    sh.SetUniformMatrix4fv("proj", 1, GL_FALSE, glm::value_ptr(mProjection));
    sh.SetUniform2f("norm", norm.x, norm.y);
    Render();
}
void SceneRenderer::DrawRects(std::vector<Rectanglef> rects, Color c) {
    for(auto rect : rects) {
        DrawRect(rect, c);
    }
}
void SceneRenderer::RenderPolies(ShaderProgram& sh) {
    if(!mUsedVertices) {
        return;
    }
    Vector2f norm(1.0);
    if(mPerspective) {
        norm = mNormalize.x;
    }
    sh.SetUniformMatrix4fv("mv", 1, GL_FALSE, glm::value_ptr(mTransform.GetTransform()));
    sh.SetUniformMatrix4fv("proj", 1, GL_FALSE, glm::value_ptr(mProjection));
    sh.SetUniform2f("norm", norm.x, norm.y);
    if(mUsedVertices) {
        glBindVertexArray(mVAO);
        glDrawElements(GL_TRIANGLES, mUsedIndices, GL_UNSIGNED_INT, 0);
        glBindVertexArray(0);
    }
}
void SceneRenderer::Render() {
    glBindVertexArray(mVAO2);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, sIndexBuffer);
    glBindBuffer(GL_ARRAY_BUFFER, mColorBuffer);
    glBufferSubData(GL_ARRAY_BUFFER,
                    0,
                    sizeof(glm::vec4) * mUsedQuads,
                    &mColors[0]);

    glBindBuffer(GL_ARRAY_BUFFER, mTexCoordBuffer);
    glBufferSubData(GL_ARRAY_BUFFER,
                    0,
                    sizeof(glm::vec4) * mUsedQuads,
                    &mTextureOffsets[0]);

    glBindBuffer(GL_ARRAY_BUFFER, mPositionBuffer);
    glBufferSubData(GL_ARRAY_BUFFER,
                    0,
                    sizeof(glm::vec4) * mUsedQuads,
                    &mPositions[0]);

    glBindBuffer(GL_ARRAY_BUFFER, mTexIDBuff);
    glBufferSubData(GL_ARRAY_BUFFER,
                    0,
                    sizeof(int) * mUsedQuads,
                    &mTextureIDs[0]);

    glBindBuffer(GL_ARRAY_BUFFER, mAngleBuffer);
    glBufferSubData(GL_ARRAY_BUFFER,
                    0,
                    sizeof(float) * mUsedQuads,
                    &mAngles[0]);
    glDrawElementsInstanced(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0, mUsedQuads);
    glBindVertexArray(0);
    glDisableVertexAttribArray(0);
    glDisableVertexAttribArray(1);
    glDisableVertexAttribArray(2);
    glDisableVertexAttribArray(3);
    glDisableVertexAttribArray(4);
    glDisableVertexAttribArray(5);
    glDisableVertexAttribArray(6);
}
void SceneRenderer::SetPerspective(float fov, float aspect, float znear, float zfar) {
    mProjection = glm::perspective(fov, aspect, znear, zfar);
    mPerspective = true;
}
void SceneRenderer::SetOrtho() {
    mProjection = glm::ortho(-mResolution.x / 2.0f, mResolution.x / 2.0f, -mResolution.y / 2.0f, mResolution.y / 2.0f);
    mPerspective = false;
}
}
