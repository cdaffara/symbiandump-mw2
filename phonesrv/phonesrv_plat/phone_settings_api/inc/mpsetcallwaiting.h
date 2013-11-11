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
*         MPsetCallWaiting defines interface to get and set call waiting setting
*         in the network. All the methods are asynchronous. Observer handles
*         completion.
*
*
*/


#ifndef     MPSETCALLWAITING_H
#define     MPSETCALLWAITING_H

//  INCLUDES
#include <e32base.h>
#include <nwdefs.h>

//  FORWARD DECLARATIONS
class MPsetCallWaitingObserver;

//  CLASS DEFINITIONS 
/**
*  MPsetCallWaiting class is virtual class for Call Waiting.
*  @lib phonesettings.lib
*  @since 1.0
*/
class   MPsetCallWaiting
    {
    public:

        // Call waiting action
        enum TSetCallWaiting
            {
            EActivateCallWaiting,
            EDeactivateCallWaiting
            };

        // Call waiting status
        enum TGetCallWaitingStatus
            {
            EStatusActive,
            EStatusDisabled,            
            EStatusNotAvailable,
            EStatusNotProvisioned,
            EStatusUnknown
            };

    public:         

        /**
        * Sets call waiting to the network.
        *
        * @param aSetting New settings for the Call Waiting.
        * @param aBsc Basic service group concerned.
        */
        virtual void SetCallWaitingL( 
            MPsetCallWaiting::TSetCallWaiting aSetting, 
            TBasicServiceGroups aBsc ) = 0;

        /**
        * Checks the call waiting status from network.
        */
        virtual void GetCallWaitingStatusL()=0;

        /**
        * Cancels the call waiting-request process.
        */
        virtual TInt CancelProcess()=0;
    };

#endif // MPSETCALLWAITING_H
// end of file
