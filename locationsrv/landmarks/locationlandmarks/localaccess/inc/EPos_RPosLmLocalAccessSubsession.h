/*
* Copyright (c) 2002-2005 Nokia Corporation and/or its subsidiary(-ies). 
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
*  local access.
*
*
*/


#ifndef RPOSLMLOCALACCESSSUBSESSION_H
#define RPOSLMLOCALACCESSSUBSESSION_H

#include <e32base.h>
#include <EPos_Landmarks.h>
#include "epos_cposlmlocaldatabase.h"
#include "EPos_CPosLmLocalInitializeOp.h"
#include "epos_rposlmlocalnameindex.h"

class RPosLandmarkServer;

/**
*  This class is a subsession handle with operations related to
*  local access.
*
*  @lib eposlmlocalaccess.lib
*  @since S60 3.0
*/
class RPosLmLocalAccessSubsession : public RSubSessionBase
    {
    friend class RPosLmLocalNameIndex;
    public:

        /**
        * C++ default constructor.
        *
        */
        RPosLmLocalAccessSubsession();

        /**
        * Creates a local access subsession with the Landmarks Server.
        *
        * @param[in] aServer Is a connected session with the Landmarks Server.
        * @return @p KErrNone if successful, otherwise one of the system-wide
        * error codes.
        */
        TInt Open( RPosLandmarkServer& aServer );

        /**
        * Closes the subsession.
        */
        void Close();

        /**
        * Registers the session at the server by sending the database URI to it.
        * @param[in] aUri Database URI.
        */
        void RegisterSessionL( const TDesC& aUri );

        /**
        * Listens for a database event from the server.
        *
        * @param[out] aEvent Will contain the event information when an event
        *   occurs.
        * @param[out] aStatus Will be completed with @p KErrNone if an event
        *   occurs and an error code if some error was encountered.
        */
        void NotifyDatabaseEvent( TPosLmEvent& aEvent, TRequestStatus& aStatus );

        /**
        * Cancels a call to @ref NotifyDatabaseEvent.
        *
        * @return @p KErrNone if the request was successfully cancelled,
        *   otherwise a system wide error code.
        */
        TInt CancelNotifyDatabaseEvent();

        /**
        * Reports an event in the Landmarks Server.
        * @param[in] aEventType Type of event.
        * @param[in] aId Optional landmarks ID.
        * @return KErrNone if successful. Otherwise any system-wide error code.
        */
        TInt ReportEvent(
            TPosLmEventType aEventType,
            TPosLmItemId aId = KPosLmNullItemId );

        /**
        * Sets a lock on the database.
        *
        * @param aLockType The type of lock to set.
        * @return KErrNone if successful, otherwise KErrLocked or one of the
        *   other system wide error codes.
        */
        TInt SetDatabaseLock( CPosLmLocalDatabase::TLockType aLockType );

        /**
        * Releases a lock on the database.
        *
        * @param aLockType The type of lock to set.
        * @return KErrNone if successful, otherwise KErrLocked or one of the
        *   other system wide error codes.
        */
        TInt ReleaseDatabaseLock( CPosLmLocalDatabase::TLockType aLockType );

        /**
        * Sends a message to the Landmarks Server to create the default
        * database.
        *
        * @param[in] aUri Database URI.
        * @param[in] aReplaceDb Boolean whether database should be replaced or not.
        */
        void CreateDefaultDbL( const TDesC& aUri, TBool aReplaceDb );

        /**
        * Initializes the database synchronously.
        *
        * @param aInitType The type of initialization.
        * @return KErrNone if successful or one of the other system wide error
        *  codes.
        */
        TInt InitializeDbSync(
            CPosLmLocalInitializeOp::TInitializeType aInitType );

        /**
        * Initializes the database asynchronously.
        *
        * @param[in] aInitType The type of initialization.
        * @param[in,out] aStatusPckg The status of the initialization.
        * @param[in,out] aProgressPckg The progress of the initialization.
        * @param[in,out] aInitFlagPckg The init flag to update after the
        *   initialization.
        * @param[out] aStatus The request status of the initialization.
        */
        void InitializeDbAsync(
            CPosLmLocalInitializeOp::TInitializeType aInitType,
            TPckgBuf<TInt>* aStatusPckg,
            TPckg<TReal32>* aProgressPckg,
            TPckg<TUint>* aInitFlagPckg,
            TRequestStatus& aStatus );

        /**
        * Cancels a call to @ref InitializeDbAsync.
        *
        * @param aInitType The type of initialization.
        * @return @p KErrNone if the request was successfully cancelled,
        *   otherwise a system wide error code.
        */
        TInt CancelInitializeDbAsync(
            CPosLmLocalInitializeOp::TInitializeType aInitType );

    private:

        // By default, prohibit copy constructor
        RPosLmLocalAccessSubsession(
            const RPosLmLocalAccessSubsession&);
        // Prohibit assigment operator
        RPosLmLocalAccessSubsession& operator=(
            const RPosLmLocalAccessSubsession&);

    private:    // Data

        TPtr8 iEventPtr;

    };

#endif      // RPOSLMLOCALACCESSSUBSESSION_H
