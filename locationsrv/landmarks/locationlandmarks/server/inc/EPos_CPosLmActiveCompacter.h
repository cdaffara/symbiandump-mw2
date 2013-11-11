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
* Description: Declaration of Active object that compacts database.
*
*
*/


#ifndef CPOSLMACTIVECOMPACTER_H
#define CPOSLMACTIVECOMPACTER_H

//  INCLUDES
#include <e32base.h>
#include <epos_cposlmlocaldbaccess.h>

// CONSTANTS

// FORWARD DECLARATIONS
class CPosLmLocalAccessSubsession;
class CPosLmCompactDatabase;

// CLASS DECLARATION

/**
* Active object that compacts database.
*/
class CPosLmActiveCompacter : public CActive
    {
    public:  // Constructors and destructor

        /**
        * Two-phased constructor.
        *
        * @param aOwner The owner of the database lock.
        * @param aDb The database to compact.
        */
        static CPosLmActiveCompacter* NewL(
        /* IN */        CPosLmLocalAccessSubsession& aOwner,
        /* IN */        CPosLmLocalDbAccess& aDb
        );

        /**
        * Destructor.
        */
        virtual ~CPosLmActiveCompacter();

    public: // From base classes

        /**
        * From CActive.
        *
        */
        void DoCancel();

        /**
        * From CActive.
        *
        */
        void RunL();

        /**
        * From CActive.
        *
        * @param aError The leave code.
        * @return Returns KErrNone.
        */
        TInt RunError(
        /* IN */    TInt aError
        );

    private:

        /**
        * C++ default constructor.
        *
        * @param aDb The database to compact.
        */
        CPosLmActiveCompacter(
        /* IN */        CPosLmLocalAccessSubsession& aOwner
        );

        /**
        * By default Symbian 2nd phase constructor is private.
        *
        * @param aDb The database to compact.
        */
        void ConstructL(
        /* IN */        CPosLmLocalDbAccess& aDb
        );

        /**
        * Completes this active object after each incremental step.
        */
        void CompleteSelf();

    private:    // Data

        CPosLmLocalAccessSubsession& iOwner;

        CPosLmCompactDatabase* iCompactDb;

    };

#endif      // CPOSLMACTIVECOMPACTER_H

// End of File

