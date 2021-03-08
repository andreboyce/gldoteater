; gldoteater.nsi
;

;--------------------------------

SetCompressor lzma
;bzip2

; The name of the installer
Name "gldoteater"

; The name of the installer file
OutFile "gldoteater.nsi.exe"

; The default installation directory
InstallDir $PROGRAMFILES\gldoteater

; Registry key to check for directory (so if you install again, it will 
; overwrite the old one automatically)
InstallDirRegKey HKLM "Software\NSIS_gldoteater" "Install_Dir"

LoadLanguageFile "${NSISDIR}\Contrib\Language Files\English.nlf"
;--------------------------------
;Version Information

  VIProductVersion "0.9.4.0"
  VIAddVersionKey /LANG=${LANG_ENGLISH} "ProductName" "gldoteater"
  VIAddVersionKey /LANG=${LANG_ENGLISH} "Comments" "a multipler pacman clone"
  VIAddVersionKey /LANG=${LANG_ENGLISH} "CompanyName" "Nil"
  VIAddVersionKey /LANG=${LANG_ENGLISH} "LegalTrademarks" "Nil"
  VIAddVersionKey /LANG=${LANG_ENGLISH} "LegalCopyright" "Šandreboyce"
  VIAddVersionKey /LANG=${LANG_ENGLISH} "FileDescription" "gldoteater"
  VIAddVersionKey /LANG=${LANG_ENGLISH} "FileVersion" "0.9.3"

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
Icon GLDotEater\partial_source\gldoteater.ico
UninstallIcon GLDotEater\partial_source\gldoteater.ico
;--------------------------------
XPStyle on
;--------------------------------

; The stuff to install
Section "gldoteater (required)"

;  ExecShell "open" "gldoteater\upx.bat"

  SectionIn RO
  
  ; Set output path to the installation directory.
  SetOutPath $INSTDIR
  
  ; Put file there
  File /x .svn\ /x *.res /x *.obj /x *.pch /x *.o /x *.a /x *.db /x *.Manifest /x *.bat GLDotEater\*.exe GLDotEater\*.dll

  SetOutPath $INSTDIR\data
  File /x .svn\ /x *.db /x *.bat GLDotEater\data\*.btm GLDotEater\data\*.txt GLDotEater\data\*.ts GLDotEater\data\*.sa GLDotEater\data\*.exe GLDotEater\data\*.tai

  SetOutPath $INSTDIR\data\sounds
  File /x .svn\ /x *.db GLDotEater\data\sounds\*.wav

  SetOutPath $INSTDIR\data\texture
  File /nonfatal /x .svn\ /x *.db GLDotEater\data\texture\*.dds
 
;  SetOutPath $INSTDIR\data\texture\maps
;  File /nonfatal /x .svn\ /x *.db GLDotEater\data\texture\maps\*.png
  
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
   SetOutPath $INSTDIR\partial_source
   File  /x .svn\ /x *.res /x *.obj /x *.pch /x *.o /x *.a GLDotEater\partial_source\*.h GLDotEater\partial_source\*.cpp GLDotEater\partial_source\*.txt GLDotEater\partial_source\*.rc GLDotEater\partial_source\*.ico GLDotEater\partial_source\*.dev GLDotEater\partial_source\*.nsi
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
  Delete $INSTDIR\data\sounds\*.*
;  Delete $INSTDIR\data\texture\maps\*.*
  Delete $INSTDIR\data\texture\*.*
  Delete $INSTDIR\data\*.*
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
