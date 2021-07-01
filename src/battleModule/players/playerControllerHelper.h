#ifndef SLAYER_ESCAPE_PLAYERCONTROLLERHELPER_H
#define SLAYER_ESCAPE_PLAYERCONTROLLERHELPER_H

#include <tuple>
#include <string>

namespace mb::battleModule {
    enum class ePlayerMoveXDirection {
        NONE = 0,
        LEFT,
        RIGHT
    };
    enum class ePlayerMoveYDirection {
        NONE = 0,
        UP,
        DOWN
    };
    enum class ePlayerMoveIntensive {
        PLAYER_STOP = 0,
        PLAYER_WALK,
        PLAYER_RUN
    };

    std::pair<std::pair<ePlayerMoveXDirection, ePlayerMoveYDirection>, ePlayerMoveIntensive> getPlayerStateByStick(float x, float y, float distance);

    std::pair<ePlayerMoveXDirection, ePlayerMoveYDirection> getPlayerDirectionByStick(float x, float y, float distance);
}

#endif// SLAYER_ESCAPE_PLAYERCONTROLLERHELPER_H
