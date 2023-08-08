#pragma once

#include <stdint.h>

// This thing has been written by RusJJ a.k.a. [-=KILL MAN=-]
// https://github.com/RusJJ

namespace ThumbV7 {

#define LSR(x,m) ((x) >> (m))
#define LSL(x,m) ((x) << (m))
inline uint32_t ROR_C(uint32_t x, uint32_t n, uint32_t shift)
{
    uint32_t m = shift % n;
    return LSR(x, m) | LSL(x, n-m);
}

union CMP2Bits // imm size is sizeof(char) (from 0x00 to 0xFF)
{
    inline static uint16_t Create(uint32_t _imm, uint32_t _reg)
    {
        CMP2Bits val; val.addr = 0x2800;

        val.reg = _reg;
        val.imm = _imm;

        return val.addr;
    }
    struct
    {
        int32_t  imm : 8;
        uint32_t reg : 3;
        uint32_t pad : 5;
    };
    uint16_t addr;
};
union CMPWBits // The logic is WAY-WAY-WAY MORE DUMB, LOL
{
    union Imm16
    {
        struct
        {
            uint16_t  imm8 : 8;
            uint16_t  imm3 : 3;
            uint16_t  i    : 1;
        };
        struct
        {
            int8_t    immval1;
            uint16_t  immval2 : 2;
            uint16_t  immval3 : 2;
        };
        struct
        {
            uint16_t  imm7 : 7;
            uint16_t  imm5 : 5;
        };
        uint32_t GetValue()
        {
            if(immval3)
            {
                union {
                    struct { uint16_t idx1 : 7; uint16_t idx2 : 1; };
                    uint8_t value;
                } imm7_plus_1;
                imm7_plus_1.idx1 = imm7; imm7_plus_1.idx2 = 1;
                uint32_t unrotated_value = imm7_plus_1.value;
                return ROR_C(unrotated_value, 32, imm5);
            }
            char retc[4] {0};
            switch(immval2)
            {
                default: return (uint32_t)imm8;
                case 1: retc[1] = immval1; retc[3] = immval1; return *(uint32_t*)retc;
                case 2: retc[0] = immval1; retc[2] = immval1; return *(uint32_t*)retc;
                case 3: retc[0] = immval1; retc[1] = immval1; retc[2] = immval1; retc[3] = immval1; return *(uint32_t*)retc;
            }
        }
        bool SetValue(uint32_t val)
        {
            return false; // We are not ready! :(
        }
    };
    inline static uint32_t Create(uint32_t _imm, uint32_t _reg)
    {
        CMPWBits val;           val.addr = 0x0F00F1B0;
        CMPWBits::Imm16 immval; if(!immval.SetValue(_imm)) return 0;

        val.reg = _reg;
        val.imm8 = immval.imm8;
        val.imm3 = immval.imm3;
        val.i    = immval.i;

        return val.addr;
    }
    inline uint32_t GetValue()
    {
        CMPWBits::Imm16 immval;
        immval.imm8 = imm8;
        immval.imm3 = imm3;
        immval.i = i;
        return immval.GetValue();
    }
    struct
    {
        int32_t  reg  : 4;
        uint32_t pad1 : 6;
        uint32_t i    : 1;
        uint32_t pad2 : 5;
        int32_t  imm8 : 8;
        int32_t  pad3 : 4;
        int32_t  imm3 : 3;
        uint32_t pad0 : 1;
    };
    uint32_t addr;
};

union MOV2Bits // imm size is sizeof(char) (from 0x00 to 0xFF)
{
    inline static uint16_t Create(uint32_t _imm, uint32_t _reg)
    {
        MOV2Bits val; val.addr = 0x2000;

        val.reg = _reg;
        val.imm = _imm;

        return val.addr;
    }
    struct
    {
        int32_t  imm : 8;
        uint32_t reg : 3;
        uint32_t pad : 5;
    };
    uint16_t addr;
};
union MOVWBits // Dumb logic as hell (imm value from 0 to 65535)
{
    union Imm16
    {
        struct
        {
            uint16_t  imm8 : 8;
            uint16_t  imm3 : 3;
            uint16_t  i    : 1;
            uint16_t  imm4 : 4;
        };
        int16_t value;
    };
    inline static uint32_t Create(int32_t _imm, uint32_t _reg)
    {
        MOVWBits val;           val.addr = 0x0000F240;
        MOVWBits::Imm16 immval; immval.value = _imm;

        val.reg = _reg;
        val.imm8 = immval.imm8;
        val.imm4 = immval.imm4;
        val.imm3 = immval.imm3;
        val.i    = immval.i;

        return val.addr;
    }
    struct
    {
        int32_t  imm4 : 4;
        uint32_t pad1 : 6;
        uint32_t i    : 1;
        uint32_t pad2 : 5;
        int32_t  imm8 : 8;
        int32_t  reg  : 4;
        int32_t  imm3 : 3;
        uint32_t pad0 : 1;
    };
    uint32_t addr;
};

};