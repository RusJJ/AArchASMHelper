#pragma once

#include <stdint.h>

// This thing has been written by RusJJ a.k.a. [-=KILL MAN=-]
// https://github.com/RusJJ

namespace Thumbv7 {

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
    inline static uint32_t Create(int16_t _imm, uint32_t _reg)
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