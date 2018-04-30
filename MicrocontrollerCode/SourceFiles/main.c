#include "mbed.h"

DigitalOut myled(LED1);
DigitalIn  pushButton(USER_BUTTON);

Serial     pc(SERIAL_TX, SERIAL_RX, "STM32", 9600);

int main() {  
    while(1) {
        myled.write(pushButton.read());

        if ( pushButton.read() )
            printf("Push button pressed \n");
    }
}
