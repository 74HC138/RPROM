//custom chip definitions for amiga 500

#pragma once

#include <stdint.h>

//definitions for the custom chip registers
struct CustomChipRegisters {
    uint16_t BLTDDAT;
    uint16_t DMACONR;
    uint16_t VPOSR;
    uint16_t VHPOSR;
    uint16_t DSKDATR;
    uint16_t JOY0DAT;
    uint16_t JOY1DAT;
    uint16_t CLXDAT;
    uint16_t ADKCONR;
    uint16_t POT0DAT;
    uint16_t POT1DAT;
    uint16_t POTGOR;
    uint16_t SERDATR;
    uint16_t DSKBYTR;
    uint16_t INTENAR;
    uint16_t INTREQR;
    uint16_t DSKPTH;
    uint16_t DSKPTL;
    uint16_t DSKLEN;
    uint16_t DSKDAT;
    uint16_t REFPTR;
    uint16_t VPOSW;
    uint16_t VHPOSW;
    uint16_t COPCON;
    uint16_t SERDAT;
    uint16_t SERPER;
    uint16_t POTGO;
    uint16_t JOYTEST;
    uint16_t STREQU;
    uint16_t STRVBL;
    uint16_t STRHOR;
    uint16_t STRLONG;
    struct __attribute__((packed)) Blitter {
        uint16_t BLTCON0;
        uint16_t BLTCON1;
        uint16_t BLTAFWM;
        uint16_t BLTALWM;
        union {
            struct {
                uint16_t BLTCPTH;
                uint16_t BLTCPTL;
            };
            uint32_t BLTCPT;
        };
        union {
            struct {
                uint16_t BLTBPTH;
                uint16_t BLTBPTL;
            };
            uint32_t BLTBPT;
        };
        union {
            struct {
                uint16_t BLTAPTH;
                uint16_t BLTAPTL;
            };
            uint32_t BLTAPT;
        };
        union {
            struct {
                uint16_t BLTDPTH;
                uint16_t BLTDPTL;
            };
            uint32_t BLTDPT;
        };
        uint16_t BLTSIZE;
        uint16_t BLTCON0L;
        uint16_t BLTSIZV;
        uint16_t BLTSIZH;
        uint16_t BLTCMOD;
        uint16_t BLTBMOD;
        uint16_t BLTAMOD;
        uint16_t BLTDMOD;
        uint16_t unused[4];
        uint16_t BLTCDAT;
        uint16_t BLTBDAT;
        uint16_t BLTADAT;
        uint16_t unused2;
    } blitter;
    uint16_t SPRHDAT; //AGA
    uint16_t BPLHDAT; //AGA
    uint16_t DENISEID;
    uint16_t DSKSYNC;
    struct __attribute__((packed)) Copper {
        union {
            struct {
                uint16_t COP1LCH;
                uint16_t COP1LCL;
            };
            uint32_t COP1LC;
        };
        union {
            struct {
                uint16_t COP2LCH;
                uint16_t COP2LCL;
            };
            uint32_t COP2LC;
        };
        uint16_t COPJMP1;
        uint16_t COPJMP2;
        uint16_t COPINS;
    } copper;
    uint16_t DIWSTRT;
    uint16_t DIWSTOP;
    uint16_t DDFSTRT;
    uint16_t DDFSTOP;
    uint16_t DMACON;
    uint16_t CLXCON;
    uint16_t INTENA;
    uint16_t INTREQ;
    struct __attribute__((packed)) Paula {
        uint16_t ADKCON;
        struct __attribute__((packed)) AudioChannel {
            uint16_t AUDLCH;
            uint16_t AUDLCL;
            uint16_t AUDLEN;
            uint16_t AUDPER;
            uint16_t AUDVOL;
            uint16_t AUDDAT;
            uint16_t unused[2];
        } audioChannel[4];
    } paula;
    struct __attribute__((packed)) Bitplanes {
        struct __attribute__((packed)) BitplanePointer {
            union {
                struct {
                    uint16_t BPLPTH;
                    uint16_t BPLPTL;
                };
                uint32_t BPLPT;
            };
        } bitplanePointers[8]; //bitplane pointers 0-5 valid for OCS/ECS, 6 and 7 for AGA additionally
        uint16_t BPLCON0;
        uint16_t BPLCON1;
        uint16_t BPLCON2;
        uint16_t BPLCON3;
        uint16_t BPL1MOD;
        uint16_t BPL2MOD;
        uint16_t BPLCON4;
        uint16_t CLXCON2;
        uint16_t BPLDAT[8]; //bitplane data 0-5 valid for OCS/ECS, 6 and 7 for AGA additionally
    } bitplanes;
    struct __attribute__((packed)) Sprites {
        struct __attribute__((packed)) SpritePointer {
            uint16_t SPRPTH;
            uint16_t SPRPTL;
        } spritePointers[8];
        struct __attribute__((packed)) SpriteControl {
            uint16_t SPRPOS;
            uint16_t SPRCTL;
            uint16_t SPRDATA;
            uint16_t SPRDATB;
        } spriteControl[8];
    } sprites;
    uint16_t COLOR[32];
    uint16_t HTOTAL;
    uint16_t HSSTOP;
    uint16_t HBSTRT;
    uint16_t HBSTOP;
    uint16_t VTOTAL;
    uint16_t VSSTOP;
    uint16_t VBSTRT;
    uint16_t VBSTOP;
    uint16_t SPRHSTRT;
    uint16_t SPRHSTOP;
    uint16_t BPLHSTRT;
    uint16_t BPLHSTOP;
    uint16_t HHPOSW;
    uint16_t HHPOSR;
    uint16_t BEAMCON0;
    uint16_t HSSTRT;
    uint16_t VSSTRT;
    uint16_t HCENTER;
    uint16_t DIWHIGH;
    uint16_t BPLHMOD;
    uint16_t SPRHPTH;
    uint16_t SPRHPTL;
    uint16_t BPLHPTH;
    uint16_t BPLHPTL;
    uint16_t unused[6];
    uint16_t FMODE;
};
typedef struct CustomChipRegisters CustomChipRegisters_t;
extern volatile CustomChipRegisters_t* const AmigaChipset;
extern const CustomChipRegisters_t* const CopperRegisters;

enum CopperRegister {
    BLTDDAT = 0x000,
    DMACONR = 0x002,
    VPOSR   = 0x004,
    VHPOSR  = 0x006,
    DSKDATR = 0x008
};


typedef enum {
        BLTPRI  = 1 << 10,
        DMAEN   = 1 << 9,
        BPLEN   = 1 << 8,
        COPEN   = 1 << 7,
        BLTEN   = 1 << 6,
        SPREN   = 1 << 5,
        DKSEN   = 1 << 4,
        AUD3EN  = 1 << 3,
        AUD2EN  = 1 << 2,
        AUD1EN  = 1 << 1,
        AUD0EN  = 1 << 0
} DMACON_e;
enum INTREGS {
        INTEN   = 1 << 14,
        EXTER   = 1 << 13,
        SDKSYN  = 1 << 12,
        RBF     = 1 << 11,
        AUD3    = 1 << 10,
        AUD2    = 1 << 9,
        AUD1    = 1 << 8,
        AUD0    = 1 << 7,
        BLIT    = 1 << 6,
        VERTB   = 1 << 5,
        COPPER  = 1 << 4,
        PORTS   = 1 << 3,
        SOFT    = 1 << 2,
        DSKBLK  = 1 << 1,
        TBE     = 1 << 0
};
typedef enum INTREGS INTENA_e;
typedef enum INTREGS INTREQ_e;

//definitions for the CIA registers
struct CIARegisters {
    uint8_t PRA;
    uint8_t xdummy0[256];
    uint8_t PRB;
    uint8_t xdummy1[256];
    uint8_t DDRA;
    uint8_t xdummy2[256];
    uint8_t DDRB;
    uint8_t xdummy3[256];
    uint8_t TALO;
    uint8_t xdummy4[256];
    uint8_t TAHI;
    uint8_t xdummy5[256];
    uint8_t TBLO;
    uint8_t xdummy6[256];
    uint8_t TBHI;
    uint8_t xdummy7[256];
    uint8_t TODLO;
    uint8_t xdummy8[256];
    uint8_t TODMID;
    uint8_t xdummy9[256];
    uint8_t TODHI;
    uint8_t xdummyA[256];
    uint8_t Unused;
    uint8_t xdummyB[256];
    uint8_t SDR;
    uint8_t xdummyC[256];
    uint8_t ICR;
    uint8_t xdummyD[256];
    uint8_t CRA;
    uint8_t xdummyE[256];
    uint8_t CRB;
};
typedef struct CIARegisters CIARegisters_t;
extern volatile CIARegisters_t* const AmigaCIAA;
extern volatile CIARegisters_t* const AmigaCIAB;

//helper macros
enum ChipRegisterAccessMode {
    MODE_CLEAR = 0,
    MODE_SET = 1
};
typedef enum ChipRegisterAccesMode ChipRegisterAccesMode_t;
#define AMIGACC_WRITE(mode, value) ((mode == MODE_CLEAR ? 0x0000 : 0x8000) | (value & 0x7fff))
#define AMIGA_COLOR(r, g, b) (((r & 0x0f) << 8) | ((g & 0x0f) << 4) | ((b & 0x0f) << 0))
#define COPPER_MOVE(destination, value) (destination & 0xfffe), value
#define COPPER_WAIT(waitV, waitH, BFD, vMask, hMask) (((waitV & 0xff) << 8) | (waitH & 0xfe) | 0x0001), ((BFD << 15) | ((vMask & 0x7f) << 8) | (hMask & 0xfe))
#define COPPER_SKIP(vPos, hPos, BFD, vMask, hMask) (((vPos & 0xff) << 8) | (hPos & 0xfe) | 0x0001), ((BFD << 15) | ((vMask & 0x7f) << 8) | (hMask & 0xfe) | 0x0001)
#define COPPER_REGISTER(register) (&CopperRegisters->#register)

/*
brief: initializes the custom chips into a default config
returns: 0 on success
*/
extern int AmigaInitSystem(int PAL, int LowRes, int Bitplanes, uint16_t** BitplaneMemory);

