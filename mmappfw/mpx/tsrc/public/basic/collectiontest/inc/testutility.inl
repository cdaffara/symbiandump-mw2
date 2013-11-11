/*
* Copyright (c) 2002 - 2007 Nokia Corporation and/or its subsidiary(-ies).
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
* Description:  Provide common utility function for all test cases
*
*/

// ----------------------------------------------------------------------------------------------------------
// Test whether aTest descriptor attribute  contains in aContainer
// ----------------------------------------------------------------------------------------------------------
//
template<typename T> 
TBool TestUtility::ContainMediaObject(const CMPXMedia& aContainer, const CMPXMedia& aTest, 
                                      TMPXAttribute& aAttr, TIdentityRelation<T> anIdentity)
    {
    TBool match = ValidateMediaObject(aContainer, aTest, aAttr);    // assume true
    if( match )
        {
        const T* contObj = aContainer.Value<T>( aAttr );
        const T* testObj = aTest.Value<T>( aAttr );
        match = (*anIdentity)(contObj, testObj);
        }
    return match;
    }

// ----------------------------------------------------------------------------------------------------------
// Test whether aTest and aContainer are contain media array and match specific content
// ----------------------------------------------------------------------------------------------------------
//
template<typename T> 
TBool TestUtility::MatchMediaArrayObject(const CMPXMedia& aContainer, const CMPXMedia& aTest, 
                                         TMPXAttribute& aAttr, TIdentityRelation<T> anIdentity)
    {
    TBool match = ValidateMediaArrayObject(aContainer, aTest);
    if( match )
        {
        TMPXAttribute arrAttr(KMPXMediaIdContainer,EMPXMediaArrayContents);
        CMPXMediaArray* contArray = aContainer.Value<CMPXMediaArray>( arrAttr );
        CMPXMediaArray* testArray = aTest.Value<CMPXMediaArray>( arrAttr );
        TInt testCnt = testArray->Count();
        
        for(TInt i = 0; i < testCnt; i++)
            {
            const CMPXMedia* contItem = ( *contArray )[i];
            const CMPXMedia* testItem = ( *testArray )[i];
            if( !TestUtility::ContainMediaObject<T>(*contItem, *testItem, aAttr, anIdentity) )
                {
                match = EFalse;
                break;
                }
            }
        }
    return match;
    }
    
    
// ----------------------------------------------------------------------------------------------------------
// Test whether aContainer containing all the T object in aArray with aAttr attribues in order
// ----------------------------------------------------------------------------------------------------------
//
template<typename T> 
TBool TestUtility::MatchMediaArrayObject(const CMPXMedia& aContainer, const RArray<T>& aArray,
                                         TMPXAttribute& aAttr, TIdentityRelation<T> anIdentity)
    {
    TBool match = ValidateMediaArrayObject(aContainer, aArray.Count());
    if( match )
        {
        CMPXMediaArray* contArray = aContainer.Value<CMPXMediaArray>( TMPXAttribute(KMPXMediaIdContainer,EMPXMediaArrayContents) );
        TInt contCnt = contArray->Count();
            
        for(TInt i = 0; i < contCnt; i++)
            {
            const CMPXMedia* contItem = ( *contArray )[i];
            const T* contObj = contItem->Value<T>( aAttr );
            if(contObj == NULL || !(*anIdentity)(contObj, &aArray[i]) )
                {
                match = EFalse;
                break;
                }
            }
        }
    return match;
    }

// END OF FILE

