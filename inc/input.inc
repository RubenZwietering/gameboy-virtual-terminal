if !def(INPUT_INC)
def INPUT_INC equ 1

def BUTTON_REPEAT_RATE 		equ 3 	; 20 repeats/s
def BUTTON_REPEAT_DELAY		equ 30 	; 0.5s delay


; INPUT_BUFFER_PUSH
; 
; input:
; - `a` : value
; 
; destroys: `f`, `hl`
macro INPUT_BUFFER_PUSH
	ld hl, input_buffer_write_ptr
	inc [hl]
	ld l, [hl]
	ld h, HIGH(input_buffer)
	ld [hl], a
endm
; INPUT_BUFFER_PUSH


; INPUT_BUFFER_POP
; 
; returns:
; - `a` : value
;
; destroys: `af`, `hl`
macro INPUT_BUFFER_POP
	ld hl, input_buffer_read_ptr
	inc [hl]
	ld l, [hl]
	ld h, HIGH(input_buffer)
	ld a, [hl]
endm
; INPUT_BUFFER_POP


; INPUT_BUFFER_UNREAD
; 
; returns:
; - `a` : value
; - `Z` : set if 0
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