v7vga: obj_dir src

# Build modes. OFFICIAL is set only by the official builder
#              FINAL is set only for the final ship version.
!ifdef OFFICIAL
RCOFF=-DOFFICIAL
!endif
!ifdef FINAL
RCFINAL=-DFINAL
!endif

obj_dir:
        mkdir obj

src:
        cd mak
        make RCFLAGS="$(RCOFF) $(RCFINAL)" v731vga.mak
        cd ..
