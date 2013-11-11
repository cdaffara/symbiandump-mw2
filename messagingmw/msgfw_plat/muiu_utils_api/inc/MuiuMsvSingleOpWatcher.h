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
* Description:  
*     Messaging progress watching classes
*
*/




#ifndef __MUIUMSVSINGLEOPWATCHER_H__
#define __MUIUMSVSINGLEOPWATCHER_H__

//  INCLUDES
#include <e32base.h>
#include <msvstd.h>


// CONSTANTS
const TInt KMsvSingleOpWatcherArrayGranularity = 4;


// FORWARD DECLARATIONS
class CMsvOperation;
class MMsvSingleOpWatcher;



// CLASS DECLARATION

/**
* CMsvOperation watcher class.
*/
class CMsvSingleOpWatcher : public CActive
    {
    public:
        /**
        * Two-phased constructor.
        */
        IMPORT_C static CMsvSingleOpWatcher* NewLC( MMsvSingleOpWatcher& aObserver );

        /**
        * Two-phased constructor.
        */
        IMPORT_C static CMsvSingleOpWatcher* NewL( MMsvSingleOpWatcher& aObserver );

        /**
        * Destructor
        */
        IMPORT_C ~CMsvSingleOpWatcher();

        /**
        * Sets operation
        * Must only be called once during the lifetime of a CMsvSingleOpWatcher
        * object.
        */
        IMPORT_C void SetOperation( CMsvOperation* aOperation );

        /**
        * Returns reference to the operation
        */
        IMPORT_C CMsvOperation& Operation() const;

    private:
        /**
        * C++ constructor
        */
        CMsvSingleOpWatcher( MMsvSingleOpWatcher& aObserver );

        /**
        * Symbian OS constructor
        */
        void ConstructL();

        /**
        * From CActive
        */
        virtual void DoCancel();

        /**
        * From CActive
        */
        virtual void RunL();

    private:
        MMsvSingleOpWatcher& iObserver;
        CMsvOperation *iOperation;
    };


/**
* Mixin class MMsvSingleOpWatcher
*
* Function OpCompleted is called when operation is completed.
*/
class MMsvSingleOpWatcher
    {
    public:
        virtual void OpCompleted( CMsvSingleOpWatcher& aOpWatcher, TInt aCompletionCode ) = 0;
    };


/**
* Class CMsvSingleOpWatcherArray
*
* Use this define when we want to use an array of CMsvSingleOpWatcher objects.
*/

typedef CArrayPtrFlat<CMsvSingleOpWatcher> CMsvSingleOpWatcherArray;


#endif // __MSVSINGLEOPWATCHER_H__

// End of file
