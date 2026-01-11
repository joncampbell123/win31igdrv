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
	.list

	??_out	egainit
	externA		ScreenSelector	; the selector for display memory
	externFP      	AllocCSToDSAlias; get a data seg alias for CS
	externFP	FreeSelector	; free a selector

sBegin	Data

	externW ssb_mask	;Mask for save screen bitmap bit


sEnd	Data

createSeg _INIT,InitSeg,byte,public,CODE
sBegin	InitSeg
assumes cs,InitSeg
page

	externW	physical_device
	externNP dosbox_ig_detect

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
	mov	ax,1			;no way to have error

cEnd

sEnd	InitSeg
end
