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
* Description:  Scrolling menu with timer
*
*/

#ifndef CSCROLLERTIMER_H
#define CSCROLLERTIMER_H

// INCLUDES
#include <aknlists.h>

// FORWARD DECLARATIONS
class CConsoleMain;

/**
*  scrolling timer class
*
*  @lib AddonMenu.lib
*/
class CScrollerTimer
        :public CActive
    {

    public:  // Constructors and destructor

        /**
        * Two-phased constructor
        * @param aMain, main console
        * @return object of CScrollerTimer
        */
        static CScrollerTimer* NewL( CConsoleMain* aMain );

        /**
        * Destructor of CModule.
        */
        virtual ~CScrollerTimer();

    public: // New functions

        /** 
        * Start
        */
        void Start();

    public: // Functions from base classes

        /**
        * RunL derived from CActive handles the completed requests.
        */
        void RunL();

        /**
        * DoCancel derived from CActive handles the Cancel
        */
        void DoCancel();

        /** 
        * RunError
        */
        TInt RunError ( TInt aError );

    private:

        /** 
        * C++ default constructor.
        */
        CScrollerTimer();

        /**
        * the second phase constructor ConstructL to safely construct things 
        * that can leave 
        * @param aMain, main console
        */
        void ConstructL( CConsoleMain* aMain );

    private:    // Data
        CConsoleMain*     iMain;            // Pointer to main console
        RTimer            iTimer;           // timer object

    };

#endif
