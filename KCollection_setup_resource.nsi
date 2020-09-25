; Script generated by the HM NIS Edit Script Wizard.

; HM NIS Edit Wizard helper defines
!define PRODUCT_NAME "Kepco DTRS Resource Collection "
!define PRODUCT_VERSION "1.1"
!define PRODUCT_PUBLISHER "SecurePoint, Inc."
!define PRODUCT_WEB_SITE ""
!define PRODUCT_DIR_REGKEY "Software\Microsoft\Windows\CurrentVersion\App Paths\KCollectionServer2.exe"
!define PRODUCT_UNINST_KEY "Software\Microsoft\Windows\CurrentVersion\Uninstall\${PRODUCT_NAME}"
!define PRODUCT_UNINST_ROOT_KEY "HKLM"

; MUI 1.67 compatible ------
!include "Library.nsh"
RequestExecutionLevel admin

Name "${PRODUCT_NAME}"



OutFile "KCollectionResource_setup.exe"
InstallDir "$PROGRAMFILES\SecurePoint\KCollection"
Icon "${NSISDIR}\Contrib\Graphics\Icons\modern-install.ico"
;SilentInstall silent
ShowInstDetails hide
ShowUnInstDetails hide
InstallDirRegKey HKLM "${PRODUCT_DIR_REGKEY}" ""


!define LVM_INSTALLPOS 1127

Section "전체 프로그램 설치" SEC01
  SetOutPath "$INSTDIR"
  Sleep 1500
  KillProcWMI::KillProc "KCollectionServer.exe"
  KillProcWMI::KillProc "Sensor_Agent.exe"

  SetOutPath "$INSTDIR"
  SetOverwrite on
  File ".\bin\Sensor_Agent.exe"
  File ".\bin\icon.ico"
  File ".\config.ini"
  File ".\config_agent.ini"
  File ".\bin\uninstall.ico"
  File ".\bin\res_manager.py"
  File ".\bin\python27.dll"
  File ".\bin\python.exe"
  File ".\bin\pythonw.exe"
  File ".\bin\tail.exe"

  CreateDirectory $INSTDIR\info
  CreateDirectory $SYSDIR\EnTmp
  CreateDirectory $INSTDIR\..\dlls
  CreateDirectory $INSTDIR\..\lib

  SetOutPath "$SYSDIR\EnTmp"
  File ".\vc_redist.x64.exe"

  SetOutPath "$INSTDIR\..\dlls"
  File /r ".\dlls\*"

  SetOutPath "$INSTDIR\..\lib"
  File /r ".\lib\*"  

  SetOverwrite on
  CreateDirectory "$SMPROGRAMS\SecurePoint"
  CreateShortCut "$SMPROGRAMS\SecurePoint\Uninstall.lnk" "$INSTDIR\uninstall.exe"
  SetAutoClose true
SectionEnd


Section -AdditionalIcons
  SetOutPath $INSTDIR
SectionEnd

Section -Post
  WriteUninstaller "$INSTDIR\Uninstall.exe"
  WriteRegStr HKLM "${PRODUCT_DIR_REGKEY}" "" "$INSTDIR\KCollectionServer.exe"
  WriteRegStr ${PRODUCT_UNINST_ROOT_KEY} "${PRODUCT_UNINST_KEY}" "DisplayName" "$(^Name)"
  WriteRegStr ${PRODUCT_UNINST_ROOT_KEY} "${PRODUCT_UNINST_KEY}" "UninstallString" "$INSTDIR\Uninstall.exe"
  WriteRegStr ${PRODUCT_UNINST_ROOT_KEY} "${PRODUCT_UNINST_KEY}" "DisplayVersion" "${PRODUCT_VERSION}"
  WriteRegStr ${PRODUCT_UNINST_ROOT_KEY} "${PRODUCT_UNINST_KEY}" "Publisher" "${PRODUCT_PUBLISHER}"
  Exec $INSTDIR\Sensor_Agent.exe
SectionEnd

Function un.onUninstSuccess
  HideWindow
  MessageBox MB_ICONINFORMATION|MB_OK "$(^Name) was successfully removed from your computer."
FunctionEnd


Section Uninstall
  KillProcWMI::KillProc "KCollectionServer.exe"
  KillProcWMI::KillProc "Sensor_Agent.exe"
  Sleep 500
  ;DeleteRegKey  HKLM "${PRODUCT_DIR_REGKEY}"
  DeleteRegKey ${PRODUCT_UNINST_ROOT_KEY} "${PRODUCT_UNINST_KEY}"
  Delete $INSTDIR\*.*
  Delete "$SMPROGRAMS\SecurePoint\*.*"
  RMDir "$SMPROGRAMS\SecurePoint"
  RMDir "$INSTDIR"
  SetAutoClose true
  
SectionEnd


Function InstallVCRedist
    Call CheckVCRedist
    Pop $R0
    StrCmp $R0 "No" 0 +3
    ExecWait "$INSTDIR\vc_redist.x64.exe /q:a"
FunctionEnd


Function CheckVCRedist
   Push $R0
   ClearErrors
   ReadRegDword $R0 HKLM "SOFTWARE\Microsoft\Windows\CurrentVersion\Uninstall\{837b34e3-7c30-493c-8f6a-2b0f04e2912c}" "Version"

   ; if VS 2005+ redist SP1 not installed, install it
   IfErrors 0 VSRedistInstalled
   StrCpy $R0 "No"

VSRedistInstalled:
   Exch $R0
FunctionEnd