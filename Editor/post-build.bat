@echo off
set source=%1
set destination=%2\Engine

:: Check if destination folder exists
if exist "%destination%" (
    echo The destination folder already exists.
    echo Copying operation canceled.
) else (
    :: If the destination folder doesn't exist, perform copy
    xcopy "%source%" "%destination%" /E /H /C /I
    echo Files and folders copied successfully.
)

pause
