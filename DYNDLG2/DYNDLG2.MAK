# Microsoft Visual C++ generated build script - Do not modify

PROJ = DYNDLG2
DEBUG = 1
PROGTYPE = 0
CALLER = 
ARGS = 
DLLS = 
D_RCDEFINES = -d_DEBUG
R_RCDEFINES = -dNDEBUG
ORIGIN = MSVC
ORIGIN_VER = 1.00
PROJPATH = A:\DYNDLG\SAMPLES\DYNDLG2\
USEMFC = 0
CC = cl
CPP = cl
CXX = cl
CCREATEPCHFLAG = 
CPPCREATEPCHFLAG = 
CUSEPCHFLAG = 
CPPUSEPCHFLAG = 
FIRSTC = DEMODLG.C   
FIRSTCPP =             
RC = rc
CFLAGS_D_WEXE = /nologo /W3 /FR /G2 /Zi /D_DEBUG /Od /AM /GA /Fd"DYNDLG2.PDB"
CFLAGS_R_WEXE = /nologo /W3 /FR /O1 /DNDEBUG /AM /GA
LFLAGS_D_WEXE = /NOLOGO /NOD /PACKC:61440 /ALIGN:16 /ONERROR:NOEXE /CO  
LFLAGS_R_WEXE = /NOLOGO /NOD /PACKC:61440 /ALIGN:16 /ONERROR:NOEXE  
LIBS_D_WEXE = oldnames libw mlibcew commdlg.lib olecli.lib olesvr.lib shell.lib 
LIBS_R_WEXE = oldnames libw mlibcew commdlg.lib olecli.lib olesvr.lib shell.lib 
RCFLAGS = /nologo
RESFLAGS = /nologo
RUNFLAGS = 
DEFFILE = DYNDLG2.DEF
OBJS_EXT = 
LIBS_EXT = 
!if "$(DEBUG)" == "1"
CFLAGS = $(CFLAGS_D_WEXE)
LFLAGS = $(LFLAGS_D_WEXE)
LIBS = $(LIBS_D_WEXE)
MAPFILE = nul
RCDEFINES = $(D_RCDEFINES)
!else
CFLAGS = $(CFLAGS_R_WEXE)
LFLAGS = $(LFLAGS_R_WEXE)
LIBS = $(LIBS_R_WEXE)
MAPFILE = nul
RCDEFINES = $(R_RCDEFINES)
!endif
!if [if exist MSVC.BND del MSVC.BND]
!endif
SBRS = DEMODLG.SBR \
		DLGCAT.SBR \
		DLGSEC.SBR


DEMODLG_DEP = a:\dyndlg\samples\dyndlg2\winxext.h \
	a:\dyndlg\samples\dyndlg2\dyndlg.h \
	a:\dyndlg\samples\dyndlg2\dlgcat.h \
	a:\dyndlg\samples\dyndlg2\dlgres.h \
	a:\dyndlg\samples\dyndlg2\dlgsec.h \
	a:\dyndlg\samples\dyndlg2\ids.h


DLGCAT_DEP = a:\dyndlg\samples\dyndlg2\debug.h \
	a:\dyndlg\samples\dyndlg2\dlgcat.h \
	a:\dyndlg\samples\dyndlg2\dlgres.h \
	a:\dyndlg\samples\dyndlg2\winxext.h


DLGSEC_DEP = a:\dyndlg\samples\dyndlg2\debug.h \
	a:\dyndlg\samples\dyndlg2\dyndlg.h \
	a:\dyndlg\samples\dyndlg2\winxext.h \
	a:\dyndlg\samples\dyndlg2\dlgcat.h \
	a:\dyndlg\samples\dyndlg2\dlgres.h \
	a:\dyndlg\samples\dyndlg2\dlgsec.h


DYNDLG2_RCDEP = 

all:	$(PROJ).EXE $(PROJ).BSC

DEMODLG.OBJ:	DEMODLG.C $(DEMODLG_DEP)
	$(CC) $(CFLAGS) $(CCREATEPCHFLAG) /c DEMODLG.C

DLGCAT.OBJ:	DLGCAT.C $(DLGCAT_DEP)
	$(CC) $(CFLAGS) $(CUSEPCHFLAG) /c DLGCAT.C

DLGSEC.OBJ:	DLGSEC.C $(DLGSEC_DEP)
	$(CC) $(CFLAGS) $(CUSEPCHFLAG) /c DLGSEC.C

DYNDLG2.RES:	DYNDLG2.RC $(DYNDLG2_RCDEP)
	$(RC) $(RCFLAGS) $(RCDEFINES) -r DYNDLG2.RC


$(PROJ).EXE::	DYNDLG2.RES

$(PROJ).EXE::	DEMODLG.OBJ DLGCAT.OBJ DLGSEC.OBJ $(OBJS_EXT) $(DEFFILE)
	echo >NUL @<<$(PROJ).CRF
DEMODLG.OBJ +
DLGCAT.OBJ +
DLGSEC.OBJ +
$(OBJS_EXT)
$(PROJ).EXE
$(MAPFILE)
c:\msvc\lib\+
c:\ole2\rel\+
c:\ole2\samp\rel\+
$(LIBS)
$(DEFFILE);
<<
	link $(LFLAGS) @$(PROJ).CRF
	$(RC) $(RESFLAGS) DYNDLG2.RES $@
	@copy $(PROJ).CRF MSVC.BND

$(PROJ).EXE::	DYNDLG2.RES
	if not exist MSVC.BND 	$(RC) $(RESFLAGS) DYNDLG2.RES $@

run: $(PROJ).EXE
	$(PROJ) $(RUNFLAGS)


$(PROJ).BSC: $(SBRS)
	bscmake @<<
/o$@ $(SBRS)
<<
