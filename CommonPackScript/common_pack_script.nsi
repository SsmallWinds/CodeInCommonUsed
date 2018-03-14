; ------------ �Զ��������ʼ ------------


; �����ļ�·��
!ifndef SorceFile_Path
    !define SorceFile_Path "D:\FutureStockTradingSystemClientRelease\����\Temp\*.*"
!endif

; �����װ�ļ�·��
!ifndef TargetFile_Path
    !define TargetFile_Path "D:\FutureStockTradingSystemClientRelease\����\Artifact\setup.exe"
!endif


; .net ����·��
!define Dotnet_Url "https://download.microsoft.com/download/B/A/4/BA4A7E71-2906-4B2D-A0E1-80CF16844F5F/dotNetFx45_Full_setup.exe"
; .net �ļ�����
!define Dotnet_file "dotNetFx45_Full_setup.exe"


;�����û���˾��
!define COMPANY_NAME "JDGJ"
;�����ִ���ļ���
!define EXE_PATH "FutureStockTradingSystemPcClient.exe"

; ��װ�ļ�ͼ��
!define InstallerIcon "D:\Pictures\icon.ico"
; ж���ļ�ͼ��
!define UnInstallerIcon "D:\Pictures\un_install.ico"
; ���Э��
!define License_Path "D:\Documents\licence.txt"
; ------------ �Զ���������� ------------



; ��װ�����ʼ���峣��
; ��Ŀ����
!define PRODUCT_NAME "�𶦹��ʽ��׶�"
;��Ŀ�汾��
!define PRODUCT_VERSION "1.0.0.0"
;����������
!define PRODUCT_PUBLISHER "RockJarl"


;!define PRODUCT_WEB_SITE "http://www.RockJarl.com"
!define PRODUCT_DIR_REGKEY "Software\Microsoft\Windows\CurrentVersion\App Paths\${COMPANY_NAME}Client.exe"
!define PRODUCT_UNINST_KEY "Software\Microsoft\Windows\CurrentVersion\Uninstall\${PRODUCT_NAME}"
!define PRODUCT_UNINST_ROOT_KEY "HKLM"


; Ϊ��װ�ļ�����ѹ����
SetCompressor lzma


; ------ MUI �ִ����涨�� (1.67 �汾���ϼ���) ------
!include "MUI.nsh"

InstallDirRegKey HKCU "Software\${COMPANY_NAME}" "" ;����ע���·��


; MUI Ԥ���峣��
!define MUI_ABORTWARNING
!define MUI_ICON ${InstallerIcon}  ;icon Path
!define MUI_UNICON ${UnInstallerIcon} ;uninstall icon Path



; ��ӭҳ��
!insertmacro MUI_PAGE_WELCOME
; ���Э��ҳ��
;!insertmacro MUI_PAGE_LICENSE ${License_Path} ;licence path
; ��װĿ¼ѡ��ҳ��
!define MUI_PAGE_CUSTOMFUNCTION_PRE directory_page_show
!insertmacro MUI_PAGE_DIRECTORY
; ��װ����ҳ��
!insertmacro MUI_PAGE_INSTFILES

; ��װ���ҳ��
!define MUI_FINISHPAGE_RUN
!define MUI_FINISHPAGE_RUN_FUNCTION OpenSelf
!insertmacro MUI_PAGE_FINISH

; ��װж�ع���ҳ��
!insertmacro MUI_UNPAGE_INSTFILES

; ��װ�����������������
!insertmacro MUI_LANGUAGE "SimpChinese"

; ��װԤ�ͷ��ļ�
!insertmacro MUI_RESERVEFILE_INSTALLOPTIONS
; ------ MUI �ִ����涨����� ------

Name "${PRODUCT_NAME} ${PRODUCT_VERSION}"
OutFile ${TargetFile_Path}
InstallDir "$PROGRAMFILES\${COMPANY_NAME}"
ShowInstDetails show
ShowUnInstDetails show
BrandingText " "

Section "PreInit" SEC01
  SetOutPath "$INSTDIR"
  SetOverwrite ifnewer
  File /r ${SorceFile_Path}  ;�����ļ�·��
  CreateShortCut "$DESKTOP\${PRODUCT_NAME}.lnk" "$INSTDIR\${EXE_PATH}"
  WriteRegStr HKCU "Software\${COMPANY_NAME}" "" $INSTDIR ;д��ע���װ·��
SectionEnd

Section -AdditionalIcons
  ;WriteIniStr "$INSTDIR\${PRODUCT_NAME}.url" "InternetShortcut" "URL" "${PRODUCT_WEB_SITE}"
  CreateDirectory "$SMPROGRAMS\${COMPANY_NAME}"
  CreateShortCut "$SMPROGRAMS\${COMPANY_NAME}\${PRODUCT_NAME}.lnk" "$INSTDIR\${EXE_PATH}"
  CreateShortCut "$SMPROGRAMS\${COMPANY_NAME}\Website.lnk" "$INSTDIR\${PRODUCT_PUBLISHER}.url"
  CreateShortCut "$SMPROGRAMS\${COMPANY_NAME}\Uninstall.lnk" "$INSTDIR\uninst.exe"
SectionEnd

Section -Post
  WriteUninstaller "$INSTDIR\uninst.exe"
  WriteRegStr ${PRODUCT_UNINST_ROOT_KEY} "${PRODUCT_UNINST_KEY}" "DisplayName" "$(^Name)"
  WriteRegStr ${PRODUCT_UNINST_ROOT_KEY} "${PRODUCT_UNINST_KEY}" "UninstallString" "$INSTDIR\uninst.exe"
  WriteRegStr ${PRODUCT_UNINST_ROOT_KEY} "${PRODUCT_UNINST_KEY}" "DisplayVersion" "${PRODUCT_VERSION}"
  ;WriteRegStr ${PRODUCT_UNINST_ROOT_KEY} "${PRODUCT_UNINST_KEY}" "URLInfoAbout" "${PRODUCT_WEB_SITE}"
  WriteRegStr ${PRODUCT_UNINST_ROOT_KEY} "${PRODUCT_UNINST_KEY}" "Publisher" "${PRODUCT_PUBLISHER}"
SectionEnd

;��װ��ɺ󣬴����
Function OpenSelf
  exec '"$INSTDIR\${EXE_PATH}"'
Functionend

;��ʼ��֮ǰ���ж�ע������Ƿ���ֵ������ʹ��ע����ֶΣ�����ʹ��Ĭ��·��
Function .onInit

  ;��鰲װ�����Ƿ����
  System::Call 'kernel32::CreateMutexA(i 0, i 0, t "${COMPANY_NAME}InstallProcessKey") i .r1 ?e'
  Pop $R0
  StrCmp $R0 0 +3
    MessageBox MB_OK|MB_ICONEXCLAMATION "��װ�����Ѿ������С�"
    Abort
	
  ;����Ƿ����.net 4.5.2
  Call IsDotNETInstalled
  Pop $0
  
  ${If} $0 == 0
    MessageBox MB_OKCANCEL "��⵽ϵͳδ��װ.Net 4.5" IDOK ok IDCancel cancel
  ${EndIf}
    Goto done
  
  ;Goto done
    
  ok:
  Call InstallDotNetOnline
  Goto done
  
  cancel:
  Abort
    
  done:	
  StrCpy $R0 "$PROGRAMFILES\${COMPANY_NAME}"
  ReadRegStr $R1 HKCU "Software\${COMPANY_NAME}" ""
    ${If} $R1 == ''
      StrCpy $INSTDIR "$R0"
    ${Else}
      StrCpy $INSTDIR "$R1"
    ${EndIf}
      ;MessageBox MB_OK "$R1"
FunctionEnd

Function InstallDotNetOnline
  NSISdl::download /TRANSLATE2 '�������� %s' '��������...' '(ʣ�� 1 ��)' '(ʣ�� 1 ����)' '(ʣ�� 1 Сʱ)' '(ʣ�� %u ��)' '(ʣ�� %u ����)' '(ʣ�� %u Сʱ)' '����ɣ�%skB(%d%%) ��С��%skB �ٶȣ�%u.%01ukB/s' /TIMEOUT=7500 /NOIEPROXY '${Dotnet_Url}' '$TEMP\${Dotnet_file}'
  Pop $R0
  StrCmp $R0 "success" 0 +3
  MessageBox MB_OK|MB_ICONQUESTION "${Dotnet_file} �ѳɹ����ء�ȷ����װ" IDNO +2
 
  SetDetailsPrint textonly
  DetailPrint "���ڰ�װ .NET Framework..."
  SetDetailsPrint listonly
  ExecWait '$TEMP\${Dotnet_file} /norestart' $R1
  Delete "$TEMP\${Dotnet_file}"
FUnctionEnd

;�ж��Ƿ�װ.net 4.5 ��4.5����
Function IsDotNETInstalled
  Push $0
  Push $1

  ReadRegStr $1 HKEY_LOCAL_MACHINE "Software\Microsoft\NET Framework Setup\NDP\v4\Client" "Version"
  ;MessageBox MB_OK "$1" #For Test

  ${If} $1 < '4.5.2'
    StrCpy $0 0
  ${Else}
    StrCpy $0 1 #�Ѿ���װ.net
    Goto done
  ${EndIf}

  done:
    ;MessageBox MB_OK "$1+$0"
    Pop $1
    Exch $0
FunctionEnd

Function directory_page_show
  StrCpy $R0 "$PROGRAMFILES\${COMPANY_NAME}"
  ReadRegStr $R1 HKCU "Software\${COMPANY_NAME}" ""
    ${If} $R1 == ''
      ;MessageBox MB_OK "ע���Ϊ��"
      goto cancel
    ${Else}
      MessageBox MB_OKCANCEL "��⵽���а汾�����ȷ��������" IDOK ok IDCancel cancel
      
    ${EndIf}
    
    ok:
    Abort
    cancel:
    
Functionend




/******************************
 *  �����ǰ�װ�����ж�ز���  *
 ******************************/

Section Uninstall
  DeleteRegKey /ifempty HKCU "Software\${COMPANY_NAME}" ;ɾ��ע����ֵ
  delete "$INSTDIR\*"
  ;Delete "$INSTDIR\${PRODUCT_NAME}.url"
  ;Delete "$INSTDIR\uninst.exe"

  Delete "$SMPROGRAMS\${COMPANY_NAME}\${PRODUCT_NAME}.lnk"
  Delete "$SMPROGRAMS\${COMPANY_NAME}\Uninstall.lnk"
  Delete "$SMPROGRAMS\${COMPANY_NAME}\Website.lnk"
  Delete "$DESKTOP\${PRODUCT_NAME}.lnk"

  RMDir "$SMPROGRAMS\${COMPANY_NAME}"

  RMDir "$INSTDIR"
  
  RMDir /r "$INSTDIR\zh-Hans"
  RMDir /r "$INSTDIR\log"
  RMDir /r "$INSTDIR\Resources"
  
  DeleteRegKey ${PRODUCT_UNINST_ROOT_KEY} "${PRODUCT_UNINST_KEY}"
  DeleteRegKey HKLM "${PRODUCT_DIR_REGKEY}"
  SetAutoClose true
SectionEnd

#-- ���� NSIS �ű��༭�������� Function ���α�������� Section ����֮���д���Ա��ⰲװ�������δ��Ԥ֪�����⡣--#

Function un.onInit
  MessageBox MB_ICONQUESTION|MB_YESNO|MB_DEFBUTTON2 "��ȷʵҪ��ȫ�Ƴ� $(^Name) ���������е������" IDYES +2
  Abort
FunctionEnd

Function un.onUninstSuccess
  HideWindow
  MessageBox MB_ICONINFORMATION|MB_OK "$(^Name) �ѳɹ��ش����ļ�����Ƴ���"
FunctionEnd
