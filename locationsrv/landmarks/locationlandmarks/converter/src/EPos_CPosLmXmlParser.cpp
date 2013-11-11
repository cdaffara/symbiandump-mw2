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
* Description: Parser that uses Symbian XML Framework to parse XML files with
*  Landmark Content Format.
 *
 *
*/


#include <utf.h>
#include <xml/xmlparsererrors.h>
#include <lbs.h>
#include <EPos_PosLmImplExtension.h>
#include <EPos_LandmarksErrors.h>
#include "EPos_CPosLmXmlParser.h"
#include "EPos_CPosLmXmlParserOperation.h"
#include "EPos_CPosLmCollectionData.h"
#include "EPos_CPosLmParsedLmBuffer.h"
#include "EPos_PosLmConverterUtils.h"
#include "EPos_LmConverter.h"

const TInt KPosLmParserBytesAtATime = 100;

// ============================ MEMBER FUNCTIONS ===============================

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
//
CPosLmXmlParser::CPosLmXmlParser()
    {
     
    }

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
//
CPosLmXmlParser::~CPosLmXmlParser()
    {
    // Notify operation that parser is deleted
    DisconnectOperation();
    ResetParser();

    iCollectionData.Close();

    iCategories.Close();
    iParsedLmBuffer.Close();
    
    }

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
//
TInt CPosLmXmlParser::ParseNextPartL( TReal32& aProgress )
    {
    __ASSERT_ALWAYS(( iParserStatus & EStateInitialized ) &&( iParserStatus & EStateOperationConstructed ), Panic(
            KPosLandmarksClientPanic, EPosLmProtocolBreak ) );
    TInt status = KPosLmOperationNotComplete;

    // Check if there are a parsed landmark before in lm parsed buffer
    TBool lmBeforeParsing = EFalse;
    if ( iParsedLmBuffer.Count()> 0 )
        {
        lmBeforeParsing = ETrue;
        }

    // We need to parse file/buffer since we don't have any landmark
    // in parsed buffer and
    // also to check that there are some more landmarks coming
    if ( iParsedLmBuffer.Count()<= 1 )
        {
        TInt nrOfLmBeforeParsing = iParsedLmBuffer.Count();
        TInt bytesBeforeParsing = iNumberOfBytesRead;

        iParserStatus |= EStateParsingStarted;
        iParserStatus &= ~EStateParsedLandmark;
        iParserStatus &= ~EStateParsingForNextLm;
        iParserStatus &= ~EStateParsingPartReady;
        iParserError = KErrNone;
        ReadNextPartL();
        iParserStatus |= EStateParsing;

        // Check if we have read a landmark
        while(!ParsedPart() && BufferLength()> 0 )
            {
            iXmlParser->ParseL(*iBuffer );
            User::LeaveIfError( iParserError );

            // Check if we are ready parsing
            /*if (( iParserStatus & EStateParsedLandmark ) &&( iParserStatus & EStateParsingForNextLm ) )
                {
                iParserStatus |= EStateParsingPartReady;
                }
            else
                if ( iParserStatus & EStateParsedLandmark )
                    {
                    iParserStatus |= EStateParsingForNextLm;
                    }*/

            if (!ParsedPart() )
                {
                ReadNextPartL();
                }
            }

        // Evaluate read bytes per read lm, used for progress
        if ( nrOfLmBeforeParsing < iParsedLmBuffer.Count() )
            {
            iIncrBytesPerLm = TReal32( iNumberOfBytesRead -TReal32( bytesBeforeParsing ) )/( iParsedLmBuffer.Count() - nrOfLmBeforeParsing );
            }

        // Finished parsing
        iParserStatus &= ~EStateParsing;
        }

    // Remove latest landmark if there where any parsed
    if ( lmBeforeParsing && iParserStatus)// & EStateParsedLandmark)
        {
        
        	delete iParsedLmBuffer[0];
        	iParsedLmBuffer.Remove( 0 );
                
        }

	if ( iParsedLmBuffer.Count()> 0 && iParsedLmBuffer[0]->Landmark())
        {
        	iNumOfParsedLandmarks++;
        }
    	else
        {
        	User::Leave( KErrPosLmUnknownFormat );
        }	
	    	
	    


		
	
    // Set progress / Linear progress
    aProgress = TReal32( iNumberOfBytesRead -iIncrBytesPerLm *( iParsedLmBuffer.Count() - 1 ) )/TReal32( iContentSize );

    // Check status if parsing is completed
    if ( IsParsingComplete() )
        {
        User::LeaveIfError( CompleteParsing());
        status = KErrNone;
        }
    
 
    return status;
    }

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
//
void CPosLmXmlParser::ResetParserOnError()
    {
    CompleteParsing();

    iParserStatus &= ~EStateParsingStarted;
    iParserStatus &= ~EStateParsing;
    iParserStatus &= ~EStateParsingReady;
    iReadTags.Reset();
    iTrees.Reset();
    iLastElementState = EXmlParserOnEndElement;
    
    iNumberOfBytesRead = 0;
    iIncrBytesPerLm = 0;

    iParserError = KErrNone;
    ResetParserPartState();
    }

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
//
void CPosLmXmlParser::SetInputBuffer( const TDesC8& aBuffer )
    {
    __ASSERT_ALWAYS(!( iParserStatus & EStateParsing ), Panic( KPosLandmarksClientPanic,
            EPosLmProtocolBreak ) );

    ResetParser();

    iInputBuffer.Set( aBuffer );

    // Set size
    iContentSize = iInputBuffer.Length();

    iParserStatus |= EStateInitialized;
    }

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
//
void CPosLmXmlParser::SetInputFileL( const TDesC& aFile )
{
	SetInputFileInternalL( NULL, aFile );
}

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
//
void CPosLmXmlParser::SetInputFileHandleL( RFile& aFileHandle )
{
	SetInputFileInternalL(&aFileHandle );
}

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
//
CPosLmOperation* CPosLmXmlParser::ParseContentL( TBool aBuildIndex )
    {
    __ASSERT_ALWAYS( iParserStatus & EStateInitialized, Panic(
            KPosLandmarksClientPanic, EPosLmProtocolBreak ) );

    // Check if we support buildindex
    if ( aBuildIndex )
        {
        User::Leave( KErrNotSupported );
        }

     // Reset parsers content
    ResetContentL();

    DisconnectOperation();

    iParserOp = CPosLmXmlParserOperation::NewL(*this );
    return iParserOp; // ownership is transferred to client
    }

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
//
TUint32 CPosLmXmlParser::NumOfParsedLandmarks() const
    {
    return iNumOfParsedLandmarks;
    }

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
//
TPosLmCollectionDataId CPosLmXmlParser::FirstCollectionDataId() const
    {
    if ( iCollectionData.Count()== 0 )
        {
        return EPosLmCollDataNone;
        }
    return iCollectionData[0]->CollectionDataId();
    }

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
//
TPosLmCollectionDataId CPosLmXmlParser::NextCollectionDataId(
        TPosLmCollectionDataId aCollectionDataId ) const
    {
    for( TInt i = 0; i < iCollectionData.Count(); i++ )
        {
        if ( iCollectionData[i]->CollectionDataId()== aCollectionDataId &&i+1 < iCollectionData.Count() )
            {
            return iCollectionData[i+1]->CollectionDataId();
            }
        }

    return EPosLmCollDataNone;
    }

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
//
TPtrC CPosLmXmlParser::CollectionData( TPosLmCollectionDataId aDataId ) const
    {
    TPtrC collectionData;
    TBool found = EFalse;
    for( TInt i = 0; !found && i < iCollectionData.Count(); i++ )
        {
        if ( iCollectionData[i]->CollectionDataId()== aDataId )
            {
            iCollectionData[i]->GetCollectionData( collectionData );
            found = ETrue;
            }
        }

    return collectionData;
    }

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
//
CPosLandmark* CPosLmXmlParser::LandmarkLC( TUint aLandmarkIndex ) const
    {
    __ASSERT_ALWAYS(
            aLandmarkIndex < iNumOfParsedLandmarks ||aLandmarkIndex == KPosLastParsedLandmark,
            Panic( KPosLandmarksClientPanic, EPosInvalidIndex ) );

    // Need to check things here
    if ( aLandmarkIndex != KPosLastParsedLandmark ||iParsedLmBuffer.Count()== 0 )
        {
        //User::Leave( KErrPosLmUnknownFormat);//KErrNotFound );
        User::Leave( KErrNotFound );
        }
	//if(!isAlreadyCreated)
		
    	return CPosLandmark::NewLC(*iParsedLmBuffer[0]->Landmark() );
    	
    //else
    	//return NULL;		
	
	
    }

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
//
CPosLandmarkCategory* CPosLmXmlParser::LandmarkCategoryLC(
        TPosLmItemId aCategoryId ) const
    {
    TInt found = KErrNotFound;
    if ( iParsedLmBuffer.Count()== 0 )
        {
        User::Leave( found );
        }

    for( TInt i=0; found == KErrNotFound &&i<iParsedLmBuffer[0]->Categories().Count(); i++ )
        {
        TPosLmItemId itemid = iParsedLmBuffer[0]->Categories()[i]->CategoryId();
        if ( itemid != KPosLmNullItemId && itemid == aCategoryId )
            {
            found = i;
            }
        }

    User::LeaveIfError( found );
    return CPosLandmarkCategory::NewLC(*iParsedLmBuffer[0]->Categories()[found] );
    }

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
//
void CPosLmXmlParser::OnStartDocumentL(
        const Xml::RDocumentParameters& /*aDocParam*/, TInt /*aErrorCode*/)
    {
    }

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
//
void CPosLmXmlParser::OnEndDocumentL( TInt /*aErrorCode*/)
    {
    iParserStatus |= EStateParsingReady;
    }

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
//
void CPosLmXmlParser::OnStartElementL( 
    const Xml::RTagInfo& aElement,
    const Xml::RAttributeArray& aAttributes, 
    TInt /*aErrorCode*/)
    {
    delete iReadContent;
    iReadContent = NULL;
    
    if ( iLastElementState == EXmlParserOnStartElement )
        {
        // new element started, but last one has not been yet closed 
        // => last element is a tree tag
        StartNewTreeL();
        }
    else
        {
        iLastElementState = EXmlParserOnStartElement;
        }

    TPosXmlTagType startTag = TagTypeL( aElement );
    
    // Save namespace of root tag
    if ( startTag == EPosXmlRoot && !iRootNs )
    	{
    	iRootNs = aElement.Uri().DesC().AllocL();
    	}

    if ( startTag != EPosXmlUnknown )
        {
        CheckSequenceOrderL( startTag );
        OnAttributesL( startTag, aAttributes );
        }

    iReadTags.AppendL( startTag );
    SetParserStateL( startTag );
    }


// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
//
void CPosLmXmlParser::CheckSequenceOrderL( TPosXmlTagType aTag )
    {
    RArray<TPosXmlTagType> allowedChildTags;
    CleanupClosePushL( allowedChildTags );
    GetNextAllowedTagsL( allowedChildTags );
    
    PosLmConverterUtils::AssertL( allowedChildTags.Find( aTag ) >= 0 );
    
    CleanupStack::PopAndDestroy( &allowedChildTags );
    }

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
//
void CPosLmXmlParser::OnEndElementL( const Xml::RTagInfo& aElement, TInt /*aErrorCode*/)
    {
    if ( iLastElementState == EXmlParserOnEndElement )
        {
        // some tree is closed
        CheckMandatoryTagsL( CurrentTree() );
        CloseCurrentTree();
        }
    else
        {
        // here checking is done for the case when
        // closing tag looks like a leaf,
        // but is expected to be a tree and is missing all child tags
        // cases like <latitide>/<longitude> fields not included in <landmark> in LMX
        CheckMandatoryTagsL( CurrentTag() );
        iLastElementState = EXmlParserOnEndElement;
        }

    TPosXmlTagType tagType = TagTypeL( aElement );

    SaveContentL( tagType );
    
    if ( tagType == EPosXmlRoot ) 
        {
        HandleRootTag();
        }
    }

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
//
void CPosLmXmlParser::SaveContentL( TPosXmlTagType aTag )
    {
    // Read property/content for end element
    if ( iReadContent )
        {
        // Convert property/content
        HBufC* content = PosLmConverterUtils::Utf8ToUnicodeLC( *iReadContent );

        // Set content on Landmark related objects and parser status
        SetContentL( aTag, *content );

        // Reset/delete read/converted content
        CleanupStack::PopAndDestroy( content );

        delete iReadContent;
        iReadContent = NULL;
        }
    else
        {
        SetContentL( aTag, KNullDesC );
        }
    }

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
//
void CPosLmXmlParser::OnContentL( const TDesC8& aBytes, TInt /*aErrorCode*/)
    {
    TPosXmlTagType currentTag = EPosXmlNothing;
    if ( iLastElementState == EXmlParserOnStartElement )
        {
        currentTag = CurrentTag();
        }
    else
        {
        currentTag = CurrentTree();
        }
    
    if ( currentTag != EPosXmlUnknown && 
         currentTag != EPosXmlNothing )
        {
        // Only leaf tags should contain data
        if ( !PosLmConverterUtils::OnlyWhiteSpaces( aBytes ) )
            {
            PosLmConverterUtils::AssertL( !IsTreeTag( currentTag ) );
            }
        
        // If there is a content from a previous parsing
        // then we need to concatenate data/strings.
        if ( iReadContent )
            {
            HBufC8* prevContent = iReadContent;
            CleanupStack::PushL( prevContent );
            iReadContent = NULL;
    
            iReadContent = HBufC8::NewL( prevContent->Length()+ aBytes.Length() );
            iReadContent->Des().Append( *prevContent );
            iReadContent->Des().Append( aBytes );
    
            CleanupStack::PopAndDestroy( prevContent );
            }
        else
            {
            iReadContent = aBytes.AllocL();
            }
        }
    }

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
//
void CPosLmXmlParser::OnStartPrefixMappingL( const RString& /*aPrefix*/,
        const RString& /*aUri*/, TInt /*aErrorCode*/)
    {
    }

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
//
void CPosLmXmlParser::OnEndPrefixMappingL( const RString& /*aPrefix*/, TInt /*aErrorCode*/)
    {
    }

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
//
void CPosLmXmlParser::OnIgnorableWhiteSpaceL( const TDesC8& /*aBytes*/, TInt /*aErrorCode*/)
    {
    }

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
//
void CPosLmXmlParser::OnSkippedEntityL( const RString& /*aName*/, TInt /*aErrorCode*/)
    {
    }

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
//
void CPosLmXmlParser::OnProcessingInstructionL( const TDesC8& /*aTarget*/,
        const TDesC8& /*aData*/, TInt /*aErrorCode*/)
    {
    }

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
//
void CPosLmXmlParser::OnOutOfData()
    {
    }

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
//
void CPosLmXmlParser::OnError( TInt aErrorCode )
    {
    iParserError = aErrorCode;
    }

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
//
TAny* CPosLmXmlParser::GetExtendedInterface( const TInt32 /*aUid*/)
    {
    return NULL;
    }

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
//
void CPosLmXmlParser::SetInputFileInternalL( RFile* aFileHandle,
        const TDesC& aFile )
    {
    __ASSERT_ALWAYS(!( iParserStatus & EStateParsing ), Panic( KPosLandmarksClientPanic,
            EPosLmProtocolBreak ) );

    ResetParser();

    if (!aFileHandle )
        {
        User::LeaveIfError( iFs.Connect() );
        User::LeaveIfError( iFile.Open( iFs, aFile,
                EFileShareReadersOnly | EFileRead ) );
        iFileHandle = &iFile;
        }
    else
        {
        iFileHandle = aFileHandle;
        }

    // Set size
    User::LeaveIfError( iFileHandle->Size( iContentSize ) );

    iParserStatus |= EStateInitialized;
    }

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
//
TBool CPosLmXmlParser::IsParsingComplete()
    {
    return(( iParserStatus & EStateParsingReady || !( BufferLength() > 0 )) &&
            (iParsedLmBuffer.Count() == 1  || iParsedLmBuffer.Count() == 0));
    }

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
//
TInt CPosLmXmlParser::CompleteParsing()
    {
    TInt err = KErrNone;
    if ( iXmlParser &&( iParserStatus & EStateParsingStarted ) )
        {
        TRAP( err, iXmlParser->ParseEndL() );
        }
    return err;
    }

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
//
void CPosLmXmlParser::AddCollectionDataL( 
    TPosLmCollectionDataId aCollId,
    const TDesC& aContent )
    {
    CPosLmCollectionData* colldata = CPosLmCollectionData::NewLC( aContent,  aCollId );
    User::LeaveIfError( iCollectionData.Append( colldata ) );
    CleanupStack::Pop( colldata );
    }

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
//
void CPosLmXmlParser::ResetParser()
    {
    iInputBuffer.Set( NULL, 0 );

	CloseCurrentFileHandle();

	iFs.Close();
    iParserStatus &= ~EStateInitialized;
    iParserStatus &= ~EStateOperationConstructed;
    ResetParserState();
    }

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
//
void CPosLmXmlParser::ResetParserState()
    {
    ResetParserOnError();

    iNumOfParsedLandmarks = 0;

    delete iXmlParser;
    iXmlParser = NULL;

    delete iBuffer;
    iBuffer = NULL;
    delete iReadContent;
    iReadContent = NULL;

    delete iWorkingLandmark;
    iWorkingLandmark = NULL;
    delete iWorkingCategory;
    iWorkingCategory = NULL;
    iCategories.ResetAndDestroy();
    iParsedLmBuffer.ResetAndDestroy();

    iCollectionData.ResetAndDestroy();
    
    iLastElementState = EXmlParserOnEndElement;
    
    iReadTags.Close();
    iTrees.Close();
    delete iRootNs;
    iRootNs = NULL;
    }

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
//
void CPosLmXmlParser::ResetParserPartState()
    {
    delete iReadMimeType;
    iReadMimeType = NULL;
    delete iReadUrl;
    iReadUrl = NULL;

    PosLmConverterUtils::SetNaN( iParsedLatitude );
    PosLmConverterUtils::SetNaN( iParsedLongitude );
    PosLmConverterUtils::SetNaN( iParsedAltitude );
    PosLmConverterUtils::SetNaN( iParsedHorAcc );
    PosLmConverterUtils::SetNaN( iParsedVertAcc );
    }

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
//
TInt CPosLmXmlParser::BufferLength() const
    {
    return iBuffer->Length();
    }

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
//
TBool CPosLmXmlParser::ParsedPart()
    {
    return( iParserStatus & EStateParsingForNextLm /*EStateParsingPartReady*/ ) ;
    }

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
//
void CPosLmXmlParser::ReadNextPartL()
    {
    // Read next chunk from file/buffer
    TPtr8 ptr = iBuffer->Des();
    if ( iInputBuffer.Ptr() )
        {
        TInt length = KPosLmParserBytesAtATime;
        if ( iNumberOfBytesRead + length > iInputBuffer.Length() )
            {
            length = iInputBuffer.Length()- iNumberOfBytesRead;
            }
        ptr = iInputBuffer.Mid( iNumberOfBytesRead, length );
        iNumberOfBytesRead += length;
        }
    else
        {
        User::LeaveIfError( iFileHandle->Read( ptr ) );
        iNumberOfBytesRead += BufferLength();
        }
    }

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
//
void CPosLmXmlParser::SetParserStateL( TPosXmlTagType aTag )
    {
    // Check status of landmark parsing
    if ( IsLandmarkStarted( aTag  ) )
        {
        // If we have found an earlier landmark then
        // we know we are ready parsing a part for this time.
        if ( iParserStatus & EStateParsedLandmark )
            {
            iParserStatus |= EStateParsingForNextLm; //EStateParsingPartReady;
            }

        iWorkingLandmark = CPosLandmark::NewL();
        iCategories.Reset();
        iWorkingItemId = 1;
        iNoOfMediaLinks = 0;
        }
    else
        {
        if ( IsCategoryStarted ( aTag ) )
            {
            iWorkingCategory = CPosLandmarkCategory::NewL();
            }
        }
    }

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
//
void CPosLmXmlParser::DisconnectOperation()
    {
    if ( iParserOp )
        {
        iParserOp->DisconnectParser();
        iParserOp = NULL;
        }
    }

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
//
void CPosLmXmlParser::OperationDestroyed()
    {
    iParserOp = NULL;
    }

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
//
TBool CPosLmXmlParser::EqualsTagL( const TDesC8& aTag, const TDesC& aDefinedTag )
    {
    HBufC* tag = HBufC::NewLC( aTag.Length() );
    TPtr16 tagptr = tag->Des();

    PosLmConverterUtils::Utf8ToUnicodeL( aTag, tagptr );

    TBool answer = ( tag->CompareF( aDefinedTag ) == 0 );
    CleanupStack::PopAndDestroy( tag );

    return answer;
    }

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
//
TBool CPosLmXmlParser::MatchesRootNamespace( const Xml::RTagInfo& aElement )
    {
    if ( iRootNs )
        {
        return ( iRootNs->Compare( aElement.Uri().DesC() ) == 0 );
        }
    else
        {
        return EFalse;
        }
    }

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
//
void CPosLmXmlParser::HandleLandmarkParsedL()
    {
    // Set parser tag/states
    iParserStatus |= EStateParsedLandmark;
    
    // Set last sucessfully parsed landmark/categories for later use
    CPosLmParsedLmBuffer* buf = CPosLmParsedLmBuffer::NewLC( *iWorkingLandmark, iCategories );

    // Reset current landmark/category
    delete iWorkingLandmark;
    iWorkingLandmark = NULL;
    iCategories.Reset();
    
    User::LeaveIfError( iParsedLmBuffer.Append( buf ) );
    CleanupStack::Pop( buf );
    
    ResetParserPartState();
    }

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
//
void CPosLmXmlParser::HandleRootTag()
    {
    // Unset parser tag/states
    iParserStatus |= EStateParsingReady;
    }

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
// 
void CPosLmXmlParser::StartNewTreeL()
    {
    __ASSERT_DEBUG( iReadTags.Count(), Panic( KPosLmConverterPanic, KErrGeneral ) );
    iTrees.AppendL( iReadTags.Count() - 1 );
    }

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
// 
void CPosLmXmlParser::CloseCurrentTree()
    {
    __ASSERT_DEBUG( iTrees.Count(), Panic( KPosLmConverterPanic, KErrGeneral ) );
    RemoveChildTags();
    iTrees.Remove( iTrees.Count() - 1 );
    }

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
//
void CPosLmXmlParser::RemoveChildTags()
    {
    TInt treeIndex = CurrentTreeTagIndex();
    // treeIndex item itself should remain in the list!
    for ( TInt i = iReadTags.Count() - 1; i > treeIndex; --i )
        {
        iReadTags.Remove( i );
        }
    }

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
//
TBool CPosLmXmlParser::ChildTagFound( TPosXmlTagType aTag )
    {
    for ( TInt i = CurrentTreeTagIndex() + 1; i < iReadTags.Count(); i++ ) 
        {
        if ( iReadTags[i] == aTag )
            {
            return ETrue;
            }
        }
    return EFalse;
    }

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
// 
TPosXmlTagType CPosLmXmlParser::CurrentTree()
    {
    TInt currentTreeTagIndex = CurrentTreeTagIndex();
    if ( currentTreeTagIndex >= 0 )
        {
        __ASSERT_DEBUG( iReadTags.Count() > currentTreeTagIndex, Panic( KPosLmConverterPanic, KErrGeneral ) );
        return iReadTags[ currentTreeTagIndex ];
        }
    else
        {
        return EPosXmlNothing;
        }
    }

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
// 
TInt CPosLmXmlParser::CurrentTreeTagIndex()
    {
    if ( iTrees.Count() )
        {
        return iTrees[ iTrees.Count() - 1 ]; // last element in iTrees
        }
    else
        {
        return -1;
        }
    }

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
// 
TPosXmlTagType CPosLmXmlParser::CurrentTag()
    {
    if ( iReadTags.Count() ) 
        {
        return iReadTags[iReadTags.Count() - 1];
        }
    return EPosXmlNothing;
    }

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
// 
TPosXmlTagType CPosLmXmlParser::LastKnownChildTag()
    {
    TInt treeIndex = CurrentTreeTagIndex();
    for ( TInt i = iReadTags.Count() - 1; i > treeIndex; i-- )
        {
        if ( iReadTags[i] != EPosXmlUnknown ) 
            {
            return iReadTags[i];
            }
        }
    return EPosXmlNothing;
    }

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
// 
void CPosLmXmlParser::GenValidSequenceL( 
    const TPosXmlTagType* aChildTags,
    TInt aChildTagsCount,
    RArray<TPosXmlTagType>& aSequence )
    {
    TInt i = 0;
    TPosXmlTagType lastChildTag = LastKnownChildTag();

    if ( lastChildTag != EPosXmlNothing )
        {
        // find last child tag, already parsed
        for ( ; i < aChildTagsCount; i++ )
            {
            if ( *( aChildTags + i ) == lastChildTag )
                {
                i++;
                break;
                }
            }
        }
    
    // add the rest of child tags as allowed for this sequence
    for ( ; i < aChildTagsCount; i++ )
        {
        aSequence.AppendL( *( aChildTags + i ) );
        }
    }

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
// 
void CPosLmXmlParser::GenValidSetL( 
    const TPosXmlTagType* aChildTags,
    TInt aChildTagsCount,
    RArray<TPosXmlTagType>& aSet )
    {
    // valid set contains all tags, not yet parsed
    for ( TInt i = 0 ; i < aChildTagsCount; i++ ) 
        {
        TPosXmlTagType tag = *( aChildTags + i );
        if ( !ChildTagFound( tag ) )
            {
            aSet.AppendL( tag );
            }
        }
    }

//--------------------------------------------------------------------------------
//----------------------------------------------------------------------------------
//

void CPosLmXmlParser::ResetContentL()
{
	// Reset parsers state
    ResetParserState();
    // Set file pointer
    if ( iFileHandle )
        {
        TInt filepos = 0;
        User::LeaveIfError( iFileHandle->Seek( ESeekStart, filepos ) );
        }

    iBuffer = HBufC8::NewL( KPosLmParserBytesAtATime );

    // Create Symbian XML parser
    _LIT8( KXmlMimeType, "text/xml" );
    iXmlParser = Xml::CParser::NewL( KXmlMimeType, *this );
    iXmlParser->ParseBeginL();
    iParserStatus |= EStateOperationConstructed;
}

//------------------------------------------------------------------------------
//-----------------------------------------------------------------------------
//
void CPosLmXmlParser::CloseCurrentFileHandle()
{
	if(iFileHandle)
	{
		iFileHandle = NULL;
    	iFile.Close();
	}
}


