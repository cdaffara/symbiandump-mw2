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
* Description: Declaration of CPhoneSettingsTestCallDivertingObserver class.
*
*/



#ifndef PHONESETTINGSTESTCALLDIVERTINGGOBSERVER_H
#define PHONESETTINGSTESTCALLDIVERTINGGOBSERVER_H

#include <e32base.h>
#include <mpsetdivertobs.h>

// CLASS DECLARATION

NONSHARABLE_CLASS(CPhoneSettingsTestCallDivertingObserver) : 
    public MPsetDivertObserver,
    public CBase
    {
    public:  // Constructors and destructor

        /**
        * Two-phased constructor.
        */
        static CPhoneSettingsTestCallDivertingObserver* NewL();

        /**
        * Destructor.
        */
        virtual ~CPhoneSettingsTestCallDivertingObserver();

    private:

        /**
        * C++ default constructor.
        */
        CPhoneSettingsTestCallDivertingObserver(); 

        /**
        * By default Symbian 2nd phase constructor is private.
        */
        void ConstructL();
        
    public: // Derived from MPsetDivertObserver       
    
        /**
        * Handles showing notes, when diverts change.
        *
        * @param aSetting Setting which was changed.
        * @param aPlural Plural notes to be used.
        */
        virtual void HandleDivertingChangedL( 
            const TCallDivertSetting& aSetting, TBool aPlural );
 
        /**
        * Handles showing of status inquiry notes.
        *
        * @param aBsc List of basic services.
        * @param aSetting Divert setting which was queried.
        * @param aPlural Plural notes to be used.
        */

        virtual void HandleDivertingStatusL( CMobilePhoneCFList& aSetting, 
            TBool aPlural );

        /**
        * Handles showing of error notes.
        *
        * @param aReason Reason for diverting error.
        */
        virtual void HandleDivertingErrorL( TInt aReason );
        
        /**
        * Handles requesting notes.
        *
        * @param aOngoing Is there a request going on.
        * @param aInterrupted Request needs to immediately cancelled.
        */
        virtual void HandleCFRequestingL( TBool aOngoing, 
            TBool aInterrupted );

        /**
        * Sets pointer so that message is passed succesfully
        * from requester to observer.
        *
        * @param aDivertEngine Source for the events to observer.
        */
        virtual void SetEngineContact( MPsetCallDiverting* aDivertEngine );  

    private: // Data
        
        // Pointer to Diverting Engine 
        MPsetCallDiverting* iDivertEngine; 
    };

#endif      // PHONESETTINGSTESTCALLDIVERTINGGOBSERVER_H

// End of File