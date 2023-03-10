#ifndef LEVELPROP_H_INCLUDED
#define LEVELPROP_H_INCLUDED

namespace solstice {
enum PropType {PROP_BOX};
class LevelProp {
    int mType;
public:
    bool IsType(int i) {return mType == i;}
};
}


#endif // LEVELPROP_H_INCLUDED
