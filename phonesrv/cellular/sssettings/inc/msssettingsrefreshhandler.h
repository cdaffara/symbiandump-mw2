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



#ifndef MSSSETTINGSREFRESHHANDLER_H
#define MSSSETTINGSREFRESHHANDLER_H


//  INCLUDES
#include <msssettingsrefreshobserver.h> 
#include <msatrefreshobserver.h>


// FORWARD DECLARATIONS
class CSSSettingsNotifyContainer;


// CLASS DECLARATION

/**
*  The Refresh handler to inform SAT refresses.
*
*  @lib SsSettings.lib
*  @since 2.8
*/
class MSSSettingsRefreshHandler
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
        * @param aChangedCsps The changed CSP fields, 
        *                     type RMobilePhone::TMobilePhoneCspFileV1Pckg.
        * @param aNewValues The new values of changed CSP fields, 
        *                   type RMobilePhone::TMobilePhoneCspFileV1Pckg.
        * @return Error code. KErrNone if successful.
        */
        virtual TInt ChangedCspTable( 
            TDes8& aChangedCsps,
            TDes8& aNewValues ) = 0;
    };

#endif      // MSSSETTINGSREFRESHHANDLER_H  
            
// End of File
