if !def(INPUT_INC)
def INPUT_INC equ 1

include "hardware.inc"

; these have to be in the format of: 2^n-1
def BUTTON_UP_FRAME_MASK 		equ (4 - 1)
def BUTTON_LEFT_FRAME_MASK 		equ (4 - 1)
def BUTTON_RIGHT_FRAME_MASK 	equ (4 - 1)
def BUTTON_DOWN_FRAME_MASK 		equ (4 - 1)

def BUTTON_SELECT_FRAME_MASK 	equ (8 - 1)
def BUTTON_START_FRAME_MASK 	equ (8 - 1)
def BUTTON_A_FRAME_MASK 		equ (8 - 1)
def BUTTON_B_FRAME_MASK 		equ (8 - 1)
;

;
def BUTTON_UP_FRAME_DELAY 		equ (4 - 1)
def BUTTON_LEFT_FRAME_DELAY 	equ (4 - 1)
def BUTTON_RIGHT_FRAME_DELAY 	equ (4 - 1)
def BUTTON_DOWN_FRAME_DELAY 	equ (4 - 1)

def BUTTON_SELECT_FRAME_DELAY 	equ (8 - 1)
def BUTTON_START_FRAME_DELAY 	equ (8 - 1)
def BUTTON_A_FRAME_DELAY 		equ (8 - 1)
def BUTTON_B_FRAME_DELAY 		equ (8 - 1)
;

;
def BUTTON_UP_SCREEN_LOCATION 		equ _SCRN0 + $05
def BUTTON_LEFT_SCREEN_LOCATION 	equ _SCRN0 + $24
def BUTTON_RIGHT_SCREEN_LOCATION 	equ _SCRN0 + $26
def BUTTON_DOWN_SCREEN_LOCATION 	equ _SCRN0 + $45

def BUTTON_SELECT_SCREEN_LOCATION 	equ _SCRN0 + $67
def BUTTON_START_SCREEN_LOCATION 	equ _SCRN0 + $69
def BUTTON_A_SCREEN_LOCATION 		equ _SCRN0 + $2c
def BUTTON_B_SCREEN_LOCATION 		equ _SCRN0 + $2a

def BUTTON_DOWN_UP_OFFSET			equ $a0
def BUTTON_REPEAT_OFFSET			equ $140
;


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