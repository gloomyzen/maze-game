#ifdef DEBUG
#ifndef MERCENARY_BATTLES_SOUNDLIBRARYDEBUG_H
#define MERCENARY_BATTLES_SOUNDLIBRARYDEBUG_H

namespace mb::debugProfile {

    class soundLibraryDebug {
      public:
        soundLibraryDebug();
        ~soundLibraryDebug();
        static soundLibraryDebug& getInstance();

        void update();

      private:
        void soundWindow(bool* windowOpened);

        bool isOpened = false;
    };
}

#endif// MERCENARY_BATTLES_SOUNDLIBRARYDEBUG_H
#endif
