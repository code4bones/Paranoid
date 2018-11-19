@SET DIR=%~dp0
@SET REDIST=%DIR%vcredist_x86sp1.exe
@SET APP=%DIR%Paranoid.exe

@REM ---------------------------------------------------------------------------------
@REM Для работы нужен рантайм VC10 ( x86 )
@REM Проверка на его наличии, и установка, если его нет в системе...
@REM ---------------------------------------------------------------------------------
@ reg query HKLM\SOFTWARE\WOW6432Node\Microsoft\VisualStudio\10.0\VC\VCRedist\x86 > nul
@if %ERRORLEVEL% EQU 0 goto install
@echo Installing %REDIST%
@ %REDIST% /passive /log %REDIST%                                                                                                   
@if %ERRORLEVEL% EQU 0 goto install

@REM ---------------------------------------------------------------------------------
@REM Рантайм небыл установлен,что-то пошло не так, детали - в логе
@REM ---------------------------------------------------------------------------------
:error
@echo Failed to install VC redistributables,code %ERRORLEVEL%
@echo You can try to run %REDIST% manually without /passive option
@echo Logfile opened...
@start %REDIST%.html
@goto done

@REM ---------------------------------------------------------------------------------
@REM Установка самрго приложения в тихом режиме, в системном трее появится красная иконка 
@REM ( если в параметрах Paranoid.conf ключ "hidden":false )
@REM ---------------------------------------------------------------------------------
:install
@echo Installing %APP%
@%APP% --silent --remove
@start %APP% --silent


:done
@echo ...Done