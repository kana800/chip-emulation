#include <stdio.h>
#include <stdint.h>

#include "instructions.h"


int main(int argc, char* argv[])
{

	uint8_t opc;
	switch (opc)
	{
		// ADC - Add Memory to Accumulator with Carry
		case 0x69: // immediate      ADC #oper
		case 0x65: // zeropage       ADC oper
		case 0x75: // zeropage,X     ADC oper,X
		case 0x6D: // absolute       ADC oper
		case 0x7D: // absolute,X     ADC oper,X
		case 0x79: // absolute,Y     ADC oper,Y
		case 0x61: // (indirect,X)   ADC (oper,X)
		case 0x71: // (indirect),Y   ADC (oper),Y
			break;
		// AND - AND Memory with Accumulator
		case 0x29: // immediate      AND #oper
		case 0x25: // zeropage       AND oper
		case 0x35: // zeropage,X     AND oper,X
		case 0x2D: // absolute       AND oper
		case 0x3D: // absolute,X     AND oper,X
		case 0x39: // absolute,Y     AND oper,Y
		case 0x21: // (indirect,X)   AND (oper,X)
		case 0x31: // (indirect),Y   AND (oper),Y
			break;
		// ASL - Shift Left One Bit
		case 0x0A: // accumulator    ASL A
		case 0x06: // zeropage       ASL oper
		case 0x16: // zeropage,X     ASL oper,X
		case 0x0E: // absolute       ASL oper
		case 0x1E: // absolute,X     ASL oper,X
		break;

		// BCC - Branch on Carry Clear
		case 0x90: // relative       BCC oper
		break;

		// BCS - Branch on Carry Set
		case 0xB0: // relative       BCS oper
		break;

		// BEQ - Branch on Result Zero
		case 0xF0: // relative       BEQ oper
		break;

		// BIT - Test Bits in Memory with Accumulator
		case 0x24: // zeropage       BIT oper
		case 0x2C: // absolute       BIT oper
		break;

		// BMI - Branch on Result Minus
		case 0x30: // relative       BMI oper
		break;

		// BNE - Branch on Result not Zero
		case 0xD0: // relative       BNE oper
		break;

		// BPL - Branch on Result Plus
		case 0x10: // relative       BPL oper
		break;

		// BRK - Force Break
		case 0x00: // implied        BRK
		break;

		// BVC - Branch on Overflow Clear
		case 0x50: // relative       BVC oper
		break;

		// BVS - Branch on Overflow Set
		case 0x70: // relative       BVS oper
		break;

		// CLC - Clear Carry Flag
		case 0x18: // implied        CLC
		break;

		// CLD - Clear Decimal Mode
		case 0xD8: // implied        CLD
		break;

		// CLI - Clear Interrupt Disable Bit
		case 0x58: // implied        CLI
		break;

		// CLV - Clear Overflow Flag
		case 0xB8: // implied        CLV
		break;

		// CMP - Compare Memory with Accumulator
		case 0xC9: // immediate      CMP #oper
		case 0xC5: // zeropage       CMP oper
		case 0xD5: // zeropage,X     CMP oper,X
		case 0xCD: // absolute       CMP oper
		case 0xDD: // absolute,X     CMP oper,X
		case 0xD9: // absolute,Y     CMP oper,Y
		case 0xC1: // (indirect,X)   CMP (oper,X)
		case 0xD1: // (indirect),Y   CMP (oper),Y
		break;

		// CPX - Compare Memory and Index X
		case 0xE0: // immediate      CPX #oper
		case 0xE4: // zeropage       CPX oper
		case 0xEC: // absolute       CPX oper
		break;

		// CPY - Compare Memory and Index Y
		case 0xC0: // immediate      CPY #oper
		case 0xC4: // zeropage       CPY oper
		case 0xCC: // absolute       CPY oper
		break;

		// DEC - Decrement Memory by One
		case 0xC6: // zeropage       DEC oper
		case 0xD6: // zeropage,X     DEC oper,X
		case 0xCE: // absolute       DEC oper
		case 0xDE: // absolute,X     DEC oper,X
		break;

		// DEX - Decrement Index X by One
		case 0xCA: // implied        DEX
		break;

		// DEY - Decrement Index Y by One
		case 0x88: // implied        DEY
		break;

		// EOR - Exclusive-OR Memory with Accumulator
		case 0x49: // immediate      EOR #oper
		case 0x45: // zeropage       EOR oper
		case 0x55: // zeropage,X     EOR oper,X
		case 0x4D: // absolute       EOR oper
		case 0x5D: // absolute,X     EOR oper,X
		case 0x59: // absolute,Y     EOR oper,Y
		case 0x41: // (indirect,X)   EOR (oper,X)
		case 0x51: // (indirect),Y   EOR (oper),Y
		break;

		// INC - Increment Memory by One
		case 0xE6: // zeropage       INC oper
		case 0xF6: // zeropage,X     INC oper,X
		case 0xEE: // absolute       INC oper
		case 0xFE: // absolute,X     INC oper,X
		break;

		// INX - Increment Index X by One
		case 0xE8: // implied        INX
		break;

		// INY - Increment Index Y by One
		case 0xC8: // implied        INY
		break;

		// JMP - Jump to New Location
		case 0x4C: // absolute       JMP oper
		case 0x6C: // indirect       JMP (oper)
		break;

		// JSR - Jump to Subroutine
		case 0x20: // absolute       JSR oper
		break;

		// LDA - Load Accumulator with Memory
		case 0xA9: // immediate      LDA #oper
		case 0xA5: // zeropage       LDA oper
		case 0xB5: // zeropage,X     LDA oper,X
		case 0xAD: // absolute       LDA oper
		case 0xBD: // absolute,X     LDA oper,X
		case 0xB9: // absolute,Y     LDA oper,Y
		case 0xA1: // (indirect,X)   LDA (oper,X)
		case 0xB1: // (indirect),Y   LDA (oper),Y
		break;

		// LDX - Load Index X with Memory
		case 0xA2: // immediate      LDX #oper
		case 0xA6: // zeropage       LDX oper
		case 0xB6: // zeropage,Y     LDX oper,Y
		case 0xAE: // absolute       LDX oper
		case 0xBE: // absolute,Y     LDX oper,Y
		break;

		// LDY - Load Index Y with Memory
		case 0xA0: // immediate      LDY #oper
		case 0xA4: // zeropage       LDY oper
		case 0xB4: // zeropage,X     LDY oper,X
		case 0xAC: // absolute       LDY oper
		case 0xBC: // absolute,X     LDY oper,X
		break;

		// LSR - Logical Shift Right
		case 0x4A: // accumulator    LSR A
		case 0x46: // zeropage       LSR oper
		case 0x56: // zeropage,X     LSR oper,X
		case 0x4E: // absolute       LSR oper
		case 0x5E: // absolute,X     LSR oper,X
		break;

		// NOP - No Operation
		case 0xEA: // implied        NOP
		break;

		// ORA - OR Memory with Accumulator
		case 0x09: // immediate      ORA #oper
		case 0x05: // zeropage       ORA oper
		case 0x15: // zeropage,X     ORA oper,X
		case 0x0D: // absolute       ORA oper
		case 0x1D: // absolute,X     ORA oper,X
		case 0x19: // absolute,Y     ORA oper,Y
		case 0x01: // (indirect,X)   ORA (oper,X)
		case 0x11: // (indirect),Y   ORA (oper),Y
		break;

		// PHA - Push Accumulator
		case 0x48: // implied        PHA
		break;

		// PHP - Push Processor Status
		case 0x08: // implied        PHP
		break;

		// PLA - Pull Accumulator
		case 0x68: // implied        PLA
		break;

		// PLP - Pull Processor Status
		case 0x28: // implied        PLP
		break;

		// ROL - Rotate Left
		case 0x2A: // accumulator    ROL A
		case 0x26: // zeropage       ROL oper
		case 0x36: // zeropage,X     ROL oper,X
		case 0x2E: // absolute       ROL oper
		case 0x3E: // absolute,X     ROL oper,X
		break;

		// ROR - Rotate Right
		case 0x6A: // accumulator    ROR A
		case 0x66: // zeropage       ROR oper
		case 0x76: // zeropage,X     ROR oper,X
		case 0x6E: // absolute       ROR oper
		case 0x7E: // absolute,X     ROR oper,X
		break;

		// RTI - Return from Interrupt
		case 0x40: // implied        RTI
		break;

		// RTS - Return from Subroutine
		case 0x60: // implied        RTS
		break;

		// SBC - Subtract Memory from Accumulator with Borrow
		case 0xE9: // immediate      SBC #oper
		case 0xE5: // zeropage       SBC oper
		case 0xF5: // zeropage,X     SBC oper,X
		case 0xED: // absolute       SBC oper
		case 0xFD: // absolute,X     SBC oper,X
		case 0xF9: // absolute,Y     SBC oper,Y
		case 0xE1: // (indirect,X)   SBC (oper,X)
		case 0xF1: // (indirect),Y   SBC (oper),Y
		break;

		// SEC - Set Carry Flag
		case 0x38: // implied        SEC
		break;

		// SED - Set Decimal Flag
		case 0xF8: // implied        SED
		break;

		// SEI - Set Interrupt Disable
		case 0x78: // implied        SEI
		break;

		// STA - Store Accumulator in Memory
		case 0x85: // zeropage       STA oper
		case 0x95: // zeropage,X     STA oper,X
		case 0x8D: // absolute       STA oper
		case 0x9D: // absolute,X     STA oper,X
		case 0x99: // absolute,Y     STA oper,Y
		case 0x81: // (indirect,X)   STA (oper,X)
		case 0x91: // (indirect),Y   STA (oper),Y
		break;

		// STX - Store Index X in Memory
		case 0x86: // zeropage       STX oper
		case 0x96: // zeropage,Y     STX oper,Y
		case 0x8E: // absolute       STX oper
		break;

		// STY - Store Index Y in Memory
		case 0x84: // zeropage       STY oper
		case 0x94: // zeropage,X     STY oper,X
		case 0x8C: // absolute       STY oper
		break;

		// TAX - Transfer Accumulator to Index X
		case 0xAA: // implied        TAX
		break;

		// TAY - Transfer Accumulator to Index Y
		case 0xA8: // implied        TAY
		break;

		// TSX - Transfer Stack Pointer to Index X
		case 0xBA: // implied        TSX
		break;

		// TXA - Transfer Index X to Accumulator
		case 0x8A: // implied        TXA
		break;

		// TXS - Transfer Index X to Stack Register
		case 0x9A: // implied        TXS
		break;

		// TYA - Transfer Index Y to Accumulator
		case 0x98: // implied        TYA
		break;

		default:
		// Unknown or unsupported opcode
		break;
	}




	return 0;
}
