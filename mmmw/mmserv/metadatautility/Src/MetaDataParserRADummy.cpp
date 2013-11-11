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
* Description:  This class implements 
*
*/



// INCLUDE FILES
#include	"MetaDataParserRA.h"
#ifdef _DEBUG
#include	<e32svr.h>
#endif

#include <s32mem.h>
#include "MetaDataSourceFile.h"


// ============================ MEMBER FUNCTIONS ===============================

// -----------------------------------------------------------------------------
// CMetaDataParserRA::CMetaDataParserRA
// C++ default constructor can NOT contain any code, that
// might leave.
// -----------------------------------------------------------------------------
//
CMetaDataParserRA::CMetaDataParserRA(
	CMetaDataSource& aSource )
	:	iSource(aSource)
	{
    }

// -----------------------------------------------------------------------------
// CMetaDataParserRA::ConstructL
// Symbian 2nd phase constructor can leave.
// -----------------------------------------------------------------------------
//
void CMetaDataParserRA::ConstructL()
    {
#ifdef _DEBUG
	RDebug::Print(_L("CMetaDataParserRA::ConstructL - Done"));
#endif
    }

// -----------------------------------------------------------------------------
// CMetaDataParserRA::NewL
// Two-phased constructor.
// -----------------------------------------------------------------------------
//
CMetaDataParserRA* CMetaDataParserRA::NewL(
	CMetaDataSource& aSource )
    {
#ifdef _DEBUG
	RDebug::Print(_L("CMetaDataParserRA::NewL"));
#endif
	CMetaDataParserRA* self = new( ELeave ) CMetaDataParserRA(aSource);
    CleanupStack::PushL( self );
    self->ConstructL();
    CleanupStack::Pop();
    return self;
    }

// Destructor
CMetaDataParserRA::~CMetaDataParserRA()
	{

	}

// -----------------------------------------------------------------------------
// CMetaDataParserRA::ParseL
// -----------------------------------------------------------------------------
//
void CMetaDataParserRA::ParseL(
	const RArray<TMetaDataFieldId>& /*aWantedFields*/,
	CMetaDataFieldContainer& /*aContainer*/ )
    {
#ifdef _DEBUG
	RDebug::Print(_L("CMetaDataParserRA::ParseL"));
#endif
    }

// -----------------------------------------------------------------------------
// CMetaDataParserRA::ValidateL
// -----------------------------------------------------------------------------
//
TBool CMetaDataParserRA::ValidateL()
	{	
	return EFalse;
	}

// -----------------------------------------------------------------------------
// CMetaDataParserRA::OpenFileL
// 
// -----------------------------------------------------------------------------
//
void CMetaDataParserRA::OpenFileL(RFile& /*aFile*/)
    {
#ifdef _DEBUG
	RDebug::Print(_L("CMetaDataParserRA::OpenFileL"));
#endif
    }

// -----------------------------------------------------------------------------
//
void CMetaDataParserRA::OpenDesL(const TDesC8& /*aDes*/)
    {
#ifdef _DEBUG
	RDebug::Print(_L("CMetaDataParserRA::OpenDesL"));
#endif
    }

// -----------------------------------------------------------------------------
// CMetaDataParserRA::GetTitleL
// -----------------------------------------------------------------------------
//
TInt CMetaDataParserRA::GetTitleL(TInt /*aCount*/)
	{
	return KErrNotFound;
	}

// -----------------------------------------------------------------------------
// CMetaDataParserRA::GetArtistL
// -----------------------------------------------------------------------------
//
TInt CMetaDataParserRA::GetArtistL(TInt /*aCount*/)
	{
	return KErrNotFound;
	}
	
// -----------------------------------------------------------------------------
// CMetaDataParserRA::GetGenreL
// -----------------------------------------------------------------------------
//
TInt CMetaDataParserRA::GetGenreL(TInt /*aCount*/)
	{
	return KErrNotFound;
	}	

// -----------------------------------------------------------------------------
// CMetaDataParserRA::GeComposerL
// -----------------------------------------------------------------------------
//
TInt CMetaDataParserRA::GetComposerL(TInt /*aCount*/)
	{
	return KErrNotFound;
	}	

// -----------------------------------------------------------------------------
// CMetaDataParserRA::GetCopyrightL
// -----------------------------------------------------------------------------
//
TInt CMetaDataParserRA::GetCopyrightL(TInt /*aCount*/)
	{
	return KErrNotFound;
	}	

//  End of File
