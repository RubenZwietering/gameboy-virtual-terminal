if !def(MEMORY_INC)
def MEMORY_INC equ 1

include "hardware.inc"

; WAIT_BLANK : loops until VRAM is available
;
; destroys `af`
macro WAIT_BLANK
	.loop\@
		ld a, [rSTAT]
		and STATF_BUSY
	jr nz, .loop\@
endm
; WAIT_BLANK


; STRING
; 
; arguments:
; - 1 : string name
; - 2 : string constant
; 
; destroys `af`, `bc`, `de`, `hl`
macro STRING
	static_assert fatal, !(_NARG < 2), "Too few arguments to STRING"
	static_assert fatal, !(_NARG > 2), "Too many arguments to STRING"

	pushs
	section "__inline_str_\1\@ section", ROMX
	\1:
		db \2, 0
	pops
endm
; STRING


; LD_STRING
; 
; arguments:
; - 1 : 16 bit register
; - 2 : string constant
; 
; destroys `af`, `bc`, `de`, `hl`
macro LD_STRING
	static_assert fatal, !(_NARG < 2), "Too few arguments to LD_STRING"
	static_assert fatal, !(_NARG > 2), "Too many arguments to LD_STRING"

	STRING __inline_str\@, \2
	ld \1, __inline_str\@
endm
; LD_STRING

endc ; MEMORY_INC