/*
* Copyright (c) 2006, 2007 Nokia Corporation and/or its subsidiary(-ies).
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
* Description:  Class definition of Status Manager
*
*/



#ifndef C_LBTLOCATIONREQUESTOR_H
#define C_LBTLOCATIONREQUESTOR_H


#include <lbs.h>
#include "lbtpsychangeobserver.h"

// FORWARD DECLARATIONS
class CLbtPsyChangeListner;


/**
 * This class defines the methods used by CLbtStrategyEngine to acquire 
 * location information. This is a wrapper class that establishes a session
 * to location server to obtain location information.
 */
NONSHARABLE_CLASS( CLbtLocationRequestor ) : public CBase,
											 public MLbtPsyChangeObserver

	{

public: // Methods

    /**
     * Factory function that instantiates an object of CLbtLocationRequestor
     *
     * @param aPosID - Identifies the module to be used by location server to 
     *                 obtain location information
     */
	static CLbtLocationRequestor* NewL(  );

    /**
     * Destructor
     */
	~CLbtLocationRequestor();

    /**
     * Requests location information from location server
     *
     * @param aPosInfo - Holds the current position of the device
     * @param aStatus - Holds the result of the asynchronous request
     * @aPosID - Identifies the module to be used by location server to obtain
     *           location information
     */
	void CurrentLocation( TPositionInfo& aPosInfo, 
	                        TRequestStatus& aStatus, TPositionModuleId aPosID,
	                        TTimeIntervalMicroSeconds aTimeOut );
	
	/**
	 * Request location information from NPP Psy
	 */
	void CurrentLocationFromNpp( TPositionInfo& aPosInfo, 
	                             TRequestStatus& aStatus,
                                 TTimeIntervalMicroSeconds aTimeOut );
	
	/**
	 * Retrieves information about the positioning technology being used
	 */
	void GetModuleInfo( TPositionModuleId aPosID );
	
	/**
	 * Retrieves position module Id used to get location information
	 */
	TPositionModuleId GetPositionModuleId();
	 
	/**
	 * Determines if GPS method is used to retrieve location information
	 */
	TBool GPSUsed() const;
	
    /**
     * Cancels an outstanding request for obtaining location information
     */
    void Cancel();
    
    /**
     * Cancels an outstanding request for obtaining location information from NPP PSY
     */
    void CancelFixFromNpp();
	
private:

    /**
     * Default constructor
     */
	CLbtLocationRequestor();

    /**
     * 2nd phase constuctor for instantiating member variables     
     * @param aPosID - Identifies the module to be used by location server to 
     *                 obtain location information
     */
	void ConstructL( );
	
    /**
     * Checks for changes 
     * @param aPosID - Identifies the module to be used by location server to 
     *                 obtain location information
     */
	void OpenConnectionL( TPositionModuleId aPosID );

private: // from MLbtPsyChangeObserver
	void HandlePsyChangeEvent( TPositionModuleId aModuleId );
	
private:

	/**
	 * Handle to the location server interface
	 */
	RPositionServer	    iPosServer;
	
	/**
	 * Handle to create a subsession with the location sever
	 */
	RPositioner		    iPositioner;
	
	/**
	 * Handle to NPP psy
	 */
	RPositioner         iNppPositioner;
	
	/**
	 * Identifies the module to be used by location server to obtain location 
	 * information
	 */
	TPositionModuleId   iPosID;
	
	/**
	 * Identifier to determine if GPS method is being used to obtain location
	 * information
	 */
	TBool               iGPSUsed;
	
	/**
	 * Identifier to determine if the handle to NPP psy is valid
	 */
	TBool              iValidNppHandle;
	
	/**
	 * Object to listen to PSY Changes if NBP is not enabled
	 */
	CLbtPsyChangeListner*		iNbpChangeListener;
	
	};

#endif	// C_LBTLOCATIONREQUESTOR_H
