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
; there will be some overhead of moving data around on the stack if `hl` is passed before one of these constants
; 
; destroys `af`, `bc`, `de`, `hl`
macro PRINTF
	static_assert fatal, !(_NARG < 1), "Too few arguments to PRINTF"

	if strin("\1", "\"") != 0 		; check if string constant
		STRING __printf_fmt_str\@, \1
		def PRINTF_FMT_STR equs "__printf_fmt_str\@"
	else
		def PRINTF_FMT_STR equs "\1"
	endc

	def FIRST_AF_NARG = 0
	def FIRST_BC_NARG = 0
	def FIRST_DE_NARG = 0
	def FIRST_HL_NARG = 0

	for N, 2, _NARG + 1
		def N = _NARG - N + 2 ; reverse loop
		if strlen("\<{d:N}>") == 1
			if strcmp(strlwr("\<{d:N}>"), "a") == 0 || strcmp(strlwr("\<{d:N}>"), "f") == 0
				def FIRST_AF_NARG = _NARG - N
			elif strcmp(strlwr("\<{d:N}>"), "b") == 0 || strcmp(strlwr("\<{d:N}>"), "c") == 0
				def FIRST_BC_NARG = _NARG - N
			elif strcmp(strlwr("\<{d:N}>"), "d") == 0 || strcmp(strlwr("\<{d:N}>"), "e") == 0
				def FIRST_DE_NARG = _NARG - N
			elif strcmp(strlwr("\<{d:N}>"), "h") == 0 || strcmp(strlwr("\<{d:N}>"), "l") == 0
				def FIRST_HL_NARG = _NARG - N
			endc
		elif strlen("\<{d:N}>") == 2
			if strcmp(strlwr("\<{d:N}>"), "af") == 0
				def FIRST_AF_NARG = _NARG - N
			elif strcmp(strlwr("\<{d:N}>"), "bc") == 0
				def FIRST_BC_NARG = _NARG - N
			elif strcmp(strlwr("\<{d:N}>"), "de") == 0
				def FIRST_DE_NARG = _NARG - N
			elif strcmp(strlwr("\<{d:N}>"), "hl") == 0
				def FIRST_HL_NARG = _NARG - N
			endc
		endc
	endr

	def FORMAT_NARG equ _NARG - 1

	shift FORMAT_NARG

	rept FORMAT_NARG
		if strlen("\1") == 2 \
		 &&(strcmp(strlwr("\1"), "af") == 0 \
		 || strcmp(strlwr("\1"), "bc") == 0 \
		 || strcmp(strlwr("\1"), "de") == 0 \
		 || strcmp(strlwr("\1"), "hl") == 0)
			push \1
		else
			if !(FIRST_AF_NARG < _NARG)
				def SAVE_AF equ 1
			endc
			if !(FIRST_BC_NARG < _NARG)
				def SAVE_BC equ 1
			endc
			if !(FIRST_DE_NARG < _NARG)
				def SAVE_DE equ 1
			endc
			if !(FIRST_HL_NARG < _NARG)
				def SAVE_HL equ 1
			endc

			if strlen("\1") == 1 \
			 &&(strcmp(strlwr("\1"), "a") == 0 \
			 || strcmp(strlwr("\1"), "b") == 0 \
			 || strcmp(strlwr("\1"), "c") == 0 \
			 || strcmp(strlwr("\1"), "d") == 0 \
			 || strcmp(strlwr("\1"), "e") == 0 \
			 || strcmp(strlwr("\1"), "h") == 0 \
			 || strcmp(strlwr("\1"), "l") == 0)
				if !def(SAVE_HL)
					if strcmp(strlwr("\1"), "l") != 0
						ld l, \1
					endc
					ld h, 0
					push hl
				else
					dec sp ; room for hl
					dec sp
	
					push af
					push hl
 
					if strcmp(strlwr("\1"), "a") != 0
						ld a, \1
					endc
					ld hl, sp+4
					ld [hli], a
					ld [hl], 0
					
					pop hl
					pop af
				endc
			elif strlen("\1") == 1 && strcmp(strlwr("\1"), "f") == 0
				if !def(SAVE_HL)
					push af
					pop hl
					ld h, 0
					push hl
				else
					dec sp ; room for hl
					dec sp

					push af
					push hl

					ld hl, sp+2
					ld a, [hl]
					ld hl, sp+4
					ld [hli], a
					ld [hl], 0
					
					pop hl
					pop af
				endc
			elif strlen("\1") == 2 && strcmp(strlwr("\1"), "sp") == 0
				if !def(SAVE_HL)
					push af
					ld hl, sp + (_NARG * 2)
					pop af
					push hl
				else
					dec sp ; room for hl
					dec sp

					push af
					push hl
					push de

					ld hl, sp+6 + (_NARG * 2)
					LD_R16_R16 de, hl
	
					ld hl, sp+6
					ld a, e
					ld [hli], a
					ld [hl], d

					pop de
					pop hl
					pop af
				endc
			elif strlen("\1") == 2 && strcmp(strlwr("\1"), "pc") == 0
				if !def(SAVE_HL)
					push af
					call ld_hl_pc
					pop af
					push hl
				else
					dec sp ; room for hl
					dec sp

					push af
					push hl
					push de

					call ld_de_pc
					ld hl, sp+6
					ld a, e
					ld [hli], a
					ld [hl], d

					pop de
					pop hl
					pop af
				endc
			elif strlen("\1") == 2 && strcmp(strlwr("\1"), "zf") == 0 ; bit 7
				if !def(SAVE_HL)
					ld hl, 1
					jr z, .zero_flag_set\@
						ld l, 0
					.zero_flag_set\@
					push hl
				else
					dec sp ; room for hl
					dec sp

					push af
					push hl

					ld a, 1
					jr z, .zero_flag_set\@
						xor a
					.zero_flag_set\@
					ld hl, sp+4
					ld [hli], a
					ld [hl], 0

					pop hl
					pop af
				endc
			elif strlen("\1") == 2 && strcmp(strlwr("\1"), "nf") == 0 ; bit 6
				if !def(SAVE_HL)
					push af
					ld hl, sp+0
					ld a, [hl]
					rlca ; bit 7
					rlca ; bit 0
					and 1
					ld l, a
					ld h, 0
					pop af
					push hl
				else
					dec sp ; room for hl
					dec sp

					push af
					push hl

					ld hl, sp+2
					ld a, [hl]
					rlca ; bit 7
					rlca ; bit 0
					and 1
					ld hl, sp+4
					ld [hli], a
					ld [hl], 0

					pop hl
					pop af
				endc
			elif strlen("\1") == 2 && strcmp(strlwr("\1"), "hf") == 0 ; bit 5
				if !def(SAVE_HL)
					push af
					ld hl, sp+0
					ld a, [hl]
					rra ; bit 4
					swap a ; bit 0
					and 1
					ld l, a
					ld h, 0
					pop af
					push hl
				else
					dec sp ; room for hl
					dec sp

					push af
					push hl

					ld hl, sp+2
					ld a, [hl]
					rra ; bit 4
					swap a ; bit 0
					and 1
					ld hl, sp+4
					ld [hli], a
					ld [hl], 0

					pop hl
					pop af
				endc
			elif strlen("\1") == 2 && strcmp(strlwr("\1"), "cf") == 0 ; bit 4
				if !def(SAVE_HL)
					ld hl, 1
					jr c, .carry_flag_set\@
						ld l, 0
					.carry_flag_set\@
					push hl
				else
					dec sp ; room for hl
					dec sp

					push af
					push hl

					ld a, 0
					adc a
					ld hl, sp+4
					ld [hli], a
					ld [hl], 0

					pop hl
					pop af
				endc
			elif strin("\1", "\"") != 0 		; check if string constant
				if !def(SAVE_HL)
					LD_STRING hl, \1
					push hl
				else
					dec sp ; room for hl
					dec sp

					push af
					push hl

					ld hl, sp+4
					STRING __printf_arg_str\@, \1
					ld a, low(__printf_arg_str\@)
					ld [hli], a
					ld [hl], high(__printf_arg_str\@)

					pop hl
					pop af
				endc
			else
				if !def(SAVE_HL)
					ld hl, \1
					push hl
				else
					dec sp ; room for hl
					dec sp

					push af
					push hl

					ld hl, sp+4
					ld a, low(\1)
					ld [hli], a
					ld [hl], high(\1)

					pop hl
					pop af
				endc
			endc

			if def(SAVE_HL)
				purge SAVE_HL
			endc
			if def(SAVE_DE)
				purge SAVE_DE
			endc
			if def(SAVE_BC)
				purge SAVE_BC
			endc
			if def(SAVE_AF)
				purge SAVE_AF
			endc
		endc

		shift -1
	endr

	ld hl, sp+0
	LD_R16_R16 bc, hl

	ld hl, PRINTF_FMT_STR

	call vprintf
	add	sp, FORMAT_NARG * 2

	purge FORMAT_NARG
	purge FIRST_HL_NARG
	purge FIRST_DE_NARG
	purge FIRST_BC_NARG
	purge FIRST_AF_NARG
	purge PRINTF_FMT_STR
endm
; PRINTF

endc ; PRINT_INC