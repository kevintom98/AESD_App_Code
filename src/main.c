/****************************************************************************************************
  File name   : main.c
  Created     : 04-Dec-2022
  Author      : Kevin Tom
                MS-ECEE-ESE
                CU Boulder, CO
  Email       : keto9919@colorado.edu
  Description : The code contains the super loop implementation for aquiring data from GPS sensor,
                parsing and sending it over MQTT/Socket code.

                Command Line Arguments:
                1. GPS Port  : Port to which GPS is connected to (eg :/dev/ttyUSB0)
                2. Baud Rate : Baud rate for GPS connection (eg: 9600) 

  Reference   : Parsing NMEA sentences
                //https://www.beyondlogic.org/ansi-c-basic-lightweight-nmea-parser-for-gps/
  
  How to compile the script
  -------------------------
  1. Open terminal in the same directory as this file.
  2. Type "make" to compile the file using gcc compiler.
  3. Run "./ClientApp <GPS_PORT> <Baud_Rate>" command to run the code.
  4. For crosscompiling do "make clean" to clear the already geenrated executables (if any).
******************************************************************************************************/


#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>


#include "UART.h"
#include "MQTT.h"


#define LOGGING (false)

int main(int argc, char **argv)
{
    int baud_rate = 0, fd = 0;
    char *p;

    //Default value if nothing is there to send
    char payload[50] = "NO DATA";
    const char* topic = "Dhiraj/Level1";

    if(argc != 3)
    {
        printf("\n\rNeed the follwing arguments:");
        printf("\n\r[1] Serial port to which GPS is connected (eg :/dev/ttyUSB0)");
        printf("\n\r[2] Baud rate to connect (eg : 9600)\n\r");
        return 1;
    }

    //Converting command line argument to integer
    baud_rate = strtol(argv[2], &p, 10);
    
    //Opening GPS port
	if ( (fd = OpenGPSPort(argv[1], baud_rate)) < 0)
	{
		printf("Cannot open GPS port\r\n");
		return 1;
	}


    //Connect to 
    mqtt_Connect();





    //Super loop at 1Hz rate
    while(1)
    {
        //get_GPS_data(fd, payload);
        if(get_Lat_Long(fd, payload) < 0)
        {
            printf("Cannot get data from sensor\n\r!");
            continue;
        }
        #if LOGGING
            printf("PAYLOAD Being Sent: %s\n\r", payload);
        #endif


        if(publish(payload, topic) < 0)
        {
            printf("Cannot get data from sensor\n\r!");
            continue;
        }
        

        //Sleeping for 1 second
        sleep(1);
    }




    //Closing MQTT connection
    mqtt_Close_Connection();




    //Closing GPS port
    if (CloseGPSPort(fd) < 0)
    {
        printf("Cannot close GPS port");
        return 1;
    }


    return 0;
}