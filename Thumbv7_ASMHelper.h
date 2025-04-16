#ifndef ARMV7THUMB_ASMHELPER_H
#define ARMV7THUMB_ASMHELPER_H

#include <stdint.h>

// This thing has been written by RusJJ a.k.a. [-=KILL MAN=-]
// https://github.com/RusJJ


#define LSR(x,m) ((x) >> (m))
#define LSL(x,m) ((x) << (m))
inline uint32_t ROR_C(uint32_t x, uint32_t n, uint32_t shift)
{
    uint32_t m = shift % n;
    return LSR(x, m) | LSL(x, n-m);
}



namespace ThumbV7 {

enum eCond : uint8_t
{
    COND_EQ, COND_NE, COND_CS, COND_CC,
    COND_MI, COND_PL, COND_VS, COND_VC,
    COND_HI, COND_LS, COND_GE, COND_LT,
    COND_GT, COND_LE, COND_AL, COND_NV,
};

struct NOPBits
{
    inline static uint16_t Create() { return 0xBF00; }
    inline static uint32_t Create4() { return 0x8000F3AF; }
};

struct RETBits
{
    inline static uint16_t Create() { return 0x4770; }
};

struct CMPBits
{
    inline static uint16_t Create(uint32_t _imm, uint32_t _reg)
    {
        uint16_t basic = 0x2800;
        basic |= (_imm & 0xFF) | ((_reg & 0x7) << 8);
        return basic;
    }
    inline static uint32_t GetImm(uint16_t opcode) { return (opcode & 0xFF); }
    inline static uint8_t GetRd(uint16_t opcode) { return ((opcode >> 8) & 0x7); }
    inline static uint32_t GetMaxImm() { return 0xFF; }
    inline static uint32_t GetMaxReg() { return 7; }
};
struct CMPRegBits
{
    inline static uint16_t Create(uint32_t _reg, uint32_t _regWith)
    {
        uint16_t basic = (_reg & 0x8) ? 0x4580 : 0x4280;
        _reg %= 8;
        basic |= ((_regWith & 0xF) << 3) | (_reg & 0x7);
        return basic;
    }
    inline static uint8_t GetRn(uint16_t opcode) { return (opcode & 0x7) + 8 * ((opcode & 0x4580)==0x4580); }
    inline static uint8_t GetRm(uint16_t opcode) { return ((opcode >> 3) & 0xF); }
    inline static uint32_t GetMaxReg() { return 0xF; }
};

struct MOVBits
{
    inline static uint16_t Create(uint32_t _imm, uint32_t _reg)
    {
        uint16_t basic = 0x2000;
        basic |= (_imm & 0xFF) | ((_reg & 0x7) << 8);
        return basic;
    }
    inline static uint32_t GetImm(uint16_t opcode) { return (opcode & 0xFF); }
    inline static uint8_t GetRd(uint16_t opcode) { return ((opcode >> 8) & 0x7); }
    inline static uint32_t GetMaxImm() { return 0xFF; }
    inline static uint32_t GetMaxReg() { return 7; }
};
struct MOVRegBits
{
    inline static uint16_t Create(uint32_t _reg, uint32_t _regWith)
    {
        uint16_t basic = (_reg & 0x8) ? 0x4680 : 0x4600;
        _reg %= 8;
        basic |= ((_regWith & 0xF) << 3) | (_reg & 0x7);
        return basic;
    }
    inline static uint8_t GetRn(uint16_t opcode) { return (opcode & 0x7) + 8 * ((opcode & 0x4680)==0x4680); }
    inline static uint8_t GetRm(uint16_t opcode) { return ((opcode >> 3) & 0xF); }
    inline static uint32_t GetMaxReg() { return 0xF; }
};

struct BBits
{
    inline static uint16_t Create(uintptr_t from, uintptr_t to)
    {
        uint16_t basic = 0xE000;
        uintptr_t offset = (uint16_t)((to - from - 4) >> 1);
        basic |= (offset & 0x7FF);
        return basic;
    }
    inline static uint32_t GetImm(uint16_t opcode) { return (opcode & 0x7FF); }
    inline static uint32_t GetMaxImm() { return 0x7FF; }
    inline static uint32_t GetMaxDist() { return (GetMaxImm() << 1) + 4; }
    inline static uintptr_t GetDest(uint16_t opcode, uintptr_t pos) { return (GetImm(opcode) << 1) + 4 + pos; }
};

struct BXBits
{
    inline static uint16_t Create(uint32_t _reg, bool isBLX = false)
    {
        uint16_t basic = isBLX ? 0x4780 : 0x4700;
        basic |= (_reg << 3);
        return basic;
    }
    inline static uint8_t GetRn(uint16_t opcode) { return ((opcode >> 3) & 0xF); }
    inline static bool IsBLX(uint16_t opcode) { return (opcode & 0x0080) != 0; }
    inline static uint32_t GetMaxReg() { return 0xF; }
};

struct BCondBits
{
    inline static uint16_t Create(uintptr_t from, uintptr_t to, eCond cond)
    {
        uint16_t basic = 0xD000;
        uintptr_t offset = (uint16_t)((to - from - 4) >> 1);
        basic |= (offset & 0xFF) | (((cond - COND_EQ) & 0xF) << 8);
        return basic;
    }
    inline static uint32_t GetImm(uint16_t opcode) { return (opcode & 0xFF); }
    inline static uint32_t GetMaxImm() { return 0xFF; }
    inline static uint32_t GetMaxDist() { return (GetMaxImm() << 1) + 4; }
    inline static uintptr_t GetDest(uint16_t opcode, uintptr_t pos) { return (GetImm(opcode) << 1) + 4 + pos; }
    inline static eCond GetCond(uint16_t opcode) { return (eCond)((opcode >> 8) & 0xF); }
};

struct LDRPCBits // LDR Rd, [PC, #offset], offset is divided by 4 (0, 4, 8, 12 &etc)
{
    inline static uint16_t Create(uint32_t _destReg, uint32_t _fromOffset = 0)
    {
        uint32_t basic = 0x4800;
        basic |= ((_fromOffset >> 2) & 0xFF) | (((_destReg) & 0x7) << 8);
        return basic;
    }
    inline static uint32_t GetMaxImm() { return 0xFF; }
    inline static uint32_t GetMaxDist() { return GetMaxImm() << 2; }
    inline static uint32_t GetImm(uint16_t opcode) { return (opcode & 0xFF) << 2; }
    inline static uint8_t GetRd(uint16_t opcode) { return (opcode & 0x7); }
    // "pos" is an address of such instruction AFTER LDR: ADD Rd, PC
    inline static uintptr_t GetDest(uint16_t opcode, uintptr_t pos) { return (GetImm(opcode) + pos) & 0xFFFFFFFC; }
};

struct SUBSPBits // SUB SP, SP, #offset  , offset is divided by 4 (0, 4, 8, 12 &etc)
{
    inline static uint16_t Create(uint32_t offset)
    {
        uint16_t basic = 0xB080;
        basic |= ((offset >> 2) & 0x7F);
        return basic;
    }
    inline static uint32_t GetImm(uint16_t opcode) { return (opcode & 0x7F) << 2; }
    inline static uint32_t GetMaxImm() { return 0x7F; }
    inline static uint32_t GetMaxOffset() { return GetMaxImm() << 2; }
};

struct ADDSPBits // ADD SP, SP, #offset  , offset is divided by 4 (0, 4, 8, 12 &etc)
{
    inline static uint16_t Create(uint32_t offset)
    {
        uint16_t basic = 0xB000;
        basic |= ((offset >> 2) & 0x7F);
        return basic;
    }
    inline static uint32_t GetImm(uint16_t opcode) { return (opcode & 0x7F) << 2; }
    inline static uint32_t GetMaxImm() { return 0x7F; }
    inline static uint32_t GetMaxOffset() { return GetMaxImm() << 2; }
};

// OLD TO BE REIMPLEMENTED BELOW !!!

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



// INCOMPLETE BELOW !!!
struct CMPWBits
{
    inline static uint32_t Create(uint32_t _imm, uint32_t _reg)
    {
        uint8_t imm3, imm8 = _imm, i = 0;
        uint32_t basic = 0x0F00F1B0;

        if(_imm <= 0xFF) imm3 = 0;
        else if((_imm & 0x00FF00FF) == _imm && (_imm >> 16) == imm8 && (_imm & 0xFF) == imm8)
        {
            imm3 = 1;
        }
        else if((_imm & 0xFF00FF00) == _imm && (_imm >> 16) == (_imm & 0xFF00))
        {
            imm3 = 2;
            imm8 = _imm >> 8;
        }
        else if((_imm >> 24) == imm8 && (_imm >> 16) == imm8 && (_imm >> 8) == imm8)
        {
            imm3 = 3;
        }
        else if((_imm & 0xFF000000) == _imm)
        {
            imm3 = 4;
            imm8 = _imm >> 24;
        }
        /*else if((_imm & 0xFF) == _imm)
        {
            imm3 = 5;
            imm8 = _imm;
        }
        else if((_imm & 0xFF0000FF) == _imm && (_imm >> 24) == (_imm & 0xFF))
        {
            imm3 = 6;
            imm8 = _imm & 0xFF;
        }
        else if((_imm & 0xFF00FFFF) == _imm && (_imm >> 24) == ((_imm >> 16) & 0xFF))
        {
            imm3 = 7;
            imm8 = _imm >> 16;
        }*/

        else return 0; // return 0 on ERROR

        basic |= (_reg & 0xF) | (((imm3 << 12) | (imm8)) << 16) | ((i & 0x1) << 10);
        return basic;
    }
    inline static uint8_t GetImm3(uint32_t opcode) { return (opcode >> 28) & 0xF; }
    inline static bool GetI(uint32_t opcode) { return (opcode >> 10) & 0x1; }
    inline static uint8_t GetImm8(uint32_t opcode) { return (opcode >> 16) & 0xFF; }
    inline static uint32_t GetImm(uint32_t opcode)
    {
        uint8_t imm3 = GetImm3(opcode);
        if(GetI(opcode)) // TODO: ass
        {
            switch(imm3)
            {
                case 0: return (GetImm8(opcode) << 16) | 0x800000;
                case 1: return ((4 * GetImm8(opcode)) << 12) | 0x200000;
                case 2: return (GetImm8(opcode) << 12) | 0x80000;
                case 3: return ((4 * GetImm8(opcode)) << 8) | 0x20000;
                case 4: return (GetImm8(opcode) << 8) | 0x8000;
                case 5: return ((4 * GetImm8(opcode)) << 4) | 0x2000;
                case 6: return (GetImm8(opcode) << 4) | 0x800;
                case 7: return ((4 * GetImm8(opcode))) | 0x200;

                default: return 0;
            }
        }
        else
        {
            switch(imm3)
            {
                case 0: return GetImm8(opcode);
                case 1: return GetImm8(opcode) | (GetImm8(opcode) << 16);
                case 2: return (GetImm8(opcode) << 8) | (GetImm8(opcode) << 24);
                case 3: return GetImm8(opcode) | (GetImm8(opcode) << 8) | (GetImm8(opcode) << 16) | (GetImm8(opcode) << 24);
                case 4: return (GetImm8(opcode) << 24) | 0x80000000;
                case 5: return ((4 * GetImm8(opcode)) << 20) | 0x20000000;
                case 6: return (GetImm8(opcode) << 20) | 0x8000000;
                case 7: return ((4 * GetImm8(opcode)) << 16) | 0x2000000;

                default: return 0;
            }
        }
    }
    inline static uint8_t GetRd(uint32_t opcode) { return (opcode & 0xF); }
    inline static uint32_t GetMaxImm() { return 0xFFFFFFFF; }
    inline static uint32_t GetMaxReg() { return 14; }
};

struct BWBits
{
    inline static uint32_t Create(uintptr_t from, uintptr_t to)
    {
        uint32_t basic = 0xB800F000;
        uintptr_t offset = (uint16_t)((to - from - 4) >> 1);
        basic |= (offset & 0x3FF) | (((offset >> 10) & 0x7FF) << 16);
        return basic;
    }
    inline static uint32_t GetImm(uint32_t opcode) { return (opcode & 0x7FF); }
    inline static uint32_t GetMaxImm() { return 0x1FFFFF; }
    inline static uint32_t GetMaxDist() { return (GetMaxImm() << 1) + 4; }
    inline static uintptr_t GetDest(uint32_t opcode, uintptr_t pos) { return (GetImm(opcode) << 1) + 4 + pos; }
};

};

#endif // ARMV7THUMB_ASMHELPER_H