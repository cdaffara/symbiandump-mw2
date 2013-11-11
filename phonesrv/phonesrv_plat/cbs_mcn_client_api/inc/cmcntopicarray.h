/*
* Copyright (c) 2003 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:  This file contains the header file of the CMcnTopicArray class.
*
*                CMcnTopicArray represents a list containing the numbers of CB topics which
*                a MCN client to CbsServer has subscribed.
*
*/



#ifndef CMCNTOPICARRAY_H
#define CMCNTOPICARRAY_H

//  INCLUDES

#include "e32base.h"

//  CLASS DECLARATION

/**
*      CMcnTopicArray represents a list containing the numbers of CB topics which
*      a MCN client to CbsServer has subscribed.
*/
class CMcnTopicArray 
    : public CBase
    {
public:     // New functions

    /**
    *   Returns a new instance of CMcnTopicArray, initially empty.
    */
    IMPORT_C static CMcnTopicArray* NewL();

    /** 
    *   Destructor. 
    */
    ~CMcnTopicArray();

    /**
    *   Adds a single CB topic to the topic list.
    *   Leaves with KErrArgument if the topic number is not in
    *   valid range. In this implementation, all values
    *   are accepted.
    *
    *   Leave reasons:
    *       KErrArgument, aTopicNumber not in valid range..
    *       KErrNoMemory, Out-of-memory occured.
    *
    *   @param aTopicNumber Number of subscribed CB topic.
    */
    IMPORT_C void AddCbTopicL( TUint16 aTopicNumber );

    /**
    *   Returns the number of topic entries in parameter aEntries.
    *   
    *   @param aEntries  The number of topic entires
    */
    void EnumerateEntries( TUint& aEntries ) const;

    /**
    *   Returns the number of topic in aTopicNumber corresponding to the index
    *   given in aIndex.
    *
    *   @param aIndex   Index of the topic number.
    *   @param aTopicNumber   Topic number corresponding to the given index. 
    */
    IMPORT_C void GetTopicNumber( const TUint& aIndex, TUint& aTopicNumber ) const;

private:
    /// Prohibited copy constructor (not implemented).
    CMcnTopicArray( const CMcnTopicArray& aArray );

    /// Prohibited assignment operator (not implemented).
    CMcnTopicArray& operator=( const CMcnTopicArray& aArray );

    /**
    *   Default constructor.
    */
    CMcnTopicArray();

    /**
    *   2nd-phase constructor.
    */
    void ConstructL();

private:    // data
    /// Own: contains the list of subscribed topics.
    CArrayFixFlat< TUint >* iTopicList;
    };

#endif      // __CMCNTOPICARRAY_H
            
// End of File


