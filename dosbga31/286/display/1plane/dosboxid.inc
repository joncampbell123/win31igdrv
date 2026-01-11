
DBID_BASEIO				equ 28h

DBID_INDEX				equ 0 ; R/W
DBID_DATA				equ 1 ; R/W
DBID_STATUS				equ 2 ; R
DBID_COMMAND				equ 2 ;   W

DBID_INDEX_PORT				equ DBID_BASEIO + DBID_INDEX
DBID_DATA_PORT				equ DBID_BASEIO + DBID_DATA
DBID_STATUS_PORT			equ DBID_BASEIO + DBID_STATUS
DBID_COMMAND_PORT			equ DBID_BASEIO + DBID_COMMAND

DBID_CMD_RESET_LATCH			equ 000h
DBID_CMD_FLUSH_WRITE			equ 001h
DBID_CMD_PUSH_STATE			equ 020h
DBID_CMD_POP_STATE			equ 021h
DBID_CMD_DISCARD_STATE			equ 022h
DBID_CMD_DISCARD_ALL_STATE		equ 023h
DBID_CMD_CLEAR_ERROR			equ 0FEh
DBID_CMD_RESET_INTERFACE		equ 0FFh

DBID_RESET_DATA_CODE			equ 00D05B0C5h
DBID_RESET_INDEX_CODE			equ 0AA55BB66h

; Damn it MASM why do you allow SHR/SHL and AND operators if I can't do it to a 32-bit
; constant down to a 16-bit integer? You're going to wish you were more cooperative
; the day I replace you with NASM (Netwide Assembler) to assemble this code.
DBID_RESET_DATA_CODE_HI			equ 00D05h
DBID_RESET_DATA_CODE_LO			equ 0B0C5h
DBID_RESET_INDEX_CODE_HI		equ 0AA55h
DBID_RESET_INDEX_CODE_LO		equ 0BB66h

DBID_REG_DEBUG_OUT			equ 00000DEB0h
DBID_REG_DEBUG_CLEAR			equ 00000DEB1h

DBID_REG_VGAIG_CAPS			equ 000684599h
DBID_REG_VGAIG_CAPS_HI			equ 00068h ; Damn it MASM
DBID_REG_VGAIG_CAPS_LO			equ 04599h
DBID_REG_VGA1G_CAPS_ENABLED		equ 1 SHL 0

DBID_REG_GET_VGA_MEMSIZE		equ 000684581h
DBID_REG_GET_VGA_MEMSIZE_HI		equ 00068h
DBID_REG_GET_VGA_MEMSIZE_LO		equ 04581h

DBID_REG_VGAIG_CTL			equ 000684590h
DBID_REG_VGAIG_CTL_HI			equ 00068h
DBID_REG_VGAIG_CTL_LO			equ 04590h

DBID_REG_VGAIG_CTL_OVERRIDE		equ 1 SHL 0
DBID_REG_VGAIG_CTL_VGAREG_LOCKOUT	equ 1 SHL 1
DBID_REG_VGAIG_CTL_3DA_LOCKOUT		equ 1 SHL 2
DBID_REG_VGAIG_CTL_DAC_LOCKOUT		equ 1 SHL 3
DBID_REG_VGAIG_CTL_OVERRIDE_REFRESH	equ 1 SHL 4
DBID_REG_VGAIG_CTL_ACPAL_BYPASS		equ 1 SHL 5
DBID_REG_VGAIG_CTL_VBEMODESET_DISABLE	equ 1 SHL 6
DBID_REG_VGA1G_CTL_A0000_FORCE		equ 1 SHL 7

DBID_REG_VGAIG_DISPLAYSIZE		equ 000684591h
DBID_REG_VGAIG_DISPLAYSIZE_HI		equ 00068h
DBID_REG_VGAIG_DISPLAYSIZE_LO		equ 04591h

DBID_REG_VGAIG_HVTOTALADD		equ 000684592h
DBID_REG_VGAIG_HVTOTALADD_HI		equ 00068h
DBID_REG_VGAIG_HVTOTALADD_LO		equ 04592h

; Damn it MASM and symbol length limit
;DBID_REG_VGAIG_FMT_BYTESPERSCANLINE    equ 000684593h
;DBID_REG_VGAIG_FMT_BYTESPERSCANLINE_HI equ 00068h
;DBID_REG_VGAIG_FMT_BYTESPERSCANLINE_LO equ 04593h
DBID_REG_VGAIG_FMT_BPSL   		equ 000684593h
DBID_REG_VGAIG_FMT_BPSL_HI		equ 00068h
DBID_REG_VGAIG_FMT_BPSL_LO		equ 04593h

; these masks sit in the high 16 bit word
DBID_REG_VGAIG_FMTHI_MASK		equ 0FFh SHL (16 - 16)
DBID_REG_VGAIG_FMTHI_1BPP		equ 1 SHL (16 - 16)

DBID_REG_VGAIG_REFRESHRATE		equ 000684594h
DBID_REG_VGAIG_REFRESHRATE_HI		equ 00068h
DBID_REG_VGAIG_REFRESHRATE_LO		equ 04594h

DBID_REG_VGAIG_DISPLAYOFFSET		equ 000684595h
DBID_REG_VGAIG_DISPLAYOFFSET_HI		equ 00068h
DBID_REG_VGAIG_DISPLAYOFFSET_LO		equ 04595h

; Damn it MASM
;DBID_REG_VGAIG_HVPELSCALE		equ 000684596h
;DBID_REG_VGAIG_HVPELSCALE_HI		equ 00068h
;DBID_REG_VGAIG_HVPELSCALE_LO		equ 04596h
DBID_REG_VGAIG_HVPS			equ 000684596h
DBID_REG_VGAIG_HVPS_HI			equ 00068h
DBID_REG_VGAIG_HVPS_LO			equ 04596h

; Damn it MASM
;DBID_REG_VGAIG_BANKWINDOW		equ 000684597h
;DBID_REG_VGAIG_BANKWINDOW_HI		equ 00068h
;DBID_REG_VGAIG_BANKWINDOW_LO		equ 04597h
DBID_REG_VGAIG_BW			equ 000684597h
DBID_REG_VGAIG_BW_HI			equ 00068h
DBID_REG_VGAIG_BW_LO			equ 04597h

DBID_REG_VGAIG_ASPECTRATIO		equ 000684598h
DBID_REG_VGAIG_ASPECTRATIO_HI		equ 00068h
DBID_REG_VGAIG_ASPECTRATIO_LO		equ 04598h

; in: c
; out: none
; destroys: AL
dosbox_id_command_write 		macro	c
					mov	al,c
					out	DBID_COMMAND_PORT,al
					endm

; in: none
; out: 32-bit data DX:AX
; destroys: DX:AX
dosbox_id_read_data_m			macro
					in	ax,DBID_DATA_PORT ; low 16
					xchg	dx,ax
					in	ax,DBID_DATA_PORT ; high 16
					xchg	dx,ax
					endm

; in: 32-bit data DX:AX
; out: none
; destroys: none
dosbox_id_write_data_m			macro
					out	DBID_DATA_PORT,ax ; low 16
					xchg	dx,ax
					out	DBID_DATA_PORT,ax ; high 16
					xchg	dx,ax
					endm

; in: none
; out: none
; destroys: AX
dosbox_id_write_data_mc			macro	c
					mov	ax,c AND 0FFFFh
					out	DBID_DATA_PORT,ax ; low 16
					mov	ax,c SHR 16
					out	DBID_DATA_PORT,ax ; high 16
					endm

; in: none
; out: none
; destroys: AX
dosbox_id_write_data_mchl		macro	h,l
					mov	ax,l
					out	DBID_DATA_PORT,ax ; low 16
					mov	ax,h
					out	DBID_DATA_PORT,ax ; high 16
					endm

; in: none
; out: 32-bit data DX:AX
; destroys: DX:AX
dosbox_id_read_regsel_m			macro
					in	ax,DBID_INDEX_PORT ; low 16
					xchg	dx,ax
					in	ax,DBID_INDEX_PORT ; high 16
					xchg	dx,ax
					endm

; in: 32-bit data DX:AX
; out: none
; destroys: none
dosbox_id_write_regsel_m		macro
					out	DBID_INDEX_PORT,ax ; low 16
					xchg	dx,ax
					out	DBID_INDEX_PORT,ax ; high 16
					xchg	dx,ax
					endm

; in: 32-bit data
; out: none
; destroys: AX
dosbox_id_write_regsel_mc		macro	c
					mov	ax,c AND 0FFFFh
					out	DBID_INDEX_PORT,ax ; low 16
					mov	ax,c SHR 16
					out	DBID_INDEX_PORT,ax ; high 16
					endm

; in: 32-bit data
; out: none
; destroys: AX
dosbox_id_write_regsel_mchl		macro	h,l
					mov	ax,l
					out	DBID_INDEX_PORT,ax ; low 16
					mov	ax,h
					out	DBID_INDEX_PORT,ax ; high 16
					endm

; in: 8-bit data AL
; out: none
; destroys: none
dosbox_id_write_data_u8_m		macro
					out	DBID_DATA_PORT,al
					endm

dosbox_id_write_debug_stringconst_mc	macro	p
					push	ds
					mov	si,seg p
					mov	ds,si
					mov	si,offset p
					call	dosbox_id_write_debug_string
					pop	ds
					endm

