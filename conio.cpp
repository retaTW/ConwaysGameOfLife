#include "conio.h"

#if defined(_WIN32) || defined(_WIN64)
    #include <conio.h> // only on windows

    int keyboard_hit() {
        return _kbhit();
    }

    int get_char() {
        return _getch();
    }

    // nothing to recover on windows
    void keyboard_recovery() {
        return;
    }

#elif defined(__linux__)
    /*
     * Refer to "Non buffered getc(3) under GNU/Linux": http://shtrom.ssji.net/skb/getc.html
     * By Olivier Mehani, shtrom-kb@ssji.net
     */
    
    #include <stdio.h>
    #include <unistd.h>
    #include <termios.h>
    #include <sys/ioctl.h>

    namespace {
        struct termios original_tio;
        struct termios activated_tio;
        bool isActivated = false;

        auto _1 = ::tcgetattr(STDIN_FILENO,&original_tio);
        auto _2 = activated_tio = original_tio;
        auto _3 = activated_tio.c_lflag &= (~ICANON & ~ECHO);
    }

    int keyboard_hit() {
        if (! ::isActivated) {
            ::isActivated = true;
            ::tcsetattr(STDIN_FILENO,TCSANOW,&activated_tio);
        }

        int is_hit;
        ::ioctl(STDIN_FILENO, FIONREAD, &is_hit);
        return is_hit;
    }

    int get_char() {
        return getchar();
    }

    void keyboard_recovery() {
        /* restore the former settings */
        ::tcsetattr(STDIN_FILENO,TCSANOW,&original_tio);
        ::isActivated = false;
        return;
    }


#endif
