/*
* Copyright (c) 2002 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:  CPsetCallWaiting is call waiting SS setting abstraction.
*
*
*/


#ifndef     PSETCALLWAITING_H
#define     PSETCALLWAITING_H

// INCLUDES
#include <etelmm.h>
#include <mmretrieve.h>
#include "msssettingsobserver.h" 
#include "nwdefs.h"
#include "mpsetcallwaiting.h" 
#include "mpsetcallwaitingobs.h" 
#include "psetconstants.h" 
 

// FORWARD DECLARATIONS
class MAdvGsmLineCallWaitingMode;
class MPsetRequestObserver;


// CLASS DECLARATION
/**
*  CPsetCallWaiting class is call waiting SS handler. 
*  @lib phonesettings.lib
*  @since 1.0
*/
class CPsetCallWaiting :    public CActive, 
                            public MPsetCallWaiting,
                            public MSSSettingsObserver
    {
    public: // constructor & destructor

        /**
        * Symbian OS 2-phase Constructor.
        *
        * @param aPhone Provides client access to 
        *               mobile phone functionality provided by TSY.
        * @param aObserver Call Waiting observer.
        * @return Created CPsetCallWaiting object.
        */
        /*****************************************************
        *    Series 60 Customer / ETel
        *    Series 60  ETel API
        *****************************************************/
        IMPORT_C static CPsetCallWaiting* NewL( RMobilePhone& aPhone, 
            MPsetCallWaitingObserver& aObserver );
      
        /**
        * Destructor.
        */
        IMPORT_C ~CPsetCallWaiting();

    public: // from base class MPsetCallWaiting

        /**
        * @see MPsetCallWaiting::SetCallWaitingL().
        */
        IMPORT_C void SetCallWaitingL( 
            TSetCallWaiting aSetting,
            TBasicServiceGroups aBsc );

        /**
        * @see MPsetCallWaiting::GetCallWaitingStatusL(). 
        */
        IMPORT_C void GetCallWaitingStatusL();


        /**
        * @see MPsetCallWaiting::CancelProcess().
        */
        IMPORT_C TInt CancelProcess();

    public: //new

        /**
        * Sets request observer.
        *
        * @param aObs Where the request completion should be informed to.
        */
        IMPORT_C void SetRequestObserver( MPsetRequestObserver* aObs );
        
        /**
        * @see MSSSettingsObserver::PhoneSettingChanged().
        */
        void PhoneSettingChanged( TSSSettingsSetting aSetting, TInt aNewValue );     

    private: // from base class CActive

        void RunL();

        void DoCancel();

    private: //new

        //Service requests for call waiting.
        enum TPSetServiceRequest 
            {
            EPSetServiceRequestNone,
            EPSetServiceRequestGetCallWaitingStatus,
            EPSetServiceRequestSetCallWaiting
            };

        void SetPsuiObserver( MPsetCallWaitingObserver& aObserver );

        void CleanupLeavePushL();

        void HandleLeave();
        
        static void DoHandleLeave( TAny* aAny );

        void RequestCompleted( const TInt& aError );

        void SetRequestStatus( TPSetServiceRequest aStatus );
        
        void ValidateBsc( TBasicServiceGroups& aBsc );

    private:    // constructors
    
        void ConstructL( MPsetCallWaitingObserver& aObserver );
        
        /*****************************************************
        *    Series 60 Customer / ETel
        *    Series 60  ETel API
        *****************************************************/
        CPsetCallWaiting( RMobilePhone& aPhone ); 
 
    private:        

        //Request observer
        MPsetRequestObserver* iReqObserver;        
        /*****************************************************
        *    Series 60 Customer / ETel
        *    Series 60  ETel API
        *****************************************************/
        //Provides client access to mobile phone functionality provided by TSY.
        RMobilePhone& iPhone;
        //Call waiting observer, shows notes associated with call waiting.
        MPsetCallWaitingObserver* iObserver;
        //Currently active request
        TPSetServiceRequest iServiceRequest;
        //Call waiting action type
        MPsetCallWaiting::TSetCallWaiting iSetValue;
        /*****************************************************
        *    Series 60 Customer / ETel
        *    Series 60  ETel API
        *****************************************************/
        //Retrieves the Call Waiting status list from the phone.
        CRetrieveMobilePhoneCWList* iCwInterrogator;     
        // Value of Alternative Line Selection setting
        TSSSettingsAlsValue iAls;
        // SsSettings pointer, accesses supplementary services on SIM
        RSSSettings* iSsSettings;
        // For run time variation.
        TBool iCallWaitingDistinquishEnabled;
    };
#endif // PSETCALLWAITING_H
// end of file