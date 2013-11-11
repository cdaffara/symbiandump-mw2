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
* Description:   SUPL Connection provides the Connection End point
* 				 for SUPL Session to send/receive packets from the
*				 network
*
*/



// INCLUDE FILES
#include 	"epos_msuplpacketnotifier.h"
#include    "epos_csuplsocketreader.h"

#include "epos_csuplconntrace.h"
// Constant Declaration
_LIT(KTraceFileName,"SUPL_PH_API::epos_csuplsocketreader.cpp");

// ============================ MEMBER FUNCTIONS ===============================

// -----------------------------------------------------------------------------
// CSuplConnection::CSuplSocketReader
// This is the method to handle request completion for
// asynchronous requests made by CSuplConnection
// (other items were commented in a header).
// -----------------------------------------------------------------------------
// 
CSuplSocketReader::CSuplSocketReader(CSecureSocket *aSocket,
									 MSuplPacketNotifier *aNotifier,
									 TBool aVariantEnabled):
	CActive(EPriorityStandard), 
	iSecureSocket(aSocket),
	iSocket(iDummySocket),
	iReadNotifier(aNotifier),
	iVariantEnabled(aVariantEnabled)
	{
    CActiveScheduler::Add(this);
    }
    
// -----------------------------------------------------------------------------
// CSuplConnection::CSuplSocketReader
// This is the method to handle request completion for
// asynchronous requests made by CSuplConnection
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//    
CSuplSocketReader::CSuplSocketReader(RSocket& aSocket,
									 MSuplPacketNotifier *aNotifier,
									 TBool aVariantEnabled):
	CActive(EPriorityStandard), 
	iSocket(aSocket),
	iReadNotifier(aNotifier),
	iVariantEnabled(aVariantEnabled)
	{
    CActiveScheduler::Add(this);
    }

// -----------------------------------------------------------------------------
// CSuplConnection::ConstructL
// This is the method to handle request completion for
// asynchronous requests made by CSuplConnection
// (other items were commented in a header).
// -----------------------------------------------------------------------------
// 
void CSuplSocketReader::ConstructL()
    {
    iTrace = CSuplConnTrace::NewL();
    User::LeaveIfError(iReadBuffer.Create(KSocketDefaultBufferSize));
    User::LeaveIfError(iAdditionalReadBuffer.Create(0));
    }

// -----------------------------------------------------------------------------
// CSuplConnection::NewL
// This is the method to handle request completion for
// asynchronous requests made by CSuplConnection
// (other items were commented in a header).
// -----------------------------------------------------------------------------
// 
CSuplSocketReader* CSuplSocketReader::NewL(CSecureSocket *aSocket, 
											MSuplPacketNotifier *aNotifier,
											TBool aVariantEnabled)
    {
    CSuplSocketReader* self = new( ELeave ) CSuplSocketReader(aSocket, aNotifier, aVariantEnabled);
    
    CleanupStack::PushL( self );
    self->ConstructL();
    CleanupStack::Pop();

    return self;
    }
    
// -----------------------------------------------------------------------------
// CSuplConnection::NewL
// This is the method to handle request completion for
// (other items were commented in a header).
// -----------------------------------------------------------------------------
// 
CSuplSocketReader* CSuplSocketReader::NewL(RSocket& aSocket,
										   MSuplPacketNotifier *aNotifier,
										   TBool aVariantEnabled)
    {
    CSuplSocketReader* self = new( ELeave ) CSuplSocketReader(aSocket, aNotifier, aVariantEnabled);
    
    CleanupStack::PushL( self );
    self->ConstructL();
    CleanupStack::Pop();

    return self;
    }

// -----------------------------------------------------------------------------
// CSuplConnection::~CSuplSocketReader
// This is the method to handle request completion for
// asynchronous requests made by CSuplConnection
// (other items were commented in a header).
// -----------------------------------------------------------------------------
// 
CSuplSocketReader::~CSuplSocketReader()
    {
    	//Cancel any outstanding request
    	iReadBuffer.Close();
    	iAdditionalReadBuffer.Close();
    	Cancel();
    	delete iTrace;
    }

// -----------------------------------------------------------------------------
// CSuplConnection::RunL
// This is the method to handle request completion for
// asynchronous requests made by CSuplConnection
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void CSuplSocketReader::RunL()
    {
    	// Log
    	iTrace->Trace(_L("CSuplSocketReader::RunL"), KTraceFileName, __LINE__);
    
    	switch(iStatus.Int())
    	{
    		case KErrNone:
    		// Read Successful
    		if(iReadNotifier)
    		{
    			// Log
    			iTrace->Trace(_L("CSuplSocketReader::RunL Read Successful"), KTraceFileName, __LINE__);
    			TInt msgSize = 0;
    			TInt error;
				if(iVariantEnabled)
					{
					iTrace->Trace(_L("CSuplSocketReader::Secure Read Successful"), KTraceFileName, __LINE__);
	    			error = iSecureSocket->GetOpt(KSOReadBytesPending, KSOLSocket, msgSize);
					}
				else
					{
					iTrace->Trace(_L("CSuplSocketReader::Not Secure Read Successful"), KTraceFileName, __LINE__);
	    			error = iSocket.GetOpt(KSOReadBytesPending, KSOLSocket, msgSize);
					}
				if(KErrNone == error)
    				{
    					if(msgSize > 0)
    						{
								if(iReRead)
								{
									RBuf OriginalBuf1;
									User::LeaveIfError(OriginalBuf1.Create(KSocketDefaultBufferSize));
									OriginalBuf1.Copy(iReadBuffer);
									TBuf<64> msg2;
									msg2.Copy(_L("Length of OriginalBuf1 is: "));
									msg2.AppendNum(OriginalBuf1.Length());
									iTrace->Trace(msg2, KTraceFileName, __LINE__);
									User::LeaveIfError(iReadBuffer.ReAlloc(KSocketDefaultBufferSize + iAdditionalReadBuffer.Length()));
									iReadBuffer.Zero();
									iReadBuffer.Append(OriginalBuf1);
									iReadBuffer.Append(iAdditionalReadBuffer);

									iReRead = EFalse;
									OriginalBuf1.Close();					
								}
    							iAdditionalReadBuffer.Zero();
    							User::LeaveIfError(iAdditionalReadBuffer.ReAlloc(msgSize));		
    							iReRead = ETrue;
    						}
    					else
    						{
    							if(iReRead)
    								{
    									RBuf OriginalBuf;
    									User::LeaveIfError(OriginalBuf.Create(KSocketDefaultBufferSize));
    									OriginalBuf.Copy(iReadBuffer);
    									
    									User::LeaveIfError(iReadBuffer.ReAlloc(KSocketDefaultBufferSize + iAdditionalReadBuffer.Length()));
    									iReadBuffer.Zero();
    									iReadBuffer.Append(OriginalBuf);
    									iReadBuffer.Append(iAdditionalReadBuffer);
    									
    									iReRead = EFalse;
    									OriginalBuf.Close();					
    								}
                                TBuf<64> msg(_L("Forwarding data recvd on port "));
                                msg.AppendNum(iPortNum);
	                            iTrace->Trace(msg, KTraceFileName, __LINE__);		
    							iReadNotifier->HandlePacket(iReadBuffer, iPortNum);
    						}		
    					//iReadNotifier->HandlePacket(iReadBuffer);
    				}
    			
    			// Re-issue Read   e32des8.h  e32des8.h 
    			IssueRead();
    		}
    		break;
    		
    		default:
    		if(iReadNotifier)
    		{
    			// Log
    			iTrace->Trace(_L("CSuplSocketReader::RunL Read Failure"), KTraceFileName, __LINE__);
    
    			iReadNotifier->HandlePacketError(iStatus.Int());
    			iReadBuffer.Zero();
    		}
    		break;
    		
    	}
    }
    
// -----------------------------------------------------------------------------
// CSuplConnection::DoCancel
// This is the method to cancel the request
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void CSuplSocketReader::DoCancel()
    {
    	// Log
    	if(iVariantEnabled)
			{
			iTrace->Trace(_L("CSuplSocketReader::Secure Read DoCancel"), KTraceFileName, __LINE__);
	    	iSecureSocket->CancelRecv();
			}
		else
			{	
			iTrace->Trace(_L("CSuplSocketReader::UnSecure ReadDoCancel"), KTraceFileName, __LINE__);
	    	iSocket.CancelRecv();
			}
    }


// -----------------------------------------------------------------------------
// CSuplSocketReader::IssueRead
// Start reading
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void CSuplSocketReader::IssueRead()
    {
    	// Log
    	iTrace->Trace(_L("CSuplSocketReader::IssueRead"), KTraceFileName, __LINE__);
    	if(iReRead)
    		{
    			iAdditionalReadBuffer.Zero();
				if(iVariantEnabled)
					{
					iTrace->Trace(_L("CSuplSocketReader::Secure IssueRead if iReRead"), KTraceFileName, __LINE__);
	    			iSecureSocket->RecvOneOrMore(iAdditionalReadBuffer, iStatus, iMsgLength);
					}
				else
					{
					iTrace->Trace(_L("CSuplSocketReader::NonSecure IssueRead if iReRead"), KTraceFileName, __LINE__);
	    			iSocket.RecvOneOrMore(iAdditionalReadBuffer, 0, iStatus, iMsgLength);
					}

    		}
    	else
    		{
    			iReadBuffer.Zero();
				if(iVariantEnabled)
					{
			    	iTrace->Trace(_L("CSuplSocketReader::Secure IssueRead if not iReRead"), KTraceFileName, __LINE__);
			    	iSecureSocket->RecvOneOrMore(iReadBuffer, iStatus, iMsgLength);		
					}
				else
					{
					iTrace->Trace(_L("CSuplSocketReader::NonSecure IssueRead if not iReRead"), KTraceFileName, __LINE__);
			    	iSocket.RecvOneOrMore(iReadBuffer, 0, iStatus, iMsgLength);
					}
    		}    	
    	SetActive();
	}

void CSuplSocketReader::SetPortNum(TUint aPortNum)
    {
    iPortNum = aPortNum;
    }        
	
//  End of File  
