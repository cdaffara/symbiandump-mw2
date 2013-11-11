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
* Description:   SUPL Notifier for the user not in his home network
*
*/



#ifndef C_SUPLROAMINGNOTIFIER_H
#define C_SUPLROAMINGNOTIFIER_H

// System Includes
#include <eiknotapi.h>

// User Includes
#include "epos_comasuplnotifier.h"

// Forward Declarations

/**
 * SUPL notifier specialization for roaming network.
 *
 * @since S60 v3.2
 */
class CSuplRoamingNotifier : public CSuplNotifier
    {
public:
    /**
     * Destructor.
     */
    virtual ~CSuplRoamingNotifier();
    
    /**
     * Symbian two phased constructor
     */
    static CSuplRoamingNotifier* NewL();
    
public: // Inherited from CSuplNotifier
	
    /**
     * Returns the query text for the SUPL notifier
     */	
	virtual HBufC* QueryTextL();
	
    /**
     * Returns the SUPL query ID
     */	    
    virtual TInt SuplQueryId();	
	    
public: // Inherited from MEikSrvNotifierBase2

    /**
     * Registers the SUPL Home notifier
     */
    virtual TNotifierInfo RegisterL();
                                      
private:
    /**
     * Private C++ constructor
     */
    CSuplRoamingNotifier();
    
    /**
     * Second phase of the two phase constructor
     */
    void ConstructL();
    
private:  // Data members   
    };

#endif // C_SUPLROAMINGNOTIFIER_H
