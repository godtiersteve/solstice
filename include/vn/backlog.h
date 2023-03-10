#ifndef BACKLOG_H_INCLUDED
#define BACKLOG_H_INCLUDED
// TODO (Stephen#1#): Finish implementing backlog.h

#include <deque>
#include <string>

using std::deque;
using std::string;

namespace solstice {
    class Backlog {
    protected:
        deque<string> mText;
        int mMaxSize;
    public:
        Backlog();
        Backlog(int maxSize);

        string GetText(int i);
        string operator[](int i);
        void PopBack();
        void PopFront();
        void PushBack(string text);
        void PushFront(string text);
    };
}

#endif // BACKLOG_H_INCLUDED
