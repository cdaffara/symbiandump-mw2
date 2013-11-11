@echo off
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
rem Description:
rem
@echo off

if "%1"=="winscw" (
call runtest.cmd ut_dialpademergencycalleventfilter
call runtest.cmd ut_dialpadmultitaphandler
call runtest.cmd mt_dialpad
call runtest.cmd ut_dialpadvoicemailboxeventfilter
call runtest.cmd ut_dialpadvideomailboxeventfilter
call runtest.cmd mt_keyhandler
call runtest.cmd ut_dialpadbluetootheventfilter
call runtest.cmd ut_dialpadkeysequenceeventfilter
call runtest.cmd ut_dialpadkeyhandler
call runtest.cmd ut_dialpadhasheventfilter
)

@echo off
if "%1"=="win32" (
call runtest_w32.cmd ut_dialpadmultitaphandler
call runtest_w32.cmd mt_dialpad
)

@echo off
if "%2"=="ctc" (
call ctcpost^
 ut_dialpad\mon.sym ut_dialpad\mon.dat^
  -p profile.txt
call ctc2html -i profile.txt
)

@echo off
if "%1"=="" echo Usage: 'runall winscw' or 'runall win32'

