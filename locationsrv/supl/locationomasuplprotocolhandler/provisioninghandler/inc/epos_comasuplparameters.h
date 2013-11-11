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
* Description:   Class definition for COMASuplParameters
*
*/




#ifndef C_COMASUPLPARAMETERS_H
#define C_COMASUPLPARAMETERS_H


#include <MWPVisitor.h>
#include <e32base.h>


class COMASuplTrace;
class CWPCharacteristic;


/**
 * COMASuplParameters contains the SUPL settings parameters
 */
class COMASuplParameters : public CBase, public MWPVisitor
    {
    
public:  // Constructors and destructor

    /**
     * Factory function that instantiates a new object of COMASuplParameters
     *
     */
     static COMASuplParameters* NewL( COMASuplTrace* aTrace );
        
    /**
     * Factory function that instantiates a new object of COMASuplParameters
     *
     */
     static COMASuplParameters* NewLC( COMASuplTrace* aTrace );

    /**
     * Destructor
     *
     */
    ~COMASuplParameters();

public:

	/*
	 * Returns the parameter value
	 *
	 */
	 const TDesC& ParameterValue(TInt aParamID) const;

	/*
	 * Returns the associated IAP ID reference
	 *
	 */
    TUint32 AssociatedIapId();
    
	/*
	 * Returns the associated IAP Name reference
	 *
	 */
    HBufC* AssociatedIapNameL( TUint32 aWapId, CCommsDatabase* aCommsDb ) const;

	/*
	 * Check to determine that current settings are valid
	 *
	 */     
    TBool IsValid() const;


public: // From base class MWPVisitor

	/*
	 * Method to handle SUPL characteristic 
	 *
	 */  
    void VisitL( CWPCharacteristic& aCharacteristic );
    
	/*
	 * Method to handle SUPL parameters
	 *
	 */  
    void VisitL( CWPParameter& aParameter );
    
	/*
	 * Method to handle characteristic link to NAPDEF 
	 *
	 */  
    void VisitLinkL( CWPCharacteristic& aCharacteristic );

private: 

	/*
	 * C++ Constructor
	 *
	 */  
    COMASuplParameters( COMASuplTrace* aTrace );

	/*
	 * Second phase constructor
	 *
	 */
	 void ConstructL();

private:

    /**
     * Array to SUPL setting parameters
     */ 
    CArrayPtr<CWPParameter>*    iSuplSettingParams;

    /**
     * Ref:to the IAP settings
     */
    CWPCharacteristic*          iNapDef;

    /**
     * Handle to an instance of COMASuplTrace used for tracing
     */ 
    COMASuplTrace*			        iTrace;

    };

#endif	// C_COMASUPLPARAMETERS_H
