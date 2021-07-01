#include "stickButton.h"

using namespace mb::battleModule;
using namespace cocos2d::ui;

stickButton::stickButton() {
    setFocusEnabled(false);
    setName("stickButton");
    loadProperty("battle/" + this->getName(), this);
    {
        // setup settings
        auto data = getPropertyObject("settings");
        auto item = data.FindMember("waitAfterEndTouch");
        if (item != data.MemberEnd() && item->value.IsNumber()) {
            waitAfterEndTouch = item->value.GetFloat();
        }
        item = data.FindMember("stickDistance");
        if (item != data.MemberEnd() && item->value.IsNumber()) {
            stickDistance = item->value.GetFloat();
        }
        item = data.FindMember("stickInertionTime");
        if (item != data.MemberEnd() && item->value.IsNumber()) {
            stickInertionTime = item->value.GetFloat();
        }
    }

    setStickEnabled(true);
    initController();
}

stickButton::~stickButton() {
    getEmitter()->onMove.disconnectAll();
    getEmitter()->onStop.disconnectAll();
}

void stickButton::initController() {
    btn = dynamic_cast<cocos2d::Sprite*>(findNode("buttonNode"));
    if (btn) {
        smallBtn = cocos2d::Sprite::create();
        smallBtn->setName("stickSmallBtn");
        loadComponent(smallBtn);
        btn->addChild(smallBtn);
    }
    addTouchEventListener([&](Ref* sender, Widget::TouchEventType type) {
        switch (type) {
        case Widget::TouchEventType::BEGAN: {
            {
                // break active actions
                stopAllActionsByTag(static_cast<int>(stickButton::eActionType::WAIT_AFTER_END_TOUCH));
                smallBtn->stopAllActionsByTag(static_cast<int>(stickButton::eActionType::WAIT_AFTER_END_TOUCH));
            }
            startPos = getTouchBeganPosition();
            auto newPos = cocos2d::Vec2(startPos.x - btn->getContentSize().width / 2,
                                        startPos.y - btn->getContentSize().height / 2);
            btn->setPosition(newPos);
            smallBtn->setPosition(cocos2d::Vec2::ZERO);
        } break;
        case Widget::TouchEventType::MOVED: {
            auto currentTouchPos = getTouchMovePosition();
            auto newPos = cocos2d::Vec2(currentTouchPos.x - startPos.x, currentTouchPos.y - startPos.y);
            while (newPos.distance(cocos2d::Vec2::ZERO) > stickDistance) {
                if (newPos.x > 0.f) {
                    newPos.x -= .1f;
                } else {
                    newPos.x += .1f;
                }
                if (newPos.y > 0.f) {
                    newPos.y -= .1f;
                } else {
                    newPos.y += .1f;
                }
            }
            smallBtn->setPosition(newPos);
            getEmitter()->onMove.emit(newPos.x, newPos.y);
        } break;
        case Widget::TouchEventType::ENDED:
        case Widget::TouchEventType::CANCELED: {
            {
                getEmitter()->onStop.emit();
                // action move btn to start position
                auto delay = cocos2d::DelayTime::create(waitAfterEndTouch);
                auto clb = cocos2d::CallFunc::create([this]() {
                    btn->setPosition(cocos2d::Vec2::ZERO);
                    smallBtn->setPosition(cocos2d::Vec2::ZERO);
                });
                auto seq = cocos2d::Sequence::create(delay, clb, nullptr);
                seq->setTag(static_cast<int>(stickButton::eActionType::WAIT_AFTER_END_TOUCH));
                runAction(seq);

                {
                    //move stick
                    auto move = cocos2d::MoveTo::create(stickInertionTime, cocos2d::Vec2::ZERO);
                    move->setTag(static_cast<int>(stickButton::eActionType::WAIT_AFTER_END_TOUCH));
                    smallBtn->runAction(move);
                }
            }
            return true;
        } break;
        default:
            break;
        }
        return true;
    });
}

void stickButton::setStickEnabled(bool value) {
    stickEnabled = value;
    setTouchEnabled(stickEnabled);
}

bool stickButton::getStickEnabled() {
    return stickEnabled;
}
