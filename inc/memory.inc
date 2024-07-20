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

endc ; MEMORY_INC