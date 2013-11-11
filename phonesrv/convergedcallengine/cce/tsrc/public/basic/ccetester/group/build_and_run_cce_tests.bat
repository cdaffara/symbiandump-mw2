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

::  This script is run from Windows command prompt
::   - runs all the CCE test cases (test cases 10.1 and 10.2 create a separate log)
::   - overwrites epoc32\winscw\c\autoexe.bat file if one exists
::   - deletes ccptestplugin.dll and ccptestplugin2.dll
::   - separete logs are created for testcases 10.1 and 10.2
::   - wmic might cause some problems when running the first time

@echo off
setlocal

:: Timeout value for testing in seconds
set testing_timeout=1800

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
   
for /f "tokens=1,2 delims=: " %%i in ("%time%") do set hours=%%i
for /f "tokens=1,2 delims=: " %%i in ("%time%") do set minutes=%%j
  
echo Time: %hours%:%minutes%, testing %testbuild%

call :build_components

call :move_svp_and_cs_plugins

echo Creating autoexec.bat...
copy \epoc32\winscw\c\ccetests.bat \epoc32\winscw\c\autoexec.bat
   
if exist \epoc32\WINSCW\C\Logs\TestFramework\cce_automated_testing_completed rd \epoc32\WINSCW\C\Logs\TestFramework\cce_automated_testing_completed

echo Running CCE test cases...
call :execute_epoc

:: Wait autoexe.bat file handle to be released
sleep 1

echo Deleting autoexec.bat...
del \epoc32\winscw\c\autoexec.bat

echo Rename test report
if exist \epoc32\WINSCW\C\logs\TestFramework\testreport_all_cce_cases.html (
   echo Renaming test report...
   ren \epoc32\WINSCW\C\logs\TestFramework\testreport_all_cce_cases.html %date%_%hours%.%minutes%_cce_report.html )
    
:noplugin_cases

:: This test case is run with only PS plugin available (test plugin is renamed)
echo Running test case 10.2

if exist \epoc32\release\winscw\%testbuild%\ccptestplugin2.dll (
    echo Deleting ccptestplugin2 so that it won't be available
    del \epoc32\release\winscw\udeb\ccptestplugin2.dll
) else (
   echo Error, ccptestplugin2 not found!
   goto end )
  
echo Create autoexec.bat for 10.2
echo atsinterface.exe -testmodule testscripter -config c:\testframework\10_02_noplugin_ps_emergency_call.txt > \epoc32\WINSCW\C\autoexec.bat
echo md c:\logs\testframework\cce_automated_testing_completed >> \epoc32\WINSCW\C\autoexec.bat

call :execute_epoc

:: Wait autoexe.bat file handle to be released
sleep 1

echo Deleting autoexec.bat...
del \epoc32\winscw\c\autoexec.bat

echo Rename testreport 10.2
if exist \epoc32\WINSCW\C\logs\TestFramework\testreport.txt (
   echo Renaming test report...
   ren \epoc32\WINSCW\C\logs\TestFramework\testreport.txt %date%_%hours%.%minutes%_cce_report_tc_10_2.txt )
    
:: This test case is run without any plugins
echo Running test case 10.2

if exist \epoc32\release\winscw\%testbuild%\ccptestplugin.dll (
   echo Deleting ccptestplugin so that it won't be available
   del \epoc32\release\winscw\udeb\ccptestplugin.dll
) else (
   echo Error, ccptestplugin not found!
   goto end )
   
echo Create autoexec.bat for 10.1
echo atsinterface.exe -testmodule testscripter -config c:\testframework\10_01_noplugin_opennewcall.txt > \epoc32\WINSCW\C\autoexec.bat
echo md c:\logs\testframework\cce_automated_testing_completed >> \epoc32\WINSCW\C\autoexec.bat

call :execute_epoc

:: Wait autoexe.bat file handle to be released
sleep 1

echo Deleting autoexec.bat...
del \epoc32\winscw\c\autoexec.bat

echo Rename testreport 10.1
if exist \epoc32\WINSCW\C\logs\TestFramework\testreport.txt (
   echo Renaming test report...
   ren \epoc32\WINSCW\C\logs\TestFramework\testreport.txt %date%_%hours%.%minutes%_cce_report_tc_10_1.txt )
    
call :restore_plugins

goto end
    
:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
:: subs
:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

:build_components

echo Check is cce built
   echo Building CCE
   pushd .
   cd ..\..\..\..\..\group
   call bldmake bldfiles
   call abld build winscw %testbuild%
   popd )

echo Check is ccetester built
   echo Building ccetester
   call bldmake bldfiles
   call abld build winscw %testbuild% 

echo Check test plugins
   echo Building ccptestplugin
   pushd .
   cd ..\..\ccptestplugin\group
   call bldmake bldfiles
   call abld build winscw %testbuild%
   popd 

echo Check ccptestplugin2
   echo Building ccptestplugin2
   pushd .
   
   cd ..\..\ccptestplugin\group
   call create_cs_plugin.bat nopause
   
   cd ..\..\ccptestplugin2\group
   call bldmake bldfiles
   call abld build winscw %testbuild%
   popd 

goto end
   
:move_svp_and_cs_plugins

echo Moving cs and voip plugins
if exist \epoc32\release\winscw\%testbuild%\svp.dll (
   move \epoc32\release\winscw\%testbuild%\svp.dll \ )
if exist \epoc32\release\winscw\%testbuild%\csplugin.dll (
   move \epoc32\release\winscw\%testbuild%\csplugin.dll \ )
goto end

:restore_plugins

echo Restoring svp and csplugin
if not exist \epoc32\release\winscw\%testbuild%\svp.dll (
   move \svp.dll \epoc32\release\winscw\%testbuild% )
if not exist \epoc32\release\winscw\%testbuild%\csplugin.dll (
   move \csplugin.dll \epoc32\release\winscw\%testbuild% )
goto end

:execute_epoc

echo Running epoc.exe
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
goto end

:end