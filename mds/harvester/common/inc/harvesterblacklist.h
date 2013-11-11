/*
* Copyright (c) 2007-2009 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:  Blacklisting of files that fail harvesting.
*
*/


#ifndef CHARVESTERBLACKLIST_H
#define CHARVESTERBLACKLIST_H

#include <e32def.h>

#include "../harvester/blacklistclient/inc/blacklistclient.h"
/**
* This class implements blacklisting of files that
* have failed harvesting.
* All files should be added to blacklist while harvesting
* and removed when harvesting is successful.
* If harvesting fails, file is left blacklisted.
*/
NONSHARABLE_CLASS( CHarvesterBlacklist ) : public CBase
    {
    public:

        /**
         * Creates and constructs a new instance of CMdsBlacklist.
         *
         * @return A pointer to the created instance.
         */
        IMPORT_C static CHarvesterBlacklist* NewL();

        /**
         * Destructor.
         */
        IMPORT_C virtual ~CHarvesterBlacklist();

        /**
         * Adds a file to blacklist with it's URI.
         * 
         * @param aUri  URI to a file.
         * @param aMediaId  Media Id of the media where file is located.
         * @return KErrNone or a system wide error code.
         */
        IMPORT_C TInt AddFile( const TDesC& aUri, TUint32 aMediaId, TTime aLastModifiedTime  );

        /**
         * Removes a file from the blacklist.
         * 
         * @param aUri  URI to a file.
         * @param aMediaId  Media Id of the media where file is located.
         * @return KErrNone or a system wide error code.
         */
        IMPORT_C TInt RemoveFile( const TDesC& aUri, TUint32 aMediaId );

        /**
         * Checks if a given file is blacklisted.
         * 
         * @param aUri      URI to a file.
         * @param aMediaId  Media id of the file.
         * @return ETrue, if the file is blacklisted, otherwise EFalse.
         */
        IMPORT_C TBool IsBlacklisted( const TDesC& aUri, TUint32 aMediaId,  TTime aLastModifiedTime );
        
         /**
         * Closes database connection through CMdSSqLiteConnection.
         */
         IMPORT_C void CloseDatabase();
         
         /**
         * Opens database connection and load blacklist.
         */
         IMPORT_C void OpenDatabase();

    private:

        /**
         * Constructor, called by NewL() only.
         */
        CHarvesterBlacklist();

        /**
         * Second phase constructor called by NewL.
         */
        void ConstructL();

        /**
         * Connect to blacklist client
         */
        void ConnectToBlackListServerL();

    private: // data
    
        RBlacklistClient iBlacklistClient;
    };

#endif // CHARVESTERBLACKLIST_H
