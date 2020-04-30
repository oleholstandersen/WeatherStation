#include "../rc-switch/RCSwitch.h"
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>

#define RF_RX_PIN 2

RCSwitch rf;
 
int main(int argc, char *argv[]) {
    if (wiringPiSetup() == -1) {
        printif("Could not setup WiringPi, make sure it is installed");
        return -1000;
    }
    rf = RCSwitch();
    rf.enableReceive(RF_RX_PIN);
    while(true) {
        printf("Received %08X\n", rf.getReceivedValue());
        fflush(stdout);
    }
    
}