rem
rem Copyright (c) 2009 Nokia Corporation and/or its subsidiary(-ies).
rem All rights reserved.
rem This component and the accompanying materials are made available
rem under the terms of "Eclipse Public License v1.0"
rem which accompanies this distribution, and is available
rem at the URL "http://www.eclipse.org/legal/epl-v10.html".
rem
rem Initial Contributors:
rem Nokia Corporation - initial contribution.
rem
rem Contributors:
rem
rem Description: For running cce tests.
rem

:: This script is run from Windows command prompt
:: Runs all the CCE test cases excluding 10.1 and 10.2
:: Overwrites autoexe.bat file if one exists

@echo off
setlocal

:: Timeout value for testing in seconds
set testing_timeout=1800

::-----------------------------------------------------------------------------------------

:: Check Cygwin
sleep > NUL
if %ERRORLEVEL% EQU 9009 (
   echo Error: Cygwin not installed!
   goto end )

:: If no/wrong parameter given, default is udeb
if /i [%1] EQU [urel] (
   set testbuild=urel
   ) else (
   if /i [%1] EQU [udeb] ( 
   set testbuild=udeb
   ) else (
   set testbuild=udeb
   ))
   
echo Testing %testbuild%

echo Creating autoexec.bat...
copy \epoc32\winscw\c\ccetests.bat \epoc32\winscw\c\autoexec.bat
   
if exist \epoc32\WINSCW\C\Logs\TestFramework\cce_automated_testing_completed rd \epoc32\WINSCW\C\Logs\TestFramework\cce_automated_testing_completed

echo Running CCE test cases...
if /i %testbuild%==udeb start \epoc32\release\winscw\udeb\epoc.exe -dtextshell --
if /i %testbuild%==urel start \epoc32\release\winscw\urel\epoc.exe -dtextshell --

echo Waiting for testing to complete
echo Time out value is %testing_timeout% seconds
set seconds_elapsed=0

:wait_testing_complete
if not exist \epoc32\WINSCW\C\Logs\TestFramework\cce_automated_testing_completed  (
   set /a seconds_elapsed=%seconds_elapsed%+1
    
   if %seconds_elapsed% GTR %testing_timeout% (
      echo.
      echo Time out during testing
      goto kill_epoc )
   (set /p prompt=) < NUL
   (set /p prompt=Time elapsed:%seconds_elapsed%) < NUL
   sleep 1
   goto wait_testing_complete )

:kill_epoc
if exist \epoc32\WINSCW\C\Logs\TestFramework\cce_automated_testing_completed  rd \epoc32\WINSCW\C\Logs\TestFramework\cce_automated_testing_completed
echo.
echo Killing epoc.exe process...

start /wait wmic process where (name="epoc.exe") delete

:: Wait autoexe.bat file handle to be released
sleep 1

echo Deleting autoexec.bat...
del \epoc32\winscw\c\autoexec.bat

:: Rename the test report using date and time
if exist \epoc32\WINSCW\C\logs\TestFramework\testreport_all_cce_cases.html (
  for /f "tokens=1,2 delims=: " %%i in ("%time%") do (
    set hours=%%i
    set minutes=%%j
    echo Renaming test report...
    ren \epoc32\WINSCW\C\logs\TestFramework\testreport_all_cce_cases.html %date%_%%i.%%j_cce_report.html ) )

:end
