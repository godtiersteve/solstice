#ifndef EDITOR_SNAPGRID_H_INCLUDED
#define EDITOR_SNAPGRID_H_INCLUDED

#include "geometry/rectangle.h"
#include "geometry/geometry.h"
#include "graphics/scenerenderer.h"
#include "engine/resizableobject.h"

namespace solstice {
    struct SnapGridCel {
        bool mOccupied;
        Rectanglef mRect;
        SnapGridCel() {mOccupied = false;}
        SnapGridCel(Rectanglef rect) {mRect = rect;}
    };
    class SnapGrid {
        std::vector<SnapGridCel> mCells;
        int mRows, mColumns, mCellSize;
        Vector2f mOrigin;
    public:
        int mTextureID;
        SnapGrid();
        SnapGrid(Vector2f origin, int rows, int cols, int cellSize);
        void Init(Vector2f origin, int rows, int cols, int cellSize);
        GridCell InGrid(IShapef* shape);
        GridCell InGrid(Rectanglef rect);
        void Snap(IShapef* shape);
        void SnapX(IShapef* shape);
        void SnapY(IShapef* shape);
        void Snap(ResizableObject* obj);
        void SnapX(ResizableObject* obj);
        void SnapY(ResizableObject* obj);
        void DrawGrid(SceneRenderer& rend);

        void Resize(int rows, int cols, int cellSize);
        int GetNumRows() {return mRows;}
        int GetNumColumns() {return mColumns;}
        int GetCellSize() {return mCellSize;}
        Vector2f GetOrigin() {return mOrigin;}
        void ResetGrid();
        inline bool CelInside(int row, int col);

    };

}


#endif // EDITOR_SNAPGRID_H_INCLUDED
