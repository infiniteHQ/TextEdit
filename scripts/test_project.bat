@echo off
setlocal enabledelayedexpansion

:: 1. Création des dossiers (mkdir -p)
if not exist "..\lib\vortex\tests\project\.vx\modules" (
    mkdir "..\lib\vortex\tests\project\.vx\modules"
)

:: 2. Copie du contenu (cp -r)
xcopy /E /I /Y "..\dist\*" "..\lib\vortex\tests\project\.vx\modules"

:: 3. Lecture de la version
set /p VERSION=<"..\lib\vortex\version.conf"

:: 4. Définition des chemins (SCRIPT_DIR est %~dp0 en batch)
set "SCRIPT_DIR=%~dp0"
:: On nettoie les chemins pour Windows (remplacement des / par des \)
set "VORTEX_PATH=%SCRIPT_DIR%..\lib\vortex\build\dist\%VERSION%\bin"
set "PROJECT_PATH=%SCRIPT_DIR%..\lib\vortex\tests\project"

:: 5. Génération du SESSION_ID (Format: editor-MM-DD-YYYY-HH-mm-ss-RANDOM)
set "datestr=%date:~3,2%-%date:~0,2%-%date:~6,4%"
set "timestr=%time:~0,2%-%time:~3,2%-%time:~6,2%"
set "timestr=%timestr: =0%"
set /a "random_num=%random% %% 9000 + 1000"

set "SESSION_ID=editor-%datestr%-%timestr%-%random_num%"

:: 6. Exécution (cd && bash ...)
cd /d "%PROJECT_PATH%"

call "%VORTEX_PATH%\handle_crash.bat" ^
  "%USERPROFILE%\.vx\sessions\%SESSION_ID%\crash\core_dumped.txt" ^
  "%VORTEX_PATH%\vortex.exe" --editor --session_id="\"%SESSION_ID%\"" ^
  ::END:: ^
  "%VORTEX_PATH%\vortex.exe" --crash --session_id="\"%SESSION_ID%\""

pause