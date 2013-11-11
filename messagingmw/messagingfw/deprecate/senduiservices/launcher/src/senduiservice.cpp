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
* Description:   SendUi service class. Receives message data from the
*                SendUi host.
*
*/




#include <eikenv.h>
#include <s32mem.h>

#include <CMessageData.h>
#include <SendUiConsts.h>
#include <CSendingService.h>

#include "senduiservice.h"
#include "senduilauncherappui.h"
#include "senduiserviceslog.h"
#include "SendUiPrivateCRKeys.h"
#include "senduiservicecommands.h"
#include "senduilauncher.h"
#include <senduisingleton.h>

typedef TBuf8<KTransferBufferSize> TTransferBuf;
// ---------------------------------------------------------------------------
// ConstructL
// ---------------------------------------------------------------------------
//
void CSendUiService::ConstructL()
    {
    iAppUi = (CSendUiLauncherAppUi*)CEikonEnv::Static()->EikAppUi();
    }

// ---------------------------------------------------------------------------
// NewLC
// ---------------------------------------------------------------------------
//
CSendUiService* CSendUiService::NewL()
    {
    CSendUiService* self = new( ELeave ) CSendUiService;
    CleanupStack::PushL( self );
    self->ConstructL();

    CleanupStack::Pop( self );
    return self;
    }


// ---------------------------------------------------------------------------
// Destructor
// ---------------------------------------------------------------------------
//
CSendUiService::~CSendUiService() 
    {
    LOGTEXT(_L("CSendUiService->~ >>"));
    delete iMessageData;
    delete iTransferBuffer;
    delete iReadStream;
    LOGTEXT(_L("CSendUiService->~ <<"));
    }

// ---------------------------------------------------------------------------
// ServiceL
// Receives message from SendUi and passes data to AppUi
// ---------------------------------------------------------------------------
//
void CSendUiService::ServiceL( const RMessage2& aMessage )
    {
    LOGTEXT(_L("CSendUiService->ServiceL >>"));

    TTransferBuf transferBuffer;
    
    switch (aMessage.Function())
        {
        case ESendUiDataBlock: // receive main message data in blocks
            {
            TPckgBuf<TTransferBuf> bufPckg;
            TPckgBuf<TInt> blockSizePckg;
            TPckgBuf<TInt> totalSizePckg;
            
            aMessage.ReadL( ESlot1, bufPckg );
            aMessage.ReadL( ESlot2, blockSizePckg );
            aMessage.ReadL( ESlot3, totalSizePckg );
            
            TInt blockSize = blockSizePckg();
            TInt totalSize = totalSizePckg();
            
            transferBuffer = bufPckg();
            
            if (!iTransferBuffer)
                {
                iTransferredBytes = 0;
                iTransferBuffer = HBufC8::NewL( totalSize );
                }
            TPtr8 bufferPtr8( iTransferBuffer->Des() ); 
            bufferPtr8.Insert( iTransferredBytes, transferBuffer );
            
            iTransferredBytes += blockSize;
            break;
            }
        case EDataSent: // this is the last data block, start reading
            {

            iReadStream = new(ELeave)RDesReadStream( *iTransferBuffer );
            TUid serviceProviderUid(KNullUid);
            TUid dataTypeUid(KNullUid);
            
            if ( iTransferBuffer ) 
                {
                LOGTEXT(_L("CSendUiService->ServiceL: ELaunchSendUi: openstream"));
                
                TUint32 tmpVal = iReadStream->ReadUint32L();
                serviceProviderUid = TUid::Uid( tmpVal );
                
                tmpVal = iReadStream->ReadUint32L();
                dataTypeUid = TUid::Uid( tmpVal );
                }
            iMessageData = CMessageData::NewL();
            break;
            }
        case ECopyFileHandle: // receive file handles
            {
            LOGTEXT(_L("CSendUiService->ServiceL: ECopyFileHandle"));

            // open source file handle
            RFile sourceFile;
            User::LeaveIfError(sourceFile.AdoptFromClient(aMessage, 0, 1));
            CleanupClosePushL(sourceFile);
            iMessageData->AppendAttachmentHandleL( sourceFile );
            CleanupStack::Pop(&sourceFile);

            LOGTEXT(_L("CSendUiService->ServiceL: ECopyFileHandle done"));
            break;
            }       
        case ELaunchSendUi: // construct message data and invoke sendui
            {
            LOGTEXT(_L("CSendUiService->ServiceL: ELaunchSendUi>>"));

            if ( iReadStream ) 
                {
                LOGTEXT(_L("CSendUiService->ServiceL: ELaunchSendUi: openstream"));
                
                
                TUint32 tmpVal = iReadStream->ReadUint32L();
                TUid serviceUid( TUid::Uid( tmpVal ));
                
                tmpVal = iReadStream->ReadUint32L();
                TBool launchEmbedded( tmpVal );
            
                tmpVal = iReadStream->ReadUint32L();
            
                if (tmpVal )
                    {
                    iMessageData->InternalizeL( *iReadStream );
                    }
                
                iReadStream->Release();
                iReadStream->Close();
            
                iAppUi->CreateAndSendMessageL(
                    serviceUid,
                    iMessageData,
                    launchEmbedded );
                }
                iAppUi->DoDelayedExitL( 0 );    
                
                LOGTEXT(_L("CSendUiService->ServiceL: complete message"));
                
            break;
            }
        default:
        // No other commands exist.
        LOGTEXT(_L("CSendUiService->ServiceL: ERROR! Unknown command"));
        }
        aMessage.Complete( KErrNone );
        
};

//  End of File

