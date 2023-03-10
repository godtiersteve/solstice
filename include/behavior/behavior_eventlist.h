#ifndef BEHAVIOR_EVENTLIST_H_INCLUDED
#define BEHAVIOR_EVENTLIST_H_INCLUDED

#include <unordered_map>
#include <memory>
#include "behavior.h"
#include "events/eventobserver.h"

namespace solstice {
    class BehaviorEventList {
    public:
        std::unordered_multimap<int, std::shared_ptr<Behavior> > mBehaviorList;
        inline void AddBehavior(std::shared_ptr<Behavior> behavior) {
            for(size_t i = 0; i < behavior->mEventList.size(); i++) {
                mBehaviorList.insert({behavior->mEventList[i], behavior});
            }
        }
        void AddBehaviors(std::vector<std::shared_ptr<Behavior> > behaviors) {
            for(size_t i = 0; i < behaviors.size(); i++) {
                AddBehavior(behaviors[i]);
            }
        }
    };
}


#endif // BEHAVIOR_EVENTLIST_H_INCLUDED
