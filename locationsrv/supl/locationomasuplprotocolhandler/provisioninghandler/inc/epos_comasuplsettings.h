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
* Description:   Class definition for COMASuplSettings
*
*/




#ifndef C_COMASUPLSETTINGS_H
#define C_COMASUPLSETTINGS_H


#include <e32base.h>


class COMASuplParameters;
class CCommsDatabase;
class COMASuplSettingsRepository;
class COMASuplTrace;

/**
 * Saves SUPL settings
 */
class COMASuplSettings : public CBase
    {
    
public:

    /**
     * Factory function that instantiates a new object of COMASuplSettings
     *
     */        
     static COMASuplSettings* NewL( COMASuplParameters* aItem, 
                                    COMASuplTrace* aTrace );
    
    /**
     * Destructor
     *
     */ 
    virtual ~COMASuplSettings();

public: // New functions
    
    /**
     * Save SUPL settings to system.
     *
     */
    void SaveL( COMASuplSettingsRepository* aRepository );

    /**
     * Returns the Setting name
     *
     */
    const TDesC& SummaryText();
    
    /**
     * Set new default name
     *
     */
    void SetDefaultNameL( const TDesC& aDefaultName );

    /**
     * Set reference to CommsDataBase. This is needed when saving 
     * SUPL settings.Must be set before calling SaveL. 
     *
     */
    void SetCommsDataBase( CCommsDatabase& aCommsDataBase );
    
    /**
     * Returns IAP ID
     *
     */
    TUint32 IapIdFromWapIdL( TUint32 aWapId ) const;

private:  // Constructors and destructor
    
    /**
     * C++ Constructor
     */
    COMASuplSettings( COMASuplParameters* aItem, COMASuplTrace* aTrace );
    
    /**
     * Symbian 2nd phase constructor
     */
    void ConstructL();
        
private:    // Data

    /** 
     * Handle to the Comms DB
     */
    CCommsDatabase*         iCommsDb;

    /**
     * Default name for SUPL settings.
     */
    HBufC*                  iDefaultName;
    
    /**
     * Reference to instance of COMASuplParameters
     *
     */
    COMASuplParameters*     iItem;
    
    /**
     * Handle to an instance of COMASuplTrace used for tracing
     */ 
    COMASuplTrace*		    iTrace;
        
    };

#endif      // C_COMASUPLSETTINGS_H
