# Microsoft Developer Studio Generated NMAKE File, Based on GGSvr.dsp
!IF "$(CFG)" == ""
CFG=GGSvr - Win32 Debug
!MESSAGE No configuration specified. Defaulting to GGSvr - Win32 Debug.
!ENDIF 

!IF "$(CFG)" != "GGSvr - Win32 Release" && "$(CFG)" != "GGSvr - Win32 Debug"
!MESSAGE Invalid configuration "$(CFG)" specified.
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "GGSvr.mak" CFG="GGSvr - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "GGSvr - Win32 Release" (based on "Win32 (x86) Static Library")
!MESSAGE "GGSvr - Win32 Debug" (based on "Win32 (x86) Static Library")
!MESSAGE 
!ERROR An invalid configuration is specified.
!ENDIF 

!IF "$(OS)" == "Windows_NT"
NULL=
!ELSE 
NULL=nul
!ENDIF 

CPP=cl.exe
RSC=rc.exe

!IF  "$(CFG)" == "GGSvr - Win32 Release"

OUTDIR=.\Release
INTDIR=.\Release
# Begin Custom Macros
OutDir=.\Release
# End Custom Macros

ALL : "$(OUTDIR)\GGSvr.lib"


CLEAN :
	-@erase "$(INTDIR)\CSAuth2.obj"
	-@erase "$(INTDIR)\GGSvr.obj"
	-@erase "$(INTDIR)\GGSvr.pch"
	-@erase "$(INTDIR)\StdAfx.obj"
	-@erase "$(INTDIR)\vc60.idb"
	-@erase "$(OUTDIR)\GGSvr.lib"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

CPP_PROJ=/nologo /MT /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_MBCS" /D "_LIB" /D "GGSVR_EXPORTS" /Fp"$(INTDIR)\GGSvr.pch" /Yu"stdafx.h" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 
BSC32=bscmake.exe
BSC32_FLAGS=/nologo /o"$(OUTDIR)\GGSvr.bsc" 
BSC32_SBRS= \
	
LIB32=link.exe -lib
LIB32_FLAGS=/nologo /out:"$(OUTDIR)\GGSvr.lib" 
LIB32_OBJS= \
	"$(INTDIR)\CSAuth2.obj" \
	"$(INTDIR)\GGSvr.obj" \
	"$(INTDIR)\StdAfx.obj"

"$(OUTDIR)\GGSvr.lib" : "$(OUTDIR)" $(DEF_FILE) $(LIB32_OBJS)
    $(LIB32) @<<
  $(LIB32_FLAGS) $(DEF_FLAGS) $(LIB32_OBJS)
<<

!ELSEIF  "$(CFG)" == "GGSvr - Win32 Debug"

OUTDIR=.\Debug
INTDIR=.\Debug
# Begin Custom Macros
OutDir=.\Debug
# End Custom Macros

ALL : "$(OUTDIR)\GGSvr.lib"


CLEAN :
	-@erase "$(INTDIR)\CSAuth2.obj"
	-@erase "$(INTDIR)\GGSvr.obj"
	-@erase "$(INTDIR)\GGSvr.pch"
	-@erase "$(INTDIR)\StdAfx.obj"
	-@erase "$(INTDIR)\vc60.idb"
	-@erase "$(INTDIR)\vc60.pdb"
	-@erase "$(OUTDIR)\GGSvr.lib"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

CPP_PROJ=/nologo /MLd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_MBCS" /D "_LIB" /Fp"$(INTDIR)\GGSvr.pch" /Yu"stdafx.h" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /GZ /c 
BSC32=bscmake.exe
BSC32_FLAGS=/nologo /o"$(OUTDIR)\GGSvr.bsc" 
BSC32_SBRS= \
	
LIB32=link.exe -lib
LIB32_FLAGS=/nologo /out:"$(OUTDIR)\GGSvr.lib" 
LIB32_OBJS= \
	"$(INTDIR)\CSAuth2.obj" \
	"$(INTDIR)\GGSvr.obj" \
	"$(INTDIR)\StdAfx.obj"

"$(OUTDIR)\GGSvr.lib" : "$(OUTDIR)" $(DEF_FILE) $(LIB32_OBJS)
    $(LIB32) @<<
  $(LIB32_FLAGS) $(DEF_FLAGS) $(LIB32_OBJS)
<<

!ENDIF 

.c{$(INTDIR)}.obj::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.cpp{$(INTDIR)}.obj::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.cxx{$(INTDIR)}.obj::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.c{$(INTDIR)}.sbr::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.cpp{$(INTDIR)}.sbr::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.cxx{$(INTDIR)}.sbr::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<


!IF "$(NO_EXTERNAL_DEPS)" != "1"
!IF EXISTS("GGSvr.dep")
!INCLUDE "GGSvr.dep"
!ELSE 
!MESSAGE Warning: cannot find "GGSvr.dep"
!ENDIF 
!ENDIF 


!IF "$(CFG)" == "GGSvr - Win32 Release" || "$(CFG)" == "GGSvr - Win32 Debug"
SOURCE=.\CSAuth2.cpp

"$(INTDIR)\CSAuth2.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\GGSvr.pch"


SOURCE=.\GGSvr.cpp

"$(INTDIR)\GGSvr.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\GGSvr.pch"


SOURCE=.\StdAfx.cpp

!IF  "$(CFG)" == "GGSvr - Win32 Release"

CPP_SWITCHES=/nologo /MT /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_MBCS" /D "_LIB" /D "GGSVR_EXPORTS" /Fp"$(INTDIR)\GGSvr.pch" /Yc"stdafx.h" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 

"$(INTDIR)\StdAfx.obj"	"$(INTDIR)\GGSvr.pch" : $(SOURCE) "$(INTDIR)"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ELSEIF  "$(CFG)" == "GGSvr - Win32 Debug"

CPP_SWITCHES=/nologo /MLd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_MBCS" /D "_LIB" /Fp"$(INTDIR)\GGSvr.pch" /Yc"stdafx.h" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /GZ /c 

"$(INTDIR)\StdAfx.obj"	"$(INTDIR)\GGSvr.pch" : $(SOURCE) "$(INTDIR)"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ENDIF 


!ENDIF 

