; https://sourceforge.net/projects/nsis/files/NSIS%202/
; gldoteater.nsi
;

;--------------------------------

SetCompressor lzma
;SetCompressor bzip2

; The name of the installer
Name "gldoteater"

; The name of the installer file
OutFile "..\..\gldoteater.nsi.exe"

; The default installation directory
InstallDir $PROGRAMFILES\gldoteater

; Registry key to check for directory (so if you install again, it will 
; overwrite the old one automatically)
InstallDirRegKey HKLM "Software\NSIS_gldoteater" "Install_Dir"

LoadLanguageFile "${NSISDIR}\Contrib\Language Files\English.nlf"
;--------------------------------
;Version Information

  VIProductVersion "1.0.0.0"
  VIAddVersionKey /LANG=${LANG_ENGLISH} "ProductName" "gldoteater"
  VIAddVersionKey /LANG=${LANG_ENGLISH} "Comments" "a multipler pacman clone"
  VIAddVersionKey /LANG=${LANG_ENGLISH} "CompanyName" "Nil"
  VIAddVersionKey /LANG=${LANG_ENGLISH} "LegalTrademarks" "Nil"
  VIAddVersionKey /LANG=${LANG_ENGLISH} "LegalCopyright" "�andreboyce"
  VIAddVersionKey /LANG=${LANG_ENGLISH} "FileDescription" "gldoteater"
  VIAddVersionKey /LANG=${LANG_ENGLISH} "FileVersion" "1.0.0"

;--------------------------------


;--------------------------------

ReserveFile "${NSISDIR}\Plugins\InstallOptions.dll"
ReserveFile "gldoteaterinstalloptions.ini"
; Pages

Page components
Page directory
Page instfiles
Page custom CreateCustom EndCustom ": InstallOptions"

UninstPage uninstConfirm
UninstPage instfiles

;--------------------------------
;icons
Icon ..\..\partial_source\gldoteater.ico
UninstallIcon ..\..\partial_source\gldoteater.ico
;--------------------------------
XPStyle on
;--------------------------------

; The stuff to install
Section "gldoteater (required)"

  ;ExecShell "open" "..\..\upx.bat"

  SectionIn RO
  
  ; Set output path to the installation directory.
  SetOutPath $INSTDIR
  
  ; Put file there
  File /x .git\ /x .svn\ /x *.res /x *.obj /x *.pch /x *.o /x *.a /x *.db /x *.Manifest /x *.bat ..\..\*.dll ..\..\gldoteater.exe

  SetOutPath $INSTDIR\data
  File /x .git\ /x .svn\ /x *.db /x *.bat ..\..\data\*.btm ..\..\data\*.txt ..\..\data\*.ts ..\..\data\*.sa ..\..\data\*.tai

  SetOutPath $INSTDIR\data\sounds
  File /x .svn\ /x *.db ..\..\data\sounds\*.wav

  SetOutPath $INSTDIR\data\texture
  File /nonfatal /x .svn\ /x *.db ..\..\data\texture\*.dds
 
;  SetOutPath $INSTDIR\data\texture\maps
;  File /nonfatal /x .svn\ /x *.db ..\..\data\texture\maps\*.png
  
  ; Write the installation path into the registry
  WriteRegStr HKLM SOFTWARE\NSIS_gldoteater "Install_Dir" "$INSTDIR"
  
  ; Write the uninstall keys for Windows
  WriteRegStr HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\gldoteater" "DisplayName" "NSIS gldoteater"
  WriteRegStr HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\gldoteater" "UninstallString" '"$INSTDIR\uninstall.exe"'
  WriteRegDWORD HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\gldoteater" "NoModify" 1
  WriteRegDWORD HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\gldoteater" "NoRepair" 1
  WriteUninstaller "uninstall.exe"
  
SectionEnd

Section "Partial Source"
   SetOutPath $INSTDIR
   File ..\..\*.vcxproj

   SetOutPath $INSTDIR\partial_source
   File /x .svn\ /x *.res /x *.obj /x *.pch /x *.o /x *.a ..\..\partial_source\*.h ..\..\partial_source\*.cpp ..\..\partial_source\*.txt ..\..\partial_source\*.rc ..\..\partial_source\*.ico ..\..\partial_source\*.bat
   ;File /nonfatal ..\..\partial_source\*.dev   

   SetOutPath $INSTDIR\partial_source\nsis_install_script
   File  ..\..\partial_source\nsis_install_script\*.ini ..\..\partial_source\nsis_install_script\*.nsi

   SetOutPath $INSTDIR\partial_source\master_server
   File  ..\..\partial_source\master_server\*.php ..\..\partial_source\master_server\*.txt

SectionEnd

Section "Desktop Shortcut"
  SetOutPath $INSTDIR
  CreateShortCut "$DESKTOP\gldoteater.lnk" "$INSTDIR\GLDotEater.exe" "" "$INSTDIR\GLDotEater.exe"  
SectionEnd

; Optional section (can be disabled by the user)
Section "Start Menu Shortcuts"
  SetOutPath $INSTDIR
  CreateDirectory "$SMPROGRAMS\gldoteater"
  CreateShortCut "$SMPROGRAMS\gldoteater\Uninstall.lnk" "$INSTDIR\uninstall.exe" "" "$INSTDIR\uninstall.exe" 0
  CreateShortCut "$SMPROGRAMS\gldoteater\gldoteater.lnk" "$INSTDIR\GLDotEater.exe" "" "$INSTDIR\GLDotEater.exe" 0
  
SectionEnd

;--------------------------------

; Uninstaller

Section "Uninstall"
  
  ; Remove registry keys
  DeleteRegKey HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\gldoteater"
  DeleteRegKey HKLM SOFTWARE\NSIS_gldoteater

  ; Remove files and uninstaller
  ;Delete $INSTDIR\data\texture\maps\*.*
  Delete $INSTDIR\data\texture\*.*
  Delete $INSTDIR\data\sounds\*.*
  Delete $INSTDIR\data\*.*
  Delete $INSTDIR\partial_source\master_server\*.*
  Delete $INSTDIR\partial_source\nsis_install_script\*.*
  Delete $INSTDIR\partial_source\*.*
  Delete $INSTDIR\*.*
  Delete $DESKTOP\gldoteater.lnk

  ; Remove shortcuts, if any
  Delete "$SMPROGRAMS\gldoteater\*.*"

  ; Remove directories used
  RMDir "$INSTDIR\data\sounds"
  RMDir "$INSTDIR\data\texture\maps"
  RMDir "$INSTDIR\data\texture"
  RMDir "$INSTDIR\data"
  RMDir "$INSTDIR\partial_source\master_server"
  ;RMDir "$INSTDIR\partial_source\nsis_install_script\nsis"
  RMDir "$INSTDIR\partial_source\nsis_install_script"  
  RMDir "$INSTDIR\partial_source"
  RMDir "$SMPROGRAMS\gldoteater"
  RMDir "$INSTDIR"

SectionEnd

; variables
;-----------------------------------
Var IniFile
Var ClientOptionServerIP
Var ClientOptionServerPort
Var ServerOptionPort
Var MasterServerOptionHost
Var MasterServerOptionPath
Var name
;-----------------------------------

Function CreateCustom

  ;Display the InstallOptions dialog

   GetTempFileName $0
   File /oname=$0 "gldoteaterinstalloptions.ini"

   StrCpy $IniFile $0

   InstallOptions::dialog $0
   Pop $R1
   StrCmp $R1 "cancel" done
   StrCmp $R1 "back" done
   StrCmp $R1 "success" done
   ;error: MessageBox MB_OK|MB_ICONSTOP "InstallOptions error:$\r$\n$R1"
   done:
FunctionEnd

Function EndCustom
   ; generate the config file

   ReadINIStr $0 $IniFile "Field 6" "State"
   StrCpy $ClientOptionServerIP $0
   ReadINIStr $0 $IniFile "Field 9" "State"
   StrCpy $ClientOptionServerPort $0

   ReadINIStr $0 $IniFile "Field 8" "State"
   StrCpy $ServerOptionPort $0

   ReadINIStr $0 $IniFile "Field 20" "State"
   StrCpy $MasterServerOptionHost $0
   ReadINIStr $0 $IniFile "Field 19" "State"
   StrCpy $MasterServerOptionPath $0

   ReadINIStr $0 $IniFile "Field 14" "State"
   StrCpy $name $0

   call WriteConfigFile

   ReadINIStr $0 $IniFile "Field 5" "State"
   StrCmp $0 1 run_program_now done
   run_program_now: Exec '"$INSTDIR\gldoteater.exe"'
   done:

FunctionEnd

Function WriteConfigFile
   ClearErrors
   SetOutPath $INSTDIR
   ;Delete $INSTDIR\data\config.txt
   FileOpen $0 "$INSTDIR\data\config.txt" w
   Pop $R1
   IfErrors error
   FileWrite $0 "nick=$name$\r$\n"
   FileWrite $0 "port=$ClientOptionServerPort$\r$\n"
   FileWrite $0 "masterserverhost=$MasterServerOptionHost$\r$\n"
   FileWrite $0 "remotehostaddress=$ClientOptionServerIP$\r$\n"
   FileWrite $0 "masterserverpath=$MasterServerOptionPath$\r$\n"
   FileClose $0
   goto done
   error: ;MessageBox MB_OK|MB_ICONSTOP "InstallOptions error:$\r$\n$R1"
   done:
FunctionEnd
