/*
* Copyright (c) 2006 Nokia Corporation and/or its subsidiary(-ies).
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
* Description:  Class for observing changes in central repository values
*
*/


#ifndef CMMPXCENREPWATCHER_H
#define CMMPXCENREPWATCHER_H

//  INCLUDES
#include <e32base.h>
#include <e32cmn.h>
#include <centralrepository.h>

// FORWARD DECLARATIONS
class MMPXCenRepObserver;

// CLASS DECLARATION

/**
*  Class for observing changes in central repository values.
*
*  @lib mpxcommon.lib
*/
class CMPXCenRepWatcher : public CActive
{
public:  // Constructors and destructor

    /**
    * Two-phased constructor.
    *
    * @since S60 3.2.3
    * @param aRepositoryUid uid of CenRep file
    * @param aId id of the key
    * @param aObserver obersver
    * @return contructed object
    */
    IMPORT_C static CMPXCenRepWatcher* NewL(const TUid& aRepositoryUid,
                                   TUint32 aId,
                                   MMPXCenRepObserver* aObserver);

    /**
    * Destructor.
    *
    * @since S60 3.2.3
    */
    IMPORT_C virtual ~CMPXCenRepWatcher();
    
public: // New functions
    /**
    * Get current value as integer.
    *
    * @since S60 3.2.3
    * @return Current value of the key as a TInt
    */
    IMPORT_C TInt CurrentValueL();
    
    /**
    * Set integer value.
    *
    * @since S60 3.2.3
    * @param aNewValue New value for the key
    */
    IMPORT_C void SetValueL(TInt aNewValue);

private: // New functions
    /**
    * C++ constructor.
    *
    * @since S60 3.2.3
    * @param aRepositoryUid uid of CenRep file
    * @param aId id of the key
    * @param aObserver obersver
    * @return contructed object
    */
    CMPXCenRepWatcher(const TUid& aRepositoryUid,
                      TUint32 aId,
                      MMPXCenRepObserver* aObserver);

    /**
    * Symbian OS 2nd phase constructor.
    *
    * @since S60 3.2.3
    */
    void ConstructL();

protected: // Functions from base classes
    /**
    * From CActive.
    *
    * @since S60 3.2.3        
    */
    void RunL();

    /**
    * From CActive.
    *
    * @since S60 3.2.3
    */
    void DoCancel();

private: // Data
    CRepository* iRepository;            // owned
    TUid iRepositoryUid;
    TUint32 iId;
    MMPXCenRepObserver* iObserver; // not owned
};

#endif      // CMMPXCENREPWATCHER_H

// End of File
