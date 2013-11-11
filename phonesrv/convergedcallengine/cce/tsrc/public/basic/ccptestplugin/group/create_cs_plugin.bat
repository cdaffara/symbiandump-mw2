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
rem Description: Creates cs plugin.
rem

@echo off
echo This scripts creates ccptestplugin2

if /i [%1] NEQ [nopause] (
echo If the folder exists already, files will be overwritten
pause )

xcopy /SI ..\..\ccptestplugin ..\..\ccptestplugin2

if exist ..\..\ccptestplugin2\group\bld.inf.orig del ..\..\ccptestplugin2\group\bld.inf.orig

ren ..\..\ccptestplugin2\group\bld.inf bld.inf.orig
ren ..\..\ccptestplugin2\group\bld_inf2.txt bld.inf

echo Done!

if /i [%1] NEQ [nopause] ( pause )
