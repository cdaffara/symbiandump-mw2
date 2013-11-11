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
* Description:  Definition of the ClientDescriptorSource class.
*
*/


#include "ClientDescriptorSource.h"
#include <MCustomCommand.h>
#include "EventNotifier.h"

#define RETURN_IF_ERROR(x) if(x != KErrNone) return x

using namespace multimedia;

CClientDescriptorSource::CClientDescriptorSource()
:iMimeType(NULL),
iDataPtr(0,0,0),
iCurrentPosition(0)
    {
    // No Impl
    }

CClientDescriptorSource::~CClientDescriptorSource()
    {
    delete iMimeType;
    delete iAsyncEventAO;
    delete iEventNotifier;
    }

TInt CClientDescriptorSource::PostConstructor()
    {
    TInt status(KErrNone);
    TRAP(status, iAsyncEventAO = CAsyncAO::NewL(*this));
    RETURN_IF_ERROR( status );
    
    if ( !iEventNotifier )
        {
        TRAP(status, iEventNotifier = CEventNotifier::NewL());
        }

    return status;
    }

// From MControl begins
TInt CClientDescriptorSource::AddObserver( MControlObserver& aObserver )
    {
    return iEventNotifier->AddObserver( aObserver );
    }

TInt CClientDescriptorSource::RemoveObserver( MControlObserver& aObserver )
    {
    return iEventNotifier->RemoveObserver( aObserver );
    }

TUid CClientDescriptorSource::Type()
    {
    return KDescriptorSourceControl;
    }

TControlType CClientDescriptorSource::ControlType()
    {
    return ESourceControl;
    }
// From MControl ends

// From MSourceControl begins
TInt CClientDescriptorSource::GetSize( TUint& aSize )
    {
    TInt status(KErrUnknown);
    if ( iDataPtr.Ptr() != NULL )
        {
        aSize = iDataPtr.Length();
        status = KErrNone;
        }
    return status;
    }

TInt CClientDescriptorSource::GetMimeType( TDes8& aMimeType )
    {
    TInt status(KErrUnknown);
    if ( iMimeType )
        {
        aMimeType.Copy(*iMimeType);
        status = KErrNone;
        }
    return status;
    }

TInt CClientDescriptorSource::Close()
    {
    delete iMimeType;
    iMimeType = NULL;
    iDataPtr.Set(0,0,0);    
    iBitRate = 0;
    return KErrNone;
    }

// From MSourceControl ends

// From MDescriptorSource begins
TInt CClientDescriptorSource::Open( TDesC8& aMimeType, TDesC8& aData )
    {
    TInt status(KErrAlreadyExists);
    Close();
    if ( aData.Size() )
        {
        TRAP( status, iMimeType = aMimeType.AllocL() );
        RETURN_IF_ERROR( status );
        
        iDataPtr.Set(const_cast<TUint8*>(aData.Ptr()), aData.Length(), aData.Length() );
        status = KErrNone;
        }
    return status;
    }

TInt CClientDescriptorSource::GetBitRate( TUint& aRate )
    {
    TInt status(KErrNone);
    
    if(iServerSourceExists)
        {
        if(!iBitRate)
            {
            iEventPckg().iType = EBitRateChanged;
            iCustomCommand->CustomCommandSync( iSourceHandle, EGetSourceBitRate, iEventPckg, KNullDesC8, iEventPckg );
            aRate = iBitRate = iEventPckg().iBitRate;
            status = KErrNone;
            }
        else
            {
            aRate = iBitRate;
            status = KErrNone;    
            }
        }
     else
        {
        aRate = 0;
        status = KErrUnknown;
        }   
    
    return status;
    }

// From MDescriptorSource ends

// From CSourceBase begins
void CClientDescriptorSource::ServerSourceCreated( MCustomCommand& aCustomCommand,
                            TMMFMessageDestination& aSourceHandle )
    {
    iServerSourceExists = ETrue;
    iCustomCommand = &aCustomCommand;
    iSourceHandle = aSourceHandle;
    
    // Send a custom command to Source indicating the source is seekable.
    TCCAttributesPckgBuf pckg;
    pckg().iSeeking = EFalse;
    pckg().iRandomSeeking = EFalse;
    pckg().iSourceSizeInBytes = iDataPtr.Length();

    iCustomCommand->CustomCommandSync( iSourceHandle, ESetConfig, pckg, KNullDesC8 );
    
    // Subcribe for source event notifications
    iAsyncEventAO->SetActive();
    iCustomCommand->CustomCommandAsync( iSourceHandle,
                                        EGetSourceEvent,
                                        KNullDesC8,
                                        KNullDesC8,
                                        iEventPckg,
                                        iAsyncEventAO->iStatus );
    }

void CClientDescriptorSource::ServerSourceDeleted()
    {
    iServerSourceExists = EFalse;
    iCustomCommand = NULL;
    }

TBool CClientDescriptorSource::IsEncrypted()
    {
    return EFalse;
    }

TUid CClientDescriptorSource::GetSourceUid()
    {
    return KDescriptorDataSourcePlugin;
    }

TInt CClientDescriptorSource::GetHeaderData( TPtr& /*aPtr*/ )
    {
    return KErrNotSupported;
    }

// From CSourceBase ends

// From MAsyncAOObserver begins
void CClientDescriptorSource::Event( TInt aError )
    {
    if ( aError == KErrNone )
        {
        switch (iEventPckg().iType)
            {
            case EFillData:
                DoFillData(iEventPckg().iFillDataSize);
                break;
            case ESeek:
                DoSeek(iEventPckg().iSeekPos);
                break;
            case EBitRateChanged:
                DoBitRateChanged(iEventPckg().iBitRate);
                break;
            default:
                break;
            };
        }

    // Subcribe for more source event notifications
    iAsyncEventAO->SetActive();
    iCustomCommand->CustomCommandAsync( iSourceHandle,
                                        EGetSourceEvent,
                                        KNullDesC8,
                                        KNullDesC8,
                                        iEventPckg,
                                        iAsyncEventAO->iStatus );
    }

// From MAsyncAOObserver ends
void CClientDescriptorSource::DoFillData( TUint aSize )
    {
    // We could use Min
    TUint length = ( (iDataPtr.Length() - iCurrentPosition) > aSize ) ? aSize : (iDataPtr.Length() - iCurrentPosition) ;
    TPtr8 dataPtr(const_cast<TUint8*>(iDataPtr.Ptr()) + iCurrentPosition, length, length);
    iCurrentPosition += length;
    
    TCCAttributesPckgBuf pckg;
    pckg().iLastBuffer = EFalse;
    if ( iCurrentPosition == iDataPtr.Length() )
        {
        pckg().iLastBuffer = ETrue;
        iCurrentPosition = 0;
        }
    iCustomCommand->CustomCommandSync( iSourceHandle, EFillDataResponse, pckg, dataPtr );
    }

void CClientDescriptorSource::DoSeek( TUint aSeekPos )
    {
    iCurrentPosition = aSeekPos;
    }

void CClientDescriptorSource::DoBitRateChanged( TUint aBitRate )
    {
    iBitRate = aBitRate;
    }

// End of file
