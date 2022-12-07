/*
* File name   : MQTT.c
* Created     : 04-Dec-2022
* Author      : Dhiraj Bennadi
* Edited By   : Kevin Tom on 5th Dec 2022
* 
*
*
*--------------------------------------------------------------------------
* Copyright 2021 HiveMQ GmbH
*
* Licensed under the Apache License, Version 2.0 (the "License");
* you may not use this file except in compliance with the License.
* You may obtain a copy of the License at
*
*       http://www.apache.org/licenses/LICENSE-2.0
*
* Unless required by applicable law or agreed to in writing, software
* distributed under the License is distributed on an "AS IS" BASIS,
* WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
* See the License for the specific language governing permissions and
* limitations under the License.
*/

#include <string.h>
#include "MQTTClient.h"
#include "pubsub_opts.h"
#include <stdlib.h>
#include <unistd.h>



#define LOGGING (false)


//Global variable storing all necessary MQTT Client parameters
MQTTClient client;


int messageArrived(void *context, char *topicName, int topicLen, MQTTClient_message *message) 
{
    printf("Message arrived %s \n", (char*)message->payload);
    return 1;
}




int mqtt_Connect()
{

    // the serverURI has to be in the format "protocol://name:port", in this case it should be "ssl://name:8883"
    MQTTClient_create(&client, "ssl://b5feec6d24bf4ded89cbd15945207fb9.s2.eu.hivemq.cloud:8883", "Publisher",MQTTCLIENT_PERSISTENCE_NONE, NULL);

    // you can set optional callbacks for context, connectionLost, messageArrived and deliveryComplete
    int i = MQTTClient_setCallbacks(client, NULL, NULL, messageArrived, NULL);
    if(i != 0)
        printf("Callback Error: %d \n", i);

    MQTTClient_connectOptions conn_opts = MQTTClient_connectOptions_initializer;
    MQTTClient_SSLOptions ssl_opts = MQTTClient_SSLOptions_initializer;
    ssl_opts.enableServerCertAuth = 0;

    // declare values for ssl options, here we use only the ones necessary for TLS, but you can optionally define a lot more
    // look here for an example: https://github.com/eclipse/paho.mqtt.c/blob/master/src/samples/paho_c_sub.c
    ssl_opts.verify              = 1;
    ssl_opts.CApath              = NULL;
    ssl_opts.keyStore            = NULL;
    ssl_opts.trustStore          = NULL;
    ssl_opts.privateKey          = NULL;
    ssl_opts.privateKeyPassword  = NULL;
    ssl_opts.enabledCipherSuites = NULL;

    // use TLS for a secure connection, "ssl_opts" includes TLS
    conn_opts.ssl               = &ssl_opts;
    conn_opts.keepAliveInterval = 10;
    conn_opts.cleansession      = 1;


    // use your credentials that you created with the cluster
    conn_opts.username = "dhirajmqtt";
    conn_opts.password = "Dhiraj@2022";
    //int j = MQTTClient_connect(client, &conn_opts);
    MQTTClient_connect(client, &conn_opts);

    return 1;
}


int mqtt_Subscribe(const char * sub_topic)
{
    if(sub_topic == NULL)
	{
		printf("Topic varaible passed if NULL\n\r");
		return -1;
	}

    int qos      = 1;
    MQTTClient_subscribe(client, sub_topic, qos);
    return 1;
}


int mqtt_Unsubscribe(const char * sub_topic)
{
    if(sub_topic == NULL)
	{
		printf("Topic varaible passed if NULL\n\r");
		return -1;
	}

    MQTTClient_unsubscribe(client, sub_topic);
    return 1;
}

void mqtt_Close_Connection()
{
    int timeout = 10000;

    MQTTClient_disconnect(client, timeout);
    MQTTClient_destroy(&client);
}

int publish(char *payload, const char * topic)
{
    if(payload == NULL)
	{
		printf("Payload varaible passed if NULL\n\r");
		return -1;
	}

	if(topic == NULL)
	{
		printf("Topic passed is NULL\n\r");
		return -1;
	}


    int qos      = 1;
    int retained = 0;
    MQTTClient_deliveryToken dt;

    int payloadlen = strlen(payload);
    MQTTClient_publish(client, topic, payloadlen, payload, qos, retained, &dt);

    printf("DATA PUBLISHED\n\r");
    printf("--------------\n\r");
    printf("DATA  : %s\n\r", payload);
    printf("TOPIC : %s\n\n\n\r", topic);

    sleep(1);

    return 1;

}





