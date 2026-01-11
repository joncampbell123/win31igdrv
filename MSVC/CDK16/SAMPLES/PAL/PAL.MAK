# Microsoft Visual C++ generated build script - Do not modify

PROJ = PAL
DEBUG = 1
PROGTYPE = 1
CALLER =
ARGS =
DLLS =
D_RCDEFINES = -d_DEBUG
R_RCDEFINES = -dNDEBUG
ORIGIN = MSVC
ORIGIN_VER = 1.00
PROJPATH = UNKNOWN
USEMFC = 0
CC = cl
CPP = cl
CXX = cl
CCREATEPCHFLAG = 
CPPCREATEPCHFLAG = /YcSTDAFX.H
CUSEPCHFLAG = 
CPPUSEPCHFLAG = /YuSTDAFX.H
FIRSTC =             
FIRSTCPP = STDAFX.CPP  
RC = rc
CFLAGS_D_WDLL = /nologo /G2 /Zp1 /W3 /WX /Zi /ALw /Od /D "_DEBUG" /D "_AFXDLL" /D "_WINDLL" /D "_AFXCTL" /FR /GD /Fd"PAL.PDB" /GEf
CFLAGS_R_WDLL = /nologo /Gs /G2 /Zp1 /W3 /ALw /Ox /D "NDEBUG" /D "_AFXDLL" /D "_WINDLL" /D "_AFXCTL" /FR /GD /GEf
LFLAGS_D_WDLL = /NOLOGO /ONERROR:NOEXE /NOD /NOE /PACKC:61440 /CO /ALIGN:16
LFLAGS_R_WDLL = /NOLOGO /ONERROR:NOEXE /NOD /NOE /PACKC:61440 /ALIGN:16
LIBS_D_WDLL = ocs25d oc25d libw ldllcew compobj storage ole2 ole2disp typelib oldnames commdlg.lib shell.lib 
LIBS_R_WDLL = ocs25 oc25 libw ldllcew compobj storage ole2 ole2disp typelib oldnames commdlg.lib shell.lib 
RCFLAGS = /nologo /i tlb16
RESFLAGS = /nologo
RUNFLAGS = 
DEFFILE = PAL.DEF
OBJS_EXT = 
LIBS_EXT = 
!if "$(DEBUG)" == "1"
CFLAGS = $(CFLAGS_D_WDLL)
LFLAGS = $(LFLAGS_D_WDLL)
LIBS = $(LIBS_D_WDLL)
MAPFILE = nul
RCDEFINES = $(D_RCDEFINES)
!else
CFLAGS = $(CFLAGS_R_WDLL)
LFLAGS = $(LFLAGS_R_WDLL)
LIBS = $(LIBS_R_WDLL)
MAPFILE = nul
RCDEFINES = $(R_RCDEFINES)
!endif
!if [if exist MSVC.BND del MSVC.BND]
!endif
SBRS = STDAFX.SBR \
		PAL.SBR \
		PALCTL.SBR \
		PALPPG.SBR


all:	$(PROJ).DLL $(PROJ).BSC

PAL.RES:	PAL.RC $(PAL_RCDEP)
	$(RC) $(RCFLAGS) $(RCDEFINES) -r PAL.RC

STDAFX.OBJ:	STDAFX.CPP $(STDAFX_DEP)
	$(CPP) $(CFLAGS) $(CPPCREATEPCHFLAG) /c STDAFX.CPP

PAL.OBJ:	PAL.CPP $(PAL_DEP)
	$(CPP) $(CFLAGS) $(CPPUSEPCHFLAG) /c PAL.CPP

PALCTL.OBJ:	PALCTL.CPP $(PALCTL_DEP)
	$(CPP) $(CFLAGS) $(CPPUSEPCHFLAG) /c PALCTL.CPP

PALPPG.OBJ:	PALPPG.CPP $(PALPPG_DEP)
	$(CPP) $(CFLAGS) $(CPPUSEPCHFLAG) /c PALPPG.CPP


$(PROJ).DLL::	PAL.RES

$(PROJ).DLL::	STDAFX.OBJ PAL.OBJ PALCTL.OBJ PALPPG.OBJ $(OBJS_EXT) $(DEFFILE)
	echo >NUL @<<$(PROJ).CRF
STDAFX.OBJ +
PAL.OBJ +
PALCTL.OBJ +
PALPPG.OBJ +
$(OBJS_EXT)
$(PROJ).DLL
$(MAPFILE)
$(LIBS)
$(DEFFILE);
<<
	link $(LFLAGS) @$(PROJ).CRF
	$(RC) $(RESFLAGS) PAL.RES $@
	@copy $(PROJ).CRF MSVC.BND
	implib /nowep $(PROJ).LIB $(PROJ).DLL

$(PROJ).DLL::	PAL.RES
	if not exist MSVC.BND 	$(RC) $(RESFLAGS) PAL.RES $@

run: $(PROJ).DLL
	$(PROJ) $(RUNFLAGS)


$(PROJ).BSC: $(SBRS)
	bscmake @<<
/o$@ $(SBRS)
<<
