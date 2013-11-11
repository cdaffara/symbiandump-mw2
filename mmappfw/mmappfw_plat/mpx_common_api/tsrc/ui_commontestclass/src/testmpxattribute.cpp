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
* Description: TMPXAttribute testing implementation
*
*/


#include <s32mem.h>
#include "commontestclass.h"

// Begin TMPXAttribute testing implementation (mpxattribute.h)=======================================

// -----------------------------------------------------------------------------
// CCommonTestClass::MpxAttDefaultConst()
// Returns: Symbian OS errors.
// -----------------------------------------------------------------------------
TInt CCommonTestClass::MpxAttDefaultConst(CStifItemParser& /*aItem*/)
    {
    FTRACE(FPrint(_L("CCommonTestClass::MpxAttDefaultConst testing TMPXAttribute::TMPXAttribute() begin")));
    iLog->Log(_L("CCommonTestClass::MpxAttDefaultConst testing TMPXAttribute::TMPXAttribute() begin"));
    TInt err=KErrNone;
    TMPXAttribute att;
    err = iMPXAttArray.Append(att);
    return err;
    }

// -----------------------------------------------------------------------------
// CCommonTestClass::MpxAttConst()
// Returns: Symbian OS errors.
// -----------------------------------------------------------------------------
TInt CCommonTestClass::MpxAttConst(CStifItemParser& aItem)
    {
    FTRACE(FPrint(_L("CCommonTestClass::MpxAttConst testing TMPXAttribute::TMPXAttribute(TInt aContentId, TUint aAttributeId) begin")));
    iLog->Log(_L("CCommonTestClass::MpxAttConst testing TMPXAttribute::TMPXAttribute(TInt aContentId, TUint aAttributeId) begin"));
    TInt err=KErrNone;
    TInt contentId;
    TUint attId;
    
    // read in parameters
    if ( aItem.GetNextInt(contentId) )
        {
        iLog->Log(_L("Missing Parameter: content ID."));
        return KErrBadTestParameter;
        }
    if ( aItem.GetNextInt(attId) )
        {
        iLog->Log(_L("Missing Parameter: attribute ID."));
        return KErrBadTestParameter;
        }
    
    TMPXAttribute att(contentId, attId);
    err = iMPXAttArray.Append(att);
    return err;
    }
		        
// -----------------------------------------------------------------------------
// CCommonTestClass::MpxAttCopyConstAtt()
// Returns: Symbian OS errors.
// -----------------------------------------------------------------------------
TInt CCommonTestClass::MpxAttCopyConstAtt(CStifItemParser& aItem)
    {
    FTRACE(FPrint(_L("CCommonTestClass::MpxAttCopyConstAtt testing TMPXAttribute::TMPXAttribute(const TMPXAttribute& aId) begin")));
    iLog->Log(_L("CCommonTestClass::MpxAttCopyConstAtt testing TMPXAttribute::TMPXAttribute(const TMPXAttribute& aId) begin"));
    TInt err=KErrNone;
    TInt contentId;
    TUint attId;
    
    // read in parameters
    if ( aItem.GetNextInt(contentId) )
        {
        iLog->Log(_L("Missing Parameter: content ID."));
        return KErrBadTestParameter;
        }
    if ( aItem.GetNextInt(attId) )
        {
        iLog->Log(_L("Missing Parameter: attribute ID."));
        return KErrBadTestParameter;
        }
    
    TMPXAttribute tempAtt(contentId, attId);
    TMPXAttribute att(tempAtt);
    err = iMPXAttArray.Append(att);
    return err;
    }
			        
// -----------------------------------------------------------------------------
// CCommonTestClass::MpxAttCopyConstAttData()
// Returns: Symbian OS errors.
// -----------------------------------------------------------------------------
TInt CCommonTestClass::MpxAttCopyConstAttData(CStifItemParser& aItem)
    {
    FTRACE(FPrint(_L("CCommonTestClass::MpxAttCopyConstAttData testing TMPXAttribute::TMPXAttribute(const TMPXAttributeData& aData) begin")));
    iLog->Log(_L("CCommonTestClass::MpxAttCopyConstAttData testing TMPXAttribute::TMPXAttribute(const TMPXAttributeData& aData) begin"));
    TInt err=KErrNone;
    TInt contentId;
    TUint attId;
    
    // read in parameters
    if ( aItem.GetNextInt(contentId) )
        {
        iLog->Log(_L("Missing Parameter: content ID."));
        return KErrBadTestParameter;
        }
    if ( aItem.GetNextInt(attId) )
        {
        iLog->Log(_L("Missing Parameter: attribute ID."));
        return KErrBadTestParameter;
        }
    
    TMPXAttributeData attData;
    attData.iContentId = contentId;
    attData.iAttributeId = attId;
    TMPXAttribute att(attData);
    err = iMPXAttArray.Append(att);
    return err;
    }

// -----------------------------------------------------------------------------
// CCommonTestClass::MpxAttOperatorAssignment()
// Returns: Symbian OS errors.
// -----------------------------------------------------------------------------
TInt CCommonTestClass::MpxAttOperatorAssignment(CStifItemParser& aItem)
    {
    FTRACE(FPrint(_L("CCommonTestClass::MpxAttOperatorAssignment testing TMPXAttribute::operator=(const TMPXAttribute& aId) begin")));
    iLog->Log(_L("CCommonTestClass::MpxAttOperatorAssignment testing TMPXAttribute::operator=(const TMPXAttribute& aId) begin"));
    TInt err=KErrNone;
    TInt index;

    // read in parameters
    if ( aItem.GetNextInt(index) )
        {
        iLog->Log(_L("Missing Parameter: index."));
        return KErrBadTestParameter;
        }
    if ( index >= iMPXAttArray.Count() )
        {
        iLog->Log(_L("Bad Parameter: Index out of range."));
        return KErrBadTestParameter;
        }

    TMPXAttribute att;
    att.operator =(iMPXAttArray[index]);
    // verify assignment was correct
    if ( !TMPXAttribute::Match(att, iMPXAttArray[index]) )
        {
        iLog->Log(_L("Verification failed."));
        err = KErrUnexpectedValue;
        }
    return err;
    }

// -----------------------------------------------------------------------------
// CCommonTestClass::MpxAttOperatorEqual()
// Returns: Symbian OS errors.
// -----------------------------------------------------------------------------
TInt CCommonTestClass::MpxAttOperatorEqual(CStifItemParser& aItem)
    {
    FTRACE(FPrint(_L("CCommonTestClass::MpxAttOperatorEqual testing TMPXAttribute::operator==(const TMPXAttribute& aId) begin")));
    iLog->Log(_L("CCommonTestClass::MpxAttOperatorEqual testing TMPXAttribute::operator==(const TMPXAttribute& aId) begin"));
    TInt err=KErrNone;
    TUint op1;
    TUint op2;
    TBool expectedResult;

    // read in parameters
    if ( aItem.GetNextInt(op1) )
        {
        iLog->Log(_L("Missing Parameter: operand 1."));
        return KErrBadTestParameter;
        }
    if ( aItem.GetNextInt(op2) )
        {
        iLog->Log(_L("Missing Parameter: operand 2."));
        return KErrBadTestParameter;
        }
    if ( aItem.GetNextInt(expectedResult) )
        {
        iLog->Log(_L("Missing Parameter: expected result."));
        return KErrBadTestParameter;
        }
    if ( (op1>=iMPXAttArray.Count()) ||
         (op2>=iMPXAttArray.Count()) )
        {
        iLog->Log(_L("Bad Parameter: operand out of range."));
        return KErrBadTestParameter;
        }
    
    TBool result = iMPXAttArray[op1].operator ==(iMPXAttArray[op2]);
    if ( result != expectedResult )
        {
        iLog->Log(_L("Verification failed: result=%d, expected result=%d."), result, expectedResult);
        err = KErrUnexpectedValue;
        }
    return err;
    }

// -----------------------------------------------------------------------------
// CCommonTestClass::MpxAttStreamingL()
// Returns: Symbian OS errors.
// -----------------------------------------------------------------------------
TInt CCommonTestClass::MpxAttStreamingL(CStifItemParser& aItem)
    {
    FTRACE(FPrint(_L("CCommonTestClass::MpxAttStreamingL testing TMPXAttribute ExternalizeL and InternalizeL begin")));
    iLog->Log(_L("CCommonTestClass::MpxAttStreamingL testing TMPXAttribute ExternalizeL and InternalizeL begin"));
    TInt err=KErrNone;
    TUint index;
    
    // read in parameters
    if ( aItem.GetNextInt(index) )
        {
        iLog->Log(_L("Missing Parameter: index."));
        return KErrBadTestParameter;
        }
    if ( index >= iMPXAttArray.Count() )
        {
        iLog->Log(_L("Bad Parameter: index out of range."));
        return KErrBadTestParameter;
        }
    
    FTRACE(FPrint(_L("CCommonTestClass::MpxAttStreamingL started ExternalizeL")));
    iLog->Log(_L("CCommonTestClass::MpxAttStreamingL started ExternalizeL"));
    CBufBase* buffer = CBufFlat::NewL( 50 );
    CleanupStack::PushL( buffer );
    RBufWriteStream writeStream( *buffer );
    CleanupClosePushL( writeStream );
    iMPXAttArray[index].ExternalizeL( writeStream );
    writeStream.CommitL();
    buffer->Compress();
    CleanupStack::PopAndDestroy( &writeStream );

    FTRACE(FPrint(_L("CCommonTestClass::MpxAttStreamingL started InternalizeL")));
    iLog->Log(_L("CCommonTestClass::MpxAttStreamingL started InternalizeL"));
    RBufReadStream readStream( *buffer );
    CleanupClosePushL( readStream );                
    TMPXAttribute att;
    att.InternalizeL( readStream );
    CleanupStack::PopAndDestroy( &readStream );
    CleanupStack::PopAndDestroy( buffer );
    
    // verify streaming was correct
    if ( !TMPXAttribute::Match(att, iMPXAttArray[index]) )
        {
        iLog->Log(_L("Streaming Failed Verification."));
        err = KErrUnexpectedValue;
        }
    return err;
    }

// -----------------------------------------------------------------------------
// CCommonTestClass::MpxAttContentId()
// Returns: Symbian OS errors.
// -----------------------------------------------------------------------------
TInt CCommonTestClass::MpxAttContentId(CStifItemParser& aItem)
    {
    FTRACE(FPrint(_L("CCommonTestClass::MpxAttContentId testing TMPXAttribute::ContentId() begin")));
    iLog->Log(_L("CCommonTestClass::MpxAttContentId testing TMPXAttribute::ContentId() begin"));
    TInt err=KErrNone;
    TUint index;
    TInt expectedContentId;
    TInt contentId;
    
    // read in parameters
    if ( aItem.GetNextInt(expectedContentId) )
        {
        iLog->Log(_L("Missing Parameter: content ID."));
        return KErrBadTestParameter;
        }
    if ( aItem.GetNextInt(index) )
        {
        iLog->Log(_L("Missing Parameter: index."));
        return KErrBadTestParameter;
        }
    if ( index >= iMPXAttArray.Count() )
        {
        iLog->Log(_L("Bad Parameter: index out of range."));
        return KErrBadTestParameter;
        }
    
    contentId = iMPXAttArray[index].ContentId();
    // verification
    if ( contentId != expectedContentId )
        {
        iLog->Log(_L("Verification Failed: contentId=%d, expectedContentId=%d."), contentId, expectedContentId);
        err = KErrUnexpectedValue;
        }
    return err;
    }

// -----------------------------------------------------------------------------
// CCommonTestClass::MpxAttAttributeId()
// Returns: Symbian OS errors.
// -----------------------------------------------------------------------------
TInt CCommonTestClass::MpxAttAttributeId(CStifItemParser& aItem)
    {
    FTRACE(FPrint(_L("CCommonTestClass::MpxAttAttributeId testing TMPXAttribute::AttributeId() begin")));
    iLog->Log(_L("CCommonTestClass::MpxAttAttributeId testing TMPXAttribute::AttributeId() begin"));
    TInt err=KErrNone;
    TUint index;
    TInt expectedAttId;
    TInt attId;
    
    // read in parameters
    if ( aItem.GetNextInt(expectedAttId) )
        {
        iLog->Log(_L("Missing Parameter: attribute ID."));
        return KErrBadTestParameter;
        }
    if ( aItem.GetNextInt(index) )
        {
        iLog->Log(_L("Missing Parameter: index."));
        return KErrBadTestParameter;
        }
    if ( index >= iMPXAttArray.Count() )
        {
        iLog->Log(_L("Bad Parameter: index out of range."));
        return KErrBadTestParameter;
        }
    
    attId = iMPXAttArray[index].AttributeId();
    // verification
    if ( attId != expectedAttId )
        {
        iLog->Log(_L("Verification Failed: attId=%d, expectedAttId=%d."), attId, expectedAttId);
        err = KErrUnexpectedValue;
        }
    return err;
    }

// -----------------------------------------------------------------------------
// CCommonTestClass::MpxAttMatch()
// Returns: Symbian OS errors.
// -----------------------------------------------------------------------------
TInt CCommonTestClass::MpxAttMatch(CStifItemParser& aItem)
    {
    FTRACE(FPrint(_L("CCommonTestClass::MpxAttMatch testing TMPXAttribute::Match() begin")));
    iLog->Log(_L("CCommonTestClass::MpxAttMatch testing TMPXAttribute::Match() begin"));
    TInt err=KErrNone;
    TUint index1;
    TUint index2;
    TBool expectedResult;
    TBool result;
    
    // read in parameters
    if ( aItem.GetNextInt(index1) )
        {
        iLog->Log(_L("Missing Parameter: index 1."));
        return KErrBadTestParameter;
        }
    if ( index1 >= iMPXAttArray.Count() )
        {
        iLog->Log(_L("Bad Parameter: index 1 out of range."));
        return KErrBadTestParameter;
        }
    if ( aItem.GetNextInt(index2) )
        {
        iLog->Log(_L("Missing Parameter: index 2."));
        return KErrBadTestParameter;
        }
    if ( index2 >= iMPXAttArray.Count() )
        {
        iLog->Log(_L("Bad Parameter: index 2 out of range."));
        return KErrBadTestParameter;
        }
    if ( aItem.GetNextInt(expectedResult) )
        {
        iLog->Log(_L("Missing Parameter: expected result."));
        return KErrBadTestParameter;
        }

    result = TMPXAttribute::Match(iMPXAttArray[index1], iMPXAttArray[index2]);
    // verification
    if ( result != expectedResult )
        {
        iLog->Log(_L("Verification Failed: result=%d, expectedResult=%d."), result, expectedResult);
        err = KErrUnexpectedValue;
        }
    return err;
    }

// -----------------------------------------------------------------------------
// CCommonTestClass::MpxAttMatchContentId()
// Returns: Symbian OS errors.
// -----------------------------------------------------------------------------
TInt CCommonTestClass::MpxAttMatchContentId(CStifItemParser& aItem)
    {
    FTRACE(FPrint(_L("CCommonTestClass::MpxAttMatchContentId testing TMPXAttribute::MatchContentId() begin")));
    iLog->Log(_L("CCommonTestClass::MpxAttMatchContentId testing TMPXAttribute::MatchContentId() begin"));
    TInt err=KErrNone;
    TUint index1;
    TUint index2;
    TBool expectedResult;
    TBool result;
    
    // read in parameters
    if ( aItem.GetNextInt(index1) )
        {
        iLog->Log(_L("Missing Parameter: index 1."));
        return KErrBadTestParameter;
        }
    if ( index1 >= iMPXAttArray.Count() )
        {
        iLog->Log(_L("Bad Parameter: index 1 out of range."));
        return KErrBadTestParameter;
        }
    if ( aItem.GetNextInt(index2) )
        {
        iLog->Log(_L("Missing Parameter: index 2."));
        return KErrBadTestParameter;
        }
    if ( index2 >= iMPXAttArray.Count() )
        {
        iLog->Log(_L("Bad Parameter: index 2 out of range."));
        return KErrBadTestParameter;
        }
    if ( aItem.GetNextInt(expectedResult) )
        {
        iLog->Log(_L("Missing Parameter: expected result."));
        return KErrBadTestParameter;
        }

    result = TMPXAttribute::MatchContentId(iMPXAttArray[index1], iMPXAttArray[index2]);
    // verification
    if ( result != expectedResult )
        {
        iLog->Log(_L("Verification Failed: result=%d, expectedResult=%d."), result, expectedResult);
        err = KErrUnexpectedValue;
        }
    return err;
    }

// -----------------------------------------------------------------------------
// CCommonTestClass::MpxAttOperatorAndAttData()
// Returns: Symbian OS errors.
// -----------------------------------------------------------------------------
TInt CCommonTestClass::MpxAttOperatorAndAttData(CStifItemParser& aItem)
    {
    FTRACE(FPrint(_L("CCommonTestClass::MpxAttOperatorAndAttData testing TMPXAttribute::operator& (const TMPXAttributeData& aData) begin")));
    iLog->Log(_L("CCommonTestClass::MpxAttOperatorAndAttData testing TMPXAttribute::operator& (const TMPXAttributeData& aData) begin"));
    TInt err=KErrNone;
    TUint index1;
    TUint index2;
    TBool expectedResult;
    TBool result;
    
    // read in parameters
    if ( aItem.GetNextInt(index1) )
        {
        iLog->Log(_L("Missing Parameter: index 1."));
        return KErrBadTestParameter;
        }
    if ( index1 >= iMPXAttArray.Count() )
        {
        iLog->Log(_L("Bad Parameter: index 1 out of range."));
        return KErrBadTestParameter;
        }
    if ( aItem.GetNextInt(index2) )
        {
        iLog->Log(_L("Missing Parameter: index 2."));
        return KErrBadTestParameter;
        }
    if ( index2 >= iMPXAttArray.Count() )
        {
        iLog->Log(_L("Bad Parameter: index 2 out of range."));
        return KErrBadTestParameter;
        }
    if ( aItem.GetNextInt(expectedResult) )
        {
        iLog->Log(_L("Missing Parameter: expected result."));
        return KErrBadTestParameter;
        }
    
    TMPXAttributeData data1 = {iMPXAttArray[index1].ContentId(), iMPXAttArray[index1].AttributeId()};
    TMPXAttributeData data2 = {iMPXAttArray[index2].ContentId(), iMPXAttArray[index2].AttributeId()};
    result = data1 & data2;
    // verification
    if ( result != expectedResult )
        {
        iLog->Log(_L("Verification Failed: result=%d, expectedResult=%d."), result, expectedResult);
        err = KErrUnexpectedValue;
        }
    return err;
    }

// -----------------------------------------------------------------------------
// CCommonTestClass::MpxAttOperatorAndData()
// Returns: Symbian OS errors.
// -----------------------------------------------------------------------------
TInt CCommonTestClass::MpxAttOperatorAndData(CStifItemParser& aItem)
    {
    FTRACE(FPrint(_L("CCommonTestClass::MpxAttOperatorAndData testing TMPXAttribute::operator& (TUint& aData) begin")));
    iLog->Log(_L("CCommonTestClass::MpxAttOperatorAndData testing TMPXAttribute::operator& (TUint& aData) begin"));
    TInt err=KErrNone;
    TUint index1;
    TUint att;
    TBool expectedResult;
    TBool result;
    
    // read in parameters
    if ( aItem.GetNextInt(index1) )
        {
        iLog->Log(_L("Missing Parameter: index 1."));
        return KErrBadTestParameter;
        }
    if ( index1 >= iMPXAttArray.Count() )
        {
        iLog->Log(_L("Bad Parameter: index 1 out of range."));
        return KErrBadTestParameter;
        }
    if ( aItem.GetNextInt(att) )
        {
        iLog->Log(_L("Missing Parameter: attribute."));
        return KErrBadTestParameter;
        }
    if ( aItem.GetNextInt(expectedResult) )
        {
        iLog->Log(_L("Missing Parameter: expected result."));
        return KErrBadTestParameter;
        }
    
    TMPXAttributeData data1 = {iMPXAttArray[index1].ContentId(), iMPXAttArray[index1].AttributeId()};
    result = data1 & att;
    // verification
    if ( result != expectedResult )
        {
        iLog->Log(_L("Verification Failed: result=%d, expectedResult=%d."), result, expectedResult);
        err = KErrUnexpectedValue;
        }
    return err;
    }

// -----------------------------------------------------------------------------
// CCommonTestClass::MpxAttOperatorOr()
// Returns: Symbian OS errors.
// -----------------------------------------------------------------------------
TInt CCommonTestClass::MpxAttOperatorOr(CStifItemParser& aItem)
    {
    FTRACE(FPrint(_L("CCommonTestClass::MpxAttOperatorOr testing TMPXAttribute::operator| () begin")));
    iLog->Log(_L("CCommonTestClass::MpxAttOperatorOr testing TMPXAttribute::operator| () begin"));
    TInt err=KErrNone;
    TUint index1;
    TUint index2;
    TUint expectedAtt;
    TMPXAttributeData result;
    
    // read in parameters
    if ( aItem.GetNextInt(index1) )
        {
        iLog->Log(_L("Missing Parameter: index 1."));
        return KErrBadTestParameter;
        }
    if ( index1 >= iMPXAttArray.Count() )
        {
        iLog->Log(_L("Bad Parameter: index 1 out of range."));
        return KErrBadTestParameter;
        }
    if ( aItem.GetNextInt(index2) )
        {
        iLog->Log(_L("Missing Parameter: index 2."));
        return KErrBadTestParameter;
        }
    if ( index2 >= iMPXAttArray.Count() )
        {
        iLog->Log(_L("Bad Parameter: index 2 out of range."));
        return KErrBadTestParameter;
        }
    if ( aItem.GetNextInt(expectedAtt) )
        {
        iLog->Log(_L("Missing Parameter: expected result."));
        return KErrBadTestParameter;
        }
    
    TMPXAttributeData data1 = {iMPXAttArray[index1].ContentId(), iMPXAttArray[index1].AttributeId()};
    TMPXAttributeData data2 = {iMPXAttArray[index2].ContentId(), iMPXAttArray[index2].AttributeId()};
    result = data1 | data2;
    // verification
    if ( result.iAttributeId != expectedAtt )
        {
        iLog->Log(_L("Verification Failed: att=%d, expectedAtt=%d."), result.iAttributeId, expectedAtt);
        err = KErrUnexpectedValue;
        }
    return err;
    }

// end of TMPXAttribute (mpxattribute.h =============================================
