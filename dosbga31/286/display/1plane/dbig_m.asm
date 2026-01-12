
        page    ,132
;-----------------------------Module-Header-----------------------------;
; Module Name:	dbig_m.ASM
;
;   This module contains functions and definitions specific to
;   the dbig_m Display Driver.
;
; Created: 22-Feb-1987
; Author:  Walt Moore [waltm]
;
; Copyright (c) 1984-1987 Microsoft Corporation
;
; Exported Functions:	none
;
; Public Functions:	physical_enable
;			physical_disable
;
; Public Data:
;		PHYS_DEVICE_SIZE		info_table_base
;		BW_THRESHOLD			physical_device
;		COLOR_FORMAT
;		SCREEN_W_BYTES
;		SCREEN_WIDTH
;		SCREEN_HEIGHT			ScreenSelector
;		COLOR_TBL_SIZE
;
;		HYPOTENUSE
;		Y_MAJOR_DIST
;		X_MAJOR_DIST
;		Y_MINOR_DIST
;		X_MINOR_DIST
;		MAX_STYLE_ERR
;
;		 H_HATCH_BR_0, H_HATCH_BR_1, H_HATCH_BR_2, H_HATCH_BR_3
;		 H_HATCH_BR_4, H_HATCH_BR_5, H_HATCH_BR_6, H_HATCH_BR_7
;		 V_HATCH_BR_0, V_HATCH_BR_1, V_HATCH_BR_2, V_HATCH_BR_3
;		 V_HATCH_BR_4, V_HATCH_BR_5, V_HATCH_BR_6, V_HATCH_BR_7
;		D1_HATCH_BR_0,D1_HATCH_BR_1,D1_HATCH_BR_2,D1_HATCH_BR_3
;		D1_HATCH_BR_4,D1_HATCH_BR_5,D1_HATCH_BR_6,D1_HATCH_BR_7
;		D2_HATCH_BR_0,D2_HATCH_BR_1,D2_HATCH_BR_2,D2_HATCH_BR_3
;		D2_HATCH_BR_4,D2_HATCH_BR_5,D2_HATCH_BR_6,D2_HATCH_BR_7
;		CR_HATCH_BR_0,CR_HATCH_BR_1,CR_HATCH_BR_2,CR_HATCH_BR_3
;		CR_HATCH_BR_4,CR_HATCH_BR_5,CR_HATCH_BR_6,CR_HATCH_BR_7
;		DC_HATCH_BR_0,DC_HATCH_BR_1,DC_HATCH_BR_2,DC_HATCH_BR_3
;		DC_HATCH_BR_4,DC_HATCH_BR_5,DC_HATCH_BR_6,DC_HATCH_BR_7
;
; General Description:
;
; Restrictions:
;
;-----------------------------------------------------------------------;
STOP_IO_TRAP	equ 4000h		; stop io trapping
START_IO_TRAP	equ 4007h		; re-start io trapping


incDevice = 1				;Include control for gdidefs.inc

	.xlist
	include cmacros.inc
	include gdidefs.inc
	include display.inc
	include macros.mac
	include	cursor.inc
	.list

	externA	ScreenSelector
	externA	WinFlags

	public	PHYS_DEVICE_SIZE	;Number of bytes in physical device
	public	BW_THRESHOLD		;Black/white threshold
	public	COLOR_FORMAT		;Color format
	public	X_SCREEN_W_BYTES	;Screen width in bytes
	public	SCREEN_W_BYTES		;Screen width in bytes
	public	SCREEN_WIDTH		;Screen width in pixels
	public	SCREEN_HEIGHT		;Screen height in scans
	public	COLOR_TBL_SIZE		;Number of entries in color table
	public	physical_enable 	;Enable routine
	public	physical_disable	;Disable

	public	physical_device 	;Physical device descriptor
	public	info_table_base 	;GDIInfo table base address

	public	BlueMoonSeg_color_table


	public	HYPOTENUSE
	public	Y_MAJOR_DIST
	public	X_MAJOR_DIST
	public	Y_MINOR_DIST
	public	X_MINOR_DIST
	public	MAX_STYLE_ERR

	public	 H_HATCH_BR_0, H_HATCH_BR_1, H_HATCH_BR_2, H_HATCH_BR_3
	public	 H_HATCH_BR_4, H_HATCH_BR_5, H_HATCH_BR_6, H_HATCH_BR_7
	public	 V_HATCH_BR_0, V_HATCH_BR_1, V_HATCH_BR_2, V_HATCH_BR_3
	public	 V_HATCH_BR_4, V_HATCH_BR_5, V_HATCH_BR_6, V_HATCH_BR_7
	public	D1_HATCH_BR_0,D1_HATCH_BR_1,D1_HATCH_BR_2,D1_HATCH_BR_3
	public	D1_HATCH_BR_4,D1_HATCH_BR_5,D1_HATCH_BR_6,D1_HATCH_BR_7
	public	D2_HATCH_BR_0,D2_HATCH_BR_1,D2_HATCH_BR_2,D2_HATCH_BR_3
	public	D2_HATCH_BR_4,D2_HATCH_BR_5,D2_HATCH_BR_6,D2_HATCH_BR_7
	public	CR_HATCH_BR_0,CR_HATCH_BR_1,CR_HATCH_BR_2,CR_HATCH_BR_3
	public	CR_HATCH_BR_4,CR_HATCH_BR_5,CR_HATCH_BR_6,CR_HATCH_BR_7
	public	DC_HATCH_BR_0,DC_HATCH_BR_1,DC_HATCH_BR_2,DC_HATCH_BR_3
	public	DC_HATCH_BR_4,DC_HATCH_BR_5,DC_HATCH_BR_6,DC_HATCH_BR_7

		externFP	AllocSelector


;-----------------------------------------------------------------------;
;	The hatched brush pattern definitions
;-----------------------------------------------------------------------;

H_HATCH_BR_0	equ	00000000b	;Horizontal Hatched brush
H_HATCH_BR_1	equ	00000000b
H_HATCH_BR_2	equ	00000000b
H_HATCH_BR_3	equ	00000000b
H_HATCH_BR_4	equ	11111111b
H_HATCH_BR_5	equ	00000000b
H_HATCH_BR_6	equ	00000000b
H_HATCH_BR_7	equ	00000000b

V_HATCH_BR_0	equ	00001000b	;Vertical Hatched brush
V_HATCH_BR_1	equ	00001000b
V_HATCH_BR_2	equ	00001000b
V_HATCH_BR_3	equ	00001000b
V_HATCH_BR_4	equ	00001000b
V_HATCH_BR_5	equ	00001000b
V_HATCH_BR_6	equ	00001000b
V_HATCH_BR_7	equ	00001000b

D1_HATCH_BR_0	equ	10000000b	;\ diagonal brush
D1_HATCH_BR_1	equ	01000000b
D1_HATCH_BR_2	equ	00100000b
D1_HATCH_BR_3	equ	00010000b
D1_HATCH_BR_4	equ	00001000b
D1_HATCH_BR_5	equ	00000100b
D1_HATCH_BR_6	equ	00000010b
D1_HATCH_BR_7	equ	00000001b

D2_HATCH_BR_0	equ	00000001b	;/ diagonal hatched brush
D2_HATCH_BR_1	equ	00000010b
D2_HATCH_BR_2	equ	00000100b
D2_HATCH_BR_3	equ	00001000b
D2_HATCH_BR_4	equ	00010000b
D2_HATCH_BR_5	equ	00100000b
D2_HATCH_BR_6	equ	01000000b
D2_HATCH_BR_7	equ	10000000b

CR_HATCH_BR_0	equ	00001000b	;+ hatched brush
CR_HATCH_BR_1	equ	00001000b
CR_HATCH_BR_2	equ	00001000b
CR_HATCH_BR_3	equ	00001000b
CR_HATCH_BR_4	equ	11111111b
CR_HATCH_BR_5	equ	00001000b
CR_HATCH_BR_6	equ	00001000b
CR_HATCH_BR_7	equ	00001000b

DC_HATCH_BR_0	equ	10000001b	;X hatched brush
DC_HATCH_BR_1	equ	01000010b
DC_HATCH_BR_2	equ	00100100b
DC_HATCH_BR_3	equ	00011000b
DC_HATCH_BR_4	equ	00011000b
DC_HATCH_BR_5	equ	00100100b
DC_HATCH_BR_6	equ	01000010b
DC_HATCH_BR_7	equ	10000001b

SCREEN_W_BYTES	equ	SCAN_BYTES*1	;"*1" to get in public symbol table

;-----------------------------------------------------------------------;
;	Line style definitions for the EGA Card
;
;	Since the style update code in the line DDA checks for a sign,
;	the values chosen for distances, HYPOTENUSE, and MAX_STYLE_ERR
;	must not be bigger than 127+min(X_MAJOR_DIST,Y_MAJOR_DIST).  If
;	this condition is met, then the sign bit will always be cleared
;	on the first subtraction after every add-back.
;-----------------------------------------------------------------------;


HYPOTENUSE	=	51		;Distance moving X and Y
Y_MAJOR_DIST	=	36		;Distance moving Y only
X_MAJOR_DIST	=	36		;Distance moving X only
Y_MINOR_DIST	=	HYPOTENUSE-X_MAJOR_DIST
X_MINOR_DIST	=	HYPOTENUSE-Y_MAJOR_DIST
MAX_STYLE_ERR	=	HYPOTENUSE*2	;Max error before updating
					;  rotating bit mask



;-----------------------------------------------------------------------;
;	The black/white threshold is used to determine the split
;	between black and white when summing an RGB Triplet
;-----------------------------------------------------------------------;


BW_THRESHOLD	equ	(3*0FFh)/2
page

sBegin	Data

globalW ScratchSel,0			;have a scratch selector
globalW ssb_mask,0FFFFh 		;Mask for save screen bitmap bit
globalB enabled_flag,0			;Display is enabled if non-zero
globalW	is_protected,WinFlags		;LSB set in protected mode
globalW SCREEN_HEIGHT,480		;Screen height in pixels
globalW X_SCREEN_W_BYTES,SCREEN_W_BYTES	;Screen width in bytes

sEnd	Data
page

public dosbox_id_write_debug_string

include dosboxid.inc

sBegin	Code
assumes cs,Code

; some parts of the code cannot use the Data segment (smartpro.asm)
globalW X_CODE_SCREEN_W_BYTES,SCREEN_W_BYTES;Screen width in bytes

dosbox_id_reset proc far
	push	ax
	push	dx

	dosbox_id_command_write DBID_CMD_RESET_INTERFACE
	dosbox_id_command_write DBID_CMD_RESET_LATCH

	dosbox_id_read_data_m ; into DX:AX
	cmp	ax,DBID_RESET_DATA_CODE_LO
	jnz	.errout
	cmp	dx,DBID_RESET_DATA_CODE_HI
	jnz	.errout

	dosbox_id_read_regsel_m ; into DX:AX
	cmp	ax,DBID_RESET_INDEX_CODE_LO
	jnz	.errout
	cmp	dx,DBID_RESET_INDEX_CODE_HI
	jnz	.errout

.ok:	clc
	jmp short .doret

.errout:stc

.doret:	pop	dx
	pop	ax
	ret
dosbox_id_reset endp

; in: DS:SI pointer ASCIIZ string
; out: none
; destroys: SI, AX
dosbox_id_write_debug_string proc far
	push	ax
	push	si

	dosbox_id_command_write DBID_CMD_RESET_LATCH
	dosbox_id_write_regsel_mc DBID_REG_DEBUG_OUT

	dosbox_id_command_write DBID_CMD_RESET_LATCH
.l1:	lodsb
	or	al,al
	jz	.ldone
	dosbox_id_write_data_u8_m ; from AL
	jmp short .l1
.ldone:

	dosbox_id_command_write DBID_CMD_FLUSH_WRITE

	pop	si
	pop	ax
	ret
dosbox_id_write_debug_string endp

sEnd	Code
page



createSeg _INIT,InitSeg,word,public,CODE
sBegin	InitSeg
assumes cs,InitSeg
assumes ds,Data
assumes es,nothing

public dosbox_ig_detect
dosbox_ig_detect	proc near

; make sure the DOSBox Integration Device is there
			call	dosbox_id_reset
			jnc	short .dosboxid_ok
			jmp	short .fail
.dosboxid_ok:

; make sure the DOSBox Integrated Graphics are active (machine=svga_dosbox)
			dosbox_id_command_write DBID_CMD_RESET_LATCH
			dosbox_id_write_regsel_mchl DBID_REG_VGAIG_CAPS_HI,DBID_REG_VGAIG_CAPS_LO
			dosbox_id_command_write DBID_CMD_RESET_LATCH
			dosbox_id_read_data_m ; into DX:AX
			test	ax,DBID_REG_VGA1G_CAPS_ENABLED
			jnz	short .dosboxig_ok
			jmp	short .fail
.dosboxig_ok:

; is there enough video memory for the mode?
; compute how much is needed.
; NTS: I KNOW these are constants. They won't be in the future, they will be variables, because
;      we'll allow the user to control the video mode and set it to any arbitrary mode they want.
			mov	ax,X_SCREEN_W_BYTES
			mov	bx,SCREEN_HEIGHT
			mul	bx			; X_SCREEN_W_BYTES (AX) * SCREEN_HEIGHT (BX) = bytes required (DX:AX)
			mov	cx,dx
			mov	bx,ax			; CX:BX = bytes required

			dosbox_id_command_write DBID_CMD_RESET_LATCH
			dosbox_id_write_regsel_mchl DBID_REG_GET_VGA_MEMSIZE_HI,DBID_REG_GET_VGA_MEMSIZE_LO
			dosbox_id_command_write DBID_CMD_RESET_LATCH
			dosbox_id_read_data_m ; into DX:AX

; is DX:AX >= CX:BX? (available video memory >= bytes required). compute DX:AX -= CX:BX
			sub	ax,bx
			sbb	dx,cx
			jns	short .pass ; if result is negative (sign bit set), then no, there is not enough video memory

.fail:
			stc
			ret
.pass:
			clc
			ret
dosbox_ig_detect	endp

COLOR_FORMAT	equ	0101h


;-----------------------------------------------------------------------;
;	PhysDeviceSize is the number of bytes that the enable routine
;	is to copy into the passed PDevice block before calling the
;	physical_enable routine.  For this driver, the length is the
;	size of the bitmap structure.
;-----------------------------------------------------------------------;

PHYS_DEVICE_SIZE equ	size BITMAP



;-----------------------------------------------------------------------;
;	Allocate the physical device block for the EGA Card.
;	For this driver, physical devices will be in the same format
;	as a normal bitmap descriptor.	This is very convienient since
;	it simplifies the structures that the code must work with.
;
;	The bmWidthPlanes field will be set to zero to simplify some
;	of the three plane code.  By setting it to zero, it can be
;	added to a memory bitmap pointer without changing the pointer.
;	This allows the code to add this in regardless of the type of
;	the device.
;
;	The actual physical block will have some extra bytes stuffed on
;	the end (IntPhysDevice structure), but only the following is static
;-----------------------------------------------------------------------;



;	The following constants keep the parameter list to BITMAP within
;	view on an editing display 80 chars wide.

SCRSEL		equ	ScreenSelector
P		equ	 COLOR_FORMAT AND 000FFh	;# color planes
B		equ	(COLOR_FORMAT AND 0FF00h) SHR 8	;# bits per pixel
H		equ	0				;new display height
W		equ	0				;display width, pels
WB		equ	0				;display width, bytes

physical_device BITMAP <SCRSEL,W,H,WB,P,B,0A0000000H,0,0,0,0,0,0,0>



;-----------------------------------------------------------------------;
;	The GDIInfo data Structure.  The specifics of the EGA
;	mode are passed to GDI via this structure.
;-----------------------------------------------------------------------;

public info_table_base_dpHorzVertRes
public info_table_base_dpMLoWinMetricRes
public info_table_base_dpMHiWinMetricRes

info_table_base label byte

	dw	30Ah			;Version = 3.10
	errnz	dpVersion

	dw	DT_RASDISPLAY		;Device classification
	errnz	dpTechnology-dpVersion-2

	dw	208 			;Horizontal size in millimeters
	errnz	dpHorzSize-dpTechnology-2

	dw	156 			;Vertical size in millimeters
	errnz	dpVertSize-dpHorzSize-2

info_table_base_dpHorzVertRes:
	dw	0			;Horizontal width in pixels
	errnz	dpHorzRes-dpVertSize-2

	dw	0			;Vertical width in pixels
	errnz	dpVertRes-dpHorzRes-2

	dw	1			;Number of bits per pixel
	errnz	dpBitsPixel-dpVertRes-2

	dw	1			;Number of planes
	errnz	dpPlanes-dpBitsPixel-2

	dw	65+6+6			;Number of brushes the device has
	errnz	dpNumBrushes-dpPlanes-2 ;  (Show lots of brushes)

	dw	10			;Number of pens the device has
	errnz	dpNumPens-dpNumBrushes-2;  (2 colors * 5 styles)

	dw	0			;Reserved

	dw	0			;Number of fonts the device has
	errnz	dpNumFonts-dpNumPens-4

	dw	2			;Number of colors in color table
	errnz	dpNumColors-dpNumFonts-2

	dw	size int_phys_device	;Size required for device descriptor
	errnz	dpDEVICEsize-dpNumColors-2

	dw	CC_NONE 		;Curves capabilities
	errnz	dpCurves-dpDEVICEsize-2

	dw	LC_POLYLINE+LC_STYLED	;Line capabilities
	errnz	dpLines-dpCurves-2

	dw	PC_SCANLINE		;Polygonal capabilities
	errnz	dpPolygonals-dpLines-2

	dw	TC_CP_STROKE+TC_RA_ABLE ;Text capabilities
	errnz	dpText-dpPolygonals-2

	dw	CP_RECTANGLE 		;Clipping capabilities
	errnz	dpClip-dpText-2

					;BitBlt capabilities
	dw	RC_BITBLT+RC_BITMAP64+RC_GDI20_OUTPUT+RC_DI_BITMAP+RC_DIBTODEV
	errnz	dpRaster-dpClip-2

	dw	X_MAJOR_DIST		;Distance moving X only
	errnz	dpAspectX-dpRaster-2

	dw	Y_MAJOR_DIST		;Distance moving Y only
	errnz	dpAspectY-dpAspectX-2

	dw	HYPOTENUSE		;Distance moving X and Y
	errnz	dpAspectXY-dpAspectY-2

	dw	MAX_STYLE_ERR		;Length of segment for line styles
	errnz	dpStyleLen-dpAspectXY-2


	errnz	dpMLoWin-dpStyleLen-2	;Metric  Lo res WinX,WinY,VptX,VptY
	dw	2080 			;  HorzSize * 10
	dw	1560 			;  VertSize * 10
info_table_base_dpMLoWinMetricRes:
	dw	0			;  HorizRes
	dw	0 			;  -VertRes


	errnz	dpMHiWin-dpMLoWin-8	;Metric  Hi res WinX,WinY,VptX,VptY
	dw	20800			;  HorzSize * 100
	dw	15600			;  VertSize * 100
info_table_base_dpMHiWinMetricRes:
	dw	0			;  HorizRes
	dw	0 			;  -VertRes


	errnz	dpELoWin-dpMHiWin-8	;English Lo res WinX,WinY,VptX,VptY
	dw	325 			;  HorzSize * 1000
	dw	325 			;  VertSize * 1000
	dw	254 			;  HorizRes * 254
	dw	-254			;  -VertRes * 254


	errnz	dpEHiWin-dpELoWin-8	;English Hi res WinX,WinY,VptX,VptY
	dw	1625			;  HorzSize * 10000
	dw	1625			;  VertSize * 10000
	dw	127 			;  HorizRes * 254
	dw	-127			;  -VertRes * 254


	errnz	dpTwpWin-dpEHiWin-8	;Twips		WinX,WinY,VptX,VptY
	dw	2340			;  HorzSize * 14400
	dw	2340			;  VertSize * 14400
	dw	127 			;  HorizRes * 254
	dw	-127			;  -VertRes * 254


	dw	96			;Logical Pixels/inch in X
	errnz	dpLogPixelsX-dpTwpWin-8

	dw	96			;Logical Pixels/inch in Y
	errnz	dpLogPixelsY-dpLogPixelsX-2

	dw	DC_IgnoreDFNP		;dpDCManage
	errnz	dpDCManage-dpLogPixelsY-2

	dw	0			;Reserved fields
	dw	0
	dw	0
	dw	0
	dw	0

; start of entries in version 3.0 of this structure


	dw	0 			; number of palette entries
	errnz	dpNumPalReg-dpDCManage-12

        dw      0                       ; number of reserved
        errnz   dpPalReserved-dpNumPalReg-2

	dw	0			; DAC resolution for RGB
	errnz	dpColorRes-dpPalReserved-2


	errnz	<(offset $)-(offset info_table_base)-(size GDIINFO)>

page

msg_starting_video_setup db 'Display driver startup beginning',10,0
msg_starting_video_shutdown db 'Display driver shutdown beginning',10,0

;---------------------------Public-Routine------------------------------;
; physical_enable
;
;   640x480 b/w graphics mode is enabled.
;
; Entry:
;	ES:DI --> ipd_format in our pDevice
;	DS: = Data
; Returns:
;	AX = non-zero to show success
; Error Returns:
;	AX = 0
; Registers Preserved:
;	BP
; Registers Destroyed:
;	AX,BX,CX,DX,SI,DI,ES,DS,FLAGS
; Calls:
;	INT 10h
; History:
;	Tue 18-Aug-1987 18:09:00 -by-  Walt Moore [waltm]
;	Added enabled_flag
;
;	Thu 26-Feb-1987 13:45:58 -by-  Walt Moore [waltm]
;	Created.
;-----------------------------------------------------------------------;


;------------------------------Pseudo-Code------------------------------;
; {
; }
;-----------------------------------------------------------------------;

DBIG_INIT_CTL	equ	DBID_REG_VGAIG_CTL_OVERRIDE + \
			DBID_REG_VGAIG_CTL_VGAREG_LOCKOUT + \
			DBID_REG_VGAIG_CTL_OVERRIDE_REFRESH + \
			DBID_REG_VGAIG_CTL_ACPAL_BYPASS + \
			DBID_REG_VGAIG_CTL_VBEMODESET_DISABLE + \
			DBID_REG_VGA1G_CTL_A0000_FORCE

physical_enable proc near

	dosbox_id_write_debug_stringconst_mc msg_starting_video_setup

; first use iNT 10h to set the mode-----eventually we'll remove that when no longer needed

;----------------------------------------------------------------------------;
; allocate the scratch selector here				             ;
;----------------------------------------------------------------------------;
	push	es
	xor	ax,ax
	cCall	AllocSelector,<ax>	; get a free selector
	pop	es
	mov	ScratchSel,ax		; save it

;-----------------------------------------------------------------------------;

	mov	enabled_flag,0FFh	;Show enabled

; clear the ctl first
	dosbox_id_command_write DBID_CMD_RESET_LATCH
	dosbox_id_write_regsel_mchl DBID_REG_VGAIG_CTL_HI,DBID_REG_VGAIG_CTL_LO
	dosbox_id_command_write DBID_CMD_RESET_LATCH
	dosbox_id_write_data_mchl 0,0

; set the format and bytes per line
	dosbox_id_command_write DBID_CMD_RESET_LATCH
	dosbox_id_write_regsel_mchl DBID_REG_VGAIG_FMT_BPSL_HI,DBID_REG_VGAIG_FMT_BPSL_LO
	dosbox_id_command_write DBID_CMD_RESET_LATCH
	mov	ax,X_SCREEN_W_BYTES
	mov	dx,DBID_REG_VGAIG_FMTHI_1BPP
	dosbox_id_write_data_m ; DX:AX

; width and height
	dosbox_id_command_write DBID_CMD_RESET_LATCH
	dosbox_id_write_regsel_mchl DBID_REG_VGAIG_DISPLAYSIZE_HI,DBID_REG_VGAIG_DISPLAYSIZE_LO
	dosbox_id_command_write DBID_CMD_RESET_LATCH
	mov	ax,SCREEN_WIDTH
	mov	dx,SCREEN_HEIGHT
	dosbox_id_write_data_m ; DX:AX

; h/v total add
	dosbox_id_command_write DBID_CMD_RESET_LATCH
	dosbox_id_write_regsel_mchl DBID_REG_VGAIG_HVTOTALADD_HI,DBID_REG_VGAIG_HVTOTALADD_LO
	dosbox_id_command_write DBID_CMD_RESET_LATCH
	mov	ax,64
	mov	dx,64
	dosbox_id_write_data_m ; DX:AX

; refresh rate
	dosbox_id_command_write DBID_CMD_RESET_LATCH
	dosbox_id_write_regsel_mchl DBID_REG_VGAIG_REFRESHRATE_HI,DBID_REG_VGAIG_REFRESHRATE_LO
	dosbox_id_command_write DBID_CMD_RESET_LATCH
	mov	ax,0
	mov	dx,70
	dosbox_id_write_data_m ; DX:AX (70.000Hz)

; scale
	dosbox_id_command_write DBID_CMD_RESET_LATCH
	dosbox_id_write_regsel_mchl DBID_REG_VGAIG_HVPS_HI,DBID_REG_VGAIG_HVPS_LO
	dosbox_id_command_write DBID_CMD_RESET_LATCH
	mov	ax,0
	mov	dx,0
	dosbox_id_write_data_m ; DX:AX (70.000Hz)

; banking
	dosbox_id_command_write DBID_CMD_RESET_LATCH
	dosbox_id_write_regsel_mchl DBID_REG_VGAIG_BW_HI,DBID_REG_VGAIG_BW_LO
	dosbox_id_command_write DBID_CMD_RESET_LATCH
	mov	ax,0
	mov	dx,0
	dosbox_id_write_data_m ; DX:AX

; aspect ratio (TODO: Someday we'll let the user set it from WIN.INI)
	dosbox_id_command_write DBID_CMD_RESET_LATCH
	dosbox_id_write_regsel_mchl DBID_REG_VGAIG_ASPECTRATIO_HI,DBID_REG_VGAIG_ASPECTRATIO_LO
	dosbox_id_command_write DBID_CMD_RESET_LATCH
	mov	ax,0
	mov	dx,0
	dosbox_id_write_data_m ; DX:AX (0:0 = square pixels)

; this ctl config bypasses the Attribute Controller, black & white must be programmed into the VGA palette itself
	xor	ax,ax
	mov	dx,3C8h
	out	dx,al	; 3C8h
	inc	dx
	out	dx,al	; 3C9h
	out	dx,al	; 3C9h
	out	dx,al	; 3C9h
	dec	al	; change 00h to FFh
	out	dx,al	; 3C9h
	out	dx,al	; 3C9h
	out	dx,al	; 3C9h

; switch on
	dosbox_id_command_write DBID_CMD_RESET_LATCH
	dosbox_id_write_regsel_mchl DBID_REG_VGAIG_CTL_HI,DBID_REG_VGAIG_CTL_LO
	dosbox_id_command_write DBID_CMD_RESET_LATCH
	mov	ax,DBIG_INIT_CTL
	xor	dx,dx
	dosbox_id_write_data_m ; DX:AX (1:1)

;----------------------------------------------------------------------------;
; at this point notify kernel that driver is cable of doing a save/restore   ;
; of its state registers and kernel should stop I/O trapping.		     ;
; Do this only if we are in protected mode.				     ;
;----------------------------------------------------------------------------;
	
	test	is_protected,1  	;will be set if in protected mode
	jz	phys_enable_ok		;not in protectde mode
	mov	ax,STOP_IO_TRAP		
	int	2fh		

;----------------------------------------------------------------------------;

phys_enable_ok:
	mov	ax,1

phys_enable_20:
	ret

physical_enable endp
page

;---------------------------Public-Routine------------------------------;
; physical_disable
;
;   640x480 graphics mode is exited.  The previous mode of the
;   adapter is restored.
;
; Entry:
;	DS:SI --> int_phys_device
;	ES:    =  Data
; Returns:
;	AX = non-zero to show success
; Error Returns:
;	None
; Registers Preserved:
;	BP
; Registers Destroyed:
;	AX,BX,CX,DX,SI,DI,ES,DS,FLAGS
; Calls:
;	INT 10h
;	init_hw_regs
; History:
;	Tue 18-Aug-1987 18:09:00 -by-  Walt Moore [waltm]
;	Added enabled_flag
;
;	Thu 26-Feb-1987 13:45:58 -by-  Walt Moore [waltm]
;	Created.
;-----------------------------------------------------------------------;

;------------------------------Pseudo-Code------------------------------;
; {
; }
;-----------------------------------------------------------------------;

	assumes ds,nothing
	assumes es,Data

physical_disable proc near

	dosbox_id_write_debug_stringconst_mc msg_starting_video_shutdown

; clear the ctl
	dosbox_id_command_write DBID_CMD_RESET_LATCH
	dosbox_id_write_regsel_mchl DBID_REG_VGAIG_CTL_HI,DBID_REG_VGAIG_CTL_LO
	dosbox_id_command_write DBID_CMD_RESET_LATCH
	dosbox_id_write_data_mchl 0,0

	mov	enabled_flag,00h 	;Show disabled

;----------------------------------------------------------------------------;
; at this point as the kernel to do the io trapping again, provided we are in;
; protected mode.							     ;
;----------------------------------------------------------------------------;
	
	test	is_protected,1		;will be set if we are in prot mode
	jz	phys_disable_ret	;we are in real mode
	mov	ax,START_IO_TRAP
	int	2fh			;start i/o trapping
phys_disable_ret:
;----------------------------------------------------------------------------;

	mov	al,1
	ret

physical_disable endp

sEnd	InitSeg
page

COLOR_TBL_SIZE	equ	2		;8 entries in the table

createSeg _BLUEMOON,BlueMoonSeg,word,public,CODE
sBegin	BlueMoonSeg

BlueMoonSeg_color_table label	dword

;		dd	xxbbggrr
		dd	00000000h	;Black
		dd	00FFFFFFh	;White

sEnd	BlueMoonSeg
end


