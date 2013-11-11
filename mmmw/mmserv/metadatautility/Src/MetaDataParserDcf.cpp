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
* Description:  This class implements an 3gp metadata parser as specified in
*                www.3gpp.org (specification 3GPP TS 26.244 V6.0.0).
*
*/



// INCLUDE FILES
#include <Oma2Agent.h>
#include	"MetaDataParserDcf.h"
#ifdef _DEBUG
#include	<e32svr.h>
#endif

// ============================ MEMBER FUNCTIONS ===============================

// -----------------------------------------------------------------------------
// CMetaDataParserDcf::CMetaDataParserDcf
// C++ default constructor can NOT contain any code, that
// might leave.
// -----------------------------------------------------------------------------
//
CMetaDataParserDcf::CMetaDataParserDcf()
	{
    }

// -----------------------------------------------------------------------------
// CMetaDataParserDcf::ConstructL
// Symbian 2nd phase constructor can leave.
// -----------------------------------------------------------------------------
//
void CMetaDataParserDcf::ConstructL()
    {
	}

// -----------------------------------------------------------------------------
// CMetaDataParserDcf::NewL
// Two-phased constructor.
// -----------------------------------------------------------------------------
//
CMetaDataParserDcf* CMetaDataParserDcf::NewL()
    {
#ifdef _DEBUG
	RDebug::Print(_L("CMetaDataParserDcf::NewL"));
#endif
	CMetaDataParserDcf* self = new( ELeave ) CMetaDataParserDcf;
    CleanupStack::PushL( self );
    self->ConstructL();
    CleanupStack::Pop();
    return self;
    }

// Destructor
CMetaDataParserDcf::~CMetaDataParserDcf()
	{
	}

// -----------------------------------------------------------------------------
// CMetaDataParserDcf::ParseL
// -----------------------------------------------------------------------------
//
void CMetaDataParserDcf::ParseL(
	const RArray<TMetaDataFieldId>& aWantedFields,
	CMetaDataFieldContainer& aContainer )
    {
#ifdef _DEBUG
	RDebug::Print(_L("CMetaDataParserDcf::ParseL"));
#endif
	iContainer = &aContainer;

	if ( aWantedFields.Count() == 0 )
		{
		ReadMetaDataEntryL(ContentAccess::ETitle, EMetaDataSongTitle);
		ReadMetaDataEntryL(ContentAccess::EDescription, EMetaDataComment);
		ReadMetaDataEntryL(ContentAccess::EAuthor, EMetaDataComposer);
		ReadMetaDataEntryL(ContentAccess::EContentURI, EMetaDataUrl);
		ReadMetaDataEntryL(EPerformer, EMetaDataArtist);
		ReadMetaDataEntryL(EAlbumTitle, EMetaDataAlbum);
		ReadMetaDataEntryL(EAlbumTrack, EMetaDataAlbumTrack);
		ReadMetaDataEntryL(EContentVendor, EMetaDataVendor);
		ReadMetaDataEntryL(ECopyright, EMetaDataCopyright);
		ReadMetaDataEntryL(EGenre, EMetaDataGenre);
		ReadMetaDataEntryL(ERatingInfo, EMetaDataRating);
		}
	else
		{
		// Look for it in the wanted field array
		TInt count( aWantedFields.Count() );
		for ( TInt i = 0; i < count; i++ )
			{
			switch ( aWantedFields[ i ] )
				{
				case EMetaDataSongTitle:
					ReadMetaDataEntryL(ContentAccess::ETitle, EMetaDataSongTitle);
					break;
				case EMetaDataComment:
					ReadMetaDataEntryL(ContentAccess::EDescription, EMetaDataComment);
					break;
				case EMetaDataComposer:
					ReadMetaDataEntryL(ContentAccess::EAuthor, EMetaDataComposer);
				    break;
				case EMetaDataUrl:
                    ReadMetaDataEntryL(ContentAccess::EContentURI, EMetaDataUrl);
					break;
				case EMetaDataArtist:
                    ReadMetaDataEntryL(EPerformer, EMetaDataArtist);
					break;
				case EMetaDataAlbum:
                    ReadMetaDataEntryL(EAlbumTitle, EMetaDataAlbum);
					break;
				case EMetaDataAlbumTrack:
                    ReadMetaDataEntryL(EAlbumTrack, EMetaDataAlbumTrack);
					break;
				case EMetaDataVendor:
					ReadMetaDataEntryL(EContentVendor, EMetaDataVendor);
					break;
				case EMetaDataCopyright:
					ReadMetaDataEntryL(ECopyright, EMetaDataCopyright);
					break;
				case EMetaDataGenre:
                    ReadMetaDataEntryL(EGenre, EMetaDataGenre);
					break;
				case EMetaDataRating:
                    ReadMetaDataEntryL(ERatingInfo, EMetaDataRating);
					break;
				default:
					break;
				}
			}
		}
 	}

// -----------------------------------------------------------------------------
// CMetaDataParserDcf::InitializeFileL
// Symbian 2nd phase constructor can leave.
// -----------------------------------------------------------------------------
//
void CMetaDataParserDcf::InitializeFileL(CMetaDataSource* aSource)
    {
#ifdef _DEBUG
	RDebug::Print(_L("CMetaDataParserDcf::InitializeFileL"));
#endif
	iSource = aSource;
	ValidateL();
#ifdef _DEBUG
	RDebug::Print(_L("CMetaDataParserDcf::InitializeFileL - Done"));
#endif
    }

// -----------------------------------------------------------------------------
// CMetaDataParserDcf::ValidateL
// -----------------------------------------------------------------------------
//
TBool CMetaDataParserDcf::ValidateL()
	{
	iExists = ETrue;
	return ETrue;
	}

void CMetaDataParserDcf::ReadMetaDataEntryL(TInt aEntryName, 
	TMetaDataFieldId aFieldId)
	{
	TInt err = KErrNone;
	TBuf <256> headerValue;
	err = ((CMetaDataSourceFile*)iSource)->GetStringAttribute(aEntryName, headerValue);

	if(err != KErrNone)
		{
		return;
		}
	TPtrC info = StripTrailingZeroes(headerValue);
	if(info.Length() > 0)
		{
		iContainer->AppendL( aFieldId, info );
		}
}

//  End of File
