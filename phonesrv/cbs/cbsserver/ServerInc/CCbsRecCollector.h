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
* Description:  This file contains the header file of the CCbsRecCollector class.
*
*/


#ifndef CCBSRECCOLLECTOR_H
#define CCBSRECCOLLECTOR_H

// INCLUDES
#include <e32base.h>

// FORWARD DECLARATIONS
class TCbsMessageCompletion;
class CCbsRecMessage;
class CCbsMessage;
class CCbsMessageFactory;
class CCbsMessageCleanupTimer;

// DATA TYPES
// Dynamic array that contains collected message pages
typedef CArrayPtrFlat<CCbsMessage> CMessageBuffer;


// CLASS DECLARATION

/**
*   This class represents a collector, which stores pages of multipaged 
*   messages. Complete messages are assembled and returned back to 
*   the caller of CollectL() method.
*  
*   CCbsRecCollector stores message pages in a number of dynamic arrays.
*   If all but one page of a message are present in collector, and
*   the remaining page is received, the pages will be assembled and
*   the corresponding message chain deleted.
*
*   The maximum number of incomplete messages stored in collector at once
*   is fixed and determined by KMaxCollectorMessages in CCbsRecCollector.cpp.
*
*   CCbsRecCollector implements a circular list to contain message.
*   Each incomplete message occupies a slot in this list. If the list already
*   contains KMaxCollectorMessages messages, the next received multipaged
*   message will delete all received pages of the oldest message in list.
*
*   On receival of a message page, the collector compares network information
*   (PLMN, LAC, CellId) of both messages to decide whether pages are of
*   the same message. In short, for pages to be of the same message
*   their network information have to meet the requirements set by the 
*   geographical scope of the already collected page.
*   See ETSI GSM 03.41 for a detailed description. 
*   
*/
class CCbsRecCollector : public CBase
    {
    public:  // Constructors and destructor
        
        /**
        *   Two-phased constructor.
        *
        *   @param  aFactory            Message factory instance
        *   @return                     New CCbsRecCollector instance
        */
         static CCbsRecCollector* NewL( CCbsMessageFactory& aFactory );
        
        /**
        *   Destructor.
        */
        virtual ~CCbsRecCollector();

    public: // New functions

        /**
        *   Called by the Receiver when a multipaged message is received.
        *   This method checks for other pages of this message to be
        *   present in the collector. If all pages are present the
        *   collector returns a pointer to completed message.
        *
        *   If some pages are still missing, function stores 
        *   the message page and returns NULL.
        *
        *   @param aMessage             Multipaged message page
        *   @return                     NULL or completed CB message
        */
        CCbsMessage* CollectL( CCbsMessage* aMessage, TInt aMessageType );
 
        /**
        *   Deletes all message pages contained in aArray.
        *
        *   @param aArray               Message chain.
        */
        void DeleteChainL( CMessageBuffer& aArray ) const; 

    private:    // new functions

        /**
        *   C++ default constructor.
        *
        *   @param aListener            Message factory instance
        */
        CCbsRecCollector( CCbsMessageFactory& aFactory  );

        /**
        *   By default constructor is private.
        */
        void ConstructL();

        /**
        *   Counts pages in message chain aArray and compares the result
        *   against the total number of pages in the message.

        *   @param  aArray              Message chain.
        *   @return                     ETrue, if the chain contains all pages
        *                               of the message and is ready to be 
        *                               merged.
        */
        TBool AllPagesPresent( const CMessageBuffer& aArray ) const;

        /**
        *   Merges all pages in message chain aArray and returns
        *   a pointer to the resulting assembled message. The pointer
        *   is also left on the cleanup stack.
        *
        *   @param aArray               Message chain
        *   @return                     Completed CB message
        */
        CCbsMessage* MergePagesLC( CMessageBuffer& aArray ) const;

        /**
        *   Finds and returns a message chain which already contains pages
        *   of aMessage's message. If none is found, NULL is returned.
        *
        *   @param aMessage             Message page for which a correct chain
        *                               is sought.
        *   @return                     NULL or message chain that contains
        *                               aMessage's pages.
        */
        CMessageBuffer* FindChainContainingPage( const CCbsMessage& aMessage ) const;
        
        /**
        *   Adds message page aMessage to the correct position in message chain
        *   aArray. Message chains are ordered in ascending page number order.
        *   Duplicate pages are not accepted.
        *
        *   Ownership of aMessage is transferred to aArray, if the given page
        *   hasn't been already collected. The given page will be deleted,
        *   if it already exists in the chain.
        *
        *   @param aMessage             Message page, which is inserted into 
        *                               the chain.
        *   @param aArray               Message chain.
        */
        void AddMessageToChainL( CCbsMessage* aMessage, 
            CMessageBuffer& aArray ) const;

        /**
        *   Checks if these pages can be merged. Returns ETrue, if merging is
        *   acceptable.
        *
        *   Decision is based network information and geographical scope of 
        *   pages. Network information consists of cell id, location area code
        *   and operator id.
        *
        *   @param aPage1               Message page.
        *   @param aPage2               Message page.
        */
        TBool CheckPageAreaInfoMatch( const CCbsMessage& aPage1, 
            const CCbsMessage& aPage2 ) const;

    private:    // prohibited functions
        // Prohibited copy constructor
        CCbsRecCollector( const CCbsRecCollector& );

        // Prohibited assignment operator
        CCbsRecCollector& operator=( const CCbsRecCollector& );

    private:    // data
        // Contains pointers to buffers, each containing a single partial
        // message with 1 or more pages. 
        CArrayPtrFlat< CMessageBuffer >* iRootNodeArray;

        // An iterator which points to a location in iRootNodeArray next used
        // to store a page of a message, which is "new" to the collector.
        // Being "new" here means that the collector doesn't contain any
        // pages of this message.
        TInt iRootNodeIterator;

        // Message factory
        CCbsMessageFactory& iFactory;
		CArrayPtrFlat< CCbsMessageCleanupTimer >*	iMessageCleanupTimerArray;

    };

#endif      // CCBSRECCOLLECTOR_H
            
// End of File
