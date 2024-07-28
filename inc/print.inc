if !def(PRINT_INC)
def PRINT_INC equ 1

include "macros.inc"
include "memory.inc"

; PRINTF
; 
; arguments:
; - 1 : printf format string
; - ... : variable amount of registers, constants or strings to format
; 
; note:
; 8 bit registers, constants and strings are loaded into `hl` and then pushed onto the stack.
; if you still want to pass `hl`, it needs to be passed after the 8 bit registers, constands and strings
; 
; destroys `af`, `bc`, `de`, `hl`
macro PRINTF
	static_assert fatal, !(_NARG < 1), "Too few arguments to PRINTF"

	pushs

	section "__printf_fmt_str\@ section", ROMX
	__printf_fmt_str\@:
		db \1, 0

	pops

	def FORMAT_NARG equ _NARG - 1

	shift FORMAT_NARG

	def MODIFIED_HL = 0

	rept FORMAT_NARG
		if strlen("\1") == 2 \
		 &&(strcmp(strlwr("\1"), "af") == 0 \
		 || strcmp(strlwr("\1"), "bc") == 0 \
		 || strcmp(strlwr("\1"), "de") == 0 \
		 || strcmp(strlwr("\1"), "hl") == 0)
			if strcmp(strlwr("\1"), "hl") == 0
				static_assert fail, !(MODIFIED_HL == 1), "PRINTF: hl was modified"
			endc
			push \1
		else
			def MODIFIED_HL = 1
			if strlen("\1") == 1 \
			 &&(strcmp(strlwr("\1"), "a") == 0 \
			 ||	strcmp(strlwr("\1"), "b") == 0 \
			 || strcmp(strlwr("\1"), "c") == 0 \
			 || strcmp(strlwr("\1"), "d") == 0 \
			 || strcmp(strlwr("\1"), "e") == 0 \
			 || strcmp(strlwr("\1"), "h") == 0 \
			 || strcmp(strlwr("\1"), "l") == 0)
				ld h, 0
				ld l, \1
			elif strin("\1", "\"") != 0
				LD_STRING hl, \1
			else
				ld hl, \1
			endc
			push hl
		endc

		shift -1
	endr

	ld hl, sp+0
	LD_R16_R16 bc, hl

	ld hl, __printf_fmt_str\@

	call vprintf
	add	sp, FORMAT_NARG * 2

	purge MODIFIED_HL
	purge FORMAT_NARG
endm
; PRINTF

endc ; PRINT_INC