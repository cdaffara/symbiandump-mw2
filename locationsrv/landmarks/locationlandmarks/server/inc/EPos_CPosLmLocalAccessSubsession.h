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
* Description: subsession class for operations related to local access.
*
*
*/


#ifndef CPOSLMLOCALACCESSSUBSESSION_H
#define CPOSLMLOCALACCESSSUBSESSION_H

#include <EPos_Landmarks.h>
#include "EPos_CPosLmSubsessionBase.h"
#include "EPos_PosLmServerCommon.h"
#include "EPos_CPosLmNameIndex.h"

class CPosLmEventHandler;
class CPosLmLocalDbAccess;
class CPosLmActiveCompacter;
class CPosLmNameIndex;


/**
* This is a subsession class for operations related to local access.
*/
class CPosLmLocalAccessSubsession : public CPosLmSubsessionBase
    {

    public: // Constructors and destructor

        /**
        * Two-phased constructor.
        *
        * @param[in] aLmServer Reference to the landmark server.
        */
        static CPosLmLocalAccessSubsession* NewLC( CPosLmServer* aServer );

        ~CPosLmLocalAccessSubsession();

    public: 

        /**
        * Checks if the db of this subsession matches the specified URI and if
        * so if the db has a write lock.
        *
        * @param[in] aUri The URI to compare with.
        * @return ETrue if the URI matches and this subsession has a write
        *   lock, otherwise EFalse.
        */
        TBool HasWriteLock( const TDesC& aUri );

        /**
        * Checks if the db of this subsession matches the specified URI and if
        * so if the db has any lock.
        *
        * @param[in] aUri The URI to compare with.
        * @return ETrue if the URI matches and this subsession has any lock,
        *   otherwise EFalse.
        */
        TBool HasAnyLock( const TDesC& aUri );

        /**
        * Handles an incoming operation event from the server. If the URI
        * matches the URI associated with this subsession, the event is
        * forwarded to the client, if it is listening for events.
        *
        * @param[in] aEvent The event.
        * @param[in] aUri A URI associated with the database from which the event
        *   was generated.
        * @param[in] aCheckUri ETrue if the URI should be checked. EFalse otherwise.
        */
        void HandleOperationEventL(
            TPosLmEvent aEvent,
            const TDesC& aUri,
            TBool aCheckUri );

        /**
        * Completes outstanding events in the event handler.
        *
        * @param[in] aError An error code to complete the request with.
        */
        void CompleteOutstandingEventRequest( TInt aError );

        /** Releases the write lock for an internal compact. */
        void ReleaseCompactLock();

    public: // Functions from base classes

        /** Handles the servicing of client requests to the server.
        * @param[in] aMessage The message containing the client request.
        */
        void ServiceL( const RMessage2& aMessage );

    private:

        /**
        * C++ default constructor.
        *
        * @param[in] aLmServer Reference to the landmark server.
        */
        CPosLmLocalAccessSubsession( CPosLmServer* aServer );

        void ConstructL();

        /**
        * Reads and stores the database URI sent from the client. The URI
        * shows which database the client is connected to. The message
        * is completed with KErrNone within this function.
        *
        * @param[in] aMessage Message containing the client request.
        */
        void RegisterUriL( const RMessage2& aMessage );

        void DatabaseLockL( const RMessage2& aMessage );

        void ForwardEventToAllSessionsL( const RMessage2& aMessage );

        void CreateDefaultDatabaseL( const RMessage2& aMessage );

        void HandleReadSortedLandmarksL( const RMessage2& aMessage );
        void GetSortedLandmarksL( 
            CPosLmNameIndex& aNameIndex,
            RWriteStream& aOut,
            TPosLmServerReadArrayParam& aParam,
            TInt aMaxSize );
        void GetSortedLandmarksReverseL( 
            CPosLmNameIndex& aNameIndex,
            RWriteStream& aOut,
            TPosLmServerReadArrayParam& aParam,
            TInt aMaxSize );
        TBool ExportIndexItemL( 
            const CPosLmNameIndex::CItem& aItem,
            RWriteStream& aOut,
            TInt& aLeftInBuffer,
            TPosLmServerReadArrayParam& aParam );
        
        void HandleReadSortedIdsL( const RMessage2& aMessage );
        
        void HandleUpdateLandmarkIndexL( const RMessage2& aMessage );

        void HandleLandmarksAddedOrUpdatedL( const RMessage2& aMessage );
        
        void HandleLandmarksRemovedL( const RMessage2& aMessage );

        void CheckDiskSizeL( const TDesC& aUri );

        void CompactIfNeededL();
        
        void HandleDataTransactionL( const RMessage2& aMessage );
        
        void HandleNameIndexStatusL( const RMessage2& aMessage );
        
        void HandleUseNameIndexL( const RMessage2& aMessage );
        
        /** Leaves, if index for this database is not loaded.
         *  To ensure that index is loaded, client has to register
         *  with EPosLmServerOpenNameIndex */
        void AssertIndexLoadedL();

    private:    // Data

        // Database URI associated with the client database.
        HBufC* iDbUri;

        CPosLmLocalDbAccess* iDbAccess;

        CPosLmEventHandler* iEventHandler;

        TBool iHasWriteLock;

        TInt iNoOfReadLocks;

        CPosLmActiveCompacter* iCompacter;
        
        TBool iUsingNameIndex;
    };

#endif //CPOSLMLOCALACCESSSUBSESSION_H

