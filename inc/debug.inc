if !def(DEBUG_INC)
def DEBUG_INC equ 1

macro DPUTS
	ld d, d
	jr .no_debug_puts\@
	dw $6464

	if _NARG == 1
			dw $0000
			db \1                   ; string
	elif _NARG == 2
			dw $0001
			dw \1                   ; address to null terminated string
			dw \2                   ; bank
	else
			static_assert 0, "Incorrect argument count!"
	endc

	.no_debug_puts\@
endm

endc ; DEBUG_INC