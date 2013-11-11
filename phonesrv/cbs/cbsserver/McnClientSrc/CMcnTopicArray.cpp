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
* Description:  Implementation of the class CMcnTopicArray.
*
*/



// INCLUDE FILES

#include <cmcntopicarray.h>

// CONSTANTS

const TInt KInitialSpaceForTopicEntries = 3;

// ================= MEMBER FUNCTIONS =======================

// -----------------------------------------------------------------------------
// CMcnTopicArray::CMcnTopicArray
// C++ default constructor can NOT contain any code, that
// might leave.
// -----------------------------------------------------------------------------
//
CMcnTopicArray::CMcnTopicArray()
    {
    }

// -----------------------------------------------------------------------------
// CMcnTopicArray::ConstructL
// Symbian 2nd phase constructor can leave.
// -----------------------------------------------------------------------------
//
void CMcnTopicArray::ConstructL()
    {
    iTopicList = new ( ELeave ) CArrayFixFlat< TUint >
        ( KInitialSpaceForTopicEntries );
    }
    
// -----------------------------------------------------------------------------
// CMcnTopicArray::NewL
// Two-phased constructor.
// -----------------------------------------------------------------------------
//
EXPORT_C CMcnTopicArray* CMcnTopicArray::NewL()
    {
    CMcnTopicArray* self = new ( ELeave ) CMcnTopicArray();
    CleanupStack::PushL( self );
    self->ConstructL();
    CleanupStack::Pop();
    return self;
    }

    
// Destructor
CMcnTopicArray::~CMcnTopicArray()
    {
    delete iTopicList;
    }

// -----------------------------------------------------------------------------
// CMcnTopicArray::AddCbTopicL
// Adds a single CB topic to the topic list.
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
EXPORT_C void CMcnTopicArray::AddCbTopicL( 
    TUint16 aTopicNumber )
    {
    iTopicList->AppendL( aTopicNumber );
    }

// -----------------------------------------------------------------------------
// CMcnTopicArray::EnumerateEntries
// Returns the number of topic entries in parameter aEntries.
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void CMcnTopicArray::EnumerateEntries( 
    TUint& aEntries ) const
    {
    aEntries = iTopicList->Count();
    }

// -----------------------------------------------------------------------------
// CMcnTopicArray::GetTopicNumber
// Returns the topicnumber in aTopicNumber corresponding to the index
// given in aIndex.
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
EXPORT_C void CMcnTopicArray::GetTopicNumber( 
    const TUint& aIndex, 
    TUint& aTopicNumber ) const
    {
    aTopicNumber = iTopicList->At( aIndex );
    }

//  End of File  
