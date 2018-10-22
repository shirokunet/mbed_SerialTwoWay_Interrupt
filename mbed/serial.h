#ifndef SERIALTW_h
#define SERIALTW_h

#include "mbed.h"

class SerialTW
{
    public:
        SerialTW(RawSerial *pc, int  data_tx_size=5, int  data_rx_size=3) : _pc(pc)
        {
            _data_tx_size = data_tx_size;
            _data_rx_size = data_rx_size;
        }
        bool read(int data_rx[]);
        void write(int data_tx[]);
    private:
        RawSerial *_pc;
        int _data_tx_size;
        int _data_rx_size;
};

#endif /* SERIALTW_h */
