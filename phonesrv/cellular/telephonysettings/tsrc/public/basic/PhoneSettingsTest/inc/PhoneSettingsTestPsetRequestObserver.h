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
* Description: Declaration of CPhoneSettingsTestPsetRequestObserver class.
*
*/



#ifndef PHONESETTINGSTESTPSETREQUESTOBSERVER_H
#define PHONESETTINGSTESTPSETREQUESTOBSERVER_H

#include <e32base.h>
#include <mpsetrequestobs.h> 

// CLASS DECLARATION

NONSHARABLE_CLASS(CPhoneSettingsTestPsetRequestObserver) : 
    public CBase, 
    public MPsetRequestObserver 
    {

    public: 
        /**
        * Two-phased constructor.
        */
        static CPhoneSettingsTestPsetRequestObserver* NewL();

        /**
        * Destructor.
        */
        virtual ~CPhoneSettingsTestPsetRequestObserver();

    private:

        /**
        * C++ default constructor.
        */
        CPhoneSettingsTestPsetRequestObserver();

        /**
        * By default Symbian 2nd phase constructor is private.
        */
        void ConstructL();    
    
    public: // Functions 
    
        /**
        * Informs the receiver that request has been completed.
        */
        virtual void RequestComplete();

        /**
        * Informs the receiver that request state has changed.
        * @param new status, or when finished KErrNone or error code.
        */
        virtual void RequestStatusChanged( TInt aNewStatus );    
    };

#endif      // PHONESETTINGSTESTPSETREQUESTOBSERVER_H

// End of File