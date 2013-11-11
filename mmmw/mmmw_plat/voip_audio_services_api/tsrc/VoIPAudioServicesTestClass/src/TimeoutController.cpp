/*
 * Copyright (c) 2008 Nokia Corporation and/or its subsidiary(-ies).
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
 * Description: voip audio service -
 *
 */

#include "VoIPAudioServicesTestClass.h"
#include "debug.h"

/*
 -------------------------------------------------------------------------------

 Class: CSimpleTimeout

 Method: CSimpleTimeout

 Description: voip audio service - Default constructor

 C++ default constructor can NOT contain any code, that might leave.

 Parameters: None

 Return Values: None

 -------------------------------------------------------------------------------
 */
CSimpleTimeout::CSimpleTimeout() :
    CActive(CActive::EPriorityStandard)
    {
    FTRACE(FPrint(_L("CSimpleTimeout::CSimpleTimeout")));
    }

/*
 -------------------------------------------------------------------------------

 Class: CSimpleTimeout

 Method: ConstructL

 Description: voip audio service - Symbian OS second phase constructor

 Symbian OS default constructor can leave.

 Parameters:

 Return Values: None

 -------------------------------------------------------------------------------
 */
void CSimpleTimeout::ConstructL(MTimeoutObserver* aObserver,
        CStifLogger* aLogger)
    {
    FTRACE(FPrint(_L("CSimpleTimeout::ConstructL")));
    iObserver = aObserver;
    iLog = aLogger;
    iTimer.CreateLocal();
    iTestCaseTimeout = 0; // Initialize

    // Add to active scheduler
    CActiveScheduler::Add(this);
    }

/*
 -------------------------------------------------------------------------------

 Class: CSimpleTimeout

 Method: NewL

 Description: voip audio service - Two-phased constructor.

 Parameters: const TTestReportOutput aReportOutput: in: Report output type

 Return Values: CSimpleTimeout* : pointer to created object

 Errors/Exceptions: Leaves if memory allocation for object fails
 Leaves if ConstructL leaves

 -------------------------------------------------------------------------------
 */
CSimpleTimeout* CSimpleTimeout::NewL(MTimeoutObserver* aTestClass,
        CStifLogger* aLogger)
    {
    FTRACE(FPrint(_L("CSimpleTimeout::NewL")));
    CSimpleTimeout* self = new (ELeave) CSimpleTimeout();
    CleanupStack::PushL(self);
    self->ConstructL(aTestClass, aLogger);
    CleanupStack::Pop(self);
    return self;
    }

/*
 -------------------------------------------------------------------------------

 Class: CSimpleTimeout

 Method: ~CSimpleTimeout

 Description: voip audio service - Destructor.

 Cancel request

 Parameters: None

 Return Values: None

 Errors/Exceptions: None

 Status: Approved

 -------------------------------------------------------------------------------
 */
CSimpleTimeout::~CSimpleTimeout()
    {
    FTRACE(FPrint(_L("CSimpleTimeout::~CSimpleTimeout")));
    Cancel();
    iTimer.Close();
    }

/*
 -------------------------------------------------------------------------------

 Class: CSimpleTimeout

 Method: Start

 Description: voip audio service - Start timeout counting

 Parameters: None

 Return Values: None

 Errors/Exceptions: None

 Status: Approved

 -------------------------------------------------------------------------------
 */
void CSimpleTimeout::Start(TTimeIntervalMicroSeconds aTimeout)
    {
    FTRACE(FPrint(_L("CSimpleTimeout::Start")));
    if (IsActive())
        {
        Cancel();
        }

    // Request timer
    TTime endTime;
    endTime.HomeTime();
    endTime = endTime + aTimeout;

    TInt64 miliseconds = aTimeout.Int64();
    miliseconds /= 1000;

    TBuf<30> dateString;
    endTime.FormatL(dateString, KFormatTimeStamp);
    iLog->Log(_L("Timer=%LD ms, EndTime=%S"), miliseconds, &dateString);

    // Store absolute timeout
    iTestCaseTimeout = endTime;

    // Taken from STIF engine
    // Note: iTimer.After() method cannot use because there needed
    // TTimeIntervalMicroSeconds32 and it is 32 bit. So then cannot create
    // timeout time that is long enough. At() uses 64 bit value=>Long enough.
    iTimer.At(iStatus, endTime);
    SetActive();
    }

/*
 -------------------------------------------------------------------------------

 Class: CSimpleTimeout

 Method: Start

 Description: voip audio service - Start timeout counting

 Parameters: None

 Return Values: None

 Errors/Exceptions: None

 Status: Approved

 -------------------------------------------------------------------------------
 */
void CSimpleTimeout::Stop()
    {
    FTRACE(FPrint(_L("CSimpleTimeout::Stop")));
    if (IsActive())
        {
        Cancel();
        }
    }
/*
 -------------------------------------------------------------------------------

 Class: CSimpleTimeout

 Method: RunL

 Description: voip audio service - RunL handles completed timeouts.

 Parameters: None

 Return Values: None

 Errors/Exceptions: None

 Status: Approved

 -------------------------------------------------------------------------------
 */
void CSimpleTimeout::RunL()
    {
    FTRACE(FPrint(_L("CSimpleTimeout::RunL")));
    iLog->Log(_L("CSimpleTimeout::RunL"));
    TTime timeout;
    timeout.HomeTime();
    // Handle the abort case when system time gets changed, but timeout is
    // still valid. All other cases should timeout since they invalidate the
    // logic of the timers.
    if (iStatus == KErrAbort)
        {
        if (iTestCaseTimeout > timeout)
            {
            RDebug::Print(_L( "Absolute timer still valid. Restaring timer. iStatus: %d" ),
                    iStatus.Int());
            // Start new timer
            iStatus = KErrNone; // reset value
            iTimer.At(iStatus, iTestCaseTimeout); // restart timer
            SetActive();
            }
        else
            {
            // Absolute timer no longer valid. Must timeout.
            iLog->Log(_L("Absolute timeout no longer valid"));
            iObserver->HandleTimeout(KErrNone);
            }
        }
    else
        {
        // Status was not KErrAbort. Timing out!
        // iLog->Log(_L("CSimpleTimeout::RunL - Timeout !!"), iTimeout);
        iLog->Log(_L("Timing out"));
        iObserver->HandleTimeout(KErrNone);
        }
    }

/*
 -------------------------------------------------------------------------------

 Class: CSimpleTimeout

 Method: DoCancel

 Description: voip audio service - Cancel active request

 Parameters: None

 Return Values: None

 Errors/Exceptions: None

 Status: Approved

 -------------------------------------------------------------------------------
 */
void CSimpleTimeout::DoCancel()
    {
    FTRACE(FPrint(_L("CSimpleTimeout::DoCancel")));
    iTimer.Cancel();
    }

/*
 -------------------------------------------------------------------------------

 Class: CSimpleTimeout

 Method: RunError

 Description: Handle errors. Just let framework handle errors because
 RunL does not leave.

 Parameters: TInt aError: in: Symbian OS error: Error code

 Return Values: TInt: Symbian OS error code

 Errors/Exceptions: None

 Status: Approved

 -------------------------------------------------------------------------------
 */
TInt CSimpleTimeout::RunError(TInt aError)
    {
    FTRACE(FPrint(_L("CSimpleTimeout::RunError")));
    iLog->Log(_L("Timeout error %d"), aError);
    iObserver->HandleTimeout(aError);
    return aError;
    }

