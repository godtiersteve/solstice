#ifndef GUIEVENT_H_INCLUDED
#define GUIEVENT_H_INCLUDED

#include "eventobserver.h"
namespace solstice {
    class GUIEntity;
    enum SolsticeGUIEvent {
        SOL_EVENT_BUTTON_PRESSED,
        SOL_EVENT_BUTTON_RELEASED,
        SOL_EVENT_INTERFACE_CLOSED,
        SOL_EVENT_INTERFACE_OPENED,
        SOL_EVENT_INTERFACE_ACTIVE,
        SOL_EVENT_INTERFACE_INACTIVE,
        SOL_EVENT_TOOLTIP_SHOW,
        SOL_EVENT_TOOLTIP_HIDE
    };
    typedef EventObserver<SolsticeGUIEvent, GUIEntity> GUIEventObserver ;
    typedef EventSubject<SolsticeGUIEvent, GUIEntity> GUIEventSubject;
}
#endif // GUIEVENT_H_INCLUDED
