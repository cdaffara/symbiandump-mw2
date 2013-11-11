/*
* Copyright (c) 2005 Nokia Corporation and/or its subsidiary(-ies). 
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
*     Class to handle system backup events.
*
*/


#ifndef CPOSLMBACKUPLISTENER_H
#define CPOSLMBACKUPLISTENER_H

// INCLUDES
#include <e32base.h>
#include <e32property.h>
#include "EPos_MPosLmBackupObserver.h"

// CLASS DECLARATION

/**
 *  Class to handle system backup/restore events.
 */
class CPosLmBackupListener : public CActive
    {
    public:    // Constructors and destructor

        /**
        * Two-phase constructor
        *
        * @param aObserver Pointer to instance of class where callback
        *        method is implemented.
        */
        static CPosLmBackupListener* NewL(
        /* IN  */ MPosLmBackupObserver* aObserver
        );

        /**
        * C++ Destructor
        */
        ~CPosLmBackupListener();

    public:     // New functions

        /**
        * Check if backup is in progress.
        * @return @ETrue if backup is in progress, otherwise @EFalse.
        */
        TBool IsBackupRunning();

    protected:  // Functions from base classes

        /**
        * From CActive.
        * Active Object event handler.
        */
        void RunL();

        /**
        * From CActive.
        * Active Object cancel handler.
        */
        void DoCancel();

    private:

        /**
        * C++ Constructor
        */
        CPosLmBackupListener();

        /**
        *  By default Symbian 2nd phase constructor is private.
        */
        void ConstructL(
        /* IN  */ MPosLmBackupObserver* aObserver
        );

        // By default, prohibit copy constructor
        CPosLmBackupListener(
            const CPosLmBackupListener& );
        // Prohibit assigment operator
        CPosLmBackupListener& operator= (
            const CPosLmBackupListener& );

    private:    // Data

        RProperty iProperty;
        MPosLmBackupObserver* iObserver;
    };

#endif // CPOSLMBACKUPLISTENER_H

// End of File
