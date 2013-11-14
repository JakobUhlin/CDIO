/*
 * UART.c
 *
 *  Created on: Oct 17, 2013
 *  Author: Jakob Uhlin
 */

#include <msp430.h>
#include "UART.h"
#include <string.h>

int uartStart = 0;
int id = 0;
char uartRxBuf[UART_RX_BUF_LEN];

#include "GSM_module.h"

void initUART(void)
{
	P5SEL |= BIT6+BIT7;				// Choose bit 3.4 (TX) and 3.5 (RX)

    UCA1CTL1|= UCSWRST;             //Have to set this flag to be able to initiate & modify other registers UCSWRST=1 NOW.
    UCA1CTL0 &= ~UCSYNC;            // 0 UART mode selected contrary to SPI mode


    UCA1CTL1 |=UCSSEL1;             //CLK=SMCK chosen

    //UCA0BR0 = 0x41;               //9600 baud rate
    //UCA0BR1 = 0x3;

    UCA1BR0 = 0xA0;                 //19200 baud rate for GPRS Sim900 setup
    UCA1BR1 = 0x01;


    UCA1MCTL|=UCBRS_6;              //Set modulation control register to 0x04 and =8mhz 19200bds
    UCA1CTL1 &= ~UCSWRST;
}


//Delay that satisfy GSM module in certain configuration
void Delay(void)
{
	__delay_cycles (1000000);
}

void uartEnable(void)
{
	UCA1IE |= UCRXIE;
}


// Disable Tx/Rx interrupts
void uartDisable(void)
{
	UCA1IE &= ~UCRXIE;
}


// Puts a vector in the output buffer and starts the sending process
void uartSend(int len, char vec)
{
	while(!(UCA1IFG & UCTXIFG));
    UCA1TXBUF = vec;
}


// Read Everything from RX that service routine says it to do.
void uartRead(char a)
{
    uartRxBuf[uartStart++] = a;

    if(ATResponseOK[id] == a)
    	id++;
}

char uart_is_tx_clear(void)
{
	return UCA1IFG &= UCTXIFG; 				// Dummy function that might be interesting but I gonna fix this with UCA1IFG &= UCTXIFG
}

void clockSetup()
{
    P4DIR   |= 0x80;                        // P4.7 SMCLK
    P4SEL   |= 0x80;                        // P4.7 for debugging freq.
    UCSCTL3 |= SELREF_2;                    // Set DCO FLL reference = REFO
    UCSCTL4 |= SELA_2;                      // Set ACLK = REFO
    __bis_SR_register(SCG0);                // Disable the FLL control loop
    UCSCTL0 = 0x0000;                       // Set lowest possible DCOx, MODx
    UCSCTL1 = DCORSEL_5;                    // Select DCO range 16MHz operation
    UCSCTL2 = FLLD_1 + 244;                 // Set DCO Multiplier for 8MHz
                                                // (N + 1) * FLLRef = Fdco

    __bic_SR_register(SCG0);                // Enable the FLL control loop

}




