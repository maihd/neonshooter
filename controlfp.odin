package neonshooter

import "core:fmt"
import syswin "core:sys/windows"
// import x86 "core:intrinsics/x86"

// 1. Bind the MSVCRT library using the default "cdecl" calling convention
foreign import msvcrt "system:msvcrt.lib" // Or "system:msvcrt" depending on your configuration

// Constants gathered from Windows <float.h>
MCW_EM :: 0x0008001F   // Interrupt Exception Mask
EM_INVALID :: 0x00000010

_PC_24 :: 0x00020000   // 24-bit precision control (x87 specific)
_MCW_PC :: 0x00030000  // Precision Mask

foreign msvcrt {
	// errno_t _controlfp_s(unsigned int *currentControl, unsigned int newControl, unsigned int mask);
	_controlfp_s :: proc "c" (currentControl: ^u32, newControl: u32, mask: u32) -> i32 ---
}

// Custom local instruction wrapper passing pointers explicitly
unsafe_set_mxcsr :: asm(p: ^u32) [ #volatile, #clobber memory ] {
	// 0x0F, 0xAE, 0x11 = ldmxcsr [rcx] (Where RCX is the first param pointer 'p')
	#byte 0x0F, 0xAE, 0x11
}

unsafe_get_mxcsr :: asm(p: ^u32) [ #volatile, #clobber memory ] {
	// 0x0F, 0xAE, 0x19 = stmxcsr [rcx]
	#byte 0x0F, 0xAE, 0x19
}

// Toggles Flush-to-Zero (Bit 15) and Denormals-are-Zero (Bit 6) via MXCSR
force_float_determinism_amd64 :: proc() {
	// when ODIN_ARCH == .amd64 {
	// 	mxcsr_state: u32 = 0

	// 	// 1. Safely store active config state into our clear variable
	// 	unsafe_get_mxcsr(&mxcsr_state)

	// 	// 2. Modify state bits (FTZ and DAZ)
	// 	mxcsr_state |= 0x8040

	// 	// 3. Apply the updated config word safely back down to CPU registers
	// 	unsafe_set_mxcsr(&mxcsr_state)
	// }

	// when ODIN_ARCH == .amd64 {
	// 	// 1. Read the current control register state safely using the native intrinsic
	// 	current_mxcsr := x86._mm_getcsr()

	// 	// 2. Tweak the target bits:
	// 	// Bit 15 (0x8000) = Flush-to-Zero (FTZ)
	// 	// Bit 6  (0x0040) = Denormals-are-Zero (DAZ)
	// 	deterministic_mxcsr := current_mxcsr | 0x8040

	// 	// 3. Write it back to the CPU hardware state safely
	// 	x86._mm_setcsr(deterministic_mxcsr)
	// }

	// when ODIN_OS == .Windows {
	// 	// Bit 15 (0x8000) = Flush-to-Zero (FTZ)
	// 	// Bit 6  (0x0040) = Denormals-are-Zero (DAZ)
	// 	// 0x1F80 is the default Windows standard state (Masking all common exceptions)
	// 	DETERMINISTIC_MASK :: u32(0x1F80 | 0x8000 | 0x0040)

	// 	// Safely update the architecture control word using the official Windows API wrapper
	// 	syswin.Set8087CW(DETERMINISTIC_MASK)
	// }

	
}
