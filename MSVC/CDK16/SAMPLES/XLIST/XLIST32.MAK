# Microsoft Visual C++ Generated NMAKE File, Format Version 20052
# MSVCPRJ: version 2.0
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Dynamic-Link Library" 0x0102

!IF "$(CFG)" == ""
CFG=Win32 ANSI Debug
!MESSAGE No configuration specified.  Defaulting to Win32 ANSI Debug.
!ENDIF 

!IF "$(CFG)" != "Win32 ANSI Release" && "$(CFG)" != "Win32 ANSI Debug" &&\
 "$(CFG)" != "Win32 Unicode Release" && "$(CFG)" != "Win32 Unicode Debug"
!MESSAGE Invalid configuration "$(CFG)" specified.
!MESSAGE You can specify a configuration when running NMAKE on this makefile
!MESSAGE by defining the macro CFG on the command line.  For example:
!MESSAGE 
!MESSAGE NMAKE /f "xlist32.mak" CFG="Win32 ANSI Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "Win32 ANSI Release" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "Win32 ANSI Debug" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "Win32 Unicode Release" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "Win32 Unicode Debug" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE 
!ERROR An invalid configuration is specified.
!ENDIF 

################################################################################
# Begin Project
# PROP MARKED_FOR_SCAN 1
MTL=MkTypLib.exe
CPP=cl.exe
RSC=rc.exe

!IF  "$(CFG)" == "Win32 ANSI Release"

# PROP BASE Use_MFC 2
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Classwizard_Name "xlist.clw"
# PROP Use_MFC 2
# PROP Use_Debug_Libraries 0
# PROP Classwizard_Name "xlist.clw"
# PROP Output_Dir "obj32"
# PROP Intermediate_Dir "obj32"
OUTDIR=.\obj32
INTDIR=.\obj32

ALL : MTL_TLBS $(OUTDIR)/xlist.ocx $(OUTDIR)/xlist.bsc

$(OUTDIR) : 
    if not exist $(OUTDIR)/nul mkdir $(OUTDIR)

# ADD BASE MTL /nologo /D "NDEBUG" /D "_WIN32" /win32
# ADD MTL /nologo /D "NDEBUG" /D "_WIN32" /win32
MTL_PROJ=/nologo /D "NDEBUG" /D "_WIN32" /win32 

MTL_TLBS : $(OUTDIR)/xlist.tlb
# ADD BASE CPP /nologo /MD /W3 /GX /Yu"stdafx.h" /Ox /D "NDEBUG" /D "_MBCS" /D "_WINDOWS" /D "_AFXDLL" /D "_AFXCTL" /D "_WINDLL" /FR /c
# ADD CPP /nologo /MD /W3 /GX /Yu"stdafx.h" /Ox /D "NDEBUG" /D "_MBCS" /D "_WINDOWS" /D "_AFXDLL" /D "_AFXCTL" /D "_WINDLL" /FR /c
CPP_PROJ=/nologo /MD /W3 /GX /Yu"stdafx.h" /Ox /D "NDEBUG" /D "_MBCS" /D "_WINDOWS" /D\
 "_AFXDLL" /D "_AFXCTL" /D "_WINDLL" /FR$(INTDIR)/\
 /Fp$(OUTDIR)/"xlist32.pch" /Fo$(INTDIR)/ /c 
CPP_OBJS=.\obj32/
# ADD BASE RSC /l 0x409 /i "$(OUTDIR)" /d "NDEBUG" /d "_WIN32"
# ADD RSC /l 0x409 /i "$(OUTDIR)" /d "NDEBUG" /d "_WIN32"
RSC_PROJ=/l 0x409 /fo$(INTDIR)/"xlist.res" /i "$(OUTDIR)" /d "NDEBUG" /d "_WIN32" 
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo /o"obj32/xlist.bsc"
# SUBTRACT BASE BSC32 /Iu
# ADD BSC32 /nologo /o"obj32/xlist.bsc"
# SUBTRACT BSC32 /Iu
BSC32_FLAGS=/nologo /o"obj32/xlist.bsc" 
BSC32_SBRS= \
	$(INTDIR)/STDAFX.SBR \
	$(INTDIR)/XLISTCTL.SBR \
	$(INTDIR)/XLISTPPG.SBR \
	$(INTDIR)/XLIST.SBR

$(OUTDIR)/xlist.bsc : $(OUTDIR)  $(BSC32_SBRS)
    $(BSC32) @<<
  $(BSC32_FLAGS) $(BSC32_SBRS)
<<

LINK32=link.exe
# ADD BASE LINK32 ocs30.lib /NOLOGO /SUBSYSTEM:windows /DLL /MACHINE:IX86 /OUT:"obj32/xlist.ocx"
# SUBTRACT BASE LINK32 /PDB:none /NODEFAULTLIB
# ADD LINK32 ocs30.lib /NOLOGO /SUBSYSTEM:windows /DLL /MACHINE:IX86 /OUT:"obj32/xlist.ocx"
# SUBTRACT LINK32 /PDB:none /NODEFAULTLIB
LINK32_FLAGS=ocs30.lib /NOLOGO /SUBSYSTEM:windows /DLL /INCREMENTAL:no\
 /MACHINE:IX86 /DEF:".\xlist32.DEF"\
 /OUT:"obj32/xlist.ocx" /IMPLIB:$(OUTDIR)/"xlist.lib" 
DEF_FILE=.\xlist32.DEF
LINK32_OBJS= \
	$(INTDIR)/STDAFX.OBJ \
	$(INTDIR)/XLISTCTL.OBJ \
	$(INTDIR)/XLISTPPG.OBJ \
	$(INTDIR)/XLIST.OBJ \
	$(INTDIR)/XLIST.res

$(OUTDIR)/xlist.ocx : $(OUTDIR)  $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

!ELSEIF  "$(CFG)" == "Win32 ANSI Debug"

# PROP BASE Use_MFC 2
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Classwizard_Name "xlist.clw"
# PROP Use_MFC 2
# PROP Use_Debug_Libraries 1
# PROP Classwizard_Name "xlist.clw"
# PROP Output_Dir "objd32"
# PROP Intermediate_Dir "objd32"
OUTDIR=.\objd32
INTDIR=.\objd32

ALL : MTL_TLBS $(OUTDIR)/xlist.ocx $(OUTDIR)/xlist.bsc

$(OUTDIR) : 
    if not exist $(OUTDIR)/nul mkdir $(OUTDIR)

# ADD BASE MTL /nologo /D "_DEBUG" /D "_WIN32" /win32
# ADD MTL /nologo /D "_DEBUG" /D "_WIN32" /win32
MTL_PROJ=/nologo /D "_DEBUG" /D "_WIN32" /win32 

MTL_TLBS : $(OUTDIR)/xlist.tlb
# ADD BASE CPP /nologo /MD /W3 /GX /Zi /Yu"stdafx.h" /Od /D "_DEBUG" /D "_MBCS" /D "_WINDOWS" /D "_AFXDLL" /D "_AFXCTL" /D "_WINDLL" /FR /c
# ADD CPP /nologo /MD /W3 /GX /Zi /Yu"stdafx.h" /Od /D "_DEBUG" /D "_MBCS" /D "_WINDOWS" /D "_AFXDLL" /D "_AFXCTL" /D "_WINDLL" /FR /c
# SUBTRACT CPP /WX
CPP_PROJ=/nologo /MD /W3 /GX /Zi /Yu"stdafx.h" /Od /D "_DEBUG" /D "_MBCS" /D "_WINDOWS"\
 /D "_AFXDLL" /D "_AFXCTL" /D "_WINDLL" /FR$(INTDIR)/\
 /Fp$(OUTDIR)/"xlist32.pch" /Fo$(INTDIR)/ /Fd$(OUTDIR)/"xlist32.pdb" /c 
CPP_OBJS=.\objd32/
# ADD BASE RSC /l 0x409 /i "$(OUTDIR)" /d "_DEBUG" /d "_WIN32"
# ADD RSC /l 0x409 /i "$(OUTDIR)" /d "_DEBUG" /d "_WIN32"
RSC_PROJ=/l 0x409 /fo$(INTDIR)/"xlist.res" /i "$(OUTDIR)" /d "_DEBUG" /d "_WIN32" 
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo /o"objd32/xlist.bsc"
# SUBTRACT BASE BSC32 /Iu
# ADD BSC32 /nologo /o"objd32/xlist.bsc"
# SUBTRACT BSC32 /Iu
BSC32_FLAGS=/nologo /o"objd32/xlist.bsc" 
BSC32_SBRS= \
	$(INTDIR)/STDAFX.SBR \
	$(INTDIR)/XLISTCTL.SBR \
	$(INTDIR)/XLISTPPG.SBR \
	$(INTDIR)/XLIST.SBR

$(OUTDIR)/xlist.bsc : $(OUTDIR)  $(BSC32_SBRS)
    $(BSC32) @<<
  $(BSC32_FLAGS) $(BSC32_SBRS)
<<

LINK32=link.exe
# ADD BASE LINK32 ocs30d.lib /NOLOGO /SUBSYSTEM:windows /DLL /DEBUG /MACHINE:IX86 /OUT:"objd32/xlist.ocx"
# SUBTRACT BASE LINK32 /PDB:none /NODEFAULTLIB
# ADD LINK32 ocs30d.lib /NOLOGO /SUBSYSTEM:windows /DLL /DEBUG /MACHINE:IX86 /OUT:"objd32/xlist.ocx"
# SUBTRACT LINK32 /PDB:none /NODEFAULTLIB
LINK32_FLAGS=ocs30d.lib /NOLOGO /SUBSYSTEM:windows /DLL /INCREMENTAL:yes\
 /DEBUG /MACHINE:IX86 /DEF:".\xlist32.DEF"\
 /OUT:"objd32/xlist.ocx" /IMPLIB:$(OUTDIR)/"xlist.lib" 
DEF_FILE=.\xlist32.DEF
LINK32_OBJS= \
	$(INTDIR)/STDAFX.OBJ \
	$(INTDIR)/XLISTCTL.OBJ \
	$(INTDIR)/XLISTPPG.OBJ \
	$(INTDIR)/XLIST.OBJ \
	$(INTDIR)/XLIST.res

$(OUTDIR)/xlist.ocx : $(OUTDIR)  $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

!ELSEIF  "$(CFG)" == "Win32 Unicode Release"

# PROP BASE Use_MFC 2
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Classwizard_Name "xlist.clw"
# PROP Use_MFC 2
# PROP Use_Debug_Libraries 0
# PROP Classwizard_Name "xlist.clw"
# PROP Output_Dir "obju32"
# PROP Intermediate_Dir "obju32"
OUTDIR=.\obju32
INTDIR=.\obju32

ALL : MTL_TLBS $(OUTDIR)/xlist.ocx $(OUTDIR)/xlist.bsc

$(OUTDIR) : 
    if not exist $(OUTDIR)/nul mkdir $(OUTDIR)

# ADD BASE MTL /nologo /D "NDEBUG" /D "_UNICODE" /D "_WIN32" /win32
# ADD MTL /nologo /D "NDEBUG" /D "_UNICODE" /D "_WIN32" /win32
MTL_PROJ=/nologo /D "NDEBUG" /D "_UNICODE" /D "_WIN32" /win32 

MTL_TLBS : $(OUTDIR)/xlist.tlb
# ADD BASE CPP /nologo /MD /W3 /GX /Yu"stdafx.h" /Ox /D "NDEBUG" /D "_UNICODE" /D "_WINDOWS" /D "_AFXDLL" /D "_AFXCTL" /D "_WINDLL" /FR /c
# ADD CPP /nologo /MD /W3 /GX /Yu"stdafx.h" /Ox /D "NDEBUG" /D "_UNICODE" /D "_WINDOWS" /D "_AFXDLL" /D "_AFXCTL" /D "_WINDLL" /FR /c
CPP_PROJ=/nologo /MD /W3 /GX /Yu"stdafx.h" /Ox /D "NDEBUG" /D "_UNICODE" /D "_WINDOWS" /D\
 "_AFXDLL" /D "_AFXCTL" /D "_WINDLL" /FR$(INTDIR)/\
 /Fp$(OUTDIR)/"xlist32.pch" /Fo$(INTDIR)/ /c 
CPP_OBJS=.\obju32/
# ADD BASE RSC /l 0x409 /i "$(OUTDIR)" /d "NDEBUG" /d "_WIN32" /d "_UNICODE"
# ADD RSC /l 0x409 /i "$(OUTDIR)" /d "NDEBUG" /d "_WIN32" /d "_UNICODE"
RSC_PROJ=/l 0x409 /fo$(INTDIR)/"xlist.res" /i "$(OUTDIR)" /d "NDEBUG" /d "_WIN32" /d\
 "_UNICODE" 
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo /o"obju32/xlist.bsc"
# SUBTRACT BASE BSC32 /Iu
# ADD BSC32 /nologo /o"obju32/xlist.bsc"
# SUBTRACT BSC32 /Iu
BSC32_FLAGS=/nologo /o"obju32/xlist.bsc" 
BSC32_SBRS= \
	$(INTDIR)/STDAFX.SBR \
	$(INTDIR)/XLISTCTL.SBR \
	$(INTDIR)/XLISTPPG.SBR \
	$(INTDIR)/XLIST.SBR

$(OUTDIR)/xlist.bsc : $(OUTDIR)  $(BSC32_SBRS)
    $(BSC32) @<<
  $(BSC32_FLAGS) $(BSC32_SBRS)
<<

LINK32=link.exe
# ADD BASE LINK32 ocs30u.lib /NOLOGO /SUBSYSTEM:windows /DLL /MACHINE:IX86 /OUT:"obju32/xlist.ocx"
# SUBTRACT BASE LINK32 /PDB:none /NODEFAULTLIB
# ADD LINK32 ocs30u.lib /NOLOGO /SUBSYSTEM:windows /DLL /MACHINE:IX86 /OUT:"obju32/xlist.ocx"
# SUBTRACT LINK32 /PDB:none /NODEFAULTLIB
LINK32_FLAGS=ocs30u.lib /NOLOGO /SUBSYSTEM:windows /DLL /INCREMENTAL:no\
 /MACHINE:IX86 /DEF:".\xlist32.DEF" \
 /OUT:"obju32/xlist.ocx" /IMPLIB:$(OUTDIR)/"xlist.lib" 
DEF_FILE=.\xlist32.DEF
LINK32_OBJS= \
	$(INTDIR)/STDAFX.OBJ \
	$(INTDIR)/XLISTCTL.OBJ \
	$(INTDIR)/XLISTPPG.OBJ \
	$(INTDIR)/XLIST.OBJ \
	$(INTDIR)/XLIST.res

$(OUTDIR)/xlist.ocx : $(OUTDIR)  $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

!ELSEIF  "$(CFG)" == "Win32 Unicode Debug"

# PROP BASE Use_MFC 2
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Classwizard_Name "xlist.clw"
# PROP Use_MFC 2
# PROP Use_Debug_Libraries 1
# PROP Classwizard_Name "xlist.clw"
# PROP Output_Dir "objdu32"
# PROP Intermediate_Dir "objdu32"
OUTDIR=.\objdu32
INTDIR=.\objdu32

ALL : MTL_TLBS $(OUTDIR)/xlist.ocx $(OUTDIR)/xlist.bsc

$(OUTDIR) : 
    if not exist $(OUTDIR)/nul mkdir $(OUTDIR)

# ADD BASE MTL /nologo /D "_DEBUG" /D "_UNICODE" /D "_WIN32" /win32
# ADD MTL /nologo /D "_DEBUG" /D "_UNICODE" /D "_WIN32" /win32
MTL_PROJ=/nologo /D "_DEBUG" /D "_UNICODE" /D "_WIN32" /win32 

MTL_TLBS : $(OUTDIR)/xlist.tlb
# ADD BASE CPP /nologo /MD /W3 /GX /Zi /Yu"stdafx.h" /Od /D "_DEBUG" /D "_UNICODE" /D "_WINDOWS" /D "_AFXDLL" /D "_AFXCTL" /D "_WINDLL" /FR /c
# SUBTRACT BASE CPP /WX
# ADD CPP /nologo /MD /W3 /GX /Zi /Yu"stdafx.h" /Od /D "_DEBUG" /D "_UNICODE" /D "_WINDOWS" /D "_AFXDLL" /D "_AFXCTL" /D "_WINDLL" /FR /c
# SUBTRACT CPP /WX
CPP_PROJ=/nologo /MD /W3 /GX /Zi /Yu"stdafx.h" /Od /D "_DEBUG" /D "_UNICODE" /D\
 "_WINDOWS" /D "_AFXDLL" /D "_AFXCTL" /D "_WINDLL" /FR$(INTDIR)/\
 /Fp$(OUTDIR)/"xlist32.pch" /Fo$(INTDIR)/ /Fd$(OUTDIR)/"xlist32.pdb" /c 
CPP_OBJS=.\objdu32/
# ADD BASE RSC /l 0x409 /i "$(OUTDIR)" /d "_DEBUG" /d "_WIN32" /d "_UNICODE"
# ADD RSC /l 0x409 /i "$(OUTDIR)" /d "_DEBUG" /d "_WIN32" /d "_UNICODE"
RSC_PROJ=/l 0x409 /fo$(INTDIR)/"xlist.res" /i "$(OUTDIR)" /d "_DEBUG" /d "_WIN32" /d\
 "_UNICODE" 
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo /o"objdu32/xlist.bsc"
# SUBTRACT BASE BSC32 /Iu
# ADD BSC32 /nologo /o"objdu32/xlist.bsc"
# SUBTRACT BSC32 /Iu
BSC32_FLAGS=/nologo /o"objdu32/xlist.bsc" 
BSC32_SBRS= \
	$(INTDIR)/STDAFX.SBR \
	$(INTDIR)/XLISTCTL.SBR \
	$(INTDIR)/XLISTPPG.SBR \
	$(INTDIR)/XLIST.SBR

$(OUTDIR)/xlist.bsc : $(OUTDIR)  $(BSC32_SBRS)
    $(BSC32) @<<
  $(BSC32_FLAGS) $(BSC32_SBRS)
<<

LINK32=link.exe
# ADD BASE LINK32 ocs30ud.lib /NOLOGO /SUBSYSTEM:windows /DLL /DEBUG /MACHINE:IX86 /OUT:"objdu32/xlist.ocx"
# SUBTRACT BASE LINK32 /PDB:none /NODEFAULTLIB
# ADD LINK32 ocs30ud.lib /NOLOGO /SUBSYSTEM:windows /DLL /DEBUG /MACHINE:IX86 /OUT:"objdu32/xlist.ocx"
# SUBTRACT LINK32 /PDB:none /NODEFAULTLIB
LINK32_FLAGS=ocs30ud.lib /NOLOGO /SUBSYSTEM:windows /DLL /INCREMENTAL:yes\
 /DEBUG /MACHINE:IX86 /DEF:".\xlist32.DEF"\
 /OUT:"objdu32/xlist.ocx" /IMPLIB:$(OUTDIR)/"xlist.lib" 
DEF_FILE=.\xlist32.DEF
LINK32_OBJS= \
	$(INTDIR)/STDAFX.OBJ \
	$(INTDIR)/XLISTCTL.OBJ \
	$(INTDIR)/XLISTPPG.OBJ \
	$(INTDIR)/XLIST.OBJ \
	$(INTDIR)/XLIST.res

$(OUTDIR)/xlist.ocx : $(OUTDIR)  $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

!ENDIF 

.c{$(CPP_OBJS)}.obj:
   $(CPP) $(CPP_PROJ) $<  

.cpp{$(CPP_OBJS)}.obj:
   $(CPP) $(CPP_PROJ) $<  

.cxx{$(CPP_OBJS)}.obj:
   $(CPP) $(CPP_PROJ) $<  

################################################################################
# Begin Group "Source Files"

################################################################################
# Begin Source File

SOURCE=.\STDAFX.CPP
DEP_STDAF=

!IF  "$(CFG)" == "Win32 ANSI Release"

# ADD BASE CPP /Yc"STDAFX.H"
# ADD CPP /Yc"STDAFX.H"

$(INTDIR)/STDAFX.OBJ :  $(SOURCE)  $(DEP_STDAF) $(INTDIR)
   $(CPP) /nologo /MD /W3 /GX /Ox /D "NDEBUG" /D "_MBCS" /D "_WINDOWS" /D\
 "_AFXDLL" /D "_AFXCTL" /D "_WINDLL" /FR$(INTDIR)/\
 /Fp$(OUTDIR)/"xlist32.pch" /Yc"STDAFX.H" /Fo$(INTDIR)/ /c  $(SOURCE) 

!ELSEIF  "$(CFG)" == "Win32 ANSI Debug"

# ADD BASE CPP /Yc"STDAFX.H"
# ADD CPP /Yc"STDAFX.H"

$(INTDIR)/STDAFX.OBJ :  $(SOURCE)  $(DEP_STDAF) $(INTDIR)
   $(CPP) /nologo /MD /W3 /GX /Zi /Od /D "_DEBUG" /D "_MBCS" /D "_WINDOWS" /D\
 "_AFXDLL" /D "_AFXCTL" /D "_WINDLL" /FR$(INTDIR)/\
 /Fp$(OUTDIR)/"xlist32.pch" /Yc"STDAFX.H" /Fo$(INTDIR)/ /Fd$(OUTDIR)/"xlist32.pdb"\
 /c  $(SOURCE) 

!ELSEIF  "$(CFG)" == "Win32 Unicode Release"

# ADD BASE CPP /Yc"STDAFX.H"
# ADD CPP /Yc"STDAFX.H"

$(INTDIR)/STDAFX.OBJ :  $(SOURCE)  $(DEP_STDAF) $(INTDIR)
   $(CPP) /nologo /MD /W3 /GX /Ox /D "NDEBUG" /D "_UNICODE" /D "_WINDOWS" /D\
 "_AFXDLL" /D "_AFXCTL" /D "_WINDLL" /FR$(INTDIR)/\
 /Fp$(OUTDIR)/"xlist32.pch" /Yc"STDAFX.H" /Fo$(INTDIR)/ /c  $(SOURCE) 

!ELSEIF  "$(CFG)" == "Win32 Unicode Debug"

# ADD BASE CPP /Yc"STDAFX.H"
# ADD CPP /Yc"STDAFX.H"

$(INTDIR)/STDAFX.OBJ :  $(SOURCE)  $(DEP_STDAF) $(INTDIR)
   $(CPP) /nologo /MD /W3 /GX /Zi /Od /D "_DEBUG" /D "_UNICODE" /D "_WINDOWS"\
 /D "_AFXDLL" /D "_AFXCTL" /D "_WINDLL" /FR$(INTDIR)/\
 /Fp$(OUTDIR)/"xlist32.pch" /Yc"STDAFX.H" /Fo$(INTDIR)/ /Fd$(OUTDIR)/"xlist32.pdb"\
 /c  $(SOURCE) 

!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\XLIST.CPP
DEP_XLIST_=

!IF  "$(CFG)" == "Win32 ANSI Release"

$(INTDIR)/XLIST.OBJ :  $(SOURCE)  $(DEP_XLIST_) $(INTDIR) $(INTDIR)/STDAFX.OBJ

!ELSEIF  "$(CFG)" == "Win32 ANSI Debug"

$(INTDIR)/XLIST.OBJ :  $(SOURCE)  $(DEP_XLIST_) $(INTDIR) $(INTDIR)/STDAFX.OBJ

!ELSEIF  "$(CFG)" == "Win32 Unicode Release"

$(INTDIR)/XLIST.OBJ :  $(SOURCE)  $(DEP_XLIST_) $(INTDIR) $(INTDIR)/STDAFX.OBJ

!ELSEIF  "$(CFG)" == "Win32 Unicode Debug"

$(INTDIR)/XLIST.OBJ :  $(SOURCE)  $(DEP_XLIST_) $(INTDIR) $(INTDIR)/STDAFX.OBJ

!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\XLISTCTL.CPP
DEP_XLISTCTL=

!IF  "$(CFG)" == "Win32 ANSI Release"

$(INTDIR)/XLISTCTL.OBJ :  $(SOURCE)  $(DEP_XLISTCTL) $(INTDIR) $(INTDIR)/STDAFX.OBJ

!ELSEIF  "$(CFG)" == "Win32 ANSI Debug"

$(INTDIR)/XLISTCTL.OBJ :  $(SOURCE)  $(DEP_XLISTCTL) $(INTDIR) $(INTDIR)/STDAFX.OBJ

!ELSEIF  "$(CFG)" == "Win32 Unicode Release"

$(INTDIR)/XLISTCTL.OBJ :  $(SOURCE)  $(DEP_XLISTCTL) $(INTDIR) $(INTDIR)/STDAFX.OBJ

!ELSEIF  "$(CFG)" == "Win32 Unicode Debug"

$(INTDIR)/XLISTCTL.OBJ :  $(SOURCE)  $(DEP_XLISTCTL) $(INTDIR) $(INTDIR)/STDAFX.OBJ

!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\XLISTPPG.CPP
DEP_XLISTPPG=

!IF  "$(CFG)" == "Win32 ANSI Release"

$(INTDIR)/XLISTPPG.OBJ :  $(SOURCE)  $(DEP_XLISTPPG) $(INTDIR) $(INTDIR)/STDAFX.OBJ

!ELSEIF  "$(CFG)" == "Win32 ANSI Debug"

$(INTDIR)/XLISTPPG.OBJ :  $(SOURCE)  $(DEP_XLISTPPG) $(INTDIR) $(INTDIR)/STDAFX.OBJ

!ELSEIF  "$(CFG)" == "Win32 Unicode Release"

$(INTDIR)/XLISTPPG.OBJ :  $(SOURCE)  $(DEP_XLISTPPG) $(INTDIR) $(INTDIR)/STDAFX.OBJ

!ELSEIF  "$(CFG)" == "Win32 Unicode Debug"

$(INTDIR)/XLISTPPG.OBJ :  $(SOURCE)  $(DEP_XLISTPPG) $(INTDIR) $(INTDIR)/STDAFX.OBJ

!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\xlist32.DEF
# End Source File
################################################################################
# Begin Source File

SOURCE=.\XLIST.RC
DEP_XLIST_R=

!IF  "$(CFG)" == "Win32 ANSI Release"

$(INTDIR)/XLIST.res :  $(SOURCE)  $(DEP_XLIST_R) $(INTDIR)
   $(RSC) /l 0x409 /fo$(INTDIR)/"XLIST.res" /i "$(OUTDIR)" /d "NDEBUG"\
 /d "_WIN32"  $(SOURCE) 

!ELSEIF  "$(CFG)" == "Win32 ANSI Debug"

$(INTDIR)/XLIST.res :  $(SOURCE)  $(DEP_XLIST_R) $(INTDIR)
   $(RSC) /l 0x409 /fo$(INTDIR)/"XLIST.res" /i "$(OUTDIR)" /d "_DEBUG"\
 /d "_WIN32"  $(SOURCE) 

!ELSEIF  "$(CFG)" == "Win32 Unicode Release"

$(INTDIR)/XLIST.res :  $(SOURCE)  $(DEP_XLIST_R) $(INTDIR)
   $(RSC) /l 0x409 /fo$(INTDIR)/"XLIST.res" /i "$(OUTDIR)" /d "NDEBUG"\
 /d "_WIN32" /d "_UNICODE"  $(SOURCE) 

!ELSEIF  "$(CFG)" == "Win32 Unicode Debug"

$(INTDIR)/XLIST.res :  $(SOURCE)  $(DEP_XLIST_R) $(INTDIR)
   $(RSC) /l 0x409 /fo$(INTDIR)/"XLIST.res" /i "$(OUTDIR)" /d\
 "_DEBUG" /d "_WIN32" /d "_UNICODE"  $(SOURCE) 

!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\XLIST.ODL
DEP_XLIST_O=

!IF  "$(CFG)" == "Win32 ANSI Release"

$(OUTDIR)/XLIST.tlb :  $(SOURCE)  $(DEP_XLIST_O) $(OUTDIR)
   $(MTL) /nologo /D "NDEBUG" /D "_WIN32" /tlb $(OUTDIR)/"XLIST.tlb" \
   /win32  $(SOURCE) 

!ELSEIF  "$(CFG)" == "Win32 ANSI Debug"

$(OUTDIR)/XLIST.tlb :  $(SOURCE)  $(DEP_XLIST_O) $(OUTDIR)
   $(MTL) /nologo /D "_DEBUG" /D "_WIN32" /tlb $(OUTDIR)/"XLIST.tlb" \
   /win32  $(SOURCE) 

!ELSEIF  "$(CFG)" == "Win32 Unicode Release"

$(OUTDIR)/XLIST.tlb :  $(SOURCE)  $(DEP_XLIST_O) $(OUTDIR)
   $(MTL) /nologo /D "NDEBUG" /D "_UNICODE" /D "_WIN32" /tlb \
   $(OUTDIR)/"XLIST.tlb" /win32 $(SOURCE) 

!ELSEIF  "$(CFG)" == "Win32 Unicode Debug"

$(OUTDIR)/XLIST.tlb :  $(SOURCE)  $(DEP_XLIST_O) $(OUTDIR)
   $(MTL) /nologo /D "_DEBUG" /D "_UNICODE" /D "_WIN32" /tlb \
   $(OUTDIR)/"XLIST.tlb" /win32 $(SOURCE) 

!ENDIF 

# End Source File
# End Group
# End Project
################################################################################
