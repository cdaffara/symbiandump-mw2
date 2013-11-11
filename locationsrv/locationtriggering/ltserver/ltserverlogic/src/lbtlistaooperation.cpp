/*
* Copyright (c) 2007 Nokia Corporation and/or its subsidiary(-ies).
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
* Description:  Active Object operation for serving server logic requests
*				 for listing of triggers.
*
*/



// INCLUDE FILES
#include <e32base.h>
#include <s32mem.h>
#include "lbtlistaooperation.h"
#include "lbtcontainer.h"
#include "lbtaoobserver.h"
#include "lbtglobal.h"
#include "lbtserverconsts.h"
#include "lbtcontainertriggerfilter.h"
#include "lbtcontainertriggerentry.h"
#include "lbtlisttriggeroptions.h"
#include "lbtserverlogicbase.h"
#include "lbttriggerfilterbyattribute.h"
#include "lbtlogger.h"

// ===================== MEMBER FUNCTIONS =====================

// ---------------------------------------------------------------------------
// CLbtListAOOperation::NewL
// Symbian Two - phase constructor
// ---------------------------------------------------------------------------
//
CLbtListAOOperation* CLbtListAOOperation::NewL(MLbtAOObserver& aObserver,
											   const RMessage2& aMessage,
											   CLbtContainer& aContainer,
											   TInt aClientType )
	{
	FUNC_ENTER("CLbtListAOOperation::NewL");
	CLbtListAOOperation* self = new (ELeave) CLbtListAOOperation(aObserver, aMessage, aContainer, aClientType);
	CleanupStack::PushL(self);
	self->ConstructL();
	CleanupStack::Pop(); // self
	return self;
	}

// ---------------------------------------------------------------------------
// CLbtListAOOperation::~CLbtListAOOperation
// Destructor
// ---------------------------------------------------------------------------
//
CLbtListAOOperation::~CLbtListAOOperation()
	{
	FUNC_ENTER("CLbtListAOOperation::~CLbtListAOOperation");
	// If the operation is not complete yet then complete the 
	// message with server busy
	if(IsActive())
		{
		Cancel();
		}
	iTriggerArray.ResetAndDestroy();
	delete iTriggerBuffer;
	}

// ---------------------------------------------------------------------------
// CLbtListAOOperation::CLbtListAOOperation
// C++ Default constructor
// ---------------------------------------------------------------------------
//
CLbtListAOOperation::CLbtListAOOperation(MLbtAOObserver& aObserver,
										 const RMessage2& aMessage,
										 CLbtContainer& aContainer,
										 TInt aClientType)
	: CLbtAOOperationBase(aObserver, aMessage, aContainer, aClientType)
	{
	FUNC_ENTER("CLbtListAOOperation::CLbtListAOOperation");
	}

// ---------------------------------------------------------------------------
// CLbtListAOOperation::ConstructL
// ---------------------------------------------------------------------------
//
void CLbtListAOOperation::ConstructL()
	{
	FUNC_ENTER("CLbtListAOOperation::ConstructL");
	CActiveScheduler::Add(this);
	iState = EStateNone;
	}

TInt CLbtListAOOperation::RunError(TInt aError)
	{
	FUNC_ENTER("CLbtListAOOperation::RunError");
	ERROR("RunL Error is %d", aError);
	return aError;
	}
	
// ---------------------------------------------------------------------------
// CLbtListAOOperation::RunL
// ---------------------------------------------------------------------------
//
void CLbtListAOOperation::RunL()
	{
	FUNC_ENTER("CLbtListAOOperation::RunL");
	// Remove this active object from the schedule queue since 
	// async requests will not be made anymore
	Deque();

	if( iStatus.Int() == KErrCancel )
		{
		LbtGlobal::RequestComplete(iMessage, KErrCancel);
		iObserver.HandleOperationClosureL(this,iStatus.Int());		
		return;
		}
	
	switch( iMessage.Function() )
		{
		case EGetFiredTriggersCount:
		case ELbtGetFiredTriggers:
		    {
		    if( iState == EStateUpdateFiredState )
		    	{
		    	iState = EStateUpdateCompleted;
		    	return;
		    	}
		    else if( iState == EStateServiceMessageReceived )
		        {
		        ServiceGetFiredTriggersL();
		        return;
		        }
		    // Got the result from container. Return the number of triggers
			TPckg<TInt> numberOfTriggers(iTriggerArray.Count());
			// Send the number of triggers to client lib
			TInt retVal = LbtGlobal::Write(iMessage, KTriggersSize, numberOfTriggers);			
			if(retVal == KErrNone)
				{
				// Complete the message
				LbtGlobal::RequestComplete(iMessage, iStatus.Int());
				}
				
			if(iTriggerArray.Count() == 0)
				{
				// Inform the observer that this operation has completed
				iObserver.HandleOperationClosureL(this, retVal);
				return;
				}
			RArray<TLbtTriggerId>  triggerIdArray;
		    TLbtTriggerId triggerId;
		    for( TInt i=0;i<iTriggerArray.Count();i++ )
		        {
		        // This array of trigger ids is passed to container to set the 
				// fired state to false.	
				triggerId = iTriggerArray[i]->TriggerEntry()->Id();
			    triggerIdArray.AppendL( triggerId );	
		        }
		        
	        // This active object was dequed from active scheduler once container completes 
			// the list triggers.To use this again,it is necessary to add this to the 
			// active scheduler.
			CActiveScheduler::Add( this );
			
			// Set the trigger fired flag to false
			iContainer.UpdateTriggerFiredState( triggerIdArray,iAOIdentificationNum,EFalse,iStatus );
		    iState = EStateUpdateFiredState;
		    SetActive();
		    triggerIdArray.Close();	
		    break;
		    }
		case ELbtListTriggerIds:
		case ELbtGetListTriggerIdsSize:
			{
			// Got the result from container. Return the number of triggers
			TPckg<TInt> numberOfTriggers(iTriggerArray.Count());
			// Send the number of triggers to client lib
			TInt retVal = LbtGlobal::Write(iMessage, KTriggersSize, numberOfTriggers);			
			if(retVal == KErrNone)
				{
				// Complete the message
				LbtGlobal::RequestComplete(iMessage, iStatus.Int());
				}
				
			if(iTriggerArray.Count() == 0)
				{
				// Inform the observer that this operation has completed
				iObserver.HandleOperationClosureL(this, retVal);
				}
			break;
			}
		case ELbtGetTriggersBufferSize: 
			{
			// If there are no triggers found for this client then return zero
			// as the buffer size and complete this operation object
			if(iTriggerArray.Count() == 0)
				{
				TPckg<TInt> triggersSize(0);
				TInt retVal = LbtGlobal::Write(iMessage, KTriggersSize, triggersSize);
				if(retVal == KErrNone)
					{
					// Complete the message
					LbtGlobal::RequestComplete(iMessage, iStatus.Int());
					}
					
				// Inform the observer that this operation has completed
				iObserver.HandleOperationClosureL(this, retVal);
				break;
				}
			
			// If there are any triggers present then populate the buffer to get the size
			if(!iTriggerBuffer)
				{
				iTriggerBuffer = CBufFlat::NewL(512);
				}
			RBufWriteStream writeStream;
			writeStream.Open(*iTriggerBuffer);
			CleanupClosePushL(writeStream);
			writeStream.WriteInt32L(iTriggerArray.Count());
			for(TInt i=0;i<iTriggerArray.Count(); ++i)
				{
				writeStream << *(iTriggerArray[i]);				
				}
			writeStream.CommitL();
			CleanupStack::PopAndDestroy(&writeStream);
			// Write the size of the buffer
			TPckg<TInt> triggersSize(iTriggerBuffer->Size());
			TInt retVal = LbtGlobal::Write(iMessage, KTriggersSize, triggersSize);
			if(retVal == KErrNone)
				{
				// Complete the message
				LbtGlobal::RequestComplete(iMessage, iStatus.Int());
				}
			break;
			}
		default:
			{
			break;
			}
		}
	}

// ---------------------------------------------------------------------------
// CLbtListAOOperation::DoCancel
// ---------------------------------------------------------------------------
//
void CLbtListAOOperation::DoCancel()
	{
	FUNC_ENTER("CLbtListAOOperation::DoCancel");
	// Cancel container notifications
	iContainer.CancelAsyncOperation(iAOIdentificationNum);
	iMessage.Complete( iStatus.Int() );
    }

// ---------------------------------------------------------------------------
// CLbtListAOOperation::StartOperationL
// ---------------------------------------------------------------------------
//
void CLbtListAOOperation::StartOperationL()
	{
	FUNC_ENTER("CLbtListAOOperation::StartOperationL");
	switch(iFunction)
		{
		case ELbtGetTriggersBufferSize: // Get the buffer size needed to store the trigger entry objects
		case EGetFiredTriggersCount: // Get number of fired triggers for the client
		case ELbtGetListTriggerIdsSize: // Get number of triggers owned by this client
			{
			CLbtListTriggerOptions* options = CLbtListTriggerOptions::NewL();
			CleanupStack::PushL(options);
			
			CLbtContainerFilter* containerFilter = CLbtContainerFilter::NewL();
			CleanupStack::PushL(containerFilter);
			
	    	TLbtSecurityPolicy securityPolicy;
			if ( iClientType == CLbtServerLogicBase::TLbtClientLibrary )
			    {
    			securityPolicy.SetOwnerSecureId( iMessage.SecureId() );	
			    }

			if(iFunction == ELbtGetListTriggerIdsSize || 
			   iFunction == ELbtGetTriggersBufferSize )
				{
				HBufC8* listOptionsBuffer = LbtGlobal::CopyClientBuffer8LC(iMessage, KParamFilter);
				
				if( listOptionsBuffer == NULL )
					{
					CleanupStack::PopAndDestroy(2);
					iObserver.HandleOperationClosureL(this, KErrBadDescriptor);
					return;
					}
				
				// Read the buffer into a stream
				RDesReadStream stream(*listOptionsBuffer);
				CleanupClosePushL(stream);
				options->InternalizeL(stream);				
				CleanupStack::PopAndDestroy(2, listOptionsBuffer); // listOptionsBuffer, stream
				}
			else if( iFunction == EGetFiredTriggersCount )
				{
				containerFilter->AddFiredInfoInFilterL(ETrue);
				}
			
			CLbtContainerListOptions* containerOptions = CLbtContainerListOptions::NewL(options, containerFilter);
			CleanupStack::Pop(2); // options, filter

            TLbtTriggerAttributeFieldsMask triggerMask;
            TLbtTriggerDynamicInfoFieldsMask dynInfoMask;

            // Add the Trigger Id attribute since it shouls always be retrieved whether the
            // client specifies it or not.
			containerOptions->ListOptions()->GetRetrievedFields( triggerMask, dynInfoMask );
			triggerMask = triggerMask | CLbtTriggerEntry::EAttributeId;
			containerOptions->ListOptions()->SetRetrievedFields( triggerMask, dynInfoMask );
			
			containerOptions->SetDataMask(CLbtContainerTriggerEntry::EContainerAttributeFireInfo);
			// Request triggers of the client
			iContainer.ListTriggers(containerOptions, iTriggerArray,iAOIdentificationNum,iStatus,securityPolicy);
			SetActive();
			break;
			}
		default:
			{
			break;
			}
		}
	}

// ---------------------------------------------------------------------------
// CLbtListAOOperation::ServiceMessageL
// ---------------------------------------------------------------------------
//
void CLbtListAOOperation::ServiceMessageL(const RMessage2 aMessage)
	{
	FUNC_ENTER("CLbtListAOOperation::ServiceMessageL");
	iMessage=aMessage;
	switch(aMessage.Function())
		{
		case ELbtGetFiredTriggers:
			{
			iMessage = aMessage;
			if( iState == EStateUpdateCompleted )
				{
				ServiceGetFiredTriggersL();
				}
			else
				{
				iState = EStateServiceMessageReceived;
				}
		    break;
			}
		case ELbtListTriggerIds: // Send the acquired triggers id's
			{
			CBufFlat* buffer = CBufFlat::NewL(512);
			CleanupStack::PushL(buffer);
			RBufWriteStream writeStream;
			writeStream.Open(*buffer);
			CleanupClosePushL(writeStream);			
			for(TInt i=0;i<iTriggerArray.Count(); ++i)
				{
				TLbtTriggerId triggerId = iTriggerArray[i]->TriggerEntry()->Id();
				TUint8* ptr = reinterpret_cast<TUint8*>(&triggerId);
				writeStream.WriteL(ptr, sizeof(TLbtTriggerId));				
				}
			writeStream.CommitL();			
			CleanupStack::PopAndDestroy(&writeStream);
			TPtr8 ptr = buffer->Ptr(0);
			
			// Write the trigger array into the IPC
			TInt retVal = LbtGlobal::Write(iMessage, KParamTriggerEntry, ptr);			
			if(retVal == KErrNone)
				{
				// Complete the message
				LbtGlobal::RequestComplete(iMessage, KErrNone);
				}
			CleanupStack::PopAndDestroy(buffer);
			// Inform the observer that this operation has completed
	        iObserver.HandleOperationClosureL(this, KErrNone);
	        break;
			}
		case ELbtGetTriggers:
			{
			TInt error = KErrNone;
			if( iTriggerBuffer )
				{
				TPtr8 ptr = iTriggerBuffer->Ptr(0);
				// Write the trigger array into the IPC
				TInt retVal = LbtGlobal::Write(iMessage, KParamTriggerEntry, ptr);			
				if(retVal == KErrNone)
					{
					// Complete the message
					LbtGlobal::RequestComplete(iMessage, KErrNone);
					}
				delete iTriggerBuffer;
				iTriggerBuffer = NULL;
				}
			else
				{
				error = KErrNotFound;
				LbtGlobal::RequestComplete(iMessage, KErrNotFound);
				}
			// Inform the observer that this operation has completed
	        iObserver.HandleOperationClosureL( this, error );
			break;
			}
		default:
			{
			// Not received the first part of the message. This 
			// should not happen and should be made sure by the client
			// library
			LbtGlobal::RequestComplete(iMessage, KErrUnknown);
			// Inform the observer that this operation has completed
	        iObserver.HandleOperationClosureL(this, KErrNone);
			break;
			}
		}
    }
	
// ---------------------------------------------------------------------------
// CLbtListAOOperation::CancelListOperation
// ---------------------------------------------------------------------------
//
void CLbtListAOOperation::CancelListOperation()
	{
	FUNC_ENTER("CLbtListAOOperation::CancelListOperation");
	if(iStatus.Int()==KRequestPending)
		{
		Cancel();
		}
	else
		{
		// HandleOperationClosureL method internally calls LoadOrUnloadStrategyPluginL 
		// method which may leave only during the loading operation,but here the expected 
		// behaviour is the unloading of strategy plugin which never fails .Hence we have ignored.
        LbtGlobal::RequestComplete(iMessage,KErrCancel);   
		TRAP_IGNORE( iObserver.HandleOperationClosureL(this,KErrCancel) );
		}	
	}

// ---------------------------------------------------------------------------
// CLbtListAOOperation::ServiceGetFiredTriggersL
// ---------------------------------------------------------------------------
//
void CLbtListAOOperation::ServiceGetFiredTriggersL()
    {
    FUNC_ENTER("CLbtListAOOperation::ServiceGetFiredTriggers");
    //TInt sizeOfFireInfo = sizeof(TLbtTriggerFireInfo);
	CBufFlat* buffer = CBufFlat::NewL(512);
	CleanupStack::PushL(buffer);
	RBufWriteStream writeStream;
	writeStream.Open(*buffer);
	CleanupClosePushL(writeStream);

	for(TInt i=0;i<iTriggerArray.Count(); ++i)
		{
		TLbtTriggerFireInfo fireInfo;
		// Get the fire info from Container
		fireInfo = iTriggerArray[i]->ExtendedTriggerInfo()->GetFiredInfo();
		TUint8* ptr = reinterpret_cast<TUint8*>(&fireInfo);
		writeStream.WriteL(ptr, sizeof(TLbtTriggerFireInfo));
		}
	writeStream.CommitL();
	CleanupStack::PopAndDestroy(&writeStream);
	TPtr8 ptr = buffer->Ptr(0);			
	// Write the trigger array into the IPC			
	TInt retVal = LbtGlobal::Write(iMessage, KParamTriggerFireInfo, ptr);			
	if(retVal == KErrNone)
		{
		// Complete the message
		LbtGlobal::RequestComplete(iMessage, KErrNone);
		}
	CleanupStack::PopAndDestroy(buffer);
	// Inform the observer that this operation has completed
    iObserver.HandleOperationClosureL(this, KErrNone);
    }
// end of file
