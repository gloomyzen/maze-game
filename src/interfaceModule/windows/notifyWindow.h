#ifndef MERCENARY_BATTLES_NOTIFYWINDOW_H
#define MERCENARY_BATTLES_NOTIFYWINDOW_H

#include "cocos2d.h"
#include "common/coreModule/scenes/windows/windowBase.h"

namespace mb::interfaceModule {

    class notifyWindow
        : public common::coreModule::windowBase
        , public taskHolder {
      public:
        notifyWindow();
        ~notifyWindow() = default;
        std::deque<nodeTasks> getTasks() override;
    };
}// namespace mb::interfaceModule


#endif// MERCENARY_BATTLES_NOTIFYWINDOW_H
