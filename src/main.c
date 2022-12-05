/****************************************************************************************************
  File name   : main.c
  Created     : 04-Dec-2022
  Author      : Kevin Tom
                MS-ECEE-ESE
                CU Boulder, CO
  Email       : keto9919@colorado.edu
  Description : 
  Reference   : parsing NMEA sentences
                //https://www.beyondlogic.org/ansi-c-basic-lightweight-nmea-parser-for-gps/
  How to compile the script
  -------------------------
  1. Open terminal in the same directory as this file.
  2. Type "make" to compile the file using gcc compiler.
  3. Run "./UART <GPS_PORT>" command to run the code.
  4. For crosscompiling do "make clean" to clear the already geenrated executables (if any).
******************************************************************************************************/

#include "UART.h"
#include "MQTT.h"
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>


int main(int argc, char **argv)
{
    int baud_rate = 0;
    char *p;
    char payload[50];

    if(argc != 3)
    {
        printf("\n\rNeed the follwing arguments:");
        printf("\n\r[1] Serial port to which GPS is connected (eg :/dev/ttyUSB0)");
        printf("\n\r[2] Baud rate to connect (eg : 9600)");
        return 1;
    }

    //Converting command line argument to integer
    baud_rate = strtol(argv[2], &p, 10);


    while(1)
    {
        get_GPS_data(argv[1], baud_rate, payload);
        publish(payload);
    }


    return 0;
}