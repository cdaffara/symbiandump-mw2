/*
* Copyright (c) 2006 Nokia Corporation and/or its subsidiary(-ies).
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
* Description:  Client session class to  server
*
*/

#ifndef RMPXSESSION_H
#define RMPXSESSION_H

#include <e32std.h>


class RMPXSessionRetry;

/**
*  Client session to playback server.
*
*  @lib MPXCommon.lib
*/
class RMPXSession
    {
    public:
        /**
         * Constructor
         *
         * @since S60 5.0
         */
        IMPORT_C RMPXSession();

        /**
         * Destructor
         *
         * @since S60 5.0
         */
        IMPORT_C ~RMPXSession();

        /** 
        * Get version info.
        *
        * @since S60 3.2.3
        * @return version info
        */
        IMPORT_C TVersion Version() const;

        /** 
        * Connect to server.
        *
        * @since S60 3.2.3
        * @param aServer server name
        * @param aImage server image name
        * @return KErrNone success, otherwise system error code
        */
        IMPORT_C TInt  Connect(const TDesC& aServer, 
                               const TDesC& aImage, 
                               const TVersion& aVersion);

        /** 
        * Reconnect to server.
        *
        * @since S60 5.0
        * @return KErrNone success, otherwise system error code
        */
        IMPORT_C TInt  Reconnect() const;
        
        /** 
        * Send a message.
        *
        * @since S60 3.2.3
        * @param aFunction function code
        * @return message completion code
        */
        IMPORT_C TInt SendReceiveL(TInt aFunction) const;

        /** 
        * Send a message.
        *
        * @since S60 3.2.3
        * @param aFunction function code
        * @param aArgs parameter to server
        * @return message completion code
        */
        IMPORT_C TInt SendReceiveL(TInt aFunction,const TIpcArgs& aArgs) const;
        
        /** 
        * Send message asynchronously.
        *
        * @since S60 3.2.3
        * @param aFunction function code
        * @param aStatus the request status object used to contain the 
        *                completion status of the request
        */
        IMPORT_C void SendReceive(TInt aFunction,TRequestStatus& aStatus) const;

        /** 
        * Send message asynchronously.
        *
        * @since S60 3.2.3
        * @param aFunction function code
        * @param aArgs parameter to server
        * @param aStatus the request status object used to contain the 
        *                completion status of the request
        */
        IMPORT_C void SendReceive(TInt aFunction,const TIpcArgs& aArgs,
                                  TRequestStatus& aStatus) const;
        
        /** 
        * Close session.
        *
        * @since S60 3.2.3
        */
        IMPORT_C void Close();

    private:
        RMPXSessionRetry* iSessionRetry;
    };

#endif // RMPXSESSION_H

// End of file
