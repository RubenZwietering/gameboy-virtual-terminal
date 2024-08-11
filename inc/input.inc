if !def(INPUT_INC)
def INPUT_INC equ 1

def BUTTON_REPEAT_RATE 		equ 3 	; 20 repeats/s
def BUTTON_REPEAT_DELAY		equ 30 	; 0.5s delay


; INPUT_BUFFER_PUSH
; 
; arguments:
; - 1 : 8 bit register or value to push
; 
; destroys: `f`, `hl`
macro INPUT_BUFFER_PUSH
	static_assert fatal, !(_NARG < 1), "Too few arguments to INPUT_BUFFER_PUSH"
	static_assert fatal, !(_NARG > 1), "Too many arguments to INPUT_BUFFER_PUSH"

	ld hl, input_buffer_write_ptr
	inc [hl]
	ld l, [hl]
	ld h, HIGH(input_buffer)
	ld [hl], \1
endm
; INPUT_BUFFER_PUSH


; INPUT_BUFFER_POP
; 
; arguments:
; - 1 : 8 bit register to put the popped value in
;
; destroys: `f`, `hl`
macro INPUT_BUFFER_POP
	static_assert fatal, !(_NARG < 1), "Too few arguments to INPUT_BUFFER_POP"
	static_assert fatal, !(_NARG > 1), "Too many arguments to INPUT_BUFFER_POP"

	ld hl, input_buffer_read_ptr
	inc [hl]
	ld l, [hl]
	ld h, HIGH(input_buffer)
	ld \1, [hl]
endm
; INPUT_BUFFER_POP


; INPUT_BUFFER_UNREAD
; 
; returns:
; - `a` : size
;
; flags:
; - `z` : set if 0
;
; destroys: `af`, `b`
macro INPUT_BUFFER_UNREAD
	ld a, [input_buffer_read_ptr]
	ld b, a
	ld a, [input_buffer_write_ptr]
	sub b
endm
; INPUT_BUFFER_UNREAD

endc ; INPUT_INC