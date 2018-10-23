#include "mbed.h" 
#include "serial.h"

DigitalOut led(LED3);

int _data_tx[_data_tx_size] = {0};
int _data_rx[_data_rx_size] = {0};

RawSerial pc(USBTX, USBRX, 115200);
SerialTW stw(&pc);

Timer tm_main;
Ticker tk_update;
Ticker tk_serial;

volatile bool _flagControl = false;
volatile bool _flagSerialTx = false;

void IRQ_Control()
{
    _flagControl = true;
}

void IRQ_SerialTx()
{
    _flagSerialTx = true;
}

void IRQ_SerialRx()
{
    led = !led;
    char ch = pc.getc();
    stw.read(ch, _data_rx);
}

int main()
{
    tm_main.start();

    const float control_cycle_sec = 0.001;
    const float serialtx_cycle_sec = 0.01;
    tk_update.attach(&IRQ_Control, control_cycle_sec);
    tk_serial.attach(&IRQ_SerialTx, serialtx_cycle_sec);

    pc.attach(&IRQ_SerialRx, Serial::RxIrq);

    /* store variable */
    int time_us = 0;
    int time_us_diff = 0;
    int time_us_z1 = 0;

    while(1) {
        if (_flagControl)
        {
            _flagControl = false;
            time_us = tm_main.read_us();
            time_us_diff = (time_us - time_us_z1)*0.2 + time_us_diff*0.8;
            time_us_z1 = time_us;
        }
        if (_flagSerialTx)
        {
            _flagSerialTx = false;
            _data_tx[0] = _data_rx[0];
            _data_tx[1] = time_us/1000;
            _data_tx[2] = time_us_diff;
            stw.write(_data_tx);
        }
    }
}
