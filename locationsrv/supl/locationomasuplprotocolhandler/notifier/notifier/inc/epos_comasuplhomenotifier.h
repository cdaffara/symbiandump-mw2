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
* Description:   SUPL Notifier for the user in his home network
*
*/



#ifndef C_SUPLHOMENOTIFIER_H
#define C_SUPLHOMENOTIFIER_H

// System Includes
#include <eiknotapi.h>

// User Includes
#include "epos_comasuplnotifier.h"

// Forward Declarations

/**
 * SUPL notifier specialization for home network.
 *
 * @since S60 v3.2
 */
class CSuplHomeNotifier : public CSuplNotifier
    {
public:
    /**
     * Destructor.
     */
    virtual ~CSuplHomeNotifier();
    
    /**
     * Symbian two phased constructor
     */
    static CSuplHomeNotifier* NewL();
    
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
    CSuplHomeNotifier();
    
    /**
     * Second phase of the two phase constructor
     */
    void ConstructL();
    
private:  // Data members   
    };

#endif // C_SUPLHOMENOTIFIER_H
