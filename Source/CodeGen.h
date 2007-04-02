#ifndef _CODEGEN_H_
#define _CODEGEN_H_

#include "CacheBlock.h"
#include "ArrayStack.h"

namespace CodeGen
{
	class CFPU;
}

class CCodeGen
{
public:
	enum CONDITION
	{
		CONDITION_EQ,
		CONDITION_NE,
		CONDITION_BL,
		CONDITION_BE,
		CONDITION_LT,
		CONDITION_AB,
		CONDITION_GT,
		CONDITION_LE,
	};

	friend							CodeGen::CFPU;

	static void						Begin(CCacheBlock*);
	static void						End();

	static void						BeginIf(bool);
	static void						EndIf();

	static void						BeginIfElse(bool);
	static void						BeginIfElseAlt();

	static void						PushVar(uint32*);
	static void						PushCst(uint32);
	static void						PushRef(void*);
	static void						PushRel(size_t);
	static void						PushTop();
	static void						PushIdx(unsigned int);

	static void						PullVar(uint32*);
	static void						PullRel(size_t);
	static void						PullTop();

	static void						Add();
	static void						Add64();
	static void						And();
	static void						And64();
	static void						Call(void*, unsigned int, bool);
	static void						Cmp(CONDITION);
	static void						Cmp64(CONDITION);
	static void						Lzc();
    static void                     MultS();
	static void						Or();
	static void						SeX();
	static void						SeX16();
	static void						Shl(uint8);
	static void						Shl64();
	static void						Shl64(uint8);
    static void                     Sra(uint8);
    static void                     Srl(uint8);
	static void						Srl64();
	static void						Srl64(uint8);
	static void						Sub();
	static void						Xor();

private:
	enum MAX_STACK
	{
		MAX_STACK = 0x100,
	};

	enum MAX_REGISTER
	{
#ifdef AMD64
		MAX_REGISTER = 14,
#else
		MAX_REGISTER = 6,
#endif
	};

	enum REL_REGISTER
	{
		REL_REGISTER = 5,
	};

	enum SYMBOLS
	{
		VARIABLE = 0x8000,
		REGISTER,
		CONSTANT,
		REFERENCE,
		RELATIVE,
#ifdef AMD64
		REGISTER64,
#endif
	};

	enum IFBLOCKS
	{
		IFBLOCK,
		IFELSEBLOCK,
	};

    enum SPECREGS
    {
        REGISTER_EAX = 0,
        REGISTER_EDX = 2,
    };

	enum REGISTER_TYPE
	{
		REGISTER_NORMAL,
		REGISTER_HASLOW,
		REGISTER_SAVED,
		REGISTER_SHIFTAMOUNT,
	};

	static unsigned int				AllocateRegister(REGISTER_TYPE = REGISTER_NORMAL);
	static void						FreeRegister(unsigned int);
	static unsigned int				GetMinimumConstantSize(uint32);
	static bool						RegisterHasNextUse(unsigned int);
	static void						LoadVariableInRegister(unsigned int, uint32);
	static void						LoadRelativeInRegister(unsigned int, uint32);
	static void						LoadConstantInRegister(unsigned int, uint32);
	static void						CopyRegister(unsigned int, unsigned int);
#ifdef AMD64
	static void						LoadRelativeInRegister64(unsigned int, uint32);
	static void						LoadConstantInRegister64(unsigned int, uint64);
#endif

	static void						LoadConditionInRegister(unsigned int, CONDITION);

	static void						ReduceToRegister();

	static void						WriteRelativeRm(unsigned int, uint32);
	static void						WriteRelativeRmRegister(unsigned int, uint32);
	static void						WriteRelativeRmFunction(unsigned int, uint32);

	static uint8					MakeRegRegRm(unsigned int, unsigned int);
	static uint8					MakeRegFunRm(unsigned int, unsigned int);

	static bool						IsTopRegCstPairCom();
	static bool						IsTopRegZeroPairCom();
	static void						GetRegCstPairCom(unsigned int*, uint32*);
    
    template <typename T> 
    static void                     UnaryRelativeSelfCallAsRegister(const T& Function)
    {
		uint32 nRelative;
		unsigned int nRegister;

		m_Shadow.Pull();
		nRelative = m_Shadow.Pull();

		nRegister = AllocateRegister();
		LoadRelativeInRegister(nRegister, nRelative);

		m_Shadow.Push(nRegister);
		m_Shadow.Push(REGISTER);

		Function();
    }

	static void						PushReg(unsigned int);
#ifdef AMD64
	static void						PushReg64(unsigned int);
#endif
	static void						ReplaceRegisterInStack(unsigned int, unsigned int);

	static void						Cmp64Eq();
	static void						Cmp64Lt(bool);

	static bool						m_nBlockStarted;

	static CArrayStack<uint32>		m_Shadow;
#ifdef AMD64
	static CArrayStack<uint32, 2>	m_PullReg64Stack;
#endif
	static bool						m_nRegisterAllocated[MAX_REGISTER];
	static unsigned int				m_nRegisterLookup[MAX_REGISTER];
	static CCacheBlock*				m_pBlock;

	static CArrayStack<uint32>		m_IfStack;
};

#endif
