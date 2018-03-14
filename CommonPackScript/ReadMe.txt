软件运行依赖环境：
1，最低系统版本 windows 7 sp1
2, .net 4.5 

打包脚本使用说明：
1，安装nsis ,并将安装目标路径下的Bin目录写入系统环境变量
2，安装vs2015，安装wpf开发环境
3，配置MainBuild.bat 文件
编辑路径为本地路径

REM 软件icon 目录
SET ICON_PATH="D:\Pictures\icon.ico"

REM 定义编译器根目录
SET COMPILER="D:\Program Files\vs2015\VC\vcvarsall.bat"

REM 定义目标路径
SET TARTGET_DIR=D:\FutureStockTradingSystemClientRelease

REM 定义临时路径
SET TEMP_DIR=%TARTGET_DIR%\Temp

REM 定义artifact（安装文件）路径
SET ARTIFACT_DIR=%TARTGET_DIR%\Artifact

REM 工程解决方案目录
SET SOLUTION_HOME=D:\FutureStockTradingSystemClient\FutureStockTradingSystemPcClient

REM PC端编译生成目录
SET COMPILE_OUTPUT_DIR=%SOLUTION_HOME%\FutureStockTradingSystemPcClient\bin\Release

4，配置FutureStockTradingSystemClient(withDotNet).nsi

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


; 安装程序初始定义常量
; 项目名称
!define PRODUCT_NAME "金鼎国际交易端"
;项目版本号
!define PRODUCT_VERSION "1.0.0.0"
;开发者名称
!define PRODUCT_PUBLISHER "RockJarl"

5，将MainBuild.bat 和 FutureStockTradingSystemClient(withDotNet).nsi放在同一文件夹,并放在和工程同一盘符中；

6，管理员身份运行MainBuild.bat
