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
* Description: A class encapsulating an RDbView.
*
*/


#ifndef CPOSLMDATABASEITERATOR_H
#define CPOSLMDATABASEITERATOR_H

// INCLUDES
#include <e32base.h>
#include <d32dbms.h>

// CONSTANTS

// FORWARD DECLARATIONS
class TPosLmDatabaseSettings;

// CLASS DECLARATION

/**
* A class encapsulating an RDbView.
*
*  @version $Revision: 1.3 $, $Date: 2005/03/09 09:30:54 $
*/
class CPosLmDatabaseIterator : public CBase
    {
    public:  // Constructors and destructor

        /**
        * Two-phased constructor.
        */
        IMPORT_C static CPosLmDatabaseIterator* NewL(RDbView& aView);

        /**
        * Destructor.
        */
        IMPORT_C virtual ~CPosLmDatabaseIterator();

    public:  // New functions

        /**
        * Moves the cursor to the next row in the rowset.
        *
        * @return ETrue if there are more rows, EFalse otherwise.
        */
        IMPORT_C TBool NextItemL();

        /**
        * Returns the database URI.
        *
        * @return A database URI.
        */
        IMPORT_C TPtrC UriL();

        /**
        * Returns the database settings.
        *
        * @param aSettings Contains the database settings on return.
        */
        IMPORT_C void GetSettingsL(
        /* OUT */       TPosLmDatabaseSettings& aSettings
        );

    private:

        /**
        * C++ default constructor.
        */
        CPosLmDatabaseIterator(RDbView& aView);

        /**
        * By default Symbian 2nd phase constructor is private.
        */
        void ConstructL();

        // By default, prohibit copy constructor
        CPosLmDatabaseIterator( const CPosLmDatabaseIterator& );
        // Prohibit assigment operator
        CPosLmDatabaseIterator& operator= ( const CPosLmDatabaseIterator& );

    private:    // Data

        RDbView iView;
        HBufC* iUri;

    };

#endif      // CPOSLMDATABASEITERATOR_H

// End of File
