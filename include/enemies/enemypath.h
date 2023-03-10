#ifndef ENEMYPATH_H_INCLUDED
#define ENEMYPATH_H_INCLUDED

#include "graphics/vector2.h"
namespace solstice {

struct EnemyPathNode {
    Vector2f position;
    float speed;
    int behaviorID;
};
class EnemyPath {
private:
    std::vector<EnemyPathNode> mNodes;
    int mCurrentNode;
public:
    EnemyPath() {mCurrentNode = -1;}
    EnemyPath(std::vector<EnemyPathNode> nodes) {mNodes = nodes; mCurrentNode = 0;}
    void AddNode(EnemyPathNode& node)       {mNodes.push_back(node);}
    void InsertNode(EnemyPathNode& node, size_t pos)    {mNodes.insert(mNodes.begin() + pos, node);}
    void RemoveNode(size_t i)                       {mNodes.erase(mNodes.begin() + i); if(mNodes.size() < mCurrentNode) {mCurrentNode =  mNodes.size() -1;}}
    EnemyPathNode& GetNode(size_t i)       {return mNodes[i];}
    size_t Size()                               {return mNodes.size();}
    void ForceNode(size_t i)                    {mCurrentNode = i;}
    void CreatePath(Vector2f start, Vector2f end, int stops);
    void CreatePathArc(Vector2f start, Vector2f end, int stops);
};
}

#endif // ENEMYPATH_H_INCLUDED
