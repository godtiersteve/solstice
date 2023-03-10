#ifndef ANIMATOR_H_INCLUDED
#define ANIMATOR_H_INCLUDED

#include <vector>
#include <algorithm>
#include <string>

//TODO : Ensure that the classes using this interface have the proper overloads
namespace solstice {
    enum AnimationStatus {ANIM_STOPPED, ANIM_PLAYING, ANIM_PAUSED};
    template<class T>
    class Animator {
    protected:
        int mLoops, mLoopsPerformed;
        unsigned int mUpdateInterval, mUpdatesPerformed;
        double mPauseDur;
        unsigned int mLoopFrame;
        int mID;
        size_t mCurFrameNum;
        bool mReverses;
        AnimationStatus mStatus;
        std::vector<T> mAnimation;
        T mCurFrame;
        std::string mRef;
        int mFlags;
    public:
        int mPriority;
        Animator();
        T& operator[](size_t i)                     {return mAnimation[i];}

        void Init();
        void SetLoops(int l)                        {mLoops = l;}
        void SetUpdateInterval(unsigned int i)      {mUpdateInterval = i;}
        void SetLoopFrame(unsigned int i)           {mLoopFrame = i;}
        void SetRef(std::string ref)                {mRef = ref;}
        void SetName(std::string name)              {mRef = name;}
        std::string GetRef()                        {return mRef;}
        std::string GetName()                       {return mRef;}
        int GetLoops()                              {return mLoops;}
        unsigned int GetLoopsPeformed()             {return mLoopsPerformed;}
        unsigned int GetUpdateInterval()            {return mUpdateInterval;}
        unsigned int GetUpdatesPerformed()          {return mUpdatesPerformed;}
        double GetPauseDuration()             {return mPauseDur;}
        unsigned int GetLoopFrame()                 {return mLoopFrame;}
        unsigned int GetCurFrameNum()               {return mCurFrameNum;}
        size_t GetNumFrames()                       {return mAnimation.size();}
        size_t Size()                               {return mAnimation.size();}
        bool GetReverses()                          {return mReverses;}
        AnimationStatus GetStatus()                 {return mStatus;}
        T& GetFrame(size_t i)                       {return mAnimation[i];}
        T& GetCurrentFrame()                        {return mCurFrame;}
        void Clear()                                {mAnimation.clear();}
        std::vector<T>& GetFrames()                 {return mAnimation;}
        int GetID()                                 {return mID;}
        void SetID(int id)                          {mID = id;}
        bool Finished();
        bool Empty()                                {return mAnimation.empty();}
        void SetFlags(int flags)                    {mFlags = flags;}
        void AddFlags(int flags)                    {mFlags |= flags;}
        void RemoveFlags(int flags)                 {mFlags &= ~flags;}
        int GetFlags()                              {return mFlags;}
        bool HasFlag(int flag)                      {return mFlags & flag;}

        void AddFrame(T frame)                      {mAnimation.push_back(frame);}
        void AddFrame(T f, size_t pos)              {mAnimation.insert(mAnimation.begin() + pos, f);}
        void Start();
        void Play();
        void Pause(double duration = -1);
        void Stop();
        void Reset();
        void Reverse();
        virtual void Update(float dur = 1.0f) = 0;
    };
    template<class T>
    T& GetAnimation(std::vector<T>& anims, std::string name);
    template<class T>
    T& GetAnimation(std::vector<T>& anims, int id);
    template<class T>
    Animator<T>::Animator() {
        mPriority = -1;
        Init();
    }
    template<class T>
    void  Animator<T>::Init() {
        mLoops = -1;
        mLoopsPerformed = 0;
        mUpdateInterval = 0;
        mUpdatesPerformed = -1;
        mLoopFrame = 0;
        mUpdatesPerformed = 0;
        mCurFrameNum = 0;
        mReverses = false;
        mID = -1;
        mStatus = ANIM_PAUSED;
    }
    template<class T>
    bool Animator<T>::Finished() {
        if(mAnimation.empty()) {
            return true;
        }
        return mLoops >= 0 && mLoopsPerformed > mLoops;
    }
    template<class T>
    void Animator<T>::Start() {
        if(!mAnimation.empty()) {
            mCurFrame = mAnimation[0];
        }
        Play();
        Update();
    }
    template<class T>
    void Animator<T>::Play() {
        mStatus = ANIM_PLAYING;
        if(!mAnimation.empty()) {
            mCurFrame = mAnimation[0];
        }
    }
    template<class T>
    void Animator<T>::Pause(double duration) {
        mStatus = ANIM_PAUSED;
        mPauseDur = duration;
    }
    template<class T>
    void Animator<T>::Stop() {
        mStatus = ANIM_STOPPED;
        mPauseDur = 0;
        mLoopsPerformed = 0;
    }
    template<class T>
    void Animator<T>::Reset() {
        if(mAnimation.empty()) {
            return;
        }
        Stop();
        Start();
    }
    template<class T>
    void Animator<T>::Reverse() {
        std::reverse(mAnimation.begin(), mAnimation.end());
    }
    template<class T>
    T& GetAnimation(std::vector<T>& anims, std::string name) {
        for(auto& anim : anims) {
            if(anim.GetRef() == name) {
                return anim;
            }
        }
        return anims[0];
    }
    template<class T>
    T& GetAnimation(std::vector<T>& anims, int id) {
        for(auto& anim : anims) {
            if(anim.GetID() == id) {
                return anim;
            }
        }
        return anims[0];
    }

}

#endif // ANIMATOR_H_INCLUDED
