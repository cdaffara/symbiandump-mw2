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
* Description:   Class definition for COMASuplProvHandler
*
*/




#ifndef C_COMASUPLPROVHANDLER_H
#define C_COMASUPLPROVHANDLER_H

#include "CWPAdapter.h"


class COMASuplParameters;
class COMASuplSettings;
class CCommsDatabase;
class COMASuplSettingsRepository;
class COMASuplTrace;


/**
 * COMASuplProvHandler handles SUPL provisioning settings.
 */ 
class COMASuplProvHandler : public CWPAdapter
    {
    
public:

    /**
     * Factory function that instantiates a new object of COMASuplProvHandler
     *
     */
    static COMASuplProvHandler* NewL();

    /**
     * Destructor
     *
     */
    virtual ~COMASuplProvHandler();

public: // From base class CWPAdapter

    /**
     * From CWPAdapter
     * 
     */     
    TInt ItemCount() const;
    
    /**
     * From CWPAdapter
     * 
     */     
    const TDesC16& SummaryTitle( TInt aIndex ) const;

    /**
     * From CWPAdapter
     * 
     */     
    const TDesC16& SummaryText( TInt aIndex ) const;

    /**
     * From CWPAdapter
     * 
     */     
    void SaveL( TInt aItem );

    /**
     * From CWPAdapter
     * 
     */     
    TBool CanSetAsDefault( TInt aItem ) const;

    /**
     * From CWPAdapter
     * 
     */     
    void SetAsDefaultL( TInt aItem );

    /**
     * From CWPAdapter
     * 
     */     
    TInt DetailsL( TInt aItem, MWPPairVisitor& aVisitor );

private: // From base class MWPVisitor

    /**
     * From MWPVisitor
     * 
     */     
    void VisitL( CWPCharacteristic& aElement );

    /**
     * From MWPVisitor
     * 
     */     
    void VisitL( CWPParameter& aElement );

    /**
     * From MWPVisitor
     * 
     */     
    void VisitLinkL( CWPCharacteristic& aCharacteristic );

private: 

    /**
     * C++ default constructor.
     * 
     */     
    COMASuplProvHandler();

    /**
     * Symbian 2nd phase constructor.
     * 
     */     
    void ConstructL();

private:  // New functions

    /**
     * Check for SUPL settings; returns ETrue if settings accepted
     * 
     */      
    TBool AcceptedSettingGroup();

    /**
     * Add SUPL setting to iSuplSettings array
     */
    void AddSuplSettingsL( COMASuplParameters* aItem );

    /**
     * Initializes the local variation flag
     */
    void SetVariation();

private:

    /** 
     * Handle to the Comms DB
     */
    CCommsDatabase*                 iCommsDb;

    /** 
     * Stores complete settings pairs
     */
    CArrayPtr<COMASuplSettings>*    iSuplSettings;

    /** 
     * Current setting item.
     */   
    COMASuplParameters*             iCurrentItem;

    /**
     * Title text for SUPL settings
     */
    HBufC*                          iTitle;
    
    /**
     * Handle to instance of COMASuplSettingsRepository
     */
    COMASuplSettingsRepository*     iRepository;
    
    /**
     * Handle to an instance of COMASuplTrace used for tracing
     */ 
    COMASuplTrace*		            iTrace;
    
    /*
     * Local Variation Flag
     */
    TBool                           iVariantEnabled;
    
    };

#endif	// C_COMASUPLPROVHANDLER_H
