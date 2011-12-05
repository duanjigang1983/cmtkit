# Microsoft Developer Studio Generated NMAKE File, Based on main.dsp
!IF "$(CFG)" == ""
CFG=main - Win32 Debug
!MESSAGE No configuration specified. Defaulting to main - Win32 Debug.
!ENDIF 

!IF "$(CFG)" != "main - Win32 Release" && "$(CFG)" != "main - Win32 Debug"
!MESSAGE Invalid configuration "$(CFG)" specified.
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "main.mak" CFG="main - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "main - Win32 Release" (based on "Win32 (x86) Console Application")
!MESSAGE "main - Win32 Debug" (based on "Win32 (x86) Console Application")
!MESSAGE 
!ERROR An invalid configuration is specified.
!ENDIF 

!IF "$(OS)" == "Windows_NT"
NULL=
!ELSE 
NULL=nul
!ENDIF 

!IF  "$(CFG)" == "main - Win32 Release"

OUTDIR=.\Release
INTDIR=.\Release

ALL : ".\cmtk.exe"


CLEAN :
	-@erase "$(INTDIR)\cm_function.obj"
	-@erase "$(INTDIR)\cm_init.obj"
	-@erase "$(INTDIR)\cm_init_dev.obj"
	-@erase "$(INTDIR)\cm_init_task.obj"
	-@erase "$(INTDIR)\cmtkp.obj"
	-@erase "$(INTDIR)\getopt.obj"
	-@erase "$(INTDIR)\IniHelper.obj"
	-@erase "$(INTDIR)\main.obj"
	-@erase "$(INTDIR)\NetHelper.obj"
	-@erase "$(INTDIR)\vc60.idb"
	-@erase ".\cmtk.exe"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

CPP=cl.exe
CPP_PROJ=/nologo /MD /W3 /GR /GX /O2 /D "WIN32" /D "NDEBUG" /D "_CONSOLE" /D "_MBCS" /D "_AFXDLL" /Fp"$(INTDIR)\main.pch" /YX /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 

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

RSC=rc.exe
BSC32=bscmake.exe
BSC32_FLAGS=/nologo /o"$(OUTDIR)\main.bsc" 
BSC32_SBRS= \
	
LINK32=link.exe
LINK32_FLAGS=ice.lib iceutil.lib /nologo /subsystem:console /incremental:no /pdb:"$(OUTDIR)\cmtk.pdb" /machine:I386 /out:"cmtk.exe" 
LINK32_OBJS= \
	"$(INTDIR)\cm_init.obj" \
	"$(INTDIR)\cmtkp.obj" \
	"$(INTDIR)\getopt.obj" \
	"$(INTDIR)\IniHelper.obj" \
	"$(INTDIR)\main.obj" \
	"$(INTDIR)\NetHelper.obj" \
	"$(INTDIR)\cm_function.obj" \
	"$(INTDIR)\cm_init_dev.obj" \
	"$(INTDIR)\cm_init_task.obj"

".\cmtk.exe" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

!ELSEIF  "$(CFG)" == "main - Win32 Debug"

OUTDIR=.\Debug
INTDIR=.\Debug

ALL : ".\cmtkD.exe"


CLEAN :
	-@erase "$(INTDIR)\cm_function.obj"
	-@erase "$(INTDIR)\cm_init.obj"
	-@erase "$(INTDIR)\cm_init_dev.obj"
	-@erase "$(INTDIR)\cm_init_task.obj"
	-@erase "$(INTDIR)\cmtkp.obj"
	-@erase "$(INTDIR)\getopt.obj"
	-@erase "$(INTDIR)\IniHelper.obj"
	-@erase "$(INTDIR)\main.obj"
	-@erase "$(INTDIR)\NetHelper.obj"
	-@erase "$(INTDIR)\vc60.idb"
	-@erase "$(INTDIR)\vc60.pdb"
	-@erase "$(OUTDIR)\cmtkD.pdb"
	-@erase ".\cmtkD.exe"
	-@erase ".\cmtkD.ilk"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

CPP=cl.exe
CPP_PROJ=/nologo /MDd /W3 /Gm /GR /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_CONSOLE" /D "_MBCS" /D "_AFXDLL" /Fp"$(INTDIR)\main.pch" /YX /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /GZ /c 

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

RSC=rc.exe
BSC32=bscmake.exe
BSC32_FLAGS=/nologo /o"$(OUTDIR)\main.bsc" 
BSC32_SBRS= \
	
LINK32=link.exe
LINK32_FLAGS=ice.lib iceutil.lib /nologo /subsystem:console /incremental:yes /pdb:"$(OUTDIR)\cmtkD.pdb" /debug /machine:I386 /out:"cmtkD.exe" /pdbtype:sept 
LINK32_OBJS= \
	"$(INTDIR)\cm_init.obj" \
	"$(INTDIR)\cmtkp.obj" \
	"$(INTDIR)\getopt.obj" \
	"$(INTDIR)\IniHelper.obj" \
	"$(INTDIR)\main.obj" \
	"$(INTDIR)\NetHelper.obj" \
	"$(INTDIR)\cm_function.obj" \
	"$(INTDIR)\cm_init_dev.obj" \
	"$(INTDIR)\cm_init_task.obj"

".\cmtkD.exe" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

!ENDIF 


!IF "$(NO_EXTERNAL_DEPS)" != "1"
!IF EXISTS("main.dep")
!INCLUDE "main.dep"
!ELSE 
!MESSAGE Warning: cannot find "main.dep"
!ENDIF 
!ENDIF 


!IF "$(CFG)" == "main - Win32 Release" || "$(CFG)" == "main - Win32 Debug"
SOURCE=.\cm_function.cpp

"$(INTDIR)\cm_function.obj" : $(SOURCE) "$(INTDIR)"


SOURCE=.\cm_init.cpp

"$(INTDIR)\cm_init.obj" : $(SOURCE) "$(INTDIR)"


SOURCE=.\cm_init_dev.cpp

"$(INTDIR)\cm_init_dev.obj" : $(SOURCE) "$(INTDIR)"


SOURCE=.\cm_init_task.cpp

"$(INTDIR)\cm_init_task.obj" : $(SOURCE) "$(INTDIR)"


SOURCE=.\cmtkp.cpp

"$(INTDIR)\cmtkp.obj" : $(SOURCE) "$(INTDIR)"


SOURCE=.\getopt.cpp

"$(INTDIR)\getopt.obj" : $(SOURCE) "$(INTDIR)"


SOURCE=.\IniHelper.cpp

"$(INTDIR)\IniHelper.obj" : $(SOURCE) "$(INTDIR)"


SOURCE=.\main.cpp

"$(INTDIR)\main.obj" : $(SOURCE) "$(INTDIR)"


SOURCE=.\NetHelper.cpp

"$(INTDIR)\NetHelper.obj" : $(SOURCE) "$(INTDIR)"



!ENDIF 

