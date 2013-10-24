/*
 * GSM_module.c
 *
 *  Created on: Oct 9, 2013
 *      Author: Administrator
 */



#include <msp430.h>
#include "GSM_module.h"
#include "UART.h"
#include "LCD.h"
#include <stdlib.h>
#include <string.h>


///////////////////////////////////////////////////////////////
/* AT Commands setup definitions*/
const unsigned char AT_Test[]		 = "AT\r";
const unsigned char AT_ConfSMS[] 	 = "AT+CMGF=1\r";
const unsigned char AT_ConfJakob[] 	 = "AT+CMGS=\"+46736525723\"\r";

///////////////////////////////////////////////////////////////
/*AT Commands responses*/
const char AT_Testresp[]	= "AT\r \rOK\r\0";//'A'+'T'+0x2E+'\r'+'O'+'K'+'\r'; It creates AT...OK..
const char AT_SMSresp[];

///////////////////////////////////////////////////////////////
// Sending the AT+Respons, that is the received response, want_respons is the
//wanted response..
void Compare_easy(char *AT_Respons, const char *want_respons)
{
	char *array= (char*)malloc(strlen(want_respons)+1);
	strcpy(array,AT_Respons);
	while(strcmp(array,want_respons));
}


// Initiating the GSM module
void initGSM(void)
{
	sendATCommand(strlen(AT_Test),AT_Test);
	uart_enable();
	dosomethingdummy();
	//while(!(UCA0IFG & UCRXIFG));
	Compare_easy(uartRxBuf, AT_Testresp);
	//sendATCommand(strlen(AT_ConfSMS),AT_ConfSMS);
	//sendATCommand(strlen(AT_ConfJakob),AT_ConfJakob); //GET EEPROM FUnction to be created!
	//Delay();
	uart_disable(); // DEBUGGA HIT, isafall fungerar var while sats och compare ar true.

}


// Sends AT command to Tx buffer
void sendATCommand(int length, const unsigned char* command)
{
	char com;
	while((length) > 0 )
	{
		length--;
		com=*(command++);
		uart_send(length, com);
	}
}

//Use this to end your message to the GSM module
void sendCTRL_Z(void)
{
	while(!(UCA0IFG & UCTXIFG));
	UCA0TXBUF=26;			// ASCII number for Ctrl+Z
}

//Temporary function to get the shit done. The program has enough time to get
//..the response from the UCA0RXBUF
void dosomethingdummy()
{
	int i;
	int d=5000;
	for (i=0;i<3000;i++ )
	{
		d--;
	}
	//while(strcmp(uartRxBuf,AT_Testresp)); //// Skriv en funktion har som hamtar in vardet den bor se, beroende pa vad som skickats
}

