#include "playerControllerHelper.h"

using namespace mb::battleModule;

std::pair<std::pair<ePlayerMoveXDirection, ePlayerMoveYDirection>, ePlayerMoveIntensive> mb::battleModule::getPlayerStateByStick(float x, float y, float distance) {
    auto result = std::pair<std::pair<ePlayerMoveXDirection, ePlayerMoveYDirection>, ePlayerMoveIntensive>(getPlayerDirectionByStick(x, y, distance), ePlayerMoveIntensive::PLAYER_STOP);
    auto absX = std::abs(x);
    auto absY = std::abs(y);
    auto absD = std::abs(distance);
    if ((absX > absD * .7f) || (absY > absD * .7f)) {
        result.second = ePlayerMoveIntensive::PLAYER_RUN;
    } else if ((absX > absD / 3) || (absY > absD / 3)) {
        result.second = ePlayerMoveIntensive::PLAYER_WALK;
    }
    return result;
}

std::pair<ePlayerMoveXDirection, ePlayerMoveYDirection> mb::battleModule::getPlayerDirectionByStick(float x, float y, float distance) {
    auto result = std::pair<ePlayerMoveXDirection, ePlayerMoveYDirection>(ePlayerMoveXDirection::NONE, ePlayerMoveYDirection::NONE);
    auto absX = std::abs(x);
    auto absY = std::abs(y);
    auto absD = std::abs(distance);
    if (absX > absD / 3) {
        if (x > 0.f) {
            result.first = ePlayerMoveXDirection::RIGHT;
        } else {
            result.first = ePlayerMoveXDirection::LEFT;
        }
    }
    if (absY > absD / 3) {
        if (y > 0.f) {
            result.second = ePlayerMoveYDirection::UP;
        } else {
            result.second = ePlayerMoveYDirection::DOWN;
        }
    }
    return result;
}
