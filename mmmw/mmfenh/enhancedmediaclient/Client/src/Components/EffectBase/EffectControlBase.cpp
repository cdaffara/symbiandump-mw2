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
* Description:  Implementation of EffectControlBase class.
*
*/


#include "EffectControlBase.h"
#include "EffectControlBaseImpl.h"

using namespace multimedia;

CEffectControlBase::CEffectControlBase()
    {
    // No Impl
    }

CEffectControlBase::~CEffectControlBase()
    {
    delete iCEffectControlBaseImpl;
    }

void CEffectControlBase::ConstructL( TUid aCIUid )
    {
    iCEffectControlBaseImpl = CEffectControlBaseImpl::NewL( *this, aCIUid );
    }

TBool CEffectControlBase::IsConnected()
    {
    TBool status(EFalse);
    if ( iCEffectControlBaseImpl )
        {
        status = iCEffectControlBaseImpl->IsConnected();
        }
    return status;
    }

TInt CEffectControlBase::Connect()
    {
    TInt status(KErrNotReady);
    if ( iCEffectControlBaseImpl )
        {
        status = iCEffectControlBaseImpl->Connect();
        }
    return status;
    }

TInt CEffectControlBase::Disconnect()
    {
    TInt status(KErrNotReady);
    if ( iCEffectControlBaseImpl )
        {
        status = iCEffectControlBaseImpl->Disconnect();
        }
    return status;
    }

TInt CEffectControlBase::CustomCommandSync( TInt aFunction,
                                           const TDesC8& aDataTo1,
                                           const TDesC8& aDataTo2,
                                           TDes8& aDataFrom )
    {
    TInt status(KErrNotReady);
    if ( iCEffectControlBaseImpl )
        {
        status = iCEffectControlBaseImpl->CustomCommandSync( aFunction, aDataTo1, aDataTo2, aDataFrom );
        }
    return status;
    }

TInt CEffectControlBase::CustomCommandSync(TInt aFunction,
                       const TDesC8& aDataTo1,
                       const TDesC8& aDataTo2)
    {
    TInt status(KErrNotReady);
    if ( iCEffectControlBaseImpl )
        {
        status = iCEffectControlBaseImpl->CustomCommandSync( aFunction, aDataTo1, aDataTo2 );
        }
    return status;
    }

void CEffectControlBase::CustomCommandAsync(TInt aFunction,
                        const TDesC8& aDataTo1,
                        const TDesC8& aDataTo2,
                        TDes8& aDataFrom,
                        TRequestStatus& aStatus)
    {
    if ( iCEffectControlBaseImpl )
        {
        iCEffectControlBaseImpl->CustomCommandAsync( aFunction, aDataTo1, aDataTo2, aDataFrom, aStatus );
        }
    else
        {
        TRequestStatus* status = &aStatus;
		User::RequestComplete(status, KErrNotReady);
        }
    }

void CEffectControlBase::CustomCommandAsync(TInt aFunction,
                        const TDesC8& aDataTo1,
                        const TDesC8& aDataTo2,
                        TRequestStatus& aStatus)
    {
    if ( iCEffectControlBaseImpl )
        {
        iCEffectControlBaseImpl->CustomCommandAsync( aFunction, aDataTo1, aDataTo2, aStatus );
        }
    else
        {
        TRequestStatus* status = &aStatus;
		User::RequestComplete(status, KErrNotReady);
        }
    }

TInt CEffectControlBase::CustomCommandSync( const TMMFMessageDestinationPckg& aDestination,
                                            TInt aFunction,
                                            const TDesC8& aDataTo1,
                                            const TDesC8& aDataTo2,
                                            TDes8& aDataFrom )
    {
    TInt status(KErrNotReady);
    if ( iCEffectControlBaseImpl )
        {
        status = iCEffectControlBaseImpl->CustomCommandSync( aDestination, aFunction, aDataTo1, aDataTo2, aDataFrom );
        }
    return status;
    }

TInt CEffectControlBase::CustomCommandSync( const TMMFMessageDestinationPckg& aDestination,
                                            TInt aFunction,
                                            const TDesC8& aDataTo1,
                                            const TDesC8& aDataTo2 )
    {
    TInt status(KErrNotReady);
    if ( iCEffectControlBaseImpl )
        {
        status = iCEffectControlBaseImpl->CustomCommandSync( aDestination, aFunction, aDataTo1, aDataTo2 );
        }
    return status;
    }

void CEffectControlBase::CustomCommandAsync( const TMMFMessageDestinationPckg& aDestination,
                                             TInt aFunction,
                                             const TDesC8& aDataTo1,
                                             const TDesC8& aDataTo2,
                                             TDes8& aDataFrom,
                                             TRequestStatus& aStatus )
    {
    if ( iCEffectControlBaseImpl )
        {
        iCEffectControlBaseImpl->CustomCommandAsync( aDestination, aFunction, aDataTo1, aDataTo2, aDataFrom, aStatus );
        }
    else
        {
        TRequestStatus* status = &aStatus;
		User::RequestComplete(status, KErrNotReady);
        }
    }

void CEffectControlBase::CustomCommandAsync( const TMMFMessageDestinationPckg& aDestination,
                                             TInt aFunction,
                                             const TDesC8& aDataTo1,
                                             const TDesC8& aDataTo2,
                                             TRequestStatus& aStatus )
    {
    if ( iCEffectControlBaseImpl )
        {
        iCEffectControlBaseImpl->CustomCommandAsync( aDestination, aFunction, aDataTo1, aDataTo2, aStatus );
        }
    else
        {
        TRequestStatus* status = &aStatus;
		User::RequestComplete(status, KErrNotReady);
        }
    }

TInt CEffectControlBase::GetMessageHandle(TMMFMessageDestinationPckg &aPckg)
	{
    TInt status(KErrNotReady);
    if ( iCEffectControlBaseImpl )
        {
        status = iCEffectControlBaseImpl->GetMessageHandle(aPckg);
        }
    return status;		
	}

MCustomCommand* CEffectControlBase::GetCustomCommand()
	{
    if ( iCEffectControlBaseImpl )
        {
        return iCEffectControlBaseImpl->GetCustomCommand();
        }
	else
		{
		return NULL;	
		}
	}


TInt CEffectControlBase::GetControlObserver( MControlObserver*& aObserver )
    {
    TInt status(KErrNotReady);
    if ( iCEffectControlBaseImpl )
        {
        aObserver = (MControlObserver*)iCEffectControlBaseImpl;
        status = KErrNone;
        }
    return status;
    }

TInt CEffectControlBase::AddedToStreamControl( MStreamControl& aStreamControl,
                                               MCustomCommand& aCustomCommand,
                                               MEffectControl& aEffect )
    {
    TInt status(KErrNotReady);
    if ( iCEffectControlBaseImpl )
        {
        status = iCEffectControlBaseImpl->AddedToStreamControl( aStreamControl, aCustomCommand, aEffect );
        }
    return status;
    }

TInt CEffectControlBase::RemovedFromStreamControl( MStreamControl& aStreamControl )
    {
    TInt status(KErrNotReady);
    if ( iCEffectControlBaseImpl )
        {
        status = iCEffectControlBaseImpl->RemovedFromStreamControl( aStreamControl );
        }
    return status;
    }

void CEffectControlBase::StreamControlDeleted( MStreamControl& aStreamControl )
    {
    if ( iCEffectControlBaseImpl )
        {
        iCEffectControlBaseImpl->StreamControlDeleted( aStreamControl );
        }
    }

void CEffectControlBase::Event( TEffectControlEvent /*aEvent*/ )
    {
    // No Impl
    }

// End of file
