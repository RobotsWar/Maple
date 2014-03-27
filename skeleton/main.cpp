#include <cstdlib>
#include <wirish/wirish.h>
#include <servos.h>
#include <terminal.h>
#include <main.h>

TERMINAL_COMMAND(hello, "Prints hello world")
{
    terminal_io()->println("Hello world");
}

/**
 * Setup function
 */
void setup()
{
}

/**
 * Loop function
 */
void loop()
{
}
