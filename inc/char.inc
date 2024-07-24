if !def(CHAR_INC)
def CHAR_INC equ 1

def CURSOR_BLINK_RATE equ 36 ; 20 for c64 cursor blink time
def CURSOR_OBJECT_PTR equ _OAMRAM
def CURSOR_CHARACTER equ $03

; PRINTF
; 
; arguments:
; - 1 : printf format string
; - ... : variable amount of 16 bit registers to format
; 
; destroys `af`, `bc`, `de`, `hl`
macro PRINTF
	static_assert fatal, !(_NARG < 1), "Too few arguments to PRINTF"

	pushs

	section "__printf_fmt_str\@ section", ROMX
	__printf_fmt_str\@:
		db \1, 0

	pops

	def FORMAT_NARG equ _NARG

	shift FORMAT_NARG - 1

	rept FORMAT_NARG - 1
		push \1

		shift -1
	endr

	ld de, __printf_fmt_str\@
	push de

	call _printf
	add	sp, FORMAT_NARG * 2

	purge FORMAT_NARG
endm
; PRINTF

endc ; CHAR_INC