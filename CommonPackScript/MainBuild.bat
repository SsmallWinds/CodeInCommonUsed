@echo off

REM ��Ҫ�û��ֶ����õ�����

REM ���icon Ŀ¼
SET ICON_PATH="D:\Pictures\icon.ico"

REM �����������Ŀ¼
SET COMPILER="D:\Program Files\vs2015\VC\vcvarsall.bat"

REM ����Ŀ��·��
SET TARTGET_DIR=D:\FutureStockTradingSystemClientRelease

REM ������ʱ·��
SET TEMP_DIR=%TARTGET_DIR%\Temp

REM ����artifact����װ�ļ���·��
SET ARTIFACT_DIR=%TARTGET_DIR%\Artifact

REM ���̽������Ŀ¼
SET SOLUTION_HOME=D:\FutureStockTradingSystemClient\FutureStockTradingSystemPcClient

REM PC�˱�������Ŀ¼
SET COMPILE_OUTPUT_DIR=%SOLUTION_HOME%\FutureStockTradingSystemPcClient\bin\Release



del /S %SOLUTION_HOME%\FutureStockTradingSystemPcClient\icon.ico

xcopy %ICON_PATH% %SOLUTION_HOME%\FutureStockTradingSystemPcClient


REM ������
if exist %COMPILE_OUTPUT_DIR% (
   RMDIR /Q/S %COMPILE_OUTPUT_DIR%
)

REM ɾ����ʷ�ļ�
if exist %TARTGET_DIR% (
   RMDIR /Q/S %TARTGET_DIR%
)

MKDIR %TEMP_DIR%
MKDIR %ARTIFACT_DIR%


REM �������
call %COMPILER%
MSBuild %SOLUTION_HOME%\FutureStockTradingSystemPcClient.sln /t:Rebuild /P:WarningLevel=2;Configuration=release   

REM ���������ļ�����ʱĿ¼
xcopy /YE %COMPILE_OUTPUT_DIR% %TEMP_DIR%
del /Q/S %TEMP_DIR%\*.pdb

REM ��װ��
makensis /DSorceFile_Path=%TEMP_DIR%\*.* /DTargetFile_Path=%ARTIFACT_DIR%\setup.exe ./FutureStockTradingSystemClient(withDotNet).nsi

PAUSE