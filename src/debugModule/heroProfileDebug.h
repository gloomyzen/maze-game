#ifdef DEBUG
#ifndef MERCENARY_BATTLES_HEROPROFILEDEBUG_H
#define MERCENARY_BATTLES_HEROPROFILEDEBUG_H

namespace mb::debugProfile {

    class heroProfileDebug {
      public:
        heroProfileDebug();
        ~heroProfileDebug();
        static heroProfileDebug& getInstance();

        void update();

      private:
        void profileWindow(bool* windowOpened);
        void leftList();
        void rightData();

        bool isOpened = false;
        int lastOpenedHero = -1;
    };
}

#endif// MERCENARY_BATTLES_HEROPROFILEDEBUG_H
#endif
