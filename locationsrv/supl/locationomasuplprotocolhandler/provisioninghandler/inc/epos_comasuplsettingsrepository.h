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
* Description:   Class definition for COMASuplSettingsRepository
*
*/




#ifndef C_COMASUPLSETTINGSREPOSITORY_H
#define C_COMASUPLSETTINGSREPOSITORY_H


#include <e32base.h>
#include "epos_csuplsettingsinternal.h"

class COMASuplTrace;

/**
 * Represents the SUPL settings repository. Uses Supl Settings API to store
 * SUPL settings information
 *
 */
class COMASuplSettingsRepository : public CActive
    {
    
public:

    /**
     * Factory function that instantiates a new object of COMASuplSettings
     *
     */        
     static COMASuplSettingsRepository* NewL();
    
    /**
     * Destructor
     *
     */ 
    ~COMASuplSettingsRepository();
    
public:

    /**
     * Save SUPL settings to system.
     *
     */
    void SaveL( const TDesC& aIAP, const TDesC& aHSLP );

private:  // Constructors and destructor
    
    /**
     * C++ Constructor
     */
    COMASuplSettingsRepository();
    
    /**
     * Symbian 2nd phase constructor
     */
    void ConstructL();
    
    /**
     * Request completion event handler
     */
    void RunL();
    
    /**
     * Implements cancellation of an outstanding request
     */
    void DoCancel();
    
private:    // Data

    enum TState
        {
        ENotInitialized = 0,
        EInitializing,
        EInitialized
        };
        
    TState          iState;
    
    CSuplSettingsInternal*  iSettings;
    
    COMASuplTrace*  iTrace;
    
    };

#endif      // C_COMASUPLSETTINGSREPOSITORY_H
