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
; Checks if a 16 bit register is not passed and loads 8 bit registers,
; constants and strings into that register and then pushes it onto the stack.
; If all registers are passed as arguments, the macro will use the stack cleverly
; to save the registers. There will be some overhead of moving data around on the 
; stack if there are no free registers to use.
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
		 || strcmp(strlwr("\1"), "hl") == 0) 							; 16 bit register
			push \1
		else
			def SAVE_AF equ !(FIRST_AF_NARG < _NARG)
			def SAVE_BC equ !(FIRST_BC_NARG < _NARG)
			def SAVE_DE equ !(FIRST_DE_NARG < _NARG)
			def SAVE_HL equ !(FIRST_HL_NARG < _NARG)

			if !SAVE_HL
				def FREE_REGISTER equs "hl"
			elif !SAVE_DE
				def FREE_REGISTER equs "de"
			elif !SAVE_BC
				def FREE_REGISTER equs "bc"
			endc
			
			if strlen("\1") == 1 \
			 &&(strcmp(strlwr("\1"), "a") == 0 \
			 || strcmp(strlwr("\1"), "b") == 0 \
			 || strcmp(strlwr("\1"), "c") == 0 \
			 || strcmp(strlwr("\1"), "d") == 0 \
			 || strcmp(strlwr("\1"), "e") == 0 \
			 || strcmp(strlwr("\1"), "h") == 0 \
			 || strcmp(strlwr("\1"), "l") == 0) 						; 8 bit register
				if def(FREE_REGISTER)
					if strin("{FREE_REGISTER}", strlwr("\1")) != 2
						ld low(FREE_REGISTER), \1
					endc
					ld high(FREE_REGISTER), 0
					push FREE_REGISTER
				else
					push hl ; dec sp ; room for 8 bit register
							; dec sp
	
					if SAVE_AF
						push af
					endc
					push hl
 
					if strcmp(strlwr("\1"), "a") != 0
						ld a, \1
					endc
					if SAVE_AF
						ld hl, sp+4
					else
						ld hl, sp+2
					endc
					ld [hli], a
					ld [hl], 0
					
					pop hl
					if SAVE_AF
						pop af
					endc
				endc
			elif strlen("\1") == 1 && strcmp(strlwr("\1"), "f") == 0 	; flags register
				if def(FREE_REGISTER)
					push af
					pop FREE_REGISTER
					ld high(FREE_REGISTER), 0
					push FREE_REGISTER
				else
					push hl ; dec sp ; room for flags register
							; dec sp

					push af
					push hl

					ld hl, sp+2
					ld a, [hli]
					inc hl ; ld hl, sp+4
					ld [hli], a
					ld [hl], 0
					
					pop hl
					pop af
				endc
			elif strlen("\1") == 2 && strcmp(strlwr("\1"), "sp") == 0 	; stack pointer
				if !SAVE_HL
					if SAVE_AF
						push af
						ld hl, sp + (_NARG * 2)
						pop af
					else
						ld hl, sp + ((_NARG - 1) * 2)
					endc
					push hl
				elif def(FREE_REGISTER)
					push hl ; dec sp ; room for stack pointer
							; dec sp

					if SAVE_AF
						push af
					endc
					push hl

					if SAVE_AF
						ld hl, sp+4 + (_NARG * 2)
					else
						ld hl, sp+2 + (_NARG * 2)
					endc
					LD_R16_R16 FREE_REGISTER, hl
	
					if SAVE_AF
						ld hl, sp+4
					else
						ld hl, sp+2
					endc
					ld a, low(FREE_REGISTER)
					ld [hli], a
					ld [hl], high(FREE_REGISTER)

					pop hl
					if SAVE_AF
						pop af
					endc
				else
					push hl ; dec sp ; room for stack pointer
							; dec sp

					if SAVE_AF
						push af
					endc
					push hl
					push de

					if SAVE_AF
						ld hl, sp+6 + (_NARG * 2)
					else
						ld hl, sp+4 + (_NARG * 2)
					endc
					LD_R16_R16 de, hl
	
					if SAVE_AF
						ld hl, sp+6
					else
						ld hl, sp+4
					endc
					ld a, e
					ld [hli], a
					ld [hl], d

					pop de
					pop hl
					if SAVE_AF
						pop af
					endc
				endc
			elif strlen("\1") == 2 && strcmp(strlwr("\1"), "pc") == 0 	; program counter
				if !SAVE_HL
					if SAVE_AF
						push af
					endc
					call ld_hl_pc
					if SAVE_AF
						pop af
					endc
					push hl
				else
					push hl ; dec sp ; room for program counter
							; dec sp

					if SAVE_AF
						push af
					endc
					push hl
					push de

					call ld_de_pc
					if SAVE_AF
						ld hl, sp+6
					else
						ld hl, sp+4
					endc
					ld a, e
					ld [hli], a
					ld [hl], d

					pop de
					pop hl
					if SAVE_AF
						pop af
					endc
				endc
			elif strlen("\1") == 2 && strcmp(strlwr("\1"), "zf") == 0 	; zero flag, bit 7
				if def(FREE_REGISTER)
					ld FREE_REGISTER, 1
					jr z, .zero_flag_set\@
						ld low(FREE_REGISTER), 0
					.zero_flag_set\@
					push FREE_REGISTER
				else
					push hl ; dec sp ; room for zero flag
							; dec sp

					if SAVE_AF
						push af
					endc
					push hl

					ld a, 1
					jr z, .zero_flag_set\@
						xor a
					.zero_flag_set\@
					if SAVE_AF
						ld hl, sp+4
					else
						ld hl, sp+2
					endc
					ld [hli], a
					ld [hl], 0

					pop hl
					if SAVE_AF
						pop af
					endc
				endc
			elif strlen("\1") == 2 && strcmp(strlwr("\1"), "nf") == 0 	; negative flag, bit 6
				if def(FREE_REGISTER)
					push af
					if !SAVE_HL
						ld hl, sp+0
						ld a, [hl]
					else
						pop FREE_REGISTER
						if SAVE_AF
							push af
						endc
						ld a, low(FREE_REGISTER)
					endc

					rlca ; bit 7
					rlca ; bit 0
					and 1
					ld low(FREE_REGISTER), a
					ld high(FREE_REGISTER), 0

					if !SAVE_HL || SAVE_AF
						pop af
					endc
					push FREE_REGISTER
				else
					push hl ; dec sp ; room for negative flag
							; dec sp

					push af
					push hl

					ld hl, sp+2
					ld a, [hli]
					rlca ; bit 7
					rlca ; bit 0
					and 1
					inc hl ; ld hl, sp+4
					ld [hli], a
					ld [hl], 0

					pop hl
					pop af
				endc
			elif strlen("\1") == 2 && strcmp(strlwr("\1"), "hf") == 0 	; half carry flag, bit 5
				if def(FREE_REGISTER)
					push af
					if !SAVE_HL
						ld hl, sp+0
						ld a, [hl]
					else
						pop FREE_REGISTER
						if SAVE_AF
							push af
						endc
						ld a, low(FREE_REGISTER)
					endc

					rra ; bit 4
					swap a ; bit 0
					and 1
					ld low(FREE_REGISTER), a
					ld high(FREE_REGISTER), 0

					if !SAVE_HL || SAVE_AF
						pop af
					endc
					push FREE_REGISTER
				else
					push hl ; dec sp ; room for half carry flag
							; dec sp

					push af
					push hl

					ld hl, sp+2
					ld a, [hli]
					rra ; bit 4
					swap a ; bit 0
					and 1
					inc hl ; ld hl, sp+4
					ld [hli], a
					ld [hl], 0

					pop hl
					pop af
				endc
			elif strlen("\1") == 2 && strcmp(strlwr("\1"), "cf") == 0 	; carry flag, bit 4
				if def(FREE_REGISTER)
					ld FREE_REGISTER, 1
					jr c, .zero_flag_set\@
						ld low(FREE_REGISTER), 0
					.zero_flag_set\@
					push FREE_REGISTER
				else
					push hl ; dec sp ; room for carry flag
							; dec sp

					if SAVE_AF
						push af
					endc
					push hl

					ld a, 0
					adc a
					if SAVE_AF
						ld hl, sp+4
					else
						ld hl, sp+2
					endc
					ld [hli], a
					ld [hl], 0

					pop hl
					if SAVE_AF
						pop af
					endc
				endc
			elif strin("\1", "\"") != 0 								; string constant
				if def(FREE_REGISTER)
					LD_STRING FREE_REGISTER, \1
					push FREE_REGISTER
				else
					push hl ; dec sp ; room for string
							; dec sp

					if SAVE_AF
						push af
					endc
					push hl

					if SAVE_AF
						ld hl, sp+4
					else
						ld hl, sp+2
					endc
					STRING __printf_arg_str\@, \1
					ld a, low(__printf_arg_str\@)
					ld [hli], a
					ld [hl], high(__printf_arg_str\@)

					pop hl
					if SAVE_AF
						pop af
					endc
				endc
			else 														; constant
				if def(FREE_REGISTER)
					ld FREE_REGISTER, \1
					push FREE_REGISTER
				else
					push hl ; dec sp ; room for constant
							; dec sp

					if SAVE_AF
						push af
					endc
					push hl

					if SAVE_AF
						ld hl, sp+4
					else
						ld hl, sp+2
					endc
					ld a, low(\1)
					ld [hli], a
					ld [hl], high(\1)

					pop hl
					if SAVE_AF
						pop af
					endc
				endc
			endc

			if def(FREE_REGISTER)
				purge FREE_REGISTER
			endc
			purge SAVE_HL
			purge SAVE_DE
			purge SAVE_BC
			purge SAVE_AF
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