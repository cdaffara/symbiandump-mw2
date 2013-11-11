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
* Description: This class is used to manage landmark databases.
*
*
*/


#ifndef CPOSLMDATABASEMANAGERIMPL_H
#define CPOSLMDATABASEMANAGERIMPL_H

//  INCLUDES
#include <e32base.h>
#include <badesca.h>
#include <EPos_CPosLmDatabaseManager.h>
#include <EPos_TPosLmDatabaseEvent.h>
#include <EPos_TPosLmDatabaseSettings.h>
#include <EPos_HPosLmDatabaseInfo.h>
#include <epos_rposlandmarkserver.h>
#include "EPos_RPosLmDatabaseEventListener.h"

// FORWARD DECLARATIONS
class CPosLmSettingsHandler;
class CPosLmDbManPluginStore;
class CPosLmDatabaseManagerPluginBase;

// CLASS DECLARATION

/**
*  This class is used to manage landmark databases. It implements the common
*  database management methods in @refCPosLmDatabaseManager and loads
*  protocol-specific database management provider implementations.
*
*  @p CPosLmDatabaseManagerImpl contains functions for listing registering,
*  unregistering, creating, deleting, copying landmark databases, etc.
*  It also has functions for managing the default landmark database. The
*  client can listen to events related to database management.
*
*  @lib eposlmdbmanprov.lib
*  @since S60 3.0
*  @version $Revision: 1.5 $, $Date: 2005/07/07 13:40:31 $
*/
class CPosLmDatabaseManagerImpl : public CPosLmDatabaseManager
    {
    public:

        /**
        * Two-phased constructor.
        *
        * @since S60 3.0
        */
        static CPosLmDatabaseManagerImpl* NewL();

        /**
        * Destructor.
        *
        * @since S60 3.0
        */
        virtual ~CPosLmDatabaseManagerImpl();

    public: // Functions from base classes

        /**
        * From CPosLmDatabaseManager.
        *
        * @return The URI of the default landmark database. The client takes
        *   ownership of the descriptor object.
        */
        HBufC* DefaultDatabaseUriLC();

        /**
        * From CPosLmDatabaseManager.
        *
        * @param aDatabaseUri The URI of the database which should be set as
        *   default.
        */
        void SetDefaultDatabaseUriL(
        /* IN  */ const TDesC& aDatabaseUri
        );

        /**
        * From CPosLmDatabaseManager.
        *
        * @param aEvent Contains the event information when an event
        *   occurs.
        * @param aStatus Is completed with @p KErrNone if an event
        *   occurs or an error code if some error is encountered.
        */
        void NotifyDatabaseEvent(
        /* OUT */ TPosLmDatabaseEvent& aEvent,
        /* OUT */ TRequestStatus& aStatus
        );

        /**
        * From CPosLmDatabaseManager.
        *
        * @return @p KErrNone if the request was successfully cancelled,
        *   otherwise a system wide error code.
        */
        TInt CancelNotifyDatabaseEvent();

        /**
        * From CPosLmDatabaseManager.
        *
        * @return The database URI associated with the event. The client takes
        *   ownership of the descriptor object.
        */
        HBufC* DatabaseUriFromLastEventLC();

        /**
        * From CPosLmDatabaseManager.
        *
        * @param aProtocol The protocol for which database URIs should be
        *   listed. If no protocol is specified, i.e. if an empty string is
        *   specified, all known database URIs are listed.
        * @return The list of database URIs.
        */
        CDesCArray* ListDatabasesLC(
        /* IN  */ const TDesC& aProtocol
        );

        /**
        * From CPosLmDatabaseManager.
        *
        * @param aDatabaseInfoArray On return, contains information about
        *   the landmark databases. Any objects which are in the array when
        *   it is passed to this function are not be removed.
        * @param aProtocol The protocol for which database URIs should be
        *   listed. If no protocol is specified, i.e. if an empty string is
        *   specified, all known database URIs are listed.
        */
        void ListDatabasesL(
        /* OUT */ RPointerArray<HPosLmDatabaseInfo>& aDatabaseInfoArray,
        /* IN  */ const TDesC& aProtocol
        );

        /**
        * From CPosLmDatabaseManager.
        *
        * @param aDatabaseInfo Information about the landmark database to
        *   register.
        */
        void RegisterDatabaseL(
        /* IN/OUT */ HPosLmDatabaseInfo& aDatabaseInfo
        );

        /**
        * From CPosLmDatabaseManager.
        *
        * @param aDatabaseUri The URI of the database to register.
        */
        void UnregisterDatabaseL(
        /* IN  */ const TDesC& aDatabaseUri
        );

        /**
        * From CPosLmDatabaseManager.
        *
        * @param aProtocol The protocol to unregister all databases for.
        */
        void UnregisterAllDatabasesL(
        /* IN  */ const TDesC& aProtocol
        );

        /**
        * From CPosLmDatabaseManager.
        *
        * @param aDatabaseUri The URI of the database to modify settings for.
        * @param aDatabaseSettings The new settings for the database.
        */
        void ModifyDatabaseSettingsL(
        /* IN  */ const TDesC& aDatabaseUri,
        /* IN  */ const TPosLmDatabaseSettings& aDatabaseSettings
        );

        /**
        * From CPosLmDatabaseManager.
        *
        * @param aDatabaseInfo An information object containing the URI of the
        *   landmark database. On return, the object contains information about
        *   the landmark database, including any database settings.
        */
        void GetDatabaseInfoL(
        /* IN/OUT */ HPosLmDatabaseInfo& aDatabaseInfo
        );

        /**
        * From CPosLmDatabaseManager.
        *
        * @param aDatabaseUri The URI of the database which should be checked
        *   for existence.
        * @return @p ETrue if the database exists, otherwise @p EFalse.
        */
        TBool DatabaseExistsL(
        /* IN  */ const TDesC&  aDatabaseUri
        );

        /**
        * From CPosLmDatabaseManager.
        *
        * @param aDatabaseUri The URI of the new database.
        */
        void CreateDatabaseL(
        /* IN/OUT */ HPosLmDatabaseInfo& aDatabaseInfo
        );

        /**
        * From CPosLmDatabaseManager.
        *
        * @param aDatabaseUri The URI of the database to delete.
        */
        void DeleteDatabaseL(
        /* IN  */ const TDesC&  aDatabaseUri
        );

        /**
        * From CPosLmDatabaseManager.
        *
        * @param aSourceUri The URI of the database to copy.
        * @param aTargetUri The URI of the new database location.
        */
        void CopyDatabaseL(
        /* IN  */ const TDesC&  aSourceUri,
        /* IN  */ const TDesC&  aTargetUri
        );

    protected:

        /**
        * C++ default constructor.
        */
        CPosLmDatabaseManagerImpl();

    private:

       /**
        * By default Symbian 2nd phase constructor is private.
        */
        void ConstructL();

        /**
         * Validates URI and extracts protocol.
         * Leaves with @p KErrArgument if the protocol is empty or
         * does not exist.
         */
        static const TPtrC GetProtocolFromUri(
        /* IN  */ const TDesC& aUri
        );

        /**
         * Returns @p ETrue if database URI starts with "file://" or if
         * no protocol is specified.
         */
        static TBool IsLocalDatabase(
        /* IN  */ const TDesC& aUri
        );

        /**
         * Retrieves the default database display name from the resource file.
         */
        HBufC* DefaultDatabaseDisplayNameL();

        /**
         * Fills in default flag, database drive and media type in
         * each object in the array. Helper for the list method.
         */
         void FillInDbInfoListL(
        /* IN/OUT */ RPointerArray<HPosLmDatabaseInfo>& aDatabaseInfoArray,
        /* IN     */ CPosLmDatabaseManagerPluginBase* aPlugin
        );

        /**
         * Fills in default flag, database drive and media type in
         * the object. Helper for the list method.
         */
         void FillInDbInfoL(
        /* IN/OUT */ HPosLmDatabaseInfo& aDatabaseInfo,
        /* IN     */ CPosLmDatabaseManagerPluginBase* aPlugin,
        /* IN     */ const TDesC& aDefaultDbUri
        );


        // By default, prohibit copy constructor
        CPosLmDatabaseManagerImpl( const CPosLmDatabaseManagerImpl& );
        // Prohibit assigment operator
        CPosLmDatabaseManagerImpl& operator= (
            const CPosLmDatabaseManagerImpl& );

    private:    // Data

        CPosLmSettingsHandler*      iSettingsHandler;
        CPosLmDbManPluginStore*     iPlugins;
        RPosLandmarkServer          iSession;
        RPosLmDatabaseEventListener iEventListener;
        HBufC*                      iDefaultDatabaseDisplayName;

    };

#endif      // CPOSLMDATABASEMANAGERIMPL_H

// End of File
