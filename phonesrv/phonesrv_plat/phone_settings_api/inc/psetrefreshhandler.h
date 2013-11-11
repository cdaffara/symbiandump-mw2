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
* Description:  The Refresh handler to inform refresses.
*
*/



#ifndef PSETREFRESHHANDLER_H
#define PSETREFRESHHANDLER_H


//  INCLUDES
#include <msssettingsrefreshobserver.h> 
#include <msatrefreshobserver.h> 


// DATA TYPES

// Changed CSP settings.
enum TPSetChangedCspSetting
    {
    EPSetNoChangedSetting = 0x0000, // No setting was changed in refresh.
    EPSetCW               = 0x0001, // Call Waiting setting was changed.
    EPSetCB               = 0x0002, // Call Barring was changed.
    EPSetCF               = 0x0004, // Call Forwarding setting was changed.
    EPSetALS              = 0x0008, // Alternate Line setting was changed.
    EPSetNetSelSup        = 0x0010, // Manual Network Selection setting 
                                    // was changed.
    };


// CLASS DECLARATION

/**
*  The Refresh handler to inform refresses.
*
*  @lib PhoneSettings.lib
*  @since 2.8
*/
class CPSetRefreshHandler :
    public CBase
    {
    public: // New functions

        /**
        * Start the waiting for refresh events.
        * 
        * @param aObserver The observer to handle refresh events.
        * @param aObservedFile The file change to be observed.
        * @param aObservedRefreshType The refresh type(s) to be observed.
        */
        virtual void NotifyFileChangeL(
            MSSSettingsRefreshObserver& aObserver,
            TSatElementaryFiles aObservedFile,
            TSatRefreshType aObservedRefreshType ) = 0;

        /**
        * Cancel the notification started in NotifyFileChangeL.
        * 
        * @param aObserver The observer whose notify to be canceled.
        * @param aObservedFile The file change to be canceled.
        * @param aObservedRefreshType The refresh type to be canceled.
        */
        virtual void CancelNotify() = 0;

        /**
        * Get the changed CSP information after Refresh.
        * 
        * @param aChangedCspSettings The changed CSP settings, 
        *                     TPSetChangedCspSetting values combined.
        * @param aNewValues The new values of changed CSP settings, 
        *                   TPSetChangedCspSetting values combined.
        * @return Error code. KErrNone if successful.
        */
        virtual TInt ChangedCspSettings( 
            TPSetChangedCspSetting& aChangedCspSettings,
            TPSetChangedCspSetting& aNewValues ) = 0;
    };

#endif      // PSETREFRESHHANDLER_H 
            
// End of File
