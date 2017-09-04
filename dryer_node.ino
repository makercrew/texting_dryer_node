// This firmware is the node portion of the texting dryer project found at
// https://www.kevinsidwar.com/projects/texting-dryer
//
// It is meant to run on an Adafruit Trinket but could be adapted to run on other
// platforms as well. When powered on it will send a single message to an RFM69
// gateway module to indicate the end of cycle on a dryer.
// **********************************************************************************
//
// Created and copyrighted by Kevin Sidwar 2017. Offered as open source under the 
// MIT License (MIT).
//
// **********************************************************************************
#include <RFM69.h>  
#include <avr/sleep.h>

// RFM69 Network Settings
#define NODE_ID       2    // Unique ID for the dryer node
#define NETWORK_ID    100
#define GATEWAY_ID    1
//#define ENCRYPT_KEY   "sampleEncryptKey"
#ifndef ENCRYPT_KEY
  #error You must #define ENCRYPTKEY to be a 16 byte secret string like "sampleEncryptKey"
#endif

// RFM69 Module Settings
#define FREQUENCY   RF69_433MHZ
#define IS_RFM69HW_HCW  //uncomment only for RFM69HW/HCW! Leave out if you have RFM69W/CW!

RFM69 rfm;
char message[] = "{\"topic\":\"home/upstairs/laundry/dryer/state\",\"msg\":\"done\"}";

void setup() 
{
  // Configure sleep mode for use later
  set_sleep_mode(SLEEP_MODE_PWR_DOWN);
  sleep_enable();

  // Initialize RFM module
  rfm.initialize(FREQUENCY,NODE_ID,NETWORK_ID);

#ifdef IS_RFM69HW_HCW
  rfm.setHighPower();
#endif

  rfm.encrypt(ENCRYPT_KEY);

  // Send the message that the dryer cycle is complete
  rfm.sendWithRetry(GATEWAY_ID, message, strlen(message), 2, 100);
}


void loop() 
{
  // There is nothing else to do so just go back to sleep.
  sleep_mode();
}