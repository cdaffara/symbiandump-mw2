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
set PATH_TO_DLL=\epoc32\release\winscw\udeb
set PATH_TO_MOVE_DLL=\epoc32\release\winscw\udeb\z\sys\bin
set SRC_PATH=\s60\mw\telephonyservices\convergedcallengine\spsettings\src,\s60\mw\telephonyservices\convergedcallengine\spsettings\backuphelper\src
set TEST_GROUP_PATH=\s60\mw\telephonyservices\convergedcallengine\spsettings\tsrc\public\basic\spsettingsUT\group
set PATH_TO_CODESCAN=\s60\mw\telephonyservices\convergedcallengine\spsettings
set PATH_TO_TEST_RESULT=\s60\mw\telephonyservices\convergedcallengine\spsettings\tsrc\public\basic\VoIPTestResults
set PATH_TO_CODESCAN_RESULT=%PATH_TO_TEST_RESULT%\codescanner

REM Run Codescanner
call codescanner -c //VAGRP006/GROUPS5/sp_ido/Static_code_analysis/SP_codescanner_config.xml %PATH_TO_CODESCAN% %PATH_TO_CODESCAN_RESULT%


REM Remove Test dlls
call del %PATH_TO_MOVE_DLL%\T_*.dll
call del %PATH_TO_MOVE_DLL%\UT_*.dll
call del %PATH_TO_MOVE_DLL%\MT_*.dll

REM Make test result folder.
call mkdir %PATH_TO_TEST_RESULT%

pushd.

REM Remove old CTC++ monitoring results
call del %PATH_TO_TEST_RESULT%\spsettingsUT_MON.dat
call del %PATH_TO_TEST_RESULT%\spsettingsUT_MON.sym

REM Clean build
call cd %TEST_GROUP_PATH%
call bldmake bldfiles
call abld test reallyclean winscw udeb

REM Compile codes with CTC++ instrumentation.
call ctcwrap -i m -C "EXCLUDE=*" -C "NO_EXCLUDE=%SRC_PATH%" abld test build winscw udeb

REM Move test DLL's
call move %PATH_TO_DLL%\T_CSPProperty.dll %PATH_TO_MOVE_DLL%
call move %PATH_TO_DLL%\T_CSPSettings.dll %PATH_TO_MOVE_DLL%
call move %PATH_TO_DLL%\T_CSPSettingsVoIPUtils.dll %PATH_TO_MOVE_DLL%
call move %PATH_TO_DLL%\T_CSPSettingsEngine.dll %PATH_TO_MOVE_DLL%
call move %PATH_TO_DLL%\ut_spsbackuphelperperformer.dll %PATH_TO_MOVE_DLL%
call move %PATH_TO_DLL%\ut_spsbackuphelpermonitor.dll %PATH_TO_MOVE_DLL%
call move %PATH_TO_DLL%\ut_spsbufferedpublisher.dll %PATH_TO_MOVE_DLL%

REM Run tests in the eunitexerunner.
call %PATH_TO_DLL%\eunitexerunner.exe -dtextshell --
call copy /Y \epoc32\WINSCW\C\shared\EUnit\Logs\Eunit_log.xml %PATH_TO_TEST_RESULT%\eunit_testreport.xml

REM Move CTC++ results to voip test result folder.
call move %TEST_GROUP_PATH%\MON.dat %PATH_TO_TEST_RESULT%\spsettingsUT_MON.dat
call move %TEST_GROUP_PATH%\MON.sym %PATH_TO_TEST_RESULT%\spsettingsUT_MON.sym

REM Remove CTC++ html reports.
call cd %PATH_TO_TEST_RESULT%
call rmdir CTCHTML /Q /S

REM Create profile.txt for test coverage analyzer summary.
call ctcpost -p profile.txt spsettingsUT_MON.sym

REM Create html reports from profile.txt
call ctc2html -i profile.txt

REM profile.txt not needed anymore
call del profile.txt

popd

echo.
echo Test results can be found at:
echo %PATH_TO_TEST_RESULT%
echo.
