# Microsoft Developer Studio Project File - Name="chuck" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Console Application" 0x0103

CFG=chuck - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "chuck.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "chuck.mak" CFG="chuck - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "chuck - Win32 Release" (based on "Win32 (x86) Console Application")
!MESSAGE "chuck - Win32 Debug" (based on "Win32 (x86) Console Application")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
RSC=rc.exe

!IF  "$(CFG)" == "chuck - Win32 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Release"
# PROP Intermediate_Dir "Release"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_CONSOLE" /D "_MBCS" /YX /FD /c
# ADD CPP /nologo /MT /W3 /GX /O2 /D "NDEBUG" /D "__WINDOWS_DS__" /D "MSDOS" /D "__PLATFORM_WIN32__" /D "WIN32" /D "_CONSOLE" /D "_MBCS" /YX /FD /c
# ADD BASE RSC /l 0x409 /d "NDEBUG"
# ADD RSC /l 0x409 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /machine:I386
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib wsock32.lib winmm.lib dsound.lib /nologo /subsystem:console /machine:I386
# Begin Special Build Tool
SOURCE="$(InputPath)"
PostBuild_Desc=copying Release\chuck.exe to current directory
PostBuild_Cmds=copy Release\chuck.exe .
# End Special Build Tool

!ELSEIF  "$(CFG)" == "chuck - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "Debug"
# PROP Intermediate_Dir "Debug"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_CONSOLE" /D "_MBCS" /YX /FD /GZ /c
# ADD CPP /nologo /MTd /W3 /Gm /GX /ZI /Od /D "_DEBUG" /D "__WINDOWS_DS__" /D "MSDOS" /D "__PLATFORM_WIN32__" /D "WIN32" /D "_CONSOLE" /D "_MBCS" /YX /FD /GZ /c
# ADD BASE RSC /l 0x409 /d "_DEBUG"
# ADD RSC /l 0x409 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /debug /machine:I386 /pdbtype:sept
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib wsock32.lib winmm.lib dsound.lib /nologo /subsystem:console /debug /machine:I386 /pdbtype:sept
# Begin Special Build Tool
SOURCE="$(InputPath)"
PostBuild_Desc=Copying Debug\chuck.exe to current directory
PostBuild_Cmds=copy Debug\chuck.exe .
# End Special Build Tool

!ENDIF 

# Begin Target

# Name "chuck - Win32 Release"
# Name "chuck - Win32 Debug"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=chuck.tab.c
# End Source File
# Begin Source File

SOURCE=chuck.yy.c
# End Source File
# Begin Source File

SOURCE=chuck_absyn.cpp
# End Source File
# Begin Source File

SOURCE=chuck_bbq.cpp
# End Source File
# Begin Source File

SOURCE=chuck_dl.cpp
# End Source File
# Begin Source File

SOURCE=chuck_emit.cpp
# End Source File
# Begin Source File

SOURCE=chuck_errmsg.cpp
# End Source File
# Begin Source File

SOURCE=chuck_frame.cpp
# End Source File
# Begin Source File

SOURCE=chuck_instr.cpp
# End Source File
# Begin Source File

SOURCE=chuck_main.cpp
# End Source File
# Begin Source File

SOURCE=chuck_symbol.cpp
# End Source File
# Begin Source File

SOURCE=chuck_table.cpp
# End Source File
# Begin Source File

SOURCE=chuck_temp.cpp
# End Source File
# Begin Source File

SOURCE=chuck_type.cpp
# End Source File
# Begin Source File

SOURCE=chuck_ugen.cpp
# End Source File
# Begin Source File

SOURCE=chuck_utils.cpp
# End Source File
# Begin Source File

SOURCE=chuck_vm.cpp
# End Source File
# Begin Source File

SOURCE=digiio_rtaudio.cpp
# End Source File
# Begin Source File

SOURCE=midiio_win32.cpp
# End Source File
# Begin Source File

SOURCE=rtaudio.cpp
# End Source File
# Begin Source File

SOURCE=ugen_filter.cpp
# End Source File
# Begin Source File

SOURCE=ugen_osc.cpp
# End Source File
# Begin Source File

SOURCE=ugen_stk.cpp
# End Source File
# Begin Source File

SOURCE=ugen_xxx.cpp
# End Source File
# Begin Source File

SOURCE=ulib_machine.cpp
# End Source File
# Begin Source File

SOURCE=ulib_math.cpp
# End Source File
# Begin Source File

SOURCE=ulib_net.cpp
# End Source File
# Begin Source File

SOURCE=ulib_std.cpp
# End Source File
# Begin Source File

SOURCE=util_buffers.cpp
# End Source File
# Begin Source File

SOURCE=util_math.c
# End Source File
# Begin Source File

SOURCE=util_network.c
# End Source File
# Begin Source File

SOURCE=util_raw.c
# End Source File
# Begin Source File

SOURCE=util_sndfile.c
# End Source File
# Begin Source File

SOURCE=util_xforms.c
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=chuck.tab.h
# End Source File
# Begin Source File

SOURCE=chuck_absyn.h
# End Source File
# Begin Source File

SOURCE=chuck_bbq.h
# End Source File
# Begin Source File

SOURCE=chuck_def.h
# End Source File
# Begin Source File

SOURCE=chuck_dl.h
# End Source File
# Begin Source File

SOURCE=chuck_emit.h
# End Source File
# Begin Source File

SOURCE=chuck_errmsg.h
# End Source File
# Begin Source File

SOURCE=chuck_frame.h
# End Source File
# Begin Source File

SOURCE=chuck_instr.h
# End Source File
# Begin Source File

SOURCE=chuck_oo.h
# End Source File
# Begin Source File

SOURCE=chuck_symbol.h
# End Source File
# Begin Source File

SOURCE=chuck_table.h
# End Source File
# Begin Source File

SOURCE=chuck_temp.h
# End Source File
# Begin Source File

SOURCE=chuck_type.h
# End Source File
# Begin Source File

SOURCE=chuck_ugen.h
# End Source File
# Begin Source File

SOURCE=chuck_utils.h
# End Source File
# Begin Source File

SOURCE=chuck_vm.h
# End Source File
# Begin Source File

SOURCE=digiio_rtaudio.h
# End Source File
# Begin Source File

SOURCE=midiio_alsa.h
# End Source File
# Begin Source File

SOURCE=midiio_oss.h
# End Source File
# Begin Source File

SOURCE=midiio_osx.h
# End Source File
# Begin Source File

SOURCE=midiio_win32.h
# End Source File
# Begin Source File

SOURCE=rtaudio.h
# End Source File
# Begin Source File

SOURCE=rterror.h
# End Source File
# Begin Source File

SOURCE=ugen_filter.h
# End Source File
# Begin Source File

SOURCE=ugen_osc.h
# End Source File
# Begin Source File

SOURCE=ugen_stk.h
# End Source File
# Begin Source File

SOURCE=ugen_xxx.h
# End Source File
# Begin Source File

SOURCE=ulib_machine.h
# End Source File
# Begin Source File

SOURCE=ulib_math.h
# End Source File
# Begin Source File

SOURCE=ulib_net.h
# End Source File
# Begin Source File

SOURCE=ulib_std.h
# End Source File
# Begin Source File

SOURCE=util_buffers.h
# End Source File
# Begin Source File

SOURCE=util_math.h
# End Source File
# Begin Source File

SOURCE=util_network.h
# End Source File
# Begin Source File

SOURCE=util_raw.h
# End Source File
# Begin Source File

SOURCE=util_sndfile.h
# End Source File
# Begin Source File

SOURCE=util_xforms.h
# End Source File
# End Group
# Begin Group "Resource Files"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;rgs;gif;jpg;jpeg;jpe"
# Begin Source File

SOURCE=chuck.lex

!IF  "$(CFG)" == "chuck - Win32 Release"

# Begin Custom Build
InputDir=.
InputPath=chuck.lex
InputName=chuck

"$(InputDir)$(InputName).yy.c" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	echo on 
	cd $(InputDir) 
	flex -o$(InputName).yy.c $(InputName).lex 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "chuck - Win32 Debug"

# Begin Custom Build
InputDir=.
InputPath=chuck.lex
InputName=chuck

"$(InputDir)$(InputName).yy.c" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	echo on 
	cd $(InputDir) 
	flex -o$(InputName).yy.c $(InputName).lex 
	
# End Custom Build

!ENDIF 

# End Source File
# Begin Source File

SOURCE=chuck.y

!IF  "$(CFG)" == "chuck - Win32 Release"

# Begin Custom Build
InputDir=.
InputPath=chuck.y
InputName=chuck

"$(InputDir)$(InputName).tab.c" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	echo on 
	cd $(InputDir) 
	bison -dv -b $(InputName) $(InputName).y 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "chuck - Win32 Debug"

# Begin Custom Build
InputDir=.
InputPath=chuck.y
InputName=chuck

"$(InputDir)$(InputName).tab.c" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	echo on 
	cd $(InputDir) 
	bison -dv -b $(InputName) $(InputName).y 
	
# End Custom Build

!ENDIF 

# End Source File
# End Group
# End Target
# End Project
