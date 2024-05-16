#pragma once
#include "Disassembler.h"

constexpr x86_reg ToCapstoneReg(dart::Register r)
{
#define REG_CASE(n) case dart::Register::R##n: \
		return X86_REG_R##n

	switch (r) {
		REG_CASE(AX); REG_CASE(BX); REG_CASE(CX); REG_CASE(DX); REG_CASE(SP);
		REG_CASE(BP); REG_CASE(SI); REG_CASE(DI); REG_CASE(8); REG_CASE(9);
		REG_CASE(10); REG_CASE(11); REG_CASE(12); REG_CASE(13); REG_CASE(14);
		REG_CASE(15);
	default:
		return X86_REG_INVALID;
	}
#undef REG_CASE
}

constexpr x86_reg ToCapstoneReg(x86_reg r)
{
#define REG_CASE(n) case X86_REG_R##n: \
  		return X86_REG_R##n

	switch (r) {
	  	REG_CASE(AX); REG_CASE(BX); REG_CASE(CX); REG_CASE(DX); REG_CASE(SP);
		REG_CASE(BP); REG_CASE(SI); REG_CASE(DI); REG_CASE(8); REG_CASE(9);
		REG_CASE(10); REG_CASE(11); REG_CASE(12); REG_CASE(13); REG_CASE(14);
		REG_CASE(15);
	default:
		return X86_REG_INVALID;
	}
#undef REG_CASE
}

constexpr dart::Register ToDartReg(x86_reg r)
{
#define REG_CASE(n) case X86_REG_R##n: \
		return dart::Register::R##n

	switch (r) {
	  REG_CASE(AX); REG_CASE(BX); REG_CASE(CX); REG_CASE(DX); REG_CASE(SP);
		REG_CASE(BP); REG_CASE(SI); REG_CASE(DI); REG_CASE(8); REG_CASE(9);
		REG_CASE(10); REG_CASE(11); REG_CASE(12); REG_CASE(13); REG_CASE(14);
		REG_CASE(15);
	default:
		return dart::Register::kNoRegister;}
	
#undef REG_CASE
}

constexpr x86_reg CSREG_ARGS_DESC = ToCapstoneReg(dart::ARGS_DESC_REG);
constexpr x86_reg CSREG_DART_SP = ToCapstoneReg(dart::SPREG);
constexpr x86_reg CSREG_DART_FP = ToCapstoneReg(dart::FPREG);
constexpr x86_reg CSREG_DART_WB_OBJECT = ToCapstoneReg(dart::kWriteBarrierObjectReg);
constexpr x86_reg CSREG_DART_WB_VALUE = ToCapstoneReg(dart::kWriteBarrierValueReg);
constexpr x86_reg CSREG_DART_WB_SLOT = ToCapstoneReg(dart::kWriteBarrierSlotReg);
constexpr x86_reg CSREG_DART_THR = ToCapstoneReg(dart::THR);
constexpr x86_reg CSREG_DART_PP = ToCapstoneReg(dart::PP);
constexpr x86_reg CSREG_DART_TMP = ToCapstoneReg(dart::TMP);

const char* GetCsRegisterName(x86_reg reg);

inline constexpr uint32_t GetCsRegSize(x86_reg reg){
  /*if (reg >= X86_REG_Q0 && reg <= X86_REG_Q31)
		return 16;
	if ((reg >= X86_REG_W0 && reg <= X86_REG_W30) || reg == X86_REG_WZR)
		return 4;
		// assume Xnn regsiter*/
	return 8;
}

namespace X64 {

class alignas(int32_t) Register {
public:
	// copy from dart constants_x64.h by merging VRegister into Register
	enum Value : int32_t {
    RAX = 0,
    RCX = 1,
    RDX = 2,
    RBX = 3,
    RSP = 4,  // SP
    RBP = 5,  // FP
    RSI = 6,
    RDI = 7,
    R8 = 8,
    R9 = 9,
    R10 = 10,
    R11 = 11,  // TMP
    R12 = 12,  // CODE_REG
    R13 = 13,
    R14 = 14,  // THR
    R15 = 15,  // PP
    kNumberOfCpuRegisters = 16,
    kNoRegister = -1,  // Signals an illegal register.
  	};

	constexpr Register() : reg(kNoRegister) {}
	constexpr Register(Value reg) : reg(reg) {}
	// map from Dart Register enum
	constexpr Register(dart::Register r) {
		switch (r) {
#define REG_CASE(n) case dart::Register::R##n: \
		reg = R##n; break
		  REG_CASE(AX); REG_CASE(BX); REG_CASE(CX); REG_CASE(DX); REG_CASE(SP);
		REG_CASE(BP); REG_CASE(SI); REG_CASE(DI);
		  REG_CASE(8); REG_CASE(9);
		  REG_CASE(10); REG_CASE(11); REG_CASE(12); REG_CASE(13); REG_CASE(14);
		  REG_CASE(15);
#undef REG_CASE
		default:
			reg = kNoRegister;
			break;
		}
	}
	// map from capstone register
	constexpr Register(x86_reg r) {
		switch (r) {
#define REG_CASE(n) case X86_REG_R##n: \
		reg = R##n; break
		  REG_CASE(AX); REG_CASE(BX); REG_CASE(CX); REG_CASE(DX); REG_CASE(SP);
		REG_CASE(BP); REG_CASE(SI); REG_CASE(DI);
		  REG_CASE(8); REG_CASE(9);
		  REG_CASE(10); REG_CASE(11); REG_CASE(12); REG_CASE(13); REG_CASE(14);
		  REG_CASE(15);
#undef REG_CASE
		default:
		  reg = kNoRegister;
		  break;
		}
	}

	constexpr bool operator==(Register a) const { return reg == a.reg; }
	constexpr bool operator!=(Register a) const { return reg != a.reg; }
	constexpr bool operator==(Value v) const { return reg == v; }
	constexpr bool operator!=(Value v) const { return reg != v; }

	inline bool IsSet() const {
		return reg != kNoRegister;
	}

	inline bool IsDecimal() const {
	  return false;
	  //return reg >= V0 && reg <= V31;
	}

	static const char* RegisterNames[];

	inline const char* Name() const {
		ASSERT(reg != kNoRegister);
		return RegisterNames[reg];
	}

	constexpr operator int() const { return reg; }
	constexpr Value value() const {
		return reg;
	}

private:
	Value reg;
};

constexpr auto ARGS_DESC_REG = Register{ dart::ARGS_DESC_REG };
constexpr auto SP_REG = Register{ dart::SPREG };
constexpr auto TMP_REG = Register{ dart::TMP };
}; // namespace X64

constexpr x86_reg ToCapstoneReg(X64::Register r)
{
#define REG_CASE(n) case X64::Register::R##n: \
		return X86_REG_R##n

	switch (r.value()) {
	  REG_CASE(AX); REG_CASE(BX); REG_CASE(CX); REG_CASE(DX); REG_CASE(SP);
		REG_CASE(BP); REG_CASE(SI); REG_CASE(DI);
		REG_CASE(8); REG_CASE(9);
		REG_CASE(10); REG_CASE(11); REG_CASE(12); REG_CASE(13); REG_CASE(14);
		REG_CASE(15);
	default:
		return X86_REG_INVALID;
	}
#undef REG_CASE
}


class AsmInstruction {
private:
	// keep this class object small size for cheap copy
	cs_insn* insn;
public:
	class Operands {
		cs_x86_op* operands;
		Operands(cs_x86_op* operands) : operands(operands) {}
	public:
		const cs_x86_op& operator[](size_t idx) const { return operands[idx]; }
		friend class AsmInstruction;
	} ops;

	AsmInstruction(cs_insn* insn) : insn((insn->id == X86_INS_NOP) ? ++insn : insn), ops(insn->detail->x86.operands) {}
	AsmInstruction& operator=(const AsmInstruction&) = default;
	// prefix increment
	AsmInstruction& operator++() {
		++insn;
		if (insn->id == X86_INS_NOP)
			++insn;
		ops = insn->detail->x86.operands;
		return *this;
	}
	AsmInstruction& operator--() {
		--insn;
		ops = insn->detail->x86.operands;
		return *this;
	}
	AsmInstruction& operator+=(int cnt) {
		insn += cnt;
		if (insn->id == X86_INS_NOP)
			++insn;
		ops = insn->detail->x86.operands;
		return *this;
	}
	AsmInstruction Next() { return AsmInstruction(insn + 1); }
	AsmInstruction Prev() { return AsmInstruction(insn - 1); }

	AsmInstruction MoveTo(uint64_t addr) {
		const auto offset = (addr - insn->address) / 4;
		return AsmInstruction(insn + offset);
	}

	friend inline bool operator==(const AsmInstruction& lhs, const AsmInstruction& rhs) {
		return lhs.insn->size == rhs.insn->size && memcmp(lhs.insn->bytes, rhs.insn->bytes, lhs.insn->size) == 0;
	}

	// libcapstone5 use MOV instead of MOVZ. so, we need this special function.
  /*bool IsMovz() {
		return insn->id == X86_INS_MOVZX || (insn->id == X86_INS_MOV && op_count() == 2 && ops[1].type == X86_OP_IMM);
	}
	bool IsBranch(x86_sse_cc cond = X86_SSE_CC_INVALID) { return insn->id == X86_INS_JB && cc() == cond; }
	bool IsDartArrayLoad() {
		if (writeback())
			return false;
		switch (insn->id) {
		case X86_INS_LDR:
			return true;
		}
		return false;
	}
	int GetLoadSize() {
		if (insn->id == X86_INS_LDRB || insn->id == X86_INS_LDURB)
			return 1;
		return GetCsRegSize(ops[0].reg);
	}
	bool IsDartArrayStore() {
		if (writeback())
			return false;
		switch (insn->id) {
		case X86_INS_STUR:
		case X86_INS_STRB:
			return true;
		}
		return false;
	}
	int GetStoreSize() {
		if (insn->id == X86_INS_STRB || insn->id == X86_INS_STURB)
			return 1;
		return GetCsRegSize(ops[0].reg);
		}*/

	cs_insn* ptr() { return insn; }
	uint64_t address() const { return insn->address; }
	uint16_t size() const { return insn->size; }
	uint64_t NextAddress() const { return insn->address + insn->size; }
	unsigned int id() const { return insn->id; }
	x86_sse_cc cc() const { return insn->detail->x86.cc; }
	bool writeback() const { return insn->detail->x86.writeback; }
	uint8_t op_count() const { return insn->detail->x86.op_count; }

	const char* mnemonic() const { return insn->mnemonic; }
	const char* op_str() const { return insn->op_str; }
};

struct AddrRange {
	uint64_t start{ 0 };
	uint64_t end{ 0 };

	AddrRange() = default;
	AddrRange(uint64_t start, uint64_t end) : start{ start }, end{ end } {}

	bool Has(uint64_t addr) const { return addr >= start && addr < end; }
};

class AsmIterator {
	cs_insn* insnStart;
	cs_insn* insnEnd;
	cs_insn* insn; // current instruction
	cs_insn dummyInsnEnd;
public:
	AsmIterator(cs_insn* start, cs_insn* end) : insnStart(start), insnEnd(end), insn(insnStart) {
		dummyInsnEnd.id = 0;
		dummyInsnEnd.address = insnEnd->address + insnEnd->size;
		dummyInsnEnd.size = 0;
	}

	AsmIterator(AsmIterator& rhs, int64_t addr) : insnStart(rhs.insnStart), insnEnd(rhs.insnEnd) {
		const auto offset = (addr - rhs.insn->address) / 4;
		insn = rhs.insn + offset;
		dummyInsnEnd.id = 0;
		dummyInsnEnd.address = rhs.dummyInsnEnd.address;
		dummyInsnEnd.size = 0;
	}

	cs_insn* Current() { return insn; }
	void SetCurrent(cs_insn* ins) { insn = ins; }
	cs_insn* Next() {
		//return (++*this).insn;
		return operator++().insn;
	}
	// prefix increment
	AsmIterator& operator++() {
		ASSERT(insn != &dummyInsnEnd);
		// assume no consecutive NOP
		if (insn == insnEnd) {
			insn = &dummyInsnEnd;
		}
		else {
			++insn;
			if (insn->id == X86_INS_NOP) {
				if (insn == insnEnd)
					insn = &dummyInsnEnd;
				else
					++insn;
			}
		}
		return *this;
	}
	AsmIterator& operator--() {
		--insn;
		return *this;
	}
	AddrRange Wrap(int64_t start) {
		return AddrRange(start, insn->address);
	}
	bool IsEnd() {
		return insn == &dummyInsnEnd;
	}

	// libcapstone5 use MOV instead of MOVZ. so, we need this special function.
	bool IsMovz() const {
		return insn->id == X86_INS_MOVZ || (insn->id == X86_INS_MOV && op_count() == 2 && ops(1).type == X64_OP_IMM);
	}
	bool IsBranch(x86_sse_cc cond = X64_CC_INVALID) const { return insn->id == X86_INS_B && cc() == cond; }
	bool IsDartArrayLoad() const {
		if (writeback())
			return false;
		switch (insn->id) {
		case X86_INS_LDUR:
		case X86_INS_LDRB:
			return true;
		}
		return false;
	}
	int GetLoadSize() const {
		if (insn->id == X86_INS_LDRB || insn->id == X86_INS_LDURB)
			return 1;
		return GetCsRegSize(ops(0).reg);
	}
	bool IsDartArrayStore() const {
		if (writeback())
			return false;
		switch (insn->id) {
		case X86_INS_STUR:
		case X86_INS_STRB:
			return true;
		}
		return false;
	}
	int GetStoreSize() const {
		if (insn->id == X86_INS_STRB || insn->id == X86_INS_STURB)
			return 1;
		return GetCsRegSize(ops(0).reg);
	}

	uint64_t address() const { return insn->address; }
	uint16_t size() const { return insn->size; }
	uint64_t NextAddress() const { return insn->address + insn->size; }
	unsigned int id() const { return insn->id; }
	x86_sse_cc cc() const { return insn->detail->x86.cc; }
	bool writeback() const { return insn->detail->x86.writeback; }
	cs_x86_op& ops(int i) const { return insn->detail->x86.operands[i]; }
	uint8_t op_count() const { return insn->detail->x86.op_count; }
};
