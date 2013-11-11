/*
* Copyright (c) 2002-2005 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:  CPsetNetwork selects network and gets network information.
*
*
*/


#ifndef     PSETNETWORK_H
#define     PSETNETWORK_H

//  INCLUDES
#include "mpsetnetworkselect.h" 
#include "mpsetnetworkinfoobs.h" 
#include "mpsetnetworkmodeobs.h" 
#include "mpsetnetworkmode.h" 
#include <etelmm.h>
#include <rmmcustomapi.h> 

// FORWARD DECLARATIONS
class CPsetSAObserver;
class CRetrieveMobilePhoneDetectedNetworks;
class RMmCustomAPI;

// CLASS DECLARATION
/**
*  CPsetNetwork class is used to handle network change requests.
*  @lib phonesettings.lib
*  @since 1.0
*/
class   CPsetNetwork :  public CActive, 
                        public MPsetNetworkSelect,
                        public MPsetNetworkMode
    {
    public: // constructor & destructor

        /**
        * Symbian OS 2-phase constructor.
        * 
        * @param aPhone TSY's phone object.
        * @param aObserver Reference to network observer.
        * @return Returns the CPsetNetwork-object
        */
        /*****************************************************
        *    Series 60 Customer / ETel
        *    Series 60  ETel API
        *****************************************************/
        IMPORT_C static CPsetNetwork* NewL( RMobilePhone& aPhone, 
            MPsetNetworkInfoObserver& aObserver );

        /**
        * Symbian OS 2-phase constructor.
        * 
        * @param aPhone TSY's phone object.
        * @param aObserver Reference to network observer.
        * @return Returns the CPsetNetwork-object
        */
        /*****************************************************
        *    Series 60 Customer / ETel
        *    Series 60  ETel API
        *****************************************************/
        IMPORT_C static CPsetNetwork* NewL( RMobilePhone& aPhone, 
            MPsetNetworkModeObserver& aObserver );


        /* Destructor */
        IMPORT_C ~CPsetNetwork();

    public: // from base class MNetworkSelect

        /**
        * @see MNetworkSelect::GetAvailableNetworksL
        */
        IMPORT_C void GetAvailableNetworksL();

        /**
        * @see MNetworkSelect::GetCurrentNetworkInfo
        */
        IMPORT_C TInt GetCurrentNetworkInfo( 
            TCurrentNetworkInfo& aInfo );

        /**
        * @see MNetworkSelect::GetNetworkSelectMode
        */
        IMPORT_C TInt GetNetworkSelectMode( TSelectMode& aMode );

        /**
        * @see MNetworkSelect::SelectNetworkL
        */
        IMPORT_C void SelectNetworkL( const TNetworkInfo& aInfo );

        /**
        * @see MNetworkSelect::CancelProcess
        */
        IMPORT_C void CancelProcess();

    public: // From base class MPsetNetworkMode
        
        /**
        * @see MPsetNetworkMode::SetNetworkModeSelectionL
        * @since 2.6
        */
        IMPORT_C void SetNetworkModeSelectionL( TUint32& aNetworkMode );

        /**
        * @see MPsetNetworkMode::GetCurrentNetworkModeSelectionL
        * @since 2.6
        */
        IMPORT_C void GetCurrentNetworkModeSelectionL();

    public: //new

        /**
        * Checks if there is a call going on.
        *
        * @return Returns a non-zero if there is a call active, or error.
        */
        IMPORT_C TInt IsCallActive();

        /**
        * Checks if there is a gprs connection active.
        * @return KErrNone, or error code.
        */
        IMPORT_C TInt IsGPRSConnected();

        /* 
        * Calls ETEL to start reseting the network.
        * @return Error code, or KErrNone.
        */
        IMPORT_C TInt ResetNetworkSearch();

        /* 
        * Sets observer (Observer starts to listen again if reset).
        * @param aObserver Value to set into observer.
        */
        IMPORT_C void SetNetSAObserver( MPsetNetworkInfoObserver& aObserver );

        /* 
        * Sets Network mode observer
        * @param aObserver Value to set into Network mode observer.
        */
        IMPORT_C void SetNetworkModeObserver( MPsetNetworkModeObserver& 
                                              aObserver );

    private: // from base class CActive

        void RunL();

        void DoCancel();

    private: // constructors

        void ConstructL( MPsetNetworkInfoObserver& aObserver );

        // this is required for the net container to create a pointer
        void ConstructL( MPsetNetworkModeObserver& aObserver );

        /*****************************************************
        *    Series 60 Customer / ETel
        *    Series 60  ETel API
        *****************************************************/
        CPsetNetwork( RMobilePhone& aPhone );        

    private: //new

        void GetRegistrationStatusL();

        void HideRequestNoteL();

        void CleanupLeavePushL();

        void HandleLeave();

        static void DoHandleLeave( TAny* aAny );

        void ClearParams();

    private:
        //Latest network settings information
        TNetworkInfo iTempNetInfo;        
        //Provides client access to mobile phone functionality provided by TSY.
        /*****************************************************
        *    Series 60 Customer / ETel
        *    Series 60  ETel API
        *****************************************************/
        RMobilePhone& iPhone;
        //Currently active request.
        MPsetNetworkInfoObserver::TServiceRequest iServiceRequest;
        //Observer
        CPsetSAObserver* iSAObserver;
        //Network information observer
        MPsetNetworkInfoObserver* iObserver;       
        //Fetched network information (names, IDs)
        /*****************************************************
        *    Series 60 Customer / ETel
        *    Series 60  ETel API
        *****************************************************/
        CRetrieveMobilePhoneDetectedNetworks* iNetworkRetrieve;
        //Current registartion status
        /*****************************************************
        *    Series 60 Customer / ETel
        *    Series 60  ETel API
        *****************************************************/
        RMobilePhone::TMobilePhoneRegistrationStatus iRegStatus;

        /*****************************************************
        *    Series 60 Customer / ETel
        *    Series 60  ETel API
        *****************************************************/
        //Selected network information
        RMobilePhone::TMobilePhoneNetworkManualSelection iNwInfo;

        //
        // Network Mode related variables
        //
        //Network mode observer
        MPsetNetworkModeObserver* iNetworkModeObserver;

        /*****************************************************
        *    Series 60 Customer / ETel
        *    Series 60  ETel API
        *****************************************************/
        // Custom phone.
        RMmCustomAPI iCustomPhone;
        
        //Currently active request.
        MPsetNetworkModeObserver::TServiceRequest iModeRequest;

        //Current Network Mode
        TUint32 iNetworkModeCaps;

        //Currently active observer, for assert handling
        TInt iActiveObserver;
    };
#endif // PSETNETWORK_H
// end of file
