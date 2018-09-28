#define F_CPU 8000000UL

#include<avr/io.h>
#include<util/delay.h>
#include"sound.h"

int main(void){

  DDRD=(1<<PD0)|(1<<PD2)|(1<<PD1);  
  PORTD=0x02;
  //PORTB|=(0b00000001);
  init_tone();
  while(1){
    PORTD^=(1<<PD2);
    PORTD^=(1<<PD1);
    if(readCapacitivePin(&PORTB, 1<<PB1)>5)play(NOTE_C4,100);
    if(readCapacitivePin(&PORTB, 1<<PB2)>5)play(NOTE_CS4,100);
    if(readCapacitivePin(&PORTB, 1<<PB3)>5)play(NOTE_D4,100);
    if(readCapacitivePin(&PORTB, 1<<PB4)>5)play(NOTE_DS4,100);
    if(readCapacitivePin(&PORTB, 1<<PB5)>5)play(NOTE_E4,100);
    
    if(readCapacitivePin(&PORTC, 1<<PC0)>5)play(NOTE_F4,100);
    if(readCapacitivePin(&PORTC, 1<<PC1)>5)play(NOTE_FS4,100);
    if(readCapacitivePin(&PORTC, 1<<PC2)>5)play(NOTE_G4,100);
    if(readCapacitivePin(&PORTC, 1<<PC3)>5)play(NOTE_GS4,100);
    if(readCapacitivePin(&PORTC, 1<<PC4)>5)play(NOTE_A4,100);
    if(readCapacitivePin(&PORTC, 1<<PC5)>5)play(NOTE_AS4,100);
    
    if(readCapacitivePin(&PORTD, 1<<PD0)>5)play(NOTE_B4,100);

  }
}
