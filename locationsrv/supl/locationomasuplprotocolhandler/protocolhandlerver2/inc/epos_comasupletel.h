/*
* Copyright (c) 2002-2005 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:   class for retirving location id from ETel.
*  Version     : %version: 2 % << Don't touch! Updated by Synergy at check-out.
*
*/


#ifndef EPOS_COMASUPLETEL_H
#define EPOS_COMASUPLETEL_H

// INCLUDES
#include <e32def.h>
#include <e32base.h>
#include <etelmm.h>

#include "epos_tomasuplecellinfo.h"

#ifdef USE_E_CELL_ID_STUB
	#include "epos_rsuplmmcustomapi.h"
#else
	#include "rmmcustomapi.h"
#endif

//Forward declaration
class COMASuplLocationId;
class RMobilePhone;
class COMASuplTrace;

/**
*  Observer class for listening to Etel Events
*  @lib eposomasuplprotocolhandler.lib
*  @since S60 v3.1u
*/

class MOMASuplETelObserver
	{
	public:
		// Location Id & Error while completing request
		virtual void LocationIDRequestCompletedL(COMASuplLocationId* aLocationId,TInt aErrorCode) = 0;
		
		// ECell-Id & Error while completing request
		virtual void LocationECellIdRequestCompletedL(TOMASuplECellInfo& aECId,TInt aErrorCode) = 0;
		
		// Location Id for comparision & Error while completing request
		virtual void ComparisionLocationIDRequestCompletedL(COMASuplLocationId* aLocationId,TInt aErrorCode) = 0;
		
	};

/**
*  Class for getting the Cell Id information	
*  @lib eposomasuplprotocolhandler.lib
*  @since S60 v3.1u
*/

class COMASuplETel : public CActive
    {
   
   
	public:
		enum TCellMode
			{
				EOMASuplCellId = 0,			
				EOMASuplECellId,
				EOMASuplCompareCellId
			};   
			
    public:  // Constructors and destructor
        
   		 /**
        * Performs construction of an object
        * @since S60  v3.1u
        * @param None
        * @return None
        */
   		static COMASuplETel* NewL(MOMASuplETelObserver& aObserver,RMobilePhone& aPhone);
   		
        /**
        * Performs destructor
        * @since S60  v3.1u
        * @param None
        * @return None
        */
        ~COMASuplETel();
        
        /**
        * Performs the retrieving of cell id information
        * @since S60  v3.1u
        * @param None
        * @return None
        */        
        void GetLocationID();
        
        /**
        * Performs the retrieving of E-cell id information
        * @since S60  v3.2
        * @param None
        * @return None
        */        
        void GetECellID();
        
        /**
         * Performs the retrieving of cell id information for comparision purposes
         * @since S60  v3.1u
         * @param None
         * @return None
         */ 
        void GetComparisionLocationID();
     private:
     
        /**
        * Handles the data retrived after LocationId
        * @since S60  v3.2
        * @param None
        * @return None
        */        
     	void HandleLocationIdL();
     	
		/**
		* Handles the data retrived after E-Cell Id
		* @since S60  v3.2
		* @param None
		* @return None
		*/        
		void HandleECellIdL();
		
		/**
		* FillECellWCDMAInfo Fills the data retrived after E-Cell Id into other struct
		* @since S60  v3.2
		* @param None
		* @return None
		*/        
		void FillECellWCDMAInfoL();
		
		/**
		* FillECellGSMInfo Fills the data retrived after E-Cell Id into other struct
		* @since S60  v3.2
		* @param None
		* @return None
		*/        
		void FillECellGSMInfoL();
        
     private:
     
       /**
        * Performs construction of an object
        * @since S60  v3.1u
        * @param aObserver, MOMASuplETelObserver observer for request completion.
        * @param aPhone,RMobilePhone 
        * @return None
        */
        COMASuplETel(MOMASuplETelObserver& aObserver,RMobilePhone& aPhone);

     	/**
        * Performs Second phase constrcution
        * @since S60  v3.1u
        * @param None
        * @return None
        */
        void ConstructL();

    protected:  // CActive        
    	 /**
        * Performs cancelling of asynchronour request
        * @since S60  v3.1u
        * @param None
        * @return None
        */ 
        void DoCancel();

        /**
        * Performs RunL
        * @since S60  v3.1u
        * @param None
        * @return None
        */ 
        void RunL();  
        
        /**
        * Performs RunError
        * @since S60  v3.1u
        * @param aError, Error code containing leave reason
        * @return TInt , informs to activescheduler about handling of leave.
        */ 
        TInt RunError(TInt aError);  
        
    private:     
    
        /**
        * Performs Copies WCDMA data from RCustomAPI to ASN defined WCDMA data structure
        * @since S60  v3.1u
        * @param None
        * @return None
        */ 
    	void CopyWCDMAData();    
        
    private:    // Data
  
      	//For access to a mobile phone
        RMobilePhone& iPhone; 
        
        // For Network information 
        RMobilePhone::TMobilePhoneNetworkInfoV1 iNetworkInfo;
        
        //for network information
        RMobilePhone::TMobilePhoneNetworkInfoV1Pckg iNetworkInfoPkg;
        
     	 // For location information
        RMobilePhone::TMobilePhoneLocationAreaV1 iLocationInfo;

		//Trace Utility
		COMASuplTrace* iTrace;
		
        //Observer for request completion
        MOMASuplETelObserver& iObserver;

		// For type of request... Cell Id to E-CellId
		TCellMode iCellMode;
		
		//For E-cell id
		RMmCustomAPI iCustomAPI;
		
		//For holding E-Cell id
		RMmCustomAPI::TMmCellInfo iMmCellInfo;
		
		//TOMASuplECellInfo mmCellInfo; 
		TOMASuplECellInfo iOMASuplECellInfo;
		
		//TOMASuplWCDMACellinfo suplwcdmaCellInfo; 
		TOMASuplWCDMACellinfo iOMASuplwcdmaCellInfo; 
		
		//RMmCustomAPI::TMmCellInfo::TWCDMACellInfo wcdmaCellInfo; 
		RMmCustomAPI::TMmCellInfo::TWCDMACellInfo iCustomWCDMACellInfo; 
		
		//Added because of etel error
		RMmCustomAPI::TMmCellInfoPckg iCellInfoPckg;
		
    };

#endif      // EPOS_COMASUPLETEL_H
            
// End of File
