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
#include	"MetaDataParserDcf2.h"
#include	"MetaDataParserFactory.h"
#ifdef _DEBUG
#include	<e32svr.h>
#endif

// ============================ MEMBER FUNCTIONS ===============================

// -----------------------------------------------------------------------------
// CMetaDataParserDcf2::CMetaDataParserDcf2
// C++ default constructor can NOT contain any code, that
// might leave.
// -----------------------------------------------------------------------------
//
CMetaDataParserDcf2::CMetaDataParserDcf2()
	{
    }

// -----------------------------------------------------------------------------
// CMetaDataParserDcf2::ConstructL
// Symbian 2nd phase constructor can leave.
// -----------------------------------------------------------------------------
//
void CMetaDataParserDcf2::ConstructL()
    {
	}

// -----------------------------------------------------------------------------
// CMetaDataParserDcf2::NewL
// Two-phased constructor.
// -----------------------------------------------------------------------------
//
CMetaDataParserDcf2* CMetaDataParserDcf2::NewL()
    {
#ifdef _DEBUG
	RDebug::Print(_L("CMetaDataParserDcf2::NewL"));
#endif
	CMetaDataParserDcf2* self = new( ELeave ) CMetaDataParserDcf2;
    CleanupStack::PushL( self );
    self->ConstructL();
    CleanupStack::Pop();
    return self;
    }

// Destructor
CMetaDataParserDcf2::~CMetaDataParserDcf2()
	{
	}

// -----------------------------------------------------------------------------
// CMetaDataParserDcf2::ParseL
// -----------------------------------------------------------------------------
//
void CMetaDataParserDcf2::ParseL(
	const RArray<TMetaDataFieldId>& aWantedFields,
	CMetaDataFieldContainer& aContainer )
    {
#ifdef _DEBUG
	RDebug::Print(_L("CMetaDataParserDcf2::ParseL"));
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
		ReadMetaDataEntryL(ERecordingYear, EMetaDataYear);
		ReadMetaDataEntryL(ECoverUri, EMetaDataJpeg);
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
				case EMetaDataYear:
                    ReadMetaDataEntryL(ERecordingYear, EMetaDataYear);
					break;
				case EMetaDataJpeg:
                    ReadMetaDataEntryL(ECoverUri, EMetaDataJpeg);
					break;
				default:
					break;
				}
			}
		}
		
	// check if need to parse from payload
	if(aWantedFields.Count() == 0)
		{
		if ( !((CMetaDataSourceFile*)iSource)->KeyExist() ) 
			{
			return; // no rights 
			}
		// rights present, parse payload for more fields
		CMetaDataParser* parser = NULL;
		TInt start = 0;
	    ((CMetaDataSourceFile*)iSource)->Seek(ESeekStart, start);
	    parser = CMetaDataParserFactory::CreateID3ParserL(*iSource);
		if (parser == NULL)
	 		{
	 		TRAPD(err, parser = CMetaDataParserFactory::CreateMP4ParserFileL(*iSource));
	    	if(err || !parser )
		   		{
				return; // no valid parsers found
				}
	 		}
	 	CleanupStack::PushL(parser);	
	 	CMetaDataFieldContainer *newContainer = CMetaDataFieldContainer::NewL();
	 	CleanupStack::PushL(newContainer);
	 	RArray<TMetaDataFieldId> zeroWantedFields;
	 	CleanupClosePushL(zeroWantedFields);
	 	parser->ParseL(zeroWantedFields, *newContainer);
	 	CleanupStack::PopAndDestroy(); // zeroWantedFields
	 	if(newContainer->Count() == 0)
	 		{
	 		CleanupStack::PopAndDestroy(2); // newContainer, parser
	 		return; 
	 		}
	 	for(TInt i = 0; i < newContainer->Count(); i++)
	 		{
	 		// copy extra items from new container to container
	 		TMetaDataFieldId newFieldId = EUnknownMetaDataField;
	 		TPtrC data = newContainer->At(i, newFieldId);
	 		TBool found = EFalse;
	 		for(TInt j = 0; j < iContainer->Count(); j++)
	 			{
	 			TMetaDataFieldId fieldId = EUnknownMetaDataField;
	 			iContainer->At(j, fieldId);
	 			if(newFieldId == fieldId)
	 				{
	 				// exists
	 				found = ETrue;
	 				break;
	 				}
	 			}
	 		if(!found)
	 			{
	 			if(newFieldId == EMetaDataJpeg)
    	 			{ 
    	 			TPtrC8 data8 = newContainer->Field8(newFieldId);
	 			    iContainer->AppendL(newFieldId, data8); 
    	 			}
	 			else
    	 			{
	 			    iContainer->AppendL(newFieldId, data);	    
    	 			}
	 			
	 			}
	 		}
	 	CleanupStack::PopAndDestroy(2); // newContainer, parser
	 	return;
		}
		
	if(aWantedFields.Count() <= aContainer.Count())
		{
		return; 
		}
	else // parse payload
		{
		if ( !((CMetaDataSourceFile*)iSource)->KeyExist() ) 
			{
			return; // no rights 
			}
		// rights present, parse payload for more fields
		CMetaDataParser* parser = NULL;
		TInt start = 0;
	    ((CMetaDataSourceFile*)iSource)->Seek(ESeekStart, start);
	    parser = CMetaDataParserFactory::CreateID3ParserL(*iSource);
		if (parser == NULL)
	 		{
	 		TRAPD(err, parser = CMetaDataParserFactory::CreateMP4ParserFileL(*iSource));
	    	if(err || !parser )
		   		{
				return; // no valid parsers found
				}
	 		}
	 	CleanupStack::PushL(parser);
		// create new wanted fields
		RArray<TMetaDataFieldId> newWantedFields;
		CleanupClosePushL(newWantedFields);
		for(TInt i = 0; i < aWantedFields.Count(); i++)
			{
			TMetaDataFieldId wantedFieldId = aWantedFields[i];
			TBool found = EFalse;
			for(TInt j = 0; j < iContainer->Count(); j++)
				{
				TMetaDataFieldId parsedFieldId = EUnknownMetaDataField;
				iContainer->At(j, parsedFieldId);
				if(wantedFieldId == parsedFieldId)
					{
					// found wanted field id
					found = ETrue;
					break;
					}
				}
			if(!found)
				{
				newWantedFields.AppendL(wantedFieldId);	
				}
			}
		if(newWantedFields.Count() != 0)
			{
			parser->ParseL(newWantedFields, *iContainer);
			}
	 	CleanupStack::PopAndDestroy(2); // newWantedFields, parser
  		}
 	}

// -----------------------------------------------------------------------------
// CMetaDataParserDcf2::InitializeFileL
// Symbian 2nd phase constructor can leave.
// -----------------------------------------------------------------------------
//
void CMetaDataParserDcf2::InitializeFileL(CMetaDataSource* aSource)
    {
#ifdef _DEBUG
	RDebug::Print(_L("CMetaDataParserDcf2::InitializeFileL"));
#endif
	iSource = aSource;
	ValidateL();
#ifdef _DEBUG
	RDebug::Print(_L("CMetaDataParserDcf2::InitializeFileL - Done"));
#endif
    }

// -----------------------------------------------------------------------------
// CMetaDataParserDcf2::ValidateL
// -----------------------------------------------------------------------------
//
TBool CMetaDataParserDcf2::ValidateL()
	{
	iExists = ETrue;
	return ETrue;
	}

void CMetaDataParserDcf2::ReadMetaDataEntryL(TInt aEntryName, 
	TMetaDataFieldId aFieldId)
	{
	if(aEntryName == ECoverUri)
		{
		HBufC8* pic = NULL;
		TInt err = KErrNone;
		err = ((CMetaDataSourceFile*)iSource)->GetAttributeL(aEntryName, &pic);
		if(err != KErrNone)
			{
			delete pic;
			return;
			}
		if(pic->Length() > 0)
			{
			iContainer->AppendL( aFieldId, *pic );
			}
		delete pic;
		return;
		}
	// else
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
