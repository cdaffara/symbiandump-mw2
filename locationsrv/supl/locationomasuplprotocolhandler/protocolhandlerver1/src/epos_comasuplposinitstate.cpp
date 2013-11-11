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
* Description:    class for handling SUPL_POS_INIT message...
*
*/



#include <etelmm.h> 
#include <hash.h>
#include <f32file.h>
#include <flogger.h>
#include <lbs/epos_comasuplpossessionbase.h>
#include "utf.h"
#include "epos_comasupllocationid.h"
#include "lbs/epos_comasuplreqasstdata.h"
#include "lbs/epos_comasuplinforequestlist.h"
#include "lbs/epos_comasuplsetcapabilities.h"
#include "lbs/epos_comasuplpospayload.h"
#include "lbs/epos_comasuplposition.h"
#include "lbs/epos_comasuplvelocity.h"

#include "epos_comasuplsession.h"
#include "epos_comasuplposinit.h"
#include "epos_comasuplposinitstate.h"
#include "epos_comasuplposrequestor.h"
#include "epos_omasuplconstants.h"
#include "epos_comasupltrace.h"


_LIT(KTraceFileName,"SUPL_OMA_SESSION::EPos_COMASuplPosInitState.cpp");



// ============================ MEMBER FUNCTIONS ===============================

// -----------------------------------------------------------------------------
// COMASuplPosInitState::COMASuplPosInitState
// C++ constructor can NOT contain any code, that might leave.
// -----------------------------------------------------------------------------
//
COMASuplPosInitState::COMASuplPosInitState(COMASuplSETCapabilities* aOldCapabilities,
										   RMobilePhone& aMobilePhone,TInt aMsgTimeout,
										   COMASuplPOSRequestor* aPosRequestor,
										   COMASuplSession::TOMASuplReqType aRequestType,
										   COMASuplAsnHandlerBase* aOMASuplAsnHandlerBase):
										   COMASuplStartState(aMobilePhone,
										   aMsgTimeout,aPosRequestor,aOMASuplAsnHandlerBase),
										   iRequestType(aRequestType),
                       iPosPayloadPresent(EFalse),iIgnorePosData(EFalse),iRepeatedPosDataFetch(EFalse)
    {
		iCurrentState =  ESUPL_POS_INIT;
		iOldCapabilities = aOldCapabilities;   // Caps recevied during SUPL_START from POS
		iRes.FillZ();
		iGenerationStatus = EFalse;
    }

// -----------------------------------------------------------------------------
// COMASuplPosInitState::~COMASuplPosInitState
// -----------------------------------------------------------------------------
//
COMASuplPosInitState::~COMASuplPosInitState()
    {
	if(iHSLPAddress)
		{
		delete iHSLPAddress;
		}
		
	if(iEncodedSuplInit)
		{
		delete iEncodedSuplInit;
		}

	if(iLocationId)
		{
		delete iLocationId;	
		iLocationId=NULL;
		}    	
		
	if(iTrace)
		{
		iTrace->Trace(_L("COMASuplPosInitState::~COMASuplPosInitState..."), KTraceFileName, __LINE__); 							
		}
    }

// -----------------------------------------------------------------------------
// COMASuplPosInitState::ConstructL
// Symbian 2nd phase constructor can leave.
// -----------------------------------------------------------------------------
//
void COMASuplPosInitState::ConstructL(TInt aAllowedCapabilities, TOMASuplPosMethod aPosMethod, HBufC8* aEncodedSuplInit, HBufC8* aHSLPAddress)
    {
    	COMASuplStartState::ConstructL(aAllowedCapabilities);
		iTrace->Trace(_L(""), KTraceFileName, __LINE__); 							    	
		
	
		switch(aPosMethod)
			{
			case EOMAAgpsSETAssisted:
				{
				iTrace->Trace(_L("EGpsSETAssisted"), KTraceFileName, __LINE__); 
				iPosMethod = COMASuplPosSessionBase::EGpsSETAssisted;
				}
				break;
	
			case EOMAAgpsSETAssistedPref:
				{
				iTrace->Trace(_L("EOMAAgpsSETAssistedPref"), KTraceFileName, __LINE__); 
				iPosMethod = COMASuplPosSessionBase::EGpsSETAssistedPreferred;
				}
				break;
	
			case EOMAAgpsSETBased:
				{
				iTrace->Trace(_L("EOMAAgpsSETBased"), KTraceFileName, __LINE__); 
				iPosMethod = COMASuplPosSessionBase::EGpsSETBased;
				}
				break;
	
			case EOMAAgpsSETBasedPref:
				{
				iTrace->Trace(_L("EOMAAgpsSETBasedPref"), KTraceFileName, __LINE__); 
				iPosMethod = COMASuplPosSessionBase::EGpsSETBasedPreferred;
				}
				break;
	
			case EOMAAutonomousGps:
				{
				iTrace->Trace(_L("EOMAAutonomousGps"), KTraceFileName, __LINE__); 
				iPosMethod = COMASuplPosSessionBase::EAutonomousGps;
				}
				break;
	
			case EOMAFLT:
				{
				iTrace->Trace(_L("EOMAFLT"), KTraceFileName, __LINE__); 
				iPosMethod = COMASuplPosSessionBase::EAFLT;
				}
				break;
	
			case EOMAeOTD:
				{
				iTrace->Trace(_L("EOMAeOTD"), KTraceFileName, __LINE__); 
				iPosMethod = COMASuplPosSessionBase::EeOTD;
				}
				break;
	
			case EOMAoTDOA:
				{
				iTrace->Trace(_L("EOMAoTDOA"), KTraceFileName, __LINE__); 
				iPosMethod = COMASuplPosSessionBase::EoTDOA;
				}
				break;
	
			case EOMANoPosition:
				{
				iTrace->Trace(_L("EOMANoPosition"), KTraceFileName, __LINE__); 
				iPosMethod = COMASuplPosSessionBase::ENoPosition;
				}
				break;
	
			default:
	
				{
				iTrace->Trace(_L("default"), KTraceFileName, __LINE__); 
				iPosMethod = COMASuplPosSessionBase::EPositionUndefined;			
				}
				break;
			}
		
		TBuf<128> msg(_L("COMASuplPosInitState::ConstructL... iPosMethod: "));
		msg.AppendNum(iPosMethod);
		iTrace->Trace(msg, KTraceFileName, __LINE__); 
		
		if(aEncodedSuplInit)
			{
				TInt len = aEncodedSuplInit->Length();
		    	if(len > 0)
		    	{
		    		iEncodedSuplInit = HBufC8::NewL(len);
		    		*iEncodedSuplInit = *aEncodedSuplInit; 
		       	}
		    			
			}
    	
    	if(aHSLPAddress)
	   		{
		    	TInt len = aHSLPAddress->Length();
 
		    	if(len > 0)
		    	{
				   	iHSLPAddress = HBufC8::NewL(len);
				  	*iHSLPAddress = *aHSLPAddress;
		    	}
			
    		}
    }

// -----------------------------------------------------------------------------
// COMASuplPosInitState::NewL
// Two-phased constructor.
// -----------------------------------------------------------------------------
//
COMASuplPosInitState* COMASuplPosInitState::NewL(COMASuplSETCapabilities* aOldCapabilities,
											RMobilePhone& aMobilePhone,TInt aMsgTimeout,
											COMASuplPOSRequestor* aPosRequestor,TInt aAllowedCapabilities,
											COMASuplSession::TOMASuplReqType aRequestType, TOMASuplPosMethod aPosMethod,
											COMASuplAsnHandlerBase* aOMASuplAsnHandlerBase,
											HBufC8* aEncodedSuplInit,
											HBufC8* aHSLPAddress
											)
    {
	    COMASuplPosInitState* self =  new ( ELeave ) COMASuplPosInitState(aOldCapabilities,
	    											  aMobilePhone,aMsgTimeout,aPosRequestor,aRequestType,aOMASuplAsnHandlerBase);
	    CleanupStack::PushL( self );
	    self->ConstructL(aAllowedCapabilities, aPosMethod, aEncodedSuplInit, aHSLPAddress);
	    CleanupStack::Pop(self);
	    return self;
    }

// -----------------------------------------------------------------------------
// COMASuplPosInitState::GenerateMessage
// Generates SUPL_POS_INIT message
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
TInt COMASuplPosInitState::GenerateMessageL() 
	{
		iGenerationStatus = ETrue;
		iTrace->Trace(_L("Start of COMASuplPosInitState::GenerateMessage"), KTraceFileName, __LINE__); 					
		iVerPresent = ComputeSuplInitVerL();
		if(iVerPresent)
			{
				iLocationIDRequestor->GetLocationID();		
			}
		else
			{
				iMsgStateObserver->OperationCompleteL(KErrGeneral);
			}
		return KErrNone;				
	}

// -----------------------------------------------------------------------------
// COMASuplPosInitState::ComputeSuplInitVer
// Generates SUPL_POS_INIT message
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
TBool COMASuplPosInitState::ComputeSuplInitVerL() 
	{
	if(iRequestType == COMASuplSession::ESUPL_TERMINAL) 
		{
			return ETrue;
		}
	
	if(iEncodedSuplInit && (iEncodedSuplInit->Length()) > 0 &&
	iHSLPAddress && iHSLPAddress->Length() > 0)
	{
		TInt blockSize = 64;
	    TChar ipad = 0x36;
	    TChar opad = 0x5c;
	    TBuf8<64> key0;
	    TInt i;

	    CMessageDigest* sha1 = CMessageDigestFactory::NewDigestLC(
	                                CMessageDigest::ESHA1 );

	    // Step 1: Append 0x00 to the key to create a string of size = blocksize
	    key0.SetLength( blockSize );
	    key0.FillZ();
	    
	    if ( iHSLPAddress->Length() != blockSize )
	        {
	        // If key greater than the block size of digest which is 64
	        // then hash the key to get a 20 byte code
	        if ( iHSLPAddress->Length() > blockSize )
	            {
	            TPtrC8 hashedKey = sha1->Hash(*iHSLPAddress );
	            for ( i = 0; i < hashedKey.Length(); i++ )
	                {
	                key0[i] = hashedKey[i];
	                }
	            sha1->Reset();
	            }
	        else if ( iHSLPAddress->Length() < blockSize )
	            {
	            for ( i = 0; i < iHSLPAddress->Length(); i++ )
	                {
	                key0[i] = (*iHSLPAddress)[i];
	                }
	            }
	        }
	    else
	        {
	        for ( i = 0; i < iHSLPAddress->Length(); i++ )
	            {
	            key0[i] = (*iHSLPAddress)[i];
	            }
	        } 
	    
	    
	     // Step 2: XOR the string obtained in Step 1 with ipad
	    for ( TInt i = 0; i < blockSize; i++ )
	        {
	        iRes.Append( key0[i] ^ ipad );
	        }
	     
	     
#ifdef PRINT_MESSAGE
	    iTrace->Trace(_L("ipad of server address  "), KTraceFileName, __LINE__); 					
	    PrintHex(iRes,__LINE__); 
#endif
	    
	    // Step 3: Append the message to the string resulting from Step 2
	    HBufC8* messageKeyString = 
	            HBufC8::NewLC( iEncodedSuplInit->Size() + iRes.Size() );
	    messageKeyString->Des().Append( iRes );
	    messageKeyString->Des().Append( *iEncodedSuplInit );

	    
	    // Step 4: Generate hash code of the string resulting in Step 3
	    TPtrC8 innerHashedMessage = sha1->Hash( messageKeyString->Des() );
	    HBufC8* step4Msg = HBufC8::NewLC(innerHashedMessage.Size());
	    step4Msg->Des().Append(innerHashedMessage);
	    sha1->Reset();
	    
	    
	    // Step 5: XOR the string obtained in Step 1 with opad
	    HBufC8* hmacString = HBufC8::NewLC( step4Msg->Size() + blockSize );
	    for ( TInt i = 0; i < blockSize; i++ )
	        {
	        hmacString->Des().Append( key0[i] ^ opad );
	        }
	        
	       
#ifdef PRINT_MESSAGE
	    iTrace->Trace(_L("xor ofserver address with opad"), KTraceFileName, __LINE__); 					
	    PrintHex(hmacString->Des(),__LINE__); 
#endif
	    
	    // Step 6: Append the string obtained in step 4 to the string 
	    // obtained in Step 5
	    hmacString->Des().Append( step4Msg->Des() );
	  
	    
	    // Step 7: Generate hash code of the string resulting from Step 6
	    iRes.FillZ();
	    iRes.Zero();
	    TPtrC8 hmacSha1Code = sha1->Hash( hmacString->Des() );

	    // Truncate hash output to 64 bits
	    iRes.Copy( hmacSha1Code.Ptr(), 8 );

	    CleanupStack::PopAndDestroy( hmacString );
	    CleanupStack::PopAndDestroy( step4Msg );
	    CleanupStack::PopAndDestroy( messageKeyString );
	    CleanupStack::PopAndDestroy( sha1 );
	    
#ifdef PRINT_MESSAGE
		iTrace->Trace(_L("Final ver..."), KTraceFileName, __LINE__); 							
		PrintHex(iRes,__LINE__);
#endif
		return ETrue;	
	}
	
	return EFalse;
	
	}	
// -----------------------------------------------------------------------------
// COMASuplPosInitState::GetPosParamsL
// Collects the optional parameters for SUPL_POS_INIT 
// message from POS Message plugin
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void COMASuplPosInitState::GetPosParamsL()
    {

    iGenerationStatus = ETrue;

    // Re-initialize the POS Requestor
    if (iPosRequestor)
        {
        iPosRequestor->DestroyList();
        iPosRequestor->CreateListL();
        }

    // Create SET capabilities object
    iCurSetCapabilities = COMASuplSETCapabilities::NewL();

    // Create the Request Assistance data object
    iReqAsstData = COMASuplReqAsstData::NewL();

    // Create Position
    iPosition = COMASuplPosition::NewL();

    // Create POS Payload
    iPosPayload = COMASuplPosPayload::NewL();

    // Create Velocity
    iVelocity = COMASuplVelocity::NewL();

    if (iPosRequestor)
        {
        // Append the SUPL POS INIT optional parameters to the list
        // in POS Requestor
        iPosRequestor->AppendInfoRequest(iCurSetCapabilities);
        iPosRequestor->AppendInfoRequest(iReqAsstData);
        iPosRequestor->AppendInfoRequest(iPosition);
        iPosRequestor->AppendInfoRequest(iPosPayload);
        iPosRequestor->AppendInfoRequest(iVelocity);

        TBuf<128> msg(_L("Filling iPosMethod in iPosRequestor: "));
        iTrace->Trace(msg, KTraceFileName, __LINE__);

        // Set self as Observer to POS Requestor
        //Comment to ignore coverity reverse NULL error
        //coverity[REVERSE_INULL :FALSE]
        iPosRequestor->SetObserver(this);
        iAllowedCapabilitiesforPOS.SetAllowedCapabilities(EFalse, EFalse,
                EFalse, EFalse, EFalse, EFalse, EFalse, EFalse);
        iPosRequestor->SetPosMethodAndAllowedCapabilities(
                iAllowedCapabilitiesforPOS, iPosMethod);
        if (iHSLPAddress)
            {
            HBufC* slpAddress = CnvUtfConverter::ConvertToUnicodeFromUtf8L(
                    *iHSLPAddress);
            msg.Copy(_L("Filling iHSLPAddress in iPosRequestor: "));
            msg.Append(*slpAddress);
            iTrace->Trace(msg, KTraceFileName, __LINE__);
            iPosRequestor->SetSLPAddressUsed(*slpAddress);
            }
        // Get the information from POS
        User::LeaveIfError(iPosRequestor->GetSuplInfoL());
        }

    }
// -----------------------------------------------------------------------------
// COMASuplPosInitState::OperationCompleteL
// An observer method that indicates the completion of
// data retrieval from POS Message plugin
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void COMASuplPosInitState::OperationCompleteL(TInt  aError ) 
	{
		if(iMsgStateObserver)
		{
			iGenerationStatus = EFalse;
			if(aError == KErrNone)
			    {
			    iLocationIDRequestor->GetComparisionLocationID();
			    }
			else
			    iMsgStateObserver->OperationCompleteL(aError);
		}
	}
// -----------------------------------------------------------------------------
// COMASuplPosInitState::ValidateSetCapabilities
// Collects the optional parameters for SUPL_POS_INIT 
// message from POS Message plugin
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
TInt COMASuplPosInitState::ValidateSetCapabilities() 
	{
		if(!iPosRequestor || iRequestType == COMASuplSession::ESUPL_NETWORK )  //No need to check status if POS is not there
			{
				return KErrNone;  
			}
			
		TInt status = iCurSetCapabilities->Status();  // Caps from POS
		
		if(status == KErrNone)
		{
 
		
			TOMASuplPosTechnology oldPosTech, curPosTech;
			COMASuplSETCapabilities::TOMASuplPrefMethod oldPrefMethod, curPrefMethod;
			TOMASuplPosProtocol oldPosProtocol, curPosProtocol;
		
			iOldCapabilities->GetSETCapabilities(oldPosTech, oldPrefMethod, oldPosProtocol);
		
			iCurSetCapabilities->GetSETCapabilities(curPosTech, curPrefMethod, curPosProtocol);
			
			TBool oldGpsSETAssisted, curGpsSETAssisted, oldGpsSETBased, curGpsSETBased;
			TBool oldAutonomousGps, curAutonomousGps, oldaFLT, curaFLT, oldAeOTD, curAeOTD;
			TBool oldOTDOA, curoTDOA;
			
			oldPosTech.GetPosTechnology(oldGpsSETAssisted, oldGpsSETBased, oldAutonomousGps, oldaFLT,
										oldAeOTD, oldOTDOA);
										
			curPosTech.GetPosTechnology(curGpsSETAssisted, curGpsSETBased, curAutonomousGps, curaFLT,
										curAeOTD, curoTDOA);
										
			if((oldGpsSETAssisted == curGpsSETAssisted) && (oldGpsSETBased == curGpsSETBased) &&
			  (oldAutonomousGps == curAutonomousGps) && (oldaFLT == curaFLT) && (oldAeOTD == curAeOTD)
			  && (oldOTDOA == curoTDOA))
			{
				TBool oldTIA801, curTIA801, oldRRLP, curRRLP, oldRRC, curRRC;
				
				oldPosProtocol.GetPosProtocol(oldTIA801, oldRRLP, oldRRC);
				curPosProtocol.GetPosProtocol(curTIA801, curRRLP, curRRC);
				
				if((oldTIA801 == curTIA801) && (oldRRLP == curRRLP) && (oldRRC == curRRC))
				{
					if(oldPrefMethod == curPrefMethod)
					{
						return KErrNone;
					}
				}
			}
			
			return KErrOMASETCapMisMatch;
		
		}
		
		return status;	
	}


	
// -----------------------------------------------------------------------------
// COMASuplPosInitState::EncodeMessageL
// Encodes message ...returns encoded data
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
HBufC8* COMASuplPosInitState::EncodeMessageL(TOMASuplVersion &aSuplVersion,
							  COMASuplSessionID* aSessionId,TInt &aError)
	{

		TInt error = KErrNone; 

		iTrace->Trace(_L("Start of COMASuplPosInitState::EncodeMessageL"), KTraceFileName, __LINE__); 					
		COMASuplState::EncodeMessageL(aSuplVersion,aSessionId,aError);

		if(iRequestType == COMASuplSession::ESUPL_NETWORK )
			{
				UpdateCapabilities(); //update with caps from POS 
			}
			
		aError = ValidateSetCapabilities(); 
		if(aError != KErrNone)
			{
				return NULL;
			}
		
		COMASuplAsnMessageBase* OMASuplMessageBase = iOMASuplAsnHandlerBase->CreateOMASuplMessageL(COMASuplAsnMessageBase::ESUPL_POSINIT);
		COMASuplPosInit* OMASuplPosInit  =  static_cast<COMASuplPosInit*>(OMASuplMessageBase);
		CleanupStack::PushL(OMASuplMessageBase);
		
		iTrace->Trace(_L("Created COMASuplPosInit"), KTraceFileName, __LINE__); 							
		
		OMASuplPosInit->SetMessageBase(aSuplVersion,aSessionId); 
		
		if(iEncodedSuplInit)
			{
				OMASuplPosInit->SetVer(iRes);		
			}
		
		//iLocationId ownership will be with "this" class
		ComputeCapabilities();

		if(iOldCapabilities && iPosRequestor)
		{
			iTrace->Trace(_L("Filling capability for POS....."), KTraceFileName, __LINE__);
			
			TOMASuplPosTechnology posTech;
			COMASuplSETCapabilities::TOMASuplPrefMethod prefMethod;
			TOMASuplPosProtocol posProtocol;

			iOldCapabilities->GetSETCapabilities(posTech, prefMethod, posProtocol);

            TBool aGpsSetAssisted, aGpsSetBased, autonomousGps, AFLT, EOTD, OTDOA;
			posTech.GetPosTechnology(aGpsSetAssisted, aGpsSetBased, autonomousGps, AFLT, EOTD, OTDOA);

            iAllowedCapabilitiesforPOS.SetAllowedCapabilities(aGpsSetAssisted, aGpsSetBased, autonomousGps, AFLT, EOTD, OTDOA, ETrue, ETrue);

		    iPosRequestor->SetPosMethodAndAllowedCapabilities (iAllowedCapabilitiesforPOS,iPosMethod  ); 
        }
		if(iOldCapabilities)
		    OMASuplPosInit->SetSuplPosInit(static_cast<COMASuplSETCapabilities*>(iOldCapabilities->CloneL()),static_cast<COMASuplLocationId*>(iLocationId->CloneL()),iECId);
		if(iECId)
			{
				OMASuplPosInit->SetECellId(iMmCellInfo);
			}
		
		// Fill up the optional parameters

		// Requested Assistance Data
		if(iReqAsstData)
			{
				if(KErrNone == iReqAsstData->Status())
					{
						COMASuplReqAsstData* reqAsstData =static_cast <COMASuplReqAsstData *> (iReqAsstData->CloneL());
						OMASuplPosInit->SetRequestAssistanceData(reqAsstData);
					}
			}

		// Position
		if(iPosition)
			{
			if(KErrNone == iPosition->Status() && !iIgnorePosData) //if iIgnorePosData is set do not use the pos data
				{
					COMASuplPosition* position =static_cast <COMASuplPosition *>(iPosition->CloneL());
					position->SetType(COMASuplInfoRequest::EOMASuplPosition);
					OMASuplPosInit->SetPosition(position);
				}
			else if(iIgnorePosData)
			    iTrace->Trace(_L("COMASuplPosInitState::EncodeMessageL ignoring pos data"), KTraceFileName, __LINE__);
			iIgnorePosData = EFalse; //reset the status back as we ignored pos data once
			}
			  
		// POS Payload     
		if(iPosPayload)
			{
			if(KErrNone == iPosPayload->Status())
				{
					COMASuplPosPayload *posPayload = static_cast <COMASuplPosPayload *>(iPosPayload->CloneL());
					StripPospayLoadL();
					OMASuplPosInit->SetSuplPosPayload(posPayload);
                    iPosPayloadPresent = ETrue;
				}
			}

		// Velocity
		if(iVelocity)
			{
				if(KErrNone == iVelocity->Status())
					{				
						COMASuplHorizVelocity* horVel = iVelocity->Velocity();
						if(!horVel)
						{
							horVel = COMASuplHorizVelocity::NewL();
							iVelocity->SetVelocity(horVel);
						}
						COMASuplVelocity *velocity =static_cast <COMASuplVelocity *>(iVelocity->CloneL());
						OMASuplPosInit->SetVelocity(velocity);
					}
			}
#ifdef PRINT_MESSAGE

		LogSETCapabilities();
		LogPosPayloadData();
		LogPosition();
		LogReqAssistanceData();
		if(iVelocity)
			{
		if(KErrNone == iVelocity->Status())
			{	
				iTrace->Trace(_L("Velocity data in SUPL_POSINIT "), KTraceFileName, __LINE__);
				LogVelocity(iVelocity);
			}
		else
			{
				iTrace->Trace(_L("No Velocity data provided for SUPL_POSINIT by POS message plugin"), KTraceFileName, __LINE__);	
				}
			}
#endif
	
		iTrace->Trace(_L("Starting Encoding...SUPL_POS_INIT"), KTraceFileName, __LINE__); 							
		
		HBufC8 *encodedBuffer = NULL;
		
		TRAPD(err,encodedBuffer = iOMASuplAsnHandlerBase->EncodeL(OMASuplPosInit,error));
		CleanupStack::PopAndDestroy(OMASuplMessageBase);
		
		
		 if ( error == KErrNone && err == KErrNone )
		 	{
		 			iTrace->Trace(_L("Encoding Done..."), KTraceFileName, __LINE__); 							
		 			aError = KErrNone;
#ifdef PRINT_MESSAGE
                    RFile file;
	                RFs   fs;
		            TInt cErr=fs.Connect();
	                TInt fErr = file.Open(fs,_L("c:\\logs\\epos\\packet.txt"),EFileWrite|EFileShareAny);
                    if (fErr == KErrNotFound)
                        {                            
                        file.Create(fs, _L("c:\\logs\\epos\\packet.txt"), EFileWrite|EFileShareAny);
	                    fErr = file.Open(fs,_L("c:\\logs\\epos\\packet.txt"),EFileWrite|EFileShareAny);
                        }
                    TInt aPos;
                    file.Seek(ESeekEnd, aPos);
                    _LIT8(KMsgType, "SUPL POS INIT");
                    file.Write(KMsgType);
                    file.Write(_L8("\r \n"));
	                file.Write(*encodedBuffer);
                    file.Write(_L8("\r \n"));
	                file.Close();
	                fs.Close();
#endif
					return encodedBuffer;
		 	}
		 else
		 	{
		 		iTrace->Trace(_L("Encoding Failed."), KTraceFileName, __LINE__); 
				if( error == KErrNone) 
					{
						delete iLocationId;
						iLocationId = NULL;
					}
				aError = error;	
		 		delete encodedBuffer;
		 		encodedBuffer = NULL;
		 	}
		 	return NULL;
	}

void COMASuplPosInitState::GetPositionComplete(TInt /*aError*/)
{
}

   
// -----------------------------------------------------------------------------
// COMASuplPosInitState::ComparisionLocationIDRequestCompletedL
// Gets called when location request for comparision completed
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void COMASuplPosInitState::ComparisionLocationIDRequestCompletedL(COMASuplLocationId* aLocationId,
        TInt aErrorCode)
    {
    
    iTrace->Trace(_L("COMASuplPosInitState::ComparisionLocationIDRequestCompletedL..."), KTraceFileName, __LINE__);      
    
    if(aErrorCode!=KErrNone)
        {
        iTrace->Trace(_L("COMASuplPosInitState::ComparisionLocationIDRequestFailed"), KTraceFileName, __LINE__);       
        if(iMsgStateObserver)
            {
            iMsgStateObserver->OperationCompleteL(aErrorCode);
            return;
            }
        }
    //if the current Cell id is the same as the Cell id when we made the request for assistance data then use the assistance data
    //and we have not already repeated a request before
    if (!iRepeatedPosDataFetch && iLocationId->GetCellId()
            == aLocationId->GetCellId())
        {
        //deleting locationId before assigning it again
        delete iLocationId;
        iLocationId = NULL;
        iLocationId = aLocationId;

        iTrace->Trace(
                _L("COMASuplPosInitState::ComparisionLocationIDRequestCompletedL Cell Id not changed since making last request, so continuing"),
                KTraceFileName, __LINE__);
        if (iMsgStateObserver)
            iMsgStateObserver->OperationCompleteL(aErrorCode);
        }
    else // Cell id has changed since previous request so assistance data may be invalid
        {
        iTrace->Trace(_L("COMASuplPosInitState::ComparisionLocationIDRequestCompletedL Cell Id changed since making last request, retrieving toe limit"), KTraceFileName, __LINE__);
        TOMASuplNavigationModel navModel;
        if(KErrNone == iReqAsstData->GetNavigationData(navModel))
            {
            TInt gpsWeek, gpsToe, NSAT, toeLimit;
            navModel.GetNavigationModel(gpsWeek, gpsToe, NSAT, toeLimit);
            if(toeLimit >= KMaxCellIdChangeToeLimit) 
                {
                iTrace->Trace(_L("COMASuplPosInitState::ComparisionLocationIDRequestCompletedL toe limit greater than KMaxCellIdChangeToeLimit. getting  pos data again"), KTraceFileName, __LINE__);
                if(!iRepeatedPosDataFetch)//if cell id changed and we have not already repeated fetching pos data
                    {
                    iLocationId = aLocationId; //update the location id member for the next request as we are making a request again
                    iRepeatedPosDataFetch = ETrue;

                    iTrace->Trace(
                            _L("COMASuplPosInitState::ComparisionLocationIDRequestCompletedL toe limit greater than KMaxCellIdChangeToeLimit. getting  pos data again"),
                            KTraceFileName, __LINE__);
                    GetPosParamsL();
                    }
                else
                    {
                    iLocationId = aLocationId;
                    iRepeatedPosDataFetch = EFalse; //reset the status

                    iTrace->Trace(
                            _L("COMASuplPosInitState::ComparisionLocationIDRequestCompletedL toe limit greater than KMaxCellIdChangeToeLimit. But repeated a pos fetch already so continuing"),
                            KTraceFileName, __LINE__);
                    if (iMsgStateObserver)
                        iMsgStateObserver->OperationCompleteL(aErrorCode);//use the pos data as it is
                    }
                }
            else //ignore the position data
                {
                iLocationId = aLocationId;
                iIgnorePosData = ETrue;
                iTrace->Trace(
                        _L("COMASuplPosInitState::ComparisionLocationIDRequestCompletedL toe limit less than KMaxCellIdChangeToeLimit. Ignoring pos data"),
                        KTraceFileName, __LINE__);
                iRepeatedPosDataFetch = EFalse; //reset the status

                if (iMsgStateObserver)
                    iMsgStateObserver->OperationCompleteL(aErrorCode);
                }
            }
        }
    }


// -----------------------------------------------------------------------------
// COMASuplPosInitState::ComputeCapabilities
// Does ANDing of SETCapabilities with AllowedCapabilities & stores in SETCapabilities
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void COMASuplPosInitState::ComputeCapabilities()
{	
	iTrace->Trace(_L("Start of COMASuplPosInitState::ComputeCapabilities"), KTraceFileName, __LINE__); 															
	
	TOMASuplPosProtocol   SETposProtocol;
	COMASuplSETCapabilities::TOMASuplPrefMethod    SETprefMethod;
	TOMASuplPosTechnology SETposTechnology;
	
	TOMASuplPosProtocol   AllowedposProtocol;
	COMASuplSETCapabilities::TOMASuplPrefMethod    AllowedprefMethod;
	TOMASuplPosTechnology AllowedposTechnology;
	
	TBool  SETGpsSETAssisted,AllowedGpsSETAssisted;
	TBool  SETGpsSETBased,AllowedGpsSETBased;
	TBool  SETAutonomousGps,AllowedAutonomousGps;
	TBool  SETeOTD,AllowedeOTD;
	TBool  SEToTDOA,AllowedoTDOA;
	TBool SETFLT, AllowedFLT;	
	TBool SETTIA801,AllowedTIA801; 
	TBool SETRRLP,AllowedRRLP;
	TBool SETRRC,AllowedRRC;
	
	iOldCapabilities->GetSETCapabilities(SETposTechnology,SETprefMethod,SETposProtocol);	
	iAllowedCapabilities->GetSETCapabilities(AllowedposTechnology,AllowedprefMethod,AllowedposProtocol);	

	SETposTechnology.GetPosTechnology(SETGpsSETAssisted,SETGpsSETBased,SETAutonomousGps,SETFLT,
													   SETeOTD,SEToTDOA);
	SETposProtocol.GetPosProtocol(SETTIA801,SETRRLP,SETRRC);

	AllowedposTechnology.GetPosTechnology(AllowedGpsSETAssisted,AllowedGpsSETBased,AllowedAutonomousGps,AllowedFLT,
													   AllowedeOTD,AllowedoTDOA);
	SETposProtocol.GetPosProtocol(AllowedTIA801,AllowedRRLP,AllowedRRC);
	AllowedprefMethod = COMASuplSETCapabilities::EOMANoPreferred; 

	SETGpsSETAssisted &= AllowedGpsSETAssisted;
	SETGpsSETBased &= AllowedGpsSETBased;
	SETAutonomousGps &= AllowedAutonomousGps;
	
	SETFLT &= AllowedFLT; 
	SETeOTD &=AllowedeOTD;
	SEToTDOA &=AllowedoTDOA;
	
	SETTIA801 &= AllowedTIA801; 
	SETRRLP &= AllowedRRLP;
	SETRRC &= AllowedRRC;
	
	SETposTechnology.SetPosTechnology(SETGpsSETAssisted,SETGpsSETBased,SETAutonomousGps,SETFLT,
													   SETeOTD,SEToTDOA);
	
	SETposProtocol.SetPosProtocol(SETTIA801,SETRRLP,SETRRC);
	
	//If client has restricted SETBased and still POS msg plugin sets SETBasedPreferred.....then 
	// it as EOMANoPreferred
	
	if(SETprefMethod == COMASuplSETCapabilities::EOMAAGpsSETBasedPreferred && (!SETGpsSETBased ))
		{
			SETprefMethod = COMASuplSETCapabilities::EOMANoPreferred; 
		}
		
	if(SETprefMethod == COMASuplSETCapabilities::EOMAAGpsSETAssitedPreferred  && (!SETGpsSETAssisted))
		{
			SETprefMethod = COMASuplSETCapabilities::EOMANoPreferred; 
		}
		
	iOldCapabilities->SetSETCapabilities(SETposTechnology,SETprefMethod,SETposProtocol);	
	
	iTrace->Trace(_L("End of COMASuplPosInitState::ComputeCapabilities"), KTraceFileName, __LINE__); 															
}

// -----------------------------------------------------------------------------
// COMASuplPosState::CancelOperation
// Cancels Operation...which will be depend on derived class implementation
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void COMASuplPosInitState::CancelOperation() 
	{ 
		 COMASuplStartState::CancelOperation();
		 iTrace->Trace(_L("COMASuplPosState::CancelOperation..."), KTraceFileName, __LINE__); 							
	}	

// -----------------------------------------------------------------------------
// COMASuplPosState::UpdateCapabilities
//
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void COMASuplPosInitState::UpdateCapabilities() 
	{
		if(!iPosRequestor)
			{
				return;
			}
			
		TOMASuplPosTechnology  curPosTech;
		COMASuplSETCapabilities::TOMASuplPrefMethod  curPrefMethod;
		TOMASuplPosProtocol  curPosProtocol;
		iCurSetCapabilities->GetSETCapabilities(curPosTech, curPrefMethod, curPosProtocol);
		iOldCapabilities->SetSETCapabilities(curPosTech, curPrefMethod, curPosProtocol);
	}
	
// -----------------------------------------------------------------------------
// COMASuplPosState::IsGenerationInProgress
//
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
TBool COMASuplPosInitState::IsGenerationInProgress()
	{
		return iGenerationStatus;
	}

// -----------------------------------------------------------------------------
// COMASuplPosInitState::StartTimer
// StartTimer...starts the timer ... 
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void COMASuplPosInitState::StartTimer()
{
	iTrace->Trace(_L("COMASuplPosInitState::StartTimer..."), KTraceFileName, __LINE__); 							
	if(!iPosPayloadPresent)
		{
			iTrace->Trace(_L("Timer Started"), KTraceFileName, __LINE__); 
			COMASuplState::StartTimer();
		}
    else
        {            
			iTrace->Trace(_L("Timer Not Started"), KTraceFileName, __LINE__); 
        }
}


TBool COMASuplPosInitState::IsPosPayloadPresent()
{
	if(iPosPayloadPresent)
	{
		return ETrue;	
	}
	else
	{
		return  EFalse;	
	}
	 
}

// -----------------------------------------------------------------------------
// COMASuplPosInitState::StripPospayLoadL
// Strip the data if it exceeds more than 8 KB
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void COMASuplPosInitState::StripPospayLoadL()
{
			HBufC8* payload;
			COMASuplPosPayload::TOMASuplPosPayloadType payloadType;
			
			iPosPayload->GetPosPayload(payload,payloadType);
			
			if(! payload) 
				{
					return;
				}
			
			TInt len = payload->Length();
			if(len > KMaxPosPayloadLength)  //Strip the data if it exceeds more than 8 Kb
				{
					HBufC8* remainingPospayloadData = HBufC8::NewL(len - KMaxPosPayloadLength);
					remainingPospayloadData->Des().Copy(payload->Des().Mid(KMaxPosPayloadLength));// Copy rest data
					iRemainingPospayload = static_cast <COMASuplPosPayload *>(iPosPayload->CloneL());	// Ownnership will not be with this class
					iRemainingPospayload->SetPosPayload(remainingPospayloadData,payloadType);			
					payload->Des().Copy(payload->Des().Left(KMaxPosPayloadLength));// Remove rest of data
					LogPacket(payload->Des());
				}
				
			//Remove following code after testing...	
			iPosPayload->GetPosPayload(payload,payloadType);
			len = payload->Length();

}

// -----------------------------------------------------------------------------
// COMASuplPosInitState::GetRemainingPospayLoadL
// Copies remaining PosPayload data to send with SUPL_POS
// Callee gets ownership....
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
COMASuplPosPayload* COMASuplPosInitState::GetRemainingPospayLoadL()
{
	return iRemainingPospayload;
}

	
void COMASuplPosInitState::LogSETCapabilities()
	{
	
#ifdef PRINT_MESSAGE		

		if(iSETCapabilities)
		{
			iTrace->Trace(_L("SET Capabilities in POS INIT....."), KTraceFileName, __LINE__);
			
			TOMASuplPosTechnology posTech;
			COMASuplSETCapabilities::TOMASuplPrefMethod prefMethod;
			TOMASuplPosProtocol posProtocol;
			
			if(iOldCapabilities)
			iOldCapabilities->GetSETCapabilities(posTech, prefMethod, posProtocol);
		
			TBuf <200> posTechStr;
			posTechStr.Append(_L("POS Technologies - "));
			TBool gpsSetAssisted, gpsSetBased, autonomousGps, AFLT, EOTD, OTDOA;
			posTech.GetPosTechnology(gpsSetAssisted, gpsSetBased, autonomousGps, AFLT, EOTD, OTDOA);
			
			if(gpsSetAssisted) posTechStr.Append(_L(" AGPS SET Assisted  "));
			if(gpsSetBased) posTechStr.Append(_L(" AGPS SET Based  "));
			if(autonomousGps) posTechStr.Append(_L(" Autonomous GPS  "));
			if(AFLT) posTechStr.Append(_L(" AFLT  "));
			if(EOTD) posTechStr.Append(_L(" EOTD  "));
			if(OTDOA) posTechStr.Append(_L(" OTDOA  "));

			iTrace->Trace(posTechStr, KTraceFileName, __LINE__);

			TBuf <80> prefMethodStr;
			prefMethodStr.Append(_L("Preferred Method - "));

			switch(prefMethod)
			{
			case COMASuplSETCapabilities::EOMAAGpsSETAssitedPreferred:
				prefMethodStr.Append(_L(" AGPS SET Assisted Preferred"));
				break;
			case COMASuplSETCapabilities::EOMAAGpsSETBasedPreferred:
				prefMethodStr.Append(_L(" AGPS SET Based Preferred"));
				break;
			case COMASuplSETCapabilities::EOMANoPreferred:
				prefMethodStr.Append(_L(" No Preference"));
				break;
			}
			
			iTrace->Trace(prefMethodStr, KTraceFileName, __LINE__);

			TBuf <80> posProtocolStr;
			posProtocolStr.Append(_L("POS Protocol - "));
			TBool tia801, rrlp, rrc;
			
			posProtocol.GetPosProtocol(tia801, rrlp, rrc);
			if(tia801) posProtocolStr.Append(_L(" TIA 801  "));
			if(rrlp) posProtocolStr.Append(_L(" RRLP  "));
			if(rrc) posProtocolStr.Append(_L(" RRC  "));
			
			iTrace->Trace(posProtocolStr, KTraceFileName, __LINE__);
		}

		// Location ID
		if(iLocationId)
		{
			iTrace->Trace(_L("Location ID in POS_INIT......."), KTraceFileName, __LINE__);
			
			TBuf<50> locationIDTypeStr;
			locationIDTypeStr.Append(_L("Location ID Type - "));
			COMASuplLocationId::TOMASuplCellInfoType cellInfoType;
			COMASuplLocationId::TOMASuplStatus status;			
			TInt refMNC, refMcc, refCI, refLAC;
			TInt err;
			
			cellInfoType = iLocationId->SuplCellInfoType();
			switch(cellInfoType)
			{	
			case COMASuplLocationId::EGSM:
				locationIDTypeStr.Append(_L(" GSM "));
				iTrace->Trace(locationIDTypeStr, KTraceFileName, __LINE__);

				COMASuplGSMCellInfo *gsmCellInfo ;

				err = iLocationId->SuplLocationId(gsmCellInfo, status);
				if(KErrNone == err)
				{
					TBuf <100> gsmCellInfoStr;
					gsmCellInfoStr.Append(_L("GSM Cell Info - "));
					gsmCellInfo->SuplGSMCellInfo(refMNC, refMcc, refCI, refLAC);
					gsmCellInfoStr.Append(_L("MNC - "));
					gsmCellInfoStr.AppendNum(refMNC);
					gsmCellInfoStr.Append(_L(" MCC - "));
					gsmCellInfoStr.AppendNum(refMcc);
					gsmCellInfoStr.Append(_L(" CI - "));
					gsmCellInfoStr.AppendNum(refCI);
					gsmCellInfoStr.Append(_L(" LAC - "));
					gsmCellInfoStr.AppendNum(refLAC);

					iTrace->Trace(gsmCellInfoStr, KTraceFileName, __LINE__);
				}
				break;

			case COMASuplLocationId::EWCDMA:
				locationIDTypeStr.Append(_L(" WCDMA "));
				iTrace->Trace(locationIDTypeStr, KTraceFileName, __LINE__);

				COMASuplCellInfo *wcdmaCellInfo ;

				err = iLocationId->SuplLocationId(wcdmaCellInfo, status);
				if(KErrNone == err)
				{
					TBuf <100> wcdmaCellInfoStr;
					wcdmaCellInfoStr.Append(_L("WCDMA Cell Info - "));
					wcdmaCellInfo->SuplCellInfo(refMNC, refMcc, refCI);
					wcdmaCellInfoStr.Append(_L(" MNC - "));
					wcdmaCellInfoStr.AppendNum(refMNC);
					wcdmaCellInfoStr.Append(_L(" MCC - "));
					wcdmaCellInfoStr.AppendNum(refMcc);
					wcdmaCellInfoStr.Append(_L(" CI - "));
					wcdmaCellInfoStr.AppendNum(refCI);

					iTrace->Trace(wcdmaCellInfoStr, KTraceFileName, __LINE__);
				}

				break;
			}
			TBuf <60> locationIdStatusStr;
			locationIdStatusStr.Append(_L("Location ID Status - "));
			switch(status)
			{
			case COMASuplLocationId::EStale:
				locationIdStatusStr.Append(_L("Stale"));
				break;
			case COMASuplLocationId::ECurrent:
				locationIdStatusStr.Append(_L("Current"));
				break;
			case COMASuplLocationId::EUnknown:
				locationIdStatusStr.Append(_L("Unknown"));
				break;
			}
			iTrace->Trace(locationIdStatusStr, KTraceFileName, __LINE__);
		}


#endif
	}
	
void COMASuplPosInitState::LogPosPayloadData()
{

#ifdef PRINT_MESSAGE
			
		if(iPosPayload)
		{
			TInt posPosPayLoadError = iPosPayload->Status();					
			if(posPosPayLoadError != KErrNone )
			{
				iTrace->Trace(_L("No POS Payload data provided for SUPL_POSINIT by POS message plugin"), KTraceFileName, __LINE__);	
				return;
			}
			
		iTrace->Trace(_L("**************** Start of SUPL_POSINIT PosPayLoad provided by POS message plugin **********************"), KTraceFileName, __LINE__);

		HBufC8* payLoad = NULL;
		COMASuplPosPayload::TOMASuplPosPayloadType payLoadType;
		
		iPosPayload->GetPosPayload(payLoad,payLoadType);
		HBufC* payLoadBuffer = NULL; 
		if(payLoad)
			{
			TInt err;
			TRAP(err,payLoadBuffer = HBufC::NewL(payLoad->Length()));
			err = 0;
			payLoadBuffer->Des().Copy(payLoad->Des());
			}
		
		if(payLoadType == COMASuplPosPayload::ETIA801)
			{
			iTrace->Trace(_L("POS DATA Type - ETIA801 "), KTraceFileName, __LINE__);
			if(payLoadBuffer)
			  iTrace->Trace(payLoadBuffer->Des(),KTraceFileName, __LINE__);
			}
		else if(payLoadType == COMASuplPosPayload::ERRLP)
			{
			iTrace->Trace(_L("POS DATA Type - ERRLP "), KTraceFileName, __LINE__);
			if(payLoadBuffer)
				iTrace->Trace(payLoadBuffer->Des(),KTraceFileName, __LINE__);	
			}
		else if(payLoadType == COMASuplPosPayload::ERRC)
			{
			iTrace->Trace(_L("POS DATA Type - ERRC "), KTraceFileName, __LINE__);
			if(payLoadBuffer)
				iTrace->Trace(payLoadBuffer->Des(),KTraceFileName, __LINE__);		
			}
		delete payLoadBuffer;
		payLoadBuffer = NULL;
		}
#endif					
}

void COMASuplPosInitState::LogPosition()
{

#ifdef PRINT_MESSAGE
 	if(!iPosition)
 		return;
 	
	if(KErrNone != iPosition->Status())
		{
			iTrace->Trace(_L("No Position is provided by POS message plugin for SUPL_POSINIT "), KTraceFileName, __LINE__);	
			return;
		}

	  	TBuf <300> positionLog;
	    TOMASuplUtcTime utcTime;
	    TOMASuplPositionEstimate posEstimate;
		TDateTime lDateTime;
		TInt  	  zoneCode;
		TInt      zone;
		TInt error;
		iTrace->Trace(_L("-------Start of Position in SUPL_POSINIT ----------"), KTraceFileName, __LINE__);			        
		
	    iPosition->GetPosition(utcTime,posEstimate );	
	    utcTime.GetUtcTime(lDateTime,zoneCode,zone);		
	    
		positionLog.Append(_L("UTC Time Log : "));
		positionLog.Append(_L("Year : "));
		positionLog.AppendNum(lDateTime.Year());
		positionLog.Append(_L("Month : "));
		positionLog.AppendNum(lDateTime.Month());
		positionLog.Append(_L("Day : "));
		positionLog.AppendNum(lDateTime.Day());	
		positionLog.Append(_L("Hour : "));
		positionLog.AppendNum(lDateTime.Hour());	
		positionLog.Append(_L("Minute : "));
		positionLog.AppendNum(lDateTime.Minute());	
		positionLog.Append(_L("Second : "));
		positionLog.AppendNum(lDateTime.Second());	
		positionLog.Append(_L("MicroSecond : "));
		positionLog.AppendNum(lDateTime.MicroSecond());	
		positionLog.Append(_L("Zone Code : "));
		positionLog.AppendNum(zoneCode);	
		positionLog.Append(_L("Zone : "));
		positionLog.AppendNum(zone);
			
		iTrace->Trace(positionLog, KTraceFileName, __LINE__); 
		
		TOMASuplPositionEstimate::TOMASuplLatitudeSign  latSign;
	    TInt               latitude;
	    TInt               longitude;
	    posEstimate.GetPositionEstimate(latSign,latitude,longitude);
	    
	    positionLog.Copy(_L("Position Estimate : "));
		positionLog.Append(_L(" latitude Sign : "));
		positionLog.AppendNum(latSign);
		positionLog.Append(_L(" Latitude : "));
		positionLog.AppendNum(latitude);
		positionLog.Append(_L(" Longitude : "));
		positionLog.AppendNum(longitude);	
		
		iTrace->Trace(positionLog, KTraceFileName, __LINE__); 

		TOMASuplUncertainty uncertainty;
		error = posEstimate.GetUncertainty(uncertainty);
		if(!error)
			{
			TInt uncertSemiMajor;
	       	TInt uncertSemiMinor;
	       	TInt orientMajorAxis;
	       	uncertainty.GetUncertainty(uncertSemiMajor,uncertSemiMinor,orientMajorAxis);
	       	
	       	positionLog.Copy(_L("Uncertainity Information of Position "));
	       	positionLog.Append(_L(" Uncertainity Semi Major : "));
			positionLog.AppendNum(uncertSemiMajor);
			positionLog.Append(_L(" Uncertainity Semi Minor : "));
			positionLog.AppendNum(uncertSemiMinor);
			positionLog.Append(_L(" Orientation Major Axis : "));
			positionLog.AppendNum(orientMajorAxis);	
			iTrace->Trace(positionLog, KTraceFileName, __LINE__);
			}
		else
			{
				iTrace->Trace(_L("Error in retriving Uncertinity"), KTraceFileName, __LINE__);
			}	

		// Retrieve Confidence
		TInt confidence;
		error = posEstimate.GetConfidence(confidence);
		if(!error)
			{
			positionLog.Copy(_L("Confidence in Position : "));
			positionLog.AppendNum(confidence);	
			iTrace->Trace(positionLog, KTraceFileName, __LINE__);
			}
		else
			{
		 	iTrace->Trace(_L("Error in retriving Confidence"), KTraceFileName, __LINE__);
			}	

		TOMASuplAltitudeInfo altitudeInfo;
		error = posEstimate.GetAltitudeInfo(altitudeInfo);
		if(!error)
			{
			TOMASuplAltitudeInfo::TOMASuplAltitudeDirection altDirection;
	    	TInt altitude;
	       	TInt altUncert;
	       	altitudeInfo.GetAltitudeInfo(altDirection,altitude,altUncert);
	       	
	       	positionLog.Copy(_L("Altitude Information of Position "));
	       	
	       	positionLog.Append(_L("Altitude Direction : "));
			positionLog.AppendNum(altDirection);
			positionLog.Append(_L(" Altitude : "));
			positionLog.AppendNum(altitude);
			positionLog.Append(_L(" Altitude Uncertainity : "));
			positionLog.AppendNum(altUncert);
			iTrace->Trace(positionLog, KTraceFileName, __LINE__);
			}
		else
			{
			iTrace->Trace(_L("Error in retriving AltitudeInfo"), KTraceFileName, __LINE__);
			}	


		COMASuplVelocity* velocity = NULL;
	    error = iPosition->GetVelocity(velocity);
	    if(!error)			
	    	{
	    	iTrace->Trace(_L("Velocity Information for position in SUPL_POS_INIT "), KTraceFileName, __LINE__);
	    	LogVelocity(velocity);
	    	}
		else
			{
			iTrace->Trace(_L("Error in retriving Velocity"), KTraceFileName, __LINE__);
			}	
			
		iTrace->Trace(_L("-------End of Position in SUPL_POSINIT ----------"), KTraceFileName, __LINE__);			        			

#endif					

}

void COMASuplPosInitState::LogReqAssistanceData()
{

#ifdef PRINT_MESSAGE
		 	if(!iReqAsstData)
		 		return;
 	
			if(KErrNone != iReqAsstData->Status())
				{
					iTrace->Trace(_L("No Assistance data is provided by POS message plugin for SUPL_POSINIT "), KTraceFileName, __LINE__);	
					return;
				}
				
			iTrace->Trace(_L("-------Start of Assistance data in SUPL_POSINIT ----------"), KTraceFileName, __LINE__);			        	
			TOMASuplNavigationModel navModel;
			TBool almanacReq, utcModel, ionModel, dgpsCorrect, 
				  refeLocation, referenceTimeRequested,acquisition, realTime;
					
			iReqAsstData->GetReqAsstData(almanacReq, utcModel, ionModel, dgpsCorrect, refeLocation, 
						 referenceTimeRequested, acquisition, realTime);
						 
			TBuf <300> reqAsstDataStr;
			reqAsstDataStr.Append(_L("Requested Assistance Data - "));
						 
			if(almanacReq) reqAsstDataStr.Append(_L(" Almanac Requested "));
			if(utcModel) reqAsstDataStr.Append(_L(" UTC Model Requested "));
			if(ionModel) reqAsstDataStr.Append(_L(" Ionospheric Model Requested "));
			if(dgpsCorrect) reqAsstDataStr.Append(_L(" DGPS Corrections "));
			if(refeLocation) reqAsstDataStr.Append(_L(" Reference Location "));
			if(referenceTimeRequested) reqAsstDataStr.Append(_L(" Reference Time "));
			if(acquisition) reqAsstDataStr.Append(_L(" Acquisition Assistance "));
			if(realTime) reqAsstDataStr.Append(_L(" Real Time Integrity "));
			
			iTrace->Trace(reqAsstDataStr, KTraceFileName, __LINE__); 			 
			
			
			if(KErrNone == iReqAsstData->GetNavigationData(navModel))
			{
				TBuf <80> satInfoStr;
				satInfoStr.Append(_L("Satellite Info  - "));

				TInt gpsWeek, gpsToe, NSAT, toeLimit;
				navModel.GetNavigationModel(gpsWeek, gpsToe, NSAT, toeLimit);
				
				if(gpsWeek) satInfoStr.Append(_L(" GPS Week "));
				if(gpsToe) satInfoStr.Append(_L(" GPS Toe "));
				if(NSAT) satInfoStr.Append(_L(" NSAT "));
				if(toeLimit) satInfoStr.Append(_L(" TOE Limit"));
				
				iTrace->Trace(satInfoStr, KTraceFileName, __LINE__);
				
				RArray<TOMASuplSatelliteInfoElement> satEleArr;
				
				if(KErrNone == navModel.GetSatInfoElement(satEleArr))
				{
					TInt count = satEleArr.Count();
					TBuf <50> satInfoElementStr;
					satInfoElementStr.Append(_L("Number of Satellite Info Elements - "));
					satInfoElementStr.AppendNum(count);
					iTrace->Trace(satInfoElementStr, KTraceFileName, __LINE__); 
					if(count > 0)
					{
						
						TOMASuplSatelliteInfoElement satInfoEle;
						
						for(TInt i = 0; i < count; i ++)
						{
							TInt err = navModel.GetSatInfoElement(satInfoEle, i);
							TInt satId,iode;
							
							satInfoEle.GetSatInfoElement(satId, iode);
							
							TBuf<50> satInfoEleStr;
							satInfoEleStr.Append(_L(" Satellite Info Element -  "));
							satInfoEleStr.AppendNum(satId);
							satInfoEleStr.Append(_L("    "));
							satInfoEleStr.AppendNum(iode);
							iTrace->Trace(satInfoEleStr, KTraceFileName, __LINE__); 
						}
					}
				}
				else
				{
					iTrace->Trace(_L("No Satelite Info OR error in retriving Satelite Info "), KTraceFileName, __LINE__); 
				}
				satEleArr.Close();
			}
			else
			{
				iTrace->Trace(_L("Error in Getting Navigationl Data... "), KTraceFileName, __LINE__); 
			}

	iTrace->Trace(_L("------- End of Assistance data in SUPL_POSINIT ----------"), KTraceFileName, __LINE__);			        	
	
#endif	
}

void COMASuplPosInitState::LogVelocity(COMASuplVelocity* 
#ifdef PRINT_MESSAGE
velocity
#endif
)
{

#ifdef PRINT_MESSAGE
		if(velocity)
		{		
		
		TBuf<256> msg;
		TOMASuplVelocityType velocityType = velocity->VelType();
		COMASuplHorizVelocity* horizVelocity = velocity->Velocity();
		TUint16 bearing;
		TUint16 horSpeed;
		switch(velocityType)	
			{
				case  EHorizVelocity:
					{
					iTrace->Trace(_L("Velocity Type : EHorizVelocity - Values "), KTraceFileName, __LINE__);
					horizVelocity->GetHorizVel(bearing,horSpeed);
					
					msg.Append(_L("Bearing : "));
					msg.AppendNum(bearing);
					msg.Append(_L("Horizontal Speed : "));
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
										
					msg.Append(_L("Bearing : "));
					msg.AppendNum(bearing);
					msg.Append(_L("Horizontal Speed : "));
					msg.AppendNum(horSpeed);
					msg.Append(_L("Vertical Direction : "));
					msg.AppendNum(verDirect);
					msg.Append(_L("Vertical Speed : "));
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
										
					msg.Append(_L("Bearing : "));
					msg.AppendNum(bearing);
					msg.Append(_L("Horizontal Speed : "));
					msg.AppendNum(horSpeed);
					msg.Append(_L("Uncertainity Speed : "));
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
										
					msg.Append(_L("Bearing : "));
					msg.AppendNum(bearing);
					msg.Append(_L("Horizontal Speed : "));
					msg.AppendNum(horSpeed);
					msg.Append(_L("Vertical Direction : "));
					msg.AppendNum(verDirect);
					msg.Append(_L("Vertical Speed : "));
					msg.AppendNum(verSpeed);					
					msg.Append(_L("Horiz Uncertain Speed : "));
					msg.AppendNum(horizUncertSpeed);
					msg.Append(_L("Vertical Uncertain Speed : "));
					msg.AppendNum(vertUncertSpeed);					
					iTrace->Trace(msg, KTraceFileName, __LINE__);																					
																
				    break;																
					}
			}
		}
#endif		
}

void COMASuplPosInitState::LogPacket(const TDesC8& 
#ifdef PRINT_MESSAGE
aPacket
#endif
)
{

#ifdef PRINT_MESSAGE
            RFile file;
            RFs   fs;
            TInt cErr=fs.Connect();
            
            TInt fErr = file.Open(fs,_L("c:\\logs\\epos\\POSINITpacket.txt"),EFileWrite|EFileShareAny);
			if (fErr == KErrNotFound)
			{                            
				file.Create(fs, _L("c:\\logs\\epos\\POSINITpacket.txt"), EFileWrite|EFileShareAny);
				fErr = file.Open(fs,_L("c:\\logs\\epos\\POSINITpacket.txt"), EFileWrite|EFileShareAny);
			}

			TInt aPos;
			file.Seek(ESeekEnd, aPos);
			file.Write(aPacket);
			file.Close();
			fs.Close();
#endif		
}

void COMASuplPosInitState::PrintHex(const TDesC8& 
#ifdef PRINT_MESSAGE
aBuffer
#endif
,
TInt 
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
	
//  End of File



