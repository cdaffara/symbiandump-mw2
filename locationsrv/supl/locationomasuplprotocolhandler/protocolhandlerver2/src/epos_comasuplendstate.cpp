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
* Description:   Class for handling SUPL_END message.
*
*/



#include <hash.h>

#include "epos_comasuplend.h"
#include "lbs/epos_comasuplposition.h"
#include "lbs/epos_eomasuplposerrors.h"
#include "lbs/epos_comasuplvelocity.h"

#include "epos_comasupltrace.h"
#include "epos_comasuplendstate.h"
#include "epos_omasuplconstants.h"

#include "epos_comasuplasnbase.h"

_LIT(KTraceFileName,"SUPL_OMA_SESSION::EPos_COMASuplEndState.cpp");

// ============================ MEMBER FUNCTIONS ===============================

// -----------------------------------------------------------------------------
// COMASuplEndState::COMASuplEndState
// C++ constructor can NOT contain any code, that might leave.
// -----------------------------------------------------------------------------
//
COMASuplEndState::COMASuplEndState(COMASuplEnd::TOMASuplStatusCode aStatusCode,COMASuplAsnHandlerBase* aOMASuplAsnHandlerBase):
																	 iOMASuplAsnHandlerBase(aOMASuplAsnHandlerBase)
    {
			iMessageTimeOut = 0; 
			iCurrentState = ESUPL_END;   
			iStatusCode = aStatusCode;
			iSetStatusCode = ETrue;
    }

// -----------------------------------------------------------------------------
// COMASuplEndState::COMASuplEndState
// C++ constructor can NOT contain any code, that might leave.
// -----------------------------------------------------------------------------
//
COMASuplEndState::COMASuplEndState(COMASuplAsnHandlerBase* aOMASuplAsnHandlerBase):iOMASuplAsnHandlerBase(aOMASuplAsnHandlerBase)
    {
			iMessageTimeOut = 0; 
			iCurrentState = ESUPL_END; 
			iSetStatusCode = EFalse;  
    } 
// -----------------------------------------------------------------------------
// COMASuplEndState::~COMASuplEndState
// -----------------------------------------------------------------------------
//
COMASuplEndState::~COMASuplEndState()
    {
    	delete iHSLPAddress;
    	delete iEncodedSuplInit;
    }

// -----------------------------------------------------------------------------
// COMASuplEndState::ConstructL
// Symbian 2nd phase constructor can leave.
// -----------------------------------------------------------------------------
//
void COMASuplEndState::ConstructL(HBufC8* aEncodedSuplInit, HBufC8* aHSLPAddress)
    {
    	COMASuplState::BaseConstructL();
    	iTrace->Trace(_L("COMASuplEndState::ConstructL..."), KTraceFileName, __LINE__); 							

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
// COMASuplEndState::NewL
// Two-phased constructor.
// -----------------------------------------------------------------------------
//
COMASuplEndState* COMASuplEndState::NewL(COMASuplEnd::TOMASuplStatusCode aStatusCode,COMASuplAsnHandlerBase* aOMASuplAsnHandlerBase,
										 HBufC8* aEncodedSuplInit,
										 HBufC8* aHSLPAddress)
    {
	    COMASuplEndState* self =  new ( ELeave ) COMASuplEndState(aStatusCode,aOMASuplAsnHandlerBase);
	    CleanupStack::PushL( self );
	    self->ConstructL(aEncodedSuplInit, aHSLPAddress);
	    CleanupStack::Pop(self);
	    return self;
    }
// -----------------------------------------------------------------------------
// COMASuplEndState::NewL
// Two-phased constructor.
// -----------------------------------------------------------------------------
//
COMASuplEndState* COMASuplEndState::NewL(COMASuplAsnHandlerBase* aOMASuplAsnHandlerBase,HBufC8* aEncodedSuplInit, HBufC8* aHSLPAddress)
    {
	    COMASuplEndState* self =  new ( ELeave ) COMASuplEndState(aOMASuplAsnHandlerBase);
	    CleanupStack::PushL( self );
	    self->ConstructL(aEncodedSuplInit, aHSLPAddress);
	    CleanupStack::Pop(self);
	    return self;
    }
// -----------------------------------------------------------------------------
// COMASuplEndState::GenerateMessage
// Generates SUPL_END message
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
TInt COMASuplEndState::GenerateMessageL() 
	{
		iTrace->Trace(_L("Start of COMASuplEndState::GenerateMessage"), KTraceFileName, __LINE__); 
		if(iEncodedSuplInit)
		{
			iVerPresent = ComputeSuplEndVerL();
			
			if(iVerPresent)
			{
				if(iMsgStateObserver)
				{
					iMsgStateObserver->OperationCompleteL(KErrNone);
					return KErrNone;
				}
			
			}
			else
			{
				iMsgStateObserver->OperationCompleteL(KErrGeneral);
				return KErrGeneral;
			}
		}
		else
		{
			if(iMsgStateObserver)
			{
				iMsgStateObserver->OperationCompleteL(KErrNone);
				return KErrNone;
			}
		}
		
		return KErrNone;		
	}
	
// -----------------------------------------------------------------------------
// COMASuplEndState::EncodeMessageL
// Encodes message ...returns encoded data
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
HBufC8* COMASuplEndState::EncodeMessageL(TOMASuplVersion &aSuplVersion,COMASuplSessionID* aSessionId,
										 TInt &aError)
	{
		iTrace->Trace(_L("Start of COMASuplEndState::Encode_SUPL_END"), KTraceFileName, __LINE__); 					
		
		COMASuplState::EncodeMessageL(aSuplVersion,aSessionId,aError);
		
	
		COMASuplAsnMessageBase* OMASuplMessageBase = iOMASuplAsnHandlerBase->CreateOMASuplMessageL(COMASuplAsnMessageBase::ESUPL_END);
		CleanupStack::PushL(OMASuplMessageBase);
		COMASuplEnd* OMASuplEnd  =  static_cast<COMASuplEnd*>(OMASuplMessageBase);
		
		iTrace->Trace(_L("**************** SUPL END **********************"), KTraceFileName, __LINE__);
		iTrace->Trace(_L("Created COMASuplEnd"), KTraceFileName, __LINE__); 							
		
		OMASuplEnd->SetMessageBase(aSuplVersion,aSessionId); 
		if(iSetStatusCode)
			{
	 			OMASuplEnd->SetStatusCode(iStatusCode);		
	 			iTrace->Trace(_L("Status Code in SUPL_END :"), KTraceFileName, __LINE__); 											
				TBuf<64> Res;			
				Res.AppendNum(iStatusCode);
				iTrace->Trace(Res, KTraceFileName, __LINE__); 
			}		
			
		if(iEncodedSuplInit)
			{
				OMASuplEnd->SetVer(iRes);

	 		}

		iTrace->Trace(_L("Starting Encoding...SUPL_END"), KTraceFileName, __LINE__); 							
		
		TInt error = KErrNone;
		HBufC8 *encodedBuffer = NULL;		

		TRAPD(err,encodedBuffer = iOMASuplAsnHandlerBase->EncodeL(OMASuplEnd,error));
		
		CleanupStack::PopAndDestroy(OMASuplEnd);
		
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
// COMASuplEndState::StartTimer
// StartTimer...starts the timer ... depending on type of message
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void COMASuplEndState::StartTimer()
	{
		iTrace->Trace(_L("COMASuplEndState::StartTimer...No Use."), KTraceFileName, __LINE__); 							
	}


// -----------------------------------------------------------------------------
// COMASuplEndState::ComputeSuplEndVerL
// Generates SUPL_POS_INIT message
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
TBool COMASuplEndState::ComputeSuplEndVerL()
{
	if(!iEncodedSuplInit) //Terminal initiated case
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
	    iRes.FillZ();
	    iRes.Zero();
	    TPtrC8 innerHashedMessage = sha1->Hash( messageKeyString->Des() );
	    HBufC8* step4Msg = HBufC8::NewLC(innerHashedMessage.Size());
	    step4Msg->Des().Append(innerHashedMessage);
	    sha1->Reset();
	    // Truncate the hash output to 64 bits
	   
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
// COMASuplEndState::ProcessMessage
// process message...
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
TInt COMASuplEndState::ProcessMessageL(COMASuplAsnMessageBase* aDecodedMessage)
{

			COMASuplEnd* OMASuplEnd = static_cast <COMASuplEnd *>(aDecodedMessage);
			COMASuplEnd::TOMASuplStatusCode statusCode;
			TInt error = OMASuplEnd->StatusCode(statusCode);
            iStatusCode = statusCode;
			OMASuplEnd->Position(iOMASuplPosition);	
			LogReceivedPosition();
            
			if(error == KErrOMASuplParamNotSet )
				{
				return KErrNone;
				}			
			else
				{
						return KErrCompletion;  //Error in status code
				}	
}

// -----------------------------------------------------------------------------
// COMASuplEndState::CancelOperation
// Cancels Operation...Cancels operation
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void COMASuplEndState::CancelOperation() 
	{ 
		 COMASuplState::CancelOperation();	
		 iTrace->Trace(_L("COMASuplEndState::CancelOperation..."), KTraceFileName, __LINE__); 							
	}	
	
// -----------------------------------------------------------------------------
// COMASuplEndState::GetPosition
// Retrive Position
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
COMASuplPosition* COMASuplEndState::GetPosition() const
{
		return iOMASuplPosition;
}

void COMASuplEndState::LogReceivedPosition() const 
	{
#ifdef PRINT_MESSAGE
            if (iOMASuplPosition)            
                {            
  	            TBuf <300> positionLog;
	            positionLog.Copy(_L("-------Position Log in SUPL END------------"));
                TOMASuplUtcTime utcTime;
                TOMASuplPositionEstimate posEstimate;
	            TDateTime lDateTime;
	            TInt  	  zoneCode;
	            TInt      zone;
	        
	            iTrace->Trace(positionLog, KTraceFileName, __LINE__); 
                iOMASuplPosition->GetPosition(utcTime,posEstimate );
                utcTime.GetUtcTime(lDateTime,zoneCode,zone);		
    
	            positionLog.Copy(_L("UTC Time Log : "));
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
	            iTrace->Trace(positionLog, KTraceFileName, __LINE__); 
	            positionLog.Copy(_L("MicroSecond : "));
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
	            positionLog.Append(_L("latitude Sign : "));
	            positionLog.AppendNum(latSign);
	            positionLog.Append(_L(" Latitude : "));
	            positionLog.AppendNum(latitude);
	            positionLog.Append(_L(" Longitude : "));
	            positionLog.AppendNum(longitude);	
	
	            iTrace->Trace(positionLog, KTraceFileName, __LINE__); 
	
	            TOMASuplUncertainty uncertainty;
	            TInt logError = posEstimate.GetUncertainty(uncertainty);
	            if(!logError)
		            {
		            TInt uncertSemiMajor;
       	            TInt uncertSemiMinor;
       	            TInt orientMajorAxis;
       	            uncertainty.GetUncertainty(uncertSemiMajor,uncertSemiMinor,orientMajorAxis);
       	
       	            positionLog.Copy(_L("Uncertainity Semi Major : "));
		            positionLog.AppendNum(uncertSemiMajor);
		            positionLog.Append(_L(" Uncertainity Semi Minor : "));
		            positionLog.AppendNum(uncertSemiMinor);
		            positionLog.Append(_L(" Orientation Major Axis : "));
		            positionLog.AppendNum(orientMajorAxis);	
		            }
	
	            iTrace->Trace(positionLog, KTraceFileName, __LINE__);
	            // Retrieve Confidence
	            TInt confidence;
	            logError = posEstimate.GetConfidence(confidence);
	            if(!logError)
		            {
		            positionLog.Copy(_L("Confidence in Position : "));	
		            positionLog.AppendNum(confidence);	
	                iTrace->Trace(positionLog, KTraceFileName, __LINE__);
		            }
	
	            TOMASuplAltitudeInfo altitudeInfo;
	            logError = posEstimate.GetAltitudeInfo(altitudeInfo);
	            if(!logError)
		            {
		            TOMASuplAltitudeInfo::TOMASuplAltitudeDirection altDirection;
    	            TInt altitude;
       	            TInt altUncert;
       	            altitudeInfo.GetAltitudeInfo(altDirection,altitude,altUncert);
       	
       	            positionLog.Copy(_L("Altitude Information "));
	                iTrace->Trace(positionLog, KTraceFileName, __LINE__);
       	
       	            positionLog.Copy(_L("Altitude Direction : "));
		            positionLog.AppendNum(altDirection);
		            positionLog.Append(_L(" Altitude : "));
		            positionLog.AppendNum(altitude);
		            positionLog.Append(_L(" Altitude Uncertainity : "));
		            positionLog.AppendNum(altUncert);
		            }
	            iTrace->Trace(positionLog, KTraceFileName, __LINE__);
	
	            COMASuplVelocity* velocity = NULL;
                logError = iOMASuplPosition->GetVelocity(velocity);
                if(!logError)			
    	            {
    	            iTrace->Trace(_L("Velocity Information"), KTraceFileName, __LINE__);
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
					
					            msg.Copy(_L("Bearing : "));
					            msg.AppendNum(bearing);
					            msg.Append(_L(" Horizontal Speed : "));
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
										
					            msg.Copy(_L("Bearing : "));
					            msg.AppendNum(bearing);
					            msg.Append(_L(" Horizontal Speed : "));
					            msg.AppendNum(horSpeed);
					            msg.Append(_L(" Vertical Direction : "));
					            msg.AppendNum(verDirect);
					            msg.Append(_L(" Vertical Speed : "));
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
										
					            msg.Copy(_L("Bearing : "));
					            msg.AppendNum(bearing);
					            msg.Append(_L(" Horizontal Speed : "));
					            msg.AppendNum(horSpeed);
					            msg.Append(_L(" Uncertainity Speed : "));
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
										
					            msg.Copy(_L("Bearing : "));
					            msg.AppendNum(bearing);
					            msg.Append(_L(" Horizontal Speed : "));
					            msg.AppendNum(horSpeed);
					            msg.Append(_L(" Vertical Direction : "));
					            msg.AppendNum(verDirect);
					            msg.Append(_L(" Vertical Speed : "));
					            msg.AppendNum(verSpeed);					
					            msg.Append(_L(" Horiz Uncertain Speed : "));
					            msg.AppendNum(horizUncertSpeed);
					            msg.Append(_L(" Vertical Uncertain Speed : "));
					            msg.AppendNum(vertUncertSpeed);					
					            iTrace->Trace(msg, KTraceFileName, __LINE__);																					
				                break;																
					            }
			                }
		                }
    	            }
                }
#endif            
		
	}

void COMASuplEndState::PrintHex(const TDesC8& 
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

COMASuplEnd::TOMASuplStatusCode COMASuplEndState::GetStatusCode()
    {
    return iStatusCode;
    }    
//  End of File




