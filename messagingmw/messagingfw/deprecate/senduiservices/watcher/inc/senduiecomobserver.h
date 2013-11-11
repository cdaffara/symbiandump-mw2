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
* Description:   Listens Ecom events. Requests service update, 
*                - if SendUI Ecom service count changes
*                - has changed since previous boot.
*
*/



#ifndef C_SENDUIECOMOBSERVER_H
#define C_SENDUIECOMOBSERVER_H

#include <e32base.h>
#include <e32property.h>

/**
 *  ?one_line_short_description
 *
 *  ?more_complete_description
 *
 *  @lib ?library
 *  @since S60 ?S60_version *** for example, S60 v3.0
 */
class CSendUiEcomObserver : public CActive
    {

public:

    static CSendUiEcomObserver* NewL();

    virtual ~CSendUiEcomObserver();

// from base class CActive

    /**
     * From CActive.
     *
     * @since S60 v3.2
     */
    void RunL();

    /**
     * From CActive.
     *
     * @since S60 v3.2
     */
    void DoCancel();
    
    void CheckForUpdateL();

private:

    CSendUiEcomObserver();

    void ConstructL();

private: // data

    /**
     * Property for making update requests
     */
    RProperty iPropertyUpdateRequest;

    /**
     * Property for tracking update operation flow
     */
    RProperty iPropertyUpdateOperation;

    REComSession& iEcomSession;
    };

#endif // C_SENDUIECOMOBSERVER_H
