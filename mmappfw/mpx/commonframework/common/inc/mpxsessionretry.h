/*
* Copyright (c) 2007 Nokia Corporation and/or its subsidiary(-ies).
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
* Description:  Client session retry implementation
*
*/

#ifndef RMPXSESSIONRETRY_H
#define RMPXSESSIONRETRY_H

#include <e32std.h>

const TInt KMPXMaxServerImageNameLength = 30;

/**
*  Client session retry implementation
*
*  @lib MPXCommon.lib
*/
class RMPXSessionRetry : public RSessionBase
    {
    public:

        /** 
        * Get version info.
        *
        * @since S60 5.0
        * @return version info
        */
        TVersion Version() const;
        
        /** 
        * Connect to server.
        *
        * @since S60 5.0
        * @param aServer server name
        * @param aImage server image name
        * @return KErrNone success, otherwise system error code
        */
        TInt Connect(const TDesC& aServer, 
                     const TDesC& aImage, 
                     const TVersion& aVersion);
        
        /** 
        * Reconnect to server.
        *
        * @since S60 5.0
        * @return KErrNone success, otherwise system error code
        */
        TInt  Reconnect();
        
        /** 
        * Send a message.
        *
        * @since S60 5.0
        * @param aFunction function code
        * @return message completion code
        */
        TInt SendReceiveL(TInt aFunction);

        /** 
        * Send a message.
        *
        * @since S60 5.0
        * @param aFunction function code
        * @param aArgs parameter to server
        * @return message completion code
        */
        TInt SendReceiveL(TInt aFunction, const TIpcArgs& aArgs);
        
        /** 
        * Send message asynchronously.
        *
        * @since S60 5.0
        * @param aFunction function code
        * @param aStatus the request status object used to contain the 
        *                completion status of the request
        */
        void SendReceive(TInt aFunction, TRequestStatus& aStatus);

        /** 
        * Send message asynchronously.
        *
        * @since S60 5.0
        * @param aFunction function code
        * @param aArgs parameter to server
        * @param aStatus the request status object used to contain the 
        *                completion status of the request
        */
        void SendReceive(TInt aFunction, const TIpcArgs& aArgs,
                         TRequestStatus& aStatus);
       
    private:       
        TVersion iVersion;  
        TBuf<KMPXMaxServerImageNameLength> iServer;
        TBuf<KMPXMaxServerImageNameLength> iImage;      
    };

#endif // RMPXSESSIONRETRY_H

// End of file
