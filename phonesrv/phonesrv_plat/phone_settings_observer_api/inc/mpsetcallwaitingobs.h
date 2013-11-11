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
* Description: 
*       MPsetCallWaitingObserver abstract call waiting observer class.
*
*
*/


#ifndef     MPSETCALLWAITINGOBS_H
#define     MPSETCALLWAITINGOBS_H

// INCLUDES
#include <mpsetcallwaiting.h> 

//  CLASS DEFINITIONS 
class   MPsetCallWaitingObserver
    {
    public:

        /**
        * Handles notes when requesting Call Waiting status.
        *
        * @param aStatus Query result.
        * @param aBsc is List of active groups.
        */
        virtual void HandleCallWaitingGetStatusL( 
            const MPsetCallWaiting::TGetCallWaitingStatus aStatus, 
            TUint8 aBsc[KPSetNumberOfBsc] ) = 0;
        
        /**
        * Handles notes when Call Waiting has been changed.
        *
        * @param aSetting Changed setting.
        * @param aResult Result of the action.
        */
        virtual void HandleCallWaitingChangedL( 
            const MPsetCallWaiting::TSetCallWaiting aSetting, 
            const TInt aResult ) = 0;

        /**
        * Handles requesting notes.
        *
        * @param aStarted Is there a request going on.
        * @param aInterrupted Request needs to be immediately cancelled.
        */
        virtual void HandleCWRequestingL( TBool aOngoing, 
            TBool aInterrupted ) = 0; 
        /**
        * Handles errors in call waiting requests.
        *
        * @param aReason Reason for the error.
        */
        virtual void HandleCWErrorL( TInt aReason ) = 0;

        /**
        * Sets pointer so that message is passed succesfully
        * from requester to observer.
        *
        * @param aEngineContact Source of events to observer.
        */
        virtual void SetEngineContact( MPsetCallWaiting& aEngineContact ) = 0;

    };

#endif // MPSETCALLWAITINGOBS_H

// end of file
