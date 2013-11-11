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
* Description: Class compacting a landmark database in steps.
*
*
*/


#ifndef CPOSLMCOMPACTDATABASE_H
#define CPOSLMCOMPACTDATABASE_H

//  INCLUDES
#include <d32dbms.h>

// FORWARD DECLARATIONS
class CPosLmLocalDbAccess;

// CLASS DECLARATION

/**
* Class compacting a landmark database in steps.
*/
class CPosLmCompactDatabase : public CBase
    {

    public:  // Constructors and destructor

        /**
        * Two-phased constructor.
        *
        * @param aDbAccess A database access.
        */
        IMPORT_C static CPosLmCompactDatabase* NewL(
        /* IN */        CPosLmLocalDbAccess& aDbAccess
        );

        /**
        * Destructor.
        */
        virtual ~CPosLmCompactDatabase();

    public: // New functions

        /**
        * Performs one step of the compact operation.
        *
        * @param aProgress Current progress.
        * @return ETrue if the compact is done, otherwise EFalse.
        */
        IMPORT_C TBool NextL(
        /* OUT */       TReal32& aProgress
        );

    private:

        /**
        * C++ default constructor.
        */
        CPosLmCompactDatabase();

        /**
        * By default Symbian 2nd phase constructor is private.
        *
        * @param aDbAccess A database access.
        */
        void ConstructL(
        /* IN */        CPosLmLocalDbAccess& aDbAccess
        );

    private: // Data

        RDbIncremental iDbIncremental;

        TInt iIncrementalStartValue;

        TInt iIncrementalStep;

    };

#endif      // CPOSLMCOMPACTDATABASE_H

// End of File
