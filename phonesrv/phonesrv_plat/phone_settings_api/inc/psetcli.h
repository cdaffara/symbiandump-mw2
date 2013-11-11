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
* Description:  CPsetCli is calling line identification restriction SS
*                setting abstraction. 
*
*
*/


#ifndef     PSETCLIR_H
#define     PSETCLIR_H

// INCLUDES
#include <etelmm.h>
#include "mpsetcliobserver.h" 
#include "mpsetcli.h" 


// FORWARD DECLARATIONS
class MPsetRequestObserver;


// CLASS DECLARATION
/**
*  CPsetCli class is calling line identification restriction SS handler. 
*  @lib phonesettings.lib
*  @since 1.0
*/
class   CPsetCli :  public CActive, 
                    public MPsetCli
    {
    public: // constructor & destructor

        // CLIR's current value
        enum TPsetClirSetting
            {
            EPsetClirPreset = -1,
            EPsetClirActive = 1, 
            EPsetClirInactive = 0
            };

        /**
        * Symbian OS 2-phase constructor.
        * 
        * @param aPhone Provides client access to 
        *               mobile phone functionality provided by TSY.
        * @param aObserver Observer for CLI.
        * @return Created CPsetCli object.
        */
        IMPORT_C static CPsetCli* NewL( RMobilePhone& aPhone, 
            MPsetCliObserver& aObserver );

        /** 
        * Destructor.
        */ 
        IMPORT_C ~CPsetCli();

    public: //from base classes

        /**
        * @see MPsetCli::GetColpModeL
        * Can leave, but due to inheritance not an L-function.
        */
        IMPORT_C void GetColpModeL();

        /**
        * @see MPsetCli::GetClipModeL
        * Can leave, but due to inheritance not an L-function.
        */
        IMPORT_C void GetClipModeL();
       
        /**
        * @see MPsetCli::GetClirModeL
        * Can leave, but due to inheritance not an L-function.
        */
        IMPORT_C void GetClirModeL();

        /**
        * @see MPsetCli::GetColrModeL
        * Can leave, but due to inheritance not an L-function.
        */
        IMPORT_C void GetColrModeL();

        /**
        * @see MPsetCli::CancelAll
        * Can leave, but due to inheritance not an L-function.
        */
        IMPORT_C void CancelAll();        
        
        /*
        * @see MPsetCli::GetCnapL
        * @since Series60_1.2
        */
        IMPORT_C void GetCnapL();

    public: //new

        /*
        * Sets request observer.
        *
        * @param aObs Where the request completion should be informed to.
        */
        IMPORT_C void SetRequestObserver( MPsetRequestObserver* aObs );

    private: //constructor

        /*****************************************************
        *    Series 60 Customer / ETel
        *    Series 60  ETel API
        *****************************************************/
        CPsetCli( RMobilePhone& aPhone );

    private: //from base class CActive
        
        void RunL();

        void DoCancel();

    private: //new

        void SetObserver( MPsetCliObserver& aObserver );

        void CleanupLeavePushL();

        void HandleLeave();

        static void DoHandleLeave( TAny* aAny );

        void RequestCompleted( const TInt& aError );

        /*****************************************************
        *    Series 60 Customer / ETel
        *    Series 60  ETel API
        *****************************************************/
        //Issues requests.
        void IssueRequestL( RMobilePhone::TMobilePhoneIdService aService );

        /*****************************************************
        *    Series 60 Customer / ETel
        *    Series 60  ETel API
        *****************************************************/
        //Set request status.
        void SetRequestStatus( RMobilePhone::TMobilePhoneIdService aStatus );

    private: //Member data
        /*****************************************************
        *    Series 60 Customer / ETel
        *    Series 60  ETel API
        *****************************************************/
        //Provides client access to mobile phone functionality provided by TSY.
        RMobilePhone& iPhone;
        //Observer for CPsetCli.
        MPsetCliObserver* iObserver;
        /*****************************************************
        *    Series 60 Customer / ETel
        *    Series 60  ETel API
        *****************************************************/
        //Current request for CPsetCli.
        RMobilePhone::TMobilePhoneIdService iRequestedService;
        /*****************************************************
        *    Series 60 Customer / ETel
        *    Series 60  ETel API
        *****************************************************/
        //The result of inquiry.
        RMobilePhone::TMobilePhoneIdServiceStatus iServiceStatus;
        //Request observer.
        MPsetRequestObserver* iReqObserver;
    };
#endif // __PSETCLIR_H
// end of file
