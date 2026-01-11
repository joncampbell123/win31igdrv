        page    ,132
;-----------------------------Module-Header-----------------------------;
; Module Name:	EGAMEMD.INC
;
; This file contains the public definitions of the EGA memory
; locations which driver output routines need to reference.
;
; Created: 29-Apr-1987
; Author:  Walt Moore [waltm]
;
; Copyright (c) 1984-1987 Microsoft Corporation
;
; Restrictions:
;
;   Enter Ye into the valley of death if you change any of these
;   locations since we're in bed with Win386.
;
;   This file is to be included by the cursor code only.
;
;-----------------------------------------------------------------------;
; DOSBox IG: No, we're not EGA and we don't pretend to be EGA.          ;
;            If Win386 is stupid enough to mistake us as one, then we   ;
;            will have to figure out how to correct it.                 ;
;                                                                       ;
;            Just put it at the very end of the 64KB video memory       ;
;            for now. Someday when bank switching is added to support   ;
;            video modes >800x600 the screen buf will have to move to   ;
;            system memory.                                             ;
;-----------------------------------------------------------------------;

EGAMem		segment at 0

		; the original code here did elaborate "EGA" segment definitions
		; that defined a screen_area for the visible screen, a save area
		; just beyond it, and then a screen buf for more cursor work.
		;
		; it would be much simpler and reduce dependence on constants that
		; will eventually become variables, to just define only the save_area
		; and screen_buf and position them correctly using the ORG directive.
		;
		; as much as I would like to just say 10000h - x - y, MASM 4.0
		; will throw a hissy fit about the fact that the number was 32-bit
		; large at any point during the expression, so, we have to work around
		; that to keep all points of the expression evaluation to a value that
		; fits in 16 bits. good lord, how did Microsoft, OEMs, and software
		; developers suffer through these older versions of MASM from the 1980s
		; and early 1990s?

		org	(0FFF0h - MASK_LENGTH - (((MAX_BUF_HEIGHT+1) and 0FFFEh) * BUF_WIDTH)) + 10h ; put it right up against the end of the 64KB segment

save_area	db	MASK_LENGTH dup (?)
screen_buf	db	(((MAX_BUF_HEIGHT+1) and 0FFFEh) * BUF_WIDTH) dup (?)

EGAMem	      ends
