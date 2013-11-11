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
#include <SendUiConsts.h>

#ifdef RD_MULTIPLE_DRIVE
#include <driveinfo.h>
#endif

#include "CMessageDataImpl.h"
//#include "CSendUiAttachment.h"

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
    }

// -----------------------------------------------------------------------------
// CMessageDataImpl::NewL
// Two-phased constructor.
// -----------------------------------------------------------------------------
//
CMessageDataImpl* CMessageDataImpl::NewL()
    {
    }

// ---------------------------------------------------------
// CMessageDataImpl::NewLC
// Two-phased constructor.
// ---------------------------------------------------------
//
CMessageDataImpl* CMessageDataImpl::NewLC()
    {
    }


// Destructor
CMessageDataImpl::~CMessageDataImpl()
    {
    }

// -----------------------------------------------------------------------------
// CMessageDataImpl::SetSubjectL
// Set message subject.
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void CMessageDataImpl::SetSubjectL( const TDesC* aSubject )
    {
    }

// -----------------------------------------------------------------------------
// CMessageDataImpl::Subject
// Returns message subject.
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
const TPtrC CMessageDataImpl::Subject() const
    {
    }    

// -----------------------------------------------------------------------------
// CMessageDataImpl::AppendAttachmentL
// Appends attachment file path to the array of attachment file paths.
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void CMessageDataImpl::AppendAttachmentL( const TDesC& aFileName )
    {
    }
    
// -----------------------------------------------------------------------------
// CMessageDataImpl::AttachmentArray
// Returns array of attachment file paths.
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
const CDesCArray& CMessageDataImpl::AttachmentArray() const
    {
    }

// -----------------------------------------------------------------------------
// CMessageDataImpl::ClearAttachmentArray
// Clears the array of attachment file paths.
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void CMessageDataImpl::ClearAttachmentArray()
    {
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

    }

// -----------------------------------------------------------------------------
// CMessageDataImpl::ToAddressArray
// Returns array of addresses and aliases.
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
const CMessageAddressArray& CMessageDataImpl::ToAddressArray() const
    {
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
    }

// -----------------------------------------------------------------------------
// CMessageDataImpl::CcAddressArray
// Returns array of CC addresses and aliases.
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
const CMessageAddressArray& CMessageDataImpl::CcAddressArray() const
    {
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
    }

// -----------------------------------------------------------------------------
// CMessageDataImpl::BccAddressArray
// Returns array of Bcc addresses and aliases.
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
const CMessageAddressArray& CMessageDataImpl::BccAddressArray() const
    {
    }

// -----------------------------------------------------------------------------
// CMessageDataImpl::SetBodyTextL
// Set message body text.
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void CMessageDataImpl::SetBodyTextL( const CRichText* aBodyText )
    {
    }

// -----------------------------------------------------------------------------
// CMessageDataImpl::BodyText
// Returns message body text.
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
const CRichText* CMessageDataImpl::BodyText() const
    {
    }
    
// -----------------------------------------------------------------------------
// CMessageDataImpl::AppendAttachmentHandleL
// Appends attachment handle to the array of attachment handles.
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void CMessageDataImpl::AppendAttachmentHandleL( const RFile& aFileHandle )
    {
    }

// -----------------------------------------------------------------------------
// CMessageDataImpl::AttachmentHandleArray
// Returns array of attachment handles.
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
const RArray<RFile>& CMessageDataImpl::AttachmentHandleArray() const
    {
    }

// -----------------------------------------------------------------------------
// CMessageDataImpl::DataType
// Returns data type id.
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
TUid CMessageDataImpl::DataType() const
    {
    }

// -----------------------------------------------------------------------------
// CMessageDataImpl::ClearAddresses
// Clears all addressee arrays.
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void CMessageDataImpl::ClearAddresses()
    {
    }

// ---------------------------------------------------------
// CMessageDataImpl::ExternalizeL
//
// ---------------------------------------------------------
//  
void CMessageDataImpl::ExternalizeL( RWriteStream& aStream ) const
    {
    }

// ---------------------------------------------------------
// CMessageDataImpl::InternalizeL
//
// ---------------------------------------------------------
//    
void CMessageDataImpl::InternalizeL( RReadStream& aStream )
    {
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
    }

// ---------------------------------------------------------
// CMessageDataImpl::CreateTempPathL
//
// ---------------------------------------------------------
//    
void CMessageDataImpl::CreateTempPathL( TDes16& aPath, RFs& aFs ) const
    {
    }

// ---------------------------------------------------------
// CMessageDataImpl::SetOpaqueDataL
//
// ---------------------------------------------------------
//    
void CMessageDataImpl::SetOpaqueDataL( const TDesC8* aData )
    {
    }
        
// ---------------------------------------------------------
// CMessageDataImpl::OpaqueData
//
// ---------------------------------------------------------
//    
const TPtrC8 CMessageDataImpl::OpaqueData() const
    {
    }



//  End of File
