	page	,132
;----------------------------Module-Header------------------------------;
; Module Name: EGAINIT.ASM
;
; EGA initialization code.
; 
; Created: 26 June 1987
; Author: Bob Grudem
;
; Copyright (c) 1985, 1986, 1987  Microsoft Corporation
;
; This module handles disabling of the RC_SAVEBITMAP raster capability
; if the EGA doesn't have 256Kbytes of display memory.  This operation
; happens at run time, because the bit will always be set at assembly-
; time if the SaveScreenBitmap code is present in the driver, though
; a particular EGA board may not be able to use it.
;-----------------------------------------------------------------------;

	title	EGA Initialization Code

incDevice = 1				;allow assembly of needed constants

	.xlist
	include cmacros.inc
	include gdidefs.inc
	include display.inc
	include macros.mac
	include	cursor.inc
	.list

	??_out	egainit
	externW		info_table_base	; type GDIINFO
	externA		ScreenSelector	; the selector for display memory
	externFP      	AllocCSToDSAlias; get a data seg alias for CS
	externFP	FreeSelector	; free a selector
	externW		SCREEN_HEIGHT
	externW		X_SCREEN_W_BYTES
	externW		real_x
	externW		real_y
	externW		save_area
	externW		screen_buf

sBegin	Code

	externW X_CODE_SCREEN_W_BYTES;Screen width in bytes

sEnd	Code

sBegin	Data

	externW ssb_mask	;Mask for save screen bitmap bit

sEnd	Data

createSeg _INIT,InitSeg,byte,public,CODE
sBegin	InitSeg
assumes cs,InitSeg
page

	externW	physical_device
	externNP dosbox_ig_detect
	externW info_table_base_dpHorzVertRes
	externW info_table_base_dpMLoWinMetricRes
	externW info_table_base_dpMHiWinMetricRes

;--------------------------Public-Routine-------------------------------;
; dev_initialization - device specific initialization
;
; Any device specific initialization is performed.
;
; Entry:
;	None
; Returns:
;	AX = 1
; Registers Preserved:
;	SI,DI,BP,DS
; Registers Destroyed:
;	AX,BX,CX,DX,ES,FLAGS
; Calls:
;	int 10h
; History:
;	Mon 21-Sep-1987 00:34:56 -by-  Walt Moore [waltm]
;	Changed it to be called from driver_initialization and
;	renamed it.
;
;	Fri 26-Jun-1987 -by- Bob Grudem    [bobgru]
;	Creation.
;-----------------------------------------------------------------------;

	assumes ds,Data
	assumes es,nothing

cProc	dev_initialization,<NEAR,PUBLIC>

cBegin

	call	dosbox_ig_detect
	jnc	.igok

	xor	ax,ax
	ret

.igok:

	push	es			; save
	mov	ax,InitSegBASE		; get the CS selector
	cCall	AllocCSToDSAlias,<ax>	; get a data segment alias
	mov	es,ax

	; init initial cursor position.
	; NTS: Not sure about older Windows, but Windows 3.1 appears to call MoveCursor at startup to center the cursor anyway.
	mov	ax,SCREEN_WIDTH/2
	mov	real_x,ax

	mov	ax,SCREEN_HEIGHT
	shr	ax,1
	mov	real_y,ax

	mov	ax,SCREEN_WIDTH
	mov	es:[physical_device.bmWidth],ax
	mov	es:WORD PTR [info_table_base_dpHorzVertRes],ax
	mov	es:[info_table_base_dpMLoWinMetricRes],ax
	mov	es:WORD PTR [info_table_base_dpMHiWinMetricRes],ax

	mov	ax,SCREEN_HEIGHT
	mov	es:[physical_device.bmHeight],ax
	mov	es:WORD PTR [info_table_base_dpHorzVertRes+2],ax

	neg	ax
	mov	es:[info_table_base_dpMLoWinMetricRes+2],ax
	mov	es:WORD PTR [info_table_base_dpMHiWinMetricRes+2],ax

	mov	ax,X_SCREEN_W_BYTES
	mov	es:[physical_device.bmWidthBytes],ax

	; decide where to put cursor buffer
	xor	ax,ax ; allocate from the end of the 64KB video memory region

	mov	bx,((MAX_BUF_HEIGHT+1) and 0FFFEh) * BUF_WIDTH
	sub	ax,bx
	mov	screen_buf,ax

	mov	bx,MASK_LENGTH
	sub	ax,bx
	mov	save_area,ax

	; done with INIT seg
	mov	bx,es
	cCall	FreeSelector,<bx>	;free the alias
	pop	es			; restore

	; now the CODE seg

	push	es			; save
	mov	ax,CodeBASE		; get the CS selector
	cCall	AllocCSToDSAlias,<ax>	; get a data segment alias
	mov	es,ax

	mov	ax,X_SCREEN_W_BYTES
	mov	es:X_CODE_SCREEN_W_BYTES,ax

	; done with CODE seg
	mov	bx,es
	cCall	FreeSelector,<bx>	;free the alias
	pop	es			; restore

	mov	ax,1			;no way to have error

cEnd

sEnd	InitSeg
end
