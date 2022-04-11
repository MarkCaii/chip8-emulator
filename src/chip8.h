#include <cstdint>
#include <chrono>
#include <random>

const unsigned int KEY_COUNT = 16;
const unsigned int MEMORY_SIZE = 4096;
const unsigned int REGISTER_COUNT = 16;
const unsigned int STACK_LEVELS = 16;
const unsigned int VIDEO_HEIGHT = 32;
const unsigned int VIDEO_WIDTH = 64;

class Chip8
{
public:
	Chip8();
    void LoadROM(char const* filename);
	void Cycle();
	
	uint8_t keypad[16]{};
	uint32_t video[64 * 32]{};

private:
	void Table0();
	void Table8();
	void TableE();
	void TableF();
	

	std::default_random_engine randGen;
	std::uniform_int_distribution<uint8_t> randByte;

	// Do nothing
	void OP_NULL();

	// CLS clear display
	void OP_00E0();

	// RET return from subroutine
	void OP_00EE();

	// JP addr jump to location
	void OP_1nnn();

	// CALL addr call subroutine
	void OP_2nnn();

	// SE Vx, byte skip next instruction if Vx = kk
	void OP_3xkk();


	// SNE Vx, byte: skip next instruction if not equal
	void OP_4xkk();

	// SE Vx, Vy: skip if Vx = Vy
	void OP_5xy0();

	// LD Vx, byte: set Vx = kk
	void OP_6xkk();

	// ADD Vx, byte: add Vx and kk and load value into Vx
	void OP_7xkk();

	/// LD Vx, Vy: load Vy in to Vx
	void OP_8xy0();

	// OR Vx, Vy: Vx = Vx OR Vy
	void OP_8xy1();

	// AND Vx, Vy: Vx = Vx AND Vy
	void OP_8xy2();

	// XOR Vx, Vy: Vx = Vx XOR Vy
	void OP_8xy3();

	// ADD Vx, Vy: set Vx = Vx + Vy, VF = carry if the result > 255 will only store up to 8 bits
	void OP_8xy4();

	// SUB Vx, Vy: set Vx = Vx - Vy, VF = NOT borrow
	void OP_8xy5();

	// SHR Vx: set Vx = Vx shifted right by 1, if the least significant bit of Vx is 1, VF is 1 otherwise 0
	void OP_8xy6();

	// SUBN Vx, Vy: set Vx = Vy - Vx, set VF = NOT Borrow
	void OP_8xy7();

	// SHL Vx {, Vy}: set Vx = Vx SHL 1. Left shift is done and the most significant bit is saved
	void OP_8xyE();

	// SNE Vx, Vy: Skip the next instruction if Vx != Vy
	void OP_9xy0();

	// LD I, addr: set I = nnn
	void OP_Annn();

	// JP V0, addr: jump to location nnn + V0
	void OP_Bnnn();

	// RND Vx, byte: set Vx = random byte AND kk
	void OP_Cxkk();

	// DRW Vx, Vy, nibble
	// Handle collisions
	void OP_Dxyn();

	// SKP Vx: skip next instruction if key with value Vx is pressed
	void OP_Ex9E();

	// SKNP Vx: skip next instruction if key with value Vx isn't pressed
	void OP_ExA1();

	// LD Vx, DT: set Vx = delay timer value
	void OP_Fx07();
	
	// LD Vx, K: by subtracting the PC by 2 this pauses because cycle adds 2,
	// essentially just staying on the same instruction until a key is pressed
	void OP_Fx0A();

	// LD DT, Vx: set daly timer = Vx
	void OP_Fx15();

	// LD ST, Vx: set sound timer = Vx
	void OP_Fx18();

	// Add I, Vx: Set I = I + Vx
	void OP_Fx1E();

	// LD F, Vx: Set I = location of sprite for digit Vx
	// Taking an offset from the start address because font characters are 5 bytes each
	void OP_Fx29();

	// LD B, Vx:
	// Store BCD representation of Vx in memory locations I, I+1, I+2
	void OP_Fx33();

	// LD [I], Vx: store registers V0 through Vx in memory starting at I
	void OP_Fx55();

	// LD Vx , [I]: read registers V0 through Vx from memory starting at I
	void OP_Fx65();

	uint8_t registers[16]{};
	uint8_t memory[4096]{};
	uint16_t index{};
	uint16_t pc{};
	uint16_t stack[16]{};
	uint8_t sp{};
	uint8_t delayTimer{};
	uint8_t soundTimer{};
	uint16_t opcode;

	typedef void (Chip8::*Chip8Func)();
	Chip8Func table[0xF + 1]{&Chip8::OP_NULL};
	Chip8Func table0[0xE + 1]{&Chip8::OP_NULL};
	Chip8Func table8[0xE + 1]{&Chip8::OP_NULL};
	Chip8Func tableE[0xE + 1]{&Chip8::OP_NULL};
	Chip8Func tableF[0x65 + 1]{&Chip8::OP_NULL};
};