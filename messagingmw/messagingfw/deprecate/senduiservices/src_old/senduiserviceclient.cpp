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
* Description:   Communicates message data to SendUILauncher server application
*
*/



#include <aknappui.h>
#include <AknServerApp.h>
#include <s32mem.h>

#include <CMessageData.h>
#include <senduiconsts.h>

#include "senduiserviceclient.h"
#include "senduiserviceslog.h"
#include "senduilauncher.h"
#include "senduiservicecommands.h"

typedef TBuf8<KTransferBufferSize> TTransferBuf;
// ======== LOCAL FUNCTIONS ========

// ---------------------------------------------------------------------------
// Launch SendUi
// ---------------------------------------------------------------------------
//
void RSendUiServiceClient::LaunchSendUiL( 
    TUid& aServiceUid,
    TUid& aServiceProviderUid, 
    TBool& aLaunchEmbedded, 
    const CMessageData* aMessageData )
    {
    CBufBase* buffer = CBufFlat::NewL( KMessageBufferSize );
    CleanupStack::PushL( buffer );
    
    RBufWriteStream bufStream;
    bufStream.Open( *buffer );
    
    bufStream.WriteUint32L( aServiceProviderUid.iUid );
    TUid dataType(KSenduiMsgDataBase);
    if ( aMessageData )
        {
        dataType = aMessageData->DataType();
        }
    bufStream.WriteUint32L( dataType.iUid );
    bufStream.WriteUint32L( aServiceUid.iUid );
    bufStream.WriteUint32L( aLaunchEmbedded ? 1 : 0);
    bufStream.WriteUint32L( aMessageData ? 1 : 0);
    
    if ( aMessageData )
        {
        aMessageData->ExternalizeL( bufStream );    
        }

    bufStream.CommitL();
    bufStream.Close();


    LOGTEXT2(_L("RSendUiServiceClient->LaunchSendUiL: buffer size %d"), buffer->Size() );
    TInt totalSize = buffer->Size();
    TInt transferredBytes(0);
    TInt blockSize(0);
    
    TTransferBuf transferBuffer;
    
    while( transferredBytes < totalSize)
        {
        // determine block size
        if ( transferredBytes + KTransferBufferSize < totalSize )
            {
            blockSize = KTransferBufferSize;
            }
        else
            {
            // the last block size
            blockSize = totalSize - transferredBytes;
            }
        buffer->Read( transferredBytes, transferBuffer, blockSize );
            
        TPckgBuf<TTransferBuf> transferBufferPckg( transferBuffer );
        TPckgBuf<TInt> totalSizePckg( totalSize );
        TPckgBuf<TInt> blockSizePckg( blockSize );
            
        SendReceive( ESendUiDataBlock, 
            TIpcArgs( &transferBufferPckg, &blockSizePckg, &totalSizePckg ) );
    
        transferredBytes += blockSize;
        } // while..
    
    SendReceive( EDataSent );

    if ( aMessageData )
    {
        TInt index(0);
        TInt fileHandleCount = aMessageData->AttachmentHandleArray().Count();
        
        // Transfer filehandles separately
        while( index < fileHandleCount )
        {
            TIpcArgs ipcArgs;
            aMessageData->AttachmentHandleArray()[index++].TransferToServer(ipcArgs, 0, 1);
            SendReceive(ECopyFileHandle, ipcArgs);
        }
    }
    SendReceive( ELaunchSendUi );
  
    CleanupStack::PopAndDestroy(buffer);
}

// ---------------------------------------------------------------------------
// Return service identifier
// ---------------------------------------------------------------------------
//
TUid RSendUiServiceClient::ServiceUid() const
    {
    return KSendUiLauncherAppServiceUid;
    }

// ---------------------------------------------------------------------------
// Destructor
// ---------------------------------------------------------------------------
//
RSendUiServiceClient::~RSendUiServiceClient() 
    { 
    Close(); 
    }
