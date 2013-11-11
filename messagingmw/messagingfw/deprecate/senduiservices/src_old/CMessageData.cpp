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
#include <e32std.h>
#include <SenduiConsts.h>
#include <CMessageData.h>
#include "CMessageDataImpl.h"
#include "CSendUiAttachment.h"

// CONSTANTS

// ============================ MEMBER FUNCTIONS ===============================

// -----------------------------------------------------------------------------
// CMessageData::CMessageData
// C++ default constructor can NOT contain any code, that
// might leave.
// -----------------------------------------------------------------------------
//
EXPORT_C CMessageData::CMessageData()
    : iDataType(KSenduiMsgDataBase)
    {
    }

// -----------------------------------------------------------------------------
// CMessageData::ConstructL
// Symbian 2nd phase constructor can leave.
// -----------------------------------------------------------------------------
//
EXPORT_C void CMessageData::ConstructL()
    {
    iMessageDataImpl = CMessageDataImpl::NewL();
    }

// -----------------------------------------------------------------------------
// CMessageData::NewL
// Two-phased constructor.
// -----------------------------------------------------------------------------
//
EXPORT_C CMessageData* CMessageData::NewL()
    {
    CMessageData* self = CMessageData::NewLC();
    
    CleanupStack::Pop( self );

    return self;
    }

// ---------------------------------------------------------
// CMessageData::NewLC
// Two-phased constructor.
// ---------------------------------------------------------
//
EXPORT_C CMessageData* CMessageData::NewLC()
    {
    CMessageData* self = new (ELeave) CMessageData;

    CleanupStack::PushL( self );
    self->ConstructL();

    return self;
    }


// Destructor
EXPORT_C CMessageData::~CMessageData()
    {
    delete iMessageDataImpl;
    }

// -----------------------------------------------------------------------------
// CMessageData::SetSubjectL
// Set message subject.
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
EXPORT_C void CMessageData::SetSubjectL( const TDesC* aSubject )
    {
    iMessageDataImpl->SetSubjectL( aSubject );
    }

// -----------------------------------------------------------------------------
// CMessageData::Subject
// Returns message subject.
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
EXPORT_C const TPtrC CMessageData::Subject() const
    {
    return iMessageDataImpl->Subject();
    }    

// -----------------------------------------------------------------------------
// CMessageData::AppendAttachmentL
// Appends attachment file path to the array of attachment file paths.
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
EXPORT_C void CMessageData::AppendAttachmentL( const TDesC& aFileName )
    {
    iMessageDataImpl->AppendAttachmentL( aFileName );
    }
    
// -----------------------------------------------------------------------------
// CMessageData::AttachmentArray
// Returns array of attachment file paths.
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
EXPORT_C const CDesCArray& CMessageData::AttachmentArray() const
    {
    return iMessageDataImpl->AttachmentArray();
    }

// -----------------------------------------------------------------------------
// CMessageData::ClearAttachmentArray
// Clears the array of attachment file paths.
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
EXPORT_C void CMessageData::ClearAttachmentArray()
    {
    iMessageDataImpl->ClearAttachmentArray();
    }    

// -----------------------------------------------------------------------------
// CMessageData::AppendToAddressL
// Encapsulates address and alias to CMessageAddress and adds to array.
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
EXPORT_C void CMessageData::AppendToAddressL(
    const TDesC& aAddress,
    const TDesC& aAlias)
    {
    iMessageDataImpl->AppendToAddressL( aAddress, aAlias );
    }

// -----------------------------------------------------------------------------
// CMessageData::ToAddressArray
// Returns array of addresses and aliases.
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
EXPORT_C const CMessageAddressArray& CMessageData::ToAddressArray() const
    {
    return iMessageDataImpl->ToAddressArray();
    }

// -----------------------------------------------------------------------------
// CMessageData::AppendCcAddressL
// Encapsulates CC address and alias to CMessageAddress and adds to array.
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
EXPORT_C void CMessageData::AppendCcAddressL(
    const TDesC& aCcAddress,
    const TDesC& aCcAlias)
    {
    iMessageDataImpl->AppendCcAddressL( aCcAddress, aCcAlias );
    }

// -----------------------------------------------------------------------------
// CMessageData::CcAddressArray
// Returns array of CC addresses and aliases.
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
EXPORT_C const CMessageAddressArray& CMessageData::CcAddressArray() const
    {
    return iMessageDataImpl->CcAddressArray();
    }

// -----------------------------------------------------------------------------
// CMessageData::AppendBccAddressL
// Encapsulates Bcc address and alias to CMessageAddress and adds to array.
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
EXPORT_C void CMessageData::AppendBccAddressL(
    const TDesC& aBccAddress,
    const TDesC& aBccAlias)
    {
    iMessageDataImpl->AppendBccAddressL( aBccAddress, aBccAlias );
    }

// -----------------------------------------------------------------------------
// CMessageData::BccAddressArray
// Returns array of Bcc addresses and aliases.
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
EXPORT_C const CMessageAddressArray& CMessageData::BccAddressArray() const
    {
    return iMessageDataImpl->BccAddressArray();
    }

// -----------------------------------------------------------------------------
// CMessageData::SetBodyTextL
// Set message body text.
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
EXPORT_C void CMessageData::SetBodyTextL( const CRichText* aBodyText )
    {
    iMessageDataImpl->SetBodyTextL( aBodyText );
    }

// -----------------------------------------------------------------------------
// CMessageData::BodyText
// Returns message body text.
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
EXPORT_C const CRichText* CMessageData::BodyText() const
    {
    return iMessageDataImpl->BodyText();
    }
    
// -----------------------------------------------------------------------------
// CMessageData::AppendAttachmentHandleL
// Appends attachment handle to the array of attachment handles.
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
EXPORT_C void CMessageData::AppendAttachmentHandleL( const RFile& aFileHandle )
    {
    iMessageDataImpl->AppendAttachmentHandleL( aFileHandle );
    }

// -----------------------------------------------------------------------------
// CMessageData::AttachmentHandleArray
// Returns array of attachment handles.
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
EXPORT_C const RArray<RFile>& CMessageData::AttachmentHandleArray() const
    {
    return iMessageDataImpl->AttachmentHandleArray();
    }

// -----------------------------------------------------------------------------
// CMessageData::DataType
// Returns data type id.
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
EXPORT_C TUid CMessageData::DataType() const
    {
    return iDataType;
    }

// -----------------------------------------------------------------------------
// CMessageData::ClearAddresses
// Clears all addressee arrays.
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
EXPORT_C void CMessageData::ClearAddresses()
    {
    iMessageDataImpl->ClearAddresses();
    }

// ---------------------------------------------------------
// CMessageData::ExternalizeL
//
// ---------------------------------------------------------
//  
EXPORT_C void CMessageData::ExternalizeL( RWriteStream& aStream ) const
    {
    iMessageDataImpl->ExternalizeL( aStream );
    }

// ---------------------------------------------------------
// CMessageData::InternalizeL
//
// ---------------------------------------------------------
//    
EXPORT_C void CMessageData::InternalizeL( RReadStream& aStream )
    {
    iMessageDataImpl->InternalizeL( aStream );
    }

// ---------------------------------------------------------
// CMessageData::SetOpaqueDataL
//
// ---------------------------------------------------------
//  
EXPORT_C void CMessageData::SetOpaqueDataL( const TDesC8* aData, TUid aDataType )
    {
    iMessageDataImpl->SetOpaqueDataL( aData );
    iDataType = aDataType;
    }
    
// ---------------------------------------------------------
// CMessageData::OpaqueData
//
// ---------------------------------------------------------
//  
EXPORT_C const TPtrC8 CMessageData::OpaqueData() const
    {
    return iMessageDataImpl->OpaqueData();
    }


//  End of File
