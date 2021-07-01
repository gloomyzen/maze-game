#include "playerController.h"
#include "common/debugModule/logManager.h"
#include "common/utilityModule/stringUtility.h"
#include "spine/spine-cocos2dx.h"

using namespace mb::battleModule;

playerController::playerController() {
    setName("playerController");
    loadProperty("battle/" + this->getName(), this);
    initJoystick();
}

void playerController::initJoystick() {
    joystick = new stickButton();
    addChild(joystick);
    auto scene = cocos2d::Director::getInstance()->getRunningScene();
    if (auto windowNode = scene->findNode("windowViewer")) {
        auto order = windowNode->getLocalZOrder();
        scene->addChild(this, order - 1);
    } else {
        LOG_ERROR("playerController::initJoystick: Can't find windowViewer node");
        return;
    }
    LOG_INFO("playerController::initJoystick: Init control");
}

playerController::~playerController() {}

void playerController::enableControl() {
    controlAvailable = true;
    // todo action for show controls
}

void playerController::disableControl() {
    controlAvailable = false;
    // todo action for hide controls
}
