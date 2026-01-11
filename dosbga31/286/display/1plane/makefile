#   Window's Sample Display Drivers.
#
#   Requirements:
#
#	MASM 4.01 or greater with the environment variable INCLUDE set to
#	the directories containing CMACROS.INC, GDIDEFS.INC, and WINDEFS.INC.
#
#	MASM 4.00 or greater with the ASM inference definition changed to
#	include the directories containing CMACROS.INC, GDIDEFS.INC, and
#	WINDEFS.INC.



#   Options:
#
#	The command line may define options to MASM by defining the OPT
#	macro.	By defining the OPT parameter in the make file, any
#	possible interaction with an environment definition is avoided.

OPT = -DPUBDEFS -DVGA_MONO -T -D?QUIET -DDEBUG
#OPT = -DPUBDEFS -T -D?QUIET -DDEBUG

#   Includes:
#
#	Define the include on up the display driver tree for this
#	particular directory.

INCS = -I. -I.\bitblt

#   Resolution is used to define if hires or lores dithering should take
#   Place.  It will be define when needed.


RES =


#   Define the default assemble command.  This command could actually
#   be overridden from the command line, but shouldn't be.

ASM = masm $(INCS) -v -ML  $(OPT)				# MASM 4.01 & >
#   ASM = masm $(INCS) -v -ML  $(OPT) -I\finc			# MASM 4.00



#   Define the default inference rules

.asm.obj:
        $(ASM) $**,$@;

all: bw rc_high dbig_m.drv dbig_m.sym

# I can't believe Microsoft never considered adding this to their makefiles
clean:
	del rc_high\*.obj
	del rc_high\*.res
	del rc_high\*.bin
	del rc_high\*.rcv
	del *.obj
	del *.sym
	del *.map
	del *.res
	del *.drv

bw:  $(OBJS)

OBJS=   inquire.obj setmode.obj disable.obj color.obj blkwhite.obj      \
        chkstk.obj cursors.obj control.obj fb.obj charwdth.obj          \
        lodither.obj hidither.obj sswitch.obj enable.obj pixel.obj      \
        cblt.obj egainit.obj output.obj scanlr.obj scanline.obj         \
        polyline.obj strblt.obj buildstr.obj smartfix.obj smartpro.obj  \
        ssb.obj robject.obj enum.obj colorinf.obj

#   Define the dependancies

inquire.obj:	inquire.asm

setmode.obj:	setmode.asm

enable.obj:	enable.asm

disable.obj:	disable.asm

color.obj:	.\color.asm

blkwhite.obj:	blkwhite.asm

chkstk.obj:	chkstk.asm

cursors.obj:	cursors.asm

control.obj:	control.asm

fb.obj: 	fb.asm

charwdth.obj:	charwdth.asm fontseg.inc
	$(ASM) $*;

lodither.obj:	dither.asm
	$(ASM) -DLORES $**,$@;

hidither.obj:	dither.asm
	$(ASM) -DHIRES $**,$@;

sswitch.obj:	sswitch.asm

robject.obj:	robject.asm

enum.obj:	enum.asm

colorinf.obj:	colorinf.asm
	$(ASM) $**,$@;

pixel.obj:	pixel.asm

cblt.obj:	cblt.asm		\
		bitblt\genlocal.blt	\
		bitblt\genconst.blt	\
		bitblt\gendata.blt	\
		bitblt\ropdefs.blt	\
		bitblt\roptable.blt	\
		       clrlocal.blt	\
		       clrconst.blt	\
		       clrdata.blt	\
		       devlocal.blt	\
		       devconst.blt	\
		       devdata.blt
	$(ASM) $*;

egainit.obj:	egainit.asm

output.obj:	output.asm

scanlr.obj:	scanlr.asm

scanline.obj:	scanline.asm

polyline.obj:	polyline.asm polybitm.asm polystyl.asm  \
                lines.inc clip.asm polyline.inc
                $(ASM) $*,$@;

strblt.obj:	strblt.asm strblt.inc    fontseg.inc
                $(ASM) $*,$@;

buildstr.obj:	buildstr.asm strblt.inc    fontseg.inc
		$(ASM) $*;

smartfix.obj:	smartfix.asm strblt.inc    fontseg.inc
		$(ASM) $*;

smartpro.obj:	smartpro.asm strblt.inc    fontseg.inc
		$(ASM) $*;

ssb.obj:	ssb.asm
		$(ASM) $*;


rc_high:
        copy dbig_m.rcv    rc_high\display.rcv
        cd    rc_high
        del *.res
        $(MAKE) blkwhite.res
        cd    ..

bitblt.obj:	   bitblt\bitblt.asm	\
		   bitblt\genlocal.blt	\
		   bitblt\genconst.blt	\
		   bitblt\gendata.blt	\
		   bitblt\ropdefs.blt	\
		   bitblt\roptable.blt	\
			  clrlocal.blt	\
			  clrconst.blt	\
			  clrdata.blt	\
			  pdevice.blt	\
			  pattern.blt	\
			  copydev.blt	\
			  devlocal.blt	\
			  devconst.blt	\
			  devdata.blt	\
			  computey.blt	\
			  special.blt	\
			  exit.blt	\
			  cursor.inc	\
		          display.inc	  
	$(ASM)    bitblt\$*,$@;

bmc_main.obj:     bitblt\bmc_main.asm 
	$(ASM)    bitblt\$*,$@;

bmc_eti.obj:      bitblt\bmc_eti.asm 
	$(ASM)    bitblt\$*,$@;

bmc_ite.obj:      bitblt\bmc_ite.asm 
	$(ASM)    bitblt\$*,$@;

discreen.obj:     bitblt\discreen.asm 
	$(ASM)    bitblt\$*,$@;

rlebm.obj:        bitblt\rlebm.asm 
	$(ASM)    bitblt\$*,$@;

scanline.obj:	scanline.asm
    $(ASM) $*,$@;

enable.obj:	   enable.asm
    $(ASM)    $*,$@;

cursor.obj:	cursor.asm egamemd.inc cursor.inc 
    $(ASM) $*,$@;

3xswitch.obj:	3xswitch.asm
    $(ASM) $**,$@;

dbig_m.obj:	dbig_m.asm
    $(ASM) $**,$@;

dbig_m.res:   dbig_m.rcv     rc_high\blkwhite.res
    copy    rc_high\blkwhite.res dbig_m.res

dbig_m.drv: dbig_m.def dbig_m.lnk dbig_m.res dbig_m.rcv	\
		       .\dbig_m.obj	     .\cursor.obj	\
		       .\bitblt.obj	     .\3xswitch.obj	\
                       .\bmc_main.obj        .\bmc_eti.obj      \
                       .\bmc_ite.obj         .\discreen.obj     \
		       .\rlebm.obj			        \
                       .\enable.obj	     .\polyline.obj	\
 		       .\scanline.obj	     .\strblt.obj	\
		       .\cblt.obj	     .\scanlr.obj	\
		       .\pixel.obj	    			\
		       .\output.obj				\
		       .\buildstr.obj	     .\smartfix.obj	\
		       .\smartpro.obj	    			\
		       .\egainit.obj				\
		       .\robject.obj	     .\colorinf.obj	\
		       .\enum.obj				\
		         control.obj           cursors.obj	\
		         inquire.obj           setmode.obj	\
		         hidither.obj          disable.obj	\
		         chkstk.obj				\
		         charwdth.obj          fb.obj		\
		         sswitch.obj
    link @dbig_m.lnk
    rc dbig_m.res dbig_m.drv

dbig_m.map:  dbig_m.drv

dbig_m.sym: dbig_m.map
    copy dbig_m.map display.map
    mapsym display
    copy display.sym dbig_m.sym
    del display.sym
    del display.map

