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
* Description:   Class for handling fallback in case of multiple SLP
*
*/



#ifndef C_COMASUPLFALLBACKHANDLER_H
#define C_COMASUPLFALLBACKHANDLER_H

#include <e32base.h>

#include "epos_csuplconnection.h"
#include "epos_csuplsettingsinternal.h"
#include "epos_omasuplconstants.h"
#include "epos_comasuplconnrequestor.h"

class CSuplSettingsInternal;
class COMASuplTrace;

/**
 *  Class for Handling Fallback...
 *
 *  @lib eposomasuplprotocolhandler.lib
 *  @since S60 v3.1
 */
 
struct TSLPDetails
{
	TUint64 iSLPId;
	TBuf<KHSLPAddressLength> iHSLPAddress;
	TBuf<KHSLPAddressLength>  iIapName;
	TBool iIsIAPDialgShown;
	TBool iTls;
	TBool iPskTls;
	TBool iHomeNetworkUsage;   //True means usage is allowed in home network
};



 
class COMASuplFallBackHandler : public CBase
{


	public :  // Constructor 
	
		/**
		* NewL Method.
		* @since Series 60 3.1
		* @param MEventCompletionObserver
		* @return Instance of COMASuplFallBackHandler
		*/ 
		static COMASuplFallBackHandler* NewL(CSuplSettingsInternal& aSettings);
		
		/**
		* Destructor.
		*/
		~COMASuplFallBackHandler();
		
	public :		
	
		/**
		* CreateConnection Method. Creates the Socket required to
		* communicate with the SLP
		* @since Series 60 3.1
		* @param None
		* @return None
		*/ 
		TInt GetNextSLPAddressL(TInt64& aSLPId,TDes& aHslpToBeUsedAddress,TDes& aIAPName,TBool& aTls,
		                        TBool& aPskTls,TInt aLastErrorCode,TBool& aIsIapDialogShown);
		
		void SetFallBackParametersL(const TDesC& aClientProvidedHSLPAddress,TBool aFallBackAllowed);
		
		void ReadSLPListForNIL(const TDesC& aClientProvidedHSLPAddress);
		
		void UpdateSLPListForHomeUsage(TBool aHomeNetwork);
		
		
	private : 
	
		//Constuctor
		COMASuplFallBackHandler(CSuplSettingsInternal& aSettings);
		
		/**
		* By default Symbian 2nd phase constructor is private.
		*/
		void ConstructL();
		
    private:
    
    void ReadSLPListL(const TDesC& aClientProvidedHSLPAddress);	
    TBool CheckErrorCodeL(TInt aLastErrorCode);	
    void GenerateHslpAddressFromIMSIL();
    void PrintSLPList();
    void GetFirstEnabledSLPL();
		
  	protected :  // Functions from CActive
      /**
      * From CActive 
      * To handle request completion
      */
      void RunL();
      
      /**
      * From CActive 
      * To cancel the asynchronous requests
      */
   		void DoCancel();  
      /**
      * From CActive 
      * To handle errors in asynchronous requests
      */
   		TInt RunError(TInt aError);
     
	private : //Data
	

	//SUPLSettings
	CSuplSettingsInternal& iSuplSettings;

	//Trace Utility
	COMASuplTrace* iTrace;

	TInt iCurrentServerCounter;

      //TBuf<KMaxHSLPLength> iClientProvidedHSLPAddress;
      
      TBool iIsFallBackAllowed;
      
      CArrayFixFlat<TSLPDetails>* iSLPList;
      
      TBool iTLSAuthenticationFailed;
      
      RBuf iGenratedHslpAddress;
      
  	    RBuf iDefaultIAPName;
		
      TInt iErrorCode;
      
      TTime iFallBackStartTime;
      
      TInt iAllowedFallBackTimerValue;
	  TBool iAttemptedBackupServer;
      
};

#endif //C_COMASUPLFALLBACKHANDLER_H
