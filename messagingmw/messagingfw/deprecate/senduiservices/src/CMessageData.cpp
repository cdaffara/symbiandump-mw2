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
#include <SendUiConsts.h>
#include <CMessageData.h>
#include "CMessageDataImpl.h"
//#include "CSendUiAttachment.h"

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
    }

// ---------------------------------------------------------
// CMessageData::NewLC
// Two-phased constructor.
// ---------------------------------------------------------
//
EXPORT_C CMessageData* CMessageData::NewLC()
    {
    }


// Destructor
EXPORT_C CMessageData::~CMessageData()
    {
    }

// -----------------------------------------------------------------------------
// CMessageData::SetSubjectL
// Set message subject.
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
EXPORT_C void CMessageData::SetSubjectL( const TDesC* aSubject )
    {
    }

// -----------------------------------------------------------------------------
// CMessageData::Subject
// Returns message subject.
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
EXPORT_C const TPtrC CMessageData::Subject() const
    {
    }    

// -----------------------------------------------------------------------------
// CMessageData::AppendAttachmentL
// Appends attachment file path to the array of attachment file paths.
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
EXPORT_C void CMessageData::AppendAttachmentL( const TDesC& aFileName )
    {
    }
    
// -----------------------------------------------------------------------------
// CMessageData::AttachmentArray
// Returns array of attachment file paths.
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
EXPORT_C const CDesCArray& CMessageData::AttachmentArray() const
    {
    }

// -----------------------------------------------------------------------------
// CMessageData::ClearAttachmentArray
// Clears the array of attachment file paths.
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
EXPORT_C void CMessageData::ClearAttachmentArray()
    {
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
    }

// -----------------------------------------------------------------------------
// CMessageData::ToAddressArray
// Returns array of addresses and aliases.
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
EXPORT_C const CMessageAddressArray& CMessageData::ToAddressArray() const
    {
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
    }

// -----------------------------------------------------------------------------
// CMessageData::CcAddressArray
// Returns array of CC addresses and aliases.
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
EXPORT_C const CMessageAddressArray& CMessageData::CcAddressArray() const
    {
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
    }

// -----------------------------------------------------------------------------
// CMessageData::BccAddressArray
// Returns array of Bcc addresses and aliases.
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
EXPORT_C const CMessageAddressArray& CMessageData::BccAddressArray() const
    {
    }

// -----------------------------------------------------------------------------
// CMessageData::SetBodyTextL
// Set message body text.
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
EXPORT_C void CMessageData::SetBodyTextL( const CRichText* aBodyText )
    {
    }

// -----------------------------------------------------------------------------
// CMessageData::BodyText
// Returns message body text.
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
EXPORT_C const CRichText* CMessageData::BodyText() const
    {
    }
    
// -----------------------------------------------------------------------------
// CMessageData::AppendAttachmentHandleL
// Appends attachment handle to the array of attachment handles.
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
EXPORT_C void CMessageData::AppendAttachmentHandleL( const RFile& aFileHandle )
    {
    }

// -----------------------------------------------------------------------------
// CMessageData::AttachmentHandleArray
// Returns array of attachment handles.
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
EXPORT_C const RArray<RFile>& CMessageData::AttachmentHandleArray() const
    {
    }

// -----------------------------------------------------------------------------
// CMessageData::DataType
// Returns data type id.
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
EXPORT_C TUid CMessageData::DataType() const
    {
    }

// -----------------------------------------------------------------------------
// CMessageData::ClearAddresses
// Clears all addressee arrays.
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
EXPORT_C void CMessageData::ClearAddresses()
    {
    }

// ---------------------------------------------------------
// CMessageData::ExternalizeL
//
// ---------------------------------------------------------
//  
EXPORT_C void CMessageData::ExternalizeL( RWriteStream& aStream ) const
    {
    }

// ---------------------------------------------------------
// CMessageData::InternalizeL
//
// ---------------------------------------------------------
//    
EXPORT_C void CMessageData::InternalizeL( RReadStream& aStream )
    {
    }

// ---------------------------------------------------------
// CMessageData::SetOpaqueDataL
//
// ---------------------------------------------------------
//  
EXPORT_C void CMessageData::SetOpaqueDataL( const TDesC8* aData, TUid aDataType )
    {
    }
    
// ---------------------------------------------------------
// CMessageData::OpaqueData
//
// ---------------------------------------------------------
//  
EXPORT_C const TPtrC8 CMessageData::OpaqueData() const
    {
    }


//  End of File
