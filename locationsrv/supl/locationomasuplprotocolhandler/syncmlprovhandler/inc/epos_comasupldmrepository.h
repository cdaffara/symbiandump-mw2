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
* Description:   Class definition for COMASuplDMRepository
*
*/




#ifndef C_COMASUPLDMREPOSITORY_H
#define C_COMASUPLDMREPOSITORY_H


#include <e32base.h>
#include <epos_csuplsettings.h>
#include "epos_csuplsettingsinternal.h"
#include "epos_csuplsettingextensionparams.h"
#include "epos_tomasuplsettingsmap.h"
class COMASuplTrace;

const TInt KSuplSettingParamLen = 512;

/**
 * Represents the SUPL settings repository. Uses Supl Settings API to store
 * SUPL settings information
 *
 */
class COMASuplDMRepository : public CBase
    {
    
public:

    /**
     * Factory function that instantiates a new object of COMASuplSettings
     *
     */        
     static COMASuplDMRepository* NewL();
    
    /**
     * Destructor
     *
     */ 
    ~COMASuplDMRepository();
    
public:

    /**
     * Initializes SUPL Settings API
     *
     */
    void Initialize();
    
    /**
     * Retrieves IAP Name
     *
     */
    void GetIAPNameLC( TDesC8*& aIAP );
    
    /**
     * Retrieves HSLP address
     *
     */
    void GetHSLPAddressLC( TDesC8*& aHSLP );
    
    /*
     * Retrieves extended parameters based on the type requested for
     * @param   [OUT]aValue value of the requested parameter is returned in this
     * @param   [IN]aType indicates the type of extension parameter that is reqested for  
     */
    void  GetExtnParamsLC(TDesC8*& aValue,TOMASuplSettingsMap::TSuplSettingType aType);

    /**
     * Starts an atomic transaction
     *
     */
    void StartTransactionL();
    
    /**
     * Commits changes in an atomic transaction
     *
     */
    void CommitTransactionL();
    
    /**
     * Reverts changes made in an atomic transaction
     *
     */
    void RollbackTransactionL();
    
    /**
     * Save SUPL settings to system.
     *
     */
    void SaveL( const TDesC& aValue,TOMASuplSettingsMap::TSuplSettingType aType,TBool aIsAtomic );

private:  // Constructors and destructor
    
    /**
     * C++ Constructor
     */
    COMASuplDMRepository();
    
    /**
     * Symbian 2nd phase constructor
     */
    void ConstructL();
    
    /**
     * Saves SUPL Settings Information
     */    
    void SaveSettingsL( const TDesC& aValue,TOMASuplSettingsMap::TSuplSettingType aType); 
    
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
    
    TRequestStatus  iStatus;
    
    //For bufferring server name
    HBufC* iServerAddress;
    
    //For bufferring prefconref
    HBufC* iIapName;
    
    //for buffering server id
    HBufC* iServerId;
    
    //For buffering toconref
    HBufC* iIap1;
    
    //For buffering conref    
    HBufC* iIap2;
    
    ///For buffering SLP
    HBufC* iSlp;
    
    //For buffering AddrType
    HBufC* iServerAddType;
    
    TInt64 iSlpId;
    
    //indicates which of the parameters have NULL string assigned
    TInt iNullMask;
    };

#endif      // C_COMASUPLDMREPOSITORY_H
