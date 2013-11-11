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
* Description: Declaration of CPhoneSettingsTestCallBarringObserver class.
*
*/



#ifndef PHONESETTINGSTESTCALLBARRINGOBSERVER_H
#define PHONESETTINGSTESTCALLBARRINGOBSERVER_H

#include <e32base.h>

// CLASS DECLARATION

NONSHARABLE_CLASS(CPhoneSettingsTestCallBarringObserver) : 
    public MPsetBarringObserver,
    public CBase
    {

    public:  // Constructors and destructor

        /**
        * Two-phased constructor.
        */
        static CPhoneSettingsTestCallBarringObserver* NewL();

        /**
        * Destructor.
        */
        virtual ~CPhoneSettingsTestCallBarringObserver();

    private:

        /**
        * C++ default constructor.
        */
        CPhoneSettingsTestCallBarringObserver(); 

        /**
        * By default Symbian 2nd phase constructor is private.
        */
        void ConstructL();
    
    public: // Derived from         
    
        /**
        * Handles mode changed notes.
        *
        * @param aType Type of barring program.
        * @param aStatus Status of the barring program.
        * @param aPlural Plural notes should be used.
        */
        virtual void HandleBarringModeChangedL( TBarringProgram aType, 
              TBarringStatus aStatus, TBool aPlural );
        
         /**
        * Handles barring mode inquiry notes.
        *
        * @param aType Type of barring program.
        * @param aStatus Status of the barring program.
        */
        virtual void HandleBarringModeStatusL( TUint8 aBsc[KPSetNumberOfBsc], 
                    TBarringStatus aStatus );
        
        /**
        * Handles errors in call barring requests.
        *
        * @param aReason Reason for error.
        */
        virtual void HandleBarringErrorL( TInt aReason);

        /**
        * Handles requesting notes.
        *
        * @param aTrue Is there a request going on.
        * @param aInterrupted Request needs to be immediately cancelled.
        */
        virtual void HandleCBRequestingL( TBool aTrue, TBool aInterrupted );
        
        /**
        * Sets pointer so that message is passed succesfully
        * from requester to observer.
        *
        * @param aBarringEngine Source of events to observer.
        */
        virtual void SetEngineContact( MPsetCallBarring* aBarringEngine );

        /**
        * Handles notes relating to cb password change requests.
        *
        * @param aSuccess Result of teh change request.
        */
        virtual void CbPasswordChangedL( TBool aSuccess );    

    private: // Data
        
        // Pointer to Barring Engine 
        MPsetCallBarring* iBarringEngine; 
    };

#endif      // PHONESETTINGSTESTCALLBARRINGOBSERVER_H

// End of File