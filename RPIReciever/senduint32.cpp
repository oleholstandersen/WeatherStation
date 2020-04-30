#include "../rc-switch/RCSwitch.h"
#include <stdlib.h>
#include <stdio.h>
#include <limits.h>


#define RF_TX_PIN 0

RCSwitch rf;
 
int main(int argc, char *argv[]) {
    if (argc != 2) {
        printf("Invalid number of arguments");
        printf("Usage: %s number");
        return -1000;
    }
    long long int llVal = atoll(arg);
    if (llVal > ULONG_MAX) {
        printf("Number must be between 0 and %i", ULONG_MAX);
        return -1000;
    }
    rf.send((unsigned long)llVal, 32);
}
