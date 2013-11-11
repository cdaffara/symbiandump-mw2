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
* Description:  Call barring class. Sets and gets call barring setting.
*
*
*/


#ifndef     PSETCALLBARRING_H
#define     PSETCALLBARRING_H

// INCLUDES
#include "mpsetbarringobs.h" 
#include "mpsetcallbarring.h" 
#include "msssettingsobserver.h" 
#include "nwdefs.h"             
#include <mmretrieve.h>
#include <etelmm.h>

// FORWARD DECLARATIONS
class MPsetRequestObserver;
class RSSSettings;

// CLASS DEFINITIONS
/**
*  CPsetCallBarring class is call barring SS handler. 
*  @lib phonesettings.lib
*  @since 1.0
*/
class CPsetCallBarring : public CActive,
                         public MPsetCallBarring,
                         public MSSSettingsObserver
    {
    public:

        // The type of request.
        enum TPSetBarringRequest
            {
            EPSetRequestNone,
            EPSetRequestChangeBarring,
            EPSetRequestGetStatus,
            EPSetRequestChangeBarringPwd,
            EPSetRequestChangeCommonPwd
            };

        /**
        * Symbian OS 2-phase Constructor.
        *
        * @param aObserver Observer for call barring.
        * @param aPhone Provides client access to mobile phone 
        *               functionality provided by TSY.
        * @return Created CPsetCallBarring object.
        */
        /*****************************************************
        *    Series 60 Customer / ETel
        *    Series 60  ETel API
        *****************************************************/
        static CPsetCallBarring* NewL( MPsetBarringObserver& aObserver, 
            RMobilePhone& aPhone );

        /**
        * Destructor.
        */
        IMPORT_C ~CPsetCallBarring();

    public: //methods from base classes

        /**
        * @see MPsetCallBarring::SetBarringL().
        */
        IMPORT_C void SetBarringL( const TCallBarringSetting& aBarring, 
            TBasicServiceGroups aBsc );

        /**
        * @see MPsetCallBarring::GetBarringStatusL().
        */      
        IMPORT_C void GetBarringStatusL( const TServiceGroup aGroup, 
            const TBarringProgram aMode ); 

        /**
        * @see MPsetCallBarring::CancelCurrentRequest().
        */
        IMPORT_C TInt CancelCurrentRequest();
        
        /**
        * @see MSSSettingsObserver::PhoneSettingChanged().
        */
        void PhoneSettingChanged( TSSSettingsSetting aSetting, TInt aNewValue );     

    public: //new

        /**
        * DEPRECATED - calls the method below.
        */
        /*****************************************************
        *    Series 60 Customer / ETel
        *    Series 60  ETel API
        *****************************************************/
        IMPORT_C void ChangePasswordL(
            RMobilePhone::TMobilePhonePasswordChangeV2& aPwds,
            TBool aIsBarringPassword );

        /**
        * Request to change ss password.
        * @param aPwds Passwords (old, new, verified new) 
        *              needed for the operation.
        * @since 2.0
        */
        /*****************************************************
        *    Series 60 Customer / ETel
        *    Series 60  ETel API
        *****************************************************/
        IMPORT_C void ChangePasswordL( 
            RMobilePhone::TMobilePhonePasswordChangeV2& aPwds );
        
        /** 
        * Sets request observer.
        *
        * @param aObs Where the request completion should be informed to.
        */        
        IMPORT_C void SetRequestObserver( MPsetRequestObserver* aObs );

    private:    // constructors

        void ConstructL( MPsetBarringObserver& aObserver );

        /*****************************************************
        *    Series 60 Customer / ETel
        *    Series 60  ETel API
        *****************************************************/
        CPsetCallBarring( RMobilePhone& aPhone );

    private:
    
        void ValidateBsc( TBasicServiceGroups& aBsc );

        void SetObserver( MPsetBarringObserver& aObserver );

        /*****************************************************
        *    Series 60 Customer / ETel
        *    Series 60  ETel API
        *****************************************************/
        RMobilePhone::TMobilePhoneCBCondition SetBarringProgram ( 
            TBarringProgram aMode );

        static void DoHandleLeave( TAny* aAny );

        void CleanupLeavePushL();

        void HandleLeave();

        void StartRequestingL( const TInt& aRequest );

        TBool GetPlurality();

        void RequestCompleted( const TInt& aError );

        //Empties member variables.
        void ClearParams();

        /*****************************************************
        *    Series 60 Customer / ETel
        *    Series 60  ETel API
        *****************************************************/
        RMobilePhone::TMobilePhoneServiceAction SetBarringAction( 
            const TBarringSetting& aSetting );


        void HandleInquiryResultL();

        void SetRequestStatus( TPSetBarringRequest aStatus );

    private: //from base class CActive

        void DoCancel();

        void RunL(); 

    private:
        /*****************************************************
        *    Series 60 Customer / ETel
        *    Series 60  ETel API
        *****************************************************/
        // Provides client access to mobile line functionality provided by TSY.
        RMobileLine* iLine;
        /*****************************************************
        *    Series 60 Customer / ETel
        *    Series 60  ETel API
        *****************************************************/
        // Provides client access to mobile phone functionality provided by TSY.
        RMobilePhone& iPhone;
        // Barring observer, handles results.
        MPsetBarringObserver* iObserver;
        // Currently active request
        TPSetBarringRequest iCurrentReq;
        // Barring settings
        TCallBarringSetting iChangeSetting;
        // Request handler
        MPsetRequestObserver* iReqObserver;
        // Retrieves the Call Barring status list from the phone.
        CRetrieveMobilePhoneCBList* iCbStatusInterrogator;
        //Barring parameters
        /*****************************************************
        *    Series 60 Customer / ETel
        *    Series 60  ETel API
        *****************************************************/    
        RMobilePhone::TMobilePhoneCBChangeV1 iBarringParameters;
        // Value of Alternative Line Selection setting
        TSSSettingsAlsValue iAls;
        // SsSettings pointer, accesses supplementary services on SIM
        RSSSettings* iSsSettings;
    };
#endif      
//  PSETCALLBARRING_H      
// End of File
