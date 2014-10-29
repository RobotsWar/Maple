#include <stdlib.h>
#include <wirish/wirish.h>
#include <servos.h>
#include <terminal.h>
#include <main.h>
 
// Force init to be called *first*, i.e. before static object allocation.
// Otherwise, statically allocated objects that need libmaple may fail.
__attribute__((constructor)) void premain()
{
    init();
}

int main(void)
{
    setup();

    while (true) {
        loop();
    }

    return 0;
}

