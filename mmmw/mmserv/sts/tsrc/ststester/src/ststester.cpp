/*
 * Copyright (c) 2010 Nokia Corporation and/or its subsidiary(-ies).
 * All rights reserved.
 * This component and the accompanying materials are made available
 * under the terms of "Eclipse Public License v1.0"
 * which accompanies this distribution, and is available
 * at the URL "http://www.eclipse.org/legal/epl-v10.html".
 *
 * Initial Contributors:
 * Nokia Corporation - initial contribution.
 *
 * Contributors:
 *
 * Description:
 * The file contains the implementation of the STS Tester.
 */

#include "ststester.h"

#include <systemtoneservice.h>
#define PROFILE_TIME 1
#include "profileutilmacro.h"

const TInt KKeyMapPageCount = 1;

const TInt KFontSize = 15;

const TOperationsPage KKeyMap[KKeyMapPageCount] =
    {
        {
        STR("Standard controls"), 5, // default softkey index
                    {
                    // Enter
                                {
                                STR(""), KOperation_ExecuteOption
                                },
                            // Up / Down / Left / Right
                                {
                                STR(""), KOperation_PreviousOption
                                },
                                {
                                STR(""), KOperation_NextOption
                                },
                                {
                                STR(""), KOperation_PreviousOptionPage
                                },
                                {
                                STR(""), KOperation_NextOptionPage
                                },
                            // 0 - 9
                                {
                                        STR("Stop Current Alarm"),
                                        EOperation_StopCurrentAlarm
                                },
                                {
                                        STR("Play Default Beep"),
                                        EOperation_PlayDefaultBeep
                                },
                                {
                                        STR("Play Default Alarm"),
                                        EOperation_PlayDefaultAlarm
                                },
                                {
                                        STR("Play Default Beep as Alarm"),
                                        EOperation_PlayDefaultasAlarm
                                },
                                {
                                        STR("Play Incoming Call Alarm"),
                                        EOperation_PlayIncomingCallAlarm
                                },
                                {
                                        STR("Play Warning Beep"),
                                        EOperation_PlayWarningBeep
                                },
                                {
                                STR(""), KOperation_None
                                },
                                {
                                STR(""), KOperation_None
                                },
                                {
                                STR(""), KOperation_None
                                },
                                {
                                STR("Exit"), KOperation_Exit
                                }
                    }
        }
    };

void CStsTester::ExecuteL()
    {
    CStsTester* self = new (ELeave) CStsTester;
    CleanupStack::PushL(self);
    self->InitL();
    self->Main();
    CleanupStack::PopAndDestroy(self);
    }

CStsTester::CStsTester() :
    CTestAppBase(KFontSize), iPlayState(EStopped)
    {
    }

CStsTester::~CStsTester()
    {
    CSystemToneService::Delete(iSts);
    }

void CStsTester::InitL()
    {
    BaseConstructL(KKeyMap, KKeyMapPageCount);
    iSts = CSystemToneService::Create();
    }

void CStsTester::Main()
    {
    TRAP_IGNORE(MainL());
    }

void CStsTester::MainL()
    {
    _LIT(KStopCurrentAlarm, "Stop Current Alarm");
    _LIT(KPlayDefault, "Play Default Beep");
    _LIT(KPlayDefaultAlarm, "Play Default Alarm");
    _LIT(KPlayDefaultAsAlarm, "Play Default Beep as Alarm");
    _LIT(KPlayIncomingCallAlarm, "Play Incoming Call Alarm");
    _LIT(KPlayWarningBeep, "Play Warning Beep");
    _LIT(KExit, "Exit");

    bool done = false;

    while (!done)
        {
        RPointerArray<TDesC> operations;
        operations.Append(&KStopCurrentAlarm);
        operations.Append(&KPlayDefault);
        operations.Append(&KPlayDefaultAlarm);
        operations.Append(&KPlayDefaultAsAlarm);
        operations.Append(&KPlayIncomingCallAlarm);
        operations.Append(&KPlayWarningBeep);
        operations.Append(&KExit);

        TInt index = SelectFromListL(TPoint(0, 0), iDisplaySize, _L(
                "Select STS operation to perform:"), operations);

        operations.Reset();

        TPtrC operationName(STR("Play Default Beep"));

        switch (index)
            {
            case -1:
                done = true;
                break;
            case 0:
                ExecuteOperation(EOperation_StopCurrentAlarm, operationName);
                break;
            case 1:
                ExecuteOperation(EOperation_PlayDefaultBeep, operationName);
                break;
            case 2:
                ExecuteOperation(EOperation_PlayDefaultAlarm, operationName);
                break;
            case 3:
                ExecuteOperation(EOperation_PlayDefaultasAlarm, operationName);
                break;
            case 4:
                ExecuteOperation(EOperation_PlayIncomingCallAlarm, operationName);
                break;
            case 5:
                ExecuteOperation(EOperation_PlayWarningBeep, operationName);
                break;
            case 6:
                done = true;
                break;
            }
        }
    }

void CStsTester::ExecuteOperation(TInt aOperation, const TDesC& /*aOperationText*/)
    {
    switch (aOperation)
        {
        case EOperation_StopCurrentAlarm:
            {
            TAG_TIME_PROFILING_BEGIN;
            iSts->StopAlarm(iCurrentContext);
            TAG_TIME_PROFILING_END;
            PRINT_TO_CONSOLE_TIME_DIFF;
            iPlayState = EStopped;
            break;
            }
        case EOperation_PlayDefaultBeep:
            {
            TAG_TIME_PROFILING_BEGIN;
            iSts->PlayTone(CSystemToneService::EDefaultBeep);
            TAG_TIME_PROFILING_END;
            PRINT_TO_CONSOLE_TIME_DIFF;
            break;
            }
        case EOperation_PlayDefaultAlarm:
            {
            // Only play if not already playing
            if (iPlayState != EPlaying)
                {
                iPlayState = EPlaying;
                TAG_TIME_PROFILING_BEGIN;
                iSts->PlayAlarm(CSystemToneService::EClockAlarm,
                        iCurrentContext, *this);
                TAG_TIME_PROFILING_END;
                PRINT_TO_CONSOLE_TIME_DIFF;
                }
            break;
            }
        case EOperation_PlayDefaultasAlarm:
            {
            // Only play if not already playing
            if (iPlayState != EPlaying)
                {
                iPlayState = EPlaying;
                TAG_TIME_PROFILING_BEGIN;
                iSts->PlayAlarm(CSystemToneService::EDefaultBeep,
                        iCurrentContext, *this);
                TAG_TIME_PROFILING_END;
                PRINT_TO_CONSOLE_TIME_DIFF;
                }
            break;
            }
        case EOperation_PlayIncomingCallAlarm:
            {
            // Only play if not already playing
            if (iPlayState != EPlaying)
                {
                iPlayState = EPlaying;
                TAG_TIME_PROFILING_BEGIN;
                iSts->PlayAlarm(CSystemToneService::EIncomingCall,
                        iCurrentContext, *this);
                TAG_TIME_PROFILING_END;
                PRINT_TO_CONSOLE_TIME_DIFF;
                }
            break;
            }
        case EOperation_PlayWarningBeep:
            {
            // Only play if not already playing
                TAG_TIME_PROFILING_BEGIN;
                iSts->PlayTone(CSystemToneService::EWarningBeep);
                TAG_TIME_PROFILING_END;
                PRINT_TO_CONSOLE_TIME_DIFF;
            break;
            }
        default:
            {
            break;
            }
        }
    }

void CStsTester::PlayAlarmComplete(unsigned int aAlarmContext)
    {
    if (aAlarmContext == iCurrentContext)
        {
        iPlayState = EStopped;
        }
    }
