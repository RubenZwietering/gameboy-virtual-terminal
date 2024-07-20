if !def(SCREEN_INC)
def SCREEN_INC equ 1

include "hardware.inc"

def _VRAM_TILE_SIZE			equ $0010
def _VRAM_BANK_SIZE			equ $1000
def _SCRN_BANK_SIZE			equ _SCRN1 - _SCRN0

def SCRN_W_B				equ (SCRN_X_B + 1)
def SCRN_H_B				equ (SCRN_Y_B + 1)
def SCRNM_VX_B				equ (SCRN_VX_B - 1)
def SCRNM_VY_B				equ (SCRN_VX_B - 1)

def SCRN_OX					equ (SCRN_VY - SCRN_Y / 2)
def SCRN_OY					equ (SCRN_VX - SCRN_X / 2)

; OAMDMA : calls oam_dma with high byte of N
;
; arguments
; - N : address of shadow OAM to call oam_dma with (has to be aligned)
;
; destroys `af`
macro OAMDMA
	ld a, HIGH(\1)
	call oam_dma ; oam dma routine
endm
; OAMDMA

endc ; SCREEN_INC