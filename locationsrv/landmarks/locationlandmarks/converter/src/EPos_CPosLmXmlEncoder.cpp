/*
* Copyright (c) 2002-2007 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description: Class used for encoding xml landmark content.
*
*
*/


#include    <lbsposition.h>
#include    <EPos_LandmarksErrors.h>
#include    "EPos_CPosLmXmlEncoder.h"
#include    "EPos_CPosLmFileOutput.h"
#include    "EPos_CPosLmBufferOutput.h"
#include    "EPos_CPosLmXmlEncoderOperation.h"
#include    "EPos_PosLmConverterUtils.h"

const TInt KPosBufferExpandSize = 600;

// ============================ MEMBER FUNCTIONS ===============================

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
//
CPosLmXmlEncoder::CPosLmXmlEncoder()
    {
    }

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
//
CPosLmXmlEncoder::~CPosLmXmlEncoder()
    {
    delete iOutput;
    delete iCollectionDataName;
    delete iCollectionDataDescription;
    }

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
//
CBufBase* CPosLmXmlEncoder::SetUseOutputBufferL()
    {
    ResetEncoderL();

    CBufBase* buffer = CreateOutPutBufferLC();

    InitializeEncodingL();

    CleanupStack::Pop(buffer);
    return buffer;
    }

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
//
void CPosLmXmlEncoder::SetOutputFileL(
    const TDesC& aOutputFile)
    {
    ResetEncoderL();

    CreateOutputFileL(aOutputFile);

    InitializeEncodingL();
    }

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
//
void CPosLmXmlEncoder::AddCollectionDataL(
    TPosLmCollectionDataId aDataId,
    const TDesC& aCollectionData)
    {
    __ASSERT_ALWAYS((iEncoderStatus != EPosUninitialized) &&
        (iEncoderStatus != EPosLmInfoAdded),
        Panic(KPosLandmarksClientPanic, EPosLmProtocolBreak));

    __ASSERT_ALWAYS(aDataId != EPosLmCollDataNone,
        Panic(KPosLandmarksClientPanic, EPosLmInvalidArgument));

    if (aDataId == EPosLmCollDataCollectionName)
        {
        if (iCollectionDataName)
            {
            User::Leave(KErrAlreadyExists);
            }
        iCollectionDataName = aCollectionData.AllocL();
        }
    else if (aDataId == EPosLmCollDataCollectionDescription)
        {
        if (iCollectionDataDescription)
            {
            User::Leave(KErrAlreadyExists);
            }
        iCollectionDataDescription = aCollectionData.AllocL();
        }
    else
        {
        // Silently ignore unknown collection data.
        return;
        }

    iEncoderStatus = EPosCollectionInfoAdded;
    }

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
//
void CPosLmXmlEncoder::AddLandmarkL( const CPosLandmark& aLandmark )
    {
    __ASSERT_ALWAYS(
        iEncoderStatus != EPosUninitialized,
        Panic( KPosLandmarksClientPanic, EPosLmProtocolBreak ) );
    
    if ( iEncoderStatus == EPosCollectionInfoAdded )
        {
        WriteCollectionDataL( iCollectionDataName, iCollectionDataDescription );
        }

    WriteLastLandmarkL();
    
    iLastLandmark = CPosLandmark::NewL( aLandmark );
    iEncoderStatus = EPosLmInfoAdded;
    }

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
//
void CPosLmXmlEncoder::AddCategoryForLatestLandmarkL( const CPosLandmarkCategory& aCategory )
    {
    __ASSERT_ALWAYS(
        iEncoderStatus == EPosLmInfoAdded,
        Panic(KPosLandmarksClientPanic, EPosLmProtocolBreak));

    iCategories.AppendL( CPosLandmarkCategory::NewL( aCategory ) );
    }

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
//
void CPosLmXmlEncoder::InitializeEncodingL()
    {
    WritePrologL();
    WriteStartTagsL();
    iEncoderStatus = EPosInitialized;
    }

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
//
CPosLmOperation* CPosLmXmlEncoder::FinalizeEncodingL()
    {
    __ASSERT_ALWAYS(
        iEncoderStatus == EPosLmInfoAdded,
        Panic(KPosLandmarksClientPanic, EPosLmProtocolBreak));

    WriteLastLandmarkL();
    WriteFinalTagsL();
    
    CPosLmXmlEncoderOperation* op = CPosLmXmlEncoderOperation::NewL( iOutput );
    // Ownership of iOutput has been transferred to op.
    iOutput = NULL;
    iEncoderStatus = EPosUninitialized;
    return op;
    }

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
//
CBufBase* CPosLmXmlEncoder::CreateOutPutBufferLC()
    {
    CPosLmBufferOutput* output = CPosLmBufferOutput::NewL(KPosBufferExpandSize);
    iOutput = output;
    return output->BufferLC();
    }

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
//
void CPosLmXmlEncoder::CreateOutputFileL( const TDesC& aOutputFile )
    {
    iOutput = CPosLmFileOutput::NewL(aOutputFile);
    }

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
//
void CPosLmXmlEncoder::ResetEncoderL()
    {
    delete iOutput;
    iOutput = NULL;

    delete iCollectionDataName;
    iCollectionDataName = NULL;
    delete iCollectionDataDescription;
    iCollectionDataDescription = NULL;
    
    delete iLastLandmark;
    iLastLandmark = NULL;
    iCategories.ResetAndDestroy();

    ResetIndentation();
    }

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
//
void CPosLmXmlEncoder::WritePrologL()
    {
    iOutput->WriteAndDeleteL( PrologStringLC() );
    }

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
//
void CPosLmXmlEncoder::WriteSTagL( TPosXmlTagType aTagType, const TDesC& aAttributes )
    {
    iOutput->WriteAndDeleteL( STagStringLC( aTagType, aAttributes ) );
    }

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
//
void CPosLmXmlEncoder::WriteETagL( TPosXmlTagType aTagType )
    {
    iOutput->WriteAndDeleteL( ETagStringLC( aTagType ) );
    }

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
//
void CPosLmXmlEncoder::WriteStringWithTagsL( const TDesC16& aDes, TPosXmlTagType aTagType )
    {
    WriteSTagL( aTagType, KNullDesC );
    WriteEscapedStringL( aDes );
    WriteETagL( aTagType );
    }

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
//
void CPosLmXmlEncoder::WriteTRealWithTagsL( TReal aValue, TPosXmlTagType aTagType )
    {
    WriteSTagL( aTagType, KNullDesC );
    iOutput->WriteTRealL( aValue );
    WriteETagL( aTagType );
    }

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
//
void CPosLmXmlEncoder::WriteEscapedStringL( const TDesC& aDes )
    {
    HBufC* des = PosLmConverterUtils::EscapeStringToXmlLC(aDes);
    iOutput->WriteL(*des);
    CleanupStack::PopAndDestroy(des);
    }

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
//
HBufC16* CPosLmXmlEncoder::PrologStringLC()
    {
    HBufC16* string = HBufC16::NewLC( KPosLongStringLength );

    string->Des().Append(KPosXmlDeclStart);
    string->Des().Append(KPosXmlSpace);

    string->Des().Append(KPosXmlVersion);
    string->Des().Append(KPosXmlEq);
    string->Des().Append(KPosXmlQuotationMark);
    string->Des().Append(KPosXmlVersionNum10);
    string->Des().Append(KPosXmlQuotationMark);

    string->Des().Append(KPosXmlSpace);

    string->Des().Append(KPosXmlEncoding);
    string->Des().Append(KPosXmlEq);
    string->Des().Append(KPosXmlQuotationMark);
    string->Des().Append(KPosXmlEncNameUTF8);
    string->Des().Append(KPosXmlQuotationMark);

    string->Des().Append(KPosXmlDeclEnd);
    string->Des().Append(KPosXmlNewLine);

    return string;
    }

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
//
void CPosLmXmlEncoder::WriteLastLandmarkL()
    {
    if ( iLastLandmark )
        {
        WriteLandmarkL( *iLastLandmark, iCategories );
        
        delete iLastLandmark;
        iLastLandmark = NULL;
        iCategories.ResetAndDestroy();
        }
    }

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
//
HBufC16* CPosLmXmlEncoder::STagStringLC( TPosXmlTagType aTagType, const TDesC& aAttributes )
    {
    TPtrC tag = TagNameL( aTagType );

    HBufC16* indent = IndentationLC();     
    HBufC16* string = HBufC16::NewLC( KPosStringLength + indent->Length() + aAttributes.Length() );

    string->Des().Append( *indent );
    string->Des().Append( KPosXmlSTagStart );
    string->Des().Append( KPosXmlNameSpacePrefix );
    string->Des().Append( tag );
    
    if ( aAttributes.Length() )
        {
        string->Des().Append( KPosXmlSpace );
        string->Des().Append( aAttributes );
        }
    
    string->Des().Append( KPosXmlTagEnd );

    if ( !IsTagLeaf( aTagType ) )
        {
        string->Des().Append( KPosXmlNewLine );
        iIndentationDepth++;
        }

    CleanupStack::Pop( string );
    CleanupStack::PopAndDestroy( indent );
    CleanupStack::PushL( string );
    return string;
    }

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
//
HBufC16* CPosLmXmlEncoder::ETagStringLC( TPosXmlTagType aTagType ) 
    {
    TPtrC tag = TagNameL( aTagType );

    if ( !IsTagLeaf( aTagType ) )
        {
        iIndentationDepth = Max( iIndentationDepth - 1, 0 );
        }

    HBufC16* indent = IndentationLC();     
    HBufC16* string = HBufC16::NewLC( KPosStringLength + indent->Length() );

    if ( !IsTagLeaf( aTagType ) )
        {
        string->Des().Append( *indent );
        }
    string->Des().Append( KPosXmlETagStart );
    string->Des().Append( KPosXmlNameSpacePrefix );
    string->Des().Append( tag );
    string->Des().Append( KPosXmlTagEnd );
    string->Des().Append( KPosXmlNewLine );

    CleanupStack::Pop( string );
    CleanupStack::PopAndDestroy( indent );
    CleanupStack::PushL( string );
    return string;
    }

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
//
void CPosLmXmlEncoder::ResetIndentation()
    {
    iIndentationDepth = 0;
    }

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
//
HBufC16* CPosLmXmlEncoder::IndentationLC()
    {
    ASSERT( iIndentationDepth >= 0 );
    TInt indentLength = iIndentationDepth * KPosXmlTab().Length();
    HBufC16* indent = HBufC16::NewLC( indentLength );
    for ( TInt i=0; i < iIndentationDepth; i++ )
        {
        indent->Des().Append( KPosXmlTab );
        }
    return indent;
    }

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
//
TBool CPosLmXmlEncoder::IsTagLeaf( TPosXmlTagType aXmlTagType )
    {
    return ( aXmlTagType < EPosXmlLmLastTree );
    }
