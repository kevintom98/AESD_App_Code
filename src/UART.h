/****************************************************************************************************
  File name   : UART.h
  Created     : 04-Dec-2022
  Author      : Kevin Tom
                MS-ECEE-ESE
                CU Boulder, CO
  Email       : keto9919@colorado.edu
  Description : 
  How to compile the script
  -------------------------
  1. Open terminal in the same directory as this file.
  2. Type "make" to compile the file using gcc compiler.
  3. Run "./UART <GPS_PORT>" command to run the code.
  4. For crosscompiling do "make clean" to clear the already geenrated executables (if any).
******************************************************************************************************/
#if !defined(UART_H)
#define UART_H

#include <stdint.h>


//Function porototypes

int get_GPS_data(char *serial_port, int baud_rate, char *payload);


int hex2int(char *c);
int parse_comma_delimited_str(char *string, char **fields, int max_fields);
int debug_print_fields(int numfields, char **fields);
int OpenGPSPort(const char *devname, int baud_rate);
void convert_time(char *date, char *time);
float GpsToDecimalDegrees(const char* nmeaPos, char quadrant);


#endif