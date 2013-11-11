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

set _testbuildlist=T_MAIL T_DB
set _testrunlist=T_MAIL T_DB

if "%1"=="BLD" goto end
echo Creating mtm directories
if not exist \epoc32\release\wins\deb\z\nul md \epoc32\release\wins\deb\z
if not exist \epoc32\release\wins\deb\z\system\nul md \epoc32\release\wins\deb\z\system
if not exist \epoc32\release\wins\deb\z\system\mtm\nul md \epoc32\release\wins\deb\z\system\mtm
if exist \epoc32\release\wins\deb\txtc.dll copy \epoc32\release\wins\deb\txtc.dll \epoc32\release\wins\deb\z\system\mtm
if not exist \epoc32\release\wins\rel\z\nul md \epoc32\release\wins\rel\z
if not exist \epoc32\release\wins\rel\z\system\nul md \epoc32\release\wins\rel\z\system
if not exist \epoc32\release\wins\rel\z\system\mtm\nul md \epoc32\release\wins\rel\z\system\mtm
if exist \epoc32\release\wins\rel\txtc.dll copy \epoc32\release\wins\rel\txtc.dll \epoc32\release\wins\rel\z\system\mtm
if not exist \epoc32\release\wins\udeb\z\nul md \epoc32\release\wins\udeb\z
if not exist \epoc32\release\wins\udeb\z\system\nul md \epoc32\release\wins\udeb\z\system
if not exist  \epoc32\release\wins\udeb\z\system\mtm\nul md \epoc32\release\wins\udeb\z\system\mtm
if exist \epoc32\release\wins\udeb\txtc.dll copy \epoc32\release\wins\udeb\txtc.dll \epoc32\release\wins\udeb\z\system\mtm
if not exist \epoc32\release\wins\urel\z\nul md \epoc32\release\wins\urel\z
if not exist \epoc32\release\wins\urel\z\system\nul md \epoc32\release\wins\urel\z\system
if not exist \epoc32\release\wins\urel\z\system\mtm\nul md \epoc32\release\wins\urel\z\system\mtm
if exist \epoc32\release\wins\urel\txtc.dll copy \epoc32\release\wins\urel\txtc.dll \epoc32\release\wins\urel\z\system\mtm
if not exist \epoc32\wins\c\nul md \epoc32\wins\c
if not exist \epoc32\wins\c\system\nul md \epoc32\wins\c\system
if not exist \epoc32\wins\c\system\mtm\nul md \epoc32\wins\c\system\mtm

:end
