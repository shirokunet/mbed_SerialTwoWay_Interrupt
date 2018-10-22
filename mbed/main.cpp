#include "mbed.h" 
#include "serial.h"

DigitalOut led(LED3);


const int data_tx_size = 2;
const int data_rx_size = 2;

int data_tx[data_tx_size] = {0};
int data_rx[data_rx_size] = {0};

RawSerial pc(USBTX, USBRX, 115200);
SerialTW stw(&pc, data_tx_size, data_rx_size);


volatile bool _interruptSerial = false;

void Rx_interrrupt()
{
    led = !led;
    if (stw.read(data_rx))
        _interruptSerial = true;
}

int main() {
    pc.attach(&Rx_interrrupt, Serial::RxIrq);

    while(1) {
        if (_interruptSerial)
        {
            _interruptSerial = false;
            pc.attach(NULL, Serial::RxIrq);

            data_tx[0] = data_rx[0];
            data_tx[1] = data_rx[1];
            data_tx[2] = data_rx[2]; 
            stw.write(data_tx);

            pc.attach(&Rx_interrrupt, Serial::RxIrq);
        }
    }
}