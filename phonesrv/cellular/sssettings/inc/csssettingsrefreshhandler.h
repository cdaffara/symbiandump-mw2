/*
* Copyright (c) 2004 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:  The Refresh handler to inform SAT refresses.
*
*/



#ifndef CSSSETTINGSREFRESHHANDLER_H
#define CSSSETTINGSREFRESHHANDLER_H


//  INCLUDES
#include <msssettingsrefreshhandler.h> 
#include <msatrefreshobserver.h>
#include <msssettingsrefreshobserver.h> 
#include <rsssettings.h> 
#include <rcustomerserviceprofilecache.h> 
#include <rsatrefresh.h>
#include <rsatsession.h> 
#include <cenrepnotifyhandler.h>
#include "csssettingsactiveobject.h" 

// DATA TYPES

// The needed functionality.
enum TSSSettingsRefreshFunction
    {
    ESSSettingsAllowRefresh = 0, // AllowRefresh functionality.
    ESSSettingsRefresh      = 1, // Refresh functionality.
    };


// FORWARD DECLARATIONS
class CSSSettingsNotifyInfo;


// CLASS DECLARATION

/**
*  The Refresh handler to inform SAT refreshes.
*  This class is extended for BT SAP functionality from 3.0.
*
*  @lib SsSettings.lib
*  @since 2.8
*/
class CSSSettingsRefreshHandler :
    public CBase,
    public MSSSettingsRefreshHandler,
    public MSatRefreshObserver,
    public MSSSettingsPubSubNotify
    {
    public:  // Constructors and destructor
        
        /**
        * Two-phased constructor.
        */
        static CSSSettingsRefreshHandler* NewL();
        
        /**
        * Destructor.
        */
        virtual ~CSSSettingsRefreshHandler();


    public: // Functions from base classes

        /**
        * @see MSSSettingsRefreshHandler::NotifyFileChangeL.
        */
        void NotifyFileChangeL(
            MSSSettingsRefreshObserver& aObserver,
            TSatElementaryFiles aObservedFiles,
            TSatRefreshType aObservedRefreshType );

        /**
        * @see MSSSettingsRefreshHandler::CancelNotify.
        */
        void CancelNotify();

        /**
        * @see MSSSettingsRefreshHandler::ChangedCspTable.
        */
        TInt ChangedCspTable( 
            TDes8& aChangedCsps,
            TDes8& aNewValues );


    public: // Functions from base classes

        /**
        * @see MSatRefreshObserver::AllowRefresh
        */
        TBool AllowRefresh(
            TSatRefreshType aType,
            const TSatRefreshFiles& aFiles );

        /**
        * @see MSatRefreshObserver::Refresh
        */
        void Refresh(
            TSatRefreshType aType,
            const TSatRefreshFiles& aFiles );
            
    public: // From base class MSSSettingsPubSubNotify
        /**
        * @see MSSSettingsPubSubNotify
        */
        void HandlePubSubNotify(
            const TUid aUid,
            const TUint32 aKeyId );


    private:

        /**
        * C++ default constructor.
        */
        CSSSettingsRefreshHandler();

        /**
        * By default Symbian 2nd phase constructor is private.
        */
        void ConstructL();

        /**
        * Common function for AllowRefresh and Refresh.
        * 
        * @param atype Refresh Type.
        * @param aFiles Refresh Files.
        * @param aFunctionality The functionality wanted.
        * @return Result of functionality.
        */
        TBool DoHandleRefresh(
            TSatRefreshType aType,
            const TSatRefreshFiles& aFiles,
            const TSSSettingsRefreshFunction aFunctionality );

    private:    // Data
         
        // Container for notify observer.
        CSSSettingsNotifyInfo*        iNotifyInfo;

        // The RSSSettings handle.
        RSSSettings                   iRSSSettings;

        // The RCustomerServiceProfileCache handle.
        RCustomerServiceProfileCache  iRCspCache;

        // The RSatRefresh handle.
        RSatRefresh*                  iRSatRefresh;

        // The RSatSession handle.
        RSatSession                   iRSatSession;
        
        // Notifier for BT SAP support.
        CSSSettingsActiveObject*      iBtSapNotifier;

    };

#endif      // CSSSETTINGSREFRESHHANDLER_H  
            
// End of File
