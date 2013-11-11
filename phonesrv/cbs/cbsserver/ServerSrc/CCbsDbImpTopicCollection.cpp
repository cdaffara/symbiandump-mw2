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
* Description:  This module contains the implementation of CCbsDbImpTopicCollection class 
*                member functions.
*    
*                Topic collection stored in the database is represented by this class.
*
*/



// INCLUDE FILES

#include <e32svr.h>
#include "CbsCommon.h"
#include "CbsServerPanic.h"
#include "CCbsDbImpTopicCollection.h"
#include "CCbsDbImp.H"
#include "CbsDbConstants.h"
#include "CbsLogger.h"

// CONSTANTS

// These values specify a range of accepted topic number values (inclusive).
const TInt KMinTopicNumber = 1;     // 000 is not accepted.
const TInt KMaxTopicNumber = 999;
// Granularity for the iIdentities array
const TInt KIdentitiesGranularity = 10;


// ==================== LOCAL FUNCTIONS ====================

// -----------------------------------------------------------------------------
// Print
// Prints out debug info about topics
// Returns: None
// -----------------------------------------------------------------------------
//
#ifdef _DEBUG
void Print( const TDesC& aText, const TInt& aValue )
    {
    TBuf<100> stuff;
    TBuf<10> value;
    value.Num( aValue );
    stuff = aText;
    stuff.Append(value);
    RDebug::Print(stuff);
    }

// -----------------------------------------------------------------------------
// PrintTopicCollectionContentL
// Prints the contentent of the Topic Collection
// Returns: None
// -----------------------------------------------------------------------------
//
void PrintTopicCollectionContentL( const CFileStore* aStore ) 
    {
    RDebug::Print(_L("Current topic collection content: "));
    TStreamId rootStreamId = aStore->Root();
    if ( rootStreamId == 0 ) 
        {
        RDebug::Print(_L("Store contains no root stream."));
        return;
        }
    RStoreReadStream rootStream;
    rootStream.OpenLC( *aStore, rootStreamId ); // on CS
    TInt identityCount( rootStream.ReadUint16L() );

    for ( TInt index( 0 ); index < identityCount; index++ )
        {
        TCbsDbTopicIdentity id;
        id.iName.Zero();
        TInt length( rootStream.ReadInt16L() );
        for ( TInt index2( 0 ); index2 < length; index2++ )
            {
            TInt character( rootStream.ReadInt16L() );
            id.iName.Append( TChar(character) );
            }
        id.iName.SetLength( length );
    
        id.iNumber = rootStream.ReadUint16L();

        TBuf<255> buf = _L("Identity name: ");
        buf.Append( id.iName );
        buf.Append( _L(", number: ") );
        buf.AppendNum( static_cast<TInt>(id.iNumber) );
        RDebug::Print( buf );
        }
   
    CleanupStack::PopAndDestroy();    // root stream
    }
#endif // _DEBUG
    

// ================= MEMBER FUNCTIONS =======================

// -----------------------------------------------------------------------------
// CCbsDbImpTopicCollection::CCbsDbImpTopicCollection
// C++ default constructor can NOT contain any code, that
// might leave.
// -----------------------------------------------------------------------------
//
CCbsDbImpTopicCollection::CCbsDbImpTopicCollection()
    { 
    }

// -----------------------------------------------------------------------------
// CCbsDbImpTopicCollection::ConstructL
// Symbian 2nd phase constructor can leave.
// -----------------------------------------------------------------------------
//
void CCbsDbImpTopicCollection::ConstructL( )
    {
    // Create an array for observers.
    iObservers = new ( ELeave ) CArrayFixFlat<MCbsDbTopicCollectionObserver*>
        ( KCbsDbObserverArraySize );

    iIdentities = new ( ELeave ) CArrayFixFlat<TCbsDbTopicIdentity>
        ( KIdentitiesGranularity );
    }

// -----------------------------------------------------------------------------
// CCbsDbImpTopicCollection::NewL
// Two-phased constructor.
// -----------------------------------------------------------------------------
//
CCbsDbImpTopicCollection* CCbsDbImpTopicCollection::NewL()
    {
    // Normal two phase construction
    CCbsDbImpTopicCollection* self = 
            new ( ELeave ) CCbsDbImpTopicCollection();
    CleanupStack::PushL( self );
    self->ConstructL();
    CleanupStack::Pop();
    return self;
    }

// Destructor
CCbsDbImpTopicCollection::~CCbsDbImpTopicCollection()
    {
    CBSLOGSTRING("CBSSERVER: >>> CCbsDbImpTopicCollection::~CCbsDbImpTopicCollection()");
    // Deallocate the observer array
    delete iObservers;
    delete iIdentities;
    CBSLOGSTRING("CBSSERVER: <<< CCbsDbImpTopicCollection::~CCbsDbImpTopicCollection()");
    }

// -----------------------------------------------------------------------------
// CCbsDbImpTopicCollection::GetTopicIdentityCount
// Returns the number of topic identities stored in the database.
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void CCbsDbImpTopicCollection::GetTopicIdentityCount( 
    TInt& aCount ) const
    {
    aCount = iIdentities->Count();
    }

// -----------------------------------------------------------------------------
// CCbsDbImpTopicCollection::GetTopicIdentityL
// Returns a topic identity matching the given topic index parameter.
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void CCbsDbImpTopicCollection::GetTopicIdentityL( 
    TInt& aIndex, 
    TCbsDbTopicIdentity& aIdentity ) const
    {
    if ( aIndex >= iIdentities->Count() || aIndex < 0 )
        {
        User::Leave( KErrNotFound );
        }
    
    aIdentity = iIdentities->At( aIndex );
    }

// -----------------------------------------------------------------------------
// CCbsDbImpTopicCollection::AddObserverL
// Adds an observer. The observer will be informed of changes 
// in the topic collection.
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void CCbsDbImpTopicCollection::AddObserverL( 
    MCbsDbTopicCollectionObserver* aObserver )
    {
    __ASSERT_DEBUG( aObserver!=0, CbsServerPanic(ECbsObserverNull) );
    iObservers->AppendL( aObserver );
    }

// -----------------------------------------------------------------------------
// CCbsDbImpTopicCollection::RemoveObserverL
// Removes an observer.
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void CCbsDbImpTopicCollection::RemoveObserver( 
    const MCbsDbTopicCollectionObserver* aObserver )
    {
    __ASSERT_DEBUG( aObserver != 0, CbsServerPanic( ECbsObserverNull ) );
    TInt amountOfObservers( iObservers->Count() );
    for ( TInt index( 0 ); index < amountOfObservers; index++ )
        {
        if ( aObserver == iObservers->At( index ) )
            {
            iObservers->Delete( index );
            return;
            }
        }
#ifdef _DEBUG
    CbsServerPanic( ECbsObserverNotFound );
#endif
    }

// -----------------------------------------------------------------------------
// CCbsDbImpTopicCollection::ClearL
// Clears the topic collection.
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void CCbsDbImpTopicCollection::Clear()
    {
    iIdentities->Reset();
    }

// -----------------------------------------------------------------------------
// CCbsDbImpTopicCollection::Apply
// Writes topic identity cache (array iIdentities) into a store.
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void CCbsDbImpTopicCollection::Apply() 
    {
    NotifyObservers();
    }

// -----------------------------------------------------------------------------
// CCbsDbImpTopicCollection::AddTopicIdentityL
// Adds a topic identity to the topic collection. This method 
// is called by the index message handling routine after resolving a single 
// topic number and name out of an index message.
// Topic identities are maintained in ascending topic number order.
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void CCbsDbImpTopicCollection::AddTopicIdentityL( 
    const TCbsDbTopicIdentity& aIdentity )
    {
    if ( aIdentity.iNumber < KMinTopicNumber || 
         aIdentity.iNumber > KMaxTopicNumber )
        {
        User::Leave( KErrArgument );
        }

	// Insert using binary search. KErrAlreadyExists is returned, but
	// all other error codes are passed to Leave.
	TKeyArrayFix key( _FOFF( TCbsDbTopicIdentity, iNumber ), ECmpTUint16 );
	TRAPD( errorCode, iIdentities->InsertIsqL( aIdentity, key ) );
	if ( errorCode != KErrAlreadyExists )
		{
		User::LeaveIfError( errorCode );
		}
    }    

// -----------------------------------------------------------------------------
// CCbsDbImpTopicCollection::NotifyObservers
// Notifies all registred observers about a topic collection contents change.
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void CCbsDbImpTopicCollection::NotifyObservers()
    {
    // Notify each observer.
    TInt count( iObservers->Count() );
    for ( TInt index( 0 ); index < count; index++ )
        {
        iObservers->At( index )->TopicCollectionContentsChangedInd();
        }
    }

// ========================== OTHER EXPORTED FUNCTIONS =========================

//  End of File  
