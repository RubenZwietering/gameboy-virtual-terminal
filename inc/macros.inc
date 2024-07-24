if !def(MACROS_INC)
def MACROS_INC equ 1

macro DO_IF_CARRY
	jr nc, .jump\@
		\1
	.jump\@
endm

macro JUMP_IF_ZERO
	jr z, .jump\@
		\1
	.jump\@
endm

; PUSH_ALL : pushes all registers
macro PUSH_ALL
	push af
	push bc
	push de
	push hl
endm
; PUSH_ALL

; POP_ALL : pops all registers
macro POP_ALL
	pop hl
	pop de
	pop bc
	pop af
endm
; POP_ALL


; SWAP_R16
; 
; description:
; swap low and high bytes of R
;
; arguments:
; - R : 16 bit register to swap
; 
; destroys `a`
macro SWAP_R16
	ld a, LOW(\1)
	ld LOW(\1), HIGH(\1)
	ld HIGH(\1), a
endm
; SWAP_R16


; LD_R16_R16
; 
; description:
; load R2 into R1
;
; arguments:
; - R1 : 16 bit to be loaded by R2
; - R2 : 16 bit to load into R1
; 
; destroys none
macro LD_R16_R16
	ld HIGH(\1), HIGH(\2)
	ld Low(\1), LOW(\2)
endm
; LD_R16_R16


; COMPLEMENT : flips all bits in register R
;
; arguments
; - R : register to flip bits
;
; output
; - bits in register R will be flipped
;
; destroys af
macro COMPLEMENT
	ld a, \1
	cpl 
	ld \1, a
endm
; COMPLEMENT


; TWOS_COMPLEMENT_R8
; 
; description:
; performs two's complement on R
; 
; arguments:
; - R : 8 bit register
; 
; destroys `af`
macro TWOS_COMPLEMENT_R8
	ld a, \1
	cpl
	inc a
	ld \1, a
endm
; TWOS_COMPLEMENT_R8


; TWOS_COMPLEMENT_R16
; 
; description:
; performs two's complement on R
; 
; arguments:
; - R : 16 bit register
; 
; destroys `af`
macro TWOS_COMPLEMENT_R16
	ld a, LOW(\1)
	cpl
	add 1
	ld LOW(\1), a
	ld a, HIGH(\1)
	cpl
	adc 0
	ld HIGH(\1), a
endm
; TWOS_COMPLEMENT_R16


; CP_N_R: compare value N to register(s) R
	;
	; arguments
	; - N : value
	; - R : register(s)
	;
	; output
	; - zero flag set if N == R
	;
	; destroys af
	; CP_N_R : macro
	; 	if (STRLEN(\2)) > 0
	; 		CP_B_N_R STRLEN(\2), \1, \2
	; 	endc
	; endm
; CP_N_R

;CP_N_R 32, de

; CP_B_N_R: compare value N to register(s) R
	;
	; arguments
	; - N : value
	; - R : register(s)
	;
	; output
	; - zero flag set if N == R
	;
	; destroys af
	; CP_B_N_R : macro
	; 	ld a, STRSUB(\3, 1, 1)
	; 	cp (\2) >> ((\1) * 8)
	; 	if ((\1) - 1) > 0
	; 		jr nz, .not_equal\@
	; 			CP_B_N_R ((\1) - 1), \2, STRSUB(\3, 2, STRLEN(\3) - 1)
	; 		.not_equal\@
	; 	endc
	; endm
; CP_B_N_R

; CP_N8_R : compare 8 bit value N to register R
;
; arguments
; - N : 8 bit value
; - R : 8 bit register
;
; output
; - zero flag set if N == R
;
; destroys af
macro CP_N8_R
	ld a, \2
	cp (\1)
endm
; CP_N8_R

; CP_N16_R : compare 16 bit value N to registers R
;
; arguments
; - N : 16 bit value
; - R : 16 bit register
;
; output
; - zero flag set if N == R
;
; destroys af
macro CP_N16_R
	CP_N8_R HIGH(\1), HIGH(\2)
	jr nz, .not_equal\@
		CP_N8_R LOW(\1), LOW(\2)
	.not_equal\@
endm
; CP_N16_R

; CP_N32_R : compare 32 bit value N to registers R
;
; arguments
; - N : 32 bit value
; - R : 2 comma separated 16 bit registers
;
; output
; - zero flag set if N == R
;
; destroys af
macro CP_N32_R
	CP_N16_R (((\1) >> 16) & $ffff), \2
	jr nz, .not_equal\@
		CP_N16_R ((\1) & $ffff), \3
	.not_equal\@
endm
; CP_N32_R

endc ; MACROS_INC