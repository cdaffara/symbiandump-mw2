/*
* Copyright (c) 2002-2005 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:   Class for handling SUPL_TRIGGER_STOP message.
*
*/



#include <hash.h>

#include "epos_comasupltriggeredstop.h"
#include "lbs/epos_eomasuplposerrors.h"

#include "epos_comasupltrace.h"
#include "epos_comasupltriggerstopstate.h"
#include "epos_omasuplconstants.h"

#include "epos_comasuplasnbase.h"

_LIT(KTraceFileName,"SUPL_OMA_SESSION::EPos_COMASuplTriggerStopState.cpp");

// ============================ MEMBER FUNCTIONS ===============================

// -----------------------------------------------------------------------------
// COMASuplTriggerStopState::COMASuplTriggerStopState
// C++ constructor can NOT contain any code, that might leave.
// -----------------------------------------------------------------------------
//
COMASuplTriggerStopState::COMASuplTriggerStopState(COMASuplEnd::TOMASuplStatusCode aStatusCode,COMASuplAsnHandlerBase* aOMASuplAsnHandlerBase):
																	 iOMASuplAsnHandlerBase(aOMASuplAsnHandlerBase)
    {
			iMessageTimeOut = 0; 
			iCurrentState = ESUPL_TRIG_STOP;   
			iStatusCode = aStatusCode;
			iSetStatusCode = ETrue;
    }

// -----------------------------------------------------------------------------
// COMASuplTriggerStopState::COMASuplTriggerStopState
// C++ constructor can NOT contain any code, that might leave.
// -----------------------------------------------------------------------------
//
COMASuplTriggerStopState::COMASuplTriggerStopState(TInt aMsgTimeout, 
                                                    COMASuplAsnHandlerBase* aOMASuplAsnHandlerBase):
													iOMASuplAsnHandlerBase(aOMASuplAsnHandlerBase)
    {
			iMessageTimeOut = aMsgTimeout; 
			iCurrentState = ESUPL_TRIG_STOP;   
			iSetStatusCode = EFalse;
    }

// -----------------------------------------------------------------------------
// COMASuplTriggerStopState::COMASuplTriggerStopState
// C++ constructor can NOT contain any code, that might leave.
// -----------------------------------------------------------------------------
//
COMASuplTriggerStopState::COMASuplTriggerStopState(COMASuplAsnHandlerBase* aOMASuplAsnHandlerBase):iOMASuplAsnHandlerBase(aOMASuplAsnHandlerBase)
    {
			iMessageTimeOut = 0; 
			iCurrentState = ESUPL_TRIG_STOP; 
			iSetStatusCode = EFalse;  
    } 
// -----------------------------------------------------------------------------
// COMASuplTriggerStopState::~COMASuplTriggerStopState
// -----------------------------------------------------------------------------
//
COMASuplTriggerStopState::~COMASuplTriggerStopState()
    {
    }

// -----------------------------------------------------------------------------
// COMASuplTriggerStopState::ConstructL
// Symbian 2nd phase constructor can leave.
// -----------------------------------------------------------------------------
//
void COMASuplTriggerStopState::ConstructL()
    {
    	COMASuplState::BaseConstructL();
    	iTrace->Trace(_L("COMASuplTriggerStopState::ConstructL..."), KTraceFileName, __LINE__); 							
    }

// -----------------------------------------------------------------------------
// COMASuplTriggerStopState::NewL
// Two-phased constructor.
// -----------------------------------------------------------------------------
//
COMASuplTriggerStopState* COMASuplTriggerStopState::NewL(COMASuplEnd::TOMASuplStatusCode aStatusCode,COMASuplAsnHandlerBase* aOMASuplAsnHandlerBase)
    {
	    COMASuplTriggerStopState* self =  new ( ELeave ) COMASuplTriggerStopState(aStatusCode,aOMASuplAsnHandlerBase);
	    CleanupStack::PushL( self );
	    self->ConstructL();
	    CleanupStack::Pop(self);
	    return self;
    }
// -----------------------------------------------------------------------------
// COMASuplTriggerStopState::NewL
// Two-phased constructor.
// -----------------------------------------------------------------------------
//
COMASuplTriggerStopState* COMASuplTriggerStopState::NewL(COMASuplAsnHandlerBase* aOMASuplAsnHandlerBase)
    {
	    COMASuplTriggerStopState* self =  new ( ELeave ) COMASuplTriggerStopState(aOMASuplAsnHandlerBase);
	    CleanupStack::PushL( self );
	    self->ConstructL();
	    CleanupStack::Pop(self);
	    return self;
    }

// -----------------------------------------------------------------------------
// COMASuplTriggerStopState::NewL
// Two-phased constructor.
// -----------------------------------------------------------------------------
//
COMASuplTriggerStopState* COMASuplTriggerStopState::NewL(TInt aMsgTimeout, COMASuplAsnHandlerBase* aOMASuplAsnHandlerBase)
    {
	    COMASuplTriggerStopState* self =  new ( ELeave ) COMASuplTriggerStopState(aMsgTimeout, aOMASuplAsnHandlerBase);
	    CleanupStack::PushL( self );
	    self->ConstructL();
	    CleanupStack::Pop(self);
	    return self;
    }
// -----------------------------------------------------------------------------
// COMASuplTriggerStopState::GenerateMessage
// Generates SUPL_END message
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
TInt COMASuplTriggerStopState::GenerateMessageL() 
	{
		iTrace->Trace(_L("Start of COMASuplTriggerStopState::GenerateMessage"), KTraceFileName, __LINE__); 
		if(iMsgStateObserver)
			{
			iMsgStateObserver->OperationCompleteL(KErrNone);
			return KErrNone;
			}
		return KErrNone;		
	}
	
// -----------------------------------------------------------------------------
// COMASuplTriggerStopState::EncodeMessageL
// Encodes message ...returns encoded data
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
HBufC8* COMASuplTriggerStopState::EncodeMessageL(TOMASuplVersion &aSuplVersion,COMASuplSessionID* aSessionId,
										 TInt &aError)
	{
    iTrace->Trace(_L("Start of COMASuplTriggerStopState::Encode_SUPL_TRIGGER_STOP"), KTraceFileName, __LINE__);
		
    COMASuplState::EncodeMessageL(aSuplVersion,aSessionId,aError);
		
    COMASuplAsnMessageBase* OMASuplMessageBase = iOMASuplAsnHandlerBase->CreateOMASuplMessageL(COMASuplAsnMessageBase::ESUPL_TRIG_STOP);
    CleanupStack::PushL(OMASuplMessageBase);
    COMASuplTriggeredStop* OMASuplTriggerStop  =  static_cast<COMASuplTriggeredStop*>(OMASuplMessageBase);
		
    iTrace->Trace(_L("**************** SUPL TRIGGER STOP **********************"), KTraceFileName, __LINE__);
    iTrace->Trace(_L("Created COMASuplTriggerStop"), KTraceFileName, __LINE__); 							
		
    OMASuplTriggerStop->SetMessageBase(aSuplVersion,aSessionId); 
    if(iSetStatusCode)
        {
        OMASuplTriggerStop->SetSuplStatusCode(iStatusCode);		
        iTrace->Trace(_L("Status Code in SUPL_TRIGGER_STOP :"), KTraceFileName, __LINE__);
        TBuf<64> Res;			
        Res.AppendNum(iStatusCode);
        iTrace->Trace(Res, KTraceFileName, __LINE__); 
        }		
			
    iTrace->Trace(_L("Starting Encoding...SUPL_TRIGGER_STOP"), KTraceFileName, __LINE__);
    TInt error = KErrNone;
    HBufC8 *encodedBuffer = NULL;		
    TRAPD(err,encodedBuffer = iOMASuplAsnHandlerBase->EncodeL(OMASuplTriggerStop,error));
    CleanupStack::PopAndDestroy(OMASuplTriggerStop);
		
    iTrace->Trace(_L("Encoding Done..."), KTraceFileName, __LINE__); 
		
    if ( error == KErrNone && err == KErrNone )
        {
        aError = KErrNone;
        return encodedBuffer;
        }
    else
        {
        iTrace->Trace(_L("Encoding Failed."), KTraceFileName, __LINE__); 
        aError = error;	
        delete encodedBuffer;
        encodedBuffer = NULL;
        }
		 	
    return NULL;
}

// -----------------------------------------------------------------------------
// COMASuplTriggerStopState::StartTimer
// StartTimer...starts the timer ... depending on type of message
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void COMASuplTriggerStopState::StartTimer()
	{
	iTrace->Trace(_L("COMASuplTriggerStopState::StartTimer...No Use."), KTraceFileName, __LINE__);
	}

// -----------------------------------------------------------------------------
// COMASuplTriggerStopState::ProcessMessage
// process message...
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
TInt COMASuplTriggerStopState::ProcessMessageL(COMASuplAsnMessageBase* aDecodedMessage)
    {
    COMASuplTriggeredStop* OMASuplTriggerStop = static_cast <COMASuplTriggeredStop *>(aDecodedMessage);
    OMASuplTriggerStop->GetSuplStatusCode(iStatusCode);
    return KErrNone;
    }

// -----------------------------------------------------------------------------
// COMASuplTriggerStopState::CancelOperation
// Cancels Operation...Cancels operation
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void COMASuplTriggerStopState::CancelOperation() 
	{ 
	COMASuplState::CancelOperation();	
	iTrace->Trace(_L("COMASuplTriggerStopState::CancelOperation..."), KTraceFileName, __LINE__);
	}	
	
void COMASuplTriggerStopState::PrintHex(const TDesC8& 
#ifdef PRINT_MESSAGE
aBuffer
#endif
,TInt 
#ifdef PRINT_MESSAGE
aLine
#endif
)
	{
#ifdef PRINT_MESSAGE
			TBuf<256> buffer;
			TBuf<2> buff;
			_LIT16(KFormat1,"%02x");
			TInt len = aBuffer.Length();
			for(TInt i = 0 ; i <len; i++)
				{
					buff.Zero();
					buff.Format(KFormat1,aBuffer[i]);
					buffer.Append(buff);	
					buffer.Append(_L(" "));	
				}

				iTrace->Trace(buffer, KTraceFileName, aLine); 											
#endif
	}

COMASuplEnd::TOMASuplStatusCode COMASuplTriggerStopState::GetStatusCode()
    {
    return iStatusCode;
    }    
//  End of File




