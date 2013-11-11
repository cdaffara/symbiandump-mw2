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
* Description:  Definition of the ClientFileSource class.
*
*/


#include <MCustomCommand.h>
#include <SourceControlObserver.h>
#include "ClientFileSource.h"
#include "EventNotifier.h"
#include "ErrorCode.h"


#define RETURN_IF_ERROR(x) if(x != KErrNone) return x

using namespace multimedia;

CClientFileSource::CClientFileSource()
:iMimeType(NULL),
iFileName(NULL)
    {
    // No Impl
    }

CClientFileSource::~CClientFileSource()
    {
    delete iMimeType;
    delete iFileName;
    delete iAsyncEventAO;
    delete iEventNotifier;
    }

TInt CClientFileSource::PostConstructor()
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
TInt CClientFileSource::AddObserver( MControlObserver& aObserver )
    {
    return iEventNotifier->AddObserver( aObserver );
    }

TInt CClientFileSource::RemoveObserver( MControlObserver& aObserver )
    {
    return iEventNotifier->RemoveObserver( aObserver );
    }

TUid CClientFileSource::Type()
    {
    return KFileSourceControl;
    }

TControlType CClientFileSource::ControlType()
    {
    return ESourceControl;
    }
// From MControl ends

// From MSourceControl begins
TInt CClientFileSource::GetSize( TUint& aSize )
    {
    TInt status(KErrUnknown);
    
    if(iServerSourceExists)
        {
        if(!iFileSize)
            {
            iEventPckg().iType = EFileSize;
            iCustomCommand->CustomCommandSync( iSourceHandle, EGetFileSourceEvent, iEventPckg, KNullDesC8, iEventPckg );
            aSize = iFileSize = iEventPckg().iFileSize;
            status = KErrNone;
            }
        else
            {
            aSize = iFileSize;
            status = KErrNone;    
            }
        }
     else
        {
        aSize = 0;
        status = KErrUnknown;
        }   
    
    return status;
    }

TInt CClientFileSource::GetMimeType( TDes8& aMimeType )
    {
    TInt status(KErrUnknown);
    if ( iMimeType )
        {
        aMimeType.Copy(*iMimeType);
        status = KErrNone;
        }
    return status;
    }

TInt CClientFileSource::Close()
    {
    delete iMimeType;
    iMimeType = NULL;
    delete iFileName;
    iFileName = NULL;
    iBitRate = 0;
    iFileSize = 0;
    return KErrNone;
    }

// From MSourceControl ends

// From MFileSource begins
TInt CClientFileSource::Open( TDesC& aFileName, TDesC8& aMimeType  )
    {
    TInt status(KErrAlreadyExists);
    Close();
    if ( aFileName.Ptr() != NULL )
        {
        if(aMimeType.Size())
            {
            TRAP( status, iMimeType = aMimeType.AllocL() );
            RETURN_IF_ERROR( status );
            }
        
        TRAPD(status,iFileName = HBufC::NewL(aFileName.Size()));
        
        if(!status)
            {
            TPtr des = iFileName->Des();
            des.Copy(aFileName);
            }
        else
            return status;   

        }
    return status;
    }

TInt CClientFileSource::GetBitRate( TUint& aRate )
    {
    TInt status(KErrNone);
    
    if(iServerSourceExists)
        {
        if(!iBitRate)
            {
            iEventPckg().iType = EFileBitRateChanged;
            iCustomCommand->CustomCommandSync( iSourceHandle, EGetFileSourceEvent, iEventPckg, KNullDesC8, iEventPckg );
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

TInt CClientFileSource::GetFileName(TPtr& aFileName)
    {
    TInt status(KErrNone);
    aFileName.Set(iFileName->Des());
    return status;
    }

// From MFileSource ends

// From CSourceBase begins
void CClientFileSource::ServerSourceCreated( MCustomCommand& aCustomCommand,
                            TMMFMessageDestination& aSourceHandle )
    {
    iServerSourceExists = ETrue;
    iCustomCommand = &aCustomCommand;
    iSourceHandle = aSourceHandle;

    

    
    // Subcribe for source event notifications
    iEventPckg().iType = EFileSourceStateChanged;
    iAsyncEventAO->SetActive();
    iCustomCommand->CustomCommandAsync( iSourceHandle,
                                        EGetFileSourceEvent,
                                        iEventPckg,
                                        KNullDesC8,
                                        iEventPckg,
                                        iAsyncEventAO->iStatus );
    }

void CClientFileSource::ServerSourceDeleted()
    {
    iServerSourceExists = EFalse;
    iCustomCommand = NULL;
    }

TBool CClientFileSource::IsEncrypted()
    {
    return EFalse;
    }

TUid CClientFileSource::GetSourceUid()
    {
    return KFileDataSourcePlugin;
    }

TInt CClientFileSource::GetHeaderData( TPtr& /*aPtr*/ )
    {
    return KErrNotSupported;
    }

// From CSourceBase ends

// From MAsyncAOObserver begins
void CClientFileSource::Event( TInt aError )
    {
    if ( aError == KErrNone )
        {
#ifdef _DEBUG
        RDebug::Print(_L("CClientFileSource::Event() Client State[%d] Type[%d]"), iEventPckg().iState, iEventPckg().iType);
#endif        
        switch (iEventPckg().iType)
            {
            case EFileBitRateChanged:
                DoBitRateChanged(iEventPckg().iBitRate);                
                break;
            case EFileSourceStateChanged:
                iState = iEventPckg().iState;
                break;

            default:
                break;
            };
        }

    // Subcribe for more source event notifications
    iEventPckg().iState = iState;
    
    if(iServerSourceExists)
        {
        iAsyncEventAO->SetActive();
        iCustomCommand->CustomCommandAsync( iSourceHandle,
                                            EGetFileSourceEvent,
                                            iEventPckg,
                                            KNullDesC8,
                                            iEventPckg,
                                            iAsyncEventAO->iStatus );
        }
    }

// From MAsyncAOObserver ends

void CClientFileSource::DoBitRateChanged( TUint aBitRate )
    {
    iBitRate = aBitRate;
    CErrorCode* event1 = new CErrorCode( KErrNone );       
    iEventNotifier->Event(this,MSourceControlObserver::KBitRateChangedEvent,event1);
    }

// End of file
