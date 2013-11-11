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
* Description:  This module contains the implementation of CCbsRecDecoder class 
*                member functions.
*    
*                This class represents a collector, which stores pages of multipaged 
*                messages. Complete messages are assembled and returned back to 
*                the caller of CollectL() method.
*  
*                CCbsRecCollector stores message pages in a number of dynamic arrays.
*                If all but one page of a message are present in collector, and
*                the remaining page is received, the pages will be assembled and
*                the corresponding message chain deleted.
* 
*                The maximum number of incomplete messages stored in collector at once
*                is fixed and determined by KMaxCollectorMessages in CCbsRecCollector.cpp.
* 
*                CCbsRecCollector implements a circular list to contain message.
*                Each incomplete message occupies a slot in this list. If the list already
*                contains KMaxCollectorMessages messages, the next received multipaged
*                message will delete all received pages of the oldest message in list.
* 
*                On receival of a message page, the collector compares network information
*                (PLMN, LAC, CellId) of both messages to decide whether pages are of
*                the same message. In short, for pages to be of the same message
*                their network information have to meet the requirements set by the 
*                geographical scope of the already collected page.
*                See ETSI GSM 03.41 for a detailed description. 
*
*/


// INCLUDE FILES
#include "CbsServerPanic.h"
#include "CCbsRecCollector.h"
#include "CCbsRecMessage.h"
#include "CCbsMessageFactory.h"
#include "CCbsMessageCleanUpTimer.h"
#include "CbsLogger.h"

// CONSTANTS

// Maximum number of pages in a single CB message.
const TInt KMaxMessagePages = 15;

// Maximum number of partial messages contained in the collector list.
const TInt KMaxCollectorMessages = 10;

// ================= MEMBER FUNCTIONS =======================

// -----------------------------------------------------------------------------
// CCbsRecCollector::CCbsRecCollector
// C++ default constructor can NOT contain any code, that
// might leave.
// -----------------------------------------------------------------------------
//
CCbsRecCollector::CCbsRecCollector( CCbsMessageFactory& aFactory )
    : iRootNodeIterator( 0 ),
    iFactory( aFactory )
    {
    }

// -----------------------------------------------------------------------------
// CCbsRecCollector::ConstructL
// Symbian 2nd phase constructor can leave.
// -----------------------------------------------------------------------------
//
void CCbsRecCollector::ConstructL()
    {
    CBSLOGSTRING("CBSSERVER: >>> CCbsRecCollector::ConstructL()");

    // initialize the root node array
    iRootNodeArray = new ( ELeave ) CArrayPtrFlat< CMessageBuffer >
        ( KMaxCollectorMessages );
    iRootNodeArray->SetReserveL( KMaxCollectorMessages );
    iMessageCleanupTimerArray = new ( ELeave ) CArrayPtrFlat< CCbsMessageCleanupTimer >
        ( KMaxCollectorMessages );

    for ( TInt i( 0 ); i < KMaxCollectorMessages; i++)
        {
        CMessageBuffer* array = new ( ELeave )
            CMessageBuffer( KMaxMessagePages );
        CleanupStack::PushL( array );
        iRootNodeArray->AppendL( array );

		CCbsMessageCleanupTimer* messageCleanUpTimer = CCbsMessageCleanupTimer::NewL( *this, *array );
		iMessageCleanupTimerArray->AppendL( messageCleanUpTimer );
        
        CleanupStack::Pop(); // array
        }
    CBSLOGSTRING("CBSSERVER: <<< CCbsRecCollector::ConstructL()");
    }


// -----------------------------------------------------------------------------
// CCbsRecCollector::NewL
// Two-phased constructor.
// -----------------------------------------------------------------------------
//
 CCbsRecCollector* CCbsRecCollector::NewL( CCbsMessageFactory& aFactory )
    {
    CBSLOGSTRING("CBSSERVER: >>> CCbsRecCollector::NewL()");

    CCbsRecCollector* self = new ( ELeave ) CCbsRecCollector( aFactory );
    CleanupStack::PushL( self );
    self->ConstructL();
    CleanupStack::Pop();

    CBSLOGSTRING("CBSSERVER: <<< CCbsRecCollector::NewL()");
    return self;
    }
    
// Destructor
CCbsRecCollector::~CCbsRecCollector()
    {
    CBSLOGSTRING("CBSSERVER: >>> CCbsRecCollector::~CCbsRecCollector()");

    if ( iRootNodeArray )
        {
        TInt rootArrayLength( iRootNodeArray->Count() );
        for ( TInt i( 0 ); i < rootArrayLength; i++ )
            {
            iRootNodeArray->At( i )->ResetAndDestroy();
            }
        iRootNodeArray->ResetAndDestroy();
        delete iRootNodeArray;
        }

    if ( iMessageCleanupTimerArray )
        {
        iMessageCleanupTimerArray->ResetAndDestroy();
        delete iMessageCleanupTimerArray;
        }

    CBSLOGSTRING("CBSSERVER: <<< CCbsRecCollector::~CCbsRecCollector()");
    }

// -----------------------------------------------------------------------------
// CCbsRecCollector::CollectL
// Adds a message to the location pointed by iRootNodeIterator.
//   
// Checks if all pages of message are present. If all pages
// are present, sets aCompleted to ECbsMessageComplete,
// combines message pages into a single message, destroys
// pages from the list and returns the complete message
// to caller in aMessage.

// Algorithm:
//   1.  Check if other pages of this message exist in list.
//       (serial number and message identifier match)
//   2.  True: Check if the other messages in chain need
//       to be deleted. This is based on geographical scope
//       and network information.
//       Add this message to the correct message chain.
//       False: Add this message to the chain pointed
//       by the iRootNodeIterator.
//   3.  Seek out the position in the chosed message chain so that
//       the page number sequence remains ordered (1, 2, .., n)
//   4.  Add the page to the correct position in chain.
//   5.  Check if all pages of this message exist (number count).
//   6.  True: Combine message pages into a single message
//       and return this page to the caller. Set aCompleted
//       to ECbsMessageComplete.
//       False: Set aCompleted to ECbsMessageIncomplete.
//
// Note: Ownership of aMessage assumed. aMessage assumed to be
// on cleanup stack.
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//  
CCbsMessage* CCbsRecCollector::CollectL( 
    CCbsMessage* aMessage, TInt aMessageType )  
    {
	CBSLOGSTRING("CBSSERVER: >>> CCbsRecCollector::CollectL()");

    // 1.
    CMessageBuffer* array = FindChainContainingPage( *aMessage );
    CCbsMessage* mergedMessage = NULL;

    if ( array  ) 
        {		
        CBSLOGSTRING("CBSSERVER: CCbsRecCollector::CollectL(): array != NULL");

        // 2. (True), 3.
        __ASSERT_DEBUG( array->Count() > 0, 
            CbsServerPanic( ECbsCollectorArrayEmpty ) );

        // Check geographical scope and network info to decide whether
        // existing pages in this chain should be deleted.
        TBool preserveExistingPages( 
            CheckPageAreaInfoMatch( *aMessage, *array->At( 0 ) ) );
		
        CBSLOGSTRING2("CBSSERVER: CCbsRecCollector::CollectL(): preserveExistingPages: %d", preserveExistingPages );

        if ( preserveExistingPages ) 
            {			
            CBSLOGSTRING("CBSSERVER: CCbsRecCollector::CollectL(): Adding msg to chain...");
            // aMessage is deleted, if it is a duplicate.
            AddMessageToChainL( aMessage, *array );			
            CBSLOGSTRING("CBSSERVER: CCbsRecCollector::CollectL(): Adding msg to chain OK.");
            }
        else 
            {			
            CBSLOGSTRING("CBSSERVER: CCbsRecCollector::CollectL(): Deleting chain...");
            DeleteChainL( *array );			
            CBSLOGSTRING("CBSSERVER: CCbsRecCollector::CollectL(): Deleting chain OK.");
            array->InsertL( 0, aMessage );			
            CBSLOGSTRING("CBSSERVER: CCbsRecCollector::CollectL(): Msg added to array.");
            }
        // aMessage, ownership transferred to msg chain or
        // aMessage has been deleted by AddMessageToChainL
        CleanupStack::Pop(); 

        // 5. Check if this chain contains all pages of the message.
        if ( AllPagesPresent( *array ) )
            {			
            CBSLOGSTRING("CBSSERVER: CCbsRecCollector::CollectL(): All pages present, merging...");

            // 6. merge creates a new copy of this message
            // leaves a pointer to msg to cleanup stack
            mergedMessage = MergePagesLC( *array ); // on CS			
            CBSLOGSTRING("CBSSERVER: CCbsRecCollector::CollectL(): Merging OK.");

			// we stop timer if its livecast message
			if ( aMessageType == ECbsMessageLivecast )
				{				
                CBSLOGSTRING("CBSSERVER: CCbsRecCollector::CollectL(): Msg type == ECbsMessageLivecast, stopping timer.");

				TKeyArrayFix key(0, ECmpTUint16);
				TInt index;
				iRootNodeArray->Find( array, key, index);
				iMessageCleanupTimerArray->At( index )->StopTimer();
                
                CBSLOGSTRING("CBSSERVER: CCbsRecCollector::CollectL(): Timer stopped.");
				}

			CBSLOGSTRING("CBSSERVER: CCbsRecCollector::CollectL(): Deleting chain...");
            DeleteChainL( *array );
			CBSLOGSTRING("CBSSERVER: CCbsRecCollector::CollectL(): Deleting chain OK.");

            CleanupStack::Pop(); // mergedMessage
            }
        }
    else
        {		
        CBSLOGSTRING("CBSSERVER: CCbsRecCollector::CollectL(): array == NULL");

		// if message pagenumber does not start from 1, its not inserted to chain.
		if ( aMessage->ThisPage () == 1 ) 
			{			
            CBSLOGSTRING("CBSSERVER: CCbsRecCollector::CollectL(): aMessage->ThisPage () == 1");

			// 2. (False)
			// add this page as the first node in chain pointed by 
			// iRootNodeIterator. Delete any pages contained in the chain
			// occuping this location first.
			array = iRootNodeArray->At( iRootNodeIterator );
			DeleteChainL( *array );
			array->InsertL( 0, aMessage );
			CleanupStack::Pop(); // aMessage, ownership transferred to msg chain.
			iRootNodeIterator++;
			// Return to the first message, if passed the
			// maximum messages.
			iRootNodeIterator %= KMaxCollectorMessages;

			if ( aMessageType == ECbsMessageLivecast )
				{				
                CBSLOGSTRING("CBSSERVER: CCbsRecCollector::CollectL(): aMessageType == ECbsMessageLivecast");

				// start timeout timer for livecast message
				TKeyArrayFix key(0, ECmpTUint16);
				TInt index;
				iRootNodeArray->Find( array, key, index);
				iMessageCleanupTimerArray->At( index )->StartTimer();
				
                CBSLOGSTRING("CBSSERVER: CCbsRecCollector::CollectL(): Timer started.");
				}
			}
		else
			{			
            CBSLOGSTRING("CBSSERVER: CCbsRecCollector::CollectL(): CleanupStack::Pop()");
			CleanupStack::Pop();
			}
        }
    // mergedMessage == NULL if msg not completed,
    // otherwise return the complete message
	CBSLOGSTRING("CBSSERVER: <<< CCbsRecCollector::CollectL(), returning mergedMessage.");

    return mergedMessage;
    }

// -----------------------------------------------------------------------------
// CCbsRecCollector::DeleteChainL
// Deletes all message pages contained in aArray.
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//  
void CCbsRecCollector::DeleteChainL( 
    CMessageBuffer& aArray ) const
    {
    aArray.ResetAndDestroy();
    }

// -----------------------------------------------------------------------------
// CCbsRecCollector::AllPagesPresent
// Returns ETrue if all pages of the message of aArray are present.
// Counts pages in message chain aArray and compares the result
// against the total number of pages in the message.
// (other items were commented in a header).
// -----------------------------------------------------------------------------
// 
TBool CCbsRecCollector::AllPagesPresent( 
    const CMessageBuffer& aArray ) const
    {
    TBool result( ETrue );
    if ( TUint( aArray.Count() ) < aArray.At( 0 )->TotalPages() )
        {
        result = EFalse;        
        }
    
    return result;
    }

// -----------------------------------------------------------------------------
// CCbsRecCollector::MergePagesLC
// Returns a complete message in aMessage.
// Merges all pages in message chain aArray and returns
// a pointer to the resulting assembled message. The pointer
// is also left on the cleanup stack.
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
CCbsMessage* CCbsRecCollector::MergePagesLC( 
    CMessageBuffer& aArray ) const
    {
    if ( aArray.Count() <= 0 )
        {
        User::Leave( KErrNotFound );
        }
    // Create a new message based on first message page in the chain.
    CCbsMessage* message = iFactory.CreateMessageL( *aArray.At( 0 ) );

    CleanupStack::PushL( message ); // left on cleanup stack

    // Traverse through the chain and merge contents.
    TInt length( 0 );
    TInt count( aArray.Count() );

    // If this is a Livecast message, use the 8-bit representation
    // (message not decoded).
    if ( message->IsLivecastMessage() )
        {
        for ( TInt j( 0 ); j < count; j++ )
            {
            length += aArray.At( j )->Contents8().Length();
            }
        
        __ASSERT_DEBUG( length >= 0, CbsServerPanic( ECbsCollectorMergeFailed ) );

        message->ReserveContentSize8L( length );
        count = aArray.Count();
        
        for ( TInt i( 1 ); i < count; i++ ) 
            {
            message->AppendContent8( aArray.At( i )->Contents8() );
            }
        }
    // Else use the 16-bit representation (message already decoded)
    else
        {
        for ( TInt j( 0 ); j < count; j++ )
            {
            length += aArray.At( j )->Contents().Length();
            }
    
	    __ASSERT_DEBUG( length >= 0, CbsServerPanic( ECbsCollectorMergeFailed ) );

	    message->ReserveContentSizeL( length );
	    count = aArray.Count();

    // Append the rest of the pages (first page handled earlier)
	    for ( TInt i( 1 ); i < count; i++ ) 
	        {
	        message->AppendContent( aArray.At( i )->Contents() );
	        }
        }
    return message;
    }

// -----------------------------------------------------------------------------
// CCbsRecCollector::FindChainContainingPage
// Returns the buffer containing pages of same message.
// Finds and returns a message chain which already contains pages
// of aMessage's message. If none is found, NULL is returned.
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
CMessageBuffer* CCbsRecCollector::FindChainContainingPage( 
    const CCbsMessage& aMessage ) const
    {
    TBool quitSeek( EFalse );
    TInt seekIterator( 0 );
    TCbsDbMessageKey key( aMessage.Key() );
    TCbsDbTopicNumber topicNumber( aMessage.TopicNumber() );
    CMessageBuffer* array = NULL;
    
    // find out if the root array contains pages of this message
    while ( ( seekIterator < KMaxCollectorMessages ) && !quitSeek  )
        {
        array = iRootNodeArray->At( seekIterator );
        if ( array->Count() > 0 ) 
            {
            CCbsMessage* msg = array->At( 0 );
            if ( key == msg->Key() && topicNumber == msg->TopicNumber() )
                {
                quitSeek = ETrue;
                }
            }
        seekIterator++;
        }

    if ( !quitSeek )
        {
        array = NULL;
        }
    
    return array;
    }

// -----------------------------------------------------------------------------
// CCbsRecCollector::AddMessageToChainL
// Adds message page aMessage to the correct position in message chain aArray
//        
// Message chains are ordered in ascending page number order.
// Duplicate pages are not accepted.
//
// Ownership of aMessage is transferred to aArray, if the given page
// hasn't been already collected. The given page will be deleted,
// if it already exists in the chain.
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void CCbsRecCollector::AddMessageToChainL( 
    CCbsMessage* aMessage, 
    CMessageBuffer& aArray ) const
    {
    // Find out a position for this page in the chain.
    TInt chainLength( aArray.Count() );    
    TInt insertPosition( -1 );
    TBool duplicate( EFalse );

    for ( TInt i( 0 ); ( i < chainLength ) && !duplicate; i++ )
        {
        CCbsMessage* msg = aArray.At( i );
        if ( insertPosition == -1 && msg->ThisPage() > aMessage->ThisPage() )
            {
            insertPosition = i;
            }
        else if ( msg->ThisPage() == aMessage->ThisPage() )
            {
            duplicate = ETrue; // This page has been already collected
            delete aMessage;
            }
        }

    // If this message was not a duplicate, add it to the chain
    if ( !duplicate )
        {
        if ( insertPosition == -1 )
            {
            aArray.AppendL( aMessage );
            }
        else
            {
            aArray.InsertL( insertPosition, aMessage );
            }
        }    
    }

// -----------------------------------------------------------------------------
// CCbsRecCollector::CheckPageAreaInfoMatch
// Checks if these pages can be merged. Returns ETrue, if merging is acceptable.
//   
// Decision is based network information and geographical scope of 
// pages. Network information consists of cell id, location area code
// and operator id.
// 
// Assumption: aPage1 and aPage have identical message
// identifiers and serial numbers. 
// Returns ETrue if the pages are of same message.
// On EFalse previous pages should be deleted.
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
TBool CCbsRecCollector::CheckPageAreaInfoMatch( 
    const CCbsMessage& aPage1, 
    const CCbsMessage& aPage2 ) const
    {   
    RMobilePhone::TMobilePhoneNetworkInfoV1 info1;
    RMobilePhone::TMobilePhoneNetworkInfoV1 info2;

    aPage1.GetPLMN( info1 );
    aPage2.GetPLMN( info2 );

    TBool result( EFalse );

    if ( info1.iCountryCode == KRecMessageNoNetworkInfo || 
        info2.iCountryCode == KRecMessageNoNetworkInfo )
        {
        // Network info is unavailable; we must assume that the aPage1
        // is of the same page area as aPage2, so we return ETrue
        // unconditionally.
        result = ETrue;
        }

    if ( info1.iCountryCode == info2.iCountryCode && 
        info1.iNetworkId == info2.iNetworkId )
        // PLMN match
        {
        if ( aPage1.LAC() == aPage2.LAC() )
            // LAC match
            {
            if ( aPage1.CellId() == aPage2.CellId() )
                // Cell match
                {
                // Full network information match
                result = ETrue;
                }
            else
                {
                // Cell mismatch
                if ( aPage1.GeographicalScope() == 
                    ECbsRecGeographicalScopeCell )
                    // Cell id mismatch and scoped cell wide.
                    {
                    result = EFalse;
                    }
                else
                    // Cell id mismatch, not scoped cell wide.
                    {
                    result = ETrue;
                    }
                }
            }            
        else
            // LAC mismatch
            {
            if ( aPage1.GeographicalScope() == 
                ECbsRecGeographicalScopePLMN )
                {
                // LAC mismatch but scoped operator-wide
                result = ETrue;
                }
            else
                {
                // LAC mismatch and not scoped operator-wide
                result = EFalse;
                }
            }
        }
    else
        // PLMN mismatch
        {
        result = EFalse; // operator mismatch
        }

    return result;    
    }


// ================= OTHER EXPORTED FUNCTIONS ==============

//  End of File  
