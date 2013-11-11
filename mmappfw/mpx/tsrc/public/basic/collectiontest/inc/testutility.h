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

#ifndef TESTUTILITY_H
#define TESTUTILITY_H

// INCLUDES
#include <e32cmn.h>
#include <mpxmediageneraldefs.h>
#include <mpxmessagegeneraldefs.h>
#include <mpxmediaarray.h>
#include <mpxmediacontainerdefs.h>
#include <mpxcollectionmessagedefs.h>

// FORWARD DECLARATIONS
class CMPXMedia;
class CMPXMediaArray;
class CMPXCollectionPath;
class CStifLoggger;

// CONSTANTS


// CLASS DECLARATION

/**
* This static class contains utilities functions 
*/
class TestUtility
    {
public:
    /**
    * Test whether aTest descriptor attribute  contains in aContainer
    * @param aContainer: media 
    * @param aTest: media
    * @param aAttr: attribute type
    * @return ETrue aTest's aAttr is containing in aConatiner
    */        
    template<typename T> 
    static TBool ContainMediaObject(const CMPXMedia& aContainer, const CMPXMedia& aTest, 
                                    TMPXAttribute& aAttr, TIdentityRelation<T> anIdentity);
    
    /**
    * Test whether aTest and aContainer are contain media array and match specific content in order
    * @param aContainer: media 
    * @param aTest: media
    * @param aAttr: attribute type
    * @return ETrue aTest's aAttr is containing in aConatiner
    */
    template<typename T> 
    static TBool MatchMediaArrayObject(const CMPXMedia& aContainer, const CMPXMedia& aTest, 
                                       TMPXAttribute& aAttr, TIdentityRelation<T> anIdentity);
    
    /**
    * Test whether aContainer containing all the T object in aArray with aAttr attribues in order
    * @param aContainer: media 
    * @param aArray: array of T objects
    * @param aAttr: attribute type
    * @return ETrue aTest's aAttr is containing in aConatiner
    */
    template<typename T> 
    static TBool MatchMediaArrayObject(const CMPXMedia& aContainer, const RArray<T>& aArray, 
                                       TMPXAttribute& aAttr, TIdentityRelation<T> anIdentity);

    /**
    * Test whether aContainer containing all the text object in aArray with aAttr attribues in order
    * @param aContainer: media 
    * @param aArray: array of text objects
    * @param aAttr: attribute type
    * @return ETrue aTest's aAttr is containing in aConatiner
    */
    static TBool MatchMediaArrayTextObject(const CMPXMedia& aContainer, const MDesC16Array& aArray, 
                                           TMPXAttribute& aAttr);
                                           
    
    /**
    * Log the content of collection path. Only selection of each level and top level Ids are logged
    * @param aPath a path to log
    * @param aLogger a logger to log the message
    */
    //static void LogCollectionPath(CMPXCollectionPath& aPath, CStifLogger* aLogger);
    
private:
    /**
    * Test whether aMedia1 and aMedia2 support same attribute
    * @param aMedia1: media 
    * @param aMedia2: media
    * @param aAttr: attribute type to match
    * @return ETrue aMedia1 and aMedia2 has same aAttr attribute
    */        
    static TBool ValidateMediaObject(const CMPXMedia& aMedia1, const CMPXMedia& aMedia2,
                                     TMPXAttribute& aAttr);
                                     
    /**
    * Test whether aMedia1 and aMedia2 support same array attribute
    * @param aMedia1: media 
    * @param aMedia2: media
    * @return ETrue aMedia1 and aMedia2 has same aAttr attribute
    */        
    static TBool ValidateMediaArrayObject(const CMPXMedia& aMedia1, const CMPXMedia& aMedia2);
    
    /**
    * Test whether aMedia is media array and same count as aArray
    * @param aMedia: media 
    * @param aArray: an array of items
    * @return ETrue aMedia is array object and same count as aArray
    */        
    static TBool ValidateMediaArrayObject(const CMPXMedia& aMedia, TInt aArrayCnt);
    };

#include "testutility.inl"

#endif  // TESTUTILITY_H
