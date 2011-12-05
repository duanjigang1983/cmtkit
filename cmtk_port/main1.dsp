# Microsoft Developer Studio Project File - Name="main1" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) External Target" 0x0106

CFG=main1 - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "main1.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "main1.mak" CFG="main1 - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "main1 - Win32 Release" (based on "Win32 (x86) External Target")
!MESSAGE "main1 - Win32 Debug" (based on "Win32 (x86) External Target")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""

!IF  "$(CFG)" == "main1 - Win32 Release"

# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Cmd_Line "NMAKE /f main.mak"
# PROP BASE Rebuild_Opt "/a"
# PROP BASE Target_File "main.exe"
# PROP BASE Bsc_Name "main.bsc"
# PROP BASE Target_Dir ""
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Release"
# PROP Intermediate_Dir "Release"
# PROP Cmd_Line "NMAKE /f main.mak"
# PROP Rebuild_Opt "/a"
# PROP Target_File "main1.exe"
# PROP Bsc_Name "main1.bsc"
# PROP Target_Dir ""

!ELSEIF  "$(CFG)" == "main1 - Win32 Debug"

# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Cmd_Line "NMAKE /f main.mak"
# PROP BASE Rebuild_Opt "/a"
# PROP BASE Target_File "main.exe"
# PROP BASE Bsc_Name "main.bsc"
# PROP BASE Target_Dir ""
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "Debug"
# PROP Intermediate_Dir "Debug"
# PROP Cmd_Line "NMAKE /f main.mak"
# PROP Rebuild_Opt "/a"
# PROP Target_File "main1.exe"
# PROP Bsc_Name "main1.bsc"
# PROP Target_Dir ""

!ENDIF 

# Begin Target

# Name "main1 - Win32 Release"
# Name "main1 - Win32 Debug"

!IF  "$(CFG)" == "main1 - Win32 Release"

!ELSEIF  "$(CFG)" == "main1 - Win32 Debug"

!ENDIF 

# Begin Source File

SOURCE=.\cm_function.cpp
# End Source File
# Begin Source File

SOURCE=.\cm_init.cpp
# End Source File
# Begin Source File

SOURCE=.\cm_init_dev.cpp
# End Source File
# Begin Source File

SOURCE=.\cm_init_task.cpp
# End Source File
# Begin Source File

SOURCE=.\cm_result.cpp
# End Source File
# Begin Source File

SOURCE=.\cm_thread.cpp
# End Source File
# Begin Source File

SOURCE=.\cm_utils.cpp
# End Source File
# Begin Source File

SOURCE=.\cmconfig.h
# End Source File
# Begin Source File

SOURCE=.\cmtkp.cpp
# End Source File
# Begin Source File

SOURCE=.\getopt.cpp
# End Source File
# Begin Source File

SOURCE=.\IniHelper.cpp
# End Source File
# Begin Source File

SOURCE=.\main.cpp
# End Source File
# Begin Source File

SOURCE=.\main.dsp
# End Source File
# Begin Source File

SOURCE=.\NetHelper.cpp
# End Source File
# End Target
# End Project
