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
* Description:   Class for handling SUPL_RESPONSE message.
*
*/



#include "epos_comasuplresponse.h"
#include "epos_comasuplsessionid.h"

#include "epos_comasuplresponsestate.h"
#include "epos_omasuplconstants.h"
#include "epos_comasupltrace.h"

_LIT(KTraceFileName,"SUPL_OMA_SESSION::EPos_COMASuplResponseState.cpp");

// ============================ MEMBER FUNCTIONS ===============================

// -----------------------------------------------------------------------------
// COMASuplResponseState::COMASuplResponseState
// C++ constructor can NOT contain any code, that might leave.
// -----------------------------------------------------------------------------
//
COMASuplResponseState::COMASuplResponseState()
    {
			iMessageTimeOut = 0; 
			iCurrentState = ESUPL_RESPONSE;   
    }

// -----------------------------------------------------------------------------
// COMASuplResponseState::~COMASuplResponseState
// -----------------------------------------------------------------------------
//
COMASuplResponseState::~COMASuplResponseState()
    {
    }

// -----------------------------------------------------------------------------
// COMASuplResponseState::ConstructL
// Symbian 2nd phase constructor can leave.
// -----------------------------------------------------------------------------
//
void COMASuplResponseState::ConstructL()
    {
    	COMASuplState::BaseConstructL();
    	iTrace->Trace(_L("COMASuplResponseState::ConstructL..."), KTraceFileName, __LINE__); 							
    }

// -----------------------------------------------------------------------------
// COMASuplResponseState::NewL
// Two-phased constructor.
// -----------------------------------------------------------------------------
//
COMASuplResponseState* COMASuplResponseState::NewL()
    {
	    COMASuplResponseState* self =  new ( ELeave ) COMASuplResponseState();
	    CleanupStack::PushL( self );
	    self->ConstructL();
	    CleanupStack::Pop(self);
	    return self;
    }

// -----------------------------------------------------------------------------
// COMASuplResponseState::ProcessMessage
// process message...
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
TInt COMASuplResponseState::ProcessMessageL(COMASuplAsnMessageBase*  aDecodedAsnMessage )
{		
	TInt error = KErrNone;
	
		COMASuplResponse* OMASuplResponse = COMASuplResponse::NewL(); 
		OMASuplResponse->Clone(aDecodedAsnMessage,error);

		COMASuplSLPAddress* SLPaddress = NULL; 
		error = OMASuplResponse->SLPAddress(SLPaddress);
		if(error == KErrNone)
			{
				error = KErrOMANonProxyModeNotSupported;
			}
		else
			{
				error = KErrNone;
			}	
			 
		iPosMethod = OMASuplResponse->SuplResponse();
		TBuf<256> msg;
		msg.Append(_L(" Positioning Technology desired by SLP  :"));
		
		switch(iPosMethod)
			{
				case EOMAAgpsSETAssisted : msg.Copy(_L("Agps SETAssisted")); break;
			    case EOMAAgpsSETBased: 	   msg.Copy(_L("Agps SETBased")); break;
			    case EOMAAgpsSETAssistedPref: msg.Copy(_L("Agps SETAssistedPref")); break;
			    case EOMAAgpsSETBasedPref: msg.Copy(_L("Agps SETBasedPref")); break;
			    case EOMAAutonomousGps: msg.Copy(_L("AutonomousGps")); break;
			    case EOMAFLT: msg.Copy(_L("FLT")); break;  
			    case EOMAeCID: msg.Copy(_L("eCID")); break;    		
			    case EOMAeOTD: msg.Copy(_L("eOTD")); break;    		 		
			    case EOMAoTDOA: msg.Copy(_L("oTDOA")); break;    		 		
			    case EOMANoPosition: msg.Copy(_L("NoPosition")); break;    		 		
			    default:msg.Copy(_L("Error in POS Method")); break;    		 		
			}
			
    	iTrace->Trace(msg, KTraceFileName, __LINE__); 
			delete OMASuplResponse; 
			return error;
}

TOMASuplPosMethod COMASuplResponseState::GetPosMethod()
{
	TBuf<128> msg(_L("ResponseState::GetPosMethod returning:"));
	msg.AppendNum(iPosMethod);
	iTrace->Trace(msg, KTraceFileName, __LINE__); 
	return iPosMethod;
}
//  End of File

