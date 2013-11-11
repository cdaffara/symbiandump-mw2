rem
rem Copyright (c) 2010 Nokia Corporation and/or its subsidiary(-ies).
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
rem Description:  Create QTMS UDEB SIS package
rem

@echo off
echo ----------------------------
echo Configuring for RnD SIS
echo ----------------------------
echo Start of configure time:
time /t

:CREATE_DIR
echo.
echo - create Sis folder (created only if needed)
mkdir .\SIS

:COPY_DLL
echo.
echo - Copy dlls [udeb]:
copy %EPOCROOT%epoc32\release\armv5\udeb\qtmsapi.dll .

if errorlevel == 1 goto END_ERROR

:CALL_ELFTRAIN
echo - Updating Version No [10.1] (calling elftran)
call elftran.exe -version 10.1 qtmsapi.dll

:MAKESIS
makesis ..\SIS\qtms.pkg ..\SIS\qtms.sis
signsis ..\SIS\qtms.sis ..\SIS\qtms.sisx rd.crt rd.key
if errorlevel == 1 goto END_ERROR

:DELETE_DLL
echo.
echo - remove the copied dlls from current folder
del qtmsapi.dll

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
echo End of configure time is:
time /t
