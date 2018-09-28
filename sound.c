#define F_CPU 8000000UL

#include<avr/io.h>
#include<util/delay.h>
#include<avr/interrupt.h>


volatile int16_t timer1_toggle_count;
volatile uint8_t *timer1_pin_port;
volatile uint8_t timer1_pin_mask;


//From the Arduino Tone Library:
void init_tone(void){
  DDRB=(1<<PB0);
  TCCR1A = 0;
  TCCR1B = 0;
  TCCR1B|=1<<WGM12;
  TCCR1B|=1<<CS10;
  timer1_pin_port = &PORTB;
  timer1_pin_mask = (1<<PB0);
  sei();
}

ISR(TIMER1_COMPA_vect){
  if (timer1_toggle_count != 0)
  {
    // toggle the pin
    PORTB^=(1<<PB0);

    if (timer1_toggle_count > 0)
      timer1_toggle_count--;
  }
  else
  {
    TIMSK &= ~(1 << OCIE1A);                 // disable the interrupt
    PORTB&= ~(1<<PB0);   // keep pin low after stop
  }
}

void play(uint16_t frequency, uint32_t duration){
  uint8_t prescalarbits = 0b001;
  int16_t toggle_count = 0;
  uint32_t ocr = 0;

      // two choices for the 16 bit timers: ck/1 or ck/64
      ocr = F_CPU / frequency / 2 - 200;

      prescalarbits = 0b001;
      if (ocr > 0xffff)
      {
        ocr = F_CPU / frequency / 2 / 64 - 1;
        prescalarbits = 0b011;
      }

      TCCR1B = (TCCR1B & 0b11111000) | prescalarbits;
      
      if (duration > 0){
        toggle_count = 2 * frequency * duration / 1000;
      }
      else{
        toggle_count = -1;
      }
      OCR1A = ocr;
      timer1_toggle_count = toggle_count;
      TIMSK|=1 << OCIE1A;
        
}

/*function copied and adapted to Cfrom the Arduino 8 bit capacitive synth, 
from Bantam tools. Available at: 

https://support.bantamtools.com/hc/en-us/articles/360001045893?_ga=2.258280015.458448953.1538141580-1372417667.1536370002
*/
uint8_t readCapacitivePin(volatile uint8_t *PORT, uint8_t MASK) {
  
  // Variables used to translate from Arduino to AVR pin naming
  volatile uint8_t* port;
  volatile uint8_t* ddr;
  volatile uint8_t* pin;
  // Here we translate the input pin number from
  //  Arduino pin number to the AVR PORT, PIN, DDR,
  //  and which bit of those registers we care about.
  uint8_t bitmask;
  port = PORT;
  if(PORT==&PORTB){
    ddr = &DDRB;
    pin = &PINB;
  }
  if(PORT==&PORTC){
    ddr = &DDRC;
    pin = &PINC;
  }
  if(PORT==&PORTD){
    ddr = &DDRD;
    pin = &PIND;
  }
    
  bitmask = MASK;
  
  // Discharge the pin first by setting it low and output
  *port &= ~(bitmask);
  *ddr  |= bitmask;
  _delay_ms(1);
  // Prevent the timer IRQ from disturbing our measurement
  cli();
  // Make the pin an input with the internal pull-up on
  *ddr &= ~(bitmask);
  *port |= bitmask;

  // Now see how long the pin to get pulled up. This manual unrolling of the loop
  // decreases the number of hardware cycles between each read of the pin,
  // thus increasing sensitivity.
  uint8_t cycles = 17;
       if (*pin & bitmask) { cycles =  0;}
  else if (*pin & bitmask) { cycles =  1;}
  else if (*pin & bitmask) { cycles =  2;}
  else if (*pin & bitmask) { cycles =  3;}
  else if (*pin & bitmask) { cycles =  4;}
  else if (*pin & bitmask) { cycles =  5;}
  else if (*pin & bitmask) { cycles =  6;}
  else if (*pin & bitmask) { cycles =  7;}
  else if (*pin & bitmask) { cycles =  8;}
  else if (*pin & bitmask) { cycles =  9;}
  else if (*pin & bitmask) { cycles = 10;}
  else if (*pin & bitmask) { cycles = 11;}
  else if (*pin & bitmask) { cycles = 12;}
  else if (*pin & bitmask) { cycles = 13;}
  else if (*pin & bitmask) { cycles = 14;}
  else if (*pin & bitmask) { cycles = 15;}
  else if (*pin & bitmask) { cycles = 16;}

  // End of timing-critical section
  sei();

  // Discharge the pin again by setting it low and output
  //  It's important to leave the pins low if you want to 
  //  be able to touch more than 1 sensor at a time - if
  //  the sensor is left pulled high, when you touch
  //  two sensors, your body will transfer the charge between
  //  sensors.
  *port &= ~(bitmask);
  *ddr  |= bitmask;

  return cycles;
}
