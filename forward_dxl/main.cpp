#include <stdlib.h>
#include <wirish/wirish.h>
#include <servos.h>
#include <terminal.h>
#include <main.h>
#include <usb_cdcacm.h>

#define DIRECTION_485   31
#define DIRECTION_TTL   30
bool transmitting = false;
bool started = false;

static void receiveMode()
{
    Serial3.waitDataToBeSent();
    digitalWrite(BOARD_LED_PIN, LOW);
    asm volatile("nop");
    // Disabling transmitter
    Serial3.enableTransmitter(false);

    // Sets the direction to receiving
    digitalWrite(DIRECTION_TTL, HIGH);
    digitalWrite(DIRECTION_485, LOW);

    // Enabling the receiver
    Serial3.enableReceiver(true);
    asm volatile("nop");
    delay_us(5);
}

static void transmitMode()
{
    digitalWrite(BOARD_LED_PIN, HIGH);
    asm volatile("nop");
    // Disabling the receiver
    Serial3.enableReceiver(false);

    // Sets the direction to transmitting
    digitalWrite(DIRECTION_TTL, LOW);
    digitalWrite(DIRECTION_485, HIGH);

    // Enabling the transmitter
    // pinMode(Serial3.txPin(), PWM);
    Serial3.enableTransmitter(true);
    asm volatile("nop");
    delay_us(5);
}

void goForward(int baud)
{
    pinMode(BOARD_LED_PIN, OUTPUT);
    pinMode(DIRECTION_TTL, OUTPUT);
    pinMode(DIRECTION_485, OUTPUT);
    Serial3.begin(baud);
    receiveMode();
    started = true;
}

void setup()
{
    terminal_init(&SerialUSB);

    goForward(57600);
//    goForward(1000000);
}

/**
 * Setup function
 */
TERMINAL_COMMAND(go, "Run the forward")
{
    int br = atoi(argv[0]);
    terminal_io()->print("Starting @");
    terminal_io()->println(br);

    goForward(br);
}

/**
 * Loop function
 */
void loop()
{
    static unsigned int t;

    if (started) {
        while (Serial3.available()) {
            char buffer[64];
            unsigned int n = 0;
            while (Serial3.available() && n<sizeof(buffer)) {
                buffer[n++] = Serial3.read();
            }
            SerialUSB.write(buffer, n);
        }

        if (SerialUSB.available()) {
            t = 0;
            if (!transmitting) {
                transmitting = true;
                transmitMode();
            }    
        }
        if (transmitting) {
            while (SerialUSB.available()) {
                Serial3.write(SerialUSB.read());
            }
            t++;
            if (t > 1) {
                transmitting = false;
                receiveMode();
            }
        }
        
        delay_us(3);    
    } else {
        terminal_tick();
    }
}
