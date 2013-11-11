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
* Description:   Class for handling SUPL_INIT message.
*
*/



#include <utf.h>
#include "epos_comasuplinit.h"
#include "epos_comasuplnotification.h"
#include "epos_comasuplsessionid.h"
#include "lbs/epos_eomasuplposerrors.h"
#include "epos_tomasuplposmethod.h"

#include "epos_comasuplinitstate.h"
#include "epos_comasupltimeouttimer.h"
#include "epos_omasuplconstants.h"
#include "epos_momasuplprivacyobserver.h"
#include "epos_comasupltrace.h"
#include <featmgr.h> //APE CENTRIC

_LIT(KTraceFileName,"SUPL_OMA_SESSION::EPos_COMASuplInitState.cpp");

const TInt KSUPLAPECentricArchSupport = 258;  //APE CENTRIC KFeatureIdFfLocationMwApeCentricArchSupport = 258

// ============================ MEMBER FUNCTIONS ===============================

// -----------------------------------------------------------------------------
// COMASuplInitState::COMASuplInitState
// C++ constructor can NOT contain any code, that might leave.
// -----------------------------------------------------------------------------
//
COMASuplInitState::COMASuplInitState(TInt aMsgTimeout,const TDesC8& aHSLPAddress,
									MOMASuplPrivacyObserver &aPrivacyObserver,TInt aSupportedMode, CPosNetworkPrivacy*	aNetworkPrivacy):
									iHSLPAddress(aHSLPAddress),
									iPrivacyObserver(aPrivacyObserver)
    {
		iMessageTimeOut = aMsgTimeout * KSecond; 
		iNoPosition = EFalse;  
		iCurrentState =  ESUPL_INIT;
		iSupportedMode = aSupportedMode;
		iNetworkPrivacy = aNetworkPrivacy;
		iGSMRequestId = KErrNotFound;
		iIsNotificationOnly = EFalse; //APE Centric
    }

// -----------------------------------------------------------------------------
// COMASuplInitState::~COMASuplInitState
// -----------------------------------------------------------------------------
//
COMASuplInitState::~COMASuplInitState()
    {
    	//delete iNetworkPrivacy;
      	if(iTrace)
    		iTrace->Trace(_L("COMASuplInitState::~COMASuplInitState..."), KTraceFileName, __LINE__); 							
    }

// -----------------------------------------------------------------------------
// COMASuplInitState::ConstructL
// Symbian 2nd phase constructor can leave.
// -----------------------------------------------------------------------------
//
void COMASuplInitState::ConstructL()
    {
    	COMASuplState::BaseConstructL();
    	// Create an instance of Network Privacy that later are
		// used to make privacy calls on.
		//iNetworkPrivacy 	= CPosNetworkPrivacy::NewL();
		ReadRequestIDFeatureFlagL();
	   	iTrace->Trace(_L("COMASuplInitState::ConstructL..."), KTraceFileName, __LINE__); 							
    }

// -----------------------------------------------------------------------------
// COMASuplInitState::NewL
// Two-phased constructor.
// -----------------------------------------------------------------------------
//
COMASuplInitState* COMASuplInitState::NewL(TInt aMsgTimeout,const TDesC8& aHSLPAddress,
										   MOMASuplPrivacyObserver &aPrivacyObserver,
										   TInt aSupportedMode, CPosNetworkPrivacy*	aNetworkPrivacy)
    {
	    COMASuplInitState* self =  new ( ELeave ) COMASuplInitState(aMsgTimeout,aHSLPAddress,
	    														   aPrivacyObserver,aSupportedMode,aNetworkPrivacy);
	    CleanupStack::PushL( self );
	    self->ConstructL();
	    CleanupStack::Pop(self);
	    return self;
    }


// -----------------------------------------------------------------------------
// COMASuplState::ProcessMessage
// process message...
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
TInt COMASuplInitState::ProcessMessageL(COMASuplAsnMessageBase* aDecodedMessage)
{
		TInt error = KErrNone;
		iSuplInit = static_cast <COMASuplInit *>(aDecodedMessage);
		iNotification   	= NULL;
		
		error = CheckForModeSupport();
		if(error == KErrOMANonProxyModeNotSupported)
			return error;
		
		// Check for the presence of SET Session ID
		error = CheckForSetSessionID();
		
		if(KErrNone != error)
			{
				return error;
			}
		
		error = CheckForSLPSessionID();
			if(KErrNone != error)
			{
				return error;
			}

		
		CheckForPositioningMethod();
		
		error	= iSuplInit->Notification(iNotification);
		if(!error)
			{
				error = HandlePrivacyL();
				return error;		
			}
		else
			{
			if(iNoPosition)
				{
				return KErrOMASuplNoPosition;
				}
			else
				{
				iCompletionCode = KErrNone;
				iPrivacyObserver.PrivacyCheckCompleteL(iCompletionCode);	
				return KErrNone;
				}
			}		
			
}
// -----------------------------------------------------------------------------
// COMASuplInitState::HandlePrivacy
// Cancels the generation if SUPL INIT
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
TInt COMASuplInitState::HandlePrivacyL() 
	{
		iTrace->Trace(_L("COMASuplInitState::HandlePrivacy..."), KTraceFileName, __LINE__); 					
		
		COMASuplNotification::TOMASuplNotificationType notifyType = iNotification->SuplNotification();
		switch(notifyType)
			{
				case COMASuplNotification::ENoNotificationNoVerification:
				case COMASuplNotification::EPrivacyOverride:
					{
		            iTrace->Trace(_L("COMASuplInitState::Privacy rule: No notification and no verification"), KTraceFileName, __LINE__); 					
					if(iNoPosition)
						{
							iCompletionCode = KErrOMASuplInformationOnly;
						}
					else
						{
							iCompletionCode = KErrNone;		
						}	
					iPrivacyObserver.PrivacyCheckCompleteL(iCompletionCode);
					return KErrNone;
					}
				case COMASuplNotification::ENotificationOnly:
					{
		            iTrace->Trace(_L("COMASuplInitState::Privacy rule: Notification Only"), KTraceFileName, __LINE__); 					
							iIsNotificationOnly = ETrue; //APE Centric			
					MakeGSMNotificationL();
					
					if(iNoPosition)
						{
						iCompletionCode = KErrOMASuplInformationOnly;
						}
					else
						{
						iCompletionCode = KErrNone;		
						}	
					iPrivacyObserver.PrivacyCheckCompleteL(iCompletionCode);
					return KErrNone;
					}
				case COMASuplNotification::ENotificationAndVerficationAllowedNA:
					{
		            iTrace->Trace(_L("COMASuplInitState::Privacy rule: Notification & Verification, Allowed on no answer"), KTraceFileName, __LINE__); 					
					return MakeGSMVerificationL(CPosNetworkPrivacy::EDecisionAccepted);	
					}
				case COMASuplNotification::ENotificationAndVerficationDeniedNA:
					{
		            iTrace->Trace(_L("COMASuplInitState::Privacy rule: Notification & Verification, Denied on no answer"), KTraceFileName, __LINE__); 					
					return MakeGSMVerificationL(CPosNetworkPrivacy::EDecisionRejected);
					}
				default:
					{
					 return KErrArgument;
					}
			}
	}
// ---------------------------------------------------------
// COMASuplInitState::MakeGSMNotificationL
//
// (other items were commented in a header).
// ---------------------------------------------------------
//
TInt COMASuplInitState::MakeGSMNotificationL()
	{
	// Constructing GSM Privacy Request Information object
	CPosSUPLPrivacyRequestInfo* suplReqInfo = 
		CPosSUPLPrivacyRequestInfo::NewLC();
	
	HBufC* requestor = NULL;
	TInt reqError = RequestorL(requestor);
	if(!reqError)
		{
		// Set name of Requestor
		if(requestor)
			suplReqInfo->SetRequestorL(*requestor,iIdType);

		delete requestor;	
		}

	HBufC* client = NULL;
	TInt clientError = ClientL(client);
	if(!clientError)
		{
			// Set name of Client
			if(client)
			{
				suplReqInfo->SetLCSClientL(*client,iIdType);	
			}

		
		delete client;
		}
	
	if(!reqError || !clientError)
		{
		// Notify Network Privacy of the request information
		// Synchronous call 
		TInt RequestId;
		TRAP_IGNORE(iNetworkPrivacy->NotifyLocationRequestL(*suplReqInfo,RequestId);)
		
		if(iIsNotificationOnly)
		{
				iGSMRequestId = RequestId;
				TBuf<64> msg;
				msg.Copy(_L("1.0 Privacy Fw Generated Notification Request id:"));
				msg.AppendNum(iGSMRequestId);
				iTrace->Trace(msg, KTraceFileName, __LINE__);		
		}
		CleanupStack::PopAndDestroy(suplReqInfo);
		return KErrNone;
		}
	else
		{
		CleanupStack::PopAndDestroy(suplReqInfo);
		return KErrArgument;
		}
	}

// ---------------------------------------------------------
// COMASuplInitState::MakeGSMVerificationL
//
// (other items were commented in a header).
// ---------------------------------------------------------
//
TInt COMASuplInitState::MakeGSMVerificationL(CPosNetworkPrivacy::TRequestDecision aTimeOutStrategy)
	{
	// Constructing GSM Privacy Request Information object
	CPosSUPLPrivacyRequestInfo* suplReqInfo = 
		CPosSUPLPrivacyRequestInfo::NewLC();
	
	HBufC* requestor = NULL;
	TInt reqError = RequestorL(requestor);
	if(!reqError)
		{
			// Set name of Requestor
			if(requestor)
				suplReqInfo->SetRequestorL(*requestor,iIdType);
			delete requestor;	
		}

	HBufC* client = NULL;
	TInt clientError = ClientL(client);
	if(!clientError)
		{
		// Set name of Client
		if(client)
			{
				iTrace->Trace(_L("calling suplReqInfo->SetLCSClientL"), KTraceFileName, __LINE__);
				suplReqInfo->SetLCSClientL(*client,iIdType);	
			}
		
		delete client;
		}
	
	if(!reqError || !clientError)
		{
			// Verify the request information using Network Privacy
			// asynchronous call with callback function HandleVerifyComplete.
			// Argument CPosNetworkPrivacy::EDecisionRejected is to show to the user 
			// what decision the GSM Network will make if the user won't answer before 
			// a timeout comes in from GSM Network. In this case verification will 
			// be rejected.
			iTrace->Trace(_L("Calling iNetworkPrivacy->VerifyLocationRequestL"), KTraceFileName, __LINE__); 					
			
			
			TRAP_IGNORE(iNetworkPrivacy->VerifyLocationRequestL(*suplReqInfo, 
				iGSMRequestId, *this, aTimeOutStrategy);) 					
				
	        TBuf<64> msg;
	        msg.Copy(_L("1.0 Privacy Fw Generated Verification Request id:"));
	        msg.AppendNum(iGSMRequestId);
	        iTrace->Trace(msg, KTraceFileName, __LINE__);       

			StartTimer();
			CleanupStack::PopAndDestroy(suplReqInfo);
			return KErrNone;
		}
	else
		{
		CleanupStack::PopAndDestroy(suplReqInfo);
		return KErrArgument;
		}
	}

// ---------------------------------------------------------
// COMASuplInitState::ConverttoUniCodeL
//
// (other items were commented in a header).
// ---------------------------------------------------------
//
TInt COMASuplInitState::ConverttoUniCodeL(HBufC*& aUniCode,HBufC8& aRequstor)
	{
	COMASuplNotification::TOMASuplEncodingType encodingType;
	TInt error = iNotification->EncodingType(encodingType);
	if(error)
		{
		 return KErrArgument;
		}	
	else
		{
		
		for(TInt i = 0; i < aRequstor.Length(); i++)
		{
			TBuf<32> printBuffer;
			TUint16 FirstChar = aRequstor[i];
			printBuffer.AppendNum(FirstChar);
			iTrace->Trace(printBuffer,KTraceFileName, __LINE__); 												
		}
		
		switch(encodingType)
			{
				case COMASuplNotification::EUCS2:
					{
					iTrace->Trace(_L("Notification Encoding Type : EUCS2"), KTraceFileName, __LINE__); 												
					aUniCode =  HBufC::NewL(aRequstor.Length());
	    			TPtr16	requestId = aUniCode->Des();
					FormUCS2String(aRequstor,requestId);	
					break;
					}
				case COMASuplNotification::EUtf8:
					{
					iTrace->Trace(_L("Notification Encoding Type : EUtf8"), KTraceFileName, __LINE__); 											
					aUniCode =  HBufC::NewL(aRequstor.Length());
					TPtr16	requestId = aUniCode->Des();
					error = CnvUtfConverter::ConvertToUnicodeFromUtf8(requestId,aRequstor);					
					break;
					}
				case COMASuplNotification::EGsmDefault:
					{
					iTrace->Trace(_L("Notification Encoding Type : EGsmDefault"), KTraceFileName, __LINE__); 					
					aUniCode =  HBufC::NewL(aRequstor.Length() * 2);
					TPtr16	requestId = aUniCode->Des();
					RBuf8 destString;
					destString.CreateL(aRequstor.Length() * 2);	
					TBuf<64> temp;
					temp.Copy(aRequstor);
		            iTrace->Trace(temp, KTraceFileName, __LINE__); 					
					GsmDefaultDecode(aRequstor,destString);
					requestId.Copy(destString);	
					destString.Close();
					break;
					}
				default:
					{
					error = KErrArgument;
					}
			}
		}
	return error;
	}
// ---------------------------------------------------------
// COMASuplInitState::Requestor
//
// (other items were commented in a header).
// ---------------------------------------------------------
//
TInt COMASuplInitState::RequestorL(HBufC*& aRequestor)
	{
	
	COMASuplNotification::TOMASuplFormatIndicator requestType;
	HBufC8* requestor = NULL;
	// Eventhough requestor id is empty...we have to show the Privacy verification note
	TInt error = iNotification->RequestorId(requestor,requestType);
	if(error == KErrNone || error == KErrOMASuplParamNotSet )
		{
			error = KErrNone; 
			if(requestor)
			{
				error = ConverttoUniCodeL(aRequestor,*requestor);
			}
			
		switch(requestType)
				{
				case COMASuplNotification::ELogicalName:
					{
		            iTrace->Trace(_L("COMASuplInitState::RequestorL Requestor type - Logical Name"), KTraceFileName, __LINE__); 					
					iIdType = CPosSUPLPrivacyRequestInfo::EIdTypeLogicalName;
					break;	
					}
				case COMASuplNotification::EEMailAddress:
					{
		            iTrace->Trace(_L("COMASuplInitState::RequestorL Requestor type - EMail Addr"), KTraceFileName, __LINE__); 					
					iIdType = CPosSUPLPrivacyRequestInfo::EIdTypeEmailAddress;
					break;		
					}
				case COMASuplNotification::EMsisdn:
					{
		            iTrace->Trace(_L("COMASuplInitState::RequestorL Requestor type - MSISDN"), KTraceFileName, __LINE__);
					iIdType = CPosSUPLPrivacyRequestInfo::EIdTypeMSISDN;
					break;	
					}
				case COMASuplNotification::EUrl:
					{
		            iTrace->Trace(_L("COMASuplInitState::RequestorL Requestor type - URL"), KTraceFileName, __LINE__);
					iIdType = CPosSUPLPrivacyRequestInfo::EIdTypeURL;
					break;	
					}
				case COMASuplNotification::ESipUrl:
					{
		            iTrace->Trace(_L("COMASuplInitState::RequestorL Requestor type - SIP URL"), KTraceFileName, __LINE__);
					iIdType = CPosSUPLPrivacyRequestInfo::EIdTypeSIPURL;
					break;
					}
				case COMASuplNotification::EMin:
					{
		            iTrace->Trace(_L("COMASuplInitState::RequestorL Requestor type - MIN"), KTraceFileName, __LINE__);
					iIdType = CPosSUPLPrivacyRequestInfo::EIdTypeMIN;
					break;	
					}
				case COMASuplNotification::EMdn:
					{
		            iTrace->Trace(_L("COMASuplInitState::RequestorL Requestor type - MDN"), KTraceFileName, __LINE__);
					iIdType = CPosSUPLPrivacyRequestInfo::EIdTypeMDN;
					break;	
					}
				case COMASuplNotification::EIMSPublicIdentity:
					{
		            iTrace->Trace(_L("COMASuplInitState::RequestorL Requestor type - IMS Public Identity"), KTraceFileName, __LINE__);
					iIdType = CPosSUPLPrivacyRequestInfo::EIdTypeIMSPublicIdentity;
					break;	
					}
				default:
					{
					error = KErrArgument;
					break;	
					}
				}
		}

	return error;
	}
// ---------------------------------------------------------
// COMASuplInitState::Client
//
// (other items were commented in a header).
// ---------------------------------------------------------
//
TInt COMASuplInitState::ClientL(HBufC*& aClient)
	{
	COMASuplNotification::TOMASuplFormatIndicator clientType;
	HBufC8* client = NULL;
	
	TInt error = iNotification->ClientName(client,clientType);
	//if(!error)
	if(error == KErrNone || error == KErrOMASuplParamNotSet )
		{
		
			error = KErrNone; 
			if(client)
			{
				error = ConverttoUniCodeL(aClient,*client);
			}
		
		switch(clientType)
			{
			case COMASuplNotification::ELogicalName:
				{
		        iTrace->Trace(_L("COMASuplInitState::ClientL Client type - Logical Name"), KTraceFileName, __LINE__);
				iIdType = CPosSUPLPrivacyRequestInfo::EIdTypeLogicalName;
				break;	
				}
			case COMASuplNotification::EEMailAddress:
				{
		        iTrace->Trace(_L("COMASuplInitState::ClientL Client type - EMail Addr"), KTraceFileName, __LINE__);
				iIdType = CPosSUPLPrivacyRequestInfo::EIdTypeEmailAddress;
				break;		
				}
			case COMASuplNotification::EMsisdn:
				{
		        iTrace->Trace(_L("COMASuplInitState::ClientL Client type - MSISDN"), KTraceFileName, __LINE__);
				iIdType = CPosSUPLPrivacyRequestInfo::EIdTypeMSISDN;
				break;	
				}
			case COMASuplNotification::EUrl:
				{
		        iTrace->Trace(_L("COMASuplInitState::ClientL Client type - URL"), KTraceFileName, __LINE__);
				iIdType = CPosSUPLPrivacyRequestInfo::EIdTypeURL;
				break;	
				}
			case COMASuplNotification::ESipUrl:
				{
		        iTrace->Trace(_L("COMASuplInitState::ClientL Client type - SIP URL"), KTraceFileName, __LINE__);
				iIdType = CPosSUPLPrivacyRequestInfo::EIdTypeSIPURL;
				break;
				}
			case COMASuplNotification::EMin:
				{
		        iTrace->Trace(_L("COMASuplInitState::ClientL Client type - MIN"), KTraceFileName, __LINE__);
				iIdType = CPosSUPLPrivacyRequestInfo::EIdTypeMIN;
				break;	
				}
			case COMASuplNotification::EMdn:
				{
		        iTrace->Trace(_L("COMASuplInitState::ClientL Client type - MDN"), KTraceFileName, __LINE__);
				iIdType = CPosSUPLPrivacyRequestInfo::EIdTypeMDN;
				break;	
				}
			case COMASuplNotification::EIMSPublicIdentity:
				{
		        iTrace->Trace(_L("COMASuplInitState::ClientL Client type - IMS Public Identity"), KTraceFileName, __LINE__);
				iIdType = CPosSUPLPrivacyRequestInfo::EIdTypeIMSPublicIdentity;
				break;	
				}
			default:
				{
				error = KErrArgument;
				break;	
				}
			}	
		}

	return error;
	}
// ---------------------------------------------------------
// COMASuplInitState::HandleVerifyComplete
//
// (other items were commented in a header).
// ---------------------------------------------------------
//
void COMASuplInitState::HandleVerifyComplete(
	TInt /*aRequestId*/, TInt aCompletionCode)
	{
	StopTimer();
	iCompletionCode = aCompletionCode;
	
	if(iNoPosition)
		{
			if(aCompletionCode)
				{
				iCompletionCode = KErrOMASuplDenied;
				}
			else
				{
				iCompletionCode = KErrOMASuplAccepted;	
				}
		}	
		
	if(KErrNone == iCompletionCode || KErrOMASuplAccepted == iCompletionCode)
		{
				TRAP_IGNORE(MakeGSMNotificationL());		
				
		}	
		
	TRAP_IGNORE(iPrivacyObserver.PrivacyCheckCompleteL(iCompletionCode));
	
	}

// -----------------------------------------------------------------------------
// COMASuplInitState::TimerExpired
// TimerExpired...Gets called when time out occurs .
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void COMASuplInitState::TimerExpiredL()
	{
	iTrace->Trace(_L("COMASuplState::TimerExpired..."), KTraceFileName, __LINE__); 							
	
	COMASuplNotification::TOMASuplNotificationType notifyType = iNotification->SuplNotification();
	if(notifyType == COMASuplNotification::ENotificationAndVerficationAllowedNA)
	 	{
		iCompletionCode = KErrNone;
	 	}
	else if(notifyType == COMASuplNotification::ENotificationAndVerficationDeniedNA )
		{
		iCompletionCode	= KErrAccessDenied;
		}
		
	if(iNoPosition)
		{
		if(iCompletionCode)
			{
			iCompletionCode = KErrOMASuplDenied;
			}
		else
			{
			iCompletionCode = KErrOMASuplAccepted;	
			}
		}			
	// Cancel ongoing verification using Network Privacy
	iNetworkPrivacy->CancelVerifyLocationRequest(iGSMRequestId);
	if(KErrNone == iCompletionCode || KErrOMASuplAccepted == iCompletionCode)
		{
			 MakeGSMNotificationL();			
		}	
	iPrivacyObserver.PrivacyCheckCompleteL(iCompletionCode);
	}
	
// -----------------------------------------------------------------------------
//  COMASuplInitState::CheckForVersion
// CheckForVersion...Checks version of protocol & received message
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
TInt COMASuplInitState::CheckForVersion()
	{
		TOMASuplVersion version;
		COMASuplSessionID* SessionID = NULL;
		iSuplInit->MessageBase(version,SessionID);
		
		TInt recMajor,recMinor,recServInd;
		TInt actMajor,actMinor,actServInd;

		version.SuplVersion(recMajor,recMinor,recServInd);		
		iOMASuplVersion.SuplVersion(actMajor,actMinor,actServInd);		
		if(recMajor == actMajor )
			{
				return KErrNone;
			}
		else
			{
				return KErrOMASuplVersionNotMatching;			
			}
	}

// -----------------------------------------------------------------------------
//  COMASuplInitState::CheckForModeSupport
// CheckForModeSupport...Checks modes of protocol & received message
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
TInt COMASuplInitState::CheckForModeSupport()
	{
		COMASuplInit::TOMASuplSLPMode SLPMode;
		
		iSuplInit->SuplInit(iPosMethod,SLPMode);
		
		COMASuplInit::TOMASuplSLPMode SupportedMode = (COMASuplInit::TOMASuplSLPMode)iSupportedMode;
		
		if(SLPMode == SupportedMode )
			{
				return KErrNone;	
			}
			else
			{
				return KErrOMANonProxyModeNotSupported;			
			}
	}

// -----------------------------------------------------------------------------
//  COMASuplInitState::CheckForSetSessionID
// CheckForModeSupport...Checks if Set session ID is present in the SUPL INT
// message received
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
TInt COMASuplInitState::CheckForSetSessionID()
	{
	    iTrace->Trace(_L("Start COMASuplInitState::CheckForSetSessionID"), KTraceFileName, __LINE__); 							
		COMASuplSessionID* retrivedSessionID = NULL;
		iSuplInit->SessionId(retrivedSessionID);
		if(retrivedSessionID)
			{
				COMASuplSETSessionID *setSessionId = NULL;
				retrivedSessionID->SETSessionID(setSessionId);
				if(setSessionId)
					{
	                    iTrace->Trace(_L("COMASuplInitState::CheckForSetSessionID, SET Session ID present"), KTraceFileName, __LINE__); 
						return KErrSuplInvalidSessionID;
					}
				else
					{
						return KErrNone;
					}
			}
		else
			{
				return KErrSuplInvalidSessionID;
			}
	}
		
// -----------------------------------------------------------------------------
//  COMASuplInitState::CheckForSetSessionID
// CheckForModeSupport...Checks if Set session ID is present in the SUPL INT
// message received
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
TInt COMASuplInitState::CheckForSLPSessionID()
	{
		COMASuplSessionID* retrivedSessionID = NULL;
		iSuplInit->SessionId(retrivedSessionID);
		if(retrivedSessionID)
			{
				TBuf8<KSLPSessionID> sessionId;
				COMASuplSLPAddress* slpIPAddress = NULL;
				COMASuplSLPSessionID* retrivedSuplSLPID =NULL;
				retrivedSessionID->SLPSessionID(retrivedSuplSLPID);
				if(retrivedSuplSLPID)
					{
						retrivedSuplSLPID->SuplSLPSessionID(sessionId,slpIPAddress);
						if(sessionId.Length() == 0 )
							return KErrSuplInvalidSessionID;
						else
							return KErrNone;	
					}
				else
					{
						return KErrSuplInvalidSessionID;
					}
			}
		else
			{
				return KErrSuplInvalidSessionID;
			}
	}

// -----------------------------------------------------------------------------
//  COMASuplInitState::CheckForPositioningMethod
// CheckForPositioningMethod...
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void COMASuplInitState::CheckForPositioningMethod()
	{
		COMASuplInit::TOMASuplSLPMode SLPMode;
		iSuplInit->SuplInit(iPosMethod,SLPMode);
		if(EOMANoPosition == iPosMethod)
			{
				iNoPosition = ETrue;
			}
	}
TInt ConvertHexToGSMDefault(TInt aHex)	
	{
		TInt GSMDefaultEquivalentHex = aHex;
		switch(aHex)
			{
				case 0x00:GSMDefaultEquivalentHex = 0x40;break;
				case 0x01:GSMDefaultEquivalentHex = 0xA3;break;
				case 0x02:GSMDefaultEquivalentHex = 0x24;break;
				case 0x03:GSMDefaultEquivalentHex = 0xA5;break;
				case 0x04:GSMDefaultEquivalentHex = 0xE8;break;
				case 0x05:GSMDefaultEquivalentHex = 0xE9;break;
				case 0x06:GSMDefaultEquivalentHex = 0xF9;break;
				case 0x07:GSMDefaultEquivalentHex = 0xEC;break;
				case 0x08:GSMDefaultEquivalentHex = 0xF2;break;
				case 0x09:GSMDefaultEquivalentHex = 0xE7;break;
				case 0x0a:GSMDefaultEquivalentHex = 0x0A;break;
				case 0x0b:GSMDefaultEquivalentHex = 0xD8;break;
				case 0x0c:GSMDefaultEquivalentHex = 0xF8;break;
				case 0x0d:GSMDefaultEquivalentHex = 0x0D;break;
				case 0x0e:GSMDefaultEquivalentHex = 0xC5;break;
				case 0x0f:GSMDefaultEquivalentHex = 0xE5;break;
				case 0x10:GSMDefaultEquivalentHex = 0x0394;break;
				case 0x11:GSMDefaultEquivalentHex = 0x5F;break;
				case 0x12:GSMDefaultEquivalentHex = 0x03A6;break;
				case 0x13:GSMDefaultEquivalentHex = 0x0393;break;
				case 0x14:GSMDefaultEquivalentHex = 0x039B;break;
				case 0x15:GSMDefaultEquivalentHex = 0x03A9;break;
				case 0x16:GSMDefaultEquivalentHex = 0x03A0;break;
				case 0x17:GSMDefaultEquivalentHex = 0x03A8;break;
				case 0x18:GSMDefaultEquivalentHex = 0x03A3;break;
				case 0x19:GSMDefaultEquivalentHex = 0x0398;break;
				case 0x1a:GSMDefaultEquivalentHex = 0x039E;break;
				case 0x1c:GSMDefaultEquivalentHex = 0x00C6;break;
				case 0x1d:GSMDefaultEquivalentHex = 0x00E6;break;
				case 0x1e:GSMDefaultEquivalentHex = 0x00DF;break;
				case 0x1f:GSMDefaultEquivalentHex = 0x00C9;break;
				case 0x24:GSMDefaultEquivalentHex = 0xA4;break;
				
				case 0x40:GSMDefaultEquivalentHex = 0xA1;break;
				case 0x5b:GSMDefaultEquivalentHex = 0xC4;break;
				case 0x5c:GSMDefaultEquivalentHex = 0xD6;break;
				case 0x5d:GSMDefaultEquivalentHex = 0xD1;break;
				case 0x5e:GSMDefaultEquivalentHex = 0xDC;break;
				case 0x5f:GSMDefaultEquivalentHex = 0xA7;break;
				case 0x60:GSMDefaultEquivalentHex = 0xBF;break;
				
				case 0x7b:GSMDefaultEquivalentHex = 0xE4;break;
				case 0x7c:GSMDefaultEquivalentHex = 0xF6;break;
				case 0x7d:GSMDefaultEquivalentHex = 0xF1;break;
				case 0x7e:GSMDefaultEquivalentHex = 0xFC;break;
				case 0x7f:GSMDefaultEquivalentHex = 0xE0;break;
				default:break;	
			}
			
		return GSMDefaultEquivalentHex;	
	}
	
// -----------------------------------------------------------------------------
// COMASuplInitState::GsmDefaultDecode
// GsmDefaultDecode...
// This function takes packed 7 bit data and pads it out to 8 bit data
// Function will actually append data onto end of an existing buffer.
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void COMASuplInitState::GsmDefaultDecode( const TDesC8& aIn, TDes8& aOut )
	{

	TInt length = aIn.Length();
	TInt count = ( length * 8 ) / 7;

	// Get raw pointers and do unpacking
	TUint8* outPtr = (TUint8*)aOut.Ptr() + aOut.Length();
	const TUint8* inPtr = aIn.Ptr();

	TInt startBit = 0;
	// Iterate over each 7 bit element
	for ( TInt ii = 0; ii < count; ii++ )
		{
		TInt from = ( *inPtr >> startBit ) & 0x7F;
		if ( startBit )
			{
			inPtr++;
			from |= ( *inPtr << ( 8 - startBit ) ) & 0x7F;
			}
		outPtr[ ii ] = (TUint8)from;
		startBit = ( startBit + 7 ) % 8;
		}
	aOut.SetLength( aOut.Length() + count );
	length = aOut.Length();
	
	for(TInt Index = 0 ; Index < length; Index++ )
		{
			if( Index == length - 1 && (aOut[Index] == 0x0000 || aOut[Index] == 0x000D))
				{
					aOut.SetLength( Index );
					break;
				}

			aOut[Index] =  ConvertHexToGSMDefault(aOut[Index]);
		}

	}

// -----------------------------------------------------------------------------
// COMASuplInitState::FormUCS2String
// FormUCS2String...
// This function takes two consecuative characters from input string and combine them to form 
// single character of 16-bit.
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void COMASuplInitState::FormUCS2String( const TDesC8& aIn, TDes& aOut )
	{
		TUint16 Res; 
		TUint16 FirstChar;
		TUint16 SecondChar;
		aOut.Zero();
		TInt Len  = aIn.Length() / 2;
		TInt Index = 0;
		TBool IsBigEndian = ETrue;
		TInt Count = 0;
		if(aIn.Length() >= 2  && ((aIn[Index] != 0xFF || aIn[Index + 1] != 0xFE ) && (aIn[Index + 1] != 0xFF || aIn[Index] != 0xFE ))) // Since this is not following algorithm
			{
				aOut.Zero();
				return;
			}
			
		if(aIn.Length() >= 2  && aIn[Index] == 0xFE && aIn[Index + 1] == 0xFF) // Big Endian
			{
				Index = 2;
				Count = 1;
			}
			
		if(aIn.Length() >= 2 && aIn[Index] == 0xFF && aIn[Index + 1] == 0xFE) // Little Endian
			{
				Index = 2;
				IsBigEndian = EFalse;
				Count = 1;
			}
			
		
		for(;Count < Len; Count++)
		{
			if(IsBigEndian)
				{
					FirstChar  = aIn[Index];
					SecondChar = aIn[Index + 1];
				}
			else
				{
					FirstChar  = aIn[Index + 1];
					SecondChar = aIn[Index];
				}	
			FirstChar = FirstChar << 8;	
			Res = FirstChar | SecondChar;
			aOut.Append(Res);
			Index = Index + 2;
		}

	}
	
TOMASuplPosMethod COMASuplInitState::GetPosMethod()
{
	TBuf<128> msg(_L("SuplInitState::GetPosMethod():"));
	msg.AppendNum(iPosMethod);
	iTrace->Trace(msg, KTraceFileName, __LINE__); 
	return iPosMethod;
}

// -----------------------------------------------------------------------------
// COMASuplInitState::GetRequestorID
//   
// 
// -----------------------------------------------------------------------------
void COMASuplInitState::GetRequestorID(TInt& aRequestorId) //APE Centric
    {
        if ( iRequestIDFeatureFlag )
        {
        	aRequestorId = iGSMRequestId;
        }
        else
        {
        	aRequestorId = 0;
        }
    }
// -----------------------------------------------------------------------------
// COMASuplInitState::ReadRequestIDFeatureFlagL
//   
// 
// -----------------------------------------------------------------------------

void COMASuplInitState::ReadRequestIDFeatureFlagL() //APE CENTRIC
{

iRequestIDFeatureFlag = EFalse;
	FeatureManager::InitializeLibL();

 	// Feature flag check provided for Product profile bits specific variation
 	
 	
	iRequestIDFeatureFlag = FeatureManager::FeatureSupported(KSUPLAPECentricArchSupport);
  FeatureManager::UnInitializeLib();
  
  if (iRequestIDFeatureFlag)
        {
						iTrace->Trace(_L("Real: Supl 1.0..APE Flag is ON..."), KTraceFileName, __LINE__); 
        }   
	else
				{
						iTrace->Trace(_L("Real: Supl 1.0..APE Flag is OFF..."), KTraceFileName, __LINE__); 
				}
	
}
//  End of File
