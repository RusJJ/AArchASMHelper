#pragma once

#include <stdint.h>

// This thing has been written by RusJJ a.k.a. [-=KILL MAN=-]
// https://github.com/RusJJ

namespace ARMv7 {

union MOVBits
{
    inline static uint32_t Create(uint32_t _imm, uint32_t _reg)
    {
        // isXreg - is register X or W?
        MOVBits val; val.addr = 0x52800000;

        val.reg = _reg;
        val.imm = _imm;

        return val.addr;
    }
    struct
    {
        uint32_t reg : 5;
        int32_t  imm : 16;
        uint32_t pad : 11; // descriptor of instruction
    };
    uint32_t addr;
};

};