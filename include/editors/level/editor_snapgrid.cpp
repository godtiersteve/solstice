#include "editor_snapgrid.h"

namespace solstice {
SnapGrid::SnapGrid() {
    Init(0, 0, 0, 0);
    mTextureID = 0;
}
SnapGrid::SnapGrid(Vector2f origin, int rows, int cols, int cellSize) {
    Init(origin, rows, cols, cellSize);
    mTextureID = 0;
}
void SnapGrid::Resize(int rows, int cols, int cellSize) {
    mRows = rows;
    mColumns = cols;
    mCellSize = cellSize;
    mCells.resize(rows * cols);
    for(size_t i = 0; i < cols; i++) {
        for(size_t j = 0; j < rows; j++) {
            Vector2f pos = Vector2f(i * cellSize, j * cellSize) + mOrigin;
            mCells[i * rows + j ].mRect = Rectanglef(pos, cellSize, cellSize);
        }
    }
}
void SnapGrid::Init(Vector2f origin, int rows, int cols, int cellSize) {
    mOrigin = origin;
    Resize(rows, cols, cellSize);
}
GridCell SnapGrid::InGrid(IShapef* shape) {
    Rectanglef rect = shape->GetAABB();
    GridCell range = GetCells(mOrigin.x, mOrigin.y, mCellSize, rect);
    for(int i = range.left; i <= range.right; i++) {
        for(int j = range.bottom; j <= range.top; j++) {
            if(CelInside(j, i)) {
                mCells[i * mRows + j].mOccupied = true;
            }
        }
    }
    return range;
}
void SnapGrid::Snap(IShapef* shape) {
    Rectanglef rect = shape->GetAABB();
    GridCell cel = GetCells(mOrigin.x, mOrigin.y, mCellSize, rect);
    if(cel) {
        Vector2f pos = Vector2f(cel.left * mCellSize, cel.bottom * mCellSize) + mOrigin;
        shape->SetPosition(pos);
    }
}
void SnapGrid::SnapX(IShapef* shape) {
    Rectanglef rect = shape->GetAABB();
    GridCell cel = GetCells(mOrigin.x, mOrigin.y, mCellSize, rect);
    if(cel) {
        Vector2f pos;
        pos.x = cel.left * mCellSize + mOrigin.x;
        pos.y = shape->GetPosition().y;
        shape->SetPosition(pos);
    }
}
void SnapGrid::SnapY(IShapef* shape) {
    Rectanglef rect = shape->GetAABB();
    GridCell cel = GetCells(mOrigin.x, mOrigin.y, mCellSize, rect);
    if(cel) {
        Vector2f pos;
        pos.x = shape->GetPosition().x;
        pos.y = cel.bottom * mCellSize + mOrigin.y;
        shape->SetPosition(pos);
    }
}
void SnapGrid::Snap(ResizableObject* obj) {
    Rectanglef rect = obj->GetHitRect();
    GridCell cel = GetCells(mOrigin.x, mOrigin.y, mCellSize, rect);
    if(cel) {
        Vector2f pos = Vector2f(cel.left * mCellSize, cel.bottom * mCellSize) + mOrigin;
        obj->SetPosition(pos);
    }
}
void SnapGrid::SnapX(ResizableObject* obj) {
    Rectanglef rect = obj->GetHitRect();
    GridCell cel = GetCells(mOrigin.x, mOrigin.y, mCellSize, rect);
    if(cel) {
        obj->SetX(cel.left * mCellSize + mOrigin.x);
    }
}
void SnapGrid::SnapY(ResizableObject* obj) {
    Rectanglef rect = obj->GetHitRect();
    GridCell cel = GetCells(mOrigin.x, mOrigin.y, mCellSize, rect);
    if(cel) {
        obj->SetY(cel.bottom * mCellSize + mOrigin.y);
    }
}
GridCell SnapGrid::InGrid(Rectanglef rect) {
    GridCell range = GetCells(mOrigin.x, mOrigin.y, mCellSize, rect);
    for(int i = range.left; i <= range.right; i++) {
        for(int j = range.bottom; j <= range.top; j++) {
            if(CelInside(j, i)) {
                mCells[i * mRows + j].mOccupied = true;
            }
        }
    }
    return range;
}

void SnapGrid::DrawGrid(SceneRenderer& rend) {
    for(size_t i = 0; i < mCells.size(); i++) {
        //rend.DrawRect(mCells[i], Color(1.0, 1.0, 1.0, 1.0));
        Color c = COL_WHITE;
        if(mCells[i].mOccupied) {
            c = COL_BLUE;
        }
        rend.DrawQuadPoly(mCells[i].mRect, mTextureID, c);
    }
}
bool SnapGrid::CelInside(int row, int col) {
    return row >= 0 && row < mRows && col >= 0 && col < mColumns;
}
void SnapGrid::ResetGrid() {
    for(size_t i = 0; i < mCells.size(); i++) {
        mCells[i].mOccupied = false;
    }
}
}
