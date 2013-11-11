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
* Description: Declaration of CPhoneSettingsTestRefreshHandler class.
*
*/



#ifndef PHONESETTINGSTESTREFRESHHANDLER_H
#define PHONESETTINGSTESTREFRESHHANDLER_H

#include <e32base.h>
#include <psetrefreshhandler.h>
#include "PhoneSettingsTestParser.h"

// CLASS DECLARATION

NONSHARABLE_CLASS(CPhoneSettingsTestRefreshHandler) :     
    public CPSetRefreshHandler    
    {
    public:  // Constructors and destructor

        /**
        * Two-phased constructor.
        */
        static CPhoneSettingsTestRefreshHandler* NewL()
            {
            CPhoneSettingsTestRefreshHandler* self = 
                new (ELeave) CPhoneSettingsTestRefreshHandler();     
            CleanupStack::PushL( self );
            self->ConstructL();
            CleanupStack::Pop();
            return self;
            }

        /**
        * Destructor.
        */
        virtual ~CPhoneSettingsTestRefreshHandler(){}

    private:

        /**
        * C++ default constructor.
        */
        CPhoneSettingsTestRefreshHandler(){}
        
        /**
        * By default Symbian 2nd phase constructor is private.
        */
        void ConstructL(){}

        /**
		* Start the waiting for refresh events.
        * 
        * @param aObserver The observer to handle refresh events.
        * @param aObservedFile The file change to be observed.
        * @param aObservedRefreshType The refresh type(s) to be observed.
		*/
		virtual void NotifyFileChangeL(
            MSSSettingsRefreshObserver& /*aObserver*/,
            TSatElementaryFiles /*aObservedFile*/,
            TSatRefreshType /*aObservedRefreshType*/ ){}

		/**
		* Cancel the notification started in NotifyFileChangeL.
        * 
        * @param aObserver The observer whose notify to be canceled.
        * @param aObservedFile The file change to be canceled.
        * @param aObservedRefreshType The refresh type to be canceled.
		*/
		virtual void CancelNotify(){}

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
            TPSetChangedCspSetting& /*aChangedCspSettings*/,
            TPSetChangedCspSetting& /*aNewValues*/ ){return KErrNone;}
    };


#endif      // PHONESETTINGSTESTREFRESHHANDLER_H

// End of File