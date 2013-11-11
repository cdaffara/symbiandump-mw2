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
* Description:   Class for handling SUPL_POS message.
*
*/



#include <bautils.h>
#include <flogger.h>

#include <lbs/epos_comasuplpossessionbase.h>
#include <lbs/epos_comasuplpospayload.h>
#include <lbs/epos_comasuplvelocity.h>
#include <lbs/epos_comasuplinforequestlist.h>
#include <lbs/epos_comasuplsetcapabilities.h>
#include <lbs/epos_eomasuplposerrors.h>

#include "epos_comasuplpos.h"
#include "epos_comasuplposrequestor.h"
#include "epos_comasuplposstate.h"
#include "epos_omasuplconstants.h"
#include "epos_comasupltrace.h"
#include "epos_comasuplasnbase.h"

_LIT(KTraceFileName,"SUPL_OMA_SESSION::EPos_COMASuplPosState.cpp");

// ============================ MEMBER FUNCTIONS ===============================

// -----------------------------------------------------------------------------
// COMASuplPosState::COMASuplPosState
// C++ constructor can NOT contain any code, that might leave.
// -----------------------------------------------------------------------------
//
COMASuplPosState::COMASuplPosState(COMASuplPosSessionBase* aPOSSession,TInt aMsgTimeout,COMASuplSETCapabilities* aSETCapabilities,
									COMASuplAsnHandlerBase* aOMASuplAsnHandlerBase):
								  iPOSSession(aPOSSession),
								  iSETCapabilities(aSETCapabilities),
								  iOMASuplAsnHandlerBase(aOMASuplAsnHandlerBase)
    {
			iMessageTimeOut = aMsgTimeout * KSecond; 
			iCurrentState = ESUPL_POS;   
    }

// -----------------------------------------------------------------------------
// COMASuplPosState::~COMASuplPosState
// -----------------------------------------------------------------------------
//
COMASuplPosState::~COMASuplPosState()
    {
		delete iPOSRequestor;
		delete iRemainingPospayload;
		
		if(iTrace)
			iTrace->Trace(_L("COMASuplPosState::~COMASuplPosState..."), KTraceFileName, __LINE__); 							
    }

// -----------------------------------------------------------------------------
// COMASuplPosState::ConstructL
// Symbian 2nd phase constructor can leave.
// -----------------------------------------------------------------------------
//
void COMASuplPosState::ConstructL()
    {
    	COMASuplState::BaseConstructL();
    	iTrace->Trace(_L("COMASuplPosState::ConstructL..."), KTraceFileName, __LINE__); 
    								
		if(iPOSSession)
		{
			iPOSRequestor = COMASuplPOSRequestor::NewL(this,iPOSSession);
			iOMASuplPosPayLoad = COMASuplPosPayload::NewL();
			iOMASuplVelocity = COMASuplVelocity::NewL();
			
			
			iPOSRequestor->CreateListL();
			iPOSRequestor->AppendInfoRequest(iOMASuplPosPayLoad);//ownership is towords iPOSRequestor
			iPOSRequestor->AppendInfoRequest(iOMASuplVelocity);
		}
			iIsLastPosPayloadSegmented = EFalse;
			iIsAnyPosPayloadFromPosInit = EFalse;
    }

// -----------------------------------------------------------------------------
// COMASuplPosState::NewL
// Two-phased constructor.
// -----------------------------------------------------------------------------
//
COMASuplPosState* COMASuplPosState::NewL(COMASuplPosSessionBase* aPOSSession,TInt aMsgTimeout,COMASuplSETCapabilities* aSETCapabilities,
																				 COMASuplAsnHandlerBase* aOMASuplAsnHandlerBase)
    {
	    COMASuplPosState* self =  new ( ELeave ) COMASuplPosState(aPOSSession,aMsgTimeout,aSETCapabilities,aOMASuplAsnHandlerBase);
	    CleanupStack::PushL( self );
	    self->ConstructL();
	    CleanupStack::Pop(self);
	    return self;
    }

// -----------------------------------------------------------------------------
// COMASuplPosState::GenerateMessage
// Generates SUPL_POS message
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
TInt COMASuplPosState::GenerateMessageL() 
	{
		iTrace->Trace(_L("Start of COMASuplPosState::GenerateMessage"), KTraceFileName, __LINE__); 
		TInt err = KErrNone;
		iGenerationStatus = ETrue;
		if(iPOSSession)
		{
			if(iIsLastPosPayloadSegmented)
				{
					OperationCompleteL(KErrNone);
				}
			else
				{
					iOMASuplPosPayLoad->SetStatus(KErrOMASuplParamNotSet);//Set this explicitly...so that one can set it again
					err = iPOSRequestor->GetSuplInfoL();	
				}	
			
			if(err != KErrNone)	
				{
					iGenerationStatus = EFalse;
				}
		}
		
		return err;
	}
	
// -----------------------------------------------------------------------------
// COMASuplPosState::EncodeMessageL
// Encodes message ...returns encoded data
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
HBufC8* COMASuplPosState::EncodeMessageL(TOMASuplVersion& aSuplVersion,COMASuplSessionID* aSessionId,TInt& aError)
{ 
    iTrace->Trace(_L("Starting Encoding..."), KTraceFileName, __LINE__); 							
    COMASuplState::EncodeMessageL(aSuplVersion,aSessionId,aError);
	HBufC8 *encodedBuffer = NULL;
	TInt error;
	aError = KErrNone;
	iRequestError = KErrNone;
	

	COMASuplAsnMessageBase* OMASuplMessageBase = iOMASuplAsnHandlerBase->CreateOMASuplMessageL(COMASuplAsnMessageBase::ESUPL_POS);
	CleanupStack::PushL(OMASuplMessageBase);
	COMASuplPos* OMASuplPos  =  static_cast<COMASuplPos*>(OMASuplMessageBase);
	OMASuplPos->SetMessageBase(aSuplVersion,aSessionId); 
	IterateSuplInfoListL(OMASuplPos,aError);

	TRAPD(err,encodedBuffer = iOMASuplAsnHandlerBase->EncodeL(OMASuplPos,error));
	CleanupStack::PopAndDestroy(OMASuplMessageBase);
	
	LogPosPayLoadData();
	LogVelocity();
	
	if(aError == KErrNone || aError == KErrOMASuplLastPosMsg )				
		{
			if ( error == KErrNone && err == KErrNone )
			{
				iTrace->Trace(_L("Encoding Done..."), KTraceFileName, __LINE__); 							
				aError = KErrNone;
				return encodedBuffer;
			}
			else
			{
				iTrace->Trace(_L("Encoding Failed."), KTraceFileName, __LINE__); 
				aError = error;	
				delete encodedBuffer;
			}
			return NULL;
		}
	else
		{
			aError = KErrOMAManParamMissing;
			iTrace->Trace(_L("POS failed to give manadatory ..."), KTraceFileName, __LINE__); 							
			delete encodedBuffer; 
			return NULL;
		}		
}

// -----------------------------------------------------------------------------
// COMASuplPosState::ProcessMessage
// process message...
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
TInt COMASuplPosState::ProcessMessageL(COMASuplAsnMessageBase*  aDecodedMessage )
{
		iTrace->Trace(_L("COMASuplPosState::ProcessMessageL"), KTraceFileName, __LINE__); 
				
		COMASuplPos* OMASuplPos = static_cast <COMASuplPos*>(aDecodedMessage);
		
			//Retrieve the Pos Payload from Supl Pos Message
			COMASuplPosPayload* OMASuplPosPayLoad = NULL;
			OMASuplPosPayLoad  =  OMASuplPos->SuplPos(); 
			
			if(!CheckPOSPayloadType(OMASuplPosPayLoad)) // Check is POS method mismatch with SETCaps
				{
					iTrace->Trace(_L("COMASuplPosState::ProcessMessageL POS method Mismatched. "), KTraceFileName, __LINE__); 
					return KErrSuplposMethodMismatch; 	
				}
			if(iPOSSession)
				{
					TRAP_IGNORE(iPOSSession->HandleSuplPosMessageL(OMASuplPosPayLoad));		
					iTrace->Trace(_L("COMASuplPosState::ProcessMessageL...PosPayLoad sent to POS Msg Plugin..."), KTraceFileName, __LINE__); 
				}
			return KErrNone;
}

// -----------------------------------------------------------------------------
// COMASuplPosState::CancelOperation
// Cancels Operation...which will be depend on derived class implementation
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void COMASuplPosState::CancelOperation() 
	{ 
		 COMASuplState::CancelOperation();
		 if(iPOSRequestor)
		 	{
			 	iPOSRequestor->Cancel();
		 	}
		 iTrace->Trace(_L("COMASuplPosState::CancelOperation..."), KTraceFileName, __LINE__); 							
	}	
// -----------------------------------------------------------------------------
// COMASuplPosState::StartTimer
// StartTimer...starts the timer ... 
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void COMASuplPosState::StartTimer()
{
	iTrace->Trace(_L("COMASuplPosState::StartTimer..."), KTraceFileName, __LINE__); 							
	if(IsLastMessage() && !iIsLastPosPayloadSegmented)
		{
			iTrace->Trace(_L("Timer Started for POS"), KTraceFileName, __LINE__); 
			COMASuplState::StartTimer();
		}
}

// -----------------------------------------------------------------------------
// COMASuplPosState::IsLastMessage
// Is last message
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
TBool COMASuplPosState::IsLastMessage()
{
	iTrace->Trace(_L("COMASuplPosState::IsLastMessage"), KTraceFileName, __LINE__); 
	if(iRequestError == KErrOMASuplLastPosMsg)
		{
			iTrace->Trace(_L("COMASuplPosState::IsLastMessage... Last Message from POS"), KTraceFileName, __LINE__); 
			return ETrue;
		}
	else
		{
			iTrace->Trace(_L("COMASuplPosState::IsLastMessage...More data from POS Payload..."), KTraceFileName, __LINE__); 
			return EFalse;
		}
}

// -----------------------------------------------------------------------------
// COMASuplPosState::OperationCompleteL
// Gets control when any of the asynchronous requests are completed
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void COMASuplPosState::OperationCompleteL(TInt aErrorCode)
{
	iGenerationStatus = EFalse;
	PseudoSegmentationL();
	if(iMsgStateObserver)
		iMsgStateObserver->OperationCompleteL(aErrorCode);
}

// -----------------------------------------------------------------------------
// COMASuplPosState::GetPositionComplete
// No use
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void COMASuplPosState::GetPositionComplete(TInt /*aError*/)
{
}

// -----------------------------------------------------------------------------
// COMASuplPosState::IterateSuplInfoListL
// Interates thro' request list 
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void COMASuplPosState::IterateSuplInfoListL(COMASuplPos* aOMASuplPos,TInt &aError)
{ 
	iTrace->Trace(_L("COMASuplPosState::IterateSuplInfoList..."), KTraceFileName, __LINE__); 
	COMASuplInfoRequestList* OMASuplInfoRequestList =  iPOSRequestor->GetOMASuplInfoRequestList();
	
	if(!OMASuplInfoRequestList) 
		{
			iTrace->Trace(_L("Failed to retrived list(RequestList).COMASuplPosState::FillSuplInfoList"), KTraceFileName, __LINE__); 		
			aError = KErrCompletion;
			return;	
		}
	
	TInt NoOfElements = OMASuplInfoRequestList->GetCount();
	
	aError = KErrNone;
	
	for(TInt cnt = 0; cnt < NoOfElements; ++cnt)
		{
			COMASuplInfoRequest* info = OMASuplInfoRequestList->GetElement(cnt);
			if(!info) 
				{
					iTrace->Trace(_L("Failed to retrive list(info).COMASuplPosState::IterateSuplInfoList"), KTraceFileName, __LINE__); 		
					continue;
				}
				
			TInt posError = info->Status();					
			switch(info->Type())
				{
					case COMASuplInfoRequest::EOMASuplVelocity: 
				 		{
							if(posError == KErrNone)
								{
									if(iOMASuplVelocity)//Use velocity
										{
											COMASuplVelocity* OMASuplVelocity = static_cast <COMASuplVelocity *>(iOMASuplVelocity->CloneL());//Ownership transfer to callee
											aOMASuplPos->SetVelocity(OMASuplVelocity);
										}
								}
							break;	
						}

					case COMASuplInfoRequest::EOMASuplPosPayload: 
						{
							if( (posError == KErrNone || posError == KErrOMASuplLastPosMsg) && iOMASuplPosPayLoad)//Use PosPayload
								{
									COMASuplPosPayload* OMASuplPosPayload =static_cast <COMASuplPosPayload *>(iOMASuplPosPayLoad->CloneL());//Ownership transfer to callee
									aOMASuplPos->SetSuplPos(OMASuplPosPayload);
									if(posError == KErrOMASuplLastPosMsg)
										{
											iRequestError = KErrOMASuplLastPosMsg;
										}
								}
							else
								{
									aError = posError;
								}	
							break;	
						}
				}
		}
		
	iTrace->Trace(_L("Done...COMASuplPosState::IterateSuplInfoList"), KTraceFileName, __LINE__); 		
}

// -----------------------------------------------------------------------------
// COMASuplPosState::IsGenerationInProgress()
// returns status of generation of Message
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
TBool COMASuplPosState::IsGenerationInProgress()
	{
		return iGenerationStatus;
	}
	
// -----------------------------------------------------------------------------
// COMASuplPosState::CheckPOSPayloadType()
// returns EFalse if there is mismatch in POS Payload type
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
TBool COMASuplPosState::CheckPOSPayloadType(const COMASuplPosPayload *aPosPayload)
	{
		if(!iSETCapabilities)	
			{
				return EFalse;		
			}
			
			HBufC8* Payload = NULL;
			COMASuplPosPayload::TOMASuplPosPayloadType ReceivedPayloadType;
			aPosPayload->GetPosPayload(Payload,ReceivedPayloadType);
			
			TOMASuplPosTechnology posTech;
			COMASuplSETCapabilities::TOMASuplPrefMethod prefMethod;
			TOMASuplPosProtocol posProtocol;
			iSETCapabilities->GetSETCapabilities(posTech,prefMethod, posProtocol);
			
			TBool SETtia801,SETRRLP,SETRRC;
			posProtocol.GetPosProtocol(SETtia801,SETRRLP,SETRRC);
			
			if(ReceivedPayloadType == COMASuplPosPayload::ETIA801)
				{
					iTrace->Trace(_L("Received POS protocol method is TIA801 "), KTraceFileName, __LINE__); 		
					return SETtia801;
				}
				
			if(ReceivedPayloadType == COMASuplPosPayload::ERRLP)
				{
					iTrace->Trace(_L("Received POS protocol method is RRLP "), KTraceFileName, __LINE__); 		
					return SETRRLP;
				}
				
			if(ReceivedPayloadType == COMASuplPosPayload::ERRC)
				{
					iTrace->Trace(_L("Received POS protocol method is RRC "), KTraceFileName, __LINE__); 		
					return SETRRC;
				}
				
		return EFalse;
	}
	

// -----------------------------------------------------------------------------
// COMASuplPosState::IsSegmentationDone
// Does the pseudo segmentation done...
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
TBool COMASuplPosState::IsSegmentationDone() const
	{
		return iIsLastPosPayloadSegmented;
	}
	
// -----------------------------------------------------------------------------
// COMASuplPosState::PseudoSegmentation
// Does the pseudo segmentation.
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void COMASuplPosState::PseudoSegmentationL()
	{
		TInt posPosPayLoadError = iOMASuplPosPayLoad->Status();					
		if(posPosPayLoadError == KErrNone || posPosPayLoadError == KErrOMASuplLastPosMsg || !iIsAnyPosPayloadFromPosInit)
			{
				HBufC8* payLoad = NULL;
				iOMASuplPosPayLoad->GetPosPayload(payLoad,iPosPayLoadType);
			if(!iIsLastPosPayloadSegmented) // First time ..will hold data
				{
					if(payLoad)
					{
						TInt posPayloadLength = payLoad->Length();
						TBuf<64> msg;
						msg.Copy(_L("Received Pospayload having length : "));
						msg.AppendNum(posPayloadLength);
						iTrace->Trace(msg, KTraceFileName, __LINE__); 		
						msg.Copy(_L("Logging Received POS Payload from POS Msg Plugin: "));
						iTrace->Trace(msg, KTraceFileName, __LINE__); 		

				    	LogPacket(payLoad->Des());
						
						if( posPayloadLength > KMaxPosPayloadLength )
						{
							delete iRemainingPospayload; 
							posPayloadLength  = posPayloadLength - KMaxPosPayloadLength; // Length of remaining data
							msg.Copy(_L("Remaining POS Payload after segmentation: "));
							msg.AppendNumUC(posPayloadLength);
							iTrace->Trace(msg, KTraceFileName, __LINE__); 		
							iRemainingPospayload = HBufC8::NewL(posPayloadLength);  
							iRemainingPospayload->Des().Copy(payLoad->Des().Mid(KMaxPosPayloadLength ));// Copy rest data
							payLoad->Des().Copy(payLoad->Des().Left(KMaxPosPayloadLength));
							iIsLastPosPayloadSegmented = ETrue;
							
								TInt len = payLoad->Length();
								msg.Copy(_L("Segmentationed POS Payload data...: "));
								msg.AppendNum(len);
								iTrace->Trace(msg, KTraceFileName, __LINE__); 		
								LogPacket(payLoad->Des());
						}
					}
				}
			else
				{
					iOMASuplVelocity->SetStatus(KErrOMASuplParamNotSet);//Set this explicitly...so that velocity will not be reconsider
					TInt remainingPosPayloadLength = iRemainingPospayload->Length();
					if( remainingPosPayloadLength > KMaxPosPayloadLength )
						{
							payLoad->Des().Copy(iRemainingPospayload->Des().Left(KMaxPosPayloadLength));
							iRemainingPospayload->Des().Copy(iRemainingPospayload->Des().Mid(KMaxPosPayloadLength));// Copy rest data
						}
					else
						{
							payLoad->Des().Copy(iRemainingPospayload->Des());
							iIsLastPosPayloadSegmented = EFalse;
						}
						
						TInt len = payLoad->Length();
						TBuf<64> msg;
						msg.Copy(_L("Segmentationed POS Payload data...: "));
						msg.AppendNum(len);
						iTrace->Trace(msg, KTraceFileName, __LINE__); 		

				}
			}
	}
	
// -----------------------------------------------------------------------------
// COMASuplPosState::SetRemainingPosPayloadFromSuplPosInit
// Sets remaining pos payload from SUPL_POS_INIT
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void COMASuplPosState::SetRemainingPosPayloadFromSuplPosInitL(COMASuplPosPayload* aRemainingPospayload)	
	{
		if(aRemainingPospayload)
			{
				iIsAnyPosPayloadFromPosInit = ETrue;
				iIsLastPosPayloadSegmented = ETrue;
				
				HBufC8* payLoad = NULL;
				COMASuplPosPayload::TOMASuplPosPayloadType payLoadType;
				aRemainingPospayload->GetPosPayload(payLoad,payLoadType);
				
				HBufC8* payLoadBuffer = HBufC8::NewL(payLoad->Length());
				payLoadBuffer->Des().Copy(payLoad->Des());
				iOMASuplPosPayLoad->SetPosPayload(payLoadBuffer,payLoadType); //Setting POS payload buffer
				iOMASuplPosPayLoad->SetStatus(KErrNone);
				
				iRemainingPospayload = HBufC8::NewL(payLoad->Length());
				iRemainingPospayload->Des().Copy(payLoad->Des());
				delete aRemainingPospayload;
			}
	}
	
// -----------------------------------------------------------------------------
// COMASuplPosState::LogPosPayLoadData
// // Logs PosPayLoadData in epostrace file
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void COMASuplPosState::LogPosPayLoadData()
	{
	
#ifdef PRINT_MESSAGE 

	if(iOMASuplPosPayLoad)
		{
		TInt posPosPayLoadError = iOMASuplPosPayLoad->Status();					
		if(!(posPosPayLoadError == KErrNone || posPosPayLoadError == KErrOMASuplLastPosMsg  ))
			{
				iTrace->Trace(_L("No POS Payload data provided for SUPL_POS from PosMsgPlugin"), KTraceFileName, __LINE__);	
				return;
			}
			
		iTrace->Trace(_L("**************** Start of SUPL POS  PosPayLoad Data from PosMsgPlugin**********************"), KTraceFileName, __LINE__);
		HBufC8* payLoad = NULL;
		COMASuplPosPayload::TOMASuplPosPayloadType payLoadType;
		
		iOMASuplPosPayLoad->GetPosPayload(payLoad,payLoadType);
		HBufC* payLoadBuffer = NULL; 
		if(payLoad)
			{
			TInt err;	
			TRAP(err,payLoadBuffer = HBufC::NewL(payLoad->Length()))	;
			err = 0;
			payLoadBuffer->Des().Copy(payLoad->Des());
			}
			
		if(payLoadType == COMASuplPosPayload::ETIA801)
			{
			iTrace->Trace(_L("POS DATA Type - ETIA801 "), KTraceFileName, __LINE__);
			if(payLoadBuffer && payLoad)
			   iTrace->Trace(payLoadBuffer->Des(),KTraceFileName, __LINE__);
			}
		else if(payLoadType == COMASuplPosPayload::ERRLP)
			{
			iTrace->Trace(_L("POS DATA Type - ERRLP "), KTraceFileName, __LINE__);
			}
		else if(payLoadType == COMASuplPosPayload::ERRC)
			{
			iTrace->Trace(_L("POS DATA Type - ERRC "), KTraceFileName, __LINE__);
			if(payLoadBuffer && payLoad )
				iTrace->Trace(payLoadBuffer->Des(),KTraceFileName, __LINE__);		
			}
			
		delete payLoadBuffer;
		payLoadBuffer = NULL;
		
		iTrace->Trace(_L("**************** End of SUPL POS  PosPayLoad Data **********************"), KTraceFileName, __LINE__);
		}
#endif

	}
	
// -----------------------------------------------------------------------------
// COMASuplPosState::LogVelocity
// Logs Velocity in epostrace file
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void COMASuplPosState::LogVelocity()
	{
	
#ifdef PRINT_MESSAGE 		
	if(iOMASuplVelocity)
		{
		TInt posVelocityError = iOMASuplVelocity->Status();					
		if(posVelocityError != KErrNone )
			{
				iTrace->Trace(_L("No Velocity Information provided for SUPL_POS "), KTraceFileName, __LINE__);	
				return;
			}
			
		iTrace->Trace(_L("Velocity Information for SUPL_POS "), KTraceFileName, __LINE__);		
		TBuf<256> msg;
		TOMASuplVelocityType velocityType = iOMASuplVelocity->VelType();
		COMASuplHorizVelocity* horizVelocity = iOMASuplVelocity->Velocity();
		TUint16 bearing;
		TUint16 horSpeed;
		switch(velocityType)	
			{
				case  EHorizVelocity:
					{
					iTrace->Trace(_L("Velocity Type : EHorizVelocity - Values "), KTraceFileName, __LINE__);
					horizVelocity->GetHorizVel(bearing,horSpeed);
					
					msg.Append(_L("Bearing "));
					msg.AppendNum(bearing);
					msg.Append(_L("Horizontal Speed "));
					msg.AppendNum(horSpeed);
					iTrace->Trace(msg, KTraceFileName, __LINE__);
					break;
					}
				case EHorizAndVertVelocity:
					{
					TUint8 verDirect;
					TUint8 verSpeed;
					COMASuplHorizAndVertVelocity* horizVertVel = (COMASuplHorizAndVertVelocity*)horizVelocity;
					horizVertVel->GetHorizAndVertVel(bearing,horSpeed,verDirect,verSpeed);
					
					iTrace->Trace(_L("Velocity Type : EHorizVelocity - Values "), KTraceFileName, __LINE__);
										
					msg.Append(_L("Bearing "));
					msg.AppendNum(bearing);
					msg.Append(_L("Horizontal Speed "));
					msg.AppendNum(horSpeed);
					msg.Append(_L("Vertical Direction "));
					msg.AppendNum(verDirect);
					msg.Append(_L("Vertical Speed "));
					msg.AppendNum(verSpeed);					
					iTrace->Trace(msg, KTraceFileName, __LINE__);
					
					break;
					}
				case EHorizUncertVelocity: 
					{
					TUint8 uncertSpeed;
					COMASuplHorizUncertVelocity* horizUncertVel = (COMASuplHorizUncertVelocity*)horizVelocity;
	
					horizUncertVel->GetHorizUncertVel(bearing,horSpeed,uncertSpeed);	
					
					iTrace->Trace(_L("Velocity Type : EHorizUncertVelocity - Values "), KTraceFileName, __LINE__);
										
					msg.Append(_L("Bearing "));
					msg.AppendNum(bearing);
					msg.Append(_L("Horizontal Speed "));
					msg.AppendNum(horSpeed);
					msg.Append(_L("Uncertainity Speed "));
					msg.AppendNum(uncertSpeed);
							
					iTrace->Trace(msg, KTraceFileName, __LINE__);
					
					break;
					}
				case EHorizAndVertUncertVelocity:
					{
					TUint8  verDirect;
					TUint8  verSpeed;
					TUint8  horizUncertSpeed;
					TUint8  vertUncertSpeed;
										
					COMASuplHorizAndVertUncertVelocity* horizVertUncertVel = (COMASuplHorizAndVertUncertVelocity*)horizVelocity;

					horizVertUncertVel->GetHorizVertUncertVel(bearing,horSpeed,verDirect,verSpeed,
																horizUncertSpeed,vertUncertSpeed);
					
					iTrace->Trace(_L("Velocity Type : EHorizAndVertUncertVelocity - Values "), KTraceFileName, __LINE__);
										
					msg.Append(_L("Bearing "));
					msg.AppendNum(bearing);
					msg.Append(_L("Horizontal Speed "));
					msg.AppendNum(horSpeed);
					msg.Append(_L("Vertical Direction "));
					msg.AppendNum(verDirect);
					msg.Append(_L("Vertical Speed "));
					msg.AppendNum(verSpeed);					
					msg.Append(_L("Horiz Uncertain Speed "));
					msg.AppendNum(horizUncertSpeed);
					msg.Append(_L("Vertical Uncertain Speed "));
					msg.AppendNum(vertUncertSpeed);					
					iTrace->Trace(msg, KTraceFileName, __LINE__);																					
																
				    break;																
					}
			}
		}
#endif
		
	}
	
void COMASuplPosState::LogPacket(const TDesC8& 
#ifdef PRINT_MESSAGE
aPacket
#endif
)
{
#ifdef PRINT_MESSAGE

			RFileLogger  logger;
			TInt err = logger.Connect();
			if(err == KErrNone)
				{
					const TText* hdr1 = _S("Header");
					const TText* mrg1 = _S("End");
					logger.CreateLog(_L("epos"),_L("PayloadFromPOSPlugin.txt"),EFileLoggingModeAppend);
					logger.HexDump(hdr1, mrg1, aPacket.Ptr(), aPacket.Length() );
				}
			logger.Close();
#endif
}

	
//  End of File
