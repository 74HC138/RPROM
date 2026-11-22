#include "amiga.h"

volatile CustomChipRegisters_t* const AmigaChipset = (CustomChipRegisters_t*) 0x00DFF000;
volatile CIARegisters_t* const AmigaCIAA = (CIARegisters_t*) 0x00BFE001;
volatile CIARegisters_t* const AmigaCIAB = (CIARegisters_t*) 0x00BFD000;

const CustomChipRegisters_t* const CopperRegisters = (CustomChipRegisters_t*) 0;

uint16_t copperList[32];

//constructs the copperlist needed to display anything
int makeCopperlist(int Bitplanes, uint16_t** BitplaneMemory) {
    if (Bitplanes < 0 || Bitplanes > 6) return 1;
    uint16_t* copperListPointer = copperList;
    for (int i = 0; i < Bitplanes; i++) {
        //construct the commands for the copper on the fly from bitplane memory pointer
        uint16_t bplHi = ((uint32_t) BitplaneMemory[i]) >> 16;
        uint16_t bplLo = ((uint32_t) BitplaneMemory[i]) & 0xffff;
        uint16_t bitplaneCommands[4] = {
            COPPER_MOVE((uint16_t) &CopperRegisters->bitplanes.bitplanePointers[i].BPLPTH, bplHi),
            COPPER_MOVE((uint16_t) &CopperRegisters->bitplanes.bitplanePointers[i].BPLPTL, bplLo)
        };
        for (int j = 0; j < 4; j++) *copperListPointer++ = bitplaneCommands[j];
    }
    return 0;
}

int AmigaInitSystem(int PAL, int LowRes, int Bitplanes, uint16_t** BitplaneMemory) {
    //Assume an ECS Amiga
    //Also works on OCS but the PAL bool must match the hardware
    //Check if requested screen config works
    if (LowRes) {
        if (Bitplanes > 6 || Bitplanes < 0) return 1;
    } else {
        if (Bitplanes > 4 || Bitplanes < 0) return 1;
    }

    //disable all interrupts
    AmigaChipset->INTENA = AMIGACC_WRITE(MODE_CLEAR, 0xffff);
    //disable all DMA operations
    AmigaChipset->DMACON = AMIGACC_WRITE(MODE_CLEAR, 0xffff);
    //disable any sounds
    for (struct AudioChannel* channel = &AmigaChipset->paula.audioChannel[0]; channel <= &AmigaChipset->paula.audioChannel[3]; channel++) {
        channel->AUDDAT = 0;
        channel->AUDLEN = 0;
        channel->AUDVOL = 0;
    }
    /*
    the above code does the same as this code but uses 6 instruction instead of 12 instructions per loop
    mind you this code is in no way timing relevant but still, 6 instructions is 6 instructions...
    for (int i = 0; i < 4; i++) {
        AmigaChipset->paula.audioChannel[i].AUDDAT = 0;
        AmigaChipset->paula.audioChannel[i].AUDLEN = 0;
        AmigaChipset->paula.audioChannel[i].AUDVOL = 0;
    }
    */
    //setup screen geometry
    //Commodore recommends:
    //      VStart  HStart  Vstop   Hstop   Results:    DIWSTRT DIWSTOP
    //PAL   $2C     $81     $12C    $1C1                $2C81   $2CC1
    //NTSC  $2C     $81     $F4     $1C1                $2C81   $F4C1
    //this results in standard overscan
    AmigaChipset->DIWSTRT = (PAL) ? 0x2C81 : 0x2C81;
    AmigaChipset->DIWSTOP = (PAL) ? 0x2CC1 : 0xFEC1;
    //set the horizontal display fetch zone
    //For Commodore recommendet screen geometry and OCS/ECS transfer:
    //Res   Start   Stop
    //Low   $38     $D0
    //High  $3C     $D4
    //Super $3E     $D6
    AmigaChipset->DDFSTRT = (LowRes) ? 0x0038 : 0x003C;
    AmigaChipset->DDFSTOP = (LowRes) ? 0x00D0 : 0x00D4;
    //enable bitplanes and set resolution
    uint16_t conVal = 0;
    conVal |= (LowRes) ? 0x0000 : 1 << 15; //set HIRES bit when not in low resolution
    conVal |= (Bitplanes & 0x07) << 12; //set the number of bitplanes
    conVal |= 1 << 9; //enable color on composite out for a good measure
    AmigaChipset->bitplanes.BPLCON0 = conVal;
    //no scrolling offset
    AmigaChipset->bitplanes.BPLCON1 = 0;
    //sprites have priority over bitplane
    AmigaChipset->bitplanes.BPLCON2 = 0x0004;
    //construct the copperlist for the display
    makeCopperlist(Bitplanes, BitplaneMemory);
    //set pointer to copperlist
    AmigaChipset->copper.COP1LC = (uint32_t) copperList;
    //enable screen and copper DMA
    AmigaChipset->DMACON = AMIGACC_WRITE(MODE_SET, (DMACON_e) DMAEN | COPEN | BPLEN);
    
    return 0;
}