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



#ifndef __EPOS_CSUPLSETTINGSNOTIFIER_H__
#define __EPOS_CSUPLSETTINGSNOTIFIER_H__

//  INCLUDES
#include <e32base.h>

// FORWARD DECLARATIONS
class CRepository;
class MSuplSettingsObserver;

// CLASS DECLARATION

/**
*  This class listens for SUPL settings events. Every change is reported to the
*  observer.
*
* This is part of @lib epos_suplsettings.lib
*  @since 3.1
*/
NONSHARABLE_CLASS(CSuplSettingsNotifier) :public CActive
    {
    public:  // Constructors and destructor

        /**
        * Two-phased constructor.
        *
        * @param aSuplSettingsEngine The SUPL Settings 
        * @param aObserver The SUPL Settings observer
        */
        static CSuplSettingsNotifier* NewL(
        /* IN  */   MSuplSettingsObserver& aObserver,/* IN */ TBool aDBListener = EFalse
        );

        /**
        * Destructor.
        */
        virtual ~CSuplSettingsNotifier();

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
        CSuplSettingsNotifier(
        /* IN  */   MSuplSettingsObserver& aObserver, /*IN*/ TBool aDBListener = EFalse);

        /**
        * By default EPOC constructor is protected.
        */
        void ConstructL();

    private:  // New functions

        void StartListening();
        
    private:
        // By default, prohibit copy constructor
        CSuplSettingsNotifier( const CSuplSettingsNotifier& );
        // Prohibit assigment operator
        CSuplSettingsNotifier& operator= ( const CSuplSettingsNotifier& );

    private:    // Data
        MSuplSettingsObserver&  iObserver;
        CRepository* iRepository;
        
        TBool iStopObserve;

         
        TInt iFallBackValue;
        TInt iFallBackTimerValue;
        TInt iTriggerServiceStatus; // 0 = TriggerON , 1 = TriggerOFF
        
        
        //used to indicate if this object was created to listen for DB Change events
        TBool iDBListener; 
         
        HBufC* iImsi;
    };

#endif      // __EPOS_CSUPLSETTINGSNOTIFIER_H__

// End of File
