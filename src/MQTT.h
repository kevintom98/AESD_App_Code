/****************************************************************************************************
  File name   : MQTT.h
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
#if !defined(MQTT_H)
#define MQTT_H


int mqtt_Connect();
int mqtt_Subscribe(const char * sub_topic);
int mqtt_Unsubscribe(const char * sub_topic);
void mqtt_Close_Connection();


void publish(char *payload, const char * topic);

#endif