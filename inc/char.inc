if !def(CHAR_INC)
def CHAR_INC equ 1

def CURSOR_BLINK_RATE equ 36 ; 20 for c64 cursor blink time
def CURSOR_OBJECT_PTR equ _OAMRAM
def CURSOR_CHARACTER equ $03

endc ; CHAR_INC