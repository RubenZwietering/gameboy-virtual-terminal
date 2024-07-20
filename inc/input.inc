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

endc ; INPUT_INC