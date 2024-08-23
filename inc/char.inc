if !def(CHAR_INC)
def CHAR_INC equ 1

def PUTCHAR_ESCAPE_SEQUENCE_HANDLERB_ACTIVE equ 0

; PUTCHAR_ESCAPE_SEQUENCE_HANDLER_SET
;
; arguments:
; - 1 : escape sequence handler function ptr
;
; destroys `f`, `hl`
macro PUTCHAR_ESCAPE_SEQUENCE_HANDLER_SET
	static_assert fatal, !(_NARG < 1), "Too few arguments to PUTCHAR_ESCAPE_SEQUENCE_HANDLER_SET"
	static_assert fatal, !(_NARG > 1), "Too many arguments to PUTCHAR_ESCAPE_SEQUENCE_HANDLER_SET"

	ld hl, putchar_escape_sequence_handler_ptr
	ld [hl], low(\1)
	if isconst(putchar_escape_sequence_handler_ptr) \
	 &&(high(putchar_escape_sequence_handler_ptr) == high(putchar_escape_sequence_handler_ptr + 1))
		inc l
	else
		inc hl
	endc
	ld [hl], high(\1)

	ld hl, putchar_escape_sequence_handler_active
	set PUTCHAR_ESCAPE_SEQUENCE_HANDLERB_ACTIVE, [hl]
endm
; PUTCHAR_ESCAPE_SEQUENCE_HANDLER_SET

; PUTCHAR_ESCAPE_SEQUENCE_HANDLER_RES
;
; destroys `f`, `hl`
macro PUTCHAR_ESCAPE_SEQUENCE_HANDLER_RES
	static_assert fatal, !(_NARG > 0), "Too many arguments to PUTCHAR_ESCAPE_SEQUENCE_HANDLER_RES"

	ld hl, putchar_escape_sequence_handler_active
	res PUTCHAR_ESCAPE_SEQUENCE_HANDLERB_ACTIVE, [hl]
endm
; PUTCHAR_ESCAPE_SEQUENCE_HANDLER_RES

endc ; CHAR_INC