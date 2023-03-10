#ifndef FRAMEINFO_H_INCLUDED
#define FRAMEINFO_H_INCLUDED

namespace solstice {
struct FrameInfo {
    int startup, active, recovery;
    FrameInfo(int start = 0, int act = 0, int rec = 0) {
        startup = start;
        active = act;
        recovery = rec;
    }
};
}


#endif // FRAMEDATA_H_INCLUDED
