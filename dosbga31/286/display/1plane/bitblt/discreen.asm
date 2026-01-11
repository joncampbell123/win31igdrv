	page	,132

ifdef	HERCULES
	CGAHERC = 1
endif
ifdef	IBM_CGA
	CGAHERC = 1
endif

ifdef	EGA_MONO
	MONO = 1
endif

ifdef	VGA_MONO
	MONO = 1
endif

ifdef	EGA_HIBW
	MONO = 1
endif

; for the MONO family of drivers we will not be using any of the device
; registers (as the mouse code assumes that no one's going to touch the
; device registers. That means that to do clipped bytes we will have to
; do it the way it is done for CGAHERC drivers. So we will create a special
; symbol for CGAHERC and MONO drivers called CGAHERCMONO. If this label is
; set, we will not touch any of the registers and do clipping by reading
; in the byte, changing the unmasked pels and writing the byte back.

ifdef	CGAHERC
	CGAHERCMONO = 1
endif

ifdef	MONO
	CGAHERCMONO = 1
endif


	.xlist
	include	cmacros.inc
	include	gdidefs.inc
	include	display.inc
	include	macros.mac
	.list


IFDEF	CGAHERC
	EXCLUSION = 1
ENDIF

;----------------------------------------------------------------------------;
; define the equates and externAs here.					             ;
;----------------------------------------------------------------------------;

	externA		__NEXTSEG	; offset to next segment

MAP_IS_HUGE	equ	10000000b	; source maps spans segments
COLOR_BLT	equ	01000000b	; the screen is a color device

;----------------------------------------------------------------------------;

	externA	SCREEN_WIDTH		; width of screen in pels
	externA	SCREEN_W_BYTES		; width of screen in bytes
	externA	ScreenSelector		; video segment address
        externA COLOR_FORMAT            ; own color format

IFDEF	CGAHERC
	externA	Y_SHIFT_COUNT		; level of interleaving
ENDIF

ifdef EXCLUSION
        externFP exclude_far            ; xeclude cursor from blt area
        externFP unexclude_far          ; redraw cursor
endif

	externFP sum_RGB_alt_far	; get nearest color 
        externFP RLEBitBlt              ; in .\rlebm.asm

	externNP mono_munge		; in .\rlebm.asm
	externNP mono_munge_ret	        ; in .\rlebm.asm

sBegin	Data

	externB	enabled_flag

sEnd	Data

createSeg   _DIMAPS,DIMapSeg,word,public,code
sBegin	DIMapSeg
	assumes	cs,DIMapSeg
	assumes	ds,Data
	assumes	es,nothing

;----------------------------------------------------------------------------;
; The following routine find the interection of two rectangles.		     ;
;									     ;
; ES:DI   ---   points to a clipping rectangle				     ;
; DS:SI   ---   points to a source rectangle			             ;
;									     ;
; Returns:								     ;
;		rectangle pointed by ES:DI is not touched	  	     ;
;		DS:SI points to intersected rectangle			     ;
;		AX has no of scans clipped off the bottom (if any clip)	     ;
;		BX has no of pels clipped off the left edge (if any)         ;
;----------------------------------------------------------------------------;

cProc	IntersectRects,<NEAR,PUBLIC>

cBegin

; clip the top

	mov	ax,[si].top
	mov	bx,es:[di].top
	max_ax	bx			; the interected top is the max of the 2
	mov	[si].top,ax		; save it

; clip the right

	mov	ax,[si].right
	mov	bx,es:[di].right
	min_ax	bx			; the interected rt is the min of 2
	mov	[si].right,ax

; now  clip the bottom taking care to remember amount clipped

	mov	ax,[si].bottom		
	mov	bx,es:[di].bottom
	sub	ax,bx			; if we do clip |ax| has amount of clip
	mov	cx,ax			; save amount of clip
	cwd				; DX = 0, if no clipping
	and	ax,dx
	add	ax,bx			; ax has minimum
	not	dx
	and	cx,dx			; cx has amount of clip
	mov	[si].bottom,ax		; save

	push	cx			; save amount of clip at bottom

; now clip the left,

	mov	ax,[si].left
	mov	bx,es:[di].left
	sub	ax,bx
	mov	cx,ax			; save the amount of clip
	cwd
	not	dx
	and	ax,dx
	add	ax,bx			; get the max of the two
	not	dx
	and	cx,dx
	neg	cx			; make it positive
	mov	bx,cx			; have for return in bx
	mov	[si].left,ax		; save new left
	pop	ax			; get back amount clipped on bottom 


; coud be possible that left > right or top > bottom this means that the
; rectangle was clipped in totality
	
	mov	cx,[si].left
	cmp	cx,[si].right
	ja	total_clip_in_x
check_total_clip_in_y:
	mov	cx,[si].top
	cmp	cx,[si].bottom
	ja	total_clip_in_y
	jmp	IntersectRectsRet
total_clip_in_x:
	mov	[si].right,cx
	jmp	short check_total_clip_in_y
total_clip_in_y:
	mov	[si].bottom,cx

IntersectRectsRet:
cEnd

;----------------------------------------------------------------------------;


sEnd	DIMapSeg

	end



