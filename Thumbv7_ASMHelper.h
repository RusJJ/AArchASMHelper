#pragma once

#include <stdint.h>

// This thing has been written by RusJJ a.k.a. [-=KILL MAN=-]
// https://github.com/RusJJ

namespace ThumbV7 {

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
union CMPWBits // The logic is way more dumb (this one is NOT working right now!)
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
            uint32_t ret;
            char retc[4] {0};
            if(immval3)
            {
                
            }
            else
            {
                switch(immval2)
                {
                    case 0:
                        ret = imm8;
                        break;
                    case 1:
                        retc[1] = immval1; retc[3] = immval1;
                        ret = *(uint32_t*)retc;
                        break;
                    case 2:
                        retc[0] = immval1; retc[2] = immval1;
                        ret = *(uint32_t*)retc;
                        break;
                    case 3:
                        retc[0] = immval1; retc[2] = immval1; retc[1] = immval1; retc[3] = immval1;
                        ret = *(uint32_t*)retc;
                        break;
                }
            }
            return ret;
        }
    };
    inline static uint32_t Create(uint16_t _imm, uint32_t _reg)
    {
        CMPWBits val;           val.addr = 0x0000F240;
        //CMPWBits::Imm16 immval; immval.value = _imm;

        //val.reg = _reg;
        //val.imm8 = immval.imm8;
        //val.imm3 = immval.imm3;
        //val.i    = immval.i;

        return val.addr;
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