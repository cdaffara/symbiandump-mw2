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
* Description:   RMmCustomAPI contains multimode API's vendor specific
*                functionality.
*
*/


#ifndef RMMCUSTOMAPI_H
#define RMMCUSTOMAPI_H

//  INCLUDES

#include <etelmm.h>
#include <et_clsvr.h>
#include <etelpckt.h>
#include <e32cmn.h> 
#include <f32file.h> 

// Cell info ant
const TInt KMaxTimeSlotIscpAmount		= 14;
const TInt KMaxCellMeasuredResults		= 32;
const TInt KMaxNetworkMeasureReports	= 8;
const TInt KMaxNmrAmount				= 15;

// CLASS DECLARATION

/**
*  RMmCustomAPI contains multimode API's vendor specific functionality.
*  RMmCustomAPI is a subsession extension i.e. it derives 
*  from RtelSubSessionBase class.

*  Notice that Custom TSY is able to handle only one instance of each Custom API
*  Set-method call at a time. Overlapping Set-method calls occurs if same 
*  Set-method is called from an other instance of  RMmCustomAPI object. Through
*  one RMmCustomAPI object overlapping calls are not allowed - trying this will
*  panic Etel Server. For each overlapping request to the client is returned 
*  error code KErrServerBusy either as a return value in case of synchronous
*  call or in a variable of type TRequestStatus in case of an asynchronous call.
*  This rule applies also in a situation where previous identical request has 
*  been cancelled by client but the triggered operation on DOS side has not been 
*  completed yet.
*
*  On the other hand Custom TSY is able to handle several instances of each
*  Get-method call at a time. Again overlapping Get-method call can be activated
*  only from an other instance of  RMmCustomAPI object.
*
*  Consider carefully consequences and implications of request cancellations.
*  If a request has triggered an operation on DOS side, the subsequent
*  calcellation of the request will not undo the operation on DOS side. It is
*  the responsibility of the Custom API's client to undo the operation if
*  necessary. Cancel request completes only when awaited DOS side response
*  arrives. Then if it is an OK-response, the request is completed normally with
*  error code KErrNone. . Or if the response is a FAILURE, the request is
*  completed with error code KErrCancel. 
*
*  APDU functionality:
*  Etel Server and Custom TSY are able to handle multiple SendAPDUReq calls
*  simultaneously. 
*  However each RMmCustoAPI instance can have only one request active at a time.
*
*  Packet data related functionality:
*  Custom TSY implements functionality that is not supported by the packet API.
*
*  Call realted functionality:
*  Custom TSY implements functionality that is not supported by the MM ETEL API.
*
*  @lib CustomAPI.lib
*  @since Series 60 2.0
*/
enum TSUPLNwMode
	{
		ESUPLGSM = 0,
		ESUPLWCDMA					
	};

enum TSUPLFrequecyMode
	{
		EFDD,
		ETDD
	};

class RMmCustomAPI //: public RTelSubSessionBase
    {
    public:

        /**
        *   C++ default ructor.
        */
         RMmCustomAPI();

public:    

	class TMmCellInfo
	        {
	        public:
	   		struct TFrequencyInfo
		   		{
				TInt	iFddUL;		// 0..16383		In case of fdd( iFddUL, iFddDL),
				TInt	iFddDL;		// 0..16383 	iFddUL is optional while iFddDL is mandatory. 
				TInt	iTddNt;		// 0..16383		
		   		};
	   
	   		struct TFddInfo
		   		{
				TInt16				iPrimaryCPICH;	//(0..127)
				TInt8				iCpichEcN0;		//(0..63), max=49      
				TInt8				iCichRscp; 		//(0..127, max=91      
				TInt				iPathloss; 		//(46..173), max=58
		   		};
		   		
	   		struct TTddInfo
		   		{
		   		TInt8				iCellParamID;	//(0..127)
		   		TInt8				iProposedTGSN;	//(0..14)
		   		TInt8				iPrimaryCcpchRscp;//(0..127)
		   		TInt				iPathloss; 		//(46..173), max=58
		   		TInt8				iTimeslotISCP[ KMaxTimeSlotIscpAmount ];//(0..127)), max. 91
		   		};
		   
	   		struct TCellMeasuredResult
		   		{
		   		TInt32				iCID;	//cellIdentity (0..268435455)
		   		TFddInfo			iFddInfo;
		   		TTddInfo			iTddInfo;
		   		};
	   
			struct TNetworkMeasureReport
				{
				TFrequencyInfo		iFrequencyInfo;
				TInt8				iCarrierRSSI;	//	UTRA-CarrierRSSI (0..127), max=76
				TCellMeasuredResult	iCellMeasuredResult[ KMaxCellMeasuredResults ];//1..32
				};
				
			struct TNmr
				{
				TInt iARFCN;
				TInt iBSIC;   
				TInt RxLEV;
				};
				
		   	struct TGSMCellInfo
				{
				TNmr 		iNmr[ KMaxNmrAmount ];												
				TInt16  	iTA;	// Timing advance(0..255)							
				};
				
	   		struct TWCDMACellInfo
		   		{
		   		TFrequencyInfo			iFrequencyInfo;	
				TInt				 	iPrimaryScrambilingCode;	//0..511 
				TNetworkMeasureReport 	iNwkMeasureReport[ KMaxNetworkMeasureReports ];	//1..8
				};	
			
            enum TNwMode
                {
                EUnknown,
                EGSM,
                EWCDMA                    
                };
				
			public: //Methods			   	
	            
	            // ructor
	           
	             TMmCellInfo();
	             ~TMmCellInfo();
	             
	        
	        public:  // Data
	        
                /*
                * Network mode
                */
                TNwMode iMode; // used for identifying mode. 
            
                /*
                * GSM cell info
                */
                TGSMCellInfo iGsmCellInfo;
            
                /*
                * WCDMA cell info
                */
                TWCDMACellInfo iWcdmaCellInfo;  
                
	        };
	        
	       typedef TPckg<TMmCellInfo> TMmCellInfoPckg;
    public: // New functions

        /**
        * Opens a subsession.
        * @since Series 60 2.0
        * @param aPhone reference to a Mobile Phone object
        * @return KErrNone if successfuls
        */
         TInt Open(RMobilePhone& aPhone);

        /**
        * Closes the subsession.
        * @since Series 60 2.0
        */
         void Close();
		
		 TInt GetCellInfo( TMmCellInfo& aCellInfo ) ;
		 
		 void GetCellInfo( TRequestStatus& aStatus, TDes8& aCellInfo  ) ;   
		 
		 void CancelAsyncRequest();     
        
    private:

        /**
        * Copy ructor
        * @since Series 60 2.0
        * @param aCustomAPI Custom API handle 
        */
        RMmCustomAPI(  RMmCustomAPI& aCustomAPI );

    protected:

        /**
        * By default Symbian 2nd phase ructor is private.
        */
         void ConstructL();

        /**
        * Destructor
        */
		void Destruct();
		void FillGSMCellInfo(TMmCellInfo& aCellInfo ) ;
		void FillWCDMACellInfo(TMmCellInfo& aCellInfo ) ;
		TMmCellInfo::TNmr GetTNmr();
		void FillMeasureRequestList(TMmCellInfo::TWCDMACellInfo& aWcdmaCellInfo) ;
		TMmCellInfo::TNetworkMeasureReport GetMeasureRequest(TInt aReadIndex) ;
		void FillCellMeasuredResultsList(TMmCellInfo::TNetworkMeasureReport& aMeasureReport) ;
		TMmCellInfo::TCellMeasuredResult GetCellMeasuredResults(TInt aReadIndex) ;
		void FillFDDSpecificFrequencyMode(TMmCellInfo::TCellMeasuredResult& aCellMeasuredResult,TInt aOption,TInt aReadIndex) ;
		void FillTDDSpecificFrequencyMode(TMmCellInfo::TCellMeasuredResult& aCellMeasuredResult,TInt aOption,TInt aReadIndex) ;
		void FillFrequencyMode(TMmCellInfo::TFrequencyInfo& aFrequencyInfo,TInt aUsedFor,TInt aInnerOuter) ;
		
		TInt GetParameterValue(TInt aLineNumber,TInt aIndex = -1) ;
    private:    // Data
    
    		RTimer iRTimer;
    		
    		TSUPLNwMode iSUPLNwMode;
    		
    		TInt iInitialTNmr; 
    		
    		TSUPLFrequecyMode iSUPLFrequecyMode;
    		
    		TRequestStatus* iStatus;
    		
    		TFileText  iFileText;
    		
    		TBuf8<1500> iLineBuffer;
    		TInt iNoOfCells;
    		TInt iTotalCellCount ;
			TInt iCellMeasuredResults ;
			TInt iCount;
			RFs iFs;
			TInt iFileError ;
			
    };

#endif      // RMMCUSTOMAPI_H


//  End of File
