if !def(DEBUG_INC)
def DEBUG_INC equ 1

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
; - 1 : string to print or address to a null terminated string if there is 2 arguments
; - 2 : bank number of the string pointer in the first argument
; ---
; modifies: none
macro DPUTS
	static_assert fatal, !(_NARG < 1), "Too few arguments to DPUTS"
	static_assert fatal, !(_NARG > 2), "Too many arguments to DPUTS"
	if def(DEBUG)

	ld d, d
	jr .no_debug_puts\@
		dw $6464

		if _NARG == 1
			dw $0000
			db \1, 0			; string
		elif _NARG == 2
			dw $0001
			dw \1					; address to null terminated string
			dw \2					; bank
		endc
	.no_debug_puts\@

	endc
endm
; DPUTS

endc ; DEBUG_INC