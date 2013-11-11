/*
* Copyright (c) 2006 Nokia Corporation and/or its subsidiary(-ies).
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
* Description:   TNE Video Clip Implementation
*
*/




#include "TNEVideoClipInfoImp.h"
#include "TNEProcessor.h"
#include <fbs.h>
#include <coemain.h>
#include <ecom/ecom.h>

// Print macro
#ifdef _DEBUG
#include <e32svr.h>
#define PRINT(x) RDebug::Print x
#else
#define PRINT(x)
#endif


// -----------------------------------------------------------------------------
// CTNEVideoClipInfo::NewL
// API exposed by TNE to create TNEVideoClipInfo using RFile handle
// -----------------------------------------------------------------------------
EXPORT_C CTNEVideoClipInfo* CTNEVideoClipInfo::NewL(const RFile& aFileHandle,
                                                    MTNEVideoClipInfoObserver& aObserver)
{
    PRINT(_L("CTNEVideoClipInfo::NewL"));

    CTNEVideoClipInfoImp* self = (CTNEVideoClipInfoImp*)NewLC(aFileHandle, aObserver);
    CleanupStack::Pop(self);

    PRINT(_L("CTNEVideoClipInfo::NewL out"));
    return self;
}

// -----------------------------------------------------------------------------
// CTNEVideoClipInfo::NewL
// API exposed by TNE to create TNEVideoClipInfo using file name
// -----------------------------------------------------------------------------
EXPORT_C CTNEVideoClipInfo* CTNEVideoClipInfo::NewL(const TDesC& aFileName,
                                                    MTNEVideoClipInfoObserver& aObserver)
{
    PRINT(_L("CTNEVideoClipInfo::NewL"));

    CTNEVideoClipInfoImp* self = (CTNEVideoClipInfoImp*)NewLC(aFileName, aObserver);
    CleanupStack::Pop(self);

    PRINT(_L("CTNEVideoClipInfo::NewL out"));
    return self;
}

EXPORT_C CTNEVideoClipInfo* CTNEVideoClipInfo::NewLC(const RFile& aFileHandle,
                                                     MTNEVideoClipInfoObserver& aObserver)
{
    PRINT(_L("CTNEVideoClipInfo::NewLC in"));
 	
    CTNEVideoClipInfoImp* self = new (ELeave) CTNEVideoClipInfoImp();
    CleanupStack::PushL(self);
    self->ConstructL(aFileHandle, aObserver);

    PRINT(_L("CTNEVideoClipInfo::NewLC out"));
    return self;
}
    
EXPORT_C CTNEVideoClipInfo* CTNEVideoClipInfo::NewLC(const TDesC& aFileName,
                                                     MTNEVideoClipInfoObserver& aObserver)
{
    PRINT(_L("CTNEVideoClipInfo::NewLC in"));

    CTNEVideoClipInfoImp* self = new (ELeave) CTNEVideoClipInfoImp();
    CleanupStack::PushL(self);
    self->ConstructL(aFileName, aObserver);

    PRINT(_L("CTNEVideoClipInfo::NewLC out"));
    return self;
}




CTNEVideoClipInfo* CTNEVideoClipInfoImp::NewL(const RFile& aFileHandle, 
                                              MTNEVideoClipInfoObserver& aObserver)
{
    CTNEVideoClipInfoImp* self = new (ELeave) CTNEVideoClipInfoImp();
    CleanupStack::PushL(self);
    self->ConstructL(aFileHandle, aObserver);
    CleanupStack::Pop(self);
    return self;
}

CTNEVideoClipInfoImp::CTNEVideoClipInfoImp()
        : iReady(EFalse)
{
	iFs = NULL;
}


TInt CTNEVideoClipInfoImp::VideoFrameCount() const
{
    __ASSERT_ALWAYS(iReady, 
                    TTNEPanic::Panic(TTNEPanic::EVideoClipInfoNotReady));

    return iVideoFrameCount;
}


void CTNEVideoClipInfoImp::ConstructL(const RFile& aFileHandle,
                                      MTNEVideoClipInfoObserver& aObserver)
{
    PRINT(_L("CTNEVideoClipInfo::ConstructL in"));

	iFileHandle.Duplicate(aFileHandle);
	
    iThumbOperation = CTNEVideoClipThumbOperation::NewL(this);

    iInfoOperation = CTNEVideoClipInfoOperation::NewL(this, aObserver);
    
    PRINT(_L("CTNEVideoClipInfo::ConstructL out"));
}

void CTNEVideoClipInfoImp::ConstructL(const TDesC& aFileName,
                                      MTNEVideoClipInfoObserver& aObserver)
{
    PRINT(_L("CTNEVideoClipInfo::ConstructL in"));
    
    // Get RFile Handle
    iFs = new (ELeave) RFs;
    User::LeaveIfError(iFs->Connect());

	TInt iError = iFileHandle.Open(*iFs, aFileName, EFileShareReadersOnly | EFileStream | EFileRead);

    iThumbOperation = CTNEVideoClipThumbOperation::NewL(this);

    iInfoOperation = CTNEVideoClipInfoOperation::NewL(this, aObserver);
    
    PRINT(_L("CTNEVideoClipInfo::ConstructL out"));
}


CTNEVideoClipInfoImp::~CTNEVideoClipInfoImp()
{
    PRINT(_L("CTNEVideoClipInfo::~CTNEVideoClipInfoImp in"));

    delete iInfoOperation;
    delete iThumbOperation;

	iFileHandle.Close();
	
    if(iFs)
    {
	    iFs->Close();
	    delete iFs;    	
    }
    
    REComSession::FinalClose();

    PRINT(_L("CTNEVideoClipInfo::~CTNEVideoClipInfoImp out"));
}


void CTNEVideoClipInfoImp::GetThumbL(MTNEVideoClipThumbObserver& aObserver, 
                                     TInt aIndex,
                                     TSize* const aResolution,
                                     TDisplayMode aDisplayMode,
                                     TBool aEnhance,
                                     TInt aPriority)
{
    PRINT(_L("CTNEVideoClipInfoImp::GetThumbL in"));

    __ASSERT_ALWAYS(iReady, 
                    TTNEPanic::Panic(TTNEPanic::EVideoClipInfoNotReady));

    iThumbOperation->StartL(aObserver, aIndex, aResolution, aDisplayMode, aEnhance, aPriority);
    
    PRINT(_L("CTNEVideoClipInfoImp::GetThumbL out"));
}


void CTNEVideoClipInfoImp::CancelThumb()
{
    PRINT(_L("CTNEVideoClipInfoImp::CancelThumb in"));

    if ( !iReady )
        {
        PRINT(_L("CTNEVideoClipInfoImp::CancelThumb not even info ready yet, cancel it and get out"));
        iInfoOperation->Cancel();
        return;
        }

    iThumbOperation->Cancel();

    PRINT(_L("CTNEVideoClipInfoImp::CancelThumb out"));
}


TPtrC CTNEVideoClipInfoImp::FileName() const
{
    __ASSERT_ALWAYS(iReady, 
                    TTNEPanic::Panic(TTNEPanic::EVideoClipInfoNotReady));

	TFileName fileName;
	iFileHandle.FullName(fileName);

    return fileName;
}


TSize CTNEVideoClipInfoImp::GetResolution() const
{
    __ASSERT_ALWAYS(iReady, 
                    TTNEPanic::Panic(TTNEPanic::EVideoClipInfoNotReady));

    return iResolution;
}

/**********************************************
***        Video Clip Info Operations        **
***                                          **
**********************************************/

CTNEVideoClipInfoOperation* CTNEVideoClipInfoOperation::NewL(CTNEVideoClipInfoImp* aInfo,
                                                             MTNEVideoClipInfoObserver& aObserver)
    {
    PRINT(_L("CTNEVideoClipInfoOperation::NewL in"));

    CTNEVideoClipInfoOperation* self = 
        new (ELeave) CTNEVideoClipInfoOperation(aInfo, aObserver);
    CleanupStack::PushL(self);
    self->ConstructL();
    CleanupStack::Pop(self);

    PRINT(_L("CTNEVideoClipInfoOperation::NewL out"));
    return self;
    }


CTNEVideoClipInfoOperation::CTNEVideoClipInfoOperation(CTNEVideoClipInfoImp* aInfo,
                                                       MTNEVideoClipInfoObserver& aObserver)
        : CActive(EPriorityStandard), iInfo(aInfo), iTNEProcessorError(KErrNone)
    {
    PRINT(_L("CTNEVideoClipInfoOperation::CTNEVideoClipInfoOperation in"));

    iObserver = &aObserver;
    CActiveScheduler::Add(this);

    PRINT(_L("CTNEVideoClipInfoOperation::CTNEVideoClipInfoOperation out"));
    }


void CTNEVideoClipInfoOperation::ConstructL()
    {
    PRINT(_L("CTNEVideoClipInfoOperation::ConstructL in"));

    CTNEProcessor* processor = CTNEProcessor::NewL();
    TRAP(iTNEProcessorError,
         processor->GetVideoClipPropertiesL((iInfo->iFileHandle), 
                                            iInfo->iFormat,
                                            iInfo->iVideoType,
                                            iInfo->iResolution,
                                            iInfo->iVideoFrameCount
                                            ));    
    delete processor;
    processor = 0;

    PRINT((_L("CTNEVideoClipInfoOperation::ConstructL GetVideoClipInfo returned: %d "), iTNEProcessorError ));

    if ((iTNEProcessorError == KErrNone)
            || (iTNEProcessorError == KErrNotFound) 
            || (iTNEProcessorError == KErrPathNotFound)
            || (iTNEProcessorError == KErrNotSupported))
        {
        // Report also certain common error conditions to the observer instead of leaving
        SetActive();
        TRequestStatus* status = &iStatus;
        User::RequestComplete(status, KErrNone);
        }
    else
        {
        User::Leave(iTNEProcessorError);
        }

    PRINT(_L("CTNEVideoClipInfoOperation::ConstructL out"));
    }


CTNEVideoClipInfoOperation::~CTNEVideoClipInfoOperation()
    {
    Cancel();
    }

void CTNEVideoClipInfoOperation::RunL()
    {
    PRINT(_L("CTNEVideoClipInfoOperation::RunL in"));

    if (iTNEProcessorError != KErrNone) 
       {
         iObserver->NotifyVideoClipInfoReady(*iInfo, iTNEProcessorError);
       }
    else
       {
         if (iTNEProcessorError == KErrNone) 
            {
              iInfo->iReady = ETrue;
            }
         iObserver->NotifyVideoClipInfoReady(*iInfo, iTNEProcessorError);
       }

    PRINT(_L("CTNEVideoClipInfoOperation::RunL out"));
    }


// ***********************************************************************
// This function gets called by the Cancel operation of the Active object.
//  Standard implementation of the cancel operation on a active object
// ***********************************************************************

void CTNEVideoClipInfoOperation::DoCancel()
    {
    }



/**********************************
***      Thumb Operations        **
***                              **
***********************************/

CTNEVideoClipThumbOperation* CTNEVideoClipThumbOperation::NewL(CTNEVideoClipInfoImp* aInfo)
    {
    PRINT(_L("CTNEVideoClipThumbOperation::NewL in"));

    CTNEVideoClipThumbOperation* self = 
        new (ELeave) CTNEVideoClipThumbOperation(aInfo);
    
    CleanupStack::PushL(self);
    self->ConstructL();
    CleanupStack::Pop(self);

    PRINT(_L("CTNEVideoClipThumbOperation::NewL out"));
    return self;
    }


CTNEVideoClipThumbOperation::CTNEVideoClipThumbOperation(CTNEVideoClipInfoImp* aInfo)
        : CActive(EPriorityIdle), iInfo(aInfo)
    {
    PRINT(_L("CTNEVideoClipThumbOperation::CTNEVideoClipThumbOperation in"));
    CActiveScheduler::Add(this);
    PRINT(_L("CTNEVideoClipThumbOperation::CTNEVideoClipThumbOperation out"));
    }


void CTNEVideoClipThumbOperation::ConstructL()
    {
    }


CTNEVideoClipThumbOperation::~CTNEVideoClipThumbOperation()
    {
    Cancel();
    }


void CTNEVideoClipThumbOperation::StartL(MTNEVideoClipThumbObserver& aObserver,
                                         TInt aIndex,
                                         TSize* const aResolution, 
                                         TDisplayMode aDisplayMode,
                                         TBool aEnhance,
                                         TInt aPriority)
    {
    PRINT(_L("CTNEVideoClipThumbOperation::StartL in"));

    __ASSERT_ALWAYS(!IsActive(), 
        TTNEPanic::Panic(TTNEPanic::EVideoClipInfoFrameOperationAlreadyRunning));
    __ASSERT_DEBUG(iTNEProcessor == 0, TTNEPanic::Panic(TTNEPanic::EInternal));

    TSize resolution;
    if (aResolution != 0)
        {
        __ASSERT_ALWAYS(aResolution->iWidth >= 0, 
            TTNEPanic::Panic(TTNEPanic::EVideoClipInfoIllegalFrameResolution));
        __ASSERT_ALWAYS(aResolution->iHeight >= 0, 
            TTNEPanic::Panic(TTNEPanic::EVideoClipInfoIllegalFrameResolution));

        resolution = *aResolution;
        }
    else
        {
        resolution = iInfo->GetResolution();
        // need check here to see if resolution retreived from the clip is bad
        if ((resolution.iHeight <=0) || (resolution.iWidth <=0 ))
          {
        resolution.iHeight = 96;
        resolution.iWidth = 128;
          }   
        }

    // Verify that IndexFrame is within limit
    __ASSERT_ALWAYS(((aIndex >= 0) && (iInfo->VideoFrameCount() == 0) || 
                                      (aIndex < iInfo->VideoFrameCount()) || 
                                      (aIndex == KBestThumbIndex) ), 
    TTNEPanic::Panic(TTNEPanic::EVideoClipInfoIllegalVideoFrameIndex));

    iIndex = aIndex;
    
    CTNEProcessor* processor = CTNEProcessor::NewLC();  
    processor->StartThumbL((iInfo->iFileHandle), aIndex, resolution, aDisplayMode,
                           aEnhance);

    /* Initialization OK. This method cannot leave any more. Start processing. */

    CleanupStack::Pop(processor);

    iTNEProcessor = processor;
    iObserver = &aObserver;

    SetPriority(aPriority);
    SetActive();
    TRequestStatus* status = &iStatus;
    User::RequestComplete(status, KErrNone);

    PRINT(_L("CTNEVideoClipThumbOperation::StartL out"));
    }


void CTNEVideoClipThumbOperation::RunL()
    {
    PRINT(_L("CTNEVideoClipThumbOperation::RunL in"));

    __ASSERT_DEBUG(iTNEProcessor != 0, TTNEPanic::Panic(TTNEPanic::EInternal));
    __ASSERT_DEBUG(iObserver != 0, TTNEPanic::Panic(TTNEPanic::EInternal));
    
    TInt err = KErrNone;
    
    if (!iThumbRequestPending)
        TRAP(err, iTNEProcessor->ProcessThumbL(iStatus));
    
    if (err != KErrNone)
        {
        delete iTNEProcessor;
        iTNEProcessor = 0;
        
        MTNEVideoClipThumbObserver* observer = iObserver;
        iObserver = 0;
        observer->NotifyVideoClipThumbCompleted(*iInfo, err, NULL);
        return;
        }
  
  // the control flows to ih263 decoder 
  // in the next loop it gets to this runl again
    if (!iThumbRequestPending)
    {        
        iThumbRequestPending = ETrue;
        iStatus = KRequestPending;
        SetActive();        
        return;
    }
    else
    {
        MTNEVideoClipThumbObserver* observer = iObserver;
        iObserver = 0;
                
        if (iStatus == KErrNone)
        {
            CFbsBitmap* frame = 0;
            iTNEProcessor->FetchThumb(frame);
            observer->NotifyVideoClipThumbCompleted(*iInfo, KErrNone, frame);            
        }
        else
        {
            observer->NotifyVideoClipThumbCompleted(*iInfo, iStatus.Int(), NULL);            
        }
        iThumbRequestPending = EFalse;
        delete iTNEProcessor;
        iTNEProcessor = 0;
    }      
        
    PRINT(_L("CTNEVideoClipThumbOperation::RunL out"));
    }


void CTNEVideoClipThumbOperation::DoCancel()
    {
    if (iTNEProcessor != 0)
        {
        delete iTNEProcessor;
        iTNEProcessor = 0;
        
        // Cancel our internal request
        if ( iStatus == KRequestPending )
        {
            PRINT((_L("CTNEVideoClipThumbOperation::DoCancel() cancel request")));
            TRequestStatus *status = &iStatus;
            User::RequestComplete(status, KErrCancel);
        }

        MTNEVideoClipThumbObserver* observer = iObserver;
        iObserver = 0;
        observer->NotifyVideoClipThumbCompleted(*iInfo, KErrCancel, NULL);
        }
    }

