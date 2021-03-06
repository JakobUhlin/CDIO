/*
 * 	GSM_module.h
 *
 *  Created on: Oct 17, 2013
 *  Author: Jakob Uhlin
 */

#ifndef GSM_MODULE_H_
#define GSM_MODULE_H_

extern const char ATResponseOK[];
// Initiating the GSM module
void initGSM(void);

// Sends AT command to Tx buffer
void sendATCommand(int length, const char *command);

//Send stop bit to GSM module, to end task
void sendCtrlZ(void);

//Compare response from GSM to wanted response
int compareEasy(char *response, const char *wantedResponse);

void readSMS();

int searchForSMS(char *message);

void doSomethingDummy();

void pwrOn(void);
void checkAT();
void whatIsTheMessage();

#endif /* GSM_MODULE_H_ */
