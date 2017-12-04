/*
    FreeRTOS V6.1.1 - Copyright (C) 2011 Real Time Engineers Ltd.

    This file is part of the FreeRTOS distribution.

    FreeRTOS is free software; you can redistribute it and/or modify it under
    the terms of the GNU General Public License (version 2) as published by the
    Free Software Foundation AND MODIFIED BY the FreeRTOS exception.
    ***NOTE*** The exception to the GPL is included to allow you to distribute
    a combined work that includes FreeRTOS without being obliged to provide the
    source code for proprietary components outside of the FreeRTOS kernel.
    FreeRTOS is distributed in the hope that it will be useful, but WITHOUT
    ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
    FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License for
    more details. You should have received a copy of the GNU General Public 
    License and the FreeRTOS license exception along with FreeRTOS; if not it 
    can be viewed here: http://www.freertos.org/a00114.html and also obtained 
    by writing to Richard Barry, contact details for whom are available on the
    FreeRTOS WEB site.

    1 tab == 4 spaces!

    http://www.FreeRTOS.org - Documentation, latest information, license and
    contact details.

    http://www.SafeRTOS.com - A version that is certified for use in safety
    critical systems.

    http://www.OpenRTOS.com - Commercial support, development, porting,
    licensing and training services.
*/
#define extern
/* FreeRTOS.org includes. */
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"

/* Demo includes. */
#include "basic_io.h"

#include "lpc17xx_i2s.h"
#include "lpc17xx_spi.h"
#include "lpc17xx_i2c.h"
#include "lpc17xx_emac.h"
#include "lpc17xx_pinsel.h"
#include "lpc17xx_gpio.h"
#include "lpc17xx_ssp.h"

#include "oled.h"

#include "easyweb.h"
#include "ethmac.h"
#include "tcpip.h"
#include "webside.h"

#include "light.h"
#include "top.h"

static void vSenderTask( void *pvParameters );
static void vReceiverTask( void *pvParameters );

unsigned int aaPagecounter=0;
unsigned int adcValue = 0;
uint8_t buf[50];
int x = 0;

static void init_ssp(void)
{
	SSP_CFG_Type SSP_ConfigStruct;
	PINSEL_CFG_Type PinCfg;

	PinCfg.Funcnum = 2;
	PinCfg.OpenDrain = 0;
	PinCfg.Pinmode = 0;
	PinCfg.Portnum = 0;
	PinCfg.Pinnum = 7;
	PINSEL_ConfigPin(&PinCfg);
	PinCfg.Pinnum = 8;
	PINSEL_ConfigPin(&PinCfg);
	PinCfg.Pinnum = 9;
	PINSEL_ConfigPin(&PinCfg);
	PinCfg.Funcnum = 0;
	PinCfg.Portnum = 2;
	PinCfg.Pinnum = 2;
	PINSEL_ConfigPin(&PinCfg);

	SSP_ConfigStructInit(&SSP_ConfigStruct);

	// Initialize SSP peripheral with parameter given in structure above
	SSP_Init(LPC_SSP1, &SSP_ConfigStruct);

	// Enable SSP peripheral
	SSP_Cmd(LPC_SSP1, ENABLE);

}

static void init_i2c(void)
{
	PINSEL_CFG_Type PinCfg;

	/* Initialize I2C2 pin connect */
	PinCfg.Funcnum = 2;
	PinCfg.Pinnum = 10;
	PinCfg.Portnum = 0;
	PINSEL_ConfigPin(&PinCfg);
	PinCfg.Pinnum = 11;
	PINSEL_ConfigPin(&PinCfg);

	// Initialize I2C peripheral
	I2C_Init(LPC_I2C2, 100000);

	/* Enable I2C1 operation */
	I2C_Cmd(LPC_I2C2, ENABLE);
}

int getLuz(){
	return light_read();
}


char buff_luz[50];
void upadateSensorLuz(int x, int y) {
    sprintf((char*)buff_luz, "Sensor luz: %d ", getLuz());
	oled_putString(x,y, (uint8_t*)buff_luz, OLED_COLOR_BLACK, OLED_COLOR_WHITE);
}
char buff_luz_p[50];
int getLuzPercent() {
	return getLuz() / 9.72;
}
void showLuzPercente() {
	int luxp = getLuzPercent();
	sprintf((char*)buff_luz_p, "Luz per: %d ", luxp);
	oled_putString(0,35, (uint8_t*)buff_luz_p, OLED_COLOR_BLACK, OLED_COLOR_WHITE);
	oled_fillRect(0,50,luxp*0.8,55,OLED_COLOR_WHITE);
	oled_fillRect(0,50,(100 - luxp)*0.8 ,55,OLED_COLOR_BLACK);
}

void init_hardware(){
	vPrintString("Starting...\n");
	uint8_t buf[50];
	init_ssp();
	init_i2c();

	oled_init();
	oled_clearScreen(OLED_COLOR_WHITE);
	oled_putString(1,3,  (uint8_t*)"________________", OLED_COLOR_BLACK, OLED_COLOR_WHITE);

	oled_putString(1,1,  (uint8_t*)"PROJETO TOP.", OLED_COLOR_BLACK, OLED_COLOR_WHITE);
	sprintf((char*)buf, "IP:%d.%d.%d.%d", MYIP_1, MYIP_2, MYIP_3, MYIP_4);
	oled_putString(1,14, (uint8_t*)buf, OLED_COLOR_BLACK, OLED_COLOR_WHITE);
	oled_line(0,10,95,10,OLED_COLOR_BLACK);

	light_init();
	light_enable();
	top_init();

	TCPLowLevelInit();

	HTTPStatus = 0;

	TCPLocalPort = TCP_PORT_HTTP;
}
char runningServer = 0;
void serverWeb(void *pvParameters){
		if (!(SocketStatus & SOCK_ACTIVE)) TCPPassiveOpen();
		DoNetworkStuff();

		HTTPServer();

}

char NewKey[100];
void InsertDynamicValues()
{
  unsigned char *key;
  unsigned int i;
  if (TCPTxDataCount < 4) return;                     // there can't be any special string

  key = TCP_TX_BUF;
  struct TOP_INFO* info = getTopInfo();

  for (i = 0; i < (TCPTxDataCount - 3); i++)
  {

    if (*key == 'A')
     if (*(key + 1) == 'D')
       if (*(key + 3) == '%')
         switch (*(key + 2))
         {
           case '1' ://CONTADOR                                 // "AD1%"?
			 {
				  vPrintString("c\n");
				 sprintf(NewKey, "%4u",info->counter);    // increment and insert page counter
				 memcpy(key, NewKey, 4);

				 break;
			 }
           case '2' : { // ip
        	   vPrintString("ip\n");
					char buf[12];
					sprintf((char*)buf, "%d.%d.%d.%d", MYIP_1, MYIP_2, MYIP_3, MYIP_4);

					sprintf(NewKey, "%s",buf);
					memcpy(key, NewKey,sizeof(buf));

				   break;
				 }
           case '3' : //luz
				 {
					  vPrintString("lu\n");
				   sprintf(NewKey, "%4u",  info->luz);
				   memcpy(key, NewKey, 4);
				   break;
				 }
           case '4' : //luz percente
				 {
					  vPrintString("lup\n");
				   sprintf(NewKey, "%3d",  info->luzPercent);
				   memcpy(key, NewKey, 3);
				   break;
				 }
         }
    key++;
  }
}



void HTTPServer(void){
  if (SocketStatus & SOCK_CONNECTED)             // check if somebody has connected to our TCP
  {
    if (SocketStatus & SOCK_DATA_AVAILABLE)      // check if remote TCP sent data
      TCPReleaseRxBuffer();                      // and throw it away

    if (SocketStatus & SOCK_TX_BUF_RELEASED)     // check if buffer is free for TX
    {
      if (!(HTTPStatus & HTTP_SEND_PAGE))        // init byte-counter and pointer to webside
      {                                          // if called the 1st time
        HTTPBytesToSend = sizeof(WebSide) - 1;   // get HTML length, ignore trailing zero
        PWebSide = (unsigned char *)WebSide;     // pointer to HTML-code
      }

      if (HTTPBytesToSend > MAX_TCP_TX_DATA_SIZE)     // transmit a segment of MAX_SIZE
      {
        if (!(HTTPStatus & HTTP_SEND_PAGE))           // 1st time, include HTTP-header
        {
          vPrintString("Make response.\n");
          ++aaPagecounter;

          memcpy(TCP_TX_BUF, GetResponse, sizeof(GetResponse) - 1);
          memcpy(TCP_TX_BUF + sizeof(GetResponse) - 1, PWebSide, MAX_TCP_TX_DATA_SIZE - sizeof(GetResponse) + 1);
          HTTPBytesToSend -= MAX_TCP_TX_DATA_SIZE - sizeof(GetResponse) + 1;
          PWebSide += MAX_TCP_TX_DATA_SIZE - sizeof(GetResponse) + 1;
        }
        else
        {
          memcpy(TCP_TX_BUF, PWebSide, MAX_TCP_TX_DATA_SIZE);
          HTTPBytesToSend -= MAX_TCP_TX_DATA_SIZE;
          PWebSide += MAX_TCP_TX_DATA_SIZE;
        }

        TCPTxDataCount = MAX_TCP_TX_DATA_SIZE;   // bytes to xfer
        InsertDynamicValues();                   // exchange some strings...
        TCPTransmitTxBuffer();                   // xfer buffer
        //vPrintString("6\n");

      }
      else if (HTTPBytesToSend)                  // transmit leftover bytes
      {
        memcpy(TCP_TX_BUF, PWebSide, HTTPBytesToSend);
        TCPTxDataCount = HTTPBytesToSend;        // bytes to xfer
        InsertDynamicValues();                   // exchange some strings...
        TCPTransmitTxBuffer();                   // send last segment
        TCPClose();                              // and close connection
        HTTPBytesToSend = 0;                     // all data sent
      }

      HTTPStatus |= HTTP_SEND_PAGE;              // ok, 1st loop executed
    }
  }
  else
    HTTPStatus &= ~HTTP_SEND_PAGE;               // reset help-flag if not connected
}




// Code Red - GetAD7Val function replaced
// Rather than using the AD convertor, in this version we simply increment
// a counter the function is called, wrapping at 1024.
volatile unsigned int aaScrollbar = 400;

unsigned int GetAD7Val(void)
{
  aaScrollbar = (aaScrollbar +16) % 1024;
  adcValue = (aaScrollbar / 10) * 1000/1024;
  return aaScrollbar;
}

xQueueHandle xQueue;

static void teste( void *pvParameters ){
	long lValueToSend;
	portBASE_TYPE xStatus;

		/* Two instances are created of this task so the value that is sent to the
		queue is passed in via the task parameter rather than be hard coded.  This way
		each instance can use a different value.  Cast the parameter to the required
		type. */
		lValueToSend = ( long ) pvParameters;

		/* As per most tasks, this task is implemented within an infinite loop. */
		for( ;; )
		{
			xStatus = xQueueSendToBack( xQueue, &lValueToSend, 0 );

			if( xStatus != pdPASS )
			{
				/* We could not write to the queue because it was full � this must
				be an error as the queue should never contain more than one item! */
				vPrintString( "teste.\r\n" );
			}

			/* Allow the other sender task to execute. */
			taskYIELD();
		}
}
void updateDisplay(){
	x++;
	if (x > 10000){

		upadateSensorLuz(1, 25);
		top_update_values(aaPagecounter, buf, getLuz(), getLuzPercent());
		x = 0;
		//vPrintString( HTTPStatus);
//		vPrintStringAndNumber( "Received = ", HTTPStatus );
	}
}

void updateServer(){
	while(1){
		serverWeb(NULL);
	//	taskYIELD();
	}
}

#define UPDATE_SERVER 100
#define UPDATE_SENSOR_LUZ 10
#define SHOW_PERCENTE_LUZ 11

int main( void ){

	init_hardware();
	updateServer();
    xQueue = xQueueCreate( 5, sizeof( long ) );

	if( xQueue != NULL ){

		xTaskCreate( updateServer, "updateServer", 240, UPDATE_SERVER, 1, NULL );

		xTaskCreate( vSenderTask, "updateSensorLuz", 240,UPDATE_SENSOR_LUZ, 1, NULL );
		xTaskCreate( vSenderTask, "showLuzPercente", 240, SHOW_PERCENTE_LUZ, 1, NULL );

		xTaskCreate( vReceiverTask, "Receiver", 240, NULL, 2, NULL );
		vTaskStartScheduler();

	}
}

/*-----------------------------------------------------------*/

static void vSenderTask( void *pvParameters ){
long lValueToSend;
portBASE_TYPE xStatus;
	lValueToSend = ( long ) pvParameters;
	for( ;; )
	{
		xStatus = xQueueSendToBack( xQueue, &lValueToSend, 0 );

		if( xStatus != pdPASS )
		{
		}

		taskYIELD();
	}
}
/*-----------------------------------------------------------*/

static void vReceiverTask( void *pvParameters ){
long lReceivedValue;
portBASE_TYPE xStatus;
const portTickType xTicksToWait = 100 / portTICK_RATE_MS;

	for( ;; )
	{
		if( uxQueueMessagesWaiting( xQueue ) != 0 )
		{
			//vPrintString( "Queue should have been empty!\r\n" );
		}
		xStatus = xQueueReceive( xQueue, &lReceivedValue, xTicksToWait );

		if( xStatus == pdPASS )
		{
			switch (lReceivedValue){
				case UPDATE_SERVER:
					updateServer(NULL);
					//vPrintString("Received UPDATE_SERVER\r\n");
					break;
				case UPDATE_SENSOR_LUZ:
					updateDisplay();
					//vPrintString("Received UPDATE_SENSOR_LUZ\r\n");
					break;
				case SHOW_PERCENTE_LUZ:
					showLuzPercente();
					//vPrintString("Received SHOW_PERCENTE_LUZ\r\n");
					break;
			}


		}
	}
}
/*-----------------------------------------------------------*/

void vApplicationMallocFailedHook( void )
{
	/* This function will only be called if an API call to create a task, queue
	or semaphore fails because there is too little heap RAM remaining. */
	for( ;; );
}
/*-----------------------------------------------------------*/

void vApplicationStackOverflowHook( xTaskHandle *pxTask, signed char *pcTaskName )
{
	/* This function will only be called if a task overflows its stack.  Note
	that stack overflow checking does slow down the context switch
	implementation. */
	for( ;; );
}
/*-----------------------------------------------------------*/

void vApplicationIdleHook( void )
{
	/* This example does not use the idle hook to perform any processing. */
}
/*-----------------------------------------------------------*/

void vApplicationTickHook( void )
{
	/* This example does not use the tick hook to perform any processing. */
}


