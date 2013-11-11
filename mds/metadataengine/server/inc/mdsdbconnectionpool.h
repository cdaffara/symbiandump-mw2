/*
* Copyright (c) 2002-2009 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:  A pool of database connections
*
*/

#ifndef __DBCONNECTIONPOOL_H__
#define __DBCONNECTIONPOOL_H__

#include <e32base.h>

class CMdSSqLiteConnection;


/**
 * A pool that handles interthread use of shared
 * database connections.
 * Current implementation uses only one shared
 * connection
 */
class MMdSDbConnectionPool
    {
    public: // Constructors and destructor

        static void SetDefaultDB( CMdSSqLiteConnection* aConnection );
        static CMdSSqLiteConnection& GetDefaultDBL();
    private:
    	friend class CMdSMaintenanceEngine;
        static void DisconnectAll();
        static void ConnectAllL();


    private: // Member data
        
        static CMdSSqLiteConnection* iConnection;
        static TBool iDBValid;
    };


#endif	// __DBCONNECTIONPOOL_H__

