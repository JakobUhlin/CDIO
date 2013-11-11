#include "msp430.h"
#include "UART.h"
#include "GSM_module.h"
#include <stdio.h>
#include <string.h>

void main(void)
{

	WDTCTL = WDTPW + WDTHOLD;

	clockSetup();
	initUART();
	__enable_interrupt();
	initGSM();
	readSMS();
//  UserExperience();
}

#pragma vector=USCI_A1_VECTOR
__interrupt void USCI_A1_ISR(void)
{
	 switch(UCA1IV) {
	    case 0:break;             // Vector 0 - no interrupt
	    case 2:                   // Vector 2 - RXIF
	    	  uartRead(UCA1RXBUF);
	    	  UCA1IFG &= ~UCRXIFG;
	        break;
	    case 4:                 // Vector 4 - TXIFG
	        break;
	    default: break;
	    }
	}
