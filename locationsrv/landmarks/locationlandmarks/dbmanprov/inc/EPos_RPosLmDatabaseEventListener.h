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
* Description: This class is a subsession handle with operations related to
*  database management event listening.
*
*
*/


#ifndef RPOSLMDATABASEEVENTLISTENER_H
#define RPOSLMDATABASEEVENTLISTENER_H

//  INCLUDES
#include <e32base.h>
#include <epos_rposlandmarkserver.h>
#include <EPos_TPosLmDatabaseEvent.h>


// FORWARD DECLARATIONS

// CLASS DECLARATION

/**
*  This class is a subsession handle with operations related to
*  database management event listening.
*
*  @lib eposlmdbmanprov.lib
*  @since S60 3.0
*  @version $Revision: 1.4 $, $Date: 2005/07/07 13:40:31 $
*/
class RPosLmDatabaseEventListener : public RSubSessionBase
    {
    public:

        /**
        * C++ default constructor.
        *
        */

        RPosLmDatabaseEventListener();

        /**
        * Creates an event-listening subsession with the landmarks server.
        *
        * @param aServer Is a connected session with the landmarks server.
        * @return @p KErrNone if successful, otherwise one of the system-wide
        * error codes.
        */

        TInt Open(
        /* IN  */ RPosLandmarkServer& aServer
        );

        /**
        * Closes the subsession.
        *
        */
        void Close();

        /**
        * Listens for database management events.
        *
        * @param aEvent Contains the event information when an event
        *   occurs.
        * @param aStatus Is completed with @p KErrNone if an event
        *   occurs or an error code if some error is encountered.
        */
        void NotifyDatabaseEvent(
        /* OUT */   TPosLmDatabaseEvent& aEvent,
        /* OUT */   TRequestStatus& aStatus
        );

        /**
        * Cancels a call to @ref NotifyDatabaseEvent.
        *
        * @return @p KErrNone if the request was successfully cancelled,
        *   otherwise a system wide error code.
        */
        TInt CancelNotifyDatabaseEvent();

        /**
        * Retrieves the database URI associated with the last event.
        *
        * @return The database URI associated with the event. The client takes
        *   ownership of the descriptor object.
        * @par Leave codes:
        * @p KErrNotFound There is no database URI associated with the event
        * or, the database URI has been discarded because the client has called
        * @ref NotifyDatabaseEvent again.
        */
        HBufC* DatabaseUriFromLastEventLC();


    private:

        // By default, prohibit copy constructor
        RPosLmDatabaseEventListener(
            const RPosLmDatabaseEventListener&);
        // Prohibit assigment operator
        RPosLmDatabaseEventListener& operator= (
            const RPosLmDatabaseEventListener&);

    private:    // Data

        TPtr8       iEventPtr;
        TInt        iUriLength;
        TPckg<TInt> iUriLengthPckg;
    };

#endif      // RPOSLMDATABASEEVENTLISTENER_H

// End of File
