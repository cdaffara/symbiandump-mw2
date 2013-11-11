/*
* Copyright (c) 2008-2008 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:  Divert base class.
*
*/


#ifndef CPSETCALLDIVERTINGBASE_H
#define CPSETCALLDIVERTINGBASE_H

//  INCLUDES
#include <e32base.h>
#include <badesca.h>
#include <mmretrieve.h>
#include <ctsydomaincrkeys.h>
#include "mcalldiverting.h" 
#include "msssettingsobserver.h" 
#include "nwdefs.h"             
#include "psetconstants.h" 

// CLASS DECLARATION
class MPsetDivertObserver;

/**
 * CPSetCallDivertingBase is base class for different type of
 * call divert implementations
 *
 *  @lib phonesettings
 *  @since S60 v5.1
 */
NONSHARABLE_CLASS( CPSetCallDivertingBase ) : public CActive,
                                              public MSSSettingsObserver,
                                              public MCallDiverting
    {
    public: // Constructors and destructors.

        /**
        * Two-phased constructor.
        *
        * @return new instance of the class.
        * @param aObserver Call divert observer.
        * @param aPhone Provides client access to 
        *               mobile phone functionality provided by TSY.
        * @param aDivert provides access to CPsetCallDiverting.
        * @return Created CPSetCallDivertingBase object.
        */
        static CPSetCallDivertingBase* NewL(
                MPsetDivertObserver& aObserver, 
                RMobilePhone& aPhone,
                CPsetCallDiverting* aDivert );

        /**
        * Destructor.
        */
        ~CPSetCallDivertingBase();
   
    public: // From base class.
        /**
         * @see MCallDiverting.
         */
        void SetRequestObserver( MPsetRequestObserver* aObs );

        /**
        * @see MCallDiverting.
        */ 
        void SetDivertingL( const TCallDivertSetting& aDivert,  
                            TBasicServiceGroups aBsc );        
        
        /**
        * @see MCallDiverting.
        */ 
        void GetDivertingStatusL( const TServiceGroup aServiceGroup, 
                                  const TCallDivertingCondition aCondition, 
                                  TBasicServiceGroups aBsc );
        /**
        * @see MCallDiverting.
        */ 
        TInt CancelCurrentRequest();
        
        /**
        * @see MCallDiverting.
        */ 
        TInt SetNewDefaultNumberL( TDes& aNumber );
        
        /**
        * @see MCallDiverting.
        */ 
        TInt GetTimerValueL();
        
        /**
        * @see MCallDiverting.
        */ 
        TInt SetTimerValueL( const TInt& aValue );
        
        /**
        * @see MCallDiverting.
        */ 
        void SwapDefaultNumberL( const TInt& aLocation );
        
        /**
        * @see MCallDiverting.
        */ 
        TInt SaveKey( TUint32 aKeyId, const TDesC& aKeyValue );
        
        /**
        * @see MCallDiverting.
        */
        TInt GetNumbersFromSharedDataL( TInt aType, 
                                CDesC16ArrayFlat& aArray );        
    protected:
        
        /**
        * Gets Als status.
        * 
        * @return used ALS line.
        */
        TSelectedLine GetSAAls();
        
        /** 
        * Creates param list for NotifySADivertChange method.
        */ 
        TCallDivertNotifySetting CreateDivertNotifySetting( 
                    TUnconditionalCFStatus aCurrentDivert, 
                    TBool aDivertChanged,
                    TBool aVoiceMbxDivert, 
                    TInt aBasic );
        
        /**
        * If unconditional voice divert is activated/cancelled, notifies observer, 
        * so that it can set on/off indicators.
        * 
        * @param aVmbxDivert vmbx number.
        * @param TDivertingStatus call forwarding status.
        */
        void HandleSANotificationL( TBool aVmbxDivert, TDivertingStatus& aCfStatus );
        
        /**
        * Verifies whether the divert-to number is to voice mail box.
        *
        * @param aTelNumber divert number.
        * @return ETrue if used number is vmbx number
        *       otherwise EFalse.
        */
        TBool IsVMBXDivertL( TDesC& aTelNumber );       
        
        /**
        * If divert activation/cancellation/etc. affects more than one bsc, 
        * inform PSUI about it.
        * 
        * @param aDivert contains forwarding information.
        * @param aBsc used basic service.
        * @return ETrue if divert is multiaffecting
        *       otherwise EFalse.
        */
        TBool IsMultiAffectingDivert( 
                        const TCallDivertSetting& aDivert, 
                        TBasicServiceGroups aBsc );
        
        /**
        * Validates used bsc, if EUnknown, updates to real value.
        * 
        * @param aBsc used basic service.
        */ 
        void ValidateBsc( TBasicServiceGroups& aBsc );
        
        /**
        * Currently active request that is handled by this class.
        */ 
        enum TPSetDivertRequest
            {
            EPSetNone, 
            EPSetChangeDivert,
            EPSetGetDivertStatus
            };
        
        /**
        * Starts new request.
        * 
        * @param aRequest defines request type.
        */ 
        void StartRequestingL( const TInt& aRequest );
        
        /**
        * Inform request observer that request status has changed.
        * 
        * @param aStatus request type.
        */
        void SetRequestStatus( TPSetDivertRequest aStatus );
        
        /**
        * Inform the request observer that request has been completed.
        *
        * @param aError request status.
        */
        void RequestCompleted( const TInt& aError );
        
    private:
        
        /**
        * Push object into cleanupstack to catch leaving.
        */
        void CleanupLeavePushL();
        
        /**
        * Object has caused a leave.
        */
        static void DoHandleLeave( TAny* aAny );
        
        /**
        * Handles leave.
        */
        void HandleLeave();
        
        /**
        * Verifies that delay time is divisible by five.
        */ 
        TBool VerifyDelayTime( const TInt& aDelayTime );
        
    protected:

        /**
        * C++ constructor.
        */
        CPSetCallDivertingBase( RMobilePhone& aPhone, CPsetCallDiverting* aDivert  );
        /**
        * Symbian OS constructor.
        */
        void ConstructL( MPsetDivertObserver& aObserver );
       
    private: //from base clases.

        /**
        * @see CActive::RunL().
        */
        virtual void RunL();
           
        /**
        * @see CActive::DoCancel().
        */
        virtual void DoCancel();
           
        /**
        * @see MSSSettingsObserver::PhoneSettingChanged().
        */
        void PhoneSettingChanged( TSSSettingsSetting aSetting, TInt aNewValue );
        
    protected:
        
        // Provides client access to mobile phone functionality provided by TSY.
        RMobilePhone& iPhone;
        
        // Provides client access to mobile line functionality provided by TSY.
        RMobileLine* iLine;
        
        // Observer for Call divert requests
        MPsetDivertObserver* iObserver;
       
        // Call divert instance.
        CPsetCallDiverting* iDivert;
       
        // Current, active request
        TPSetDivertRequest iCurrentReq;
       
        // Value of Alternative Line Selection setting
        TSSSettingsAlsValue iAls;
       
        // SsSettings pointer, accesses supplementary services on SIM
        RSSSettings* iSsSettings;
       
        // Plural notes are shown when operations affect several groups.
        TBool iPluralNotes;
       
        // Request observer
        MPsetRequestObserver* iReqObserver;
       
        // Call forwarding reason.
        RMobilePhone::TMobilePhoneCFCondition iReason;
       
        // Divert condition
        RMobilePhone::TMobilePhoneCFInfoEntryV1 iDivertInfo;
       
        // Call divert settings
        RMobilePhone::TMobilePhoneCFChangeV1 iChangeInfo;
       
        // Retrieves the Call Forwarding status list from the phone.
        CRetrieveMobilePhoneCFList* iCfInterrogator;
    
        // Central repository.
        CRepository* iRepository;
    
        // Status check.
        TBool iCFStatusCheck;
       
        // Basic service code.
        TInt iBsc;
       
        // Call forwarding settings.
        TCallDivertSetting iDivertStatus;
       
        // Is request connected to video diverts.
        TBool iVideoDivert;
       
        // Is request connected to voice diverts.
        TBool iVoiceDivert;
       
        // Is feature manager intialized. 
        TBool iFeatureManagerInitialized;
    };


#endif      // CPSETCALLDIVERTINGBASE_H 

// End of File
