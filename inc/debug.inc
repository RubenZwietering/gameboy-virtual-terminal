if !def(DEBUG_INC)
def DEBUG_INC equ 1

include "memory.inc"

; BREAKPOINT
; ---
;
; A software breakpoint (`ld b, b`) used by some emulators
;
; ---
; modifies: none
macro BREAKPOINT
	static_assert fatal, !(_NARG > 0), "Too many arguments to BREAKPOINT"
	if def(DEBUG)

	ld b, b

	endc
endm
; BREAKPOINT

; DPUTS
; ---
;
; A debug print (`ld d, d`) used by some emulators
;
; ---
; parameters:
; - 1 : string constant or address to a null terminated string
; - 2 : optional bank number for/of the string
; ---
; modifies: none
macro DPUTS
	static_assert fatal, !(_NARG < 1), "Too few arguments to DPUTS"
	static_assert fatal, !(_NARG > 2), "Too many arguments to DPUTS"
	if def(DEBUG)

	ld d, d
	jr .__debug_puts_jr_over_data\@
		dw $6464

		if strin("\1", "\"") != 0		; check if string constant
			if _NARG == 1
				dw $0000
				db \1, 0 				; string
			elif _NARG == 2
				dw $0001
				STRING __debug_puts_str\@, \1
				dw __debug_puts_str\@ 	; address to null terminated string
				dw \2 					; bank
			endc
		else
			dw $0001
			if _NARG == 1
				dw \1					; address to null terminated string
				dw bank(\1) 			; bank
			elif _NARG == 2
				dw \1					; address to null terminated string
				dw \2					; bank
			endc
		endc

	.__debug_puts_jr_over_data\@

	endc
endm
; DPUTS


; DPUTSJP
; ---
;
; A jump to DPUTS and back, in case space is tight.
;
; ---
; parameters: see DPUTS
; 
; ---
; modifies: none
macro DPUTSJP
	if def(DEBUG)

	pushs
	section "__debug_puts_jp\@ function", romx
		__debug_puts_jp_to_function\@:
		DPUTS \#
		jp :+ 							; return from function
	pops

	jp __debug_puts_jp_to_function\@ 	; no call so stack pointer stays intact
	:

	endc
endm
; DPUTSJP


; DPUTSCJP
; ---
;
; A conditional jump to DPUTS and back.
;
; ---
; parameters:
; - 1 : jump condition
; - ... : see DPUTS
; 
; ---
; modifies: none
macro DPUTSCJP
	static_assert fatal, !(_NARG < 1), "Too few arguments to DPUTSCJP"
	if def(DEBUG)

	shift

	pushs
	section "__debug_puts_cjp\@ function", romx
		__debug_puts_cjp_to_function\@:
		DPUTS \#
		jp :+ 							; return from function
	pops

	shift -1

	jp \1, __debug_puts_cjp_to_function\@ 	; no call so stack pointer stays intact
	:

	endc
endm
; DPUTSCJP


endc ; DEBUG_INC