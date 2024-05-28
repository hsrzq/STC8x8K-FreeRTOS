#ifndef __STC8x8K_H__
#define __STC8x8K_H__

#include <8052.h>

__sfr __at(0x8E) AUXR;

__sfr __at(0xE3) DPS;
__sfr16 __at(0x8382) DPTR0;
__sfr16 __at(0xE5E4) DPTR1;

__sfr16 __at(0x8C8A) TMR0;
__sfr16 __at(0x8D8B) TMR1;
__sfr16 __at(0xD6D7) TMR2;
__sfr16 __at(0xD4D5) TMR3;
__sfr16 __at(0xD2D3) TMR4;

#endif // __STC8x8K_H__
