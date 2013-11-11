/*
* Copyright (c) 2005 Nokia Corporation and/or its subsidiary(-ies).
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
* Description:   Connection manager bearer callback
*
*/




#ifndef __MSGBEAREROBSCALLBACK__
#define __MSGBEAREROBSCALLBACK__

#include <e32base.h>

//This must be kept as low as possible
const TInt KMaxNumberOfSubscribers         = 3;

//Possible events
enum TMsgBearerEvent
    {
    EMsgBearerActive = 0,
    EMsgBearerSuspended,
    EMsgBearerLost
    };

// CLASS DECLARATION

class MMsgBearerObsCallback
    {
    public:

        /**
        * A connection manager object uses this callback function to indicate
        * that a change in the status of the bearer has occurred. It is important
        * to note that a call to the notification handler (HandleBearerEventL())
        * takes place inside the RunL() method of a Connection Manager, so the
        * listening object MUST return the control to the Manager AS SOON AS
        * POSSIBLE in order not to clog the scheduler. 
        */
        virtual void HandleBearerEventL( TBool aAuthoritativeClose,
                                         TMsgBearerEvent aBearerEvent ) = 0;
    };

#endif    //__MSGBEAREROBSCALLBACK__
            
// End of File
