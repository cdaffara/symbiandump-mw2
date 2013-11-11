/*
* Copyright (c) 2008 Nokia Corporation and/or its subsidiary(-ies).
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
* Description:  Definition of Movement Detection API
*
*/


#ifndef C_LBTCELLIDMVMTDET_H
#define C_LBTCELLIDMVMTDET_H

#include <e32base.h>

#include <commdb.h>
#include "lbtmovementdetector.h"
#include "lbtcellidmvmtdetgetcelldata.h"
#include "lbtcellidmvmtdetcelloberserver.h"
#include "lbtcellinfo.h"
#include "lbtcellidmvmtdetconsts.h"


/**
 *  CLbtCellIdMvmtDet is an ECom plugin that implements CLbtMovementDetector.
 *
 *  This class is responsible for monitering movement of mobile based on
 *  GSM information available to mobile at any instant.
 *
 *  @lib movementdetector.lib
 *  @since  S60 v4.0
 */
NONSHARABLE_CLASS( CLbtCellIdMvmtDet) : public CLbtMovementDetector,
										public MLbtCellIdMvmtDetCellOberserver
    {

public:

   /**
     * Load a movement detector and initalises it. Detector does not start running until
     * a call to @p Start().
     *
     * @since S60 TACO
     */
     static CLbtCellIdMvmtDet* NewL();

    /**
     * Unloads this movement detector.
     *
     * @since S60 TACO
     */
     virtual ~CLbtCellIdMvmtDet();

	// From CLbtMovementDetector

    /**
     * Request notification for movement. This registers an object as an observer to
     * movement notifications.
     *
     * @since S60 TACO
     * @param aObserver An observer to be registered. Object must implement @p MMovementObserver
     *
     * @param aSleepDuration Time duration during which movement detection should listen for movement.
     */
     virtual void RequestNotificationL( MLbtMovementObserver* aObserver,
                                                    TInt aSleepDuration );

    /**
     * Request removal of movement notification request.
     *
     * @since S60 TACO
     */
     virtual void StopNotification();

private://from MLbtCellIdMvmtDetGsmOberserver

	 void HandleCellDataL( RMmCustomAPI::TMmCellInfo& aCellInfo, TInt& aError );
	 
private:

	/**  
	 * Initialises communication database,opens a session to 
	 * RTelServer and RMobilePhone.
	 */
	 void InitializeL();
	 
	/**
	 * Gets the information regarding ARFCN,BSIC,timing advance
	 * for the mobile at that instance.
	 *
	 */
	 void GetCellInfo();
	 
	/**
	 *
	 */
	 void HandleGsmDataL( RMmCustomAPI::TMmCellInfo& aCellInfo );
	 
	/**
	 *
	 */
	 void HandleWcmdaDataL( RMmCustomAPI::TMmCellInfo& aCellInfo ); 
	 
	/**
	 * 
	 */
     TInt ComputeED( TReal& aED, CLbtCellInfo* aCurrCellInfo, 
                                    CLbtCellInfo* aPrevCellInfo );

	/**
	 *
	 */
	 void ComputeVarianceOfED();
	 
	/**
	 *
	 */
	 void CalculateSDForWcdmaNmr(TReal& aCarrierRSSISD,TReal& aCpichEcN0SD,
                                 TReal& aCpichRscpSD,TReal& aPathlossSD,
                                 TInt& aCellChangeCount );
	/**
	 *
	 */
	 TBool MovementDetected( TReal& aCarrierRSSISD,TReal& aCpichEcN0SD,
                             TReal& aCpichRscpSD,TReal& aPathlossSD,
                             TInt& aCellChangeCount  ); 
                             
    /**
     * 
     */
     void DetectShortGsmMovement();
     
    /**
     * 
     */
     void DetectShortWcdmaMovement();
     
    /**
     * 
     */
     void CheckBsicVariation(); 
     
    /**
     * 
     */
     TReal CalculateComparisionFactor( TInt aSleepDuration );
                               
	/**
	 * 2nd phase constuctor for instantiating member variables
	 */
	 void ConstructL();
	
	/**
	 * Default C++ Constructor
	 */
	 CLbtCellIdMvmtDet();
	 
private: // data

    /**
     * 
     */
    TTime                               iStartTime; 
    /**
     * Sleep duration
     */
    TInt                                iSleepDuration;
    
    /**
     *
     */
    TReal                               iSumOfED;

    /**
     *
     */
    TReal                               iAvgOfVariance;

    /**
     *
     */
    RArray< TReal >                     iVarianceOfED;

    /**
     *
     */
    RArray< TReal >                     iED;
    
    /**
     * 
     */
    RArray< TReal >                     iSumOfSd;
    
    /**
     *
     */
    RArray< TReal >                     iVarianceOfRssi;  

    /**
     * Handle to telephony server.
     */
    RTelServer 			                iTelServer;
   
    /**
     * Handle to mobile phone functionality.
     */
    RMobilePhone 		                iMPhone;
    
    /**
     * Handle to etel multimode custom api.
     */
    RMmCustomAPI 		                iCApi;
    /**
     * Previous network type
     */
    RMmCustomAPI::TMmCellInfo::TNwMode  iPreviousNetworkMode; 

    /**
     *
     */
    CLbtCellInfo*                        iPrevCellInfo;
    
    /**
     *
     */
    CLbtCellInfo*                        iInitialCellInfo;
    
    /**
     *
     */
    CLbtCellInfo*                        iCurrentCellInfo;
    
    /**
     *
     */
    CLbtCellIdMvmtDetGetCellData*        iCellIdMvmtDetGetCellData;
    
    /**
     * Wcdma cell info
     */
    CLbtCellInfo*                        iWcdmaCellInfo; 
    
    
    /**
     * Pointer to movement detection observer.
     * Not own
     */
    MLbtMovementObserver*               iObserver;
    
    };


#endif // C_LBTCELLIDMVMTDET_H
