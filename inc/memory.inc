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
; - ... : string constants and/or bytes
; 
; destroys none
macro STRING
	static_assert fatal, !(_NARG < 2), "Too few arguments to STRING"

	pushs
	section "__inline_str_\1\@ section", ROMX
	\1:
		shift
		db \#, 0
	pops
endm
; STRING


; DW_STRING_PTR
; 
; arguments:
; - ... : string constants and/or bytes
; 
; destroys none
macro DW_STRING_PTR
	assert fatal, !(_NARG < 1), "Too few arguments to DW_STRING_PTR"

	STRING __inline_str_ptr\@, \#
	dw __inline_str_ptr\@
endm
; DW_STRING_PTR


; LD_STRING
; 
; arguments:
; - 1 : 16 bit register
; - ... : string constants and/or bytes
; 
; destroys none
macro LD_STRING
	static_assert fatal, !(_NARG < 2), "Too few arguments to LD_STRING"

	shift
	STRING __inline_str\@, \#
	shift -1
	ld \1, __inline_str\@
endm
; LD_STRING

endc ; MEMORY_INC