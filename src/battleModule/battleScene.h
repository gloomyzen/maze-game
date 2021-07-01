#ifndef MERCENARY_BATTLES_BATTLESCENE_H
#define MERCENARY_BATTLES_BATTLESCENE_H

#include "battleModule/battleField.h"
#include "battleModule/players/playerController.h"
#include "cocos2d.h"
#include "common/coreModule/nodes/nodeProperties.h"
#include "common/coreModule/scenes/windows/windowBase.h"
#include <tuple>

namespace mb::battleModule {

    class playerBase;

    class battleScene
        : public common::coreModule::nodeProperties
        , public cocos2d::Layer
        , public taskHolder {
    public:
        battleScene();
        ~battleScene();
        std::deque<nodeTasks> getTasks() override;

        void initHelpers();
        void update(float dt) override;

    private:
        void trackLookAt(cocos2d::Node* node);
        void lookAt();
        // world section
        cocos2d::Layer* world = nullptr;// layer for background //owner
        cocos2d::Node* objects = nullptr;// layer for objects and players //owner
        battleField* maze = nullptr;// owner
        playerController* plrController = nullptr;// owner
        playerBase* player = nullptr;// owner
        playerBase* enemy = nullptr;// owner
        cocos2d::Node* lookAtNode = nullptr;

        int onMoveId;
        int onStopId;

#ifdef DEBUG
        bool physicsDebugDraw = false;// for debug
#endif
    };
}// namespace mb::battleModule


#endif// MERCENARY_BATTLES_BATTLESCENE_H
