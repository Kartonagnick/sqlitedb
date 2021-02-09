@echo off 
call :checkParent
if errorlevel 1 (exit /b 1)
rem ============================================================================
rem ============================================================================
:main
    setlocal
    @echo [MAKE] run...

    rem set "eDEBUG=ON"

    rem set "VC=msvc:64:all:dynamic"
    rem set "MG=mingw:64:all:static"
    rem set "order=%VC%"
    rem set "order=%MG%"
    rem set "order=%VC%; %MG%"
    set "order=all"

    rem for development
    rem (call :generate) && (goto :success) || (goto :failed)

    (call :clean)    || (goto :failed)
    (call :build)    || (goto :failed)
    (call :runTests) || (goto :failed)
    (call :install)  || (goto :failed)
:success
    @echo [MAKE] completed successfully
exit /b 0

:failed
    @echo [MAKE] finished with erros
exit /b 1

rem ............................................................................

:clean
    call "%eDIR_BAT_ENGINE%\run.bat"  ^
        "--clean: all" 
exit /b

:generate
    call "%eDIR_BAT_ENGINE%\run.bat"  ^
        "--generate: cmake-makefiles" ^
        "--configurations: %order%"   ^
        "--defines: UNSTABLE_RELEASE"
exit /b

:build
    call "%eDIR_BAT_ENGINE%\run.bat"  ^
        "--build: cmake-makefiles"    ^
        "--configurations: %order%"   ^
        "--defines: STABLE_RELEASE"
exit /b

:runTests
    call "%eDIR_BAT_ENGINE%\run.bat"  ^
        "--runTests: test*.exe"       ^
        "--exclude: mingw*-dynamic"   ^
        "--configurations: %order%"
exit /b

:install
    call "%eDIR_BAT_ENGINE%\run.bat" ^
        "--custom: finalize"         ^
        "--configurations: %order%"
exit /b

rem ============================================================================
rem ============================================================================

:findWorkspace
    if defined eDIR_WORKSPACE (exit /b)
    if not defined eWORKSPACE_SYMPTOMS (
        set "eWORKSPACE_SYMPTOMS=3rd_party;programs"
    ) 
    setlocal
    set "dir_start=%~1"
    if not defined dir_start (set "dir_start=%CD%")
    if not exist "%dir_start%" (goto :findWorkspaceFailed)
    set "DRIVE=%dir_start:~0,3%"
    pushd "%dir_start%" 
:loopFindWorkspace
    call :checkWorkspaceSymptoms
    if not errorlevel 1    (goto :findWorkspaceSuccess)
    if "%DRIVE%" == "%CD%" (goto :findWorkspaceFailed )
    cd ..
    goto :loopFindWorkspace
exit /b

:findWorkspaceSuccess
    endlocal & set "eDIR_WORKSPACE=%CD%"
    popd
exit /b 0

:findWorkspaceFailed
    popd
exit /b 1

rem ............................................................................

:checkWorkspaceSymptoms
    set "enumerator=%eWORKSPACE_SYMPTOMS%"
:loopWorkspaceSymptoms
    for /F "tokens=1* delims=;" %%a in ("%enumerator%") do (
        for /F "tokens=*" %%a in ("%%a") do (
            if not exist "%CD%\%%a" exit /b 1
        )
        set "enumerator=%%b"
    )
    if defined enumerator goto :loopWorkspaceSymptoms
exit /b 

rem ============================================================================
rem ============================================================================

:normalize
    set "%~1=%~dpfn2"
exit /b

:checkParent
    if errorlevel 1 (
        @echo [ERROR] was broken at launch
        exit /b 1
    )
    if not defined eDIR_OWNER (
        @echo off & cls & @echo. & @echo.
        call :normalize eDIR_OWNER "%~dp0."
    )
    (call :findWorkspace) || (call :findWorkspace "C:\workspace")
    if errorlevel 1 (
        @echo [ERROR] 'WorkSpace' not found
        exit /b 1
    )
    if not defined eDIR_BAT_SCRIPTS (
        set "eDIR_BAT_SCRIPTS=%eDIR_WORKSPACE%\scripts\bat"
    )
    if not defined eDIR_BAT_ENGINE (
        set "eDIR_BAT_ENGINE=%eDIR_BAT_SCRIPTS%\engine"
    )
exit /b 0

rem ============================================================================
rem ============================================================================
