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
* Description:  This class implements an MP4 metadata parser 
*
*/



// INCLUDE FILES
#include	"MetaDataParserMP4.h"
#ifdef _DEBUG
#include	<e32svr.h>
#endif

const TUint KCustomFileBufferSize	= 10*1024;

// ============================ MEMBER FUNCTIONS ===============================

// -----------------------------------------------------------------------------
// CMetaDataParserMP4::CMetaDataParserMP4
// C++ default constructor can NOT contain any code, that
// might leave.
// -----------------------------------------------------------------------------
//
CMetaDataParserMP4::CMetaDataParserMP4()
	:	iMP4Handle(NULL)
	{
    }

// Destructor
CMetaDataParserMP4::~CMetaDataParserMP4()
	{
	MP4ParseClose(iMP4Handle);
	if ( iExists )
		{
		delete iCharacterSet;
		iFs.Close();
		}
	}

// -----------------------------------------------------------------------------
// CMetaDataParserMP4::InitializeFileL
// Symbian 2nd phase constructor can leave.
// -----------------------------------------------------------------------------
//
void CMetaDataParserMP4::InitializeFileL(
	ContentAccess::CData *aData )
    {
	MP4Err err;
	//TBuf<256> name(aFileName);
	err = MP4ParseOpenCAF(&iMP4Handle, aData);
#ifdef _DEBUG
	RDebug::Print(_L("CMetaDataParserMP4::InitializeFileL - MP4ParseOpen err = %d"), err);
#endif
	User::LeaveIfError(TranslateMP4Err(err));
	MP4Err mp4Err = MP4SetCustomFileBufferSizes( iMP4Handle, 0, 0, KCustomFileBufferSize);
	if ( (mp4Err == MP4_OK) && ValidateL() )
		{
		User::LeaveIfError(iFs.Connect());
		// Get list of charconv supported character sets
		iCharacterSet = CCnvCharacterSetConverter::CreateArrayOfCharacterSetsAvailableL(iFs);
		}
#ifdef _DEBUG
	RDebug::Print(_L("CMetaDataParserMP4::InitializeFileL - Done"));
#endif
    }

// -----------------------------------------------------------------------------
// CMetaDataParserMP4::InitializeDesL
// Symbian 2nd phase constructor can leave.
// -----------------------------------------------------------------------------
//
void CMetaDataParserMP4::InitializeDesL(
	const TDesC8& aDes )
    {
	MP4Err err;
	err = MP4ParseOpen(&iMP4Handle, NULL);
#ifdef _DEBUG
	RDebug::Print(_L("CMetaDataParserMP4::InitializeDesL - MP4ParseOpen err = %d"), err);
#endif
	User::LeaveIfError(TranslateMP4Err(err));

	TUint8* buffer = CONST_CAST(TUint8*, aDes.Ptr());
	err = MP4ParseWriteData(iMP4Handle, buffer, aDes.Size());
	User::LeaveIfError(TranslateMP4Err(err));

	// This call with empty buffer indicates to the MP4 library that this is the last buffer.
	err = MP4ParseWriteData(iMP4Handle, NULL, 0);
	User::LeaveIfError(TranslateMP4Err(err));

	if ( ValidateL() )
		{
		User::LeaveIfError(iFs.Connect());
		// Get list of charconv supported character sets
		iCharacterSet = CCnvCharacterSetConverter::CreateArrayOfCharacterSetsAvailableL(iFs);
		}
#ifdef _DEBUG
	RDebug::Print(_L("CMetaDataParserMP4::InitializeDesL - Done"));
#endif
    }


// -----------------------------------------------------------------------------
// CMetaDataParserMP4::TranslateMP4Err
// -----------------------------------------------------------------------------
//
TInt CMetaDataParserMP4::TranslateMP4Err(
	MP4Err aError )
	{
#ifdef _DEBUG
	RDebug::Print(_L("CMetaDataParserMP4::TranslateMP4Err [%d]"), aError);
#endif
	TInt err;
	switch ( aError )
		{
		case MP4_OK:
			err = KErrNone;
			break;
		case MP4_OUT_OF_MEMORY:
			err = KErrNoMemory;
			break;
		case MP4_NOT_AVAILABLE:
		case MP4_UDTA_NOT_FOUND:
			err = KErrNotReady;
			break;
		case MP4_FILE_ERROR:
			err = KErrBadHandle;
			break;
		case MP4_INVALID_TYPE:
			err = KErrNotSupported;
			break;
		case MP4_TIMESCALE_NOT_SET:
			err = KErrNotReady;
			break;
		case MP4_NOT_STREAMABLE:
		case MP4_NO_REQUESTED_FRAME:
		case MP4_CANT_SEEK:
		case MP4_INVALID_INPUT_STREAM:
		case MP4_NO_FRAME:
			err = KErrArgument;
			break;
		case MP4_ERROR:
		case MP4_FILE_MODE:
		case MP4_BUFFER_TOO_SMALL:
		case MP4_END_OF_VIDEO:
		case MP4_METADATA_ERROR:
		case MP4_NO_VIDEO:
		case MP4_NO_AUDIO:
			err = KErrGeneral;
			break;
		default:
			err = KErrGeneral;
			break;
		}
	return err;
	}

// -----------------------------------------------------------------------------
// CMetaDataParserMP4::GetDurationL
// -----------------------------------------------------------------------------
//
void CMetaDataParserMP4::GetDurationL(
	TMetaDataFieldId aFieldId )
	{
#ifdef _DEBUG
	RDebug::Print(_L("CMetaDataParser3gp::GetDurationL"));
#endif
	MP4Err err;
	mp4_u32 audiolength = 0; 
	mp4_u32 audiotype = 0; 
	mp4_u8 framespersample = 0;
	mp4_u32 timescale = 0;
	mp4_u32 averagebitrate = 0;
	err = MP4ParseRequestAudioDescription(iMP4Handle, &audiolength, &audiotype, &framespersample, &timescale, &averagebitrate);
	if(err == MP4_OK)
		{
		TBuf16<10> data16;
		TReal duration = (TReal)audiolength / 1000; // seconds
		TRealFormat realFormat(9, 3);
		data16.AppendNum(duration, realFormat);
		iContainer->AppendL( aFieldId, data16);
		}
	}


// -----------------------------------------------------------------------------
// CMetaDataParserMP4::ConvertToUnicodeL
// -----------------------------------------------------------------------------
//
TInt CMetaDataParserMP4::ConvertToUnicodeL(
	const TDesC8& aDesc,
	TDes16& aUnicode )
	{
	TPtrC8 unicodeData;
    TUint characterSetId = 0;
	CCnvCharacterSetConverter* charSetConv = CCnvCharacterSetConverter::NewLC();
	TInt state = CCnvCharacterSetConverter::KStateDefault;

	TInt bom = UnicodeBOM(aDesc);
	if ( bom == KUnicodeBOMBigEndian )
		{
		// UTF-16 Big Endian with BOM
		// characterSetId = KCharacterSetIdentifierUcs2;
		TPtrC8 text = StripTrailingZeroes(aDesc, 1);
		characterSetId = KCharacterSetIdentifierUnicodeBig;
		// Skip BOM & Terminate marks
		unicodeData.Set( text.Mid(2) );
		}
	else if ( bom == KUnicodeBOMLittleEndian )
		{
		// UTF-16 Big Endian with BOM
		characterSetId = KCharacterSetIdentifierUnicodeLittle;
		TPtrC8 text = StripTrailingZeroes(aDesc, 1);
		// Skip BOM & Terminate marks
		unicodeData.Set( text.Mid( 2) );
		}
	else	// KUnicodeBOMNotFound
		{
		// UTF-8
		TPtrC8 text = StripTrailingZeroes(aDesc, 0);
		characterSetId = KCharacterSetIdentifierUtf8;
		unicodeData.Set( text );
		}

	charSetConv->PrepareToConvertToOrFromL(characterSetId, *iCharacterSet, iFs);
	TInt err = charSetConv->ConvertToUnicode(aUnicode, unicodeData, state);

#ifdef _DEBUG
	RDebug::Print(_L("CMetaDataParserID3v24::ConvertToUnicode :-> Tag Size[%d] Unicode Tag Size[%d]Bytes Unconverted[%d]"),
		unicodeData.Length(), aUnicode.Length(), err);
#endif

	CleanupStack::PopAndDestroy();	// charSetConv
	return err;
	}

//  End of File
