/*
* Copyright (c) 2004 Nokia Corporation and/or its subsidiary(-ies).
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
* Description:  This class is the main interface to MetaDataUtility.
*
*/



// INCLUDE FILES
#include	<MetaDataUtility.h>
#include	"MetaDataUtilityBody.h"
#ifdef _DEBUG
#include	<e32svr.h>
#endif

// ============================ MEMBER FUNCTIONS ===============================

// -----------------------------------------------------------------------------
// CMetaDataUtility::CMetaDataUtility
// C++ default constructor can NOT contain any code, that
// might leave.
// -----------------------------------------------------------------------------
//
CMetaDataUtility::CMetaDataUtility()
	:	iBody(NULL)
    {
    }

// -----------------------------------------------------------------------------
// CMetaDataUtility::ConstructL
// Symbian 2nd phase constructor can leave.
// -----------------------------------------------------------------------------
//
void CMetaDataUtility::ConstructL()
    {
	iBody = CMetaDataUtilityBody::NewL();
#ifdef _DEBUG
	RDebug::Print(_L("CMetaDataUtility::ConstructL - Done!"));
#endif
    }

// -----------------------------------------------------------------------------
// CMetaDataUtility::NewL
// Two-phased constructor.
// -----------------------------------------------------------------------------
//
EXPORT_C CMetaDataUtility* CMetaDataUtility::NewL()
    {
#ifdef _DEBUG
	RDebug::Print(_L("CMetaDataUtility::NewL"));
#endif
	CMetaDataUtility* self = new( ELeave ) CMetaDataUtility;
    CleanupStack::PushL( self );
    self->ConstructL();
    CleanupStack::Pop();
    return self;
    }

// Destructor
EXPORT_C CMetaDataUtility::~CMetaDataUtility()
	{
	delete iBody;
	}

// -----------------------------------------------------------------------------
// CMetaDataUtility::OpenFileL
// -----------------------------------------------------------------------------
//
EXPORT_C void CMetaDataUtility::OpenFileL(
	const TDesC& aFileName )
	{
	RArray<TMetaDataFieldId> wantedFields;
	CleanupClosePushL(wantedFields);
	iBody->OpenFileL(aFileName, wantedFields);
	CleanupStack::PopAndDestroy();
	}

// -----------------------------------------------------------------------------
// CMetaDataUtility::OpenFileL
// -----------------------------------------------------------------------------
//
EXPORT_C void CMetaDataUtility::OpenFileL(
	const TDesC& aFileName,
	const RArray<TMetaDataFieldId>& aWantedFields )
	{
	iBody->OpenFileL(aFileName, aWantedFields);
	}

// -----------------------------------------------------------------------------
// CMetaDataUtility::OpenFileL
// -----------------------------------------------------------------------------
//
EXPORT_C void CMetaDataUtility::OpenFileL(
	RFile& aFile )
	{
	RArray<TMetaDataFieldId> wantedFields;
	CleanupClosePushL(wantedFields);
	iBody->OpenFileL(aFile, wantedFields);
	CleanupStack::PopAndDestroy();
	}

// -----------------------------------------------------------------------------
// CMetaDataUtility::OpenFileL
// -----------------------------------------------------------------------------
//
EXPORT_C void CMetaDataUtility::OpenFileL(
	RFile& aFile,
	const RArray<TMetaDataFieldId>& aWantedFields )
	{
	iBody->OpenFileL(aFile, aWantedFields);
	}

// -----------------------------------------------------------------------------
// CMetaDataUtility::OpenDesL
// -----------------------------------------------------------------------------
//
EXPORT_C void CMetaDataUtility::OpenDesL(
	const TDesC8& aDes )
	{
	RArray<TMetaDataFieldId> wantedFields;
	CleanupClosePushL(wantedFields);
	iBody->OpenDesL(aDes, wantedFields);
	CleanupStack::PopAndDestroy();
	}

// -----------------------------------------------------------------------------
// CMetaDataUtility::OpenDesL
// -----------------------------------------------------------------------------
//
EXPORT_C void CMetaDataUtility::OpenDesL(
	const TDesC8& aDes,
	const RArray<TMetaDataFieldId>& aWantedFields )
	{
	iBody->OpenDesL(aDes, aWantedFields);
	}
// -----------------------------------------------------------------------------
// CMetaDataUtility::OpenFileL
// -----------------------------------------------------------------------------
//
EXPORT_C void CMetaDataUtility::OpenFileL(
	const TDesC& aFileName,
	const TDesC8& aMimeType )
	{
	#ifdef _DEBUG
	RDebug::Print(_L("CMetaDataUtility::OpenFileL with MimeType"));
	#endif
	RArray<TMetaDataFieldId> wantedFields;
	CleanupClosePushL(wantedFields);
	iBody->OpenFileL(aFileName, wantedFields, aMimeType);
	CleanupStack::PopAndDestroy();
	}

// -----------------------------------------------------------------------------
// CMetaDataUtility::OpenFileL
// -----------------------------------------------------------------------------
//
EXPORT_C void CMetaDataUtility::OpenFileL(
	const TDesC& aFileName,
	const RArray<TMetaDataFieldId>& aWantedFields,
	const TDesC8& aMimeType )
	{
	#ifdef _DEBUG
		RDebug::Print(_L("CMetaDataUtility::OpenFileL with MimeType"));
	#endif
		iBody->OpenFileL(aFileName, aWantedFields, aMimeType);
	}

// -----------------------------------------------------------------------------
// CMetaDataUtility::OpenFileL
// -----------------------------------------------------------------------------
//
EXPORT_C void CMetaDataUtility::OpenFileL(
	RFile& aFile,
	const TDesC8& aMimeType )
	{
	#ifdef _DEBUG
		RDebug::Print(_L("CMetaDataUtility::OpenFileL with MimeType"));
	#endif
		RArray<TMetaDataFieldId> wantedFields;
		CleanupClosePushL(wantedFields);
		iBody->OpenFileL(aFile, wantedFields, aMimeType);
		CleanupStack::PopAndDestroy();
	}

// -----------------------------------------------------------------------------
// CMetaDataUtility::OpenFileL
// -----------------------------------------------------------------------------
//
EXPORT_C void CMetaDataUtility::OpenFileL(
	RFile& aFile,
	const RArray<TMetaDataFieldId>& aWantedFields,
	const TDesC8& aMimeType )
	{
	#ifdef _DEBUG
		RDebug::Print(_L("CMetaDataUtility::OpenFileL with MimeType"));
	#endif
		iBody->OpenFileL(aFile, aWantedFields, aMimeType);
	}

// -----------------------------------------------------------------------------
// CMetaDataUtility::OpenDesL
// -----------------------------------------------------------------------------
//
EXPORT_C void CMetaDataUtility::OpenDesL(
	const TDesC8& aDes,
	const TDesC8& aMimeType )
	{
	#ifdef _DEBUG
		RDebug::Print(_L("CMetaDataUtility::OpenFileL with MimeType"));
	#endif
		RArray<TMetaDataFieldId> wantedFields;
		CleanupClosePushL(wantedFields);
		iBody->OpenDesL(aDes, wantedFields, aMimeType);
		CleanupStack::PopAndDestroy();
	}

// -----------------------------------------------------------------------------
// CMetaDataUtility::OpenDesL
// -----------------------------------------------------------------------------
//
EXPORT_C void CMetaDataUtility::OpenDesL(
	const TDesC8& aDes,
	const RArray<TMetaDataFieldId>& aWantedFields,
	const TDesC8& aMimeType )
	{
	#ifdef _DEBUG
		RDebug::Print(_L("CMetaDataUtility::OpenFileL with MimeType"));
	#endif
		iBody->OpenDesL(aDes, aWantedFields, aMimeType);
	}

// -----------------------------------------------------------------------------
// CMetaDataUtility::MetaDataCount
// -----------------------------------------------------------------------------
//
EXPORT_C TInt CMetaDataUtility::MetaDataCount() const
   	{
	return iBody->MetaDataCount();
    }

// -----------------------------------------------------------------------------
// CMetaDataUtility::MetaDataFieldsL
// -----------------------------------------------------------------------------
//
EXPORT_C const CMetaDataFieldContainer& CMetaDataUtility::MetaDataFieldsL()
   	{
	return iBody->MetaDataFieldsL();
	}

// -----------------------------------------------------------------------------
// CMetaDataUtility::ResetL
// -----------------------------------------------------------------------------
//
EXPORT_C void CMetaDataUtility::ResetL()
   	{
	iBody->ResetL();
	}
	
// -----------------------------------------------------------------------------
// CMetaDataUtility::ID3Version
// -----------------------------------------------------------------------------
//	
EXPORT_C TID3Version CMetaDataUtility::ID3Version()
    {
	return iBody->ID3Version();
    }

// -----------------------------------------------------------------------------
// CMetaDataUtility::InitChunkData
// -----------------------------------------------------------------------------
//	
EXPORT_C TInt CMetaDataUtility::InitChunkData(
        const TDesC8& aMimeType, 
        MMDUChunkDataObserver& aObserver )
    {
	return iBody->InitChunkData( (TDesC8&) aMimeType, aObserver );
    }

// -----------------------------------------------------------------------------
// CMetaDataUtility::ProcessChunkData
// -----------------------------------------------------------------------------
//	
EXPORT_C TInt CMetaDataUtility::ProcessChunkData( 
        const TDesC8& aChunk,
        TBool aFinalChunk )
    {
	return iBody->ProcessChunkData( (TDesC8&) aChunk, aFinalChunk );
    }

// -----------------------------------------------------------------------------
// CMetaDataUtility::ParseChunkData
// -----------------------------------------------------------------------------
//  
EXPORT_C TInt CMetaDataUtility::ParseChunkData()
    {
    return iBody->ParseChunkData();
    }

// -----------------------------------------------------------------------------
// CMetaDataUtility::IsChunkDataMode
// -----------------------------------------------------------------------------
//	
EXPORT_C TBool CMetaDataUtility::IsChunkDataMode()
    {
	return iBody->IsChunkDataMode();
    }

// -----------------------------------------------------------------------------
// CMetaDataUtility::CloseChunkData
// -----------------------------------------------------------------------------
//	
EXPORT_C void CMetaDataUtility::CloseChunkData()
    {
	return iBody->CloseChunkData();
    }



//  End of File
