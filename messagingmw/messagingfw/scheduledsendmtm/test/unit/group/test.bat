@rem
@rem Copyright (c) 2009 Nokia Corporation and/or its subsidiary(-ies).
@rem All rights reserved.
@rem This component and the accompanying materials are made available
@rem under the terms of "Eclipse Public License v1.0"
@rem which accompanies this distribution, and is available
@rem at the URL "http://www.eclipse.org/legal/epl-v10.html".
@rem
@rem Initial Contributors:
@rem Nokia Corporation - initial contribution.
@rem
@rem Contributors:
@rem
@rem Description:
@rem
@echo off
if "%1"=="" goto usage
if "%2"=="" goto usage

@echo on

md \epoc32\wins\c\logs\msgtest
md \epoc32\wins\c\logs\schsend

rem rd \schsendtest_%1 /s
md \schsendtest_%1 
md \schsendtest_%1\system\libs
md \schsendtest_%1\system\mtm
md \schsendtest_%1\logs\msgtest
md \schsendtest_%1\logs\schsend
copy \epoc32\release\%1\%2\t*.exe \schsendtest_%1
copy \epoc32\release\%1\%2\msvtestutils.dll \schsendtest_%1\system\libs
copy \msg\schsend\test\pigeon\pigmtmu.dat \schsendtest_%1\system\mtm
copy \epoc32\release\%1\%2\pigeonmtm.dll \schsendtest_%1\system\libs

@echo off

echo.
echo.
echo.
echo.
echo The SMCM test files are now stored in folder \schsendtest_%1
echo.
echo These files may now be copied onto a device for testing.
echo.

goto end

:usage

@echo off
echo.
echo USAGE: test [platform] [variant]
echo.
echo e.g.:  test thumb urel
echo        test armi udeb
echo.

:end