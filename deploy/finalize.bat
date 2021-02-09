@echo off
call :checkParent
if errorlevel 1 (exit /b 1)
rem =========================================================================
rem =========================================================================

:main
    setlocal
    set "eGROUP=%~1"
    @echo.
    @echo [FINALIZE] %eGROUP%

    call :normalizePath eDIR_PRODUCT ^
        "%eDIR_PRODUCT%\%eEXPANDED_SUFFIX%" 

    set "dir_=%eDIR_WORKSPACE%\external\%eNAME_PROJECT%"
    if defined eVERSION (set "dir_=%dir_%\%eVERSION%")
    call :normalizePath eDIR_INSTALL "%dir_%"

    if not defined eGROUP (
        (call :copyIncludes) && (goto :success) || (goto :failed)
    )

    set "eDIR_INSTALL=%eDIR_INSTALL%\%eEXPANDED_SUFFIX%"
    (call :copyLibraries) || (goto :failed)
:success
    @echo [FINALIZE] completed successfully
    if not defined eGROUP (@echo.)
exit /b 0

:failed
    @echo [FINALIZE] finished with erros
    if not defined eGROUP (@echo.)
exit /b 1 

rem =========================================================================
rem =========================================================================

:copyIncludes
    @echo [INSTALL] include...
    call :copyDirectory "include"
exit /b

:copyDirectory
    @echo  src: %eDIR_SOURCE%\%~1
    @echo  dst: %eDIR_INSTALL%\include

    xcopy "%eDIR_SOURCE%\%~1" ^
        "%eDIR_INSTALL%\include"\ /e /y /i >nul

    if errorlevel 1 (
        @echo [ERROR] can`t copy '%~1'
        exit /b 1
    )    
exit /b

:copyLibraries
    @echo [INSTALL] %eGROUP% libraries...
    call :copyFiles *.a *.lib *.pdb
exit /b

:copyFiles
    set "src=%eDIR_PRODUCT%"
    set "dst=%eDIR_INSTALL%"
    if not defined eDEBUG (goto :doCopy)
    @echo   src: %src%
    @echo   dst: %dst%
:doCopy
    robocopy "%src%" "%dst%" %* /xf "test.*" /s >nul 2>nul
    if %errorlevel% LSS 8 exit /b 0
    @echo [ERROR] can`t copy files    
exit /b 1

rem =========================================================================
rem =========================================================================

:normalizePath
    set "%~1=%~dpfn2"
exit /b

:checkParent
    if errorlevel 1 (
        @echo [ERROR] was started already in error state
        exit /b 1
    )
    if not defined eDIR_OWNER (
        @echo off
        @echo [ERROR] should be run from under the parent batch file
        exit /b 1
    )
exit /b

rem =========================================================================
rem =========================================================================
