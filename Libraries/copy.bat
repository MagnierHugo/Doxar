@echo off
setlocal enabledelayedexpansion

:: Prompt for the root directory
set /p RootDir="Enter the root directory: "

:: Prompt for the destination directory
set /p DestDir="Enter the destination directory: "

:: Ensure the destination directory exists
if not exist "%DestDir%" (
    echo Destination directory does not exist. Creating it...
    mkdir "%DestDir%"
)

:: Traverse the root directory and copy .h and .hpp files, preserving directory structure
for /r "%RootDir%" %%f in (*.h *.hpp *.inl) do (
    :: Get the parent directory relative to the root directory
    set "relativeDir=%%f"
    set "relativeDir=!relativeDir:%RootDir%=!"
    
    :: Create the directory in the destination folder
    mkdir "%DestDir%!relativeDir!\.."

    :: Copy the header file
    copy "%%f" "%DestDir%!relativeDir!"
)

echo Done copying header files.
pause