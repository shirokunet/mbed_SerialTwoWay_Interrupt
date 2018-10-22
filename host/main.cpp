#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "serial.h"
#include "unistd.h"

#define device "/dev/ttyACM0"
#define data_size 2

serial_t serial;

bool read(int data_rx[], int timeout_ms=10)
{
    uint8_t buf[256];
    int ret;

    /* Read up to buf size or xx ms timeout */
    if ((ret = serial_read(&serial, buf, sizeof(buf), timeout_ms)) < 0) {
        fprintf(stderr, "serial_read(): %s\n", serial_errmsg(&serial));
        exit(1);
    }
    // printf("read %d bytes: _%s_\n", ret, buf);

    if (ret == 0)
    {
        printf("Could Not Find Data.\n");
        return false;
    }

    /* convert */
    char c[128];
    snprintf(c, 128, "%s", buf);

    /* split */
    char* tp[data_size+1];
    for (int i = 0; i < data_size+1; ++i)
    {
        if (i == 0)
            tp[i] = strtok(c, ",");     // get s
        else
            tp[i] = strtok(NULL, ",");  // get data
    }

    /* check start signal */
    if (c[0] == 's')
    {
        for (int i = 0; i < data_size; ++i)
        {
            data_rx[i] = atoi(tp[i+1]);
        }
    }

    /* print rx data*/
    printf("rx: s,");
    for (int i = 0; i < data_size; ++i)
        printf("%d,", data_rx[i]);
    printf("\n");

    return true;
}

void write(int data_tx[])
{
    unsigned char s[20] = "";
    unsigned char data_s[16] = "";
    for (int i = 0; i < data_size; ++i)
    {
        unsigned char input[16];
        snprintf((char *)input, sizeof input, "%d,", data_tx[i]);
        strncat((char *) data_s, (char *) input, 16);
    }
    snprintf((char *)s, sizeof s, "s,%s\r\n", data_s);
    printf("tx: %s", s);
    serial_write(&serial, s, sizeof(s));
}

int main(void) {
    serial_open(&serial, device, 115200);
    
    int data_tx[data_size] = {0}; 
    int data_rx[data_size] = {0}; 

    write(data_tx);
    while(1)
    {
        data_tx[0]++;
        data_tx[1]--;
        write(data_tx);
        read(data_rx, 10);
    }

    serial_close(&serial);
    return 0;
}