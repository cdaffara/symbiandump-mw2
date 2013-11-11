/*
* Copyright (c) 2007 Nokia Corporation and/or its subsidiary(-ies).
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
* Description:  Active object to cleanup broken links
*
*/


#ifndef C_CMPXBROKENLINKCLEANUP_H
#define C_CMPXBROKENLINKCLEANUP_H


// FOWARD DECLARATION
class MMPXFileScanStateObserver;
class MMPXBrokenLinkObserver;

/**
 *  Active Object to cleanup broken links
 *
 *  @lib mpxfilehandler.lib
 *  @since S60 3.1
 */
NONSHARABLE_CLASS( CMPXBrokenLinkCleanup ) : public CActive
    {
public:

    static CMPXBrokenLinkCleanup* NewL( MMPXFileScanStateObserver& aObserver,
                                        MMPXBrokenLinkObserver& aBrokenLinkObserver );

    virtual ~CMPXBrokenLinkCleanup();

public: // New functions
    
    /**
    * Start the cleanup operation
    */
    void Start();
    
    /**
    * Reset the object
    */
    void Reset();
    
    /**
    * Add a broken link for cleanup
    * @param aFile file to remove
    * @param aColid collection id of the file
    * @param aDb database of the item, ownership not xfer
    */
    void AddBrokenLinkL( const TDesC& aFile,
                         TInt aColId,
                         CMPXHarvesterDB* aDb );
    
protected: // From Base Class

    /**
    * From CActive
    */
    void DoCancel();
    
    /**
    * From CActive
    */ 
    void RunL();
    
    /**
    *  From CActive
    */
    TInt RunError(TInt aError);    

private: // New functions
    
    /**
    * Performs one iteration of the broken link check 
    * @return ETrue if all done, EFalse otherwise
    */
    TBool DoBrokenLinkL();
    
private:

    /**
    * Private constructor
    */
    CMPXBrokenLinkCleanup( MMPXFileScanStateObserver& aObserver,
                           MMPXBrokenLinkObserver& aBrokenLinkObserver );

    /**
    * Two Phased Constructor
    */
    void ConstructL();

private: // data
    TBool iCleaningUp; // Currently cleaning up
    
    MMPXFileScanStateObserver& iStateObserver; // Observer for the state
    MMPXBrokenLinkObserver&    iBrokenLinkObserver;
    CDesCArray*                iBrokenLinks;
    RArray<TInt>               iColIds;
    RPointerArray<CMPXHarvesterDB> iDBRefs; // objects not owned.
    };

#endif // C_CMPXBROKENLINKCLEANUP_H
