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
* Description:  Notifier for changes.
*
*/


#ifndef CSSSETTINGSNOTIFIER_H
#define CSSSETTINGSNOTIFIER_H

//  INCLUDES
#include <cenrepnotifyhandler.h>

#include "rsssettings.h" 
#include "csssettingsactiveobject.h" 

// FORWARD DECLARATIONS
class CSSSettingsAlsNotifier;


// CLASS DECLARATION

/**
*  Notifier for changes
*  @since   1.0
*  @lib     SsSettings.lib
*/
class CSSSettingsNotifier :   public CActive,
    public MCenRepNotifyHandlerCallback
    {
    public:  // Constructors and destructor
        
        /**
        * C++ constructor.
        * @param aSettings reference to RSSSettings
        */
        CSSSettingsNotifier( RSSSettings& aSettings );

        /**
        * Symbian OS constructor. Must be called before usage!
        */
        void ConstructL();
        
        /**
        * Destructor.
        */
        virtual ~CSSSettingsNotifier();

    public: // New functions
        
        /**
        * Adds observer.
        * @param aSetting Set observer for this setting
        * @return KErrNone if succesful.
        */
        TInt AddNotifier( TSSSettingsSetting aSetting );

        /**
        * Removes observer
        * @param aSetting Remove observer for this setting
        */
        void RemoveNotifier( TSSSettingsSetting aSetting );


        /**
        * To get ALS controller.
        * @return The Als controller. NULL if not supported.
        */
        CSSSettingsAlsNotifier* GetAlsHandler();

        /**
        * Handle SIM refresh
        * @return Error code
        */
        TInt HandleRefresh();


    protected:  // Functions from base classes
        
        /**
        * From CActive. This is called when ALS Blocking status
        * is changed.
        */
        void RunL();

        /**
        * From CActive. ALS blocking change notification is cancelled.
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

    private:

        // Prohibit copy constructor if not deriving from CBase.
        CSSSettingsNotifier( const CSSSettingsNotifier& );
        // Prohibit assigment operator if not deriving from CBase.
        CSSSettingsNotifier& operator= ( const CSSSettingsNotifier& );
        
   private:
   
        // Determines if als supported.
        TBool AlsSupport( 
            TBool aPPSupport, 
            TBool aCSPSupport,
            TInt aCSPError );

    private:
        
        // Settings data
        RSSSettings&                     iSettings;

        // Als controller
        CSSSettingsAlsNotifier*          iAls;
        
        // valid in RunL
        RMmCustomAPI::TGetAlsBlockStatus iAlsBlockStatus;

        // Central repository for SSSettings.
        CRepository*                     iRepository;
        
        // Central repository notify handlers. 
        // Must use multiple handlers to observe all the keys:
        // KSettingsAlsLine
        // KSettingsCLIR
        // KSettingsCUG
        CCenRepNotifyHandler*            iCenRepAlsNotifyHandler;
        CCenRepNotifyHandler*            iCenRepClirNotifyHandler;
        CCenRepNotifyHandler*            iCenRepCugNotifyHandler;
     
        // The notified keys.
        TUint32                          iNotifyKeys;
    };

#endif      // CSSSETTINGSNOTIFIER_H
            
// End of File
