/*
* Copyright (c) 2002 - 2007 Nokia Corporation and/or its subsidiary(-ies).
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
* Description:  Obtain input from console or output to console
*
*/

#ifndef CCONSOLEREADER_H
#define CCONSOLEREADER_H

// INCLUDES
#include <aknlists.h>
#include <e32cons.h>

// FORWARD DECLARATIONS
class CConsoleMain;

/**
*  console read class
*
*  @lib AddonMenu.lib
*/
class CConsoleReader
    :public CActive
    {
    public:  // Constructors and destructor

        /**
        * Two-phased constructor
        * @param aMain, main console
        * @param aConsole, console
        * @return object of CConsoleReader
        */
        static CConsoleReader* NewL( CConsoleMain* aMain, 
                                     CConsoleBase* aConsole );

        /**
        * Start
        */
        void Start();

        /**
        * Destructor of CConsoleReader.
        */
        virtual ~CConsoleReader();


    public: // Functions from base classes

        /**
        * RunL derived from CActive handles the completed requests.
        */
        void RunL();

        /**
        * DoCancel derived from CActive handles the Cancel
        */
        void DoCancel();

    private:

        /** 
        * C++ default constructor.
        * @param aMain, main console
        * @param aConsole, console
        */
        CConsoleReader( CConsoleMain* aMain, 
                        CConsoleBase* aConsole );

        /**
        * the second phase constructor ConstructL to safely construct things 
        * that can leave 
        */
        void ConstructL();

    private:    // Data
        CConsoleBase*   iConsole;   // Pointer to console
        CConsoleMain*   iMain;      // Backpointer
    };

#endif
