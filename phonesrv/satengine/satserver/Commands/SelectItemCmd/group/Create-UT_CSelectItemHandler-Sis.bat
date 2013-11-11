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
rem Description: This BAT copies Test DLL to local folder and makes SIS file
rem

@echo off
echo.
echo ====================================
echo.
echo Digia EUnit - Test
echo.
echo This BAT copies Test DLL
echo to local folder and makes SIS file
echo.
echo ====================================
echo.

:CHECK_EPOCROOT
echo.
echo - verify epoc root
set EPOCROOT
if errorlevel == 1 goto END_ERROR 

:COPY_DLL
echo.
echo - copy test dll to current directory
copy %EPOCROOT%epoc32\release\thumb\urel\UT_CSelectItemHandler.dll .
if errorlevel == 1 goto END_ERROR 

:CREATE_DIR
echo.
echo - create Sis folder (created only if needed)
mkdir ..\Sis

:MAKESIS
echo.
echo - makesis UT_CSelectItemHandler.pkg ..\sis\UT_CSelectItemHandler.sis
call makesis UT_CSelectItemHandler.pkg ..\sis\UT_CSelectItemHandler.sis
if errorlevel == 1 goto END_ERROR

:DELETE_DLL
echo.
echo - remove the copied dll from current folder
del UT_CSelectItemHandler.dll
goto END

:END_ERROR
echo.
echo Sis creation failed. 
goto FINAL_END

:END
echo.
echo Sis file created succesfully
echo ============================

:FINAL_END
echo.

