/*
* Copyright (c) 2006-2009 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:  An interface to Location Manager
*
*/

#ifndef R_RLOCATIONMANAGER_H
#define R_RLOCATIONMANAGER_H

#include <e32base.h>
#include <lbsposition.h>
#include <etel3rdparty.h>

/**
 *  Base class for Location Manager client interfaces.
 *  @lib LocationManager.lib
 *  @since S60 3.1
 */
NONSHARABLE_CLASS( RLocationManager ) : public RSessionBase
    {
public:

    /**
     * C++ default constructor.
     * @since S60 3.1
     */
    IMPORT_C RLocationManager();
    
    /**
     * Creates new session to Location Manager Server.
     * @since S60 3.1
     * @param None.
     * @return KErrNone if successful.
     */
    IMPORT_C TInt Connect();
    
    /**
     * Closes the connection with Location Manager Server
     * @since S60 3.1
     * @param None.
     */
    IMPORT_C void Close();

private:
    /**
     * Returns the version of Location Manager.
     */
    TVersion Version() const;

protected:
	/**
	 * Completes an asynchronous request with an error code.
	 * @param aError Symbian error code
	 */
	void CompleteRequest(TRequestStatus& aStatus, TInt aError);

    };
    
#endif // R_RLOCATIONMANAGER_H

//End of File
