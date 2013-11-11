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
* Description: Declaration of CPhoneSettingsTestCallWaitingObserver class.
*
*/



#ifndef PHONESETTINGSTESTCALLWAITINGGOBSERVER_H
#define PHONESETTINGSTESTCALLWAITINGGOBSERVER_H

#include <e32base.h>
#include <mpsetcallwaitingobs.h>

// CLASS DECLARATION

NONSHARABLE_CLASS(CPhoneSettingsTestCallWaitingObserver) : 
    public MPsetCallWaitingObserver,
    public CBase
    {
    public:  // Constructors and destructor

        /**
        * Two-phased constructor.
        */
        static CPhoneSettingsTestCallWaitingObserver* NewL();

        /**
        * Destructor.
        */
        virtual ~CPhoneSettingsTestCallWaitingObserver();

    private:

        /**
        * C++ default constructor.
        */
        CPhoneSettingsTestCallWaitingObserver(); 

        /**
        * By default Symbian 2nd phase constructor is private.
        */
        void ConstructL();
        
    public: // Derived from MPsetCallWaitingObserver       
    
        /**
        * Handles notes when requesting Call Waiting status.
        *
        * @param aStatus Query result.
        * @param aBsc is List of active groups.
        */
        virtual void HandleCallWaitingGetStatusL( 
            const MPsetCallWaiting::TGetCallWaitingStatus aStatus, 
            TUint8 aBsc[KPSetNumberOfBsc] );
        
        /**
        * Handles notes when Call Waiting has been changed.
        *
        * @param aSetting Changed setting.
        * @param aResult Result of the action.
        */
        virtual void HandleCallWaitingChangedL( 
            const MPsetCallWaiting::TSetCallWaiting aSetting, 
            const TInt aResult );

        /**
        * Handles requesting notes.
        *
        * @param aStarted Is there a request going on.
        * @param aInterrupted Request needs to be immediately cancelled.
        */
        virtual void HandleCWRequestingL( TBool aOngoing, 
            TBool aInterrupted ); 
        /**
        * Handles errors in call waiting requests.
        *
        * @param aReason Reason for the error.
        */
        virtual void HandleCWErrorL( TInt aReason );

        /**
        * Sets pointer so that message is passed succesfully
        * from requester to observer.
        *
        * @param aEngineContact Source of events to observer.
        */
        virtual void SetEngineContact( MPsetCallWaiting& aEngineContact );        

    private: // Data
        
        // Pointer to MPsetCallWaiting
        MPsetCallWaiting* iEngineContact; 
    };

#endif      // PHONESETTINGSTESTCALLWAITINGGOBSERVER_H

// End of File