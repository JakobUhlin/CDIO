/*
 * 	GSM_module.c
 *
 *  Created on: Oct 17, 2013
 *  Author: Amanda
 */



#include <msp430.h>
#include "GSM_module.h"
#include "UART.h"
#include <stdlib.h>
#include <string.h>


#define SIZECommand 	6
#define SIZEResponse 	2
int initCount=0;

///////////////////////////////////////////////////////////////
/* AT Commands setup definitions*/

const char ATtest[] = "AT\r";
const char ATecho[] = "ATE0\r";
const char ATtextMode[] = "AT+CMGF=1\r";
const char ATsetPhoneNumber[] = "AT+CMGS=\"+46736525723\"\r";
const char ATsetSmsStorage[] = "AT+CPMS=\"SM\"\r";
const char ATreadUnreadSms[] = "AT+CMGL=\"REC UNREAD\"\r";
const char ATdeleteAllSms[] = "AT+CMGDA=\"DEL ALL\"\r";


///////////////////////////////////////////////////////////////
/*AT Responses setup definitions*/

const char ATResponseOK[] = "OK\r\n";

///////////////////////////////////////////////////////////////
// Initiating the GSM module
void initGSM(void)
{

	pwrOn();

	//Test command
	sendATCommand(strlen(ATtest), ATtest);
	Delay();

	//Echo mode off
	sendATCommand(strlen(ATecho), ATecho);
	uartEnable();

    while(id < strlen(ATResponseOK));

    uartRxBuf[uartStart] = '\0';
    uartDisable();
    uartStart = 0;
    id = 0;

    //Check if OK
    if((compareEasy(uartRxBuf, ATResponseOK)) == 1)
    {
    	sendATCommand(strlen(ATtextMode), ATtextMode);
    	uartEnable();

    	while(id < strlen(ATResponseOK));

    	uartRxBuf[uartStart] = '\0';
    	uartDisable();
    	uartStart = 0;
    	id = 0;
    }
}

// Sending the AT response, that is the received response, wantedResponse is the
//wanted response..
int compareEasy(char *response, const char *wantedResponse)
{
	if(!(strcmp(response, wantedResponse)))
		return 1;
	else
		return -1;
}


// Sends AT command to Tx buffer
void sendATCommand(int length, const char *command)
{
	char com;
    while(length > 0 )
    {
    	length--;

        com = *(command++);
        uartSend(length, com);
    }
}

//Use this to end your SMS
void sendCtrlZ(void)
{
	while(!(UCA1IFG & UCTXIFG));		//TX buffer ready?
    UCA1TXBUF=26;                       //ASCII number for Ctrl+Z
}

//Read the unread SMS
void readSMS()
{
    sendATCommand(strlen(ATsetSmsStorage), ATsetSmsStorage);
	uartEnable();

	while(id < strlen(ATResponseOK));

	uartRxBuf[uartStart] = '\0';
	uartDisable();
	uartStart = 0;
	id = 0;


    sendATCommand(strlen(ATreadUnreadSms), ATreadUnreadSms);
	uartEnable();

	while(id < strlen(ATResponseOK));

	uartRxBuf[uartStart] = '\0';
	uartDisable();
	uartStart = 0;
	id = 0;

    sendATCommand(strlen(ATsetPhoneNumber), ATsetPhoneNumber);//Send the telephone number to SIM900
    Delay();

	int startSMS = searchForSMS(uartRxBuf);
	while(startSMS < strlen(uartRxBuf))
	{
		uartSend(startSMS, uartRxBuf[startSMS]);
		startSMS++;

		if(uartRxBuf[startSMS] == '\r')
			if(uartRxBuf[startSMS++]=='\n')
				startSMS = strlen(uartRxBuf);
	}

	sendCtrlZ();
}

//Search for the start of the SMS
int searchForSMS(char *message)
{
	int i = 0;

	while(i < strlen(message))
		if(message[i] == '\r')
			if(message[i++]=='\n')
			return i;
}

//The program has enough time to get the
//response from the UCA0RXBUF
void doSomethingDummy()
{
    int i;
    int d = 5000;
    for (i = 0; i < 3000; i++ )
    {
    	d--;
    }
}

void pwrOn(void)
{
	initCount=1;
	P4DIR |= BIT2;

	//if(!(P4OUT & BIT2))
	//{
		P4OUT &= ~BIT2;
		int i = 0;

		while(i < 100)
		{
			Delay();
			i++;
		}
	//}
}
