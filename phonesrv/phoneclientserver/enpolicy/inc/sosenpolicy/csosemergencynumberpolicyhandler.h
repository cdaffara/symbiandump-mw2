/*
* Copyright (c) 2004-2005 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:  Provides SOS emergency number policy API implementation.
*
*/



#ifndef CSOSEMERGENCYNUMBERPOLICYHANDLER_H
#define CSOSEMERGENCYNUMBERPOLICYHANDLER_H

// INCLUDES
#include    "csosemergencynumberpolicymanager.h" 
#include    "csosenpolicysimstatusmonitor.h" 
#include    "csosenpolicyenlisthandler.h" 
#include    <mmretrieve.h>
#include    <e32property.h>

// DATA TYPES
/*
*   EEnPolicyPanicSystemAgentConnectionFailure     System agent failure.
*/
    enum TEnPolicyPanic
        {
        EEnPolicyPanicCentralRepositoryConnectionFailure = 0
        };

// FORWARD DECLARATIONS
class CSosEnPolicySimStatusMonitor;
class CSosEnPolicyENListHandler;
class CRepository;
class RSatSession;
class RSatRefresh;

// CLASS DECLARATION

/**
*  Active object to handle the Publish&Subscriber notify
*  
*  @since Series 60 Series60_2.8
*/
NONSHARABLE_CLASS( CSosEnPolicyActive ) : public CActive
    {
    public: // Constructors and destructor
        /**
        * Constructor.
        */
        CSosEnPolicyActive( 
            CSosEmergencyNumberPolicyHandler& aEmergNumHandler );

        /**
        * Destructor.
        */
        ~CSosEnPolicyActive();

    private: // Derived from CActive

        /** 
        * @see CActive::DoCancel()
        */
        void DoCancel();

        /**
        * @see CActive::RunL()
        */
        void RunL();

    private: // Data
        CSosEmergencyNumberPolicyHandler&   iEmergNumHandler;

    };


/**
*  CSosEmergencyNumberPolicyHandler
*  Description: Defines SOS emergency number policy.
*
*  @since Series60_2.6
*/
NONSHARABLE_CLASS( CSosEmergencyNumberPolicyHandler ) : 
    public CActive
    {
    public:  // Constructors and destructor
      
        /**
        * Two-phased constructor.
        * @param aETelServer a reference of RTelServer.
        * @return a instance of SOS emergency number policy handler.
        */
        static CSosEmergencyNumberPolicyHandler* NewL( 
            RTelServer& aETelServer );

        /**
        * Destructor.
        */
        ~CSosEmergencyNumberPolicyHandler();

    public: // New
        /**
        * Emergency number check.
        * @param aTelNumber a telephone number to be checked.
        * @param aMode a check mode, Normal/Advanced.
        * @param aIsEmergencyNumber  return True if the number is emergency.
        * @return error code.
        */
        TInt IsEmergencyNumber( 
                                TPhCltTelephoneNumber& aTelNumber, 
                                TPhCltEmergencyNumberQueryMode aMode,
                                TBool& aIsEmergencyNumber );

        /**
        * SIM Emergency numbers action status.
        */
        enum TSosEnPolicySimEmergencyNumberAction 
            {
            // Reads emergency numbers from SIM
            ESosEnPolicySimEmergencyNumberRead,
            // Clears emergency numbers in SIM
            ESosEnPolicySimEmergencyNumberClear
            };

        /**
        * Observer for SIM status changes.
        *
        * @param aAction Action to be handled.
        */
        void CpsssmHandleSimStatusEvent( 
            TSosEnPolicySimEmergencyNumberAction aAction );

    private:  // Functions from base classes

        /** 
        * @see CActive::DoCancel()
        */
        void DoCancel();

        /**
        * @see CActive::RunL()
        */
        void RunL();

        /**
        * From CActive
        */
        TInt RunError( TInt aError );

    private:

        /** 
        * Initializes values to Central Repository list.
        */
        void InitializeCentralRepositoryList();

    private:
        /**
        * C++ default constructor.
        */
        CSosEmergencyNumberPolicyHandler();

        /**
        * Symbian 2nd phase constructor is private.
        * @param aETelServer a reference of RTelServer.
        */
        void ConstructL( RTelServer& aETelServer );

    private:// Data

        // The RMobilePhone instance.
        /*****************************************************
        *   Series 60 Customer / ETel
        *   Series 60  ETel API
        *****************************************************/
        RMobilePhone                    iMobilePhone;

        // SIM status monitor
        CSosEnPolicySimStatusMonitor*   iSimMonitor;

        // Emergency Number list handler
        CSosEnPolicyENListHandler*  iENListHandler;

        // Reads Emergency Number list from the phone or SIM store.
        /*****************************************************
        *   Series 60 Customer / ETel
        *   Series 60  ETel API
        *****************************************************/
        CRetrieveMobilePhoneENList*     iSimEnRetriver;

        // Storage for emergency number read.
        /*****************************************************
        *   Series 60 Customer / ETel
        *   Series 60  ETel API
        *****************************************************/
        RMobileENStore                  iENStore;

        // Central repository instance.
        CRepository*                    iCenRepSession;

        // Sat refresh listener
        CSosEnPolicySatRefresh* iSatRefresh;
        
        //Sim state
        TInt iSimState;
        
    };

#endif  // CSOSEMERGENCYNUMBERPOLICYHANDLER_H
// End of File
