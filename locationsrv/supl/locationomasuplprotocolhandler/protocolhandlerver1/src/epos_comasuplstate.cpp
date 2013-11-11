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
* Description:   Implementation of class COMASUPLState
*
*/



// INCLUDE FILES

#include <utf.h>

#include "epos_comasuplsessionid.h"

#include "epos_comasuplstate.h"
#include "epos_comasupltimeouttimer.h"
#include "epos_comasupltrace.h"

_LIT(KTraceFileName,"SUPL_OMA_SESSION::EPos_COMASuplState.cpp");

#if defined(_DEBUG)

	_LIT( KPanicMsg, "COMASUPLState");

	enum TOMASuplStatePanicCode
	    {
	    	EInvalidStateObserver = -1001,
	    	EInvalidMsgProcess =  -1002
	    	
	    };

	void Panic( TOMASuplStatePanicCode aReason )
	    {
	    	User::Panic( KPanicMsg, aReason );
	    }
    
#endif


// ============================ MEMBER FUNCTIONS ===============================
//Default constructor
COMASuplState::COMASuplState()
	{
		iMessageTimeOut = 0;
		iCurrentState = ESUPL_NONE;
	}
	
//Destructor
COMASuplState::~COMASuplState()
	{
		CancelOperation(); //Manish
		delete iTimer;
		delete iTrace;
	}

// -----------------------------------------------------------------------------
// COMASuplState::GenerateMessage
// Generates message...which message to be generate will be depend on derived class implementation
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
TInt COMASuplState::GenerateMessageL() 
	{ 
		//Nothing to do here 
		iTrace->Trace(_L("COMASuplState::GenerateMessage...No use"), KTraceFileName, __LINE__); 							
		return KErrNotSupported;
	}
	
// -----------------------------------------------------------------------------
// COMASuplState::CancelOperation
// Cancels Operation...which will be depend on derived class implementation
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void COMASuplState::CancelOperation() 
	{ 
		 StopTimer();
		 //Debug mode
		 TBuf<64> msg(_L("COMASuplState::CancelOperation..."));
		 switch(iCurrentState)
		 	{
		 		case ESUPL_NONE:
		 			msg.Append(_L("ESUPL_NONE"));
		 			break;
		 		case ESUPL_START:	
					msg.Append(_L("ESUPL_START"));
					break;
		 		case ESUPL_RESPONSE:
					msg.Append(_L("ESUPL_RESPONSE"));
					break;
		 		case ESUPL_POS:
					msg.Append(_L("ESUPL_POS"));
					break;
		 		case ESUPL_POS_INIT:
					msg.Append(_L("ESUPL_POS_INIT"));
					break;
				case ESUPL_INIT:
					msg.Append(_L("ESUPL_INIT"));
					break;
				case ESUPL_END:
					msg.Append(_L("ESUPL_END"));
					break;
				default:
					msg.Append(_L("WRONG MESSAGE"));
		 	}
		 if(iTrace)
		 iTrace->Trace(msg, KTraceFileName, __LINE__); 							
	}	
	
// -----------------------------------------------------------------------------
// COMASuplState::SetMsgStateObserver
// Sets the message state observer
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void COMASuplState::SetMsgStateObserver(MOMASuplMsgStateObserver* aMsgStateObserver)
	{
		__ASSERT_DEBUG(aMsgStateObserver, Panic(EInvalidStateObserver) );
		iMsgStateObserver = aMsgStateObserver;
	}

// -----------------------------------------------------------------------------
// COMASuplState::EncodeMessageL
// Encodes message...which message to be encode will be depend on derived class implementation
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
HBufC8* COMASuplState::EncodeMessageL(TOMASuplVersion& 
#if defined(PRINT_MESSAGE)
aSuplVersion
#endif
,
COMASuplSessionID* 
#if defined(PRINT_MESSAGE)
aSessionId
#endif
,
TInt &aError) 
	{
#ifdef PRINT_MESSAGE		//This is only for logging purpose. 
			TInt major,minor,ServInd;	
			TBuf<128> msg(_L("SUPL Version is : Major :")); 
			TBuf<4>  ver; 			
			
			aSuplVersion.SuplVersion(major,minor,ServInd);
			
			ver.AppendNum(major);
			msg.Append(ver);
			
			msg.Append(_L("  Minor : "));
			ver.Zero();
			ver.AppendNum(minor);
			msg.Append(ver);

			msg.Append(_L("  ServInd : "));
			ver.Zero();
			ver.AppendNum(ServInd);
			msg.Append(ver);
			
			iTrace->Trace(msg,KTraceFileName, __LINE__); 							
			
			COMASuplSETSessionID* suplSETID = NULL;
			aSessionId->SETSessionID(suplSETID);
			
			COMASuplSLPSessionID* suplSLPID = NULL;
			aSessionId->SLPSessionID(suplSLPID);
			
			//SET Part			
			if(suplSETID)
				{
					msg.Copy(_L("SET Session ID : "));
					TBuf<4> id;	
					TInt    SETSessionID;
					TBuf8<64> IPvAddress8;
					TBuf<64>  IPvAddress;
					
					suplSETID->SuplSETSessionID(SETSessionID,IPvAddress8);	
					COMASuplSETSessionID::TSuplSETIDType setIDType = suplSETID->SuplSETIDType();
					if(setIDType == COMASuplSETSessionID::EIMSI )
						{
						iTrace->Trace(_L("SET ID is of type IMSI"),KTraceFileName, __LINE__);
						iTrace->Trace(_L("IMSI Value :"),KTraceFileName, __LINE__);
						msg.Copy(IPvAddress8);
						}
					else
						{
					TLex8 lex(IPvAddress8);
				
					TChar chr;	
					
					chr = lex.Get();
					TUint i = chr;
					IPvAddress.AppendNum(i);
					IPvAddress.Append(_L("."));
					
					chr = lex.Get();
					i = chr;
					IPvAddress.AppendNum(i);
					IPvAddress.Append(_L("."));

					chr = lex.Get();
					i = chr;
					IPvAddress.AppendNum(i);
					IPvAddress.Append(_L("."));

					chr = lex.Get();
					i = chr;
					IPvAddress.AppendNum(i);
					
					id.AppendNum(SETSessionID);
					msg.Append(id);
					msg.Append(_L("  IPAddress :"));
					msg.Append(IPvAddress);
						}				
				}
			else
				{
					msg.Copy(_L("SET Session Id is not set."));
				}
				
			//SLP Part			
			if(suplSLPID)
				{
					msg.Copy(_L("SLP Session ID : "));
					TBuf8<4> sessionId8;
					TBuf<32> sessionId;
					TBuf8<64> ipAddress8(_L8("No Address field in SLP Session ID"));				
					TBuf<64> ipAddress;

					COMASuplSLPAddress* slpIPAddress = NULL;
					suplSLPID->SuplSLPSessionID(sessionId8,slpIPAddress);
					
					if(slpIPAddress)
						{
							COMASuplSLPAddress::TSuplSLPAddressType slpAddType = slpIPAddress->SLPAddressType();
							if(slpAddType == COMASuplSLPAddress::EFqdn)
								{
								HBufC8* fqdn = NULL;
								slpIPAddress->Fqdn(fqdn);
								iTrace->Trace(_L("SLP Address is FQDN"),KTraceFileName, __LINE__); 
								iTrace->Trace(_L("FQDN Value"),KTraceFileName, __LINE__); 
								if(fqdn)
								{	
									HBufC* fqdnBuffer = NULL; 
									fqdnBuffer = HBufC::NewL(fqdn->Length());
									fqdnBuffer->Des().Copy(fqdn->Des());
									iTrace->Trace(fqdnBuffer->Des(),KTraceFileName, __LINE__);
									delete fqdnBuffer;
									fqdnBuffer = NULL; 
								}
									
								aError = KErrNotSupported;
								return NULL;
								}
							else
							{
							TInt err = slpIPAddress->IPvAddress(ipAddress8);	
							if(err != KErrNone)
								{
									msg.Copy(_L("SLP Address is not set."));
									iTrace->Trace(msg,KTraceFileName, __LINE__); 								
									return NULL;
								}	
							}							
						}
					
					TLex8 lex(ipAddress8);
					TChar chr;	
					
					chr = lex.Get();
					TUint i = chr;
					ipAddress.AppendNum(i);
					ipAddress.Append(_L("."));
					
					chr = lex.Get();
					i = chr;
					ipAddress.AppendNum(i);
					ipAddress.Append(_L("."));

					chr = lex.Get();
					i = chr;
					ipAddress.AppendNum(i);
					ipAddress.Append(_L("."));

					chr = lex.Get();
					i = chr;
					ipAddress.AppendNum(i);
					
					TLex8 lex1(sessionId8);
					for(TInt i = 0; i < 4; i++)	
						{
							chr = lex1.Get();
							TUint num = chr;
							sessionId.AppendNum(num);
						}
						
					msg.Append(sessionId);
					msg.Append(_L("  IPAddress : "));
					msg.Append(ipAddress);
					iTrace->Trace(msg,KTraceFileName, __LINE__); 								
				}
			else
				{
					msg.Copy(_L("SLP Session Id is not set."));
					iTrace->Trace(msg,KTraceFileName, __LINE__); 								
				}
		#endif
		
		aError = KErrNotSupported;
		return NULL;
	}
	
// -----------------------------------------------------------------------------
// COMASuplState::EncodeMessageL
// Encodes message...which message to be encode will be depend on derived class implementation
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//	
TInt COMASuplState::ProcessMessageL(COMASuplAsnMessageBase* /*aDecodedMessage*/) 
	{
		iTrace->Trace(_L("COMASuplState::ProcessMessage...No use"), KTraceFileName, __LINE__); 							
		return KErrNotSupported;
	}
	
// -----------------------------------------------------------------------------
// COMASuplState::BaseConstructL
// BaseConstructL.... initilization for some of the member var takes place
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void COMASuplState::BaseConstructL()
	{
		iTrace = COMASuplTrace::NewL();
		iTrace->Trace(_L("COMASuplState::BaseConstructL..."), KTraceFileName, __LINE__); 							
	    iTimer = COMASuplTimeoutTimer::NewL(*this);
	}

// -----------------------------------------------------------------------------
// COMASuplState::GetSuplState
// GetSuplState.... returns type of message
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
COMASuplState::TSuplMessageType COMASuplState::GetSuplState()
	{
		return iCurrentState;
	}

// -----------------------------------------------------------------------------
// COMASuplState::StartTimer
// StartTimer...starts the timer ... depending on type of message
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void COMASuplState::StartTimer()
	{
		iTrace->Trace(_L("COMASuplState::StartTimer..."), KTraceFileName, __LINE__); 							
		iTimer->StartTimer(iMessageTimeOut); 	
	}

// -----------------------------------------------------------------------------
// COMASuplState::StopTimer
// StopTimer...stops the timer .
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void COMASuplState::StopTimer()
	{
		iTimer->StopTimer();	 		
		if(iTrace)	
		    iTrace->Trace(_L("COMASuplState::StopTimer..."), KTraceFileName, __LINE__); 							
	}
	
// -----------------------------------------------------------------------------
// COMASuplState::TimerExpired
// TimerExpired...Gets called when time out occurs .
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void COMASuplState::TimerExpiredL()
	{
		iTrace->Trace(_L("COMASuplState::TimerExpired..."), KTraceFileName, __LINE__); 							
		if(iMsgStateObserver)  
			iMsgStateObserver->OperationCompleteL(KErrTimedOut);
	}
	
void COMASuplState::SetVersion(TOMASuplVersion& aOMASuplVersion)
	{
		iOMASuplVersion = aOMASuplVersion;
	}



