/****************************************************************************************************
  File name   : UART.c
  Created     : 04-Dec-2022
  Author      : Kevin Tom
                MS-ECEE-ESE
                CU Boulder, CO
  Email       : keto9919@colorado.edu
  Description : The file has all the interface APIs for getting data from serial port
				on a linux system.
  Reference   : Parsing NMEA sentences
                https://www.beyondlogic.org/ansi-c-basic-lightweight-nmea-parser-for-gps/
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



static int parse_comma_delimited_str(char *string, char **fields, int max_fields);
static void convert_time(char *date, char *time);
static float GpsToDecimalDegrees(const char* nmeaPos, char quadrant);
static int get_GPS_data(int fd, char **field);



//Make this macro true to turn on logging
#define LOGGING 	false

//Boolean to enalbe time parsing
bool get_time=false;





/*
  Name         : get_Lat(int fd, char *payload)
  Descrirption : This function reads values from GPS sensor and return 
				 latitude value to the caller.
  Arguments    : fd      - file descriptor of the opened file(port to which GPS is connected)
                 payload - pointer to char string to be written.
  Returns      : int - Value showing if the operation was sucessfull.
				 +ve value - If operation is sucessfull
				 -ve value - If operation failed
*/
int get_Lat(int fd, char *payload)
{
	if(payload == NULL)
	{
		printf("Payload varaible passed if NULL\n\r");
		return -1;
	}

	if(fd < 0)
	{
		printf("File descriptor passed is not valid\n\r");
		return -1;
	}

	float lat = 0;
	char *gps_data[20];
	get_GPS_data(fd,gps_data);

	#if LOGGING
		printf("\n\n\rRaw Latitude : %s, Direction : %c",*(gps_data+2), *gps_data[3]);
	#endif

	lat = GpsToDecimalDegrees(*(gps_data+2), *gps_data[3]);
	snprintf(payload,30,"Latitude : %f",lat);
	
	return 1;
}



/*
  Name         : get_Long(int fd, char *payload)
  Descrirption : This function reads values from GPS sensor and return longitude 
  				 value to the caller.
  Arguments    : fd      - file descriptor of the opened file(port to which GPS is connected)
                 payload - pointer to char string to be written.
  Returns      : int - Value showing if the operation was sucessfull.
				 +ve value - If operation is sucessfull
				 -ve value - If operation failed
*/
int get_Long(int fd, char *payload)
{
	if(payload == NULL)
	{
		printf("Payload varaible passed if NULL\n\r");
		return -1;
	}

	if(fd < 0)
	{
		printf("File descriptor passed is not valid\n\r");
		return -1;
	}

	float lon = 0;
	char *gps_data[20];
	get_GPS_data(fd,gps_data);

	#if LOGGING
		printf("\n\n\rRaw Longitude : %s, Direction : %c",*(gps_data+4), *gps_data[5]);
	#endif

	lon = GpsToDecimalDegrees(*(gps_data+4), *gps_data[5]);
	snprintf(payload,30,"Longitude : %f",lon);
	
	return 1;
}







/*
  Name         : get_Lat_Long(int fd, char *payload)
  Descrirption : This function reads values from GPS sensor and return longitude and latitude
  				 value to the caller.
  Arguments    : fd      - file descriptor of the opened file(port to which GPS is connected)
                 payload - pointer to char string to be written.
  Returns      : int - Value showing if the operation was sucessfull.
				 +ve value - If operation is sucessfull
				 -ve value - If operation failed
*/
int get_Lat_Long(int fd, char *payload)
{
	if(payload == NULL)
	{
		printf("Payload varaible passed if NULL\n\r");
		return -1;
	}

	if(fd < 0)
	{
		printf("File descriptor passed is not valid\n\r");
		return -1;
	}
	float lon = 0, lat=0;
	char *gps_data[20];
	char data[30];
	get_GPS_data(fd,gps_data);
	

	//Converting Latitude and Longitude to actual values
	lat = GpsToDecimalDegrees(*(gps_data+2), *gps_data[3]);
	lon = GpsToDecimalDegrees(*(gps_data+4), *gps_data[5]);

	#if LOGGING
		printf("\n\rFunction: get_Lat_Long()");
		printf("\n\r------------------------");
		printf("\n\rRaw Latitude : %s, Direction : %c",*(gps_data+2), *gps_data[3]);
		printf("\n\rRaw Longitude : %s, Direction : %c",*(gps_data+4), *gps_data[5]);
		printf("\n\rConverted Latitude : %d, Longitude %d",lat,lon);
	#endif

	snprintf(payload,30,"Latitude : %f,",lat);
	snprintf(data,30,"Longitude : %f",lon);
	strncat(payload,data, 30);

	return 1;
}






/*
  Name         : get_Satellites(int fd, char *payload)
  Descrirption : This function reads values from GPS sensor and retur the number of satellites
  				 connected to the caller.
  Arguments    : fd      - file descriptor of the opened file(port to which GPS is connected)
                 payload - pointer to char string to be written.
  Returns      : int - Value showing if the operation was sucessfull.
				 +ve value - If operation is sucessfull
				 -ve value - If operation failed
*/
int get_Satellites(int fd, char *payload)
{
	if(payload == NULL)
	{
		printf("Payload varaible passed if NULL\n\r");
		return -1;
	}

	if(fd < 0)
	{
		printf("File descriptor passed is not valid\n\r");
		return -1;
	}

	char *gps_data[20];
	get_GPS_data(fd,gps_data);

	#if LOGGING
		printf("\n\rFunction: get_Satellites()");
		printf("\n\r-------------------------");
		printf("\n\n\rRaw Number of Satellites : %s",*gps_data[7]);
	#endif

	snprintf(payload, 30,"Satellite : %s", gps_data[7]);
	
	return 1;
}




/*
  Name         : print_time_UTC(int fd)
  Descrirption : This function prints UTC time read from the GPS sensor to the console
  Arguments    : fd      - file descriptor of the opened file(port to which GPS is connected)
  Returns      : int - Value showing if the operation was sucessfull.
				 +ve value - If operation is sucessfull
				 -ve value - If operation failed
*/
int print_time_UTC(int fd)
{
	if(fd < 0)
	{
		printf("File descriptor passed is not valid\n\r");
		return -1;
	}

	char *gps_data[20];
	get_GPS_data(fd,gps_data);

	return 1;
}







static int get_GPS_data(int fd, char **field)
{
	char buffer[255];
	int nbytes = 0;
	int i =0;
	bool gps_signal_found = false;

	do 
  	{
		if ((nbytes = read(fd, &buffer, sizeof(buffer))) < 0) 
    	{
			perror("Read");
			return -1;
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
			#if LOGGING
				printf("\nRAW Data : %s\r\n", buffer);
			#endif
			buffer[nbytes - 1] = '\0';

			//Checking talker ID
			//Reference : https://cdn.sparkfun.com/assets/0/b/0/f/7/u-blox8-M8_ReceiverDescrProtSpec__UBX-13003221__Public.pdf
			//Page 104
			if ((strncmp(buffer, "$GP", 3) == 0) | (strncmp(buffer, "$GN", 3) == 0)) 
			{

				if (strncmp(&buffer[3], "GGA", 3) == 0) 
				{
					#if LOGGING
						printf("\nRAW Data (GGA): %s\r\n", buffer);
					#endif
					i = parse_comma_delimited_str(buffer, field, 20);

					//Not enough data
					if(i<7)
						continue;

					gps_signal_found = true;
				}


				if (strncmp(&buffer[3], "RMC", 3) == 0 && get_time) 
				{
					#if LOGGING
						printf("\nRAW Data (RMC): %s\r\n", buffer);
					#endif
					i = parse_comma_delimited_str(buffer, field, 20);
					convert_time(field[9],field[1]);
				}
			}
		}
		}
	}while(!gps_signal_found);

	return 1;
}




static int parse_comma_delimited_str(char *string, char **fields, int max_fields)
{
	int i = 0;
	fields[i++] = string;

	while ((i < max_fields) && NULL != (string = strchr(string, ','))) {
		*string = '\0';
		fields[i++] = ++string;
	}

	return --i;
}




static void convert_time(char *date, char *time)
{
	//struct timespec ts;
	struct tm gpstime;
	//time_t secs;
	char tempbuf[2];
	//int ret;


	if ((strlen(date) != 6) | (strlen(time) != 9)) 
	{
		#if LOGGING
			printf("No date or time fix. Exiting\r\n");
		#endif
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




static float GpsToDecimalDegrees(const char* nmeaPos, char quadrant)
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


int CloseGPSPort(int fd)
{
	if (close(fd) < 0) 
  	{
		perror("Close");
		return -1;
	}
	return 1;
}