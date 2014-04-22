; Installation script for mcercle
; Written by Cyril Frausti
; Licence : CC-BY-SA - http://creativecommons.org/licenses/by-sa/3.0/
; WebSite : http://frausti.fr

;--------------------------------
;Include Modern UI
	!include "MUI2.nsh"
	
; MUI Settings
;--------------------------------
;General
	; General Product Description Definitions
	!define SOFT_NAME 		"mcercle"
	!define SOFT_VERSION 	"14.04"
	!define SOFT_WEB_SITE 	"http://fraustelec.fr"
	!define SOFT_BUILD 		"1cf"
	!define ARCH       		"win32"
	!define ICON 			"install.ico"
	
	SetCompressor /final /solid lzma
	CRCCheck force
	XPStyle on
	
	;Name and file
	Name "${SOFT_NAME} ${SOFT_VERSION}"
	OutFile "Installer_${SOFT_NAME}-${SOFT_VERSION}-${ARCH}_${SOFT_BUILD}.exe"

	;Default installation folder
	InstallDir "$PROGRAMFILES\${SOFT_NAME}"
  
	;Get installation folder from registry if available
	InstallDirRegKey HKCU "Software\${SOFT_NAME}" ""

	;Request application privileges for Windows Vista
	; we request for admin because we write stuff into the registry
	RequestExecutionLevel admin
;--------------------------------
;Interface Settings
	!define MUI_ABORTWARNING
	!define MUI_ICON ${ICON}
	!define MUI_UNICON "uninstall.ico"
	!define MUI_HEADERIMAGE
	!define MUI_HEADERIMAGE_BITMAP "logo.bmp"	
;--------------------------------
;Language Selection Dialog Settings
	;Remember the installer language
	!define MUI_LANGDLL_REGISTRY_ROOT "HKCU" 
	!define MUI_LANGDLL_REGISTRY_KEY "Software\${SOFT_NAME}" 
	!define MUI_LANGDLL_REGISTRY_VALUENAME "Installer Language"

;--------------------------------
;Pages
	;!insertmacro MUI_PAGE_LICENSE "..\Licence.txt"
	!insertmacro MUI_PAGE_DIRECTORY
	!insertmacro MUI_PAGE_INSTFILES

	!insertmacro MUI_UNPAGE_CONFIRM
	!insertmacro MUI_UNPAGE_INSTFILES

	BrandingText "mcercle "
;--------------------------------
;Languages
	;!insertmacro MUI_LANGUAGE "English" ;first language is the default language
	!insertmacro MUI_LANGUAGE "French"

;--------------------------------
;Reserve Files  
	;If you are using solid compression, files that are required before
	;the actual installation should be stored first in the data block,
	;because this will make your installer start faster.
	!insertmacro MUI_RESERVEFILE_LANGDLL

;--------------------------------
;Installer Sections

Section "Logiciel"

	SetOutPath "$INSTDIR"  
	; copy every files in the "files" directory
	File /nonfatal /r "mcercle.exe"
	File /nonfatal /r "..\..\art\logo\mcercle.ico"
	File /nonfatal /r "C:\Qt\4.8.5\bin\QtCore4.dll"
	File /nonfatal /r "C:\Qt\4.8.5\bin\QtGui4.dll"
	File /nonfatal /r "C:\Qt\4.8.5\bin\QtSql4.dll"
	File /nonfatal /r "C:\mingw\bin\mingwm10.dll"
	File /nonfatal /r "C:\mingw\bin\libgcc_s_dw2-1.dll"	
	SetOutPath "$INSTDIR\sqldrivers"	
	File /nonfatal /r "C:\Qt\4.8.5\plugins\sqldrivers\qsqlite4.dll"	
	SetOutPath "$INSTDIR\imageformats"	
	File /nonfatal /r "C:\Qt\4.8.5\plugins\imageformats\qjpeg4.dll"	
	SetOutPath "$INSTDIR\lang"
	File /nonfatal /r "C:\Qt\4.8.5\translations\qt_fr.qm"
	
	SetOutPath "$INSTDIR" 
	;Store installation folder
	WriteRegStr HKCU "Software\${SOFT_NAME}" "" $INSTDIR
	; write uninstall strings
	WriteRegStr HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\${SOFT_NAME}" "DisplayName" "${SOFT_NAME} ${SOFT_VERSION} (Supprimer)"
	WriteRegStr HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\${SOFT_NAME}" "Publisher" "Cyril Frausti"
	WriteRegStr HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\${SOFT_NAME}" "UninstallString" '"$INSTDIR\Uninstall.exe"'

	; use defaults for parameters, icon, etc.
	; shortcuts in the start menu
	SetShellVarContext all ; all users
	CreateDirectory "$SMPROGRAMS\${SOFT_NAME}"
	CreateShortCut "$SMPROGRAMS\${SOFT_NAME}\mcercle.lnk" "$INSTDIR\mcercle.exe" 0 "$INSTDIR\mcercle.ico"
	CreateShortCut "$DESKTOP\mcercle.lnk" "$INSTDIR\mcercle.exe" 0 "$INSTDIR\mcercle.ico"
	;Create uninstaller
	WriteUninstaller "$INSTDIR\Uninstall.exe"
	
 
SectionEnd

;--------------------------------
;Installer Functions

Function .onInit

	!insertmacro MUI_LANGDLL_DISPLAY

FunctionEnd

;--------------------------------
;Uninstaller Section

UninstallIcon ${ICON}

Section "Uninstall"
	SetShellVarContext all ; all users
	;remove start menu shortcuts
	Delete "$DESKTOP\mcercle.lnk"
	Delete "$SMPROGRAMS\${SOFT_NAME}\mcercle.lnk"
	RMDir "$SMPROGRAMS\${SOFT_NAME}"

	;ADD YOUR OWN FILES HERE...
	Delete "$INSTDIR\*.*"
	RMDir /r "$INSTDIR"

	;remove installation registary keys
	DeleteRegKey HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\${SOFT_NAME}"
	DeleteRegKey /ifempty HKCU "Software\${SOFT_NAME}"
	
	IfFileExists "$INSTDIR" 0 NoErrorMsg
	MessageBox MB_OK "Note: $INSTDIR could not be removed!" IDOK 0 ; skipped if file doesn't exist
	NoErrorMsg:
  
SectionEnd

;--------------------------------
;Uninstaller Functions

Function un.onInit

	!insertmacro MUI_UNGETLANGUAGE
  
FunctionEnd

