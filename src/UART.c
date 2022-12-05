/****************************************************************************************************
  File name   : UART.c
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
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#include <termios.h>
#include <time.h>
#include <stdlib.h>
#include <stdbool.h>
#include "UART.h"




int get_GPS_data(char *serial_port, int baud_rate, char *payload)
{
	int fd = 0;
	char buffer[255];
	int nbytes = 0;
	int i =0;
	char *field[20];
	bool gps_signal_found = false;
  	//float lat =0.0,lon=0.0,speed = 0;
	float lat = 0;


	if(baud_rate != 9600)
	{
		printf("Baud rate not supported\n\r");
		return 1;
	}

	//Opening GPS port
	if ( (fd = OpenGPSPort(serial_port, baud_rate)) < 0)
	{
		printf("Cannot open GPS port\r\n");
		return 1;
	}

	do 
  	{
		if ((nbytes = read(fd, &buffer, sizeof(buffer))) < 0) 
    	{
			perror("Read");
			goto close_port;
		} 
    	else 
   	 	{
			if (nbytes == 0) 
      		{
				printf("No communication from GPS module, waiting for some time and trying again\r\n");
				sleep(1);
			} 
      	else 
      	{
        //printf("RAW Data: %s\r\n", buffer);
				buffer[nbytes - 1] = '\0';

        //Checking talker ID
        //Reference : https://cdn.sparkfun.com/assets/0/b/0/f/7/u-blox8-M8_ReceiverDescrProtSpec__UBX-13003221__Public.pdf
        //Page 104
        if ((strncmp(buffer, "$GP", 3) == 0) | (strncmp(buffer, "$GN", 3) == 0)) 
          {
            if (strncmp(&buffer[3], "GGA", 3) == 0) 
            {
              printf("RAW Data: %s\r\n", buffer);
              i = parse_comma_delimited_str(buffer, field, 20);
              
			  lat = GpsToDecimalDegrees(field[2],*field[3]);
//              lon = GpsToDecimalDegrees(field[4],*field[5]);
				i = (i/1);
    

            //   printf("UTC Time  :%s\r\n",field[1]);
            //   printf("Latitude  :%f\r\n",lat);
            //   printf("Longitude :%f\r\n",lon);
            //   printf("Altitude  :%sm\r\n",field[9]);
            //   printf("Satellites:%s\r\n",field[7]);
				//payload =  field[2];

				//strncpy(payload,field[2],10);
				snprintf(payload,15,"%f",lat);
				gps_signal_found = true;
            }
            // if (strncmp(&buffer[3], "RMC", 3) == 0) 
            // {
            //   i = parse_comma_delimited_str(buffer, field, 20);
            //   convert_time(field[9],field[1]);
            // }
          }
			}
		}
	}while(!gps_signal_found);


  	close_port:
	if (close(fd) < 0) 
  	{
		perror("Close");
		return 1;
	}
	return 0;
}




int hexchar2int(char c)
{
    if (c >= '0' && c <= '9')
        return c - '0';
    if (c >= 'A' && c <= 'F')
        return c - 'A' + 10;
    if (c >= 'a' && c <= 'f')
        return c - 'a' + 10;
    return -1;
}

int hex2int(char *c)
{
	int value;

	value = hexchar2int(c[0]);
	value = value << 4;
	value += hexchar2int(c[1]);

	return value;
}




int parse_comma_delimited_str(char *string, char **fields, int max_fields)
{
	int i = 0;
	fields[i++] = string;

	while ((i < max_fields) && NULL != (string = strchr(string, ','))) {
		*string = '\0';
		fields[i++] = ++string;
	}

	return --i;
}




void convert_time(char *date, char *time)
{
	//struct timespec ts;
	struct tm gpstime;
	//time_t secs;
	char tempbuf[2];
	//int ret;


	if ((strlen(date) != 6) | (strlen(time) != 9)) {
		printf("No date or time fix. Exiting\r\n");
		//return 1;
	}

	// Parse day:
	strncpy(tempbuf, (char *)date, 2);
	tempbuf[2] = '\0';
	gpstime.tm_mday = atoi(tempbuf);

	// Parse month:
	strncpy(tempbuf, (char *)date+2, 2);
	tempbuf[2] = '\0';
	gpstime.tm_mon = atoi(tempbuf) - 1;

	// Parse year:
	strncpy(tempbuf, (char *)date+4, 2);
	tempbuf[2] = '\0';
	gpstime.tm_year = atoi(tempbuf) + 100;

	// Parse hour:
	strncpy(tempbuf, (char *)time, 2);
	tempbuf[2] = '\0';
	gpstime.tm_hour = atoi(tempbuf);

	// Parse minutes:
	strncpy(tempbuf, (char *)time+2, 2);
	tempbuf[2] = '\0';
	gpstime.tm_min = atoi(tempbuf);

	// Parse seconds:
	strncpy(tempbuf, (char *)time+4, 2);
	tempbuf[2] = '\0';
	gpstime.tm_sec = atoi(tempbuf);

	printf("UTC_Date  :  %02d/%02d/%02d \r\nUTC_Time  : %02d:%02d:%02d\r\n",gpstime.tm_mday,(gpstime.tm_mon)+1,(gpstime.tm_year)%100, gpstime.tm_hour, gpstime.tm_min, gpstime.tm_sec);
}





int OpenGPSPort(const char *devname, int baud_rate)
{

  if(baud_rate != 9600)
  {
    printf("\n\rBaud rate not supported");
    return -1;
  }

  //File descriptor
	int fd;

  //Serial port strucutre
	struct termios options;

	if ((fd = open(devname, O_RDWR | O_NOCTTY | O_NDELAY)) < 0) 
  {
		perror("Open");
		return -1;
	}

	// Set to blocking
	fcntl(fd, F_SETFL, 0);

	// Get port attributes
	tcgetattr(fd, &options);


  //Current supported baudrate
  if (baud_rate == 9600)
  {
    	// Set input and output baud rates
      cfsetispeed(&options, B9600);
      cfsetospeed(&options, B9600);
  }


	// Set input modes
	options.c_iflag |= ICRNL;

	// Set 8 bits, no parity, 1 stop bit
	options.c_cflag &= ~PARENB;
	options.c_cflag &= ~CSTOPB;
	options.c_cflag &= ~CSIZE;
	options.c_cflag |= CS8;
	options.c_lflag &= ~ECHO;
	options.c_lflag |= ICANON;

	// Set port attributes
	tcsetattr(fd, TCSAFLUSH, &options);

	return(fd);
}


float GpsToDecimalDegrees(const char* nmeaPos, char quadrant)
{
  float v= 0;
  if(strlen(nmeaPos)>5)
  {
    char integerPart[3+1];
    int digitCount= (nmeaPos[4]=='.' ? 2 : 3);
    memcpy(integerPart, nmeaPos, digitCount);
    integerPart[digitCount]= 0;
    nmeaPos+= digitCount;
    v= atoi(integerPart) + atof(nmeaPos)/60.;
    if(quadrant=='W' || quadrant=='S')
      v= -v;
  }
  return v;
}