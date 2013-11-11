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
* Description:  Implementation of the ClientDataBufferSource class.
*
*/



#include "ClientDataBufferSource.h"
#include "DataBufferQueueItem.h"
#include "DataBufferSourceCustomCommands.h"
#include "EventNotifier.h"
#include "DataBufferProcessedEvent.h"
#include "DataBufferSourceUid.h"
#include "DRMConfigIntfcImpl.h"

#include <e32std.h>
#include <MCustomCommand.h>
#include <DataBuffer.h>
#include <MimeTypes.h>
#include <SourceControlObserver.h>
#include "tracemacros.h"

#define RETURN_IF_ERROR(x) if(x != KErrNone) return x

using namespace multimedia;

// CONSTANTS
const TInt KMinBufferSize = 5120;


CClientDataBufferSource::CClientDataBufferSource()
:iDRMType(ENone)
    {
	iFixedDurationSupport = false;
    }

CClientDataBufferSource::~CClientDataBufferSource()
    {
    delete iMimeType;
    EmptyQueue();
    delete iEventNotifier;
    delete iAudioBufferQueue;
    delete iBufTypeSupportEvent;
    iAllowedOutputDeviceList.Close();
    }

TInt CClientDataBufferSource::PostConstructor()
    {
    TInt status(KErrNone);
    // Make sure this doesn't get called second time around.
    if ( !iEventNotifier )
        {
        TRAP(status, iEventNotifier = CEventNotifier::NewL());
        }
    
    RETURN_IF_ERROR(status);
    
    iAudioBufferQueue = new TSglQue<CDataBufferQueueItem>(_FOFF(CDataBufferQueueItem, iLink));
    if (!iAudioBufferQueue)
        {
        status = KErrNoMemory;
        }
    RETURN_IF_ERROR(status);
    
    TRAP(status,iBufTypeSupportEvent  = CBufTypeSupEventAO::NewL(*this)); 
    
    return status;
    }

TInt CClientDataBufferSource::AddObserver( MControlObserver& aObserver )
    {
    return iEventNotifier->AddObserver( aObserver );
    }

TInt CClientDataBufferSource::RemoveObserver( MControlObserver& aObserver )
    {
    return iEventNotifier->RemoveObserver( aObserver );
    }

TUid CClientDataBufferSource::Type()
    {
    return KDataBufferSourceControl;
    }

TControlType CClientDataBufferSource::ControlType()
    {
    return ESourceControl;
    }

TInt CClientDataBufferSource::GetSize( TUint& aSize )
    {
    TInt status(KErrNone);
    aSize = iSourceSize;
    return status;
    }

TInt CClientDataBufferSource::GetMimeType( TDes8& aMimeType )
    {
    aMimeType.Copy(*iMimeType);
    return KErrNone;
    }

TInt CClientDataBufferSource::Close()
    {
    TInt status(KErrNone);
    iBufferSeqNum = 0;
    return status;
    }
    
TInt CClientDataBufferSource::Open( TDesC8& aMimeType , MDataBuffer& /*aHeaderData */)
    {
    TInt status(KErrNotSupported);
    // Source is already attached to controller
    if ( iServerSourceExists )
        {
        status = KErrInUse;
        }
    else
        {
        delete iMimeType;
        iMimeType = NULL;
        TRAP( status, iMimeType = aMimeType.AllocL() );
        iBufferSeqNum = 0;
        }
    return status;
    }

TInt CClientDataBufferSource::GetMinimumBufferSize( TUint& aBufferSize )
    {
    TInt status(KErrNone);
    aBufferSize = KMinBufferSize;
    return status;
    }

TInt CClientDataBufferSource::SetSize( TUint aSize )
    {
    TInt status(KErrNone);
    iSourceSize = aSize;
    if ( iServerSourceExists )
        {
        TPckgBuf<TInt> pckg(aSize);
        status = iCustomCommand->CustomCommandSync( iSourceHandlePckg, ESetSize, pckg, KNullDesC8 );
        }
    return status;
    }

TInt CClientDataBufferSource::WriteData(MDataBuffer& aBuffer )
    {
    EMC_TRACE3(_L("CClientDataBufferSource::WriteData:size[%d]lastbuf[%d]"), \
            aBuffer.GetBufferPtr().Length(), aBuffer.IsLastBuffer());
/*
	TUint8* bufPtr = const_cast<TUint8*>(aBuffer.GetBufferPtr().Ptr());
	if ( aBuffer.GetBufferPtr().Length() > 5 )
    	{
    	RDebug::Print(_L("CClientDataBufferSource::WriteData data[0x%x][0x%x][0x%x][0x%x][0x%x]"), \
    	*bufPtr,*(bufPtr+1),*(bufPtr+2),*(bufPtr+3),*(bufPtr+4));
        }
*/

    TInt status(KErrNotReady);
    // For now we can only write audio data only if controller is loaded
    // and source is added to controller.
    if ( iServerSourceExists )
        {
        // Append item to queue
        CDataBufferQueueItem* item(NULL);
        TRAP(status, item  = CDataBufferQueueItem::NewL( *this,
                                                         aBuffer,
                                                         aBuffer.IsLastBuffer(),
                                                         iBufferSeqNum ));
        RETURN_IF_ERROR(status);
        
        iAudioBufferQueue->AddLast(*item);

        // Send data to server source
        item->SetActive();
        TPckgBuf<TUint> bufSeqNum;
        bufSeqNum() = iBufferSeqNum;
        iCustomCommand->CustomCommandAsync(
            iSourceHandlePckg,
            (TInt)EProcessBuffer,
            aBuffer.GetBufferPtr(),
            item->GetDataBufferAttributesDesc(),
            item->iStatus);
        status = KErrNone;
        iBufferSeqNum++;
        }
    return status;
    }

TInt CClientDataBufferSource::EmptyBuffers()
    {
    TInt status(KErrNone);
    return status;
    }

TInt CClientDataBufferSource::GetBufferingConfig(TBufferingConfig& aConfig)
    {
	TPckgBuf<TBufferingConfig> pckg;
    if(iServerSourceExists)
        {
		iCustomCommand->CustomCommandSync( iSourceHandlePckg, EGetCurrentBufferingConfig, KNullDesC8, KNullDesC8, pckg );
		aConfig = pckg();
		return KErrNone;
        }
    else
        {
		return KErrNotFound;    
        }
    }
    
TInt CClientDataBufferSource::SetBufferingConfig(TBufferingConfig aConfig)
    {
	TPckgBuf<TBufferingConfig> pckg(aConfig);
	if ( iServerSourceExists)
		{
		iCustomCommand->CustomCommandSync( iSourceHandlePckg, ESetBufferingConfig, pckg, KNullDesC8 );              
		return KErrNone;    
		}
	else
		{
		return KErrNotFound;    
		}
    }
    
TInt CClientDataBufferSource::GetBitRate(TUint& aRate)
    {
    //Source Custom Command
    TPckgBuf<TUint> pckg;
    if(iServerSourceExists)
        {
		iCustomCommand->CustomCommandSync( iSourceHandlePckg, EGetBitRate, KNullDesC8, KNullDesC8, pckg );
		aRate = pckg();
		return KErrNone;
        }
    else
        {
		aRate = 0;
		return KErrNotFound;    
        }
    }
              
TInt CClientDataBufferSource::GetBufferingTypesSupported(TArray<TBufferingConfig::TBufferingType>& aArray)
    {
	RArray<TBufferingConfig::TBufferingType> temp;
	temp.Array() = aArray;
	temp.Append(TBufferingConfig::BUFFERINGNONE);
	temp.Append(TBufferingConfig::FIXEDSIZE);
	if(iFixedDurationSupport)
		{
        temp.Append(TBufferingConfig::FIXEDDURATION);
		}
	aArray = temp.Array();
	return KErrNone;
    }

TInt CClientDataBufferSource::GetInterface(
                                TUid aInterfaceId,
                                TVersion& aVer,
                                TAny*& aInterfaceImpl )
    {
    TInt status(KErrNotFound);
    aInterfaceImpl = NULL;
    if ( ( aInterfaceId == KDRMConfigIntfc ) &&
         ( ( aVer.iMajor == KDRMConfigIntfcMajorVer1 ) &&
           ( aVer.iMinor == KDRMConfigIntfcMinorVer1 ) &&
           ( aVer.iBuild == KDRMConfigIntfcBuildVer1 ) ) )
            {
            CDRMConfigIntcfImpl* temp(NULL);
            TRAP(status, temp = CDRMConfigIntcfImpl::NewL(*this));
            if ( status == KErrNone )
                {
                this->SetChild(*((CChildIntfc*)temp));
                temp->SetParent(*(CParentIntfc*)this);
                aInterfaceImpl = (CDRMConfigIntfc*)temp;
                }
            }
    return status;
    }

void CClientDataBufferSource::ServerSourceCreated( MCustomCommand& aCustomCommand,
                                                     TMMFMessageDestination& aSourceHandle )
    {
    iServerSourceExists = ETrue;
    iCustomCommand = &aCustomCommand;
    iSourceHandle = aSourceHandle;    
    iSourceHandlePckg() = aSourceHandle;
    
    // Send DRM Config to server side plugin
    TRAP_IGNORE( DoCommitDRMConfigL() );
    
    iBufTypeSupportEvent->SetActive();
    iCustomCommand->CustomCommandAsync(
        iSourceHandlePckg,
        (TInt)EGetBufferingConfigSupported,
        KNullDesC8,
        KNullDesC8,
        iBufTypeSupportEvent->iStatus);
    
    }

void CClientDataBufferSource::ServerSourceDeleted()
    {
    iServerSourceExists = EFalse;
    iCustomCommand = NULL;
    // Do client buffer cleanup
    // It should've already been done when server side source was deleted.
    // This is just to make sure.
    }

TBool CClientDataBufferSource::IsEncrypted()
    {
	return KErrNotSupported;   
    }
    
TUid CClientDataBufferSource::GetSourceUid()
    {
	return KMmfDataBufferSource;
    }
    
TInt CClientDataBufferSource::GetHeaderData(TPtr& /*aPtr*/)
    {
	return KErrNotSupported;      
    }


void CClientDataBufferSource::BufferProcessed( CDataBufferQueueItem* aItem )
    {
    // Create a event object
    CDataBufferProcessedEvent* event = new CDataBufferProcessedEvent(
        &(aItem->DataBuffer()),
        aItem->Error() );
    
    // Remove the item from the list and delete it.
    iAudioBufferQueue->Remove(*aItem);
    delete aItem;
    
    // Send event to client that buffer is processed
    iEventNotifier->Event( this, MSourceControlObserver::KBufferProcessedEvent, event );
    }

void CClientDataBufferSource::HandleCancel( CDataBufferQueueItem& aItem )
    {
    EMC_TRACE1(_L("CClientDataBufferSource::HandleCancel:"));
    if ( iServerSourceExists )
        {
        TPckgBuf<TUint> bufSeqPckg(aItem.GetBufferSequenceNumber());
        iCustomCommand->CustomCommandSync(
                iSourceHandlePckg,
                (TInt)ECancel,
                bufSeqPckg,
                KNullDesC8
                );
        }
    else
        {
        aItem.CompleteSelf(KErrDied);
        }
    }

void CClientDataBufferSource::BufferingTypesSupportedChanged()
	{
	iFixedDurationSupport = true;
	}

TInt CClientDataBufferSource::SetDRMType( TDRMType aDRMType )
    {
    TInt status(KErrNone);
    switch (aDRMType)
        {
        case ENone:
        case EOMA1:
        case EOMA2:
        case EWMDRM:
            iDRMType = aDRMType;
            break;
        default:
            status = KErrArgument;
            break;
        };
    return status;
    }

TInt CClientDataBufferSource::GetDRMType( TDRMType& aDRMType )
    {
    TInt status(KErrNone);
    aDRMType = iDRMType;
    return status;
    }

TInt CClientDataBufferSource::AppendAllowedOutputDevice( TDRMAllowedOutputDevice aOutputDevice )
    {
    TInt status(KErrAlreadyExists);
    switch (aOutputDevice)
        {
        case EAudioAllowAll:
        case EAudioAllowAnalog:
        case EAudioAllowFMTx:
        case EAudioAllowBTA2DP:
        case EAudioAllowBTHFPHSP:
        case EAudioAllowUplink:
        case EAudioAllowUSB:
        case EAudioAllowRecording:
        case EAudioAllowVisualization:
		/**
		* RIM CR 417-7642: HDMI with HDCP to Resctricted Audio Output API
		* Due to addition of new ENUMs to CRestrictedAudioOutput::TAllowedOutputPreference for HDMI and HDCP
		* EAllowAudioHDMI and EAllowAudioHdmiHdcpRequired,the same is matched by adding
		* EAudioAllowHDMI and EAudioAllowHdmiHdcpRequired
		*/
		case EAudioAllowHDMI:
		case EAudioAllowHdmiHdcpRequired:
            if ( iAllowedOutputDeviceList.Find( aOutputDevice ) == KErrNotFound )
                {
                status = iAllowedOutputDeviceList.Append( aOutputDevice );
                }
            break;
        default:
            status = KErrArgument;
            break;
        };
    return status;
    }

TInt CClientDataBufferSource::RemoveAllowedOutputDevice( TDRMAllowedOutputDevice aOutputDevice )
    {
    TInt status = iAllowedOutputDeviceList.Find( aOutputDevice );
    if ( status == KErrNotFound )
        {
        iAllowedOutputDeviceList.Remove(status);
        status = KErrNone;
        }
    return status;
    }

TInt CClientDataBufferSource::GetAllowedOutputDeviceCount( TInt& aCount )
    {
    TInt status(KErrNone);
    aCount = iAllowedOutputDeviceList.Count();
    return status;
    }

TInt CClientDataBufferSource::GetAllowedOutputDevice( TInt aIndex, TDRMAllowedOutputDevice& aOutputDevice )
    {
    TInt status(KErrArgument);
    if ( aIndex <= iAllowedOutputDeviceList.Count() )
        {
        aOutputDevice = iAllowedOutputDeviceList[aIndex];
        status = KErrNone;
        }
    return status;
    }

TInt CClientDataBufferSource::ResetDRMConfig()
    {
    iDRMType = ENone;
    iAllowedOutputDeviceList.Reset();
    return KErrNone;
    }

TInt CClientDataBufferSource::CommitDRMConfig()
    {
    TInt status(KErrNone);
    TRAP(status, DoCommitDRMConfigL());
    return status;
    }

void CClientDataBufferSource::EmptyQueue()
    {
    CDataBufferQueueItem* bufferItem;
    while ( !iAudioBufferQueue->IsEmpty() )
        {
        bufferItem = iAudioBufferQueue->First();
        iAudioBufferQueue->Remove(*bufferItem);
        delete bufferItem;
        }
    }

void CClientDataBufferSource::DoCommitDRMConfigL()
    {
    if ( iServerSourceExists )
        {
        // Package DRM Info into a descriptor and send it to server side plugin
        TInt descSize = sizeof(TDRMType); // For iDRMType
        descSize += sizeof(TInt); // For array size
        descSize += (iAllowedOutputDeviceList.Count() * sizeof(TDRMAllowedOutputDevice)); // For array

        HBufC8* buf(NULL);
        buf = HBufC8::NewL(descSize);
        CleanupStack::PushL(buf);
        
    	RDesWriteStream stream;
        TPtr8 ptr = buf->Des();
    	stream.Open(ptr);
    	CleanupClosePushL(stream);
    	
    	// Write DRM Type
    	stream.WriteInt32L(iDRMType);
    	// Write Allowed Output Device Count
    	stream.WriteInt32L(iAllowedOutputDeviceList.Count());
    	// Write Allowed Output Device enumeration
    	for (TInt i=0;i<iAllowedOutputDeviceList.Count();i++)
    	    {
    		stream.WriteInt32L(iAllowedOutputDeviceList[i]);
    	    }
    	ptr.SetLength(descSize);
        User::LeaveIfError(iCustomCommand->CustomCommandSync( iSourceHandlePckg, ESetDRMConfig, ptr, KNullDesC8 ));
        
    	// Delete objects
    	CleanupStack::PopAndDestroy(&stream);//stream
    	CleanupStack::PopAndDestroy(buf);//stream
        }
    }

// End of file
