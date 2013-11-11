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

md \epoc32\data\z\system\msgtest
md \epoc32\data\z\system\msgtest\urel
md \epoc32\data\z\system\msgtest\udeb
call autoprep autotest.txt defaultbat.txt

call setrel urel

if "%1"== "lubbock" goto lubbock0

call \epoc32\rom\tools\buildrom -D_SERIAL_DOWNLOAD assabet techview msgtest
del sys$rom.zip
wzzip sys$rom.zip ab_001~1.img
goto trgtest

:lubbock0
call \epoc32\rom\tools\buildrom -D_SERIAL_DOWNLOAD lubbock techview msgtest
REM del flashimg.zip
del lubbock.zip
REM wzzip flashimg.zip la_001~1.img
wzzip lubbock.zip la_001~1.img
goto trgtest

:trgtest
md logs
del logs\test.log
if "%1"== "lubbock" goto lubbock1
..\trgtest\trgtest 2 sys$rom.zip logs\test.log
goto logs

:lubbock1
REM ..\trgtest\trgtest 2 flashimg.zip logs\test.log
..\trgtest\trgtest 2 lubbock.zip logs\test.log
goto logs

:logs
cd logs
perl ..\logsplit.pl test.log
cd ..


