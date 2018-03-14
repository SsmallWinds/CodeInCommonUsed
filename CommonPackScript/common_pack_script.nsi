; ------------ 自定义变量开始 ------------


; 输入文件路径
!ifndef SorceFile_Path
    !define SorceFile_Path "D:\FutureStockTradingSystemClientRelease\主线\Temp\*.*"
!endif

; 输出安装文件路径
!ifndef TargetFile_Path
    !define TargetFile_Path "D:\FutureStockTradingSystemClientRelease\主线\Artifact\setup.exe"
!endif


; .net 下载路径
!define Dotnet_Url "https://download.microsoft.com/download/B/A/4/BA4A7E71-2906-4B2D-A0E1-80CF16844F5F/dotNetFx45_Full_setup.exe"
; .net 文件名称
!define Dotnet_file "dotNetFx45_Full_setup.exe"


;定义用户公司名
!define COMPANY_NAME "JDGJ"
;定义可执行文件名
!define EXE_PATH "FutureStockTradingSystemPcClient.exe"

; 安装文件图标
!define InstallerIcon "D:\Pictures\icon.ico"
; 卸载文件图标
!define UnInstallerIcon "D:\Pictures\un_install.ico"
; 许可协议
!define License_Path "D:\Documents\licence.txt"
; ------------ 自定义变量结束 ------------



; 安装程序初始定义常量
; 项目名称
!define PRODUCT_NAME "金鼎国际交易端"
;项目版本号
!define PRODUCT_VERSION "1.0.0.0"
;开发者名称
!define PRODUCT_PUBLISHER "RockJarl"


;!define PRODUCT_WEB_SITE "http://www.RockJarl.com"
!define PRODUCT_DIR_REGKEY "Software\Microsoft\Windows\CurrentVersion\App Paths\${COMPANY_NAME}Client.exe"
!define PRODUCT_UNINST_KEY "Software\Microsoft\Windows\CurrentVersion\Uninstall\${PRODUCT_NAME}"
!define PRODUCT_UNINST_ROOT_KEY "HKLM"


; 为安装文件设置压缩器
SetCompressor lzma


; ------ MUI 现代界面定义 (1.67 版本以上兼容) ------
!include "MUI.nsh"

InstallDirRegKey HKCU "Software\${COMPANY_NAME}" "" ;设置注册表路径


; MUI 预定义常量
!define MUI_ABORTWARNING
!define MUI_ICON ${InstallerIcon}  ;icon Path
!define MUI_UNICON ${UnInstallerIcon} ;uninstall icon Path



; 欢迎页面
!insertmacro MUI_PAGE_WELCOME
; 许可协议页面
;!insertmacro MUI_PAGE_LICENSE ${License_Path} ;licence path
; 安装目录选择页面
!define MUI_PAGE_CUSTOMFUNCTION_PRE directory_page_show
!insertmacro MUI_PAGE_DIRECTORY
; 安装过程页面
!insertmacro MUI_PAGE_INSTFILES

; 安装完成页面
!define MUI_FINISHPAGE_RUN
!define MUI_FINISHPAGE_RUN_FUNCTION OpenSelf
!insertmacro MUI_PAGE_FINISH

; 安装卸载过程页面
!insertmacro MUI_UNPAGE_INSTFILES

; 安装界面包含的语言设置
!insertmacro MUI_LANGUAGE "SimpChinese"

; 安装预释放文件
!insertmacro MUI_RESERVEFILE_INSTALLOPTIONS
; ------ MUI 现代界面定义结束 ------

Name "${PRODUCT_NAME} ${PRODUCT_VERSION}"
OutFile ${TargetFile_Path}
InstallDir "$PROGRAMFILES\${COMPANY_NAME}"
ShowInstDetails show
ShowUnInstDetails show
BrandingText " "

Section "PreInit" SEC01
  SetOutPath "$INSTDIR"
  SetOverwrite ifnewer
  File /r ${SorceFile_Path}  ;设置文件路径
  CreateShortCut "$DESKTOP\${PRODUCT_NAME}.lnk" "$INSTDIR\${EXE_PATH}"
  WriteRegStr HKCU "Software\${COMPANY_NAME}" "" $INSTDIR ;写入注册表安装路径
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

;安装完成后，打开软件
Function OpenSelf
  exec '"$INSTDIR\${EXE_PATH}"'
Functionend

;初始化之前，判断注册表中是否有值，有则使用注册表字段，否则使用默认路径
Function .onInit

  ;检查安装进程是否存在
  System::Call 'kernel32::CreateMutexA(i 0, i 0, t "${COMPANY_NAME}InstallProcessKey") i .r1 ?e'
  Pop $R0
  StrCmp $R0 0 +3
    MessageBox MB_OK|MB_ICONEXCLAMATION "安装程序已经在运行。"
    Abort
	
  ;检测是否存在.net 4.5.2
  Call IsDotNETInstalled
  Pop $0
  
  ${If} $0 == 0
    MessageBox MB_OKCANCEL "检测到系统未安装.Net 4.5" IDOK ok IDCancel cancel
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
  NSISdl::download /TRANSLATE2 '正在下载 %s' '正在连接...' '(剩余 1 秒)' '(剩余 1 分钟)' '(剩余 1 小时)' '(剩余 %u 秒)' '(剩余 %u 分钟)' '(剩余 %u 小时)' '已完成：%skB(%d%%) 大小：%skB 速度：%u.%01ukB/s' /TIMEOUT=7500 /NOIEPROXY '${Dotnet_Url}' '$TEMP\${Dotnet_file}'
  Pop $R0
  StrCmp $R0 "success" 0 +3
  MessageBox MB_OK|MB_ICONQUESTION "${Dotnet_file} 已成功下载。确定安装" IDNO +2
 
  SetDetailsPrint textonly
  DetailPrint "正在安装 .NET Framework..."
  SetDetailsPrint listonly
  ExecWait '$TEMP\${Dotnet_file} /norestart' $R1
  Delete "$TEMP\${Dotnet_file}"
FUnctionEnd

;判断是否安装.net 4.5 或4.5以上
Function IsDotNETInstalled
  Push $0
  Push $1

  ReadRegStr $1 HKEY_LOCAL_MACHINE "Software\Microsoft\NET Framework Setup\NDP\v4\Client" "Version"
  ;MessageBox MB_OK "$1" #For Test

  ${If} $1 < '4.5.2'
    StrCpy $0 0
  ${Else}
    StrCpy $0 1 #已经安装.net
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
      ;MessageBox MB_OK "注册表为空"
      goto cancel
    ${Else}
      MessageBox MB_OKCANCEL "检测到已有版本，点击确定升级！" IDOK ok IDCancel cancel
      
    ${EndIf}
    
    ok:
    Abort
    cancel:
    
Functionend




/******************************
 *  以下是安装程序的卸载部分  *
 ******************************/

Section Uninstall
  DeleteRegKey /ifempty HKCU "Software\${COMPANY_NAME}" ;删除注册表键值
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

#-- 根据 NSIS 脚本编辑规则，所有 Function 区段必须放置在 Section 区段之后编写，以避免安装程序出现未可预知的问题。--#

Function un.onInit
  MessageBox MB_ICONQUESTION|MB_YESNO|MB_DEFBUTTON2 "您确实要完全移除 $(^Name) ，及其所有的组件？" IDYES +2
  Abort
FunctionEnd

Function un.onUninstSuccess
  HideWindow
  MessageBox MB_ICONINFORMATION|MB_OK "$(^Name) 已成功地从您的计算机移除。"
FunctionEnd
