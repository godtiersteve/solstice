#ifndef EVENTOBSERVER_H_INCLUDED
#define EVENTOBSERVER_H_INCLUDED
#include "engine/gameobject.h"
#include <vector>
namespace solstice {
enum SolsticeEvent {
    SOL_EVENT_NONE,
    SOL_EVENT_IDLE,
    SOL_EVENT_DETECTED,
    SOL_EVENT_SOUNDCREATED,
    SOL_EVENT_GRABBED,
    SOL_EVENT_THROWN,
    SOL_EVENT_DAMAGED,
    SOL_EVENT_LOWHEALTH,
    SOL_EVENT_H2O_LOW,
    SOL_EVENT_BARRIERBROKEN,
    SOL_EVENT_ATTACKING,
    SOL_EVENT_JUMP,
    SOL_EVENT_HURLED,
    SOL_EVENT_DESTROYED,
    SOL_EVENT_ALLY_DESTROYED,
    SOL_EVENT_LEADER_DESTROYED,
    SOL_EVENT_RALLY,
    SOL_EVENT_WATERTOUCH,
    SDL_EVENT_WATERSPOTTED,
    SOL_EVENT_GROUNDTOUCH,
    SOL_EVENT_WALLTOUCH,
    SOL_EVENT_ENTITYTOUCH,
    SOL_EVENT_ALLYTOUCH,
    SOL_EVENT_ENEMYTOUCH,
    SOL_EVENT_KNOCKDOWN,
    SOL_EVENT_ALARM_ENABLED,
    SOL_EVENT_ALARM_DISABLED,
    SOL_EVENT_ENTITY_NEAR,
    SOL_EVENT_ALLY_NEAR,
    SOL_EVENT_ENEMY_NEAR,
    SOL_EVENT_ENEMY_SEEN,
    SOL_EVENT_ALLY_SEEN,
    SOL_EVENT_ENEMY_ESCAPED,
    SOL_EVENT_MESSAGE_SENT,
    SOL_EVENT_CUTSCENE_TRIGGER,
    SOL_EVENT_TARGET_CAUGHT,
    SOL_EVENT_COMM_SENT,
    SOL_EVENT_PAUSE
};
template<class E, class O>
class EventObserver {
public:
    virtual ~EventObserver() {}
    virtual void OnNotify(const O& entity, E event) =  0;
};

template<class E, class O>
class EventSubject {
protected:
    std::vector<EventObserver<E, O>*> mObservers;
public:
    void AddObserver(EventObserver<E, O>* obs) {mObservers.push_back(obs);}
    void RemoveObserver(EventObserver<E, O>* obs) {
        for(auto i = mObservers.begin(); i != mObservers.end(); i++) {
            if(*i == obs) {
                i = mObservers.erase(i);
                return;
            }
        }
    }
    void Notify(const O& entity, E event) {
        for(size_t i = 0; i < mObservers.size(); i++) {
            mObservers[i]->OnNotify(entity, event);
        }
    }
};

typedef EventObserver<SolsticeEvent, GameObject> GameObjectEventObserver;
typedef EventSubject<SolsticeEvent, GameObject> GameObjectEventSubject;
}

#endif // EVENTOBSERVER_H_INCLUDED
