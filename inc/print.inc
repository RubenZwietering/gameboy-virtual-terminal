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

	def FIRST_HL_NARG = 0

	for N, 2, _NARG + 1
		if strlen("\<{d:N}>") == 2 && strcmp(strlwr("\<{d:N}>"), "hl") == 0
			def FIRST_HL_NARG = _NARG - N
			break
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
			if FIRST_HL_NARG < _NARG
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
				elif strlen("\1") == 1  && strcmp(strlwr("\1"), "f") == 0
					push af
					pop hl
					ld h, 0
				elif strlen("\1") == 2 && strcmp(strlwr("\1"), "sp") == 0
					push af
					ld hl, sp + (_NARG * 2)
					pop af
				elif strlen("\1") == 2 && strcmp(strlwr("\1"), "pc") == 0
					ld hl, @
				elif strlen("\1") == 2 && strcmp(strlwr("\1"), "zf") == 0
					; bit 7
					ld hl, 1
					jr z, .zero_flag_set\@
						ld l, 0
					.zero_flag_set\@
				elif strlen("\1") == 2 && strcmp(strlwr("\1"), "nf") == 0
					; bit 6
					push af
					ld hl, sp+0
					ld a, [hl]
					rra ; bit 5
					rra ; bit 4
					swap a ; bit 1
					and 1
					ld h, 0
					ld l, a
					pop af
				elif strlen("\1") == 2 && strcmp(strlwr("\1"), "hf") == 0
					; bit 5
					push af
					ld hl, sp+0
					ld a, [hl]
					rra ; bit 4
					swap a ; bit 1
					and 1
					ld h, 0
					ld l, a
					pop af
				elif strlen("\1") == 2 && strcmp(strlwr("\1"), "cf") == 0
					; bit 4
					ld hl, 1
					jr c, .carry_flag_set\@
						ld l, 0
					.carry_flag_set\@
				elif strin("\1", "\"") != 0 		; check if string constant
					LD_STRING hl, \1
				else
					ld hl, \1
				endc
				push hl
			else
				dec sp ; room for hl
				dec sp

				push af
				push hl

				if strlen("\1") == 1 \
				 &&(strcmp(strlwr("\1"), "a") == 0 \
				 ||	strcmp(strlwr("\1"), "b") == 0 \
				 || strcmp(strlwr("\1"), "c") == 0 \
				 || strcmp(strlwr("\1"), "d") == 0 \
				 || strcmp(strlwr("\1"), "e") == 0 \
				 || strcmp(strlwr("\1"), "h") == 0 \
				 || strcmp(strlwr("\1"), "l") == 0)
					ld hl, sp+4
					ld [hl], \1
					inc hl
					xor a
					ld [hl], a
				elif strlen("\1") == 1  && strcmp(strlwr("\1"), "f") == 0
					push de
					push af
					ld hl, sp+9
					pop de

					xor a
					ld [hld], a
					ld [hl], e
					pop de
				elif strlen("\1") == 2 && strcmp(strlwr("\1"), "sp") == 0
					push de
					ld hl, sp+6 + (_NARG * 2)
					LD_R16_R16 de, hl
	
					ld hl, sp+6
					ld a, e
					ld [hli], a
					ld [hl], d
					pop de
				elif strlen("\1") == 2 && strcmp(strlwr("\1"), "pc") == 0
					ld hl, sp+4
					ld a, low(@-2)
					ld [hli], a
					ld a, high(@-5)
					ld [hl], a
				elif strlen("\1") == 2 && strcmp(strlwr("\1"), "zf") == 0
					; bit 7
					ld a, 1
					jr z, .zero_flag_set\@
						xor a
					.zero_flag_set\@
					ld hl, sp+4
					ld [hli], a
					xor a
					ld [hl], a
				elif strlen("\1") == 2 && strcmp(strlwr("\1"), "nf") == 0
					; bit 6
					ld hl, sp+2
					ld a, [hl]
					rra ; bit 5
					rra ; bit 4
					swap a ; bit 1
					and 1
					ld hl, sp+4
					ld [hli], a
					xor a
					ld [hl], a
				elif strlen("\1") == 2 && strcmp(strlwr("\1"), "hf") == 0
					; bit 5
					ld hl, sp+2
					ld a, [hl]
					rra ; bit 4
					swap a ; bit 1
					and 1
					ld hl, sp+4
					ld [hli], a
					xor a
					ld [hl], a
				elif strlen("\1") == 2 && strcmp(strlwr("\1"), "cf") == 0
					; bit 4
					ld a, 0
					adc a
					ld hl, sp+6
					ld [hli], a
					xor a
					ld [hl], a
				elif strin("\1", "\"") != 0 		; check if string constant
					ld hl, sp+4
					STRING __printf_arg_str\@, \1
					ld a, low(__printf_arg_str\@)
					ld [hli], a
					ld a, high(__printf_arg_str\@)
					ld [hl], a
				else
					ld hl, sp+4
					ld a, low(\1)
					ld [hli], a
					ld a, high(\1)
					ld [hl], a
				endc
					
				pop hl
				pop af
			endc
		endc

		shift -1
	endr

	ld hl, sp+0
	LD_R16_R16 bc, hl

	ld hl, PRINTF_FMT_STR

	call vprintf
	add	sp, FORMAT_NARG * 2

	purge PRINTF_FMT_STR
	purge FIRST_HL_NARG
	purge FORMAT_NARG
endm
; PRINTF

endc ; PRINT_INC