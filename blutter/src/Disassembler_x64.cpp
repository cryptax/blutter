#include "pch.h"
#include "Disassembler.h"

namespace X64 {
const char* Register::RegisterNames[] = {
  "rax", "rcx", "rdx", "rbx", "rsp", "rbp", "rsi", "rdi", "r8", "r9",
  "r10", "r11", "r12", "r13", "rTHR", "rPP",
};
}

// singleton of capstone handle. use for global resolve register name  of aarch64
static csh g_cshandle;
const char* GetCsRegisterName(x86_reg reg)
{
	if (g_cshandle == 0)
		cs_open(CS_ARCH_X64, CS_MODE_LITTLE_ENDIAN, &g_cshandle);
	return cs_reg_name(g_cshandle, reg);
}

Disassembler::Disassembler(bool hasDetail)
{
	if (cs_open(CS_ARCH_X64, CS_MODE_LITTLE_ENDIAN, &cshandle) != CS_ERR_OK)
		throw std::runtime_error("Cannot open capstone engine");

	if (hasDetail)
		cs_option(cshandle, CS_OPT_DETAIL, CS_OPT_ON);
}
