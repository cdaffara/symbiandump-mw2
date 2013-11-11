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
*       See class description below.
*
*/



#ifndef __EPOS_CSUPLSESSIONNOTIFIER_H__
#define __EPOS_CSUPLSESSIONNOTIFIER_H__

//  INCLUDES
#include <e32base.h>

// FORWARD DECLARATIONS
class CRepository;
class CActiveSessionDatabaseHandler;
class MSuplSessionObserver;

// CLASS DECLARATION

/**
*  This class listens for SUPL session events. Every change is reported to the
*  observer.
*
* This is part of @lib epos_suplsettings.lib
*  @since 3.1
*/
NONSHARABLE_CLASS(CSuplSessionNotifier) :public CActive
    {
    public:  // Constructors and destructor

       /**
        * Two-phased constructor.
        *
        * @param aObserver The SUPL Settings observer
        */
        static CSuplSessionNotifier* NewL(
        /* IN  */   MSuplSessionObserver& aObserver );
       
       /**
        * Destructor.
        */
        virtual ~CSuplSessionNotifier();

    public: // New functions


    protected:  // Functions from base classes

        /**
        * From CActive
        */
        void RunL();

        /**
        * From CActive
        */
        void DoCancel();

        /**
        * From CActive
        */
        TInt RunError(
        /* IN  */       TInt  aError
        );

    protected:

        /**
        * C++ default constructor.
        */
        CSuplSessionNotifier(
        /* IN  */   MSuplSessionObserver& aObserver);
        
       
        /**
        * By default EPOC constructor is protected.
        */
        void ConstructL();

    private:  // New functions

        void StartListening();
        
    private:
        // By default, prohibit copy constructor
        CSuplSessionNotifier( const CSuplSessionNotifier& );
        // Prohibit assigment operator
        CSuplSessionNotifier& operator= ( const CSuplSessionNotifier& );

    private:    // Data
        MSuplSessionObserver&  iObserver;
        
        CRepository* iRepository;
        
       
    };

#endif      // __EPOS_CSUPLSESSIONNOTIFIER_H__

// End of File
