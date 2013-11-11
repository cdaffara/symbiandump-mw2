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
* Description:   Class for handling SUPL_TRIGGER_RESPONSE message.
*
*/



#include "epos_comasupltriggeredresponse.h"
#include "epos_comasuplsessionid.h"

#include "epos_comasupltriggerresponsestate.h"
#include "epos_omasuplconstants.h"
#include "epos_comasupltrace.h"

_LIT(KTraceFileName,"SUPL_OMA_SESSION::EPos_COMASuplTriggerResponseState.cpp");

// ============================ MEMBER FUNCTIONS ===============================

// -----------------------------------------------------------------------------
// COMASuplTriggerResponseState::COMASuplTriggerResponseState
// C++ constructor can NOT contain any code, that might leave.
// -----------------------------------------------------------------------------
//
COMASuplTriggerResponseState::COMASuplTriggerResponseState():
        iTriggerParamsPresent(EFalse)
    {
			iMessageTimeOut = 0; 
			iCurrentState = ESUPL_TRIG_RESPONSE;   
    }

// -----------------------------------------------------------------------------
// COMASuplTriggerResponseState::~COMASuplTriggerResponseState
// -----------------------------------------------------------------------------
//
COMASuplTriggerResponseState::~COMASuplTriggerResponseState()
    {
    }

// -----------------------------------------------------------------------------
// COMASuplTriggerResponseState::ConstructL
// Symbian 2nd phase constructor can leave.
// -----------------------------------------------------------------------------
//
void COMASuplTriggerResponseState::ConstructL()
    {
    	COMASuplState::BaseConstructL();
    	iTrace->Trace(_L("COMASuplTriggerResponseState::ConstructL..."), KTraceFileName, __LINE__); 							
    }

// -----------------------------------------------------------------------------
// COMASuplTriggerResponseState::NewL
// Two-phased constructor.
// -----------------------------------------------------------------------------
//
COMASuplTriggerResponseState* COMASuplTriggerResponseState::NewL()
    {
	    COMASuplTriggerResponseState* self =  new ( ELeave ) COMASuplTriggerResponseState();
	    CleanupStack::PushL( self );
	    self->ConstructL();
	    CleanupStack::Pop(self);
	    return self;
    }

// -----------------------------------------------------------------------------
// COMASuplTriggerResponseState::ProcessMessage
// process message...
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
TInt COMASuplTriggerResponseState::ProcessMessageL(COMASuplAsnMessageBase*  aDecodedAsnMessage )
{		
    iTrace->Trace(_L("COMASuplTriggerResponseState::ProcessMessageL Start"), KTraceFileName, __LINE__);
    TInt error = KErrNone;
	
    COMASuplTriggeredResponse* OMASuplResponse = static_cast <COMASuplTriggeredResponse *>(aDecodedAsnMessage);
    
    COMASuplSLPAddress* SLPaddress = NULL; 
    iTrace->Trace(_L("GetSLPAddress "), KTraceFileName, __LINE__);
    error = OMASuplResponse->GetSLPAddress(SLPaddress);
    if(error == KErrNone)
        {
        error = KErrOMANonProxyModeNotSupported;
        }
    else
        {
        error = KErrNone;
        }	
		iTrace->Trace(_L("GetPosMethod "), KTraceFileName, __LINE__);	 
    OMASuplResponse->GetPosMethod(iPosMethod);
    TBuf<256> msg;
    msg.Copy(_L(" Positioning Technology desired by SLP  :"));
		
    switch(iPosMethod)
        {
        case EOMAAgpsSETAssisted : msg.Append(_L("Agps SETAssisted")); break;
        case EOMAAgpsSETBased: 	   msg.Append(_L("Agps SETBased")); break;
        case EOMAAgpsSETAssistedPref: msg.Append(_L("Agps SETAssistedPref")); break;
        case EOMAAgpsSETBasedPref: msg.Append(_L("Agps SETBasedPref")); break;
        case EOMAAutonomousGps: msg.Append(_L("AutonomousGps")); break;
        case EOMAFLT: msg.Append(_L("FLT")); break;  
        case EOMAeCID: msg.Append(_L("eCID")); break;    		
        case EOMAeOTD: msg.Append(_L("eOTD")); break;    		 		
        case EOMAoTDOA: msg.Append(_L("oTDOA")); break;    		 		
        case EOMANoPosition: msg.Append(_L("NoPosition")); break;    		 		
        default:msg.Append(_L("Error in POS Method")); break;    		 		
        }
			
    iTrace->Trace(msg, KTraceFileName, __LINE__); 
    iTrace->Trace(_L("Calling GetTriggerParams "), KTraceFileName, __LINE__);
    error = OMASuplResponse->GetTriggerParams(iTriggerParams);
    if (error == KErrNone)
        {
        iTrace->Trace(_L("Trigger Params present"), KTraceFileName, __LINE__);
        iTriggerParamsPresent = ETrue;                    
        error = KErrNone;
        }
    else 
        {
        iTrace->Trace(_L("Trigger Params not present"), KTraceFileName, __LINE__);
        iTriggerParamsPresent = EFalse;                    
        error = KErrNone;
        }
		iTrace->Trace(_L("Calling GetReportMode "), KTraceFileName, __LINE__);
    error = OMASuplResponse->GetReportMode(iReportingMode);
    if (error == KErrNone)
        {
        iTrace->Trace(_L("Reporting mode present"), KTraceFileName, __LINE__); 
        iReportingModeParamsPresent = ETrue;                    
        error = KErrNone;
        }
    else 
        {
        iTrace->Trace(_L("Reporting mode not present"), KTraceFileName, __LINE__); 
        iReportingModeParamsPresent = EFalse;                    
        error = KErrNone;
        }
		iTrace->Trace(_L("Calling GetSuplNetworkInfo "), KTraceFileName, __LINE__);
    error = OMASuplResponse->GetSupNetworkInfo(iSupportedNetInfo);
    if (error == KErrNone)
        {
        iTrace->Trace(_L("Supported Network Information present"), KTraceFileName, __LINE__);
        iSupportedNetInfoPresent = ETrue;                    
        error = KErrNone;
        }
    else 
        {
        iTrace->Trace(_L("Supported Network Information not present"), KTraceFileName, __LINE__);
        iSupportedNetInfoPresent = EFalse;                    
        error = KErrNone;
        }
    
    iTrace->Trace(_L("COMASuplTriggerResponseState::ProcessMessageL End"), KTraceFileName, __LINE__);
    return error;
}

TOMASuplPosMethod COMASuplTriggerResponseState::GetPosMethod()
{
	TBuf<128> msg(_L("TriggerResponseState::GetPosMethod returning:"));
	msg.AppendNum(iPosMethod);
	iTrace->Trace(msg, KTraceFileName, __LINE__); 
	return iPosMethod;
}

TBool COMASuplTriggerResponseState::GetTriggerParamsFlag()
{
	TBuf<128> msg(_L("TriggerResponseState::GetTriggerParamsFlag returning: "));
	msg.AppendNum(iTriggerParamsPresent);
	iTrace->Trace(msg, KTraceFileName, __LINE__); 
	return iTriggerParamsPresent;
}

TOMASuplTriggerParams COMASuplTriggerResponseState::GetTriggerParams()
{
	iTrace->Trace(_L("COMASuplTriggerResponseState::GetTriggerParams"), KTraceFileName, __LINE__); 
	return iTriggerParams;
}
//  End of File

