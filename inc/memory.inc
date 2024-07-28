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

pushs

section "__inline_str\@ section", ROMX
__inline_str\@:
	db \2, 0

pops

ld \1, __inline_str\@
endm
; LD_STRING

endc ; MEMORY_INC