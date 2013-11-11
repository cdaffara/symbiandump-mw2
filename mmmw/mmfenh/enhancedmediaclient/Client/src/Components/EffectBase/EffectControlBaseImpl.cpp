/*
* Copyright (c) 2006 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:  Implementation of EffectControlBaseImpl class.
*
*/

#include <StreamControl.h>
#include <MCustomCommand.h>
#include <ControlObserver.h>
#include <EffectControl.h>
#include <VolumeControl.h>

#include "CustomInterfaceBuilderTypes.h"
#include "EffectControlBaseImpl.h"
#include "EffectControlBase.h"

const TUid KUidCIFBuilderParser = {0x10207B29};

using namespace multimedia;

CEffectControlBaseImpl::CEffectControlBaseImpl( CEffectControlBase& aParent, TUid aCIUid )
: CActive(CActive::EPriorityStandard),
  iParent(aParent),
  iCIUid(aCIUid),
  iStreamControl(NULL),
  iCustomCommand(NULL),
  iEffect(NULL)
    {
    CActiveScheduler::Add(this);
    }

CEffectControlBaseImpl::~CEffectControlBaseImpl()
    {
    Cancel();
    // Signal MStreamControl if this effect class is deleted before
    // removing from it
    if ( iStreamControl )
        {
        iStreamControl->RemoveEffect( *iEffect );
        };
    }

CEffectControlBaseImpl* CEffectControlBaseImpl::NewL( CEffectControlBase& aParent, TUid aCIUid )
    {
    CEffectControlBaseImpl* self = new (ELeave) CEffectControlBaseImpl( aParent, aCIUid );
    CleanupStack::PushL( self );
    self->ConstructL();
    CleanupStack::Pop( self );
    return self;
    }

void CEffectControlBaseImpl::ConstructL()
    {
    // No impl yet
    }

TInt CEffectControlBaseImpl::AddedToStreamControl( MStreamControl& aStreamControl,
                                                   MCustomCommand& aCustomCommand,
                                                   MEffectControl& aEffect )
    {
    TInt status(KErrAlreadyExists);
    if ( !iStreamControl )
        {
        iStreamControl = &aStreamControl;
        iCustomCommand = &aCustomCommand;
        iEffect = &aEffect;
        status = KErrNone;
        }

	// This is done here to get the MessageHandler if the Controller
	// is already loaded, otherwise we dont return an Error from Connect    
    if(iMsgHndlrHandlePckg().InterfaceId() == KNullUid)
    	{
		status = Connect();
		if(status == KErrNone)
	        {
/*
	        // This results in subclasses getting notification asynchronously.
	        // In the mean time clients may already start setting attributes on
	        // effect objects which will be lost if proxy is not created
	        // In order to eliminate this possibility, notify synchronously
	        CActive::Cancel();
	        CActive::SetActive();
	        TRequestStatus* status = &iStatus;
			User::RequestComplete( status, EControllerLoaded );			
*/
	        iParent.Event( CEffectControlBase::ECIBuilderCreated );
	        }
	    else
	    	{
	    	// This Error is Overwritten because the Connect failed.
	    	// Its ok here since we are just checking if the controller
	    	// is loaded and the msghandler created.	
	    	status = KErrNone;	
	    	}    
        }

    return status;
    }

TInt CEffectControlBaseImpl::RemovedFromStreamControl( MStreamControl& /*aStreamControl*/ )
    {
    Disconnect();	
    iStreamControl = NULL;
    iCustomCommand = NULL;
    iEffect = NULL;
    return KErrNone;
    }

void CEffectControlBaseImpl::StreamControlDeleted( MStreamControl& /*aStreamControl*/ )
    {
    iStreamControl = NULL;
    iCustomCommand = NULL;
    iMsgHndlrHandlePckg().InterfaceId() = KNullUid;
    }

TBool CEffectControlBaseImpl::IsConnected()
    {
    if ( iMsgHndlrHandlePckg().InterfaceId() != KNullUid )
        {
        return ETrue;
        }
    else
        {
        return EFalse;
        }
    }

TInt CEffectControlBaseImpl::Connect()
    {
    TInt status(KErrNone);
	
	// Since the Volume Effect does not have Proxy 
	// and message handler we return here only
	if(iEffect->Type() == KVolumeEffectControl)
		{
		return KErrNone;	
		}
	
	if(!iCustomCommand)
		{
		return KErrNotReady;	
		}
	
	TMMFMessageDestination     destination(KUidCustomInterfaceBuilder);
	TMMFMessageDestinationPckg destinationPckg(destination);

    if ( iMsgHndlrHandlePckg().InterfaceId() == KNullUid )
        {

        status = iCustomCommand->CustomCommandSync(destinationPckg, ECibGetBuilder, KNullDesC8, KNullDesC8, iBuilderHandle);

		if(iBuilderHandle().InterfaceId() == KNullUid || status != KErrNone)
			{
			return status;
			}
		
        // Get handle to message handler from CI Bldr parser
        status = BuilderParserMessage(ECIFCreateMessageHandler);
        if( iMsgHndlrHandlePckg().InterfaceId() == KNullUid )
            {
            status = KErrNotSupported;
            }
        }

    return status;
    }

TInt CEffectControlBaseImpl::Disconnect()
    {
    TInt status(KErrNotReady);
    if ( ( iMsgHndlrHandlePckg().InterfaceId() != KNullUid ) && (iCustomCommand) )
        {
        // Get handle to message handler from CI Bldr parser
        status = BuilderParserMessage(ECIFRemoveMessageHandler);
		// This is done here since the Interface ID is not set to NULL when
		// we set it using KNullUid.
		TMMFMessageDestination temp;	    
        iMsgHndlrHandlePckg() = temp;
        }
    return status;
    }

TInt CEffectControlBaseImpl::CustomCommandSync(TInt aFunction,
                       const TDesC8& aDataTo1,
                       const TDesC8& aDataTo2,
                       TDes8& aDataFrom)
    {
    return CustomCommandSync( iMsgHndlrHandlePckg,
                              aFunction,
                              aDataTo1,
                              aDataTo2,
                              aDataFrom );
    }

TInt CEffectControlBaseImpl::CustomCommandSync(TInt aFunction,
                       const TDesC8& aDataTo1,
                       const TDesC8& aDataTo2)
    {
    return CustomCommandSync( iMsgHndlrHandlePckg,
                              aFunction,
                              aDataTo1,
                              aDataTo2 );
    }

void CEffectControlBaseImpl::CustomCommandAsync(TInt aFunction,
                        const TDesC8& aDataTo1,
                        const TDesC8& aDataTo2,
                        TDes8& aDataFrom,
                        TRequestStatus& aStatus)
    {
    CustomCommandAsync( iMsgHndlrHandlePckg,
                        aFunction,
                        aDataTo1,
                        aDataTo2,
                        aDataFrom,
                        aStatus );
    }

void CEffectControlBaseImpl::CustomCommandAsync(TInt aFunction,
                        const TDesC8& aDataTo1,
                        const TDesC8& aDataTo2,
                        TRequestStatus& aStatus)
    {
    CustomCommandAsync( iMsgHndlrHandlePckg,
                        aFunction,
                        aDataTo1,
                        aDataTo2,
                        aStatus );
    }

TInt CEffectControlBaseImpl::CustomCommandSync( const TMMFMessageDestinationPckg& aDestination,
                                                TInt aFunction,
                                                const TDesC8& aDataTo1,
                                                const TDesC8& aDataTo2,
                                                TDes8& aDataFrom )
    {
    TInt status(KErrNotReady);
    if ( ( iCustomCommand ) && ( aDestination().InterfaceId() != KNullUid ) )
        {
        status = iCustomCommand->CustomCommandSync( aDestination,
                                                    aFunction,
                                                    aDataTo1,
                                                    aDataTo2,
                                                    aDataFrom );
        }
    return status;
    }

TInt CEffectControlBaseImpl::CustomCommandSync( const TMMFMessageDestinationPckg& aDestination,
                                                TInt aFunction,
                                                const TDesC8& aDataTo1,
                                                const TDesC8& aDataTo2 )
    {
    TInt status(KErrNotReady);
    if ( ( iCustomCommand ) && ( aDestination().InterfaceId() != KNullUid ) )
        {
        status = iCustomCommand->CustomCommandSync( aDestination,
                                                    aFunction,
                                                    aDataTo1,
                                                    aDataTo2 );
        }
    return status;
    }

void CEffectControlBaseImpl::CustomCommandAsync( const TMMFMessageDestinationPckg& aDestination,
                                                 TInt aFunction,
                                                 const TDesC8& aDataTo1,
                                                 const TDesC8& aDataTo2,
                                                 TDes8& aDataFrom,
                                                 TRequestStatus& aStatus )
    {
    if ( ( iCustomCommand ) && ( aDestination().InterfaceId() != KNullUid ) )
        {
        iCustomCommand->CustomCommandAsync( aDestination,
                                            aFunction,
                                            aDataTo1,
                                            aDataTo2,
                                            aDataFrom,
                                            aStatus );
        }
    else
        {
        TRequestStatus* status = &aStatus;
		User::RequestComplete(status, KErrNotReady);
        }
    }

void CEffectControlBaseImpl::CustomCommandAsync( const TMMFMessageDestinationPckg& aDestination,
                                                 TInt aFunction,
                                                 const TDesC8& aDataTo1,
                                                 const TDesC8& aDataTo2,
                                                 TRequestStatus& aStatus )

    {
    if ( ( iCustomCommand ) && ( aDestination().InterfaceId() != KNullUid ) )
        {
        iCustomCommand->CustomCommandAsync( aDestination,
                                            aFunction,
                                            aDataTo1,
                                            aDataTo2,
                                            aStatus );
        }
    else
        {
        TRequestStatus* status = &aStatus;
		User::RequestComplete(status, KErrNotReady);
        }    
    }

TInt CEffectControlBaseImpl::GetMessageHandle(TMMFMessageDestinationPckg &aPckg)
	{
    TInt status(KErrNone);
    aPckg = iMsgHndlrHandlePckg;
    return status;		
	}

MCustomCommand* CEffectControlBaseImpl::GetCustomCommand()
	{
    return iCustomCommand;
	}
	
void CEffectControlBaseImpl::RunL()
    {    
    TInt status(KErrNone);	 
    if ( iStatus.Int() == EControllerLoaded )
        {
        status = Connect();
        if(status == KErrNone)
        	{
        	iParent.Event( CEffectControlBase::ECIBuilderCreated );
        	}
        }
    else if ( iStatus.Int() == EControllerUnloaded )
        {
        status = Disconnect();
        if(status == KErrNone)
        	{
        	iParent.Event( CEffectControlBase::EMessageHandlerDeleted );
        	}
        }
    }

void CEffectControlBaseImpl::DoCancel()
    {
    // Do Nothing
    }

TInt CEffectControlBaseImpl::RunError( TInt /*aError*/ )
    {
    // RunL should never leave, if it did that means
    // the code left during event handling
    return KErrNone;
    }

void CEffectControlBaseImpl::Event( MControl* /*aControl*/, TUint aEventType, TAny* /*aEventObject*/ )
    {
    if ( aEventType == EControllerLoaded )
        {
        CActive::Cancel();
        CActive::SetActive();
        TRequestStatus* status = &iStatus;
		User::RequestComplete( status, EControllerLoaded );
        }
    else if ( aEventType == EControllerUnloaded )
        {
        CActive::Cancel();
        CActive::SetActive();
        TRequestStatus* status = &iStatus;
		User::RequestComplete( status, EControllerUnloaded );
        }
    }

TInt CEffectControlBaseImpl::BuilderParserMessage(CIFBuilderParserFunctions aCommand)
    {
    TInt status(KErrNotReady);
    if ( iCustomCommand )
        {
        TMMFMessageDestination     ciBldr(KUidCIFBuilderParser);
        TMMFMessageDestinationPckg ciBldrPckg(ciBldr);
        TPckgBuf<TUid> uidPckg;
        uidPckg() = iCIUid;
        switch (aCommand)
            {
            case ECIFCreateMessageHandler:
            status = iCustomCommand->CustomCommandSync( iBuilderHandle,
                                                        ECibBuild,
                                                        uidPckg,
                                                        KNullDesC8,
                                                        iMsgHndlrHandlePckg);
                break;
            case ECIFRemoveMessageHandler:
            status = iCustomCommand->CustomCommandSync( iBuilderHandle,
                                                        ECibRemove,
                                                        iMsgHndlrHandlePckg,
                                                        KNullDesC8);
                break;
            default:
                status = KErrUnknown;
                break;
            };
        }
    return status;
    }


// End of file
