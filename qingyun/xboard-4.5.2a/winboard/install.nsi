;tuxsection installation script
;Written by tuxtoken <mailto:tuxtoken@gmail.com>
;
;Requires NSIS >= 2.35

;--------------------------------
; Build environment

  !define top_srcdir "D:\code\qingyun\reserved\winboard"
  !define srcdir "D:\code\qingyun\reserved\winboard"
  !define VERSION "0.1"
  !addplugindir @srcdir@

;--------------------------------
;General

  ;Name and file
  Name "MeowBoard ${VERSION}"
  OutFile "MeowBoard_0_1_setup.exe"

  SetCompressor /SOLID LZMA

  ;Default installation folder
  InstallDir "$PROGRAMFILES\MeowBoard"
  
  ;Get installation folder from registry if available
  InstallDirRegKey HKLM "Software\MeowBoard" ""

  RequestExecutionLevel user

;--------------------------------
; Libtool executable target path

  !ifndef LT_EXEDIR
    !define LT_EXEDIR ""
  !endif

;--------------------------------
;Include Modern UI and functions

  !include "MUI2.nsh"
  !include "WordFunc.nsh"
  !include Library.nsh
  !include "WinVer.nsh"
  !include "FileFunc.nsh"
  !include "Memento.nsh"
	!include FontReg.nsh
	!include FontName.nsh
	!include WinMessages.nsh

;--------------------------------
;Required functions

  !insertmacro GetParameters
  !insertmacro GetOptions
  !insertmacro un.GetParameters
  !insertmacro un.GetOptions

;--------------------------------
;Variables

  Var MUI_TEMP
  Var STARTMENU_FOLDER
  Var PREVIOUS_INSTALLDIR
  Var PREVIOUS_VERSION
  Var PREVIOUS_VERSION_STATE
  Var REINSTALL_UNINSTALL
  Var REINSTALL_UNINSTALLBUTTON
  Var ALL_USERS_DEFAULT
  Var ALL_USERS
  Var ALL_USERS_BUTTON
  Var IS_ADMIN
  Var USERNAME
  Var PERFORM_UPDATE

;--------------------------------
;Interface Settings

  !define MUI_ICON "${top_srcdir}/bitmaps/icon_ow.ico"
  !define MUI_UNICON "${top_srcdir}/bitmaps/analyze_14.ico"

  !define MUI_ABORTWARNING

;--------------------------------
;Memento settings

!define MEMENTO_REGISTRY_ROOT SHELL_CONTEXT
!define MEMENTO_REGISTRY_KEY "Software\MeowBoard"

;--------------------------------
;Pages

  !define MUI_PAGE_CUSTOMFUNCTION_PRE PageLicensePre
  !insertmacro MUI_PAGE_LICENSE "${top_srcdir}\COPYING"

  Page custom PageReinstall PageLeaveReinstall
  Page custom PageAllUsers PageLeaveAllUsers

  !define MUI_PAGE_CUSTOMFUNCTION_PRE PageComponentsPre
  !insertmacro MUI_PAGE_COMPONENTS

  !define MUI_PAGE_CUSTOMFUNCTION_PRE PageDirectoryPre
  !insertmacro MUI_PAGE_DIRECTORY

  ;Start Menu Folder Page Configuration
  !define MUI_STARTMENUPAGE_REGISTRY_ROOT "SHCTX"
  !define MUI_STARTMENUPAGE_REGISTRY_KEY "Software\MeowBoard"
  !define MUI_STARTMENUPAGE_REGISTRY_VALUENAME "Startmenu"
  !define MUI_STARTMENUPAGE_DEFAULTFOLDER "MeowBoard"
  
  !define MUI_PAGE_CUSTOMFUNCTION_PRE PageStartmenuPre
  !insertmacro MUI_PAGE_STARTMENU Application $STARTMENU_FOLDER

  !define MUI_PAGE_CUSTOMFUNCTION_SHOW PageInstfilesShow
  !define MUI_PAGE_CUSTOMFUNCTION_LEAVE PostInstPage
  !insertmacro MUI_PAGE_INSTFILES

  !define MUI_FINISHPAGE_RUN
  !define MUI_FINISHPAGE_RUN_FUNCTION CustomFinishRun
  !define MUI_FINISHPAGE_RUN_TEXT "&Start MeowBoard now"
  !insertmacro MUI_PAGE_FINISH
  
  !define MUI_PAGE_CUSTOMFUNCTION_PRE un.ConfirmPagePre
  !insertmacro MUI_UNPAGE_CONFIRM
  !insertmacro MUI_UNPAGE_INSTFILES
  !define MUI_PAGE_CUSTOMFUNCTION_PRE un.FinishPagePre
  !insertmacro MUI_UNPAGE_FINISH

Function GetUserInfo
  ClearErrors
  UserInfo::GetName
  ${If} ${Errors}
    StrCpy $IS_ADMIN 1
    Return
  ${EndIf}

  Pop $USERNAME
  UserInfo::GetAccountType
  Pop $R0
  ${Switch} $R0
    ${Case} "Admin"
    ${Case} "Power"
      StrCpy $IS_ADMIN 1
      ${Break}
    ${Default}
      StrCpy $IS_ADMIN 0
      ${Break}
  ${EndSwitch}

FunctionEnd

Function UpdateShellVarContext

  ${If} $ALL_USERS == 1
    SetShellVarContext all
  ${Else}
    SetShellVarContext current
  ${EndIf}

FunctionEnd

Function ReadAllUsersCommandline

  ${GetParameters} $R0

  ${GetOptions} $R0 "/user" $R1

  ${Unless} ${Errors}
    ${If} $R1 == "current"
    ${OrIf} $R1 == "=current"
      StrCpy $ALL_USERS 0
    ${ElseIf} $R1 == "all"
    ${OrIf} $R1 == "=all"
      StrCpy $ALL_USERS 1
    ${Else}
      MessageBox MB_ICONSTOP "Invalid option for /user. Has to be either /user=all or /user=current" /SD IDOK
      Abort
    ${EndIf}
  ${EndUnless}
  Call UpdateShellVarContext

FunctionEnd

Function CheckPrevInstallDirExists

  ${If} $PREVIOUS_INSTALLDIR != ""

    ; Make sure directory is valid
    Push $R0
    Push $R1
    StrCpy $R0 "$PREVIOUS_INSTALLDIR" "" -1
    ${If} $R0 == '\'
    ${OrIf} $R0 == '/'
      StrCpy $R0 $PREVIOUS_INSTALLDIR*.*
    ${Else}
      StrCpy $R0 $PREVIOUS_INSTALLDIR\*.*
    ${EndIf}
    ${IfNot} ${FileExists} $R0
      StrCpy $PREVIOUS_INSTALLDIR ""
    ${EndIf}
    Pop $R1
    Pop $R0

  ${EndIf}

FunctionEnd

Function ReadPreviousVersion

  ReadRegStr $PREVIOUS_INSTALLDIR HKLM "Software\MeowBoard" ""

  Call CheckPrevInstallDirExists

  ${If} $PREVIOUS_INSTALLDIR != ""
    ;Detect version
    ReadRegStr $PREVIOUS_VERSION HKLM "Software\MeowBoard" "Version"
    ${If} $PREVIOUS_VERSION != ""
      StrCpy $ALL_USERS 1
      SetShellVarContext all
      return
    ${EndIf}
  ${EndIf}
  
  ReadRegStr $PREVIOUS_INSTALLDIR HKCU "Software\MeowBoard" ""

  Call CheckPrevInstallDirExists

  ${If} $PREVIOUS_INSTALLDIR != ""
    ;Detect version
    ReadRegStr $PREVIOUS_VERSION HKCU "Software\MeowBoard" "Version"
    ${If} $PREVIOUS_VERSION != ""
      StrCpy $ALL_USERS 0
      SetShellVarContext current
      return
    ${EndIf}
  ${EndIf}
 
FunctionEnd

Function LoadPreviousSettings

  ; Component selection
  ${MementoSectionRestore}

  ; Startmenu
  !define ID "Application"

  !ifdef MUI_STARTMENUPAGE_${ID}_REGISTRY_ROOT & MUI_STARTMENUPAGE_${ID}_REGISTRY_KEY & MUI_STARTMENUPAGE_${ID}_REGISTRY_VALUENAME

    ReadRegStr $mui.StartMenuPage.RegistryLocation "${MUI_STARTMENUPAGE_${ID}_REGISTRY_ROOT}" "${MUI_STARTMENUPAGE_${ID}_REGISTRY_KEY}" "${MUI_STARTMENUPAGE_${ID}_REGISTRY_VALUENAME}"

    ${if} $mui.StartMenuPage.RegistryLocation != ""
      StrCpy "$STARTMENU_FOLDER" $mui.StartMenuPage.RegistryLocation
    ${else}
      StrCpy "$STARTMENU_FOLDER" ""
    ${endif}

    !undef ID

  !endif

  ${If} $PREVIOUS_INSTALLDIR != ""
    StrCpy $INSTDIR $PREVIOUS_INSTALLDIR
  ${EndIf}

FunctionEnd

Function ReadUpdateCommandline

  ${GetParameters} $R0

  ${GetOptions} $R0 "/update" $R1

  ${If} ${Errors}
    StrCpy $PERFORM_UPDATE 0
  ${Else}
    StrCpy $PERFORM_UPDATE 1
  ${EndIf}

FunctionEnd

Function .onInit

  ${Unless} ${AtLeastWin2000}
    MessageBox MB_YESNO|MB_ICONSTOP "Unsupported operating system.$\nMeowBoard ${VERSION} requires at least Windows XP and may not work correctly on your system.$\nDo you really want to continue with the installation?" /SD IDNO IDYES installonoldwindows
    Abort
installonoldwindows:
  ${EndUnless}
  
  ; /update argument
  Call ReadUpdateCommandline

  Call GetUserInfo

  ; Initialize $ALL_USERS with default value
  ${If} $IS_ADMIN == 1
    StrCpy $ALL_USERS 1
  ${Else}
    StrCpy $ALL_USERS 0
  ${EndIf}
  Call UpdateShellVarContext

  ; See if previous version exists
  ; This can change ALL_USERS
  Call ReadPreviousVersion

  ; Load _all_ previous settings.
  ; Need to do it now as up to now, $ALL_USERS was possibly reflecting a
  ; previous installation. After this call, $ALL_USERS reflects the requested
  ; installation mode for this installation.
  Call LoadPreviousSettings

  Call ReadAllUsersCommandline

  ${If} $ALL_USERS == 1
    ${If} $IS_ADMIN == 0

      ${If} $PREVIOUS_VERSION != ""
        MessageBox MB_ICONSTOP "MeowBoard has been previously installed for all users.$\nPlease restart the installer with Administrator privileges." /SD IDOK
        Abort
      ${Else}
        MessageBox MB_ICONSTOP "Cannot install for all users.$\nPlease restart the installer with Administrator privileges." /SD IDOK
        Abort
      ${EndIf}
    ${EndIf}
  ${EndIf}
        
  ${If} $PREVIOUS_VERSION == ""

    StrCpy $PERFORM_UPDATE 0

  ${Else}

    Push "${VERSION}"
    Push $PREVIOUS_VERSION
    Call FZVersionCompare

    ${If} $PREVIOUS_VERSION_STATE != "newer"
      StrCpy $PERFORM_UPDATE 0
    ${EndIf}

  ${EndIf}

  StrCpy $ALL_USERS_DEFAULT $ALL_USERS

FunctionEnd

Function FZVersionCompare

  Exch $1
  Exch
  Exch $0

  Push $2
  Push $3
  Push $4

versioncomparebegin:
  ${If} $0 == ""
  ${AndIf} $1 == ""
    StrCpy $PREVIOUS_VERSION_STATE "same"
    goto versioncomparedone
  ${EndIf}

  StrCpy $2 0
  StrCpy $3 0

  ; Parse rc / beta suffixes for segments
  StrCpy $4 $0 2
  ${If} $4 == "rc"
    StrCpy $2 100
    StrCpy $0 $0 "" 2
  ${Else}
    StrCpy $4 $0 4
    ${If} $4 == "beta"
      StrCpy $0 $0 "" 4
    ${Else}
      StrCpy $2 10000
    ${EndIf}
  ${EndIf}

  StrCpy $4 $1 2
  ${If} $4 == "rc"
    StrCpy $3 100
    StrCpy $1 $1 "" 2
  ${Else}
    StrCpy $4 $1 4
    ${If} $4 == "beta"
      StrCpy $1 $1 "" 4
    ${Else}
      StrCpy $3 10000
    ${EndIf}
  ${EndIf}

split1loop:

  StrCmp $0 "" split1loopdone
  StrCpy $4 $0 1
  StrCpy $0 $0 "" 1
  StrCmp $4 "." split1loopdone
  StrCmp $4 "-" split1loopdone
  StrCpy $2 $2$4
  goto split1loop
split1loopdone:

split2loop:

  StrCmp $1 "" split2loopdone
  StrCpy $4 $1 1
  StrCpy $1 $1 "" 1
  StrCmp $4 "." split2loopdone
  StrCmp $4 "-" split2loopdone
  StrCpy $3 $3$4
  goto split2loop
split2loopdone:

  ${If} $2 > $3
    StrCpy $PREVIOUS_VERSION_STATE "newer"
  ${ElseIf} $3 > $2
    StrCpy $PREVIOUS_VERSION_STATE "older"
  ${Else}
    goto versioncomparebegin
  ${EndIf}


versioncomparedone:

  Pop $4
  Pop $3
  Pop $2
  Pop $1
  Pop $0

FunctionEnd

Function PageLicensePre

  ${If} $PERFORM_UPDATE == 1
    Abort
  ${EndIf}

FunctionEnd

Function PageReinstall

  ${If} $PREVIOUS_VERSION == ""
    Abort
  ${EndIf}

  ${If} $PERFORM_UPDATE == 1
    StrCpy $REINSTALL_UNINSTALL 1
    Abort
  ${EndIf}

  nsDialogs::Create /NOUNLOAD 1018
  Pop $0

  ${If} $PREVIOUS_VERSION_STATE == "newer"

    !insertmacro MUI_HEADER_TEXT "Already Installed" "Choose how you want to install MeowBoard."
    nsDialogs::CreateItem /NOUNLOAD STATIC ${WS_VISIBLE}|${WS_CHILD}|${WS_CLIPSIBLINGS} 0 0 0 100% 40 "An older version of MeowBoard is installed on your system. Select the operation you want to perform and click Next to continue."
    Pop $R0
    nsDialogs::CreateItem /NOUNLOAD BUTTON ${BS_AUTORADIOBUTTON}|${BS_VCENTER}|${BS_MULTILINE}|${WS_VISIBLE}|${WS_CHILD}|${WS_CLIPSIBLINGS}|${WS_GROUP}|${WS_TABSTOP} 0 10 55 100% 30 "Upgrade MeowBoard using previous settings (recommended)"
    Pop $REINSTALL_UNINSTALLBUTTON
    nsDialogs::CreateItem /NOUNLOAD BUTTON ${BS_AUTORADIOBUTTON}|${BS_TOP}|${BS_MULTILINE}|${WS_VISIBLE}|${WS_CHILD}|${WS_CLIPSIBLINGS} 0 10 85 100% 50 "Change settings (advanced)"
    Pop $R0

    ${If} $REINSTALL_UNINSTALL == ""
      StrCpy $REINSTALL_UNINSTALL 1
    ${EndIf}

  ${ElseIf} $PREVIOUS_VERSION_STATE == "older"

    !insertmacro MUI_HEADER_TEXT "Already Installed" "Choose how you want to install MeowBoard."
    nsDialogs::CreateItem /NOUNLOAD STATIC ${WS_VISIBLE}|${WS_CHILD}|${WS_CLIPSIBLINGS} 0 0 0 100% 40 "A newer version of MeowBoard is already installed! It is not recommended that you downgrade to an older version. Select the operation you want to perform and click Next to continue."
    Pop $R0
    nsDialogs::CreateItem /NOUNLOAD BUTTON ${BS_AUTORADIOBUTTON}|${BS_VCENTER}|${BS_MULTILINE}|${WS_VISIBLE}|${WS_CHILD}|${WS_CLIPSIBLINGS}|${WS_GROUP}|${WS_TABSTOP} 0 10 55 100% 30 "Downgrade MeowBoard using previous settings (recommended)"
    Pop $REINSTALL_UNINSTALLBUTTON
    nsDialogs::CreateItem /NOUNLOAD BUTTON ${BS_AUTORADIOBUTTON}|${BS_TOP}|${BS_MULTILINE}|${WS_VISIBLE}|${WS_CHILD}|${WS_CLIPSIBLINGS} 0 10 85 100% 50 "Change settings (advanced)"
    Pop $R0

    ${If} $REINSTALL_UNINSTALL == ""
      StrCpy $REINSTALL_UNINSTALL 1
    ${EndIf}

  ${ElseIf} $PREVIOUS_VERSION_STATE == "same"

    !insertmacro MUI_HEADER_TEXT "Already Installed" "Choose the maintenance option to perform."
    nsDialogs::CreateItem /NOUNLOAD STATIC ${WS_VISIBLE}|${WS_CHILD}|${WS_CLIPSIBLINGS} 0 0 0 100% 40 "MeowBoard ${VERSION} is already installed. Select the operation you want to perform and click Next to continue."
    Pop $R0
    nsDialogs::CreateItem /NOUNLOAD BUTTON ${BS_AUTORADIOBUTTON}|${BS_VCENTER}|${BS_MULTILINE}|${WS_VISIBLE}|${WS_CHILD}|${WS_CLIPSIBLINGS}|${WS_GROUP}|${WS_TABSTOP} 0 10 55 100% 30 "Add/Remove/Reinstall components"
    Pop $R0
    nsDialogs::CreateItem /NOUNLOAD BUTTON ${BS_AUTORADIOBUTTON}|${BS_TOP}|${BS_MULTILINE}|${WS_VISIBLE}|${WS_CHILD}|${WS_CLIPSIBLINGS} 0 10 85 100% 50 "Uninstall MeowBoard"
    Pop $REINSTALL_UNINSTALLBUTTON

    ${If} $REINSTALL_UNINSTALL == ""
      StrCpy $REINSTALL_UNINSTALL 2
    ${EndIf}

  ${Else}

    MessageBox MB_ICONSTOP "Unknown value of PREVIOUS_VERSION_STATE, aborting" /SD IDOK
    Abort

  ${EndIf}

  ${If} $REINSTALL_UNINSTALL == "1"
    SendMessage $REINSTALL_UNINSTALLBUTTON ${BM_SETCHECK} 1 0
  ${Else}
    SendMessage $R0 ${BM_SETCHECK} 1 0
  ${EndIf}

  nsDialogs::Show

FunctionEnd

Function RunUninstaller

  ${If} $ALL_USERS_DEFAULT == 1
    ReadRegStr $R1 HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\MeowBoard" "UninstallString"
  ${Else}
    ReadRegStr $R1 HKCU "Software\Microsoft\Windows\CurrentVersion\Uninstall\MeowBoard" "UninstallString"
  ${EndIf}

  ${If} $R1 == ""
    Return
  ${EndIf}

  ;Run uninstaller
  HideWindow

  ClearErrors

  ${If} $PREVIOUS_VERSION_STATE == "same"
  ${AndIf} $REINSTALL_UNINSTALL == "1"
    ExecWait '$R1 _?=$INSTDIR'
  ${Else}
    ExecWait '$R1 /frominstall /keepstartmenudir _?=$INSTDIR'
  ${EndIf}

  IfErrors no_remove_uninstaller

  IfFileExists "$INSTDIR\uninstall.exe" 0 no_remove_uninstaller

    Delete "$R1"
    RMDir $INSTDIR

 no_remove_uninstaller:

FunctionEnd

Function PageLeaveReinstall

  SendMessage $REINSTALL_UNINSTALLBUTTON ${BM_GETCHECK} 0 0 $R0
  ${If} $R0 == 1
    ; Option to uninstall old version selected
    StrCpy $REINSTALL_UNINSTALL 1
  ${Else}
    ; Custom up/downgrade or add/remove/reinstall
    StrCpy $REINSTALL_UNINSTALL 2
  ${EndIf}

  ${If} $REINSTALL_UNINSTALL == 1

    ${If} $PREVIOUS_VERSION_STATE == "same"

      Call RunUninstaller
      Quit

    ${Else}

      ; Need to reload defaults. User could have
      ; chosen custom, change something, went back and selected
      ; the express option.
      StrCpy $ALL_USERS $ALL_USERS_DEFAULT
      Call UpdateShellVarContext
      Call LoadPreviousSettings

    ${EndIf}

  ${EndIf}

FunctionEnd

Function PageAllUsers

  ${If} $REINSTALL_UNINSTALL == 1
    ; Keep same settings
    Abort
  ${EndIf}

  !insertmacro MUI_HEADER_TEXT "Choose Installation Options" "Who should this application be installed for?"

  nsDialogs::Create /NOUNLOAD 1018
  Pop $0

  nsDialogs::CreateItem /NOUNLOAD STATIC ${WS_VISIBLE}|${WS_CHILD}|${WS_CLIPSIBLINGS} 0 0 0 100% 30 "Please select whether you wish to make this software available to all users or just yourself."
  Pop $R0
  
  ${If} $IS_ADMIN == 1
    StrCpy $R2 ${BS_AUTORADIOBUTTON}|${BS_VCENTER}|${BS_MULTILINE}|${WS_VISIBLE}|${WS_CHILD}|${WS_CLIPSIBLINGS}|${WS_GROUP}|${WS_TABSTOP}
  ${Else}
    StrCpy $R2 ${BS_AUTORADIOBUTTON}|${BS_VCENTER}|${BS_MULTILINE}|${WS_VISIBLE}|${WS_CHILD}|${WS_CLIPSIBLINGS}|${WS_GROUP}|${WS_TABSTOP}|${WS_DISABLED}
  ${EndIf}
  nsDialogs::CreateItem /NOUNLOAD BUTTON $R2 0 10 55 100% 30 "&Anyone who uses this computer (all users)"
  Pop $ALL_USERS_BUTTON
  
  StrCpy $R2 ${BS_AUTORADIOBUTTON}|${BS_TOP}|${BS_MULTILINE}|${WS_VISIBLE}|${WS_CHILD}|${WS_CLIPSIBLINGS}
  
  ${If} $USERNAME != ""
    nsDialogs::CreateItem /NOUNLOAD BUTTON $R2 0 10 85 100% 50 "&Only for me ($USERNAME)"
  ${Else}
    nsDialogs::CreateItem /NOUNLOAD BUTTON $R2 0 10 85 100% 50 "&Only for me"
  ${EndIf}
  Pop $R0
  
  ${If} $PREVIOUS_VERSION != ""
    ${If} $ALL_USERS_DEFAULT == 1
      nsDialogs::CreateItem /NOUNLOAD STATIC ${WS_VISIBLE}|${WS_CHILD}|${WS_CLIPSIBLINGS} 0 0 -30 100% 30 "MeowBoard has been previously installed for all users."
    ${Else}
      nsDialogs::CreateItem /NOUNLOAD STATIC ${WS_VISIBLE}|${WS_CHILD}|${WS_CLIPSIBLINGS} 0 0 -30 100% 30 "MeowBoard has been previously installed for this user only."
    ${EndIf}
  ${Else}
    nsDialogs::CreateItem /NOUNLOAD STATIC ${WS_VISIBLE}|${WS_CHILD}|${WS_CLIPSIBLINGS} 0 0 -30 100% 30 "Installation for all users requires Administrator privileges."
  ${EndIf}
  Pop $R1

  ${If} $ALL_USERS == "1"
    SendMessage $ALL_USERS_BUTTON ${BM_SETCHECK} 1 0
  ${Else}
    SendMessage $R0 ${BM_SETCHECK} 1 0
  ${EndIf}
  
  nsDialogs::Show

FunctionEnd

Function PageLeaveAllUsers

  SendMessage $ALL_USERS_BUTTON ${BM_GETCHECK} 0 0 $R0
  ${If} $R0 == 0
    StrCpy $ALL_USERS "0"
  ${Else}
    StrCpy $ALL_USERS "1"
  ${EndIf}
  Call UpdateShellVarContext

FunctionEnd

Function PageComponentsPre

  ${If} $REINSTALL_UNINSTALL == 1

    Abort

  ${EndIf}

FunctionEnd

Function PageDirectoryPre

  ${If} $REINSTALL_UNINSTALL == "1"

    Abort

  ${EndIf}

FunctionEnd

Function PageStartmenuPre

  ${If} $REINSTALL_UNINSTALL == "1"

    ${If} "$STARTMENU_FOLDER" == ""

      StrCpy "$STARTMENU_FOLDER" ">"

    ${EndIf}

    Abort

  ${EndIf}

FunctionEnd

Function CheckMeowBoardRunning

  Push "Winboard.exe"
  Pop $R1

  ${If} $PERFORM_UPDATE == 1
    StrCpy $R0 10
  ${Else}
    StrCpy $R0 2
  ${EndIf}

  ${While} $R1 == 1
 
    ${If} $R0 == 0

      ${ExitWhile}

    ${EndIf}

    Sleep 500

    Push "Winboard.exe"
    Pop $R1

    IntOp $R0 $R0 - 1

  ${EndWhile}

  Push "Winboard.exe"
  Pop $R1

  ${While} $R1 == 1

    MessageBox MB_ABORTRETRYIGNORE|MB_DEFBUTTON2 "MeowBoard appears to be running.$\nPlease close all running instances of MeowBoard before continuing the installation." /SD IDIGNORE IDABORT CheckMeowBoardRunning_abort IDIGNORE CheckMeowBoardRunning_ignore

    Push "Winboard.exe"
    Pop $R1
    
  ${EndWhile}

 CheckMeowBoardRunning_ignore:
  Return

 CheckMeowBoardRunning_abort:
  Quit

FunctionEnd

Function PageInstfilesShow

  Call CheckMeowBoardRunning

  ${If} $REINSTALL_UNINSTALL != ""

    Call RunUninstaller
    BringToFront

  ${EndIf}

FunctionEnd

Function .OnInstFailed


FunctionEnd

Function .onInstSuccess

  ${MementoSectionSave}


FunctionEnd

;--------------------------------
;Languages
 
  !insertmacro MUI_LANGUAGE "English"

;--------------------------------
;Installer Sections

Section "MeowBoard" SecMain

  SectionIn 1 RO

  SetOutPath "$INSTDIR"
  
  File "${srcdir}\chinese(simp).lng"
  File "${top_srcdir}\chinese(trad).lng"
  File "${top_srcdir}\gloox.dll"
  File "${top_srcdir}\settings.ini"
  File "${srcdir}\vietnamese.lng"
  File "${top_srcdir}\Winboard.exe"
  File "${top_srcdir}\winboard.hlp"
  File "${top_srcdir}\winboard.ini"
  File "${top_srcdir}\COPYING"
    
  SetOutPath "$INSTDIR\bitmaps"
  File "${top_srcdir}/bitmaps\*.bmp"
  File "${top_srcdir}/bitmaps\*.ico"

  SetOutPath "$INSTDIR\textures"
  File "${top_srcdir}/textures\*.bmp"

  ;Create uninstaller
  WriteUninstaller "$INSTDIR\uninstall.exe"

  WriteRegStr SHCTX "Software\MeowBoard" "" $INSTDIR
  WriteRegStr SHCTX "Software\MeowBoard" "Version" "${VERSION}"

  WriteRegExpandStr SHCTX "Software\Microsoft\Windows\CurrentVersion\Uninstall\MeowBoard" "UninstallString" "$INSTDIR\uninstall.exe"
  WriteRegExpandStr SHCTX "Software\Microsoft\Windows\CurrentVersion\Uninstall\MeowBoard" "InstallLocation" "$INSTDIR"
  WriteRegStr SHCTX "Software\Microsoft\Windows\CurrentVersion\Uninstall\MeowBoard" "DisplayName" "MeowBoard ${VERSION}"
  WriteRegStr SHCTX "Software\Microsoft\Windows\CurrentVersion\Uninstall\MeowBoard" "DisplayIcon" "$INSTDIR\Winboard.exe"
  WriteRegStr SHCTX "Software\Microsoft\Windows\CurrentVersion\Uninstall\MeowBoard" "DisplayVersion" "${VERSION}"
  WriteRegStr SHCTX "Software\Microsoft\Windows\CurrentVersion\Uninstall\MeowBoard" "URLInfoAbout" "http://www.meowchess.com"
  WriteRegStr SHCTX "Software\Microsoft\Windows\CurrentVersion\Uninstall\MeowBoard" "HelpLink" "http://www.meowchess.com"
  WriteRegDWORD SHCTX "Software\Microsoft\Windows\CurrentVersion\Uninstall\MeowBoard" "NoModify" "1"
  WriteRegDWORD SHCTX "Software\Microsoft\Windows\CurrentVersion\Uninstall\MeowBoard" "NoRepair" "1"

  !insertmacro MUI_STARTMENU_WRITE_BEGIN Application
    
  ;Create shortcuts
  SetOutPath "$INSTDIR"
  CreateDirectory "$SMPROGRAMS\$STARTMENU_FOLDER"
  CreateShortCut "$SMPROGRAMS\$STARTMENU_FOLDER\Uninstall.lnk" "$INSTDIR\Uninstall.exe"
  CreateShortCut "$SMPROGRAMS\$STARTMENU_FOLDER\MeowBoard.lnk" "$INSTDIR\Winboard.exe"
  
  ${If} ${AtLeastWin7}
    ; Setting AppID
    push "MeowBoard.AppID"
    push "$SMPROGRAMS\$STARTMENU_FOLDER\MeowBoard.lnk"
  ${EndIf}

  !insertmacro MUI_STARTMENU_WRITE_END

  Push $R0
  StrCpy $R0 "$STARTMENU_FOLDER" 1
  ${if} $R0 == ">"
    ;Write folder to registry
    WriteRegStr "${MUI_STARTMENUPAGE_Application_REGISTRY_ROOT}" "${MUI_STARTMENUPAGE_Application_REGISTRY_KEY}" "${MUI_STARTMENUPAGE_Application_REGISTRY_VALUENAME}" ">"
  ${endif}
  Pop $R0

SectionEnd


Section "Fonts"
; Alternate for older versions of NSIS: pre NSIS v2.0rc1
;  push $1
;  System::Call "Shell32::SHGetSpecialFolderLocation(i $HWNDPARENT, i ${CSIDL_FONTS}|${CSIDL_FLAG_CREATE}, *i .r0)"
;  System::Call "Shell32::SHGetPathFromIDList(i r0, t .r1)"
;  System::Call 'shell32::SHGetMalloc(*i . r2)' ; IMalloc
;  System::Call '$2->5(i r0)' ; ->Free
;  System::Call '$2->2()' ; ->Release
;  StrCpy $FONT_DIR $1
;  pop $1

	SetOutPath "$FONTS"
	File "${top_srcdir}\XIANGQI.TTF"
 
  StrCpy $FONT_DIR $FONTS
 
  !insertmacro InstallTTFFont '${top_srcdir}\XIANGQI.TTF'
 
  SendMessage ${HWND_BROADCAST} ${WM_FONTCHANGE} 0 0 /TIMEOUT=5000
SectionEnd

!if "@MeowBoard_LINGUAS@" != ""

  !macro INSTALLLANGFILE LANG

     SetOutPath "$INSTDIR\locales\${LANG}"
     File /oname=MeowBoard.mo "..\locales\${LANG}.mo"

  !macroend

  ${MementoSection} "Language files" SecLang

    ; installlangfiles.nsh is generated by configure and just contains a series of
    ; !insertmacro INSTALLLANGFILE <lang>
 
  ${MementoSectionEnd}

!endif

${MementoSection} "Shell Extension" SecShellExt

SetOutPath "$INSTDIR"
  !define LIBRARY_SHELL_EXTENSION
  !define LIBRARY_COM
  !define LIBRARY_IGNORE_VERSION

${MementoSectionEnd}

${MementoUnselectedSection} "Desktop Icon" SecDesktop

  CreateShortCut "$DESKTOP\MeowBoard.lnk" "$INSTDIR\Winboard.exe" "" "$INSTDIR\Winboard.exe" 0

${MementoSectionEnd}

${MementoSectionDone}

;--------------------------------
;Functions

Function PostInstPage

  ; Don't advance automatically if details expanded
  FindWindow $R0 "#32770" "" $HWNDPARENT
  GetDlgItem $R0 $R0 1016
  System::Call user32::IsWindowVisible(i$R0)i.s
  Pop $R0

  StrCmp $R0 0 +2
  SetAutoClose false

FunctionEnd

Function CustomFinishRun


FunctionEnd

;--------------------------------
;Descriptions

  ;Language strings
  LangString DESC_SecMain ${LANG_ENGLISH} "Required program files."
  LangString DESC_SecIconSets ${LANG_ENGLISH} "Additional icon sets."
!if "@MeowBoard_LINGUAS@" != ""
  LangString DESC_SecLang ${LANG_ENGLISH} "Language files."
!endif
  LangString DESC_SecShellExt ${LANG_ENGLISH} "Shell extension for advanced drag && drop support. Required for drag && drop from MeowBoard into Explorer."
  LangString DESC_SecDesktop ${LANG_ENGLISH} "Create desktop icon for MeowBoard"

  ;Assign language strings to sections
  !insertmacro MUI_FUNCTION_DESCRIPTION_BEGIN
    !insertmacro MUI_DESCRIPTION_TEXT ${SecMain} $(DESC_SecMain)
    !insertmacro MUI_DESCRIPTION_TEXT ${SecIconSets} $(DESC_SecIconSets)
!if "@MeowBoard_LINGUAS@" != ""
    !insertmacro MUI_DESCRIPTION_TEXT ${SecLang} $(DESC_SecLang)
!endif
    !insertmacro MUI_DESCRIPTION_TEXT ${SecShellExt} $(DESC_SecShellExt)
    !insertmacro MUI_DESCRIPTION_TEXT ${SecDesktop} $(DESC_SecDesktop)
  !insertmacro MUI_FUNCTION_DESCRIPTION_END

;--------------------------------
;Uninstaller Variables

Var un.REMOVE_ALL_USERS
Var un.REMOVE_CURRENT_USER

;--------------------------------
;Uninstaller Functions

Function un.GetUserInfo
  ClearErrors
  UserInfo::GetName
  ${If} ${Errors}
    StrCpy $IS_ADMIN 1
    Return
  ${EndIf}

  Pop $USERNAME
  UserInfo::GetAccountType
  Pop $R0
  ${Switch} $R0
    ${Case} "Admin"
    ${Case} "Power"
      StrCpy $IS_ADMIN 1
      ${Break}
    ${Default}
      StrCpy $IS_ADMIN 0
      ${Break}
  ${EndSwitch}

FunctionEnd

Function un.ReadPreviousVersion

  ReadRegStr $R0 HKLM "Software\MeowBoard" ""

  ${If} $R0 != ""
    ;Detect version
    ReadRegStr $R2 HKLM "Software\MeowBoard" "Version"
    ${If} $R2 == ""
      StrCpy $R0 ""
    ${EndIf}
  ${EndIf}

  ReadRegStr $R1 HKCU "Software\MeowBoard" ""
  
  ${If} $R1 != ""
    ;Detect version
    ReadRegStr $R2 HKCU "Software\MeowBoard" "Version"
    ${If} $R2 == ""
      StrCpy $R1 ""
    ${EndIf}
  ${EndIf}

  ${If} $R1 == $INSTDIR
    Strcpy $un.REMOVE_CURRENT_USER 1
  ${EndIf}
  ${If} $R0 == $INSTDIR
    Strcpy $un.REMOVE_ALL_USERS 1
  ${EndIf}
  ${If} $un.REMOVE_CURRENT_USER != 1
  ${AndIf} $un.REMOVE_ALL_USERS != 1
    ${If} $R1 != ""
      Strcpy $un.REMOVE_CURRENT_USER 1
      ${If} $R0 == $R1
        Strcpy $un.REMOVE_ALL_USERS 1
      ${EndIf}
    ${Else}
      StrCpy $un.REMOVE_ALL_USERS = 1
    ${EndIf}
  ${EndIf}

FunctionEnd

Function un.onInit

  Call un.GetUserInfo
  Call un.ReadPreviousVersion

  ${If} $un.REMOVE_ALL_USERS == 1
  ${AndIf} $IS_ADMIN == 0
    MessageBox MB_ICONSTOP "MeowBoard has been installed for all users.$\nPlease restart the uninstaller with Administrator privileges to remove it." /SD IDOK
    Abort
  ${EndIf}
  
FunctionEnd

Function un.RemoveStartmenu

  !insertmacro MUI_STARTMENU_GETFOLDER Application $MUI_TEMP
  
  Delete "$SMPROGRAMS\$MUI_TEMP\Uninstall.lnk"
  Delete "$SMPROGRAMS\$MUI_TEMP\MeowBoard.lnk"
  
  ${un.GetParameters} $R0

  ${un.GetOptions} $R0 "/keepstartmenudir" $R1
  ${If} ${Errors}
 
    ;Delete empty start menu parent diretories
    StrCpy $MUI_TEMP "$SMPROGRAMS\$MUI_TEMP"
 
    startMenuDeleteLoop:
      RMDir $MUI_TEMP
      GetFullPathName $MUI_TEMP "$MUI_TEMP\.."
    
      IfErrors startMenuDeleteLoopDone
  
      StrCmp $MUI_TEMP $SMPROGRAMS startMenuDeleteLoopDone startMenuDeleteLoop
    startMenuDeleteLoopDone:

  ${EndUnless}

FunctionEnd

Function un.ConfirmPagePre
  
  ${un.GetParameters} $R0

  ${un.GetOptions} $R0 "/frominstall" $R1
  ${Unless} ${Errors}
    Abort
  ${EndUnless}

FunctionEnd

Function un.FinishPagePre
  
  ${un.GetParameters} $R0

  ${un.GetOptions} $R0 "/frominstall" $R1
  ${Unless} ${Errors}
    SetRebootFlag false
    Abort
  ${EndUnless}

FunctionEnd

;--------------------------------
;Uninstaller Section

!if "@MeowBoard_LINGUAS@" != ""

  !macro UNINSTALLLANGFILE LANG

    Delete "$INSTDIR\locales\${LANG}\MeowBoard.mo"
    RMDir "$INSTDIR\locales\${LANG}"

  !macroend

!endif

!macro UNINSTALLICONSET SET
  
  Delete "$INSTDIR\bitmaps\*.bmp"
  Delete "$INSTDIR\bitmaps\*.ico"
  Delete "$INSTDIR\textures\*.bmp"

!macroend

Section "Uninstall"

  !insertmacro UnInstallLib REGDLL NOTSHARED REBOOT_PROTECTED "$INSTDIR\gloox.dll"

  !define LIBRARY_X64
  ${If} ${RunningX64}
    !insertmacro UnInstallLib REGDLL NOTSHARED REBOOT_PROTECTED "$INSTDIR\fzshellext_64.dll"
  ${Else}
    !insertmacro UnInstallLib DLL NOTSHARED REBOOT_PROTECTED "$INSTDIR\fzshellext_64.dll"
  ${Endif}
  !undef LIBRARY_X64

  Delete "$INSTDIR\Winboard.exe"
  Delete "${srcdir}\chinese(simp).lng"
  Delete "${top_srcdir}\chinese(trad).lng"
  Delete "${top_srcdir}\gloox.dll"
  Delete "${top_srcdir}\settings.ini"
  Delete "${srcdir}\vietnamese.lng"
  Delete "${top_srcdir}\Winboard.exe"
  Delete "${top_srcdir}\winboard.hlp"
  Delete "${top_srcdir}\winboard.ini"
  Delete "${top_srcdir}\COPYING"

  !insertmacro UNINSTALLICONSET cyril
  !insertmacro UNINSTALLICONSET blukis
  !insertmacro UNINSTALLICONSET lone
  !insertmacro UNINSTALLICONSET minimal
  !insertmacro UNINSTALLICONSET opencrystal

!if "@MeowBoard_LINGUAS@" != ""
  ; uninstalllangfiles.nsh is generated by configure and just contains a series of
  ; !insertmacro UNINSTALLLANGFILE <lang>
!endif

  Delete "$INSTDIR\uninstall.exe"

!if "@MeowBoard_LINGUAS@" != ""
  RMDir "$INSTDIR\locales"
!endif
  RMDir "$INSTDIR\bitmaps"
  RMDir "$INSTDIR\textures"
  RMDir /REBOOTOK "$INSTDIR"

  ${If} $un.REMOVE_ALL_USERS == 1
    SetShellVarContext all
    Call un.RemoveStartmenu

    DeleteRegKey /ifempty HKLM "Software\MeowBoard"
    DeleteRegKey HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\MeowBoard"

    Delete "$DESKTOP\MeowBoard.lnk"
  ${EndIf}
  ${If} $un.REMOVE_CURRENT_USER == 1
    SetShellVarContext current
    Call un.RemoveStartmenu

    DeleteRegKey /ifempty HKCU "Software\MeowBoard"
    DeleteRegKey HKCU "Software\Microsoft\Windows\CurrentVersion\Uninstall\MeowBoard"

    Delete "$DESKTOP\MeowBoard.lnk"
  ${EndIf}

SectionEnd
