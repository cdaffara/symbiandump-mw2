/*
* Copyright (c) 2006 Nokia Corporation and/or its subsidiary(-ies).
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
* Description: AlwaysOnline server central repository controller
*
*/


#ifndef CAOCENREPCONTROL_H
#define CAOCENREPCONTROL_H

// INCLUDES
#include <e32base.h>

#include "AlwaysOnlineManager.h"

// CONSTANTS
// MACROS
// DATA TYPES
// FUNCTION PROTOTYPES
// FORWARD DECLARATIONS
// CLASS DECLARATION


/**
* Utility class for always online to easily handle central repository.
*
* @since S60 3.1
*/
class CAOCenRepControl : public CBase
    {
    public:  // Constructors and destructor

        /**
        * Creates object from CAOCenRepControl and leaves it to cleanup stack
        * @since S60 3.1
        * @return, Constructed object
        */
        static CAOCenRepControl* NewLC( CAlwaysOnlineManager& aCallBack );

        /**
        * Create object from CAOCenRepControl
        * @since S60 3.1
        * @return, Constructed object
        */
        static CAOCenRepControl* NewL( CAlwaysOnlineManager& aCallBack );

        /**
        * Destructor
        * @since S60 3.1
        */
        virtual ~CAOCenRepControl();

    public: // New functions

        /**
        * Recreates CoreAppUi CenRep session and start notifiers.
        * @since S60 3.0
        */
        void ReCreateCoreAppUiCenRepSessionL();
            
        /**
        * Sets cenrep key.
        * @since S60 3.1
        * @param aRepository, cenrep repository
        * @param aKeyId, key to set
        * @param aValue, value
        * @return TInt, result
        */
        TInt SetCenRepKey( 
            const TUid& aRepository, 
            const TUint32 aKeyId, 
            TInt aValue );
        
        /**
        * Gets cenrep key.
        * @since S60 3.1
        * @param aRepository, cenrep repository
        * @param aKeyId key to get
        * @param aValue, value to return
        * @return TInt, result
        */    
        TInt GetCenRepKey( 
            const TUid& aRepository, 
            const TUint32 aKeyId, 
            TInt& aValue );

        /**
        * Deletes cenrep key.
        * @since S60 3.1
        * @param aRepository, cenrep repository
        * @param aKeyId, key to get
        * @return TInt, result
        */   
        TInt DelCenRepKey(
            const TUid& aRepository,
            const TUint32 aKeyId );

        /**
        * Update the array, which contains disabled plugin's UIDs
        * @param aDisabledPluginUidsArray, UIDs array
        * @since S60 3.1
        */
        void UpdateDisabledPluginsUids( 
            CArrayFixFlat<TUid>& aDisabledPluginUidsArray );

        /**
        * Adds or removes plugin's UID from cenrep and updates
        * the total count of disabled plugins.
        * @since S60 3.1
        * @param const aPlugin, UID to add/remove
        * @param aDisable, add or remove?
        */
        void SetPluginStatus( const TUid& aPlugin, TBool aDisable );
        
        /**
        * Gets first free key from cenrep.
        * @since S60 3.1
        * @param aFreeKey, free cenrep key
        * @return TBool, did it found?
        */
        TBool GetFirstFreeKey( TUint32& aFreeKey );
        
        /**
        * Gets the key, which contains certain UID.
        * @since S60 3.1
        * @param aValue, value to search
        * @param aKey, free cenrep key
        * @return TBool, did it found?
        */
        TBool GetKeyWithValue( const TUid& aValue, TUint32& aKey );
        
        /**
        * @since S60 3.1
        * @param aIncrease, increase or decrease?
        * @return TInt, result
        */
        TInt UpdateDisabledPluginsCount( TBool aIncrease );

    public: // Functions from base classes

    protected:  // Constructors

        /**
        * Default constructor for classCAOCenRepControl
        * @since S60 3.1
        * @return, Constructed object
        */
        CAOCenRepControl( CAlwaysOnlineManager& aCallBack );

        /**
        * Symbian 2-phase constructor
        * @since S60 3.1
        */
        void ConstructL();

    private:  // New functions

        /**
        * Creates CoreAppUi CenRep session and start notifiers.
        * @since S60 3.0
        */
        void CreateCoreAppUiCenRepSessionL();
        
        /**
        * Creates AoServer CenRep session.
        * @since S60 3.1
        */
        void CreateAoServerCenRepSessionL();

    private:  // Functions from base classes

    private:    // Data
        // Call back class for iCoreAppUiCenRepNotifier
        CAlwaysOnlineManager&           iCallBack;
        // Central Repository session for KCRUidCoreApplicationUIs
        CRepository*                    iCoreAppUiCenRepSession;
        // Central Repository session for KCRUidAOSettings
        CRepository*                    iAoServerCenRepSession;
        // Notifier to listen changes of offline state
        CCenRepNotifyHandler*           iCoreAppUiCenRepNotifier;
    };

#endif //  CAOCENREPCONTROL_H

// End of File
