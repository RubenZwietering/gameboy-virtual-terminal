if !def(KEYBOARD_INC)
def KEYBOARD_INC equ 1

def KEYBOARD_INPUT_BUFFER_SIZE equ 3 * 2 ; 3 scancodes

; KEYBOARD_INPUT_BUFFER_PUSH
; 
; input:
; - `a` : value
; 
; destroys: `f`, `hl`
macro KEYBOARD_INPUT_BUFFER_PUSH
	ld hl, keyboard_input_buffer_write_ptr
	inc [hl]
	jr nz, .not_end_write_ptr\@
		ld [hl], low(keyboard_input_buffer)
	.not_end_write_ptr\@
	ld l, [hl]
	ld h, high(keyboard_input_buffer)
	ld [hl], a
endm
; KEYBOARD_INPUT_BUFFER_PUSH


; KEYBOARD_INPUT_BUFFER_POP
; 
; returns:
; - `a` : value
;
; destroys: `af`, `hl`
macro KEYBOARD_INPUT_BUFFER_POP
	ld hl, keyboard_input_buffer_read_ptr
	inc [hl]
	jr nz, .not_end_read_ptr\@
		ld [hl], low(keyboard_input_buffer)
	.not_end_read_ptr\@
	ld l, [hl]
	ld h, high(keyboard_input_buffer)
	ld a, [hl]
endm
; KEYBOARD_INPUT_BUFFER_POP


; KEYBOARD_INPUT_BUFFER_UNREAD
;
; returns:
; - `a` : size
;
; flags:
; - `z` : set if 0
;
; destroys: `af`, `b`
macro KEYBOARD_INPUT_BUFFER_UNREAD
	ld a, [keyboard_input_buffer_read_ptr]
	ld b, a
	ld a, [keyboard_input_buffer_write_ptr]
	sub b
	jr nc, .no_wrap\@
		add KEYBOARD_INPUT_BUFFER_SIZE
	.no_wrap\@
endm
; KEYBOARD_INPUT_BUFFER_UNREAD

def KEYBOARD_STATEB_KEY_DOWN 	equ 0
def KEYBOARD_STATEB_EXTENDED 	equ 1
def KEYBOARD_STATEB_EXTENDED1 	equ 2

def KEYBOARD_STATEF_KEY_DOWN 	equ (1 << KEYBOARD_STATEB_KEY_DOWN)
def KEYBOARD_STATEF_EXTENDED 	equ (1 << KEYBOARD_STATEB_EXTENDED)

; https://github.com/torvalds/linux/blob/master/include/uapi/linux/input-event-codes.h

def KEY_RESERVED          equ 0
def KEY_ESC               equ 1
def KEY_1                 equ 2
def KEY_2                 equ 3
def KEY_3                 equ 4
def KEY_4                 equ 5
def KEY_5                 equ 6
def KEY_6                 equ 7
def KEY_7                 equ 8
def KEY_8                 equ 9
def KEY_9                 equ 10
def KEY_0                 equ 11
def KEY_MINUS             equ 12
def KEY_EQUAL             equ 13
def KEY_BACKSPACE         equ 14
def KEY_TAB               equ 15
def KEY_Q                 equ 16
def KEY_W                 equ 17
def KEY_E                 equ 18
def KEY_R                 equ 19
def KEY_T                 equ 20
def KEY_Y                 equ 21
def KEY_U                 equ 22
def KEY_I                 equ 23
def KEY_O                 equ 24
def KEY_P                 equ 25
def KEY_LEFTBRACE         equ 26
def KEY_RIGHTBRACE        equ 27
def KEY_ENTER             equ 28
def KEY_LEFTCTRL          equ 29
def KEY_A                 equ 30
def KEY_S                 equ 31
def KEY_D                 equ 32
def KEY_F                 equ 33
def KEY_G                 equ 34
def KEY_H                 equ 35
def KEY_J                 equ 36
def KEY_K                 equ 37
def KEY_L                 equ 38
def KEY_SEMICOLON         equ 39
def KEY_APOSTROPHE        equ 40
def KEY_GRAVE             equ 41
def KEY_LEFTSHIFT         equ 42
def KEY_BACKSLASH         equ 43
def KEY_Z                 equ 44
def KEY_X                 equ 45
def KEY_C                 equ 46
def KEY_V                 equ 47
def KEY_B                 equ 48
def KEY_N                 equ 49
def KEY_M                 equ 50
def KEY_COMMA             equ 51
def KEY_DOT               equ 52
def KEY_SLASH             equ 53
def KEY_RIGHTSHIFT        equ 54
def KEY_KPASTERISK        equ 55
def KEY_LEFTALT           equ 56
def KEY_SPACE             equ 57
def KEY_CAPSLOCK          equ 58
def KEY_F1                equ 59
def KEY_F2                equ 60
def KEY_F3                equ 61
def KEY_F4                equ 62
def KEY_F5                equ 63
def KEY_F6                equ 64
def KEY_F7                equ 65
def KEY_F8                equ 66
def KEY_F9                equ 67
def KEY_F10               equ 68
def KEY_NUMLOCK           equ 69
def KEY_SCROLLLOCK        equ 70
def KEY_KP7               equ 71
def KEY_KP8               equ 72
def KEY_KP9               equ 73
def KEY_KPMINUS           equ 74
def KEY_KP4               equ 75
def KEY_KP5               equ 76
def KEY_KP6               equ 77
def KEY_KPPLUS            equ 78
def KEY_KP1               equ 79
def KEY_KP2               equ 80
def KEY_KP3               equ 81
def KEY_KP0               equ 82
def KEY_KPDOT             equ 83

def KEY_ZENKAKUHANKAKU    equ 85
def KEY_102ND             equ 86
def KEY_F11               equ 87
def KEY_F12               equ 88
def KEY_RO                equ 89
def KEY_KATAKANA          equ 90
def KEY_HIRAGANA          equ 91
def KEY_HENKAN            equ 92
def KEY_KATAKANAHIRAGANA  equ 93
def KEY_MUHENKAN          equ 94
def KEY_KPJPCOMMA         equ 95
def KEY_KPENTER           equ 96
def KEY_RIGHTCTRL         equ 97
def KEY_KPSLASH           equ 98
def KEY_SYSRQ             equ 99
def KEY_RIGHTALT          equ 100
def KEY_LINEFEED          equ 101
def KEY_HOME              equ 102
def KEY_UP                equ 103
def KEY_PAGEUP            equ 104
def KEY_LEFT              equ 105
def KEY_RIGHT             equ 106
def KEY_END               equ 107
def KEY_DOWN              equ 108
def KEY_PAGEDOWN          equ 109
def KEY_INSERT            equ 110
def KEY_DELETE            equ 111
def KEY_MACRO             equ 112
def KEY_MUTE              equ 113
def KEY_VOLUMEDOWN        equ 114
def KEY_VOLUMEUP          equ 115
def KEY_POWER             equ 116	/* SC System Power Down */
def KEY_KPEQUAL           equ 117
def KEY_KPPLUSMINUS       equ 118
def KEY_PAUSE             equ 119
def KEY_SCALE             equ 120	/* AL Compiz Scale (Expose) */
def KEY_KPCOMMA           equ 121
def KEY_HANGEUL           equ 122
def KEY_HANGUEL           equ KEY_HANGEUL
def KEY_HANJA             equ 123
def KEY_YEN               equ 124
def KEY_LEFTMETA          equ 125
def KEY_RIGHTMETA         equ 126
def KEY_COMPOSE           equ 127

endc ; KEYBOARD_INC