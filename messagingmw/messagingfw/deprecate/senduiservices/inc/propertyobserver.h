/*
* Copyright (c) 2009 Nokia Corporation and/or its subsidiary(-ies).
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
* Description:   Observes sending service list and invokes an local sending service
*                update. This is required to keep the sending service list up-to-date
*                in a loaded sendui object.
*
*/



#ifndef C_PROPERTYOBSERVER_H
#define C_PROPERTYOBSERVER_H

#include <e32base.h>
#include <e32property.h>

class CSendUiImpl;

/**
 *  Property observer for catching sending service list updates
 *
 *  @lib sendui.lib
 *  @since S60 S60 v3.2
 */

NONSHARABLE_CLASS ( CPropertyObserver ): public CActive
    {

public:

    static CPropertyObserver* NewL( CSendUiImpl* aParent );

    virtual ~CPropertyObserver();

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

private:

    CPropertyObserver( CSendUiImpl* aParent );

    void ConstructL();

private: // data


    /**
     * Used for accessing GetEcomServicesL from CSendUiImpl
     * Not own.
     */
    CSendUiImpl* iParent;
    
    RProperty iPropertyUpdateOperation;

    };


#endif // C_PROPERTYOBSERVER_H
