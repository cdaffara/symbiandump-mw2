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
* Description:  Interface for ALS control
*
*/


#ifndef CSSSETTINGSALSNOTIFIER_H
#define CSSSETTINGSALSNOTIFIER_H

//  INCLUDES
#include <etelmm.h>
#include <cenrepnotifyhandler.h>

#include "rsssettings.h" 
#include "csssettingsactiveobject.h" 

// FORWARD DECLARATIONS
class RMmCustomAPI;
class CRepository;
class CCenRepNotifyHandler;
class RProperty;


// CLASS DECLARATION

/**
*  Interface for Als control.
*
*  @since   1.0
*  @lib     SsSettings.lib
*/
class CSSSettingsAlsNotifier :   public CActive,
    public MCenRepNotifyHandlerCallback,
    public MSSSettingsPubSubNotify
    {
    public:  // Constructors and destructor

        /**
        * Two-phased constructor.
        * @param aMobilePhone Mobile phone
        * @param aCustomPhone Pre-opened custom interface.
        * @param aNotifyHandler Notify changes.
        */
        static CSSSettingsAlsNotifier* NewL(
            RMobilePhone& aMobilePhone,
            RMmCustomAPI& aCustomPhone,
            MCenRepNotifyHandlerCallback& aAlsNotifyHandler );

        /**
        * Destructor.
        */
        virtual ~CSSSettingsAlsNotifier();

    public: // New functions
        
        /**
        * Starts the notifier.
        * @return Error code
        */
        TInt NotifyAlsChange();

        /**
        * Cancel notify
        */
        void CancelNotify();

        /**
        * Set Als value.
        * @param aValue
        * @return If successful, KErrNone
        *         KErrNotSupported MS doesn't support ALS
        */
        TInt SetAlsValue( TSSSettingsAlsValue aValue );

        /**
        * Get Als value
        * @param aValue the value.
        * @return If successful, KErrNone
        */
        TInt GetAlsValue( TSSSettingsAlsValue& aValue );

        /**
        * Get Als support bits.
        * @param aPPSupport Product profile support for als
        * @param aSimSupport Sim support for Als.
        * @param aCSPSupport CSP ALS support for Als.
        * @param aCSPSupport CSP ALS error for als.
        */
        void GetAlsSupport( TBool& aPPSupport, TBool& aSimSupport, TBool& aCSPSupport, TInt& aCSPError );

        /**
        * Handle SIM refresh
        * @return If successful, KErrNone
        */
        TInt HandleRefresh();

        
    protected:  // New functions
        
        /**
        * Gets PP support from ETel.
        * @param aIsAlsSopportedInPP Is supported.
        * @return If successful, KErrNone
        */
        TInt CheckAlsSupportInProductProfile( TBool& aIsAlsSopportedInPP );

        /**
        * Gets Sim support from ETel.
        * @param aIsAlsSopportedInSim Is supported.
        * @return If successful, KErrNone
        */
        TInt CheckAlsSupportInSim( TBool& aIsAlsSopportedInSim );

        /**
        * Checks Als support from Sim and PP if not checked.
        * @return If successful, KErrNone
        */
        TInt EnsureAlsStatus();

    protected:  // Functions from base classes
        
        /**
        * From CActive.
        */
        void RunL();

        /**
        * From CActive.
        */
        void DoCancel();

        /** 
        * From MCenRepNotifyHandlerCallback
        */
        void HandleNotifyInt(
            TUint32 aId,
            TInt aNewValue );

        /** 
        * From MCenRepNotifyHandlerCallback.
        */
        void HandleNotifyError(
            TUint32 aId,
            TInt aError,
            CCenRepNotifyHandler* aHandler );

        /**
        * From MSSSettingsAlsNotify.
        */
        void HandlePubSubNotify(
            const TUid aUid,
            const TUint32 aKeyId );

    private:

        /**
        * C++ constructor.
        */
        CSSSettingsAlsNotifier( 
            RMobilePhone& aMobilePhone,
            RMmCustomAPI& aCustomPhone,
            MCenRepNotifyHandlerCallback& aAlsNotifyHandler );

        /**
        * By default Symbian OS constructor is private.
        */
        void ConstructL();

        // Prohibit copy constructor if not deriving from CBase.
        CSSSettingsAlsNotifier( const CSSSettingsAlsNotifier& );
        // Prohibit assigment operator if not deriving from CBase.
        CSSSettingsAlsNotifier& operator= ( const CSSSettingsAlsNotifier& );
              
        /**
        * Checks if Alternate Line is to be supported in parameter.
        * @param aContainer The parameter to be checked.
        * @return ETrue if supported, otherwise EFalse.
        */
        TBool CheckIfAlsCSPSupported( const RMobilePhone::TCspCPHSTeleservices aContainer ) const;

    private:
         
        //Provides client access to mobile phone functionality provided by TSY.
        RMobilePhone& iMobilePhone;
        
        // Custom phone.
        RMmCustomAPI& iCustomPhone;                 

        // Does PP support ALS.
        TBool iPPSupportAls;
        
        // Does SIM support ALS.
        TBool iSimSupportAls;
        
        // Does SIM support CSP ALS.
        TBool iAlsCSPSupport;
        
        // CSP ALS error.
        TInt iAlsCSPError;
        
        // Is PP and Sim support checked.
        TBool iAlsStatusChecked;
                            
        // Active ALS, only valid in RunL.
        RMobilePhone::TMobilePhoneALSLine iSimActiveAls;
        
        // Als Notify handler.
        MCenRepNotifyHandlerCallback*     iAlsNotifyHandler;

        // Central repository for ALS.
        CRepository*                      iRepository;
        
        // Central repository notify handler for ALS.
        CCenRepNotifyHandler*             iCenRepNotifyHandler;

        // Notifier for ALS support in Product Profile.
        CSSSettingsActiveObject*          iPPSupportsAlNotifier;
        
        // Notifier for ALS support in SIM.
        CSSSettingsActiveObject*          iSimSupportsAlsNotifier;
    };


#endif      // CSSSETTINGSALSNOTIFIER_H
            
// End of File
