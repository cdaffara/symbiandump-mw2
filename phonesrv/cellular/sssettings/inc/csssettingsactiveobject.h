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
* Description:  Interface for Active object handling Publish and subscribe 
*                notify events.
*
*/


#ifndef CSSSETTINGSACTIVEOBJECT_H
#define CSSSETTINGSACTIVEOBJECT_H

//  INCLUDES
#include <e32base.h>
#include <e32property.h>


// CLASS DECLARATION

/**
*  Interface for Publish and Subscribe notify.
*
*  @since   3.0
*  @lib     SsSettings.lib
*/
class MSSSettingsPubSubNotify
    {
    public: // New functions
    
        virtual void HandlePubSubNotify(
            const TUid aUid,
            const TUint32 aKeyId ) = 0;
    };

/**
* Interface for Active object handling Publish and subscribe notify events.
* @lib sssettings.lib
* @since 3.0
*/
NONSHARABLE_CLASS( CSSSettingsActiveObject )
    : public CActive
    {
    public:  // Constructors and destructor

        /**
        * Two-phased constructor.
        *
        * @param aNotifyUid The Uid to be notified.
        * @param aNotifyKey The key under aNotifyUid to be notified.
        * @param aNotifyHandler The notifier to be informed about notify events.
        * @return CSSSettingsActiveObject instance pointer.
        */
        static CSSSettingsActiveObject* NewL(
            const TUid aNotifyUid,
            const TUint32 aNotifyKey,
            MSSSettingsPubSubNotify& aNotifyHandler );


        /**
        * Destructor.
        */
        virtual ~CSSSettingsActiveObject();


    public: // New functions
        
        /**
        * Starts the notify.
        *
        * @return Error code.
        */
        TInt NotifyKeyChange();

        /**
        * Cancel notify
        */
        void CancelNotify();


    protected:  // Functions from base classes
        
        /**
        * From CActive.
        */
        void RunL();

        /**
        * From CActive.
        */
        void DoCancel();
        
        
    private:
    
        /**
        * Constructor.
        */
        CSSSettingsActiveObject(
            const TUid aNotifyUid,
            const TUint32 aNotifyKey,
            MSSSettingsPubSubNotify& aNotifyHandler );

        /**
        * By default Symbian OS constructor is private.
        */
        void ConstructL();


    private: // Data
    
        // Central Repository.
        RProperty                iProperty;
        
        // Uid to be notified.
        TUid                     iNotifyUid;
        
        // Key to be notified.
        TUint32                  iNotifyKey;
        
        // Notify handler.
        MSSSettingsPubSubNotify* iNotifyHandler;

        // The notify information.
        TBool                    iNotify;
    };

#endif // CSSSETTINGSACTIVEOBJECT_H
            
// End of File
