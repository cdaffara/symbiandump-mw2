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

#include "epos_comasuplreport.h"
#include "lbs/epos_eomasuplposerrors.h"

#include "epos_comasupltrace.h"
#include "epos_comasuplreportstate.h"
#include "epos_omasuplconstants.h"

#include "epos_comasuplasnbase.h"

_LIT(KTraceFileName,"SUPL_OMA_SESSION::EPos_COMASuplReportState.cpp");

// ============================ MEMBER FUNCTIONS ===============================

// -----------------------------------------------------------------------------
// COMASuplReportState::COMASuplReportState
// C++ constructor can NOT contain any code, that might leave.
// -----------------------------------------------------------------------------
//
COMASuplReportState::COMASuplReportState(COMASuplAsnHandlerBase* aOMASuplAsnHandlerBase,                                                                              HBufC8* aEncodedSuplInit, HBufC8* aHSLPAddress):
                                         iOMASuplAsnHandlerBase(aOMASuplAsnHandlerBase),
                                         iEncodedSuplInit(aEncodedSuplInit),
                                         iHSLPAddress(aHSLPAddress),
                                         iReportDataPresent(EFalse)
    {
			iMessageTimeOut = 0; 
			iCurrentState = ESUPL_REPORT; 
    } 

// -----------------------------------------------------------------------------
// COMASuplReportState::COMASuplReportState
// C++ constructor can NOT contain any code, that might leave.
// -----------------------------------------------------------------------------
//
COMASuplReportState::COMASuplReportState(TInt aMsgTimeout,
                                         COMASuplAsnHandlerBase* aOMASuplAsnHandlerBase,                                                                              HBufC8* aEncodedSuplInit, HBufC8* aHSLPAddress):
                                         iOMASuplAsnHandlerBase(aOMASuplAsnHandlerBase),
                                         iEncodedSuplInit(aEncodedSuplInit),
                                         iHSLPAddress(aHSLPAddress),
                                         iReportDataPresent(EFalse)
    {
			iMessageTimeOut = aMsgTimeout * KSecond; 
			iCurrentState = ESUPL_REPORT; 
    } 
// -----------------------------------------------------------------------------
// COMASuplReportState::~COMASuplReportState
// -----------------------------------------------------------------------------
//
COMASuplReportState::~COMASuplReportState()
    {
    iReportDataList.Reset();
	iReportDataList.Close();            
    }

// -----------------------------------------------------------------------------
// COMASuplReportState::ConstructL
// Symbian 2nd phase constructor can leave.
// -----------------------------------------------------------------------------
//
void COMASuplReportState::ConstructL()
    {
    COMASuplState::BaseConstructL();
    iTrace->Trace(_L("COMASuplReportState::ConstructL..."), KTraceFileName, __LINE__); 							
    }

// -----------------------------------------------------------------------------
// COMASuplReportState::NewL
// Two-phased constructor.
// -----------------------------------------------------------------------------
//
COMASuplReportState* COMASuplReportState::NewL(COMASuplAsnHandlerBase* aOMASuplAsnHandlerBase,
                                               HBufC8* aEncodedSuplInit,
                                               HBufC8* aHSLPAddress)
    {
	    COMASuplReportState* self =  new ( ELeave ) COMASuplReportState(aOMASuplAsnHandlerBase, 
                        aEncodedSuplInit, aHSLPAddress);
	    CleanupStack::PushL( self );
	    self->ConstructL();
	    CleanupStack::Pop(self);
	    return self;
    }

// -----------------------------------------------------------------------------
// COMASuplReportState::NewL
// Two-phased constructor.
// -----------------------------------------------------------------------------
//
COMASuplReportState* COMASuplReportState::NewL(TInt aMsgTimeout, COMASuplAsnHandlerBase* aOMASuplAsnHandlerBase,
                                               HBufC8* aEncodedSuplInit,
                                               HBufC8* aHSLPAddress)
    {
	    COMASuplReportState* self =  new ( ELeave ) COMASuplReportState(aMsgTimeout, aOMASuplAsnHandlerBase, 
                        aEncodedSuplInit, aHSLPAddress);
	    CleanupStack::PushL( self );
	    self->ConstructL();
	    CleanupStack::Pop(self);
	    return self;
    }
// -----------------------------------------------------------------------------
// COMASuplReportState::GenerateMessage
// Generates SUPL_END message
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
TInt COMASuplReportState::GenerateMessageL() 
	{
		iTrace->Trace(_L("Start of COMASuplReportState::GenerateMessage"), KTraceFileName, __LINE__); 

		if(iEncodedSuplInit)
		{
			iVerPresent = ComputeSuplReportVerL();
			
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
// COMASuplReportState::EncodeMessageL
// Encodes message ...returns encoded data
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
HBufC8* COMASuplReportState::EncodeMessageL(TOMASuplVersion &aSuplVersion,COMASuplSessionID* aSessionId,
										 TInt &aError)
	{
    iTrace->Trace(_L("Start of COMASuplReportState::Encode_SUPL_TRIGGER_STOP"), KTraceFileName, __LINE__);
		
    COMASuplState::EncodeMessageL(aSuplVersion,aSessionId,aError);
		
    COMASuplAsnMessageBase* OMASuplMessageBase = iOMASuplAsnHandlerBase->CreateOMASuplMessageL(COMASuplAsnMessageBase::ESUPL_REPORT);
    CleanupStack::PushL(OMASuplMessageBase);
    COMASuplReport* OMASuplReport  =  static_cast<COMASuplReport*>(OMASuplMessageBase);
		
    iTrace->Trace(_L("**************** SUPL TRIGGER REPORT **********************"), KTraceFileName, __LINE__);
    iTrace->Trace(_L("Created COMASuplReport"), KTraceFileName, __LINE__); 							
		
    OMASuplReport->SetMessageBase(aSuplVersion,aSessionId); 
       
    // If report data present, add it
    if (iReportDataPresent)
        OMASuplReport->SetReportDataList(iReportDataList);

    if(iEncodedSuplInit)
        {
        OMASuplReport->SetVer(iRes);

        }

    iTrace->Trace(_L("Starting Encoding...SUPL_REPORT"), KTraceFileName, __LINE__);
    TInt error = KErrNone;
    HBufC8 *encodedBuffer = NULL;		
    TRAPD(err,encodedBuffer = iOMASuplAsnHandlerBase->EncodeL(OMASuplReport,error));
    CleanupStack::PopAndDestroy(OMASuplReport);
		
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
// COMASuplReportState::ProcessMessage
// process message...
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
TInt COMASuplReportState::ProcessMessageL(COMASuplAsnMessageBase* aDecodedMessage)
    {
    COMASuplReport* OMASuplReport = static_cast <COMASuplReport *>(aDecodedMessage);
    OMASuplReport->GetReportDataList(iReportDataList);
    return KErrNone;
    }

// -----------------------------------------------------------------------------
// COMASuplReportState::CancelOperation
// Cancels Operation...Cancels operation
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void COMASuplReportState::CancelOperation() 
	{ 
	COMASuplState::CancelOperation();	
	iTrace->Trace(_L("COMASuplReportState::CancelOperation..."), KTraceFileName, __LINE__);
	}	

// -----------------------------------------------------------------------------
// COMASuplReportState::CancelOperation
// Cancels Operation...Cancels operation
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void COMASuplReportState::AddToReportDataL(COMASuplReportData aReportData) 
	{ 
	iTrace->Trace(_L("COMASuplReportState::AddToReportData..."), KTraceFileName, __LINE__);
    COMASuplReportData *reportData = &aReportData;            
    iReportDataPresent = ETrue;
    User::LeaveIfError(iReportDataList.Append(reportData));
	}

// -----------------------------------------------------------------------------
// COMASuplReportState::ComputeSuplReportVerL
// Generates SUPL_POS_INIT message
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
TBool COMASuplReportState::ComputeSuplReportVerL()
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
// COMASuplReportState::PrintHex
// Prints hex packet 
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void COMASuplReportState::PrintHex(const TDesC8& 
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

// -----------------------------------------------------------------------------
// COMASuplReportState::GetPosition
// Retrive Position
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
COMASuplPosition* COMASuplReportState::GetPositionL() const
{
    if (iReportDataList.Count() > 0)
        {        
        COMASuplPositionData* posData = COMASuplPositionData::NewL();
        COMASuplReportData* reportData = NULL;
        reportData = static_cast<COMASuplReportData*>(iReportDataList[0]);
        reportData->GetPosData(posData);
        COMASuplPosition* position;
        posData->GetPosition(position);
        return position;
        }
    else
        return NULL;        
}

//  End of File




