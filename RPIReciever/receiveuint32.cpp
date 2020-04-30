#include "../rc-switch/RCSwitch.h"
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <time.h>

#define RF_RX_PIN 2

RCSwitch rf;

int main(int argc, char *argv[]) {
    if (wiringPiSetup() == -1) {
        printf("Could not setup WiringPi, make sure it is installed");
        return -1000;
    }
    rf = RCSwitch();
    rf.enableReceive(RF_RX_PIN);
    unsigned long latestValue = 0;
    unsigned long valueCount = 0;
    time_t latestTime = time(nullptr);
    sleep(1);
    printf("Listening...");
    fflush(stdout);
    while(true) {
        if (rf.available()) {
            unsigned long value = rf.getReceivedValue();
            time_t now = time(nullptr);
            if (value == latestValue && (difftime(now, latestTime) < 1.0)) {
                printf("+");
            }
            else {
                valueCount++;
                printf("\nReceived value %08u: %08X (%c)", valueCount, value, value & 0xFF);
                latestValue = value;
                latestTime = time(nullptr);
            }
            fflush(stdout);
            rf.resetAvailable();
        }
    }
}
