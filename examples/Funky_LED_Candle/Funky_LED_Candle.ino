
#include <avr/sleep.h>

#ifndef cbi
#define cbi(sfr, bit) (_SFR_BYTE(sfr) &= ~_BV(bit))
#endif
#ifndef sbi
#define sbi(sfr, bit) (_SFR_BYTE(sfr) |= _BV(bit))
#endif


int ledCandle = 3;           // LED connected here with 220Ohm resistor
int max_count = 0;          // counter for the flicker table
int count = 0;
int countTwo = 0;
unsigned long runtime = 600000;  //10*60*1000 ms or 10 minutes

byte Flicker_table[] PROGMEM = {    
0x77,	0x7B,	0x7D,	0x7B,	0x78,	0xBA,	0x95,	0x53,	0x5E,	0x3E,	0x3E,	0x43,	0x52,	0x35,	0x3E,	0x75,	0x95,
0x4B,	0x65,	0x7B,	0x52,	0x63,	0x58,	0x6B,	0x55,	0x62,	0x77,	0x72,	0x87,	0x85,	0x8C,	0x5D,	0x7A,	0x2E,
0x53,	0x5D,	0x50,	0x62,	0x55,	0x6F,	0x67,	0x58,	0x78,	0x4E,	0x55,	0x95,	0x6B,	0x6D,	0x7D,	0x73,	0x73,
0x7D,	0x5B,	0x6B,	0x68,	0x6A,	0x6A,	0x60,	0x6B,	0x77,	0x77,	0x98,	0x7A,	0x87,	0x60,	0x5B,	0x6B,	0xC9,
0x53,	0x4A,	0x53,	0x68,	0x45,	0x3B,	0x65,	0x67,	0x9C,	0x60,	0x67,	0x53,	0x73,	0x75,	0x63,	0x8A,	0x81,
0x7F,	0x83,	0x88,	0x80,	0x63,	0x58,	0x58,	0x6B,	0x7A,	0x78,	0x7B,	0x83,	0x8A,	0x87,	0x8A,	0x85,	0x08,
0x83,	0x88,	0x8A,	0x8A,	0x8A,	0x8A,	0x8A,	0x8C,	0x8A,	0x8A,	0x8A,	0x8A,	0x8A,	0x88,	0x83,	0x7F,	0x0B,
0x7D,	0x7B,	0x7A,	0x78,	0x77,	0x75,	0x70,	0x70,	0x72,	0x70,	0x70,	0x4D,	0x41,	0x6D,	0x68,	0x60,	0xB5,
0x35,	0x35,	0x3D,	0x3B,	0x41,	0x45,	0x52,	0x5E,	0x41,	0x53,	0x5D,	0x60,	0x65,	0x6A,	0x50,	0x48,	0xA0,
0x4B,	0x4E,	0x35,	0x35,	0x31,	0x33,	0x33,	0x36,	0x3B,	0x40,	0x50,	0x4E,	0x52,	0x5D,	0x60,	0x53,	0x15,
0x56,	0x4B,	0x35,	0x2D,	0x2E,	0x2E,	0x35,	0x38,	0x38,	0x39,	0x3B,	0x38,	0x31,	0x58,	0x40,	0x60,	0x77,
0x4D,	0x50,	0x5A,	0x5D,	0x43,	0x40,	0x53,	0x58,	0x5A,	0x55,	0x4E,	0x31,	0x31,	0x2B,	0x2E,	0x33,	0xD3,
0x31,	0x36,	0x35,	0x36,	0x38,	0x39,	0x39,	0x38,	0x40,	0x4A,	0x41,	0x3B,	0x50,	0x62,	0x40,	0x36,	0x4E,
0x29,	0x2D,	0x45,	0x5E,	0x5D,	0x52,	0x3E,	0x33,	0x3B,	0x43,	0x35,	0x45,	0x38,	0x35,	0x31,	0x33,	0x3E,
0x36,	0x36,	0x39,	0x36,	0x38,	0x3B,	0x41,	0x36,	0x36,	0x30,	0x39,	0x33,	0x2B,	0x29,	0x33,	0x5A,	0x98,
0x63,	0x56,	0x41,	0x3D,	0x50,	0x52,	0x55,	0x60,	0x65,	0x55,	0x3B,	0x30,	0x2E,	0x30,	0x3B,	0x4E,	0x66,
0x36,	0x2E,	0x2B,	0x3B,	0x39,	0x3D,	0x4A,	0x50,	0x3D,	0x45,	0x58,	0x4E,	0x4B,	0x4E,	0x4A,	0x45,	0xC5,
0x58,	0x4B,	0x55,	0x5D,	0x5B,	0x56,	0x58,	0x5E,	0x60,	0x77,	0x5E,	0x38,	0x5A,	0x52,	0x46,	0x4B,	0x79,
0x50,	0x4A,	0x4A,	0x35,	0x4E,	0x41,	0x2E,	0x36,	0x36,	0x38,	0x52,	0x4B,	0x46,	0x3B,	0x33,	0x40,	0xC4,
0x4E,	0x60,	0x5A,	0x50,	0x4D,	0x43,	0x4A,	0x50,	0x4B,	0x48,	0x40,	0x3D,	0x40,	0x46,	0x52,	0x5B,	0xFA,
0x62,	0x63,	0x63,	0x5B,	0x52,	0x46,	0x5B,	0x43,	0x55,	0x45,	0x26,	0x35,	0x3B,	0x5B,	0x43,	0x4D,	0xDB,
0x40,	0x56,	0x58,	0x5A,	0x5D,	0x50,	0x46,	0x45,	0x45,	0x41,	0x3B,	0x43,	0x72,	0x87,	0x90,	0x8A,	0x08,
0x8F,	0x97,	0x9D,	0x95,	0x73,	0x65,	0x6B,	0x4D,	0x46,	0x45,	0x55,	0x55,	0x41,	0x56,	0x55,	0x55,	0x31,
0x56,	0x5A,	0x5A,	0x5B,	0x5E,	0x56,	0x62,	0x55,	0x65,	0x58,	0x5A,	0x62,	0x68,	0x6D,	0x6D,	0x6B,	0x89,
0x68,	0x6A,	0x6F,	0x65,	0x6D,	0x31,	0x6F,	0x55,	0x48,	0x50,	0x55,	0x67,	0x5A,	0x41,	0x55,	0x5E,	0xC5,
0x60,	0x65,	0x68,	0x68,	0x63,	0x60,	0x6A,	0x60,	0x67,	0x6A,	0x7F,	0x83,	0x8C,	0x87,	0x88,	0x92,	0x3D,
0x8D,	0x8F,	0x88,	0x8C,	0x8C,	0x85,	0x82,	0x6A,	0x4E,	0x35,	0x23,	0x11,	0x19,	0x43,	0x3B,	0x41,	0x93,
0x67,	0x4A,	0x4A,	0x3B,	0x2E,	0x30,	0x45,	0x41,	0x4A,	0x58,	0x48,	0x70,	0x5B,	0x6D,	0x72,	0x62,	0x2F,
0x75,	0x67,	0x56,	0x5A,	0x5E,	0x55,	0x4D,	0x77,	0x53,	0x2D,	0x36,	0x41,	0x5D,	0x55,	0x40,	0x40,	0x03,
0x40,	0x40,	0x40,	0x3E,	0x41,	0x5E,	0x82,	0x92,	0x88,	0x88,	0x90,	0x94,	0x88,	0x85,	0x7B,	0x63,	0x4F,
0x55,	0x53,	0x56,	0x55,	0x50,	0x53,	0x55,	0x03,	0x34,	0x01,	0x3A,	0x7E,	0xFF,	0x01,	0x60,	0x3E,	0x36

 
  };
                             

 ISR(PCINT0_vect) {
 }


void setup() {
  pinMode(ledCandle, OUTPUT);   
  pinMode(9,INPUT);
  digitalWrite(9,HIGH);

  max_count=sizeof(Flicker_table);
}

void loop(){

 long now=millis();

while(millis()-now < runtime) {   
  analogWrite(ledCandle, pgm_read_byte(Flicker_table+count));  
    count++;                                       
    if(count > max_count )
      count = 0;    // Helps makes sure our next normal glow doesnt start in an arbitrary place on the table
    delay(60);  // the delay for our flicker, make it faster to to make it flicker a little more violently
}
  digitalWrite(ledCandle,LOW);
  sbi(GIMSK,PCIE0); // Turn on Pin Change interrupt
  sbi(PCMSK0,PCINT1); // Which pins are affected by the interrupt

  set_sleep_mode(SLEEP_MODE_PWR_DOWN); // Set sleep mode
  sleep_mode(); // System sleeps here

  cbi(GIMSK,PCIE0); // Turn off Pin Change interrupt

 }


