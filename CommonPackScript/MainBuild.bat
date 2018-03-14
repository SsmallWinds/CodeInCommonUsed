@echo off

REM 需要用户手动配置的内容

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



del /S %SOLUTION_HOME%\FutureStockTradingSystemPcClient\icon.ico

xcopy %ICON_PATH% %SOLUTION_HOME%\FutureStockTradingSystemPcClient


REM 清理工程
if exist %COMPILE_OUTPUT_DIR% (
   RMDIR /Q/S %COMPILE_OUTPUT_DIR%
)

REM 删除历史文件
if exist %TARTGET_DIR% (
   RMDIR /Q/S %TARTGET_DIR%
)

MKDIR %TEMP_DIR%
MKDIR %ARTIFACT_DIR%


REM 打包编译
call %COMPILER%
MSBuild %SOLUTION_HOME%\FutureStockTradingSystemPcClient.sln /t:Rebuild /P:WarningLevel=2;Configuration=release   

REM 拷贝生成文件到临时目录
xcopy /YE %COMPILE_OUTPUT_DIR% %TEMP_DIR%
del /Q/S %TEMP_DIR%\*.pdb

REM 打安装包
makensis /DSorceFile_Path=%TEMP_DIR%\*.* /DTargetFile_Path=%ARTIFACT_DIR%\setup.exe ./FutureStockTradingSystemClient(withDotNet).nsi

PAUSE