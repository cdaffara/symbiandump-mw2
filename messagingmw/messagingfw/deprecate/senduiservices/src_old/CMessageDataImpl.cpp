/*
* Copyright (c) 2002-2006 Nokia Corporation and/or its subsidiary(-ies).
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
* Description:   Encapsulates the message data for sending services.
*
*/



// INCLUDE FILES
#include <s32file.h>
#include <txtrich.h>
#include <eikenv.h>
#include <s32mem.h>
#include <s32std.h>
#include <msvstore.h>
#include <SenduiConsts.h>

#ifdef RD_MULTIPLE_DRIVE
#include <driveinfo.h>
#endif

#include "CMessageDataImpl.h"
#include "CSendUiAttachment.h"

// CONSTANTS
const TInt KMDArrayGranularity = 1;
const TInt KMDRichTextStoreGranularity = 512;

_LIT( KSenduiTempFile, ":\\system\\temp\\sendui\\%x_%x");

// ============================ MEMBER FUNCTIONS ===============================

// -----------------------------------------------------------------------------
// CMessageDataImpl::CMessageDataImpl
// C++ default constructor can NOT contain any code, that
// might leave.
// -----------------------------------------------------------------------------
//
CMessageDataImpl::CMessageDataImpl()
    : iDataType(KSenduiMsgDataBase)
    {
    }

// -----------------------------------------------------------------------------
// CMessageDataImpl::ConstructL
// Symbian 2nd phase constructor can leave.
// -----------------------------------------------------------------------------
//
void CMessageDataImpl::ConstructL()
    {
    iAttachments = new ( ELeave ) CDesCArrayFlat( KMDArrayGranularity );
    iToAddresses = new ( ELeave ) CMessageAddressArray( KMDArrayGranularity );
    iCcAddresses = new ( ELeave ) CMessageAddressArray( KMDArrayGranularity );
    iBccAddresses = new ( ELeave ) CMessageAddressArray( KMDArrayGranularity );

    CEikonEnv* eikonEnv = CEikonEnv::Static();
    iBodyText = CRichText::NewL( eikonEnv->SystemParaFormatLayerL(),
                                eikonEnv->SystemCharFormatLayerL() );
    }

// -----------------------------------------------------------------------------
// CMessageDataImpl::NewL
// Two-phased constructor.
// -----------------------------------------------------------------------------
//
CMessageDataImpl* CMessageDataImpl::NewL()
    {
    CMessageDataImpl* self = CMessageDataImpl::NewLC();
    
    CleanupStack::Pop( self );

    return self;
    }

// ---------------------------------------------------------
// CMessageDataImpl::NewLC
// Two-phased constructor.
// ---------------------------------------------------------
//
CMessageDataImpl* CMessageDataImpl::NewLC()
    {
    CMessageDataImpl* self = new (ELeave) CMessageDataImpl;

    CleanupStack::PushL( self );
    self->ConstructL();

    return self;
    }


// Destructor
CMessageDataImpl::~CMessageDataImpl()
    {
    TInt i = 0;
    
    delete iSubject;
    delete iAttachments;
    delete iBodyText;
    delete iOpaqueData;
    
    if ( iToAddresses )
        {
        iToAddresses->ResetAndDestroy();
        delete iToAddresses;
        }

    if ( iCcAddresses )
        {
        iCcAddresses->ResetAndDestroy();
        delete iCcAddresses;
        }

    if ( iBccAddresses )
        {
        iBccAddresses->ResetAndDestroy();
        delete iBccAddresses;
        }
        
    for (i = 0; i < iAttachmentHandles.Count(); i++)
        {
        iAttachmentHandles[i].Close();  
        }
    iAttachmentHandles.Close();
    }

// -----------------------------------------------------------------------------
// CMessageDataImpl::SetSubjectL
// Set message subject.
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void CMessageDataImpl::SetSubjectL( const TDesC* aSubject )
    {
    delete iSubject;
    iSubject = NULL;

    if ( aSubject )
        {
        iSubject = aSubject->AllocL();
        }
    }

// -----------------------------------------------------------------------------
// CMessageDataImpl::Subject
// Returns message subject.
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
const TPtrC CMessageDataImpl::Subject() const
    {
    return iSubject ? TPtrC( *iSubject ) : TPtrC();
    }    

// -----------------------------------------------------------------------------
// CMessageDataImpl::AppendAttachmentL
// Appends attachment file path to the array of attachment file paths.
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void CMessageDataImpl::AppendAttachmentL( const TDesC& aFileName )
    {
    iAttachments->AppendL( aFileName );
    }
    
// -----------------------------------------------------------------------------
// CMessageDataImpl::AttachmentArray
// Returns array of attachment file paths.
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
const CDesCArray& CMessageDataImpl::AttachmentArray() const
    {
    return *iAttachments;
    }

// -----------------------------------------------------------------------------
// CMessageDataImpl::ClearAttachmentArray
// Clears the array of attachment file paths.
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void CMessageDataImpl::ClearAttachmentArray()
    {
    TInt i( iAttachmentHandles.Count() );
    iAttachments->Reset();

    while( i-- )
    	{
    	iAttachmentHandles[i].Close();
    	}
    iAttachmentHandles.Reset();
    }    

// -----------------------------------------------------------------------------
// CMessageDataImpl::AppendToAddressL
// Encapsulates address and alias to CMessageAddress and adds to array.
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void CMessageDataImpl::AppendToAddressL(
    const TDesC& aAddress,
    const TDesC& aAlias)
    {
    CMessageAddress* messageAddress = CMessageAddress::NewLC();

    messageAddress->SetAddressL( aAddress );
    messageAddress->SetAliasL( aAlias );

    iToAddresses->AppendL( messageAddress );

    CleanupStack::Pop( messageAddress );
    }

// -----------------------------------------------------------------------------
// CMessageDataImpl::ToAddressArray
// Returns array of addresses and aliases.
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
const CMessageAddressArray& CMessageDataImpl::ToAddressArray() const
    {
    return *iToAddresses;
    }

// -----------------------------------------------------------------------------
// CMessageDataImpl::AppendCcAddressL
// Encapsulates CC address and alias to CMessageAddress and adds to array.
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void CMessageDataImpl::AppendCcAddressL(
    const TDesC& aCcAddress,
    const TDesC& aCcAlias)
    {
    CMessageAddress* messageAddress = CMessageAddress::NewLC();

    messageAddress->SetAddressL( aCcAddress );
    messageAddress->SetAliasL( aCcAlias );

    iCcAddresses->AppendL( messageAddress );

    CleanupStack::Pop( messageAddress );
    }

// -----------------------------------------------------------------------------
// CMessageDataImpl::CcAddressArray
// Returns array of CC addresses and aliases.
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
const CMessageAddressArray& CMessageDataImpl::CcAddressArray() const
    {
    return *iCcAddresses;
    }

// -----------------------------------------------------------------------------
// CMessageDataImpl::AppendBccAddressL
// Encapsulates Bcc address and alias to CMessageAddress and adds to array.
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void CMessageDataImpl::AppendBccAddressL(
    const TDesC& aBccAddress,
    const TDesC& aBccAlias)
    {
    CMessageAddress* messageAddress = CMessageAddress::NewLC();

    messageAddress->SetAddressL( aBccAddress );
    messageAddress->SetAliasL( aBccAlias );

    iBccAddresses->AppendL( messageAddress );

    CleanupStack::Pop( messageAddress );
    }

// -----------------------------------------------------------------------------
// CMessageDataImpl::BccAddressArray
// Returns array of Bcc addresses and aliases.
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
const CMessageAddressArray& CMessageDataImpl::BccAddressArray() const
    {
    return *iBccAddresses;
    }

// -----------------------------------------------------------------------------
// CMessageDataImpl::SetBodyTextL
// Set message body text.
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void CMessageDataImpl::SetBodyTextL( const CRichText* aBodyText )
    {
    iBodyText->Reset();

    if ( aBodyText )
        {
        // Save and restore the rich text object
        CStreamStore* store = CBufStore::NewLC( KMDRichTextStoreGranularity ); 
        
        TStreamId id = aBodyText->StoreL( *store );
        iBodyText->RestoreL( *store, id );
        
        CleanupStack::PopAndDestroy( store );
        }
    }

// -----------------------------------------------------------------------------
// CMessageDataImpl::BodyText
// Returns message body text.
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
const CRichText* CMessageDataImpl::BodyText() const
    {
    return iBodyText;
    }
    
// -----------------------------------------------------------------------------
// CMessageDataImpl::AppendAttachmentHandleL
// Appends attachment handle to the array of attachment handles.
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void CMessageDataImpl::AppendAttachmentHandleL( const RFile& aFileHandle )
    {
    if ( !aFileHandle.SubSessionHandle() )
        {
        User::Leave( KErrBadHandle );
        }
    RFile tempHandle;
    tempHandle.Duplicate( aFileHandle );
    CleanupClosePushL( tempHandle );
    iAttachmentHandles.AppendL( tempHandle );
    CleanupStack::Pop( );
    }

// -----------------------------------------------------------------------------
// CMessageDataImpl::AttachmentHandleArray
// Returns array of attachment handles.
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
const RArray<RFile>& CMessageDataImpl::AttachmentHandleArray() const
    {
    return iAttachmentHandles;
    }

// -----------------------------------------------------------------------------
// CMessageDataImpl::DataType
// Returns data type id.
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
TUid CMessageDataImpl::DataType() const
    {
    return iDataType;
    }

// -----------------------------------------------------------------------------
// CMessageDataImpl::ClearAddresses
// Clears all addressee arrays.
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void CMessageDataImpl::ClearAddresses()
    {
    iToAddresses->ResetAndDestroy();
    iCcAddresses->ResetAndDestroy();
    iBccAddresses->ResetAndDestroy();
    }

// ---------------------------------------------------------
// CMessageDataImpl::ExternalizeL
//
// ---------------------------------------------------------
//  
void CMessageDataImpl::ExternalizeL( RWriteStream& aStream ) const
    {
    aStream.WriteUint32L( iDataType.iUid );
    
    TInt length(0);
    if ( iSubject )
        {
        length = iSubject->Length();
        }
    aStream.WriteUint32L( length);
    
    if ( length > 0)
        {
        aStream << iSubject->Des();
        }

    // Store text to a file store
    
    length = 0;
    
    if ( iBodyText->DocumentLength() > 0 )
        {
        TStreamId streamId(0);
    
        RFs& fs = CCoeEnv::Static()->FsSession();
        CFileStore* theStore;
        TParse fileStoreName;

	    HBufC* filenameBuf = HBufC::NewLC(KMaxFileName);
        TPtr fileName = filenameBuf->Des();

        CreateTempPathL( fileName, fs );
    
        length = fileName.Length();
        aStream.WriteUint32L( length );
        if (length)
            {
            aStream << fileName; // write temporary filename
            }
    
        theStore = CDirectFileStore::ReplaceLC
            ( fs, fileName, EFileRead|EFileWrite );
        theStore->SetTypeL( KDirectFileStoreLayoutUid );
        // externalize the rich text
        streamId = iBodyText->StoreL( *theStore ); // Store and get ID
        CleanupStack::PopAndDestroy( 2, filenameBuf ); // pop and destroy store, filenameBuf
        aStream << streamId;
        }
    else
        {
        aStream.WriteUint32L( 0 ); // no file storage
        }

    TUint32 count = iAttachments->Count();
    aStream << count;       // compressed value
    for ( TInt ii = 0; ii < count; ++ii )
        {
        TPtrC16 text = iAttachments->MdcaPoint(ii);
                
        aStream.WriteUint32L( text.Length() );
        aStream << text ;
        }
    
    ExternalizeArrayL( iToAddresses, aStream );
    ExternalizeArrayL( iBccAddresses, aStream );
    ExternalizeArrayL( iCcAddresses, aStream );

    // Opaque data
    length = 0;
    if ( iOpaqueData )
        {
        length = iOpaqueData->Length();
        }
    aStream.WriteUint32L( length );
    if ( length > 0 )
        {
        aStream << iOpaqueData->Des();
        }
    }

// ---------------------------------------------------------
// CMessageDataImpl::InternalizeL
//
// ---------------------------------------------------------
//    
void CMessageDataImpl::InternalizeL( RReadStream& aStream )
    {
    TUint32 tmpVal; 
    tmpVal  = aStream.ReadUint32L( ); 
    iDataType = TUid::Uid( tmpVal );
    
    tmpVal  = aStream.ReadInt32L( );
    if ( tmpVal )
        {
        iSubject = HBufC::NewL( aStream, tmpVal );
        }

    // something in filestorage
    tmpVal  = aStream.ReadInt32L( );
    if (tmpVal)
        {
        HBufC* fileName = HBufC::NewLC( aStream, tmpVal );

        TStreamId streamId;
        aStream >> streamId;

        // Restore text from file store
        RFs& fs = CCoeEnv::Static()->FsSession();
        CFileStore* theStore;
        
        theStore = CDirectFileStore::OpenLC(
            fs, 
            fileName->Des(),
            EFileRead|EFileShareReadersOnly );
            
        if (theStore->Type()[0]!= KDirectFileStoreLayoutUid )
            {
            User::Leave( KErrUnknown );
            }
        // internalize from the store
        iBodyText->RestoreL( *theStore, streamId );
        CleanupStack::PopAndDestroy( theStore ); // store
        fs.Delete( *fileName );
        CleanupStack::PopAndDestroy( fileName );
        }
    
    
    // Filepath attachments
    TInt count = aStream.ReadUint32L( );
    HBufC16* buf;
    for ( TInt ii = 0; ii < count; ++ii )
        {
        tmpVal  = aStream.ReadUint32L( ); // string length
        buf = HBufC16::NewLC( aStream, tmpVal );
        iAttachments->AppendL( *buf );
        CleanupStack::PopAndDestroy( buf );
        }
    
    InternalizeArrayL( iToAddresses, aStream );
    InternalizeArrayL( iBccAddresses, aStream );
    InternalizeArrayL( iCcAddresses, aStream );

    tmpVal  = aStream.ReadInt32L( );
    if ( tmpVal )
        {
        iOpaqueData = HBufC8::NewL( aStream, tmpVal );
        }
    }
    
// ---------------------------------------------------------
// CMessageDataImpl::ExternalizeArrayL
//
// ---------------------------------------------------------
//
void CMessageDataImpl::ExternalizeArrayL(
    CMessageAddressArray* anArray,
    RWriteStream& aStream ) const
    {

    TInt count = anArray->Count();
    aStream.WriteUint32L( count );       // compressed value
    for ( TInt ii = 0; ii < count; ++ii )
        {
        anArray->At(ii)->ExternalizeL( aStream );
        }
    }
    
// ---------------------------------------------------------
// CMessageDataImpl::InternalizeArrayL
//
// ---------------------------------------------------------
//    
void CMessageDataImpl::InternalizeArrayL(
    CMessageAddressArray* anArray,
    RReadStream& aStream )
    {
    TInt count;
    count = aStream.ReadUint32L();
    anArray->Reset();
    for ( TInt ii = 0; ii < count; ++ii )
        {
        CMessageAddress* messageAddress = CMessageAddress::NewLC();
        messageAddress->InternalizeL( aStream );
        anArray->AppendL( messageAddress );
        CleanupStack::Pop( messageAddress );
        }
    }

// ---------------------------------------------------------
// CMessageDataImpl::CreateTempPathL
//
// ---------------------------------------------------------
//    
void CMessageDataImpl::CreateTempPathL( TDes16& aPath, RFs& aFs ) const
    {
    aPath.Zero();

    TInt drive = EDriveD;
#ifdef RD_MULTIPLE_DRIVE
    User::LeaveIfError(
        DriveInfo::GetDefaultDrive( DriveInfo::EDefaultRam, drive ) );
#endif
    TChar driveChar;
    aFs.DriveToChar( drive, driveChar );
    
    aPath.Append( driveChar );
    aPath.Append( KSenduiTempFile );

    TTime time;
    time.UniversalTime();

    TFileName* tempDir = new (ELeave) TFileName();
    tempDir->Format(
        aPath ,
        I64HIGH( time.Int64() ),
        I64LOW( time.Int64() ) );
    aPath = *tempDir;
    delete tempDir;

    TInt err = aFs.MkDirAll( aPath );
    if ( err && err != KErrAlreadyExists )
        {
        User::Leave( err );
        }
    }

// ---------------------------------------------------------
// CMessageDataImpl::SetOpaqueDataL
//
// ---------------------------------------------------------
//    
void CMessageDataImpl::SetOpaqueDataL( const TDesC8* aData )
    {
    delete iOpaqueData;
    iOpaqueData = NULL;
    if ( aData )
        {
        iOpaqueData = aData->AllocL();
        }
    }
        
// ---------------------------------------------------------
// CMessageDataImpl::OpaqueData
//
// ---------------------------------------------------------
//    
const TPtrC8 CMessageDataImpl::OpaqueData() const
    {
    return iOpaqueData ? TPtrC8( *iOpaqueData ) : TPtrC8();
    }



//  End of File
