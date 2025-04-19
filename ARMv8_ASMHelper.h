#ifndef ARMV8_ASMHELPER_H
#define ARMV8_ASMHELPER_H

#include <stdint.h>

// This thing has been written by RusJJ a.k.a. [-=KILL MAN=-]
// https://github.com/RusJJ

namespace ARMv8 {

enum eCond : uint8_t
{
    COND_EQ, COND_NE, COND_CS, COND_CC,
    COND_MI, COND_PL, COND_VS, COND_VC,
    COND_HI, COND_LS, COND_GE, COND_LT,
    COND_GT, COND_LE, COND_AL, COND_NV,
};

struct NOPBits
{
    inline static uint32_t Create() { return 0xD503201F; }
};

struct RETBits
{
    inline static uint32_t Create() { return 0xD65F03C0; }
};

struct CMPBits // CMP is an alias for SUBS
{
    inline static uint32_t Create(uint32_t _imm, uint32_t _reg, bool isXreg, bool lslShift12 = false)
    {
        uint32_t basic = isXreg ? 0xF100001F : 0x7100001F;
        basic |= ((_imm & 0xFFF) << 10) | ((_reg & 0x1F) << 5) | (lslShift12 << 22);
        return basic;
    }
    inline static uint8_t GetShift(uint32_t opcode) { return ((opcode >> 22) & 0x1) * 12; }
    inline static uint32_t GetImm(uint32_t opcode) { return ((opcode >> 10) & 0xFFF) << GetShift(opcode); }
    inline static uint8_t GetRd(uint32_t opcode) { return ((opcode >> 5) & 0x1F); }
    inline static bool IsX(uint32_t opcode) { return (opcode & 0x80000000) != 0; }
    inline static uint32_t GetMaxImm() { return 0xFFF; }
};

struct CMPRegBits
{
    inline static uint32_t Create(uint32_t _reg, uint32_t _regWith, bool isXreg, uint8_t lslRegShift = 0)
    {
        uint32_t basic = isXreg ? 0xEB00001F : 0x6B00001F;
        uint8_t option = lslRegShift / 8;
        lslRegShift %= 8;
        basic |= ((_regWith & 0x1F) << 16) | ((_reg & 0x1F) << 5) | ((lslRegShift & 0x7) << 10) | ((option & 0x7) << 13);
        return basic;
    }
    inline static uint8_t GetOption(uint32_t opcode) { return (opcode >> 13) & 0x7; }
    inline static uint8_t GetShift(uint32_t opcode) { return ((opcode >> 10) & 0x7) + 8 * GetOption(opcode); }
    inline static uint8_t GetRn(uint32_t opcode) { return ((opcode >> 5) & 0x1F); }
    inline static uint8_t GetRm(uint32_t opcode) { return ((opcode >> 16) & 0x1F); }
    inline static bool IsX(uint32_t opcode) { return (opcode & 0x80000000) != 0; }
};

struct CMNBits // CMN is an alias for ADDS
{
    inline static uint32_t Create(uint32_t _imm, uint32_t _reg, bool isXreg, bool lslShift12 = false)
    {
        uint32_t basic = isXreg ? 0xB100001F : 0x3100001F;
        basic |= ((_imm & 0xFFF) << 10) | ((_reg & 0x1F) << 5) | (lslShift12 << 22);
        return basic;
    }
    inline static uint8_t GetShift(uint32_t opcode) { return ((opcode >> 22) & 0x1) * 12; }
    inline static uint16_t GetImm(uint32_t opcode) { return ((opcode >> 10) & 0xFFF) << GetShift(opcode); }
    inline static uint8_t GetRd(uint32_t opcode) { return ((opcode >> 5) & 0x1F); }
    inline static bool IsX(uint32_t opcode) { return (opcode & 0x80000000) != 0; }
    inline static uint32_t GetMaxImm() { return 0xFFF; }
};

struct CMNRegBits
{
    inline static uint32_t Create(uint32_t _reg, uint32_t _regWith, bool isXreg, uint8_t lslRegShift = 0)
    {
        uint32_t basic = isXreg ? 0xAB00001F : 0x2B00001F;
        uint8_t option = lslRegShift / 8;
        lslRegShift %= 8;
        basic |= ((_regWith & 0x1F) << 16) | ((_reg & 0x1F) << 5) | ((lslRegShift & 0x7) << 10) | ((option & 0x7) << 13);
        return basic;
    }
    inline static uint8_t GetOption(uint32_t opcode) { return (opcode >> 13) & 0x7; }
    inline static uint8_t GetShift(uint32_t opcode) { return ((opcode >> 10) & 0x7) + 8 * GetOption(opcode); }
    inline static uint8_t GetRn(uint32_t opcode) { return ((opcode >> 5) & 0x1F); }
    inline static uint8_t GetRm(uint32_t opcode) { return ((opcode >> 16) & 0x1F); }
    inline static bool IsX(uint32_t opcode) { return (opcode & 0x80000000) != 0; }
};

struct MOVBits // A real MOV (not ORR or ADDS)
{
    inline static uint32_t Create(uint32_t _imm, uint32_t _reg, bool isXreg)
    {
        uint32_t basic = isXreg ? 0xD2800000 : 0x52800000;
        basic |= ((_imm & 0xFFFF) << 5) | (_reg & 0x1F);
        return basic;
    }
    inline static uint16_t GetImm(uint32_t opcode) { return (opcode >> 5) & 0xFFFF; }
    inline static uint8_t GetRd(uint32_t opcode) { return (opcode & 0x1F); }
    inline static bool IsX(uint32_t opcode) { return (opcode & 0x80000000) != 0; }
    inline static uint32_t GetMaxImm() { return 0xFFFF; }
};

struct MOVNBits // A real MOVN (not ORR or ADDS)
{
    inline static uint32_t Create(uint32_t _imm, uint32_t _reg, bool isXreg)
    {
        uint32_t basic = isXreg ? 0x92800000 : 0x12800000;
        basic |= (((_imm & 0xFFFF) - 1) << 5) | (_reg & 0x1F);
        return basic;
    }
    inline static uint16_t GetImm(uint32_t opcode) { return 1 + ((opcode >> 5) & 0xFFFF); }
    inline static uint8_t GetRd(uint32_t opcode) { return (opcode & 0x1F); }
    inline static bool IsX(uint32_t opcode) { return (opcode & 0x80000000) != 0; }
    inline static uint32_t GetMaxImm() { return 0xFFFF; }
};

struct BBits
{
    inline static uint32_t Create(uintptr_t from, uintptr_t to, bool isBL = false)
    {
        uint32_t basic = isBL ? 0x94000000 : 0x14000000;
        basic |= (((to - from) >> 2) & 0x03FFFFFF);
        return basic;
    }
    inline static uint32_t GetImm(uint32_t opcode) { return ((opcode & 0x03FFFFFF) << 2); }
    inline static uintptr_t GetDest(uint32_t opcode, uintptr_t pos) { return GetImm(opcode) + pos; }
    inline static bool IsBL(uint32_t opcode) { return (opcode & 0x80000000) != 0; }
    inline static uint32_t GetMaxImm() { return 0x03FFFFFF; }
    inline static uint32_t GetMaxDist() { return 0x0FFFFFFC; }
};

struct BRBits
{
    inline static uint32_t Create(uint32_t _reg, bool isBLR = false)
    {
        uint32_t basic = isBLR ? 0xD63F0000 : 0xD61F0000;
        basic |= ((_reg & 0x1F) << 5);
        return basic;
    }
    inline static uint8_t GetRn(uint32_t opcode) { return ((opcode >> 5) & 0x1F); }
    inline static bool IsBLR(uint32_t opcode) { return (opcode & 0x00200000) != 0; }
};

struct BCondBits
{
    inline static uint32_t Create(uintptr_t from, uintptr_t to, eCond cond)
    {
        uint32_t basic = 0x54000000 | ((cond - COND_EQ) & 0xF);
        basic |= (((to - from) >> 2) & 0x7FFFF) << 5;
        return basic;
    }
    inline static uint32_t GetImm(uint32_t opcode) { return (((opcode >> 5) & 0x7FFFF) << 2); }
    inline static uintptr_t GetDest(uint32_t opcode, uintptr_t pos) { return GetImm(opcode) + pos; }
    inline static eCond GetCond(uint32_t opcode) { return (eCond)(opcode & 0xF); }
    inline static uint32_t GetMaxImm() { return 0x7FFFF; }
    inline static uint32_t GetMaxDist() { return 0x001FFFFC; }
};

struct ADRBits
{
    inline static uint32_t Create(uintptr_t from, uintptr_t to, uint32_t _reg, bool isADRP)
    {
        uint32_t basic = isADRP ? 0x90000000 : 0x10000000;
        uintptr_t offset = ((to & 0xFFFFF000) - (from & 0xFFFFF000)) >> 12;
        basic |= (((offset >> 2) & 0x7FFFF) << 5) | ((offset & 0x3) << 29) | ((_reg) & 0x1F);
        return basic;
    }
    inline static uint8_t GetRd(uint32_t opcode) { return (opcode & 0x1F); }
    inline static uint32_t GetImm(uint32_t opcode)
    {
        return ((((opcode >> 5) & 0x7FFFF) << 2) | (((opcode >> 29) & 0x3)) << 12);
    }
    inline static uintptr_t GetDest(uint32_t opcode, uintptr_t pos) { return GetImm(opcode) + pos & 0xFFFFF000; }
    inline static bool IsADRP(uint32_t opcode) { return (opcode & 0x80000000) != 0; }
};

struct MOVRegBits
{
    inline static uint32_t Create(uint32_t _destReg, uint32_t _fromReg, bool isXreg, bool isMVN = false)
    {
        uint32_t basic = isXreg ? 0xAA0003E0 : 0x2A0003E0;
        basic |= (_destReg & 0x1F) | ((_fromReg & 0x1F) << 16) | (isMVN << 21);
        return basic;
    }
    inline static bool IsMVN(uint32_t opcode) { return ((opcode >> 21) & 0x1); }
    inline static uint8_t GetRd(uint32_t opcode) { return (opcode & 0x1F); }
    inline static uint8_t GetRm(uint32_t opcode) { return ((opcode >> 16) & 0x1F); }
    inline static bool IsX(uint32_t opcode) { return (opcode & 0x80000000) != 0; }
};

struct LDRBits
{
    inline static uint32_t Create(uint32_t _destReg, uint32_t _fromReg, bool isXreg, uint32_t _fromOffset = 0)
    {
        uint32_t basic = isXreg ? 0xF9400000 : 0xB9400000;
        basic |= (((_fromOffset >> (2 + isXreg)) & 0xFFF) << 10) | (((_fromReg) & 0x1F) << 5) | ((_destReg) & 0x1F);
        return basic;
    }
    inline static uint8_t GetRd(uint32_t opcode) { return (opcode & 0x1F); }
    inline static uint8_t GetRn(uint32_t opcode) { return ((opcode >> 5) & 0x1F); }
    inline static uint32_t GetImm(uint32_t opcode) { return ((opcode >> 10) & 0xFFF) << (2 + IsX(opcode)); }
    inline static bool IsX(uint32_t opcode) { return (opcode & 0x40000000) != 0; }
};

struct LDRBBits
{
    inline static uint32_t Create(uint32_t _destReg, uint32_t _fromReg, uint32_t _fromOffset = 0)
    {
        uint32_t basic = 0x39400000;
        basic |= (((_fromOffset) & 0xFFF) << 10) | (((_fromReg) & 0x1F) << 5) | ((_destReg) & 0x1F);
        return basic;
    }
    inline static uint8_t GetRd(uint32_t opcode) { return (opcode & 0x1F); }
    inline static uint8_t GetRn(uint32_t opcode) { return ((opcode >> 5) & 0x1F); }
    inline static uint32_t GetImm(uint32_t opcode) { return ((opcode >> 10) & 0xFFF); }
};

struct LDRHBits
{
    inline static uint32_t Create(uint32_t _destReg, uint32_t _fromReg, uint32_t _fromOffset = 0)
    {
        uint32_t basic = 0x79400000;
        basic |= (((_fromOffset >> 1) & 0xFFF) << 10) | (((_fromReg) & 0x1F) << 5) | ((_destReg) & 0x1F);
        return basic;
    }
    inline static uint8_t GetRd(uint32_t opcode) { return (opcode & 0x1F); }
    inline static uint8_t GetRn(uint32_t opcode) { return ((opcode >> 5) & 0x1F); }
    inline static uint32_t GetImm(uint32_t opcode) { return ((opcode >> 10) & 0xFFF) << 1; }
};

struct LDRSBits
{
    inline static uint32_t Create(uint32_t _destReg, uint32_t _fromReg, uint32_t _fromOffset = 0)
    {
        uint32_t basic = 0xBD400000;
        basic |= (((_fromOffset >> 2) & 0xFFF) << 10) | (((_fromReg) & 0x1F) << 5) | ((_destReg) & 0x1F);
        return basic;
    }
    inline static uint8_t GetRd(uint32_t opcode) { return (opcode & 0x1F); }
    inline static uint8_t GetRn(uint32_t opcode) { return ((opcode >> 5) & 0x1F); }
    inline static uint32_t GetImm(uint32_t opcode) { return ((opcode >> 10) & 0xFFF) << 2; }
};

struct LDRDBits
{
    inline static uint32_t Create(uint32_t _destReg, uint32_t _fromReg, uint32_t _fromOffset = 0)
    {
        uint32_t basic = 0x5C000000;
        basic |= (((_fromOffset >> 2) & 0xFFF) << 10) | (((_fromReg) & 0x1F) << 5) | ((_destReg) & 0x1F);
        return basic;
    }
    inline static uint8_t GetRd(uint32_t opcode) { return (opcode & 0x1F); }
    inline static uint8_t GetRn(uint32_t opcode) { return ((opcode >> 5) & 0x1F); }
    inline static uint32_t GetImm(uint32_t opcode) { return ((opcode >> 10) & 0xFFF) << 2; }
};

struct LDRLiteralBits
{
    inline static uint32_t Create(uint32_t _destReg, bool isXreg, uint32_t _fromOffset = 0)
    {
        uint32_t basic = isXreg ? 0x58000000 : 0x18000000;
        basic |= (((_fromOffset >> 2) & 0x7FFFF) << 5) | ((_destReg) & 0x1F);
        return basic;
    }
    inline static uint8_t GetRt(uint32_t opcode) { return (opcode & 0x1F); }
    inline static uint32_t GetImm(uint32_t opcode) { return ((opcode >> 5) & 0x7FFFF); }
    inline static bool IsX(uint32_t opcode) { return (opcode & 0x40000000) != 0; }
};

struct LDRSLiteralBits
{
    inline static uint32_t Create(uint32_t _destReg, uint32_t _fromOffset = 0)
    {
        uint32_t basic = 0x1C000000;
        basic |= (((_fromOffset >> 2) & 0x7FFFF) << 5) | ((_destReg) & 0x1F);
        return basic;
    }
    inline static uint8_t GetRt(uint32_t opcode) { return (opcode & 0x1F); }
    inline static uint32_t GetImm(uint32_t opcode) { return ((opcode >> 5) & 0x7FFFF); }
};

struct STRBits
{
    inline static uint32_t Create(uint32_t _saveReg, uint32_t _toReg, bool isXreg, uint32_t _toOffset = 0)
    {
        uint32_t basic = isXreg ? 0xF9000000 : 0xB9000000;
        basic |= (((_toOffset >> (2 + isXreg)) & 0xFFF) << 10) | (((_toReg) & 0x1F) << 5) | ((_saveReg) & 0x1F);
        return basic;
    }
    inline static uint8_t GetRt(uint32_t opcode) { return (opcode & 0x1F); }
    inline static uint8_t GetRn(uint32_t opcode) { return ((opcode >> 5) & 0x1F); }
    inline static uint32_t GetImm(uint32_t opcode) { return ((opcode >> 10) & 0xFFF) << (2 + IsX(opcode)); }
    inline static bool IsX(uint32_t opcode) { return (opcode & 0x40000000) != 0; }
};

struct STRBBits
{
    inline static uint32_t Create(uint32_t _saveReg, uint32_t _toReg, uint32_t _toOffset = 0)
    {
        uint32_t basic = 0x39000000;
        basic |= ((_toOffset & 0xFFF) << 10) | (((_toReg) & 0x1F) << 5) | ((_saveReg) & 0x1F);
        return basic;
    }
    inline static uint8_t GetRt(uint32_t opcode) { return (opcode & 0x1F); }
    inline static uint8_t GetRn(uint32_t opcode) { return ((opcode >> 5) & 0x1F); }
    inline static uint32_t GetImm(uint32_t opcode) { return (opcode >> 10) & 0xFFF; }
};

struct STRHBits
{
    inline static uint32_t Create(uint32_t _saveReg, uint32_t _toReg, uint32_t _toOffset = 0)
    {
        uint32_t basic = 0x79000000;
        basic |= (((_toOffset >> 1) & 0xFFF) << 10) | (((_toReg) & 0x1F) << 5) | ((_saveReg) & 0x1F);
        return basic;
    }
    inline static uint8_t GetRt(uint32_t opcode) { return (opcode & 0x1F); }
    inline static uint8_t GetRn(uint32_t opcode) { return ((opcode >> 5) & 0x1F); }
    inline static uint32_t GetImm(uint32_t opcode) { return ((opcode >> 10) & 0xFFF) << 1; }
};

struct STRSBits
{
    inline static uint32_t Create(uint32_t _saveReg, uint32_t _toReg, uint32_t _toOffset = 0)
    {
        uint32_t basic = 0xBD000000;
        basic |= (((_toOffset >> 2) & 0xFFF) << 10) | (((_toReg) & 0x1F) << 5) | ((_saveReg) & 0x1F);
        return basic;
    }
    inline static uint8_t GetRt(uint32_t opcode) { return (opcode & 0x1F); }
    inline static uint8_t GetRn(uint32_t opcode) { return ((opcode >> 5) & 0x1F); }
    inline static uint32_t GetImm(uint32_t opcode) { return ((opcode >> 10) & 0xFFF) << 2; }
};

struct SUBBits
{
    inline static uint32_t Create(uint32_t _destReg, uint32_t _whatReg, uint32_t _imm, bool isXreg)
    {
        uint32_t basic = isXreg ? 0xD1000000 : 0x51000000;
        basic |= ((_destReg) & 0x1F) | (((_whatReg) & 0x1F) << 5) | (((_imm) & 0xFFF) << 10);
        return basic;
    }
    inline static uint8_t GetRd(uint32_t opcode) { return (opcode & 0x1F); }
    inline static uint8_t GetRn(uint32_t opcode) { return ((opcode >> 5) & 0x1F); }
    inline static uint32_t GetImm(uint32_t opcode) { return ((opcode >> 10) & 0xFFF); }
    inline static bool IsX(uint32_t opcode) { return (opcode & 0x80000000) != 0; }
    inline static uint32_t GetMaxImm() { return 0xFFF; }
};

struct SUBRegBits
{
    inline static uint32_t Create(uint32_t _destReg, uint32_t _whatReg, uint32_t _withReg, bool isXreg)
    {
        uint32_t basic = isXreg ? 0xCB000000 : 0x4B000000;
        basic |= ((_destReg) & 0x1F) | (((_whatReg) & 0x1F) << 5) | (((_withReg) & 0x1F) << 16);
        return basic;
    }
    inline static uint8_t GetRd(uint32_t opcode) { return (opcode & 0x1F); }
    inline static uint8_t GetRn(uint32_t opcode) { return ((opcode >> 5) & 0x1F); }
    inline static uint8_t GetRm(uint32_t opcode) { return ((opcode >> 16) & 0x1F); }
    inline static bool IsX(uint32_t opcode) { return (opcode & 0x80000000) != 0; }
};

struct SUBSPBits
{
    inline static uint32_t Create(uint32_t _imm) { return SUBBits::Create(31, 31, _imm, true); }
    inline static uint32_t GetImm(uint32_t opcode) { return SUBBits::GetImm(opcode); }
    inline static uint32_t GetMaxImm() { return SUBBits::GetMaxImm(); }
};

struct ADDBits
{
    inline static uint32_t Create(uint32_t _destReg, uint32_t _whatReg, uint32_t _imm, bool isXreg)
    {
        uint32_t basic = isXreg ? 0x91000000 : 0x11000000;
        basic |= ((_destReg) & 0x1F) | (((_whatReg) & 0x1F) << 5) | (((_imm) & 0xFFF) << 10);
        return basic;
    }
    inline static uint8_t GetRd(uint32_t opcode) { return (opcode & 0x1F); }
    inline static uint8_t GetRn(uint32_t opcode) { return ((opcode >> 5) & 0x1F); }
    inline static uint32_t GetImm(uint32_t opcode) { return ((opcode >> 10) & 0xFFF); }
    inline static bool IsX(uint32_t opcode) { return (opcode & 0x80000000) != 0; }
    inline static uint32_t GetMaxImm() { return 0xFFF; }
};

struct ADDRegBits
{
    inline static uint32_t Create(uint32_t _destReg, uint32_t _whatReg, uint32_t _withReg, bool isXreg)
    {
        uint32_t basic = isXreg ? 0x8B000000 : 0x0B000000;
        basic |= ((_destReg) & 0x1F) | (((_whatReg) & 0x1F) << 5) | (((_withReg) & 0x1F) << 16);
        return basic;
    }
    inline static uint8_t GetRd(uint32_t opcode) { return (opcode & 0x1F); }
    inline static uint8_t GetRn(uint32_t opcode) { return ((opcode >> 5) & 0x1F); }
    inline static uint8_t GetRm(uint32_t opcode) { return ((opcode >> 16) & 0x1F); }
    inline static bool IsX(uint32_t opcode) { return (opcode & 0x80000000) != 0; }
};

struct ADDSPBits
{
    inline static uint32_t Create(uint32_t _imm) { return ADDBits::Create(31, 31, _imm, true); }
    inline static uint32_t GetImm(uint32_t opcode) { return ADDBits::GetImm(opcode); }
    inline static uint32_t GetMaxImm() { return ADDBits::GetMaxImm(); }
};

struct CBZBits
{
    inline static uint32_t Create(uintptr_t from, uintptr_t to, uint32_t _reg, bool isXreg)
    {
        uint32_t basic = isXreg ? 0xB4000000 : 0x34000000;
        basic |= ((((to - from) >> 2) & 0x7FFFF) << 5) | (_reg & 0x1F);
        return basic;
    }
    inline static uint8_t GetRn(uint32_t opcode) { return (opcode & 0x1F); }
    inline static uint32_t GetImm(uint32_t opcode) { return ((opcode >> 5) & 0x7FFFF) << 2; }
    inline static uintptr_t GetDest(uint32_t opcode, uintptr_t pos) { return GetImm(opcode) + pos; }
    inline static bool IsX(uint32_t opcode) { return (opcode & 0x80000000) != 0; }
    inline static uint32_t GetMaxImm() { return 0x7FFFF; }
    inline static uint32_t GetMaxDist() { return 0x001FFFFC; }
};

struct CBNZBits
{
    inline static uint32_t Create(uintptr_t from, uintptr_t to, uint32_t _reg, bool isXreg)
    {
        uint32_t basic = isXreg ? 0xB5000000 : 0x35000000;
        basic |= ((((to - from) >> 2) & 0x7FFFF) << 5) | (_reg & 0x1F);
        return basic;
    }
    inline static uint8_t GetRn(uint32_t opcode) { return (opcode & 0x1F); }
    inline static uint32_t GetImm(uint32_t opcode) { return ((opcode >> 5) & 0x7FFFF) << 2; }
    inline static uintptr_t GetDest(uint32_t opcode, uintptr_t pos) { return GetImm(opcode) + pos; }
    inline static bool IsX(uint32_t opcode) { return (opcode & 0x80000000) != 0; }
    inline static uint32_t GetMaxImm() { return 0x7FFFF; }
    inline static uint32_t GetMaxDist() { return 0x001FFFFC; }
};

struct RBITBits // Reverse BITS
{
    inline static uint32_t Create(uint32_t _destReg, uint32_t _fromReg, bool isXreg)
    {
        uint32_t basic = isXreg ? 0xDAC00000 : 0x5AC00000;
        basic |= (_destReg & 0x1F) | ((_fromReg & 0x1F) << 5);
        return basic;
    }
    inline static uint8_t GetRd(uint32_t opcode) { return (opcode & 0x1F); }
    inline static uint8_t GetRn(uint32_t opcode) { return ((opcode >> 5) & 0x1F); }
    inline static bool IsX(uint32_t opcode) { return (opcode & 0x80000000) != 0; }
};

struct REVBits // Reverse BYTES
{
    inline static uint32_t Create(uint32_t _destReg, uint32_t _fromReg, bool isXreg)
    {
        uint32_t basic = isXreg ? 0xDAC00C00 : 0x5AC00800;
        basic |= (_destReg & 0x1F) | ((_fromReg & 0x1F) << 5);
        return basic;
    }
    inline static uint8_t GetRd(uint32_t opcode) { return (opcode & 0x1F); }
    inline static uint8_t GetRn(uint32_t opcode) { return ((opcode >> 5) & 0x1F); }
    inline static bool IsX(uint32_t opcode) { return (opcode & 0x80000000) != 0; }
};

struct MULBits // DestReg = WhatReg * WithReg
{
    inline static uint32_t Create(uint32_t _destReg, uint32_t _whatReg, uint32_t _withReg, bool isXreg)
    {
        uint32_t basic = isXreg ? 0x9B007C00 : 0x1B007C00;
        basic |= ((_destReg) & 0x1F) | (((_whatReg) & 0x1F) << 5) | (((_withReg) & 0x1F) << 16);
        return basic;
    }
    inline static uint8_t GetRd(uint32_t opcode) { return (opcode & 0x1F); }
    inline static uint8_t GetRn(uint32_t opcode) { return ((opcode >> 5) & 0x1F); }
    inline static uint8_t GetRm(uint32_t opcode) { return ((opcode >> 16) & 0x1F); }
    inline static bool IsX(uint32_t opcode) { return (opcode & 0x80000000) != 0; }
};

struct MNEGBits // DestReg = -(WhatReg * WithReg)
{
    inline static uint32_t Create(uint32_t _destReg, uint32_t _whatReg, uint32_t _withReg, bool isXreg)
    {
        uint32_t basic = isXreg ? 0x9B00FC00 : 0x1B00FC00;
        basic |= ((_destReg) & 0x1F) | (((_whatReg) & 0x1F) << 5) | (((_withReg) & 0x1F) << 16);
        return basic;
    }
    inline static uint8_t GetRd(uint32_t opcode) { return (opcode & 0x1F); }
    inline static uint8_t GetRn(uint32_t opcode) { return ((opcode >> 5) & 0x1F); }
    inline static uint8_t GetRm(uint32_t opcode) { return ((opcode >> 16) & 0x1F); }
    inline static bool IsX(uint32_t opcode) { return (opcode & 0x80000000) != 0; }
};



// INCOMPLETE BELOW !!!

union FMOVBits // FMOV (incomplete)
{
    inline static uint32_t Create(float _imm, uint32_t _reg, bool isDoublePrec)
    {
        FMOVBits val; val.addr = isDoublePrec ? 0x1E601000 : 0x1E201000;
    
        val.reg = _reg;
        val.imm = _imm;
    
        return val.addr;
    }
    inline float GetValue()
    {
        uint32_t val;
        if(imm < 64)
        {
            val = 0x40000000 | (imm*0x100000)>>1;
            return *(float*)&val;
        }
        else if(imm < 128)
        {
            val = 0x3E000000 | ((imm-64)*0x100000)>>1;
            return *(float*)&val;
        }
        else if(imm < 192)
        {
            val = 0xC0000000 | ((imm-128)*0x100000)>>1;
            return *(float*)&val;
        }
        else
        {
            val = 0xBE000000 | ((imm-192)*0x100000)>>1;
            return *(float*)&val;
        }
    }
    struct
    {
        uint32_t reg  : 5;
        uint32_t pad1 : 8;
        uint32_t imm  : 8;
        uint32_t pad2 : 11;
    };
    uint32_t addr;
};

};

#endif // ARMV8_ASMHELPER_H