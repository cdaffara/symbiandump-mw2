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
* Description:  Header file for qop field in message
*
*/

#ifndef T_TOMASUPLECELLINFO_H
#define T_TOMASUPLECELLINFO_H

// INCLUDES
#include <e32def.h>
#include <e32base.h>

#include "epos_suplconstants.h"

	struct TOMASuplFrequencyInfo
		{
			TInt	iFddUL;		// 0..16383		In case of fdd( iFddUL, iFddDL),
			TInt	iFddDL;		// 0..16383 	iFddUL is optional while iFddDL is mandatory. 
			TInt	iTddNt;		// 0..16383		
		};

	struct TOMASuplFddInfo
 		{
			TInt16				iPrimaryCPICH;	//(0..127)
			TInt8				iCpichEcN0;		//(0..63), max=49      
			TInt8				iCichRscp; 		//(0..127, max=91      
			TInt				iPathloss; 		//(46..173), max=58
 		};
   		
	struct TOMASuplTddInfo
 		{
	 		TInt8				iCellParamID;	//(0..127)
	 		TInt8				iProposedTGSN;	//(0..14)
	 		TInt8				iPrimaryCcpchRscp;//(0..127)
	 		TInt				iPathloss; 		//(46..173), max=58
	 		TInt8				iTimeslotISCP[ KOMASUPLMaxTimeSlotIscpAmount ];//(0..127)), max. 91
 		};
   
	struct TOMASuplCellMeasuredResult
   		{
   		TInt32 					iCID;	//cellIdentity (0..268435455)
   		TOMASuplFddInfo			iFddInfo;
   		TOMASuplTddInfo			iTddInfo;
   		};


	struct TOMASuplNetworkMeasureReport
		{
		TOMASuplFrequencyInfo		iFrequencyInfo;
		TInt8						iCarrierRSSI;	//	UTRA-CarrierRSSI (0..127), max=76
		TOMASuplCellMeasuredResult	iCellMeasuredResult[ KOMASUPLMaxCellMeasuredResults ];//1..32
		};
		
	struct TOMASuplNmr
		{
		TInt iARFCN;
		TInt iBSIC;
		TInt iRxLEV;
		};
		
   	struct TOMASuplGSMCellinfo
		{
		TOMASuplNmr 	iNmr[ KOMASUPLMaxNmrAmount ];												
		TInt16  		iTA;	// Timing advance(0..255)							
		};
		
	struct TOMASuplWCDMACellinfo
 		{
   		TOMASuplFrequencyInfo			iFrequencyInfo;	
		TInt				 			iPrimaryScrambilingCode;	//0..511 
		TOMASuplNetworkMeasureReport 	iNwkMeasureReport[ KOMASUPLMaxNetworkMeasureReports ];	//1..8
		};
		
	enum TOMASuplNwMode
		{
		EOMASuplUnknown,
		EOMASuplGSM,
		EOMASuplWCDMA					
		};

/**
 *  Class for holding E-Cell id 
 *  @lib eposomasuplasnconverter.lib
 *  @since S60 S60_3.2
 */
 
class TOMASuplECellInfo 
 {
 
public: 
 
        /**
        * Performs construction of an object
        * @since Series 60 3.2
        * @param None
        * @return Returns instance of TOMASuplECellInfo
        */
     	IMPORT_C TOMASuplECellInfo();
     	
public:      	// Setter method
        /**
        * Setter method for WCDMA in E-CellID
        * @since Series 60 3.2
        * @aTOMASuplWCDMACellinfo, TOMASuplWCDMACellinfo
        * @return None											 
        */				
     	IMPORT_C void SetWCDMACellInfo(TOMASuplWCDMACellinfo&  aTOMASuplWCDMACellinfo); 
     	
       /**
        * Setter method for GSM in E-CellID
        * @since Series 60 3.2
        * @aTOMASuplGSMCellinfo, TOMASuplGSMCellinfo
        * @return None											 
        */				
    	IMPORT_C void SetGSMCellInfo(TOMASuplGSMCellinfo& aTOMASuplGSMCellinfo); 
    	
public:      	// Getter method    	

       /**
        * Getter method for WCDMA in E-CellID
        * @since Series 60 3.2
        * @aTOMASuplWCDMACellinfo, TOMASuplWCDMACellinfo, Fills WCDMA info
        * @return None											 
        */				
     	IMPORT_C void WCDMACellInfo(TOMASuplWCDMACellinfo&  aTOMASuplWCDMACellinfo) const; 
     	
       /**
        * Getter method for GSM in E-CellID
        * @since Series 60 3.2
        * @aTOMASuplGSMCellinfo, TOMASuplGSMCellinfo, Fills GSM info
        * @return None											 
        */				
     	IMPORT_C void GSMCellInfo(TOMASuplGSMCellinfo& aTOMASuplGSMCellinfo) const; 
     	
       /**
        * Getter method for NetworkMode
        * @since Series 60 3.2
        * @None
        * @return TNwMode, returns type of network used , GSM OR WCDMA
        */				
     	IMPORT_C TOMASuplNwMode NetworkMode() const; 
        											 
private:  // Data
	        
        	TOMASuplNwMode	iOMASuplNwMode;// used for identifying mode. 
        	TOMASuplGSMCellinfo iOMASuplGsmCellInfo;
        	TOMASuplWCDMACellinfo iOMASuplWcdmaCellInfo;      
        	
   };


#endif // T_TOMASUPLECELLINFO_H