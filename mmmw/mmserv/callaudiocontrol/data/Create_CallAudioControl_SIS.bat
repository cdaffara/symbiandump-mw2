@ECHO OFF
SETLOCAL
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
rem Description:  Call Audio Control 
rem

IF "%1" == "" GOTO SYNTAX_ERROR

IF "%1" == "3.1" (
	SET SIS_FILE_SUFFIX="_S60_3.1"
	echo. Generating the SIS file for S60 3.1
) ELSE IF "%1" == "3.2" (
	SET SIS_FILE_SUFFIX="_S60_3.2_5.x"
	echo. Generating the SIS file for S60 3.2 and onwards
) ELSE (
	GOTO SYNTAX_ERROR
)

echo. Attempt to create .SIS file...
call makesis -v CallAudioControl.pkg CallAudioControl%SIS_FILE_SUFFIX%.sis

GOTO EOF

:SIS_FILE_CREATION_ERROR
echo. SIS file is not generated. Please fix the problem and run this script again !!
echo.
GOTO EOF

:SYNTAX_ERROR
echo. Usage::::
echo.
echo. %0 [S60_VERSION]
echo.
echo.     S60_VERSION : 3.1 or 3.2
echo.
echo. Eg: For creating the SIS files in a S60 3.1 SDKs
echo.
echo. 		%0 3.1
echo.
echo.     For creating the SIS files in a S60 3.2 or 5.x SDKs
echo.
echo. 		%0 3.2
echo.

GOTO EOF

:EOF
ENDLOCAL
