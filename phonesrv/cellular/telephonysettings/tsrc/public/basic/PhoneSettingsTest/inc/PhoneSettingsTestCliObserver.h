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
* Description: Declaration of CPhoneSettingsTestCliObserver class.
*
*/



#ifndef PHONESETTINGSTESTCLIOBSERVER_H
#define PHONESETTINGSTESTCLIOBSERVER_H

#include <e32base.h>
#include <mpsetcliobserver.h>

// CLASS DECLARATION

NONSHARABLE_CLASS(CPhoneSettingsTestCliObserver) : 
    public MPsetCliObserver,
    public CBase
    {
    public:  // Constructors and destructor

        /**
        * Two-phased constructor.
        */
        static CPhoneSettingsTestCliObserver* NewL();

        /**
        * Destructor.
        */
        virtual ~CPhoneSettingsTestCliObserver();

    private:

        /**
        * C++ default constructor.
        */
        CPhoneSettingsTestCliObserver(); 

        /**
        * By default Symbian 2nd phase constructor is private.
        */
        void ConstructL();    

    public: // Derived from MPsetCliObserver

        /**
        * Sets pointer so that message is passed succesfully
        * from requester to observer.
        *
        * @param aEngine Source of events to observer.
        */
        virtual void SetEngineContact( MPsetCli* aEngine );

        /**
        * Handles requesting notes.
        *
        * @param aOngoing Is request already going on.
        * @param aInterrupted Request needs to be immediately cancelled.
        */
        virtual void HandleCliRequestingL( TBool aOngoing, 
            TBool aInterrupted ); 

        /**
        * Handles notes for changing any of CLI features.
        *
        * @param aType Type of the CLI affected.
        */
        virtual void CliInformationL( TPsuiCli aType );

        /**
        * Checks the CLI status from network.
        *
        * @param aBsc List of basic service groups.
        * @param aMode Which CLI feature was inquiried for.
        */
        virtual void HandleCliStatusL( TUint8 aBsc[KPSetNumberOfBsc], TPsuiCli aMode );

        /**
        * Checks the CNAP status from network.
        * @since 1.2
        *
        * @param aStatus CNAP status.
        */        
        virtual void HandleCnapStatusL( TInt aStatus );

        /**
        * Handles errors.
        * @since 2.0
        *
        * @param aError error code.
        */
        virtual void HandleCliErrorL( TInt aError );

    private: // Data
        
        // Cli Engine contact 
        MPsetCli* iEngine; 
    };

#endif      // PHONESETTINGSTESTCLIOBSERVER_H

// End of File