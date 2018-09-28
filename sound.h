#include"pitch.h"
void init_tone(void);
void play(uint16_t frequency, uint32_t duration);
uint8_t readCapacitivePin(volatile uint8_t *PORT, uint8_t MASK);
