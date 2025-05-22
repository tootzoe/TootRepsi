@echo off
setlocal

::  Change this UE ROOT folder according to your environment
SET UE_FOLDER=D:/UnrealEngine


echo "++++++++========= Ver: 2025-02-20 ===++++++++++++++"
echo  Current Dir:  %~dp0
echo  UE_FOLDER Dir:  %UE_FOLDER%
echo "++++++++=============================++++++++++++++"


if exist "Source/*Editor.Target.cs" goto init

echo "No c/c++ relevance files, skip compilance....."
PAUSE
exit /b 1

:: echo %PROJECT%

 :: echo "++++++++=====================  For loop find *.pdb ========++++++++++++++"

 :: FOR    %%F in (Binaries\Win64\UnrealEditor-*.patch_*.pdb) do echo "%%~F"

 :: echo "++++++++=====================  For loop find *.pdb done ========++++++++++++++"

:init

FOR /f %%F in ('dir /b *.uproject') do SET PRJNAMEPATH=%%~dpnxF
FOR /f %%F in ('dir /b *.uproject') do SET PRJNAME=%%~nF

FOR /f %%F in ('dir /b Source\*Editor.Target.cs') do for %%G in ("%%~nF") do SET TARGETNAME=%%~nG

echo %PRJNAMEPATH%
echo %PRJNAME%
echo %TARGETNAME%


echo "Raw build++++==== UE build UnrealEditor Dlls for:  %PRJNAME%.uproject, Target=%TARGETNAME%   ====++++"

::  UE-5.5x
::call %UE_FOLDER%/Engine/Build/BatchFiles/Build.bat -Target="%TARGETNAME% Win64 Development" -project="%PRJNAMEPATH%"  -WaitMutex -FromMsBuild -architecture=x64

::  UE-5.6x
SET ZOESTR=%UE_FOLDER%/Engine/Build/BatchFiles/Build.bat -Target="%TARGETNAME% Win64 Development -Project=%PRJNAMEPATH%" -Target="ShaderCompileWorker Win64 Development -Project=%PRJNAMEPATH% -Quiet" -WaitMutex -FromMsBuild -architecture=x64
:: call %UE_FOLDER%/Engine/Build/BatchFiles/Build.bat -Target="%TARGETNAME% Win64 Development -Project=%PRJNAMEPATH%" -Target="ShaderCompileWorker Win64 Development -Project=%PRJNAMEPATH% -Quiet" -WaitMutex -FromMsBuild -architecture=x64




:: cmd.exe /c "D:\UnrealEngine\Engine\Build\BatchFiles\Build.bat -Target="FirstRun560cppEditor Win64 Development -Project=D:\workprj\uePrj\FirstRun560cpp\FirstRun560cpp.uproject" -Target="ShaderCompileWorker Win64 Development -Project=D:\workprj\uePrj\FirstRun560cpp\FirstRun560cpp.uproject -Quiet" -WaitMutex -FromMsBuild -architecture=x64"

ECHO:
ECHO:
ECHO %ZOESTR%
ECHO:
ECHO:


if %ERRORLEVEL% NEQ 0   pause

:: D:\UnrealEngine\Engine\Build\BatchFiles\Build.bat -Target="FirstRun560cppEditor Win64 Development -Project=D:\workprj\uePrj\FirstRun560cpp\FirstRun560cpp.uproject" -Target="ShaderCompileWorker Win64 Development -Project=D:\workprj\uePrj\FirstRun560cpp\FirstRun560cpp.uproject -Quiet" -WaitMutex -FromMsBuild -architecture=x64
