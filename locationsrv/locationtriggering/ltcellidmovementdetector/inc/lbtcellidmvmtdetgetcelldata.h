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
* Description:  Definition of CLbtCellIdMvmtDetGetCellData class.
*
*/



#ifndef C_LBTCELLIDMVMTDETGETCELLDATA_H
#define C_LBTCELLIDMVMTDETGETCELLDATA_H


// INCLUDE FILES
#include <e32base.h>
#include <rmmcustomapi.h>

#include "lbtcellidmvmtdetcelloberserver.h"


class CLbtCellIdMvmtDetGetCellData : public CActive
	{
	
public:		

    /**
	 * Instantiates a new object ofCLbtCellIdMvmtDetGetCellData
	 */
	static CLbtCellIdMvmtDetGetCellData* NewL(
	                    MLbtCellIdMvmtDetCellOberserver& aObserver,
	  					RMmCustomAPI& aMnCustomApi);
		
	/**
	 * Destructor
	 */
    virtual ~CLbtCellIdMvmtDetGetCellData();

    /**
     * Initiates operation to acquire NW measurements at 1s intervals
     */
    void Start();
    
	/**
	 * Gets the information regarding ARFCN,BSIC,timing advance
	 * for the mobile at that instance.
	 */
	void GetCellInfo();
	  
    /**
     * Terminates NW measurement acquisition
     */
	void Stop();
	     
protected : // From CActive

    /**
     *
     */    
    void RunL();
      
    /**
     *
     */    
    void DoCancel();   
    
private:

    /**
	 * Default C++ Constructor
	 */
	CLbtCellIdMvmtDetGetCellData(MLbtCellIdMvmtDetCellOberserver& aObserver,
	  									RMmCustomAPI& aMnCustomApi);  
	/**
	 * 2nd phase constuctor for instantiating member variables
	 */ 
	void ConstructL();
	  
	/**
	 *
	 */
	enum TState
        {
        EIdle = 1,
        ETimerOperation,
        EProcessCellInfo
        };
	  
private: // data
	
	/**
	 * Reference to MLbtCellIdMvmtDetCellOberserver class.
	 */ 
	MLbtCellIdMvmtDetCellOberserver& iObserver;

	/**
	 * 
	 */ 
    TInt                            iReqID;
    
	/**
	 * 
	 */ 
    TState                          iState;

	/**
	 * 
	 */ 
    RMmCustomAPI&                   iCApi;

	/**
	 * 
	 */ 
    RMmCustomAPI::TMmCellInfo       iCellInfo;

	/**
	 * 
	 */ 
    RMmCustomAPI::TMmCellInfoPckg   iCellPckg;

	/**
	 * 
	 */ 
    RTimer                          iTimer;

	/**
	 * 
	 */ 
    TBool                           iStopNotification;

	};
    
#endif // C_LBTCELLIDMVMTDETGETCELLDATA_H
