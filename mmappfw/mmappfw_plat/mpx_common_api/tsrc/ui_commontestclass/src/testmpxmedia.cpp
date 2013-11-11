/*
* Copyright (c) 2002 Nokia Corporation and/or its subsidiary(-ies).
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
* Description:  MPXMedia and MPXMediaArray testing implementation (mpxmedia.h,mpxmediaarray.h)
*
*/


#include <s32mem.h>
#include <mpxmedia.h>
#include <mpxmediaarray.h>
#include <mpxcollectionpath.h>
#include "commontestclass.h"

// Begin MPXMedia and MPXMediaArray testing implementation (mpxmedia.h,mpxmediaarray.h)========================

// -----------------------------------------------------------------------------
// CCommonTestClass::MpxMediaNewL()
// Returns: Symbian OS errors.
// -----------------------------------------------------------------------------
TInt CCommonTestClass::MpxMediaNewL(CStifItemParser& /*aItem*/)
    {
    FTRACE(FPrint(_L("CCommonTestClass::MpxMediaNewL testing CMPXMedia::NewL() begin")));
    iLog->Log(_L("CCommonTestClass::MpxMediaNewL testing CMPXMedia::NewL() begin"));
    TInt err=KErrNone;
    CMPXMedia* media = CMPXMedia::NewL();
    CleanupStack::PushL(media);
    if ( iMPXMediaArray )
        {
        iMPXMediaArray->AppendL(media);
        CleanupStack::Pop(media);
        }
    else
        {
        CleanupStack::PopAndDestroy(media);
        }
    return err;
    }

// -----------------------------------------------------------------------------
// CCommonTestClass::MpxMediaNewLSupportedIdsL()
// Returns: Symbian OS errors.
// -----------------------------------------------------------------------------
TInt CCommonTestClass::MpxMediaNewLSupportedIdsL(CStifItemParser& aItem)
    {
    FTRACE(FPrint(_L("CCommonTestClass::MpxMediaNewLSupportedIdsL testing CMPXMedia::NewL() begin")));
    iLog->Log(_L("CCommonTestClass::MpxMediaNewLSupportedIdsL testing CMPXMedia::NewL() begin"));
    TInt err=KErrNone;
    RArray<TInt> supportedIds;
    TInt id;
    
    CleanupClosePushL(supportedIds);
    // read in parameters
    while ( aItem.GetNextInt(id) == KErrNone )
        {
        supportedIds.AppendL(id);
        }

    CMPXMedia* media = CMPXMedia::NewL(supportedIds.Array());
    CleanupStack::PushL(media);
    if ( iMPXMediaArray )
        {
        iMPXMediaArray->AppendL(media);
        CleanupStack::Pop(media);
        }
    else
        {
        CleanupStack::PopAndDestroy(media);
        }
    CleanupStack::PopAndDestroy(&supportedIds);
    return err;
    }

// -----------------------------------------------------------------------------
// CCommonTestClass::MpxMediaNewLMediaL()
// Returns: Symbian OS errors.
// -----------------------------------------------------------------------------
TInt CCommonTestClass::MpxMediaNewLMediaL(CStifItemParser& aItem)
    {
    FTRACE(FPrint(_L("CCommonTestClass::MpxMediaNewLMediaL testing CMPXMedia::NewL() begin")));
    iLog->Log(_L("CCommonTestClass::MpxMediaNewLMediaL testing CMPXMedia::NewL() begin"));
    TInt err=KErrNone;
    TUint index;
    
    // read in parameters
    if ( aItem.GetNextInt(index) )
        {
        iLog->Log(_L("Missing Parameter: index."));
        return KErrBadTestParameter;
        }
    if ( !iMPXMediaArray )
        {
        iLog->Log(_L("Error: MPXMediaArray not created."));
        return KErrBadTestParameter;
        }
    if ( index >= iMPXMediaArray->Count() )
        {
        iLog->Log(_L("Bad Parameter: index out of range."));
        return KErrBadTestParameter;
        }
    
    CMPXMedia* media = CMPXMedia::NewL(*(*iMPXMediaArray)[index]);
    CleanupStack::PushL(media);
    iMPXMediaArray->AppendL(media);
    CleanupStack::Pop(media);
    return err;
    }

// -----------------------------------------------------------------------------
// CCommonTestClass::MpxMediaNewLDataHandleL()
// Returns: Symbian OS errors.
// -----------------------------------------------------------------------------
TInt CCommonTestClass::MpxMediaNewLDataHandleL(CStifItemParser& aItem)
    {
    FTRACE(FPrint(_L("CCommonTestClass::MpxMediaNewLDataHandleL testing CMPXMedia::NewL() begin")));
    iLog->Log(_L("CCommonTestClass::MpxMediaNewLDataHandleL testing CMPXMedia::NewL() begin"));
    TInt err=KErrNone;
    TUint index;
    
    // read in parameters
    if ( aItem.GetNextInt(index) )
        {
        iLog->Log(_L("Missing Parameter: index."));
        return KErrBadTestParameter;
        }
    if ( !iMPXMediaArray )
        {
        iLog->Log(_L("Error: MPXMediaArray not created."));
        return KErrBadTestParameter;
        }
    if ( index >= iMPXMediaArray->Count() )
        {
        iLog->Log(_L("Bad Parameter: index out of range."));
        return KErrBadTestParameter;
        }
    
    CMPXMedia* media = CMPXMedia::NewL((*iMPXMediaArray)[index]->Data());
    CleanupStack::PushL(media);
    iMPXMediaArray->AppendL(media);
    CleanupStack::Pop(media);
    return err;
    }

// -----------------------------------------------------------------------------
// CCommonTestClass::MpxMediaNewLStreamL()
// Returns: Symbian OS errors.
// -----------------------------------------------------------------------------
TInt CCommonTestClass::MpxMediaNewLStreamL(CStifItemParser& aItem)
    {
    FTRACE(FPrint(_L("CCommonTestClass::MpxMediaNewLStreamL testing CMPXMedia::NewL() begin")));
    iLog->Log(_L("CCommonTestClass::MpxMediaNewLStreamL testing CMPXMedia::NewL() begin"));
    TInt err=KErrNone;
    TUint index;
    
    // read in parameters
    if ( aItem.GetNextInt(index) )
        {
        iLog->Log(_L("Missing Parameter: index."));
        return KErrBadTestParameter;
        }
    if ( !iMPXMediaArray )
        {
        iLog->Log(_L("Error: MPXMediaArray not created."));
        return KErrBadTestParameter;
        }
    if ( index >= iMPXMediaArray->Count() )
        {
        iLog->Log(_L("Bad Parameter: index out of range."));
        return KErrBadTestParameter;
        }

    CBufBase* buffer = CBufFlat::NewL( 100 );
    CleanupStack::PushL( buffer );
    RBufWriteStream writeStream( *buffer );
    CleanupClosePushL( writeStream );
    (*iMPXMediaArray)[index]->ExternalizeL( writeStream );
    writeStream.CommitL();
    buffer->Compress();
    CleanupStack::PopAndDestroy( &writeStream );

    RBufReadStream readStream( *buffer );
    CleanupClosePushL( readStream );                
    CMPXMedia* media = CMPXMedia::NewL(readStream);
    CleanupStack::PushL(media);
    iMPXMediaArray->AppendL(media);
    CleanupStack::Pop(media);
    CleanupStack::PopAndDestroy( &readStream );
    CleanupStack::PopAndDestroy( buffer );
    return err;
    }

// -----------------------------------------------------------------------------
// CCommonTestClass::MpxMediaOperatorAssignment()
// Returns: Symbian OS errors.
// -----------------------------------------------------------------------------
TInt CCommonTestClass::MpxMediaOperatorAssignment(CStifItemParser& aItem)
    {
    FTRACE(FPrint(_L("CCommonTestClass::MpxMediaOperatorAssignment testing CMPXMedia::operator=() begin")));
    iLog->Log(_L("CCommonTestClass::MpxMediaOperatorAssignment testing CMPXMedia::operator=() begin"));
    TInt err=KErrNone;
    TUint index;

    // read in parameters
    if ( aItem.GetNextInt(index) )
        {
        iLog->Log(_L("Missing Parameter: index."));
        return KErrBadTestParameter;
        }
    if ( !iMPXMediaArray )
        {
        iLog->Log(_L("Error: MPXMediaArray not created."));
        return KErrBadTestParameter;
        }
    if ( index >= iMPXMediaArray->Count() )
        {
        iLog->Log(_L("Bad Parameter: index out of range."));
        return KErrBadTestParameter;
        }

    CMPXMedia* media( NULL );
    TRAP(err , media= CMPXMedia::NewL());
    CleanupStack::PushL(media);
    media->operator =(*(*iMPXMediaArray)[index]);
    TRAP(err , iMPXMediaArray->AppendL(media));
    CleanupStack::Pop(media);
    return err;
    }

// -----------------------------------------------------------------------------
// CCommonTestClass::MpxMediaSupportedIds()
// Returns: Symbian OS errors.
// -----------------------------------------------------------------------------
TInt CCommonTestClass::MpxMediaSupportedIds(CStifItemParser& aItem)
    {
    FTRACE(FPrint(_L("CCommonTestClass::MpxMediaSupportedIds testing CMPXMedia::SupportedIds() begin")));
    iLog->Log(_L("CCommonTestClass::MpxMediaSupportedIds testing CMPXMedia::SupportedIds() begin"));
    TInt err=KErrNone;
    TUint index;
    RArray<TInt> expectedSupportedIds;
    TInt id;

    // read in parameters
    if ( aItem.GetNextInt(index) )
        {
        iLog->Log(_L("Missing Parameter: index."));
        return KErrBadTestParameter;
        }
    if ( !iMPXMediaArray )
        {
        iLog->Log(_L("Error: MPXMediaArray not created."));
        return KErrBadTestParameter;
        }
    if ( index >= iMPXMediaArray->Count() )
        {
        iLog->Log(_L("Bad Parameter: index out of range."));
        return KErrBadTestParameter;
        }

    while ( aItem.GetNextInt(id) == KErrNone )
        {
        expectedSupportedIds.Append(id);
        }
    TArray<TInt> supportedIds = (*iMPXMediaArray)[index]->SupportedIds();
    
    // verify supported IDS
    if ( supportedIds.Count() == expectedSupportedIds.Count() )
        {
        for (TInt i=0; i<supportedIds.Count(); i++)
            {
            if ( expectedSupportedIds.Find(supportedIds[i]) == KErrNotFound )
                {
                iLog->Log(_L("Verification Failed: ContentID not Found = %d."), supportedIds[i]);
                err = KErrUnexpectedValue;
                }
            }
        }
    else
        {
        iLog->Log(_L("Verification Failed: count=%d, expected count=%d."), supportedIds.Count(), expectedSupportedIds.Count());
        err = KErrUnexpectedValue;
        }
    
    return err;
    }

// -----------------------------------------------------------------------------
// CCommonTestClass::MpxMediaAttributes()
// Returns: Symbian OS errors.
// -----------------------------------------------------------------------------
TInt CCommonTestClass::MpxMediaAttributes(CStifItemParser& aItem)
    {
    FTRACE(FPrint(_L("CCommonTestClass::MpxMediaAttributes testing CMPXMedia::Attributes() begin")));
    iLog->Log(_L("CCommonTestClass::MpxMediaAttributes testing CMPXMedia::Attributes() begin"));
    TInt err=KErrNone;
    TUint mediaIndex;
    TUint attIndex;
    
    // read in parameters
    if ( aItem.GetNextInt(mediaIndex) )
        {
        iLog->Log(_L("Missing Parameter: Media index."));
        return KErrBadTestParameter;
        }
    if ( !iMPXMediaArray )
        {
        iLog->Log(_L("Error: MPXMediaArray not created."));
        return KErrBadTestParameter;
        }
    if ( mediaIndex >= iMPXMediaArray->Count() )
        {
        iLog->Log(_L("Bad Parameter: Media index out of range."));
        return KErrBadTestParameter;
        }
    if ( aItem.GetNextInt(attIndex) )
        {
        iLog->Log(_L("Missing Parameter: Attribute index."));
        return KErrBadTestParameter;
        }
    if ( attIndex >= iMPXAttArray.Count() )
        {
        iLog->Log(_L("Bad Parameter: Attribute index out of range."));
        return KErrBadTestParameter;
        }
    
    TArray<TMPXAttribute> attributes = (*iMPXMediaArray)[mediaIndex]->Attributes();
    // verify attributes
    TBool found = EFalse;
    for (TInt i=0; i<attributes.Count(); i++)
        {
        if ( iMPXAttArray[attIndex] == attributes[i] )
            {
            found = ETrue;
            }
        }
    if ( !found )
        {
        iLog->Log(_L("Verification Failed: Attribute not found - contentID=%d, attID=%d."), 
                iMPXAttArray[attIndex].ContentId(), iMPXAttArray[attIndex].AttributeId());
        err = KErrUnexpectedValue;
        }
    return err;
    }

// -----------------------------------------------------------------------------
// CCommonTestClass::MpxMediaSetTObjectValueL()
// Returns: Symbian OS errors.
// -----------------------------------------------------------------------------
TInt CCommonTestClass::MpxMediaSetTObjectValueL(CStifItemParser& aItem)
    {
    FTRACE(FPrint(_L("CCommonTestClass::MpxMediaSetTObjectValueL testing CMPXMedia::SetTObjectValueL() begin")));
    iLog->Log(_L("CCommonTestClass::MpxMediaSetTObjectValueL testing CMPXMedia::SetTObjectValueL() begin"));
    TInt err=KErrNone;
    TUint mediaIndex;
    TUint attIndex;
    TInt attValue;
    
    // read in parameters
    if ( aItem.GetNextInt(mediaIndex) )
        {
        iLog->Log(_L("Missing Parameter: Media index."));
        return KErrBadTestParameter;
        }
    if ( !iMPXMediaArray )
        {
        iLog->Log(_L("Error: MPXMediaArray not created."));
        return KErrBadTestParameter;
        }
    if ( mediaIndex >= iMPXMediaArray->Count() )
        {
        iLog->Log(_L("Bad Parameter: Media index out of range."));
        return KErrBadTestParameter;
        }
    if ( aItem.GetNextInt(attIndex) )
        {
        iLog->Log(_L("Missing Parameter: Attribute index."));
        return KErrBadTestParameter;
        }
    if ( attIndex >= iMPXAttArray.Count() )
        {
        iLog->Log(_L("Bad Parameter: Attribute index out of range."));
        return KErrBadTestParameter;
        }
    if ( aItem.GetNextInt(attValue) )
        {
        iLog->Log(_L("Missing Parameter: Attribute value."));
        return KErrBadTestParameter;
        }
    
    // set attribue
    (*iMPXMediaArray)[mediaIndex]->SetTObjectValueL(iMPXAttArray[attIndex], attValue);
    return err;
    }

// -----------------------------------------------------------------------------
// CCommonTestClass::MpxMediaSetCObjectValueL()
// Returns: Symbian OS errors.
// -----------------------------------------------------------------------------
TInt CCommonTestClass::MpxMediaSetCObjectValueL(CStifItemParser& aItem)
    {
    FTRACE(FPrint(_L("CCommonTestClass::MpxMediaSetCObjectValueL testing CMPXMedia::SetCObjectValueL() begin")));
    iLog->Log(_L("CCommonTestClass::MpxMediaSetCObjectValueL testing CMPXMedia::SetCObjectValueL() begin"));
    TInt err=KErrNone;
    TUint mediaIndex;
    TUint attIndex;
    TPtrC className;
    
    // read in parameters
    if ( aItem.GetNextInt(mediaIndex) )
        {
        iLog->Log(_L("Missing Parameter: Media index."));
        return KErrBadTestParameter;
        }
    if ( !iMPXMediaArray )
        {
        iLog->Log(_L("Error: MPXMediaArray not created."));
        return KErrBadTestParameter;
        }
    if ( mediaIndex >= iMPXMediaArray->Count() )
        {
        iLog->Log(_L("Bad Parameter: Media index out of range."));
        return KErrBadTestParameter;
        }
    if ( aItem.GetNextInt(attIndex) )
        {
        iLog->Log(_L("Missing Parameter: Attribute index."));
        return KErrBadTestParameter;
        }
    if ( attIndex >= iMPXAttArray.Count() )
        {
        iLog->Log(_L("Bad Parameter: Attribute index out of range."));
        return KErrBadTestParameter;
        }
    if ( aItem.GetNextString(className) )
        {
        iLog->Log(_L("Missing Parameter: Class name."));
        return KErrBadTestParameter;
        }
   
    // set class object
    if ( className == _L("CMPXMedia") )
        {
        CMPXMedia* media = CMPXMedia::NewL();
        CleanupStack::PushL(media);
        (*iMPXMediaArray)[mediaIndex]->SetCObjectValueL(iMPXAttArray[attIndex], media);
        CleanupStack::PopAndDestroy(media);
        }
    else if ( className == _L("CMPXMediaArray") )
        {
        CMPXMediaArray* media = CMPXMediaArray::NewL();
        CleanupStack::PushL(media);
        (*iMPXMediaArray)[mediaIndex]->SetCObjectValueL(iMPXAttArray[attIndex], media);
        CleanupStack::PopAndDestroy(media);
        }
    else if ( className == _L("CMPXCollectionPath") )
        {
        CMPXCollectionPath* media = CMPXCollectionPath::NewL();
        CleanupStack::PushL(media);
        (*iMPXMediaArray)[mediaIndex]->SetCObjectValueL(iMPXAttArray[attIndex], media);
        CleanupStack::PopAndDestroy(media);
        }
    else
        {
        iLog->Log(_L("Bad Parameter: Invalid class name."));
        return KErrBadTestParameter;
        }
    return err;
    }

// -----------------------------------------------------------------------------
// CCommonTestClass::MpxMediaSetNoNewLCObjectL()
// Returns: Symbian OS errors.
// -----------------------------------------------------------------------------
TInt CCommonTestClass::MpxMediaSetNoNewLCObjectL(CStifItemParser& aItem)
    {
    FTRACE(FPrint(_L("CCommonTestClass::MpxMediaSetNoNewLCObjectL testing CMPXMedia::SetNoNewLCObjectL() begin")));
    iLog->Log(_L("CCommonTestClass::MpxMediaSetNoNewLCObjectL testing CMPXMedia::SetNoNewLCObjectL() begin"));
    TInt err=KErrNone;
    TUint mediaIndex;
    TUint attIndex;
    TPtrC className;

    // read in parameters
    if ( aItem.GetNextInt(mediaIndex) )
        {
        iLog->Log(_L("Missing Parameter: Media index."));
        return KErrBadTestParameter;
        }
    if ( !iMPXMediaArray )
        {
        iLog->Log(_L("Error: MPXMediaArray not created."));
        return KErrBadTestParameter;
        }
    if ( mediaIndex >= iMPXMediaArray->Count() )
        {
        iLog->Log(_L("Bad Parameter: Media index out of range."));
        return KErrBadTestParameter;
        }
    if ( aItem.GetNextInt(attIndex) )
        {
        iLog->Log(_L("Missing Parameter: Attribute index."));
        return KErrBadTestParameter;
        }
    if ( attIndex >= iMPXAttArray.Count() )
        {
        iLog->Log(_L("Bad Parameter: Attribute index out of range."));
        return KErrBadTestParameter;
        }
    if ( aItem.GetNextString(className) )
        {
        iLog->Log(_L("Missing Parameter: Class name."));
        return KErrBadTestParameter;
        }

    // set class object
    if ( className == _L("CMPXMedia") )
        {
        CMPXMedia* media = CMPXMedia::NewL();
        CleanupStack::PushL(media);
        (*iMPXMediaArray)[mediaIndex]->SetNoNewLCObjectL(iMPXAttArray[attIndex], media);
        CleanupStack::PopAndDestroy(media);
        }
    else if ( className == _L("CMPXMediaArray") )
        {
        CMPXMediaArray* media = CMPXMediaArray::NewL();
        CleanupStack::PushL(media);
        (*iMPXMediaArray)[mediaIndex]->SetNoNewLCObjectL(iMPXAttArray[attIndex], media);
        CleanupStack::PopAndDestroy(media);
        }
    else if ( className == _L("CMPXCollectionPath") )
        {
        CMPXCollectionPath* media = CMPXCollectionPath::NewL();
        CleanupStack::PushL(media);
        (*iMPXMediaArray)[mediaIndex]->SetNoNewLCObjectL(iMPXAttArray[attIndex], media);
        CleanupStack::PopAndDestroy(media);
        }
    else
        {
        iLog->Log(_L("Bad Parameter: Invalid class name."));
        return KErrBadTestParameter;
        }
    return err;
    }

// -----------------------------------------------------------------------------
// CCommonTestClass::MpxMediaSetTextValueL()
// Returns: Symbian OS errors.
// -----------------------------------------------------------------------------
TInt CCommonTestClass::MpxMediaSetTextValueL(CStifItemParser& aItem)
    {
    FTRACE(FPrint(_L("CCommonTestClass::MpxMediaSetTextValueL testing CMPXMedia::SetTextValueL() begin")));
    iLog->Log(_L("CCommonTestClass::MpxMediaSetTextValueL testing CMPXMedia::SetTextValueL() begin"));
    TInt err=KErrNone;
    TUint mediaIndex;
    TUint attIndex;
    TPtrC attValue;
    
    // read in parameters
    if ( aItem.GetNextInt(mediaIndex) )
        {
        iLog->Log(_L("Missing Parameter: Media index."));
        return KErrBadTestParameter;
        }
    if ( !iMPXMediaArray )
        {
        iLog->Log(_L("Error: MPXMediaArray not created."));
        return KErrBadTestParameter;
        } 
    if ( mediaIndex >= iMPXMediaArray->Count() )
        {
        iLog->Log(_L("Bad Parameter: Media index out of range."));
        return KErrBadTestParameter;
        }
    if ( aItem.GetNextInt(attIndex) )
        {
        iLog->Log(_L("Missing Parameter: Attribute index."));
        return KErrBadTestParameter;
        }
    if ( attIndex >= iMPXAttArray.Count() )
        {
        iLog->Log(_L("Bad Parameter: Attribute index out of range."));
        return KErrBadTestParameter;
        }
    if ( aItem.GetNextString(attValue) )
        {
        iLog->Log(_L("Missing Parameter: Attribute value."));
        return KErrBadTestParameter;
        }
    
    // set attribue
    (*iMPXMediaArray)[mediaIndex]->SetTextValueL(iMPXAttArray[attIndex], attValue);
    return err;
    }

// -----------------------------------------------------------------------------
// CCommonTestClass::MpxMediaIsSupported()
// Returns: Symbian OS errors.
// -----------------------------------------------------------------------------
TInt CCommonTestClass::MpxMediaIsSupported(CStifItemParser& aItem)
    {
    FTRACE(FPrint(_L("CCommonTestClass::MpxMediaIsSupported testing CMPXMedia::IsSupported() begin")));
    iLog->Log(_L("CCommonTestClass::MpxMediaIsSupported testing CMPXMedia::IsSupported() begin"));
    TInt err=KErrNone;
    TUint mediaIndex;
    TUint attIndex;
    TBool expectedResult;
    
    // read in parameters
    if ( aItem.GetNextInt(mediaIndex) )
        {
        iLog->Log(_L("Missing Parameter: Media index."));
        return KErrBadTestParameter;
        }
    if ( !iMPXMediaArray )
        {
        iLog->Log(_L("Error: MPXMediaArray not created."));
        return KErrBadTestParameter;
        }
    if ( mediaIndex >= iMPXMediaArray->Count() )
        {
        iLog->Log(_L("Bad Parameter: Media index out of range."));
        return KErrBadTestParameter;
        }
    if ( aItem.GetNextInt(attIndex) )
        {
        iLog->Log(_L("Missing Parameter: Attribute index."));
        return KErrBadTestParameter;
        }
    if ( attIndex >= iMPXAttArray.Count() )
        {
        iLog->Log(_L("Bad Parameter: Attribute index out of range."));
        return KErrBadTestParameter;
        }
    if ( aItem.GetNextInt(expectedResult) )
        {
        iLog->Log(_L("Missing Parameter: Expected result."));
        return KErrBadTestParameter;
        }
    
    // call IsSupported
    TBool result = (*iMPXMediaArray)[mediaIndex]->IsSupported(iMPXAttArray[attIndex]);
    
    // verification
    if ( result != expectedResult )
        {
        iLog->Log(_L("Verification Failed: result=%d, expectedResult=%d."), result, expectedResult);
        err = KErrUnexpectedValue;
        }
    return err;
    }

// -----------------------------------------------------------------------------
// CCommonTestClass::MpxMediaCount()
// Returns: Symbian OS errors.
// -----------------------------------------------------------------------------
TInt CCommonTestClass::MpxMediaCount(CStifItemParser& aItem)
    {
    FTRACE(FPrint(_L("CCommonTestClass::MpxMediaCount testing CMPXMedia::Count() begin")));
    iLog->Log(_L("CCommonTestClass::MpxMediaCount testing CMPXMedia::Count() begin"));
    TInt err=KErrNone;
    TUint mediaIndex;
    TInt expectedResult;
    
    // read in parameters
    if ( aItem.GetNextInt(mediaIndex) )
        {
        iLog->Log(_L("Missing Parameter: Media index."));
        return KErrBadTestParameter;
        }
    if ( !iMPXMediaArray )
        {
        iLog->Log(_L("Error: MPXMediaArray not created."));
        return KErrBadTestParameter;
        }
    if ( mediaIndex >= iMPXMediaArray->Count() )
        {
        iLog->Log(_L("Bad Parameter: Media index out of range."));
        return KErrBadTestParameter;
        }
    if ( aItem.GetNextInt(expectedResult) )
        {
        iLog->Log(_L("Missing Parameter: Expected result."));
        return KErrBadTestParameter;
        }
    
    // call Count()
    TInt result = (*iMPXMediaArray)[mediaIndex]->Count();

    // verification
    if ( result != expectedResult )
        {
        iLog->Log(_L("Verification Failed: result=%d, expectedResult=%d."), result, expectedResult);
        err = KErrUnexpectedValue;
        }
    return err;
    }

// -----------------------------------------------------------------------------
// CCommonTestClass::MpxMediaAttribute()
// Returns: Symbian OS errors.
// -----------------------------------------------------------------------------
TInt CCommonTestClass::MpxMediaAttribute(CStifItemParser& aItem)
    {
    FTRACE(FPrint(_L("CCommonTestClass::MpxMediaAttribute testing CMPXMedia::Attribute() begin")));
    iLog->Log(_L("CCommonTestClass::MpxMediaAttribute testing CMPXMedia::Attribute() begin"));
    TInt err=KErrNone;
    TUint mediaIndex;
    TUint attIndex;
    TInt expectedAttIndex;
    
    // read in parameters
    if ( aItem.GetNextInt(mediaIndex) )
        {
        iLog->Log(_L("Missing Parameter: Media index."));
        return KErrBadTestParameter;
        }
    if ( !iMPXMediaArray )
        {
        iLog->Log(_L("Error: MPXMediaArray not created."));
        return KErrBadTestParameter;
        }
    if ( mediaIndex >= iMPXMediaArray->Count() )
        {
        iLog->Log(_L("Bad Parameter: Media index out of range."));
        return KErrBadTestParameter;
        }
    if ( aItem.GetNextInt(attIndex) )
        {
        iLog->Log(_L("Missing Parameter: Index of Attribute in MPXMedia."));
        return KErrBadTestParameter;
        }
    if ( attIndex >= (*iMPXMediaArray)[mediaIndex]->Count() )
        {
        iLog->Log(_L("Bad Parameter: Index of Attribute out of range."));
        return KErrBadTestParameter;
        }
    if ( aItem.GetNextInt(expectedAttIndex) )
        {
        iLog->Log(_L("Missing Parameter: Expected Attribute Index."));
        return KErrBadTestParameter;
        }
    if ( expectedAttIndex >= iMPXAttArray.Count() )
        {
        iLog->Log(_L("Bad Parameter: Expected Attribute index out of range."));
        return KErrBadTestParameter;
        }

    TMPXAttribute result = (*iMPXMediaArray)[mediaIndex]->Attribute(attIndex);
    TMPXAttribute expectedResult = iMPXAttArray[expectedAttIndex];
    
    // verification
    if ( !(result == expectedResult) )
        {
        iLog->Log(_L("Verification Failed: contentID=%d, expectedContentID=%d, attributeID=%d, expectedAttributeID=%d."), 
                result.ContentId(), expectedResult.ContentId(), result.AttributeId(), expectedResult.AttributeId());
        err = KErrUnexpectedValue;
        }
    return err;
    }

// -----------------------------------------------------------------------------
// CCommonTestClass::MpxMediaAttributesSet()
// Returns: Symbian OS errors.
// -----------------------------------------------------------------------------
TInt CCommonTestClass::MpxMediaAttributesSet(CStifItemParser& aItem)
    {
    FTRACE(FPrint(_L("CCommonTestClass::MpxMediaAttributesSet testing CMPXMedia::AttributesSet() begin")));
    iLog->Log(_L("CCommonTestClass::MpxMediaAttributesSet testing CMPXMedia::AttributesSet() begin"));
    TInt err=KErrNone;
    TUint mediaIndex;
    TInt contentID;
    TUint expectedAttributes;

    // read in parameters
    if ( aItem.GetNextInt(mediaIndex) )
        {
        iLog->Log(_L("Missing Parameter: Media index."));
        return KErrBadTestParameter;
        }
    if ( !iMPXMediaArray )
        {
        iLog->Log(_L("Error: MPXMediaArray not created."));
        return KErrBadTestParameter;
        }
    if ( mediaIndex >= iMPXMediaArray->Count() )
        {
        iLog->Log(_L("Bad Parameter: Media index out of range."));
        return KErrBadTestParameter;
        }
    if ( aItem.GetNextInt(contentID) )
        {
        iLog->Log(_L("Missing Parameter: ContentID."));
        return KErrBadTestParameter;
        }
    if ( aItem.GetNextInt(expectedAttributes) )
        {
        iLog->Log(_L("Missing Parameter: Expected Attributes."));
        return KErrBadTestParameter;
        }

    TUint result = (*iMPXMediaArray)[mediaIndex]->AttributesSet(contentID);
    // verification
    if ( result != expectedAttributes )
        {
        iLog->Log(_L("Verification Failed: attributes=%d, expectedAttributes=%d."), result, expectedAttributes);
        err = KErrUnexpectedValue;
        }
    return err;
    }

// -----------------------------------------------------------------------------
// CCommonTestClass::MpxMediaIndex()
// Returns: Symbian OS errors.
// -----------------------------------------------------------------------------
TInt CCommonTestClass::MpxMediaIndex(CStifItemParser& aItem)
    {
    FTRACE(FPrint(_L("CCommonTestClass::MpxMediaIndex testing CMPXMedia::Index() begin")));
    iLog->Log(_L("CCommonTestClass::MpxMediaIndex testing CMPXMedia::Index() begin"));
    TInt err=KErrNone;
    TUint mediaIndex;
    TUint attIndex;
    TInt expectedIndex;

    // read in parameters
    if ( aItem.GetNextInt(mediaIndex) )
        {
        iLog->Log(_L("Missing Parameter: Media index."));
        return KErrBadTestParameter;
        }
    if ( !iMPXMediaArray )
        {
        iLog->Log(_L("Error: MPXMediaArray not created."));
        return KErrBadTestParameter;
        }
    if ( mediaIndex >= iMPXMediaArray->Count() )
        {
        iLog->Log(_L("Bad Parameter: Media index out of range."));
        return KErrBadTestParameter;
        }
    if ( aItem.GetNextInt(attIndex) )
        {
        iLog->Log(_L("Missing Parameter: Attribute Index."));
        return KErrBadTestParameter;
        }
    if ( attIndex >= iMPXAttArray.Count() )
        {
        iLog->Log(_L("Bad Parameter: Attribute Index out of range."));
        return KErrBadTestParameter;
        }
    if ( aItem.GetNextInt(expectedIndex) )
        {
        iLog->Log(_L("Missing Parameter: Expected Index."));
        return KErrBadTestParameter;
        }
    
    TInt result = (*iMPXMediaArray)[mediaIndex]->Index(iMPXAttArray[attIndex]);
    // verification
    if ( result != expectedIndex )
        {
        iLog->Log(_L("Verification Failed: index=%d, expectedIndex=%d."), result, expectedIndex);
        err = KErrUnexpectedValue;
        }
    return err;
    }

// -----------------------------------------------------------------------------
// CCommonTestClass::MpxMediaTypeIndex()
// Returns: Symbian OS errors.
// -----------------------------------------------------------------------------
TInt CCommonTestClass::MpxMediaTypeIndex(CStifItemParser& aItem)
    {
    FTRACE(FPrint(_L("CCommonTestClass::MpxMediaTypeIndex testing CMPXMedia::Type() begin")));
    iLog->Log(_L("CCommonTestClass::MpxMediaTypeIndex testing CMPXMedia::Type() begin"));
    TInt err=KErrNone;
    TUint mediaIndex;
    TUint index;
    TInt expectedType;

    // read in parameters
    if ( aItem.GetNextInt(mediaIndex) )
        {
        iLog->Log(_L("Missing Parameter: Media index."));
        return KErrBadTestParameter;
        }
    if ( !iMPXMediaArray )
        {
        iLog->Log(_L("Error: MPXMediaArray not created."));
        return KErrBadTestParameter;
        }
    if ( mediaIndex >= iMPXMediaArray->Count() )
        {
        iLog->Log(_L("Bad Parameter: Media index out of range."));
        return KErrBadTestParameter;
        }
    if ( aItem.GetNextInt(index) )
        {
        iLog->Log(_L("Missing Parameter: Index of Attribute."));
        return KErrBadTestParameter;
        }
    if ( index >= (*iMPXMediaArray)[mediaIndex]->Count() )
        {
        iLog->Log(_L("Bad Parameter: Index of Attribute out of range."));
        return KErrBadTestParameter;
        }
    if ( aItem.GetNextInt(expectedType) )
        {
        iLog->Log(_L("Missing Parameter: Expected Type."));
        return KErrBadTestParameter;
        }

    TMPXAttributeType type = (*iMPXMediaArray)[mediaIndex]->Type(index);
    // verification
    if ( type != expectedType )
        {
        iLog->Log(_L("Verification Failed: type=%d, expectedType=%d."), type, expectedType);
        err = KErrUnexpectedValue;
        }
    return err;
    }

// -----------------------------------------------------------------------------
// CCommonTestClass::MpxMediaTypeAttribute()
// Returns: Symbian OS errors.
// -----------------------------------------------------------------------------
TInt CCommonTestClass::MpxMediaTypeAttribute(CStifItemParser& aItem)
    {
    FTRACE(FPrint(_L("CCommonTestClass::MpxMediaTypeAttribute testing CMPXMedia::Type() begin")));
    iLog->Log(_L("CCommonTestClass::MpxMediaTypeAttribute testing CMPXMedia::Type() begin"));
    TInt err=KErrNone;
    TUint mediaIndex;
    TUint attIndex;
    TInt expectedType;

    // read in parameters
    if ( aItem.GetNextInt(mediaIndex) )
        {
        iLog->Log(_L("Missing Parameter: Media index."));
        return KErrBadTestParameter;
        }
    if ( !iMPXMediaArray )
        {
        iLog->Log(_L("Error: MPXMediaArray not created."));
        return KErrBadTestParameter;
        }
    if ( mediaIndex >= iMPXMediaArray->Count() )
        {
        iLog->Log(_L("Bad Parameter: Media index out of range."));
        return KErrBadTestParameter;
        }
    if ( aItem.GetNextInt(attIndex) )
        {
        iLog->Log(_L("Missing Parameter: Attribute Index."));
        return KErrBadTestParameter;
        }
    if ( attIndex >= iMPXAttArray.Count() )
        {
        iLog->Log(_L("Bad Parameter: Attribute Index out of range."));
        return KErrBadTestParameter;
        }
    if ( aItem.GetNextInt(expectedType) )
        {
        iLog->Log(_L("Missing Parameter: Expected Type."));
        return KErrBadTestParameter;
        }

    TMPXAttributeType type = (*iMPXMediaArray)[mediaIndex]->Type(iMPXAttArray[attIndex]);
    // verification
    if ( type != expectedType )
        {
        iLog->Log(_L("Verification Failed: type=%d, expectedType=%d."), type, expectedType);
        err = KErrUnexpectedValue;
        }
    return err;
    }

// -----------------------------------------------------------------------------
// CCommonTestClass::MpxMediaReset()
// Returns: Symbian OS errors.
// -----------------------------------------------------------------------------
TInt CCommonTestClass::MpxMediaReset(CStifItemParser& aItem)
    {
    FTRACE(FPrint(_L("CCommonTestClass::MpxMediaReset testing CMPXMedia::Reset() begin")));
    iLog->Log(_L("CCommonTestClass::MpxMediaReset testing CMPXMedia::Reset() begin"));
    TInt err=KErrNone;
    TUint mediaIndex;

    // read in parameters
    if ( aItem.GetNextInt(mediaIndex) )
        {
        iLog->Log(_L("Missing Parameter: Media index."));
        return KErrBadTestParameter;
        }
    if ( !iMPXMediaArray )
        {
        iLog->Log(_L("Error: MPXMediaArray not created."));
        return KErrBadTestParameter;
        }
    if ( mediaIndex >= iMPXMediaArray->Count() )
        {
        iLog->Log(_L("Bad Parameter: Media index out of range."));
        return KErrBadTestParameter;
        }
    
    (*iMPXMediaArray)[mediaIndex]->Reset();
    return err;
    }

// -----------------------------------------------------------------------------
// CCommonTestClass::MpxMediaDeleteAttribute()
// Returns: Symbian OS errors.
// -----------------------------------------------------------------------------
TInt CCommonTestClass::MpxMediaDeleteAttribute(CStifItemParser& aItem)
    {
    FTRACE(FPrint(_L("CCommonTestClass::MpxMediaDeleteAttribute testing CMPXMedia::Delete() begin")));
    iLog->Log(_L("CCommonTestClass::MpxMediaDeleteAttribute testing CMPXMedia::Delete() begin"));
    TInt err=KErrNone;
    TUint mediaIndex;
    TUint attIndex;

    // read in parameters
    if ( aItem.GetNextInt(mediaIndex) )
        {
        iLog->Log(_L("Missing Parameter: Media index."));
        return KErrBadTestParameter;
        }
    if ( !iMPXMediaArray )
        {
        iLog->Log(_L("Error: MPXMediaArray not created."));
        return KErrBadTestParameter;
        }
    if ( mediaIndex >= iMPXMediaArray->Count() )
        {
        iLog->Log(_L("Bad Parameter: Media index out of range."));
        return KErrBadTestParameter;
        }
    if ( aItem.GetNextInt(attIndex) )
        {
        iLog->Log(_L("Missing Parameter: Attribute Index."));
        return KErrBadTestParameter;
        }
    if ( attIndex >= iMPXAttArray.Count() )
        {
        iLog->Log(_L("Bad Parameter: Attribute Index out of range."));
        return KErrBadTestParameter;
        }

    (*iMPXMediaArray)[mediaIndex]->Delete(iMPXAttArray[attIndex]);
    return err;
    }

// -----------------------------------------------------------------------------
// CCommonTestClass::MpxMediaDeleteIndex()
// Returns: Symbian OS errors.
// -----------------------------------------------------------------------------
TInt CCommonTestClass::MpxMediaDeleteIndex(CStifItemParser& aItem)
    {
    FTRACE(FPrint(_L("CCommonTestClass::MpxMediaDeleteIndex testing CMPXMedia::Delete() begin")));
    iLog->Log(_L("CCommonTestClass::MpxMediaDeleteIndex testing CMPXMedia::Delete() begin"));
    TInt err=KErrNone;
    TUint mediaIndex;
    TUint index;

    // read in parameters
    if ( aItem.GetNextInt(mediaIndex) )
        {
        iLog->Log(_L("Missing Parameter: Media index."));
        return KErrBadTestParameter;
        }
    if ( !iMPXMediaArray )
        {
        iLog->Log(_L("Error: MPXMediaArray not created."));
        return KErrBadTestParameter;
        }
    if ( mediaIndex >= iMPXMediaArray->Count() )
        {
        iLog->Log(_L("Bad Parameter: Media index out of range."));
        return KErrBadTestParameter;
        }
    if ( aItem.GetNextInt(index) )
        {
        iLog->Log(_L("Missing Parameter: Index of Attribute."));
        return KErrBadTestParameter;
        }
    if ( index >= (*iMPXMediaArray)[mediaIndex]->Count() )
        {
        iLog->Log(_L("Bad Parameter: Index of Attribute out of range."));
        return KErrBadTestParameter;
        }

    (*iMPXMediaArray)[mediaIndex]->Delete(index);
    return err;
    }

// -----------------------------------------------------------------------------
// CCommonTestClass::MpxMediaValueText()
// Returns: Symbian OS errors.
// -----------------------------------------------------------------------------
TInt CCommonTestClass::MpxMediaValueText(CStifItemParser& aItem)
    {
    FTRACE(FPrint(_L("CCommonTestClass::MpxMediaValueText testing CMPXMedia::ValueText() begin")));
    iLog->Log(_L("CCommonTestClass::MpxMediaValueText testing CMPXMedia::ValueText() begin"));
    TInt err=KErrNone;
    TUint mediaIndex;
    TUint attIndex;
    TPtrC expectedValue;

    // read in parameters
    if ( aItem.GetNextInt(mediaIndex) )
        {
        iLog->Log(_L("Missing Parameter: Media index."));
        return KErrBadTestParameter;
        }
    if ( !iMPXMediaArray )
        {
        iLog->Log(_L("Error: MPXMediaArray not created."));
        return KErrBadTestParameter;
        }
    if ( mediaIndex >= iMPXMediaArray->Count() )
        {
        iLog->Log(_L("Bad Parameter: Media index out of range."));
        return KErrBadTestParameter;
        }
    if ( aItem.GetNextInt(attIndex) )
        {
        iLog->Log(_L("Missing Parameter: Attribute Index."));
        return KErrBadTestParameter;
        }
    if ( attIndex >= iMPXAttArray.Count() )
        {
        iLog->Log(_L("Bad Parameter: Attribute Index out of range."));
        return KErrBadTestParameter;
        }
    if ( aItem.GetNextString(expectedValue) )
        {
        iLog->Log(_L("Missing Parameter: Attribute value."));
        return KErrBadTestParameter;
        }

    TPtrC value((*iMPXMediaArray)[mediaIndex]->ValueText(iMPXAttArray[attIndex]));
    // verification
    if ( value != expectedValue )
        {
        iLog->Log(_L("Verification Failed: value=%S, expectedValue=%S."), &value, &expectedValue);
        err = KErrUnexpectedValue;
        }
    return err;
    }

// -----------------------------------------------------------------------------
// CCommonTestClass::MpxMediaValue()
// Returns: Symbian OS errors.
// -----------------------------------------------------------------------------
TInt CCommonTestClass::MpxMediaValue(CStifItemParser& aItem)
    {
    FTRACE(FPrint(_L("CCommonTestClass::MpxMediaValue testing CMPXMedia::Value() begin")));
    iLog->Log(_L("CCommonTestClass::MpxMediaValue testing CMPXMedia::Value() begin"));
    TInt err=KErrNone;
    TUint mediaIndex;
    TUint attIndex;
    TPtrC className;

    // read in parameters
    if ( aItem.GetNextInt(mediaIndex) )
        {
        iLog->Log(_L("Missing Parameter: Media index."));
        return KErrBadTestParameter;
        }
    if ( !iMPXMediaArray )
        {
        iLog->Log(_L("Error: MPXMediaArray not created."));
        return KErrBadTestParameter;
        }
    if ( mediaIndex >= iMPXMediaArray->Count() )
        {
        iLog->Log(_L("Bad Parameter: Media index out of range."));
        return KErrBadTestParameter;
        }
    if ( aItem.GetNextInt(attIndex) )
        {
        iLog->Log(_L("Missing Parameter: Attribute Index."));
        return KErrBadTestParameter;
        }
    if ( attIndex >= iMPXAttArray.Count() )
        {
        iLog->Log(_L("Bad Parameter: Attribute Index out of range."));
        return KErrBadTestParameter;
        }
    if ( aItem.GetNextString(className) )
        {
        iLog->Log(_L("Missing Parameter: Class name."));
        return KErrBadTestParameter;
        }

    // get attribute value
    if ( className == _L("CMPXMedia") )
        {
        CMPXMedia* media = (*iMPXMediaArray)[mediaIndex]->Value<CMPXMedia>(iMPXAttArray[attIndex]);
        // verification
        if ( !media )
            {
            iLog->Log(_L("Verification Failed: Function returned NULL."));
            err = KErrUnexpectedValue;
            }
        }
    else if ( className == _L("CMPXMediaArray") )
        {
        CMPXMediaArray* media = (*iMPXMediaArray)[mediaIndex]->Value<CMPXMediaArray>(iMPXAttArray[attIndex]);
        // verification
        if ( !media )
            {
            iLog->Log(_L("Verification Failed: Function returned NULL."));
            err = KErrUnexpectedValue;
            }
        }
    else if ( className == _L("CMPXCollectionPath") )
        {
        CMPXCollectionPath* media = (*iMPXMediaArray)[mediaIndex]->Value<CMPXCollectionPath>(iMPXAttArray[attIndex]);
        // verification
        if ( !media )
            {
            iLog->Log(_L("Verification Failed: Function returned NULL."));
            err = KErrUnexpectedValue;
            }
        }
    else
        {
        iLog->Log(_L("Bad Parameter: Invalid class name."));
        return KErrBadTestParameter;
        }
    return err;
    }

// -----------------------------------------------------------------------------
// CCommonTestClass::MpxMediaValueTObjectL()
// Returns: Symbian OS errors.
// -----------------------------------------------------------------------------
TInt CCommonTestClass::MpxMediaValueTObjectL(CStifItemParser& aItem)
    {
    FTRACE(FPrint(_L("CCommonTestClass::MpxMediaValueTObjectL testing CMPXMedia::ValueTObjectL() begin")));
    iLog->Log(_L("CCommonTestClass::MpxMediaValueTObjectL testing CMPXMedia::ValueTObjectL() begin"));
    TInt err=KErrNone;
    TUint mediaIndex;
    TUint attIndex;
    TInt expectedValue;

    // read in parameters
    if ( aItem.GetNextInt(mediaIndex) )
        {
        iLog->Log(_L("Missing Parameter: Media index."));
        return KErrBadTestParameter;
        }
    if ( !iMPXMediaArray )
        {
        iLog->Log(_L("Error: MPXMediaArray not created."));
        return KErrBadTestParameter;
        }
    if ( mediaIndex >= iMPXMediaArray->Count() )
        {
        iLog->Log(_L("Bad Parameter: Media index out of range."));
        return KErrBadTestParameter;
        }
    if ( aItem.GetNextInt(attIndex) )
        {
        iLog->Log(_L("Missing Parameter: Attribute Index."));
        return KErrBadTestParameter;
        }
    if ( attIndex >= iMPXAttArray.Count() )
        {
        iLog->Log(_L("Bad Parameter: Attribute Index out of range."));
        return KErrBadTestParameter;
        }
    if ( aItem.GetNextInt(expectedValue) )
        {
        iLog->Log(_L("Missing Parameter: Expected value."));
        return KErrBadTestParameter;
        }

    // get attribute value
    TInt value = (*iMPXMediaArray)[mediaIndex]->ValueTObjectL<TInt>(iMPXAttArray[attIndex]);
    // verification
    if ( value != expectedValue )
        {
        iLog->Log(_L("Verification Failed: value=%d, expectedValue=%d."), value, expectedValue);
        err = KErrUnexpectedValue;
        }
    return err;
    }

// -----------------------------------------------------------------------------
// CCommonTestClass::MpxMediaValueCObjectL()
// Returns: Symbian OS errors.
// -----------------------------------------------------------------------------
TInt CCommonTestClass::MpxMediaValueCObjectL(CStifItemParser& aItem)
    {
    FTRACE(FPrint(_L("CCommonTestClass::MpxMediaValueCObjectL testing CMPXMedia::ValueCObjectL() begin")));
    iLog->Log(_L("CCommonTestClass::MpxMediaValueCObjectL testing CMPXMedia::ValueCObjectL() begin"));
    TInt err=KErrNone;
    TUint mediaIndex;
    TUint attIndex;
    TPtrC className;

    // read in parameters
    if ( aItem.GetNextInt(mediaIndex) )
        {
        iLog->Log(_L("Missing Parameter: Media index."));
        return KErrBadTestParameter;
        }
    if ( !iMPXMediaArray )
        {
        iLog->Log(_L("Error: MPXMediaArray not created."));
        return KErrBadTestParameter;
        }
    if ( mediaIndex >= iMPXMediaArray->Count() )
        {
        iLog->Log(_L("Bad Parameter: Media index out of range."));
        return KErrBadTestParameter;
        }
    if ( aItem.GetNextInt(attIndex) )
        {
        iLog->Log(_L("Missing Parameter: Attribute Index."));
        return KErrBadTestParameter;
        }
    if ( attIndex >= iMPXAttArray.Count() )
        {
        iLog->Log(_L("Bad Parameter: Attribute Index out of range."));
        return KErrBadTestParameter;
        }
    if ( aItem.GetNextString(className) )
        {
        iLog->Log(_L("Missing Parameter: Class name."));
        return KErrBadTestParameter;
        }

    // Verify Attribute
    if ( !(*iMPXMediaArray)[mediaIndex]->IsSupported(iMPXAttArray[attIndex]) )
        {
        iLog->Log(_L("Bad Parameter: Invalid Attribute."));
        return KErrBadTestParameter;
        }
    // get attribute value
    if ( className == _L("CMPXMedia") )
        {
        CMPXMedia* media = (*iMPXMediaArray)[mediaIndex]->ValueCObjectL<CMPXMedia>(iMPXAttArray[attIndex]);
        delete media;
        }
    else if ( className == _L("CMPXMediaArray") )
        {
        CMPXMediaArray* media = (*iMPXMediaArray)[mediaIndex]->ValueCObjectL<CMPXMediaArray>(iMPXAttArray[attIndex]);
        delete media;
        }
    else if ( className == _L("CMPXCollectionPath") )
        {
        CMPXCollectionPath* path = (*iMPXMediaArray)[mediaIndex]->ValueCObjectL<CMPXCollectionPath>(iMPXAttArray[attIndex]);
        delete path;
        }
    else
        {
        iLog->Log(_L("Bad Parameter: Invalid class name."));
        return KErrBadTestParameter;
        }
    return err;
    }

// -----------------------------------------------------------------------------
// CCommonTestClass::MpxMediaValueNoNewLCObjectL()
// Returns: Symbian OS errors.
// -----------------------------------------------------------------------------
TInt CCommonTestClass::MpxMediaValueNoNewLCObjectL(CStifItemParser& /*aItem*/)
    {
    FTRACE(FPrint(_L("CCommonTestClass::MpxMediaValueNoNewLCObjectL testing CMPXMedia::ValueNoNewLCObjectL() begin")));
    iLog->Log(_L("CCommonTestClass::MpxMediaValueNoNewLCObjectL testing CMPXMedia::ValueNoNewLCObjectL() begin"));
    TInt err=KErrNone;
    
    return err;
    }

// -----------------------------------------------------------------------------
// CCommonTestClass::MpxMediaStreamingL()
// Returns: Symbian OS errors.
// -----------------------------------------------------------------------------
TInt CCommonTestClass::MpxMediaStreamingL(CStifItemParser& aItem)
    {
    FTRACE(FPrint(_L("CCommonTestClass::MpxMediaStreamingL testing CMPXMedia ExternalizeL and InternalizeL begin")));
    iLog->Log(_L("CCommonTestClass::MpxMediaStreamingL testing CMPXMedia ExternalizeL and InternalizeL begin"));
    TInt err=KErrNone;
    TUint mediaIndex;
    
    // read in parameters
    if ( aItem.GetNextInt(mediaIndex) )
        {
        iLog->Log(_L("Missing Parameter: Media index."));
        return KErrBadTestParameter;
        }
    if ( !iMPXMediaArray )
        {
        iLog->Log(_L("Error: MPXMediaArray not created."));
        return KErrBadTestParameter;
        }
    if ( mediaIndex >= iMPXMediaArray->Count() )
        {
        iLog->Log(_L("Bad Parameter: Media index out of range."));
        return KErrBadTestParameter;
        }
    
    iLog->Log(_L("Start ExternalizeL"));
    CBufBase* buffer = CBufFlat::NewL( 200 );
    CleanupStack::PushL( buffer );
    RBufWriteStream writeStream( *buffer );
    CleanupClosePushL( writeStream );
    (*iMPXMediaArray)[mediaIndex]->ExternalizeL( writeStream );
    writeStream.CommitL();
    buffer->Compress();
    CleanupStack::PopAndDestroy( &writeStream );

    iLog->Log(_L("Start InternalizeL"));
    RBufReadStream readStream( *buffer );
    CleanupClosePushL( readStream );                
    CMPXMedia* media = CMPXMedia::NewL();
    CleanupStack::PushL( media );
    media->InternalizeL( readStream );
    // verification
    for (TInt i=0;i<(*iMPXMediaArray)[mediaIndex]->Count();i++)
        {
        if ( media->IsSupported((*iMPXMediaArray)[mediaIndex]->Attribute(i)) == EFalse )
            {
            iLog->Log(_L("Verification Failed: Attribute at index %d is not supported."), i);
            err = KErrUnexpectedValue;
            }
        }
    CleanupStack::PopAndDestroy( media );
    CleanupStack::PopAndDestroy( &readStream );
    CleanupStack::PopAndDestroy( buffer );
    return err;
    }

// -----------------------------------------------------------------------------
// CCommonTestClass::MpxMediaArrayNewL()
// Returns: Symbian OS errors.
// -----------------------------------------------------------------------------
TInt CCommonTestClass::MpxMediaArrayNewL(CStifItemParser& /*aItem*/)
    {
    FTRACE(FPrint(_L("CCommonTestClass::MpxMediaArrayNewL testing CMPXMediaArray::NewL() begin")));
    iLog->Log(_L("CCommonTestClass::MpxMediaArrayNewL testing CMPXMediaArray::NewL() begin"));
    TInt err=KErrNone;

    if ( iMPXMediaArray )
        {
        delete iMPXMediaArray;
        iMPXMediaArray = NULL;
        }
    iMPXMediaArray = CMPXMediaArray::NewL();
    return err;
    }

// -----------------------------------------------------------------------------
// CCommonTestClass::MpxMediaArrayNewLMediaArrayL()
// Returns: Symbian OS errors.
// -----------------------------------------------------------------------------
TInt CCommonTestClass::MpxMediaArrayNewLMediaArrayL(CStifItemParser& /*aItem*/)
    {
    FTRACE(FPrint(_L("CCommonTestClass::MpxMediaArrayNewLMediaArrayL testing CMPXMediaArray::NewL() begin")));
    iLog->Log(_L("CCommonTestClass::MpxMediaArrayNewLMediaArrayL testing CMPXMediaArray::NewL() begin"));
    TInt err=KErrNone;

    if ( !iMPXMediaArray )
        {
        iLog->Log(_L("Error: MPXMediaArray not created."));
        return KErrBadTestParameter;
        }
    CMPXMediaArray* mediaArray = CMPXMediaArray::NewL(*iMPXMediaArray);
    // verification
    if ( mediaArray->Count() != iMPXMediaArray->Count() )
        {
        iLog->Log(_L("Verification Failed: New array count=%d, old array count=%d."), 
                mediaArray->Count(), iMPXMediaArray->Count());
        err = KErrUnexpectedValue;
        }
    delete mediaArray;
    return err;
    }

// -----------------------------------------------------------------------------
// CCommonTestClass::MpxMediaArrayCount()
// Returns: Symbian OS errors.
// -----------------------------------------------------------------------------
TInt CCommonTestClass::MpxMediaArrayCount(CStifItemParser& aItem)
    {
    FTRACE(FPrint(_L("CCommonTestClass::MpxMediaArrayCount testing CMPXMediaArray::Count() begin")));
    iLog->Log(_L("CCommonTestClass::MpxMediaArrayCount testing CMPXMediaArray::Count() begin"));
    TInt err=KErrNone;
    TUint expectedCount;
    
    // read in parameters
    if ( aItem.GetNextInt(expectedCount) )
        {
        iLog->Log(_L("Missing Parameter: Expected count."));
        return KErrBadTestParameter;
        }

    TInt count = iMPXMediaArray->Count();
    // verification
    if ( count != expectedCount )
        {
        iLog->Log(_L("Verification Failed: count=%d, expectedCount=%d."), count, expectedCount);
        err = KErrUnexpectedValue;
        }
    return err;
    }

// -----------------------------------------------------------------------------
// CCommonTestClass::MpxMediaArrayAtLByIndexL()
// Returns: Symbian OS errors.
// -----------------------------------------------------------------------------
TInt CCommonTestClass::MpxMediaArrayAtLByIndexL(CStifItemParser& aItem)
    {
    FTRACE(FPrint(_L("CCommonTestClass::MpxMediaArrayAtLByIndexL testing CMPXMediaArray::Count() begin")));
    iLog->Log(_L("CCommonTestClass::MpxMediaArrayAtLByIndexL testing CMPXMediaArray::Count() begin"));
    TInt err = KErrNone;
    
    TUint expectedIndex;
    
    // read in parameters
    if ( aItem.GetNextInt(expectedIndex) )
        {
        iLog->Log(_L("Missing Parameter: Expected Index."));
        return KErrBadTestParameter;
        }
    TInt count = iMPXMediaArray->Count();
    if( expectedIndex >= count )
    	{
        iLog->Log(_L("Incorrect Parameter: Expected Index greater than the count of media array."));
        return KErrBadTestParameter;
    	}
    CMPXMedia* media = iMPXMediaArray->AtL( expectedIndex );
    
    if( media )
    	{
    	iLog->Log(_L("MpxMediaArray::AtL(TInt aIndex) tested seccessfully."));
    	return err;
    	}
    else
    	{
        iLog->Log(_L("No media pointer returns."));
        return KErrBadTestParameter;
    	} 
    }


// -----------------------------------------------------------------------------
// CCommonTestClass::MpxMediaArrayAppendLByPtrL()
// Returns: Symbian OS errors.
// -----------------------------------------------------------------------------
TInt CCommonTestClass::MpxMediaArrayAppendLByPtrL(CStifItemParser& /*aItem*/)
    {
    FTRACE(FPrint(_L("CCommonTestClass::MpxMediaArrayAppendLByPtrL testing CMPXMediaArray::AppendL() begin")));
    iLog->Log(_L("CCommonTestClass::MpxMediaArrayAppendLByPtrL testing CMPXMediaArray::AppendL() begin"));
    TInt err=KErrNone;
    
    CMPXMedia* media = CMPXMedia::NewL();
    CleanupStack::PushL(media);
    iMPXMediaArray->AppendL(media);
    CleanupStack::Pop(media);
    return err;
    }

// -----------------------------------------------------------------------------
// CCommonTestClass::MpxMediaArrayAppendLByRefL()
// Returns: Symbian OS errors.
// -----------------------------------------------------------------------------
TInt CCommonTestClass::MpxMediaArrayAppendLByRefL(CStifItemParser& /*aItem*/)
    {
    FTRACE(FPrint(_L("CCommonTestClass::MpxMediaArrayAppendLByRefL testing CMPXMediaArray::AppendL() begin")));
    iLog->Log(_L("CCommonTestClass::MpxMediaArrayAppendLByRefL testing CMPXMediaArray::AppendL() begin"));
    TInt err=KErrNone;
    
    CMPXMedia* media = CMPXMedia::NewL();
    CleanupStack::PushL(media);
    iMPXMediaArray->AppendL(*media);
    CleanupStack::PopAndDestroy(media);
    return err;
    }

// -----------------------------------------------------------------------------
// CCommonTestClass::MpxMediaArrayReset()
// Returns: Symbian OS errors.
// -----------------------------------------------------------------------------
TInt CCommonTestClass::MpxMediaArrayReset(CStifItemParser& /*aItem*/)
    {
    FTRACE(FPrint(_L("CCommonTestClass::MpxMediaArrayReset testing CMPXMediaArray::Reset() begin")));
    iLog->Log(_L("CCommonTestClass::MpxMediaArrayReset testing CMPXMediaArray::Reset() begin"));
    TInt err=KErrNone;
    
    iMPXMediaArray->Reset();
    return err;
    }

// -----------------------------------------------------------------------------
// CCommonTestClass::MpxMediaArrayRemove()
// Returns: Symbian OS errors.
// -----------------------------------------------------------------------------
TInt CCommonTestClass::MpxMediaArrayRemove(CStifItemParser& aItem)
    {
    FTRACE(FPrint(_L("CCommonTestClass::MpxMediaArrayRemove testing CMPXMediaArray::Remove() begin")));
    iLog->Log(_L("CCommonTestClass::MpxMediaArrayRemove testing CMPXMediaArray::Remove() begin"));
    TInt err=KErrNone;
    TUint index;
    
    // read in parameters
    if ( aItem.GetNextInt(index) )
        {
        iLog->Log(_L("Missing Parameter: Index."));
        return KErrBadTestParameter;
        }
    // verify index
    if ( index >= iMPXMediaArray->Count() )
        {
        iLog->Log(_L("Bad Parameter: Index out of range."));
        return KErrBadTestParameter;
        }

    iMPXMediaArray->Remove(index);
    return err;
    }

// -----------------------------------------------------------------------------
// CCommonTestClass::MpxMediaArrayInsertByPtr()
// Returns: Symbian OS errors.
// -----------------------------------------------------------------------------
TInt CCommonTestClass::MpxMediaArrayInsertByPtr(CStifItemParser& aItem)
    {
    FTRACE(FPrint(_L("CCommonTestClass::MpxMediaArrayInsertByPtr testing CMPXMediaArray::Insert() begin")));
    iLog->Log(_L("CCommonTestClass::MpxMediaArrayInsertByPtr testing CMPXMediaArray::Insert() begin"));
    TInt err=KErrNone;
    TUint index;
    
    // read in parameters
    if ( aItem.GetNextInt(index) )
        {
        iLog->Log(_L("Missing Parameter: Index."));
        return KErrBadTestParameter;
        }
    // verify index
    if ( index > iMPXMediaArray->Count() )
        {
        iLog->Log(_L("Bad Parameter: Index out of range."));
        return KErrBadTestParameter;
        }

    CMPXMedia* media( NULL );
    TRAP(err,media = CMPXMedia::NewL());
    err = iMPXMediaArray->Insert(media, index);
    if ( err )
        {
        iLog->Log(_L("Error: Insert return error code %d."), err);
        delete media;
        }
    return err;
    }

// -----------------------------------------------------------------------------
// CCommonTestClass::MpxMediaArrayInsertByRef()
// Returns: Symbian OS errors.
// -----------------------------------------------------------------------------
TInt CCommonTestClass::MpxMediaArrayInsertByRef(CStifItemParser& aItem)
    {
    FTRACE(FPrint(_L("CCommonTestClass::MpxMediaArrayInsertByRef testing CMPXMediaArray::Insert() begin")));
    iLog->Log(_L("CCommonTestClass::MpxMediaArrayInsertByRef testing CMPXMediaArray::Insert() begin"));
    TInt err=KErrNone;
    TUint index;
    
    // read in parameters
    if ( aItem.GetNextInt(index) )
        {
        iLog->Log(_L("Missing Parameter: Index."));
        return KErrBadTestParameter;
        }
    // verify index
    if ( index > iMPXMediaArray->Count() )
        {
        iLog->Log(_L("Bad Parameter: Index out of range."));
        return KErrBadTestParameter;
        }

    CMPXMedia* media( NULL );
    TRAP(err , media= CMPXMedia::NewL());
    err = iMPXMediaArray->Insert(*media, index);
    if ( err )
        {
        iLog->Log(_L("Error: Insert return error code %d."), err);
        }
    delete media;
    return err;
    }

// -----------------------------------------------------------------------------
// CCommonTestClass::MpxMediaArrayInsertLByPtrL()
// Returns: Symbian OS errors.
// -----------------------------------------------------------------------------
TInt CCommonTestClass::MpxMediaArrayInsertLByPtrL(CStifItemParser& aItem)
    {
    FTRACE(FPrint(_L("CCommonTestClass::MpxMediaArrayInsertLByPtrL testing CMPXMediaArray::InsertL() begin")));
    iLog->Log(_L("CCommonTestClass::MpxMediaArrayInsertLByPtrL testing CMPXMediaArray::InsertL() begin"));
    TInt err=KErrNone;
    TUint index;
    
    // read in parameters
    if ( aItem.GetNextInt(index) )
        {
        iLog->Log(_L("Missing Parameter: Index."));
        return KErrBadTestParameter;
        }
    // verify index
    if ( index > iMPXMediaArray->Count() )
        {
        iLog->Log(_L("Bad Parameter: Index out of range."));
        return KErrBadTestParameter;
        }

    CMPXMedia* media( NULL );
    TRAP(err , media= CMPXMedia::NewL());
    CleanupStack::PushL(media);
    TRAP(err , iMPXMediaArray->InsertL(media, index));
    CleanupStack::Pop(media);
    return err;
    }

// -----------------------------------------------------------------------------
// CCommonTestClass::MpxMediaArrayInsertLByRefL()
// Returns: Symbian OS errors.
// -----------------------------------------------------------------------------
TInt CCommonTestClass::MpxMediaArrayInsertLByRefL(CStifItemParser& aItem)
    {
    FTRACE(FPrint(_L("CCommonTestClass::MpxMediaArrayInsertLByRefL testing CMPXMediaArray::InsertL() begin")));
    iLog->Log(_L("CCommonTestClass::MpxMediaArrayInsertLByRefL testing CMPXMediaArray::InsertL() begin"));
    TInt err=KErrNone;
    TUint index;
    
    // read in parameters
    if ( aItem.GetNextInt(index) )
        {
        iLog->Log(_L("Missing Parameter: Index."));
        return KErrBadTestParameter;
        }
    // verify index
    if ( index > iMPXMediaArray->Count() )
        {
        iLog->Log(_L("Bad Parameter: Index out of range."));
        return KErrBadTestParameter;
        }

    CMPXMedia* media( NULL );
    TRAP(err , media= CMPXMedia::NewL());
    CleanupStack::PushL(media);
    TRAP(err , iMPXMediaArray->InsertL(*media, index));
    CleanupStack::PopAndDestroy(media);
    return err;
    }

// -----------------------------------------------------------------------------
// CCommonTestClass::MpxMediaArraySet()
// Returns: Symbian OS errors.
// -----------------------------------------------------------------------------
TInt CCommonTestClass::MpxMediaArraySet(CStifItemParser& aItem)
    {
    FTRACE(FPrint(_L("CCommonTestClass::MpxMediaArraySet testing CMPXMediaArray::Set() begin")));
    iLog->Log(_L("CCommonTestClass::MpxMediaArraySet testing CMPXMediaArray::Set() begin"));
    TInt err=KErrNone;
    TUint index;
    
    // read in parameters
    if ( aItem.GetNextInt(index) )
        {
        iLog->Log(_L("Missing Parameter: Index."));
        return KErrBadTestParameter;
        }
    // verify index
    if ( index >= iMPXMediaArray->Count() )
        {
        iLog->Log(_L("Bad Parameter: Index out of range."));
        return KErrBadTestParameter;
        }

    CMPXMedia* media( NULL );
    TRAP(err , media= CMPXMedia::NewL());
    CleanupStack::PushL(media);
    iMPXMediaArray->Set(*media, index);
    CleanupStack::PopAndDestroy(media);
    return err;
    }

// -----------------------------------------------------------------------------
// CCommonTestClass::MpxMediaArrayStreamingL()
// Returns: Symbian OS errors.
// -----------------------------------------------------------------------------
TInt CCommonTestClass::MpxMediaArrayStreamingL(CStifItemParser& /*aItem*/)
    {
    FTRACE(FPrint(_L("CCommonTestClass::MpxMediaArrayStreamingL testing CMPXMediaArray ExternalizeL and InternalizeL begin")));
    iLog->Log(_L("CCommonTestClass::MpxMediaArrayStreamingL testing CMPXMediaArray ExternalizeL and InternalizeL begin"));
    TInt err=KErrNone;
    
    iLog->Log(_L("Start ExternalizeL"));
    CBufBase* buffer = CBufFlat::NewL( 200 );
    CleanupStack::PushL( buffer );
    RBufWriteStream writeStream( *buffer );
    CleanupClosePushL( writeStream );
    iMPXMediaArray->ExternalizeL( writeStream );
    writeStream.CommitL();
    buffer->Compress();
    CleanupStack::PopAndDestroy( &writeStream );

    iLog->Log(_L("Start InternalizeL"));
    RBufReadStream readStream( *buffer );
    CleanupClosePushL( readStream );                
    CMPXMediaArray* media = CMPXMediaArray::NewL();
    CleanupStack::PushL( media );
    media->InternalizeL( readStream );
    // verification
    if ( media->Count() != iMPXMediaArray->Count() )
        {
        iLog->Log(_L("Verification Failed: New Array Count=%d, Old Array Count=%d."), 
                media->Count(), iMPXMediaArray->Count());
        err = KErrUnexpectedValue;
        }
    CleanupStack::PopAndDestroy( media );
    CleanupStack::PopAndDestroy( &readStream );
    CleanupStack::PopAndDestroy( buffer );
    return err;
    }

// -----------------------------------------------------------------------------
// CCommonTestClass::MpxMediaCopyL()
// Returns: Symbian OS errors.
// add MpxMediaCopyL to testmpxmedia.cpp  9/18/2008 10:47
// -----------------------------------------------------------------------------
TInt CCommonTestClass::MpxMediaCopyL(CStifItemParser& aItem)
    {
    FTRACE(FPrint(_L("CCommonTestClass::MpxMediaCopyL testing CMPXMedia::CopyL() begin")));
    iLog->Log(_L("CCommonTestClass::MpxMediaCopyL testing CMPXMedia::CopyL() begin"));
    TInt err=KErrNone;
    TUint index;
    // read in parameters
        if ( aItem.GetNextInt(index) )
            {
            iLog->Log(_L("Missing Parameter: index."));
            return KErrBadTestParameter;
            }
        if ( !iMPXMediaArray )
            {
            iLog->Log(_L("Error: MPXMediaArray not created."));
            return KErrBadTestParameter;
            }
        if ( index >= iMPXMediaArray->Count() )
            {
            iLog->Log(_L("Bad Parameter: index out of range."));
            return KErrBadTestParameter;
            }

    CMPXMedia* media( NULL );
    TRAP(err , media= CMPXMedia::NewL());
    CleanupStack::PushL(media);
    TRAP(err,iMPXMediaArray->AppendL(media));
    CMPXMedia* media1=media->CopyL(*(*iMPXMediaArray)[index]);
    CleanupStack::PushL(media1);
    TRAP(err,iMPXMediaArray->AppendL(media1));
    CleanupStack::Pop(media1);
    CleanupStack::Pop(media);
    return err;
}
// -----------------------------------------------------------------------------
// CCommonTestClass::MpxMediaDeleteL()
// Returns: Symbian OS errors.
// add MpxMediaDeleteL to testmpxmedia.cpp
// -----------------------------------------------------------------------------
TInt CCommonTestClass::MpxMediaDeleteL(CStifItemParser&)
    {
	FTRACE(FPrint(_L("CCommonTestClass::MpxMediaDeleteL testing CMPXMedia::~ begin")));
    iLog->Log(_L("CCommonTestClass::MpxMediaDeleteL testing CMPXMedia::~ begin"));
    TInt err = KErrNone;
    delete iMPXMediaArray;
    iMPXMediaArray = NULL;
	FTRACE(FPrint(_L("CCommonTestClass::MpxMediaDeleteL testing CMPXMedia::~ end err=%d"), err));
    iLog->Log(_L("CCommonTestClass::MpxMediaDeleteL testing CMPXMedia::~ end err=%d"), err);
	return err;
    }
// -----------------------------------------------------------------------------
// CCommonTestClass::MpxMediaHeapMemoryInfoL()
// Returns: Symbian OS errors.
// add MpxMediaDeleteL to testmpxmedia.cpp
// -----------------------------------------------------------------------------
TInt CCommonTestClass::MpxMediaHeapMemoryInfoL(CStifItemParser&)
    {
	FTRACE(FPrint(_L("CCommonTestClass::MpxMediaHeapMemoryInfoL testing CMPXMedia::HeapMemoryInfoL begin")));
    iLog->Log(_L("CCommonTestClass::MpxMediaHeapMemoryInfoL testing CMPXMedia::HeapMemoryInfoL begin"));
    
    TInt err = KErrNone;
    TInt total=0;
    TInt used=0;
    TInt& aTotal=total; 
    TInt& aUsed=used;

    CMPXMedia* media( NULL );
    TRAP(err , media= CMPXMedia::NewL());
    CleanupStack::PushL(media);
    TRAP(err,iMPXMediaArray->AppendL(media));
    TRAP(err,media->HeapMemoryInfoL(aTotal,aUsed));
    CleanupStack::Pop(media);
	
    FTRACE(FPrint(_L("CCommonTestClass::MpxMediaHeapMemoryInfoL testing CMPXMedia::HeapMemoryInfoLend err=%d"), err));
    iLog->Log(_L("CCommonTestClass::MpxMediaHeapMemoryInfoL testing CMPXMedia::HeapMemoryInfoL end err=%d"), err);
	return err;
    }


// -----------------------------------------------------------------------------
// CCommonTestClass::MpxMediaMergeMediaL()
// Returns: Symbian OS errors.
// add MpxMediaMergeMediaL to testmpxmedia.cpp
// -----------------------------------------------------------------------------
TInt CCommonTestClass::MpxMediaMergeMediaL(CStifItemParser&)
    {
	FTRACE(FPrint(_L("CCommonTestClass::MpxMediaMergeMediaL testing CMPXMedia::MergeMediaL begin")));
    iLog->Log(_L("CCommonTestClass::MpxMediaMergeMediaL testing CMPXMedia::MergeMediaL begin"));
    TInt err = KErrNone;
    TUint index=0;

    CMPXMedia* media( NULL );
    TRAP(err , media= CMPXMedia::NewL());
    CleanupStack::PushL(media);
    TRAP(err,iMPXMediaArray->AppendL(media));
    TRAP(err,media->MergeMediaL(*(*iMPXMediaArray)[index]));
    CleanupStack::Pop(media);
    
	FTRACE(FPrint(_L("CCommonTestClass::MpxMediaMergeMediaL testing CMPXMedia::MergeMediaL err=%d"), err));
    iLog->Log(_L("CCommonTestClass::MpxMediaMergeMediaL testing CMPXMedia::MergeMediaL end err=%d"), err);
	return err;
    }

// -----------------------------------------------------------------------------
// CCommonTestClass::MpxMediaMatchL()
// Returns: Symbian OS errors.
// add MpxMediaMatchL to testmpxmedia.cpp  9/18/2008 10:47
// -----------------------------------------------------------------------------
TInt CCommonTestClass::MpxMediaMatchL(CStifItemParser&)
    {
    FTRACE(FPrint(_L("CCommonTestClass::MpxMediaMatchL testing CMPXMedia::Match begin")));
    iLog->Log(_L("CCommonTestClass::MpxMediaMatchL testing CMPXMedia::Match begin"));
    TInt err = KErrNone;
    TUint index=0;    

    CMPXMedia* media( NULL );
    TRAP(err , media= CMPXMedia::NewL());
    CleanupStack::PushL(media);
    TRAP(err ,iMPXMediaArray->AppendL(media));       
    TInt countAttributes = iMPXAttArray.Count();
    TInt countMedias = iMPXMediaArray->Count();      
    
    if(countAttributes>0 & countMedias>0)
    {
        for(TInt i=0;i<countAttributes;i++)
        {
        media->Match(*(*iMPXMediaArray)[index],iMPXAttArray[i]);
        }
    }
    CleanupStack::Pop(media);
    
    FTRACE(FPrint(_L("CCommonTestClass::MpxMediaMatchL testing CMPXMedia::Match err=%d"), err));
    iLog->Log(_L("CCommonTestClass::MpxMediaMatchL testing CMPXMedia::Match end err=%d"), err);
    return err;
    }


// -----------------------------------------------------------------------------
// CCommonTestClass::MpxMediaSetErrorL()
// Returns: Symbian OS errors.
// add MpxMediaSetErrorL to testmpxmedia.cpp 9/18/2008 10:47
// -----------------------------------------------------------------------------
TInt CCommonTestClass::MpxMediaSetErrorL(CStifItemParser& aItem)
    {
    FTRACE(FPrint(_L("CCommonTestClass::MpxMediaSetErrorL testing CMPXMedia::SetErrorL() begin")));
    iLog->Log(_L("CCommonTestClass::MpxMediaSetErrorL testing CMPXMedia::SetErrorL() begin"));
       TInt err=KErrNone;
       TUint mediaIndex;
       TUint attIndex;
       TInt aError=1;
       // read in parameters
       if ( aItem.GetNextInt(mediaIndex) )
           {
           iLog->Log(_L("Missing Parameter: Media index."));
           return KErrBadTestParameter;
           }
       if ( !iMPXMediaArray )
           {
           iLog->Log(_L("Error: MPXMediaArray not created."));
           return KErrBadTestParameter;
           }
       if ( mediaIndex >= iMPXMediaArray->Count() )
           {
           iLog->Log(_L("Bad Parameter: Media index out of range."));
           return KErrBadTestParameter;
           }
       if ( aItem.GetNextInt(attIndex) )
           {
           iLog->Log(_L("Missing Parameter: Attribute index."));
           return KErrBadTestParameter;
           }
       if ( attIndex >= iMPXAttArray.Count() )
           {
           iLog->Log(_L("Bad Parameter: Attribute index out of range."));
           return KErrBadTestParameter;
           }
       // set Error 
           (*iMPXMediaArray)[mediaIndex]->SetErrorL(iMPXAttArray[attIndex], aError);
         
    return err;
    }


// -----------------------------------------------------------------------------
// CCommonTestClass::MpxMediaErrorL()
// Returns: Symbian OS errors.
// add MpxMediaErrorL to testmpxmedia.cpp  9/18/2008 10:47
// -----------------------------------------------------------------------------
TInt CCommonTestClass::MpxMediaErrorL(CStifItemParser& aItem)
    {
    FTRACE(FPrint(_L("CCommonTestClass::MpxMediaErrorL testing CMPXMedia::Error() begin")));
    iLog->Log(_L("CCommonTestClass::MpxMediaErrorL testing CMPXMedia::Error() begin"));
    TInt err=KErrNone;
    TUint mediaIndex;
    TUint attIndex;
      // read in parameters
	if ( aItem.GetNextInt(mediaIndex) )
        {
        iLog->Log(_L("Missing Parameter: Media index."));
        return KErrBadTestParameter;
        }
	if ( !iMPXMediaArray )
        {
        iLog->Log(_L("Error: MPXMediaArray not created."));
        return KErrBadTestParameter;
        }
    if ( mediaIndex >= iMPXMediaArray->Count() )
        {
        iLog->Log(_L("Bad Parameter: Media index out of range."));
        return KErrBadTestParameter;
        }
	if ( aItem.GetNextInt(attIndex) )
	    {
	    iLog->Log(_L("Missing Parameter: Attribute index."));
	    return KErrBadTestParameter;
	    }
	if ( attIndex >= iMPXAttArray.Count() )
	    {
	    iLog->Log(_L("Bad Parameter: Attribute index out of range."));
	    return KErrBadTestParameter;
	    }
	  // Error 
	(*iMPXMediaArray)[mediaIndex]->Error(iMPXAttArray[attIndex]);
	        
    return err;
    }

// -----------------------------------------------------------------------------
// CCommonTestClass::MpxMediaInternalizeL()
// Returns: Symbian OS errors.

// -----------------------------------------------------------------------------
TInt CCommonTestClass::MpxMediaInternalizeL(CStifItemParser& )
    {
	FTRACE(FPrint(_L("CCommonTestClass::MpxMediaInternalizeL testing CMPXMedia::InternalizeL begin")));
    iLog->Log(_L("CCommonTestClass::MpxMediaInternalizeL testing CMPXMedia::InternalizeL begin"));
    TInt err = KErrNone;

    if ( iMPXMediaArray != NULL )
        {
    	FTRACE(FPrint(_L("CCommonTestClass::MpxMediaInternalizeL started InternalizeL")));
        iLog->Log(_L("CCommonTestClass::MpxMediaInternalizeL started InternalizeL"));
        CBufBase* buffer = CBufFlat::NewL( 50 );
        CleanupStack::PushL( buffer );
        RBufWriteStream writeStream( *buffer );
        CleanupClosePushL( writeStream );
        iMPXMediaArray->ExternalizeL( writeStream );
        writeStream.CommitL();
        buffer->Compress();
        CleanupStack::PopAndDestroy( &writeStream );

        RBufReadStream readStream( *buffer );
        CleanupClosePushL( readStream );   
        
        CMPXMedia* media = CMPXMedia::NewL();
        CleanupStack::PushL(media);
        media->InternalizeL(readStream);
        CleanupStack::PopAndDestroy( media );
        CleanupStack::PopAndDestroy( &readStream );
        CleanupStack::PopAndDestroy( buffer );
        }
    else
        {
        err = KErrBadTestParameter;
    	FTRACE(FPrint(_L("CCommonTestClass::MpxMediaInternalizeL Stif test script is wrong.")));
        iLog->Log(_L("CCommonTestClass::MpxMediaInternalizeL Stif test script is wrong."));
        }


	FTRACE(FPrint(_L("CCommonTestClass::MpxMediaInternalizeL testing CMPXMedia::InternalizeL end err=%d"), err));
    iLog->Log(_L("CCommonTestClass::MpxMediaInternalizeL testing CMPXMedia::InternalizeL end err=%d"), err);
	return err;
    }
    
// -----------------------------------------------------------------------------
// CCommonTestClass::MpxMediaExternalizeL()
// Returns: Symbian OS errors.
// -----------------------------------------------------------------------------
TInt CCommonTestClass::MpxMediaExternalizeL(CStifItemParser& )
    {
	FTRACE(FPrint(_L("CCommonTestClass::MpxMediaExternalizeL testing CMPXMedia::ExternalizeL begin")));
    iLog->Log(_L("CCommonTestClass::MpxMediaExternalizeL testing CMPXMedia::ExternalizeL begin"));
    TInt err = KErrNone;
    if ( iMPXMediaArray != NULL )
        {
    	FTRACE(FPrint(_L("CCommonTestClass::MpxMediaExternalizeL started ExternalizeL")));
        iLog->Log(_L("CCommonTestClass::MpxMediaExternalizeL started ExternalizeL"));
        CBufBase* buffer = CBufFlat::NewL( 50 );
        CleanupStack::PushL( buffer );
        RBufWriteStream writeStream( *buffer );
        CleanupClosePushL( writeStream );
        iMPXMediaArray->ExternalizeL( writeStream );
        writeStream.CommitL();
        buffer->Compress();
        CleanupStack::PopAndDestroy( &writeStream );
        CleanupStack::PopAndDestroy( buffer ); 
        }
    else
        {
        err = KErrBadTestParameter;
    	FTRACE(FPrint(_L("CCommonTestClass::MpxMediaExternalizeL Stif test script is wrong.")));
        iLog->Log(_L("CCommonTestClass::MpxMediaExternalizeL Stif test script is wrong."));
        }
	FTRACE(FPrint(_L("CCommonTestClass::MpxMediaExternalizeL testing CMPXMedia::ExternalizeL end err=%d"), err));
    iLog->Log(_L("CCommonTestClass::MpxMediaExternalizeL testing CMPXMedia::ExternalizeL end err=%d"), err);
	return err;
    }

// end of MPXMedia and MPXMediaArray (mpxmedia.h,mpxmediaarray.h) =============================================
