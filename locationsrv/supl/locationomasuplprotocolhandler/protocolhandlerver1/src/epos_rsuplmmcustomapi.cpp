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



// INCLUDE FILES
#include <etelext.h>
#include <etelmm.h>
#include <s32file.h>

#define _READ_FROM_FILE_

#include "epos_rsuplmmcustomapi.h"

 const TInt KTimeRequest = 2000000;

 const TInt KTA = 100; // 0 to 255
 const TInt KPrimaryScrambilingCode = 200; // 0 to 511
 const TInt KCarrierRSSI = 100; //0 to 127

 const TInt KMeasuredCID			= 500; 	 // 0 to 268435455

 const TInt KPrimaryCPICH = 125;  //0 to 127
 const TInt KCpichEcN0 = 50;		 // 0 to 63, max=49      
 const TInt KCichRscp = 125; 	 // 0 to 127, max=91      
 const TInt KPathloss = 100; 	 // 6 to 173, max=58
 const TInt KCellParamID = 125;	 // 0 to 127
 const TInt KProposedTGSN = 10;	 // 0 to 14
 const TInt KPrimaryCcpchRscp = 100; // 0 to 127
 const TInt KFddUL = 12000;			// 0 to 16383	
 const TInt KFddDL = 14000;			// 0 to 16383 	
 const TInt KTddDt = 15000;			// 0 to 16383	
 	
_LIT(KMyFile,"c:\\logs\\epos\\input.txt");
 	   		
 RMmCustomAPI::RMmCustomAPI()
    {
    	
    }
    
RMmCustomAPI::TMmCellInfo::TMmCellInfo()
	{
	}

RMmCustomAPI::TMmCellInfo::~TMmCellInfo()
	{
	}

 void RMmCustomAPI::ConstructL()
    {
    	iFileError  = iFs.Connect();
    	iRTimer.CreateLocal();

    	iSUPLNwMode = ESUPLWCDMA;
    	#ifdef _READ_FROM_FILE_
    		iSUPLNwMode = (TSUPLNwMode)GetParameterValue(1);
    	#endif
    	
    	iInitialTNmr = 0;
		iSUPLFrequecyMode = EFDD;
		iSUPLFrequecyMode = ETDD;
		#ifdef _READ_FROM_FILE_
			iSUPLFrequecyMode = (TSUPLFrequecyMode)GetParameterValue(16);
		#endif	
		iNoOfCells = GetParameterValue(2);
    }

 void RMmCustomAPI::Destruct()
    {
    	iRTimer.Close();
    	iFs.Close();
    }

// -----------------------------------------------------------------------------
// RMmCustomAPI::Open
// This method opens a RMmCustomAPI subsession from RMobilePhone.
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
 TInt RMmCustomAPI::Open( RMobilePhone& /*aPhone*/ )
    {
    	TInt error = KErrNone;
    	TRAP(error,ConstructL());
    	return error;   
    }

// -----------------------------------------------------------------------------
// RMmCustomAPI::Close
// This method closes a RMmCustomAPI subsession
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
 void RMmCustomAPI::Close()
    {
    	Destruct();
    }

 TInt RMmCustomAPI::GetCellInfo ( TMmCellInfo& aCellInfo ) 
	{
		TInt error = KErrNone;
	
		if( iSUPLNwMode == ESUPLGSM )
			{
				FillGSMCellInfo(aCellInfo);
			}
		else
			{
				FillWCDMACellInfo(aCellInfo); 
			}
			
		return error;
	}


 void RMmCustomAPI::GetCellInfo (TRequestStatus& aStatus, TDes8& aCellInfo ) 
	{
	     TInt error = KErrNone;

		 if(error == KErrNone)
		 	{
		 		 TMmCellInfoPckg& infoPkg = static_cast<TMmCellInfoPckg&>(aCellInfo );
		 		 TMmCellInfo& CellInfo = infoPkg();
		 		 GetCellInfo(CellInfo);
				 TTimeIntervalMicroSeconds32 microsec(KTimeRequest);
				 iStatus = &aStatus; 
				 iRTimer.After(*iStatus, microsec); 		
		 	}
	 	else
	 		{
				TRequestStatus* pStat = &aStatus;
				User::RequestComplete(pStat, error);
	 		}
	}


 void RMmCustomAPI::FillGSMCellInfo ( TMmCellInfo& aCellInfo ) 
	{
		aCellInfo.iGsmCellInfo.iTA = KTA;
		#ifdef _READ_FROM_FILE_ 
			aCellInfo.iGsmCellInfo.iTA = GetParameterValue(9);
		#endif
		iInitialTNmr = 0;
		for(TInt i = 0; i < KMaxNmrAmount; i++)
			{
				aCellInfo.iGsmCellInfo.iNmr[i] = GetTNmr();
			}
			
		aCellInfo.iMode = TMmCellInfo::EGSM;	
	}

 RMmCustomAPI::TMmCellInfo::TNmr RMmCustomAPI::GetTNmr()
 	{
 			RMmCustomAPI::TMmCellInfo::TNmr tNmr;
			tNmr.iARFCN  = iInitialTNmr;
			tNmr.iBSIC   = iInitialTNmr;
			tNmr.RxLEV   = iInitialTNmr;
 			iInitialTNmr++;
		#ifdef _READ_FROM_FILE_	
			tNmr.iARFCN  = GetParameterValue(6,iInitialTNmr);
			tNmr.iBSIC   = GetParameterValue(7,iInitialTNmr);
			tNmr.RxLEV   = GetParameterValue(8,iInitialTNmr);
    	#endif	
			return tNmr;				
 	}

 void RMmCustomAPI::FillWCDMACellInfo ( TMmCellInfo& aCellInfo ) 
	{
		TMmCellInfo::TWCDMACellInfo wcdmaCellInfo;
		TMmCellInfo::TFrequencyInfo frequencyInfo;
		FillMeasureRequestList(wcdmaCellInfo);
		FillFrequencyMode(frequencyInfo,1,0); // 0 means Freq info used for wcdmaCellInfo struct

		aCellInfo.iMode = TMmCellInfo::EWCDMA;	
		wcdmaCellInfo.iPrimaryScrambilingCode = KPrimaryScrambilingCode;
		#ifdef _READ_FROM_FILE_
			wcdmaCellInfo.iPrimaryScrambilingCode = GetParameterValue(15); 
		#endif
		wcdmaCellInfo.iFrequencyInfo = frequencyInfo;
		aCellInfo.iWcdmaCellInfo = wcdmaCellInfo;
	}

 void RMmCustomAPI::FillMeasureRequestList(TMmCellInfo::TWCDMACellInfo& aWcdmaCellInfo) 
	{
//		for(TInt i = 0; i < KMaxFrequency; i++)	

		iTotalCellCount = 1;
		iCellMeasuredResults = GetParameterValue(3);
		iCount = 1;
		
		for(TInt i = 0; i < iNoOfCells; i++)	
			{
				aWcdmaCellInfo.iNwkMeasureReport[i] = GetMeasureRequest((i + 1) * iCount);
				iCount++;
			}
	}

 RMmCustomAPI::TMmCellInfo::TNetworkMeasureReport RMmCustomAPI::GetMeasureRequest(TInt /*i*/) 
	{
			TMmCellInfo::TNetworkMeasureReport measureReport;
			TMmCellInfo::TFrequencyInfo frequencyInfo;
			FillFrequencyMode(frequencyInfo,iCount,1);// 1 means Freq info used for MeasuredResults struct
			measureReport.iCarrierRSSI = KCarrierRSSI;
			measureReport.iCarrierRSSI = -1;
			
			#ifdef _READ_FROM_FILE_
                measureReport.iCarrierRSSI = GetParameterValue(20,iCount); 
			#endif
			measureReport.iFrequencyInfo = frequencyInfo;
			FillCellMeasuredResultsList(measureReport);
			return measureReport;
	}

 void RMmCustomAPI::FillCellMeasuredResultsList(TMmCellInfo::TNetworkMeasureReport& aMeasureReport) 
 	{
  	for(TInt i = 0; i < iCellMeasuredResults; i++ )
	 		{
	 			aMeasureReport.iCellMeasuredResult[i] = GetCellMeasuredResults( iTotalCellCount);
	 			iTotalCellCount++;
	 		}
 	}

RMmCustomAPI::TMmCellInfo::TCellMeasuredResult RMmCustomAPI::GetCellMeasuredResults(TInt aReadIndex) 
	{
		TMmCellInfo::TCellMeasuredResult cellMeasuredResult;
		cellMeasuredResult.iCID = KMeasuredCID;
		
		#ifdef _READ_FROM_FILE_
			cellMeasuredResult.iCID = GetParameterValue(21,aReadIndex); 
		#endif
		
		if( iSUPLFrequecyMode == EFDD)
			{
				FillFDDSpecificFrequencyMode(cellMeasuredResult,0,aReadIndex);
				FillTDDSpecificFrequencyMode(cellMeasuredResult,-1,aReadIndex);
			}
		else
			{
				FillTDDSpecificFrequencyMode(cellMeasuredResult,0,aReadIndex);
				FillFDDSpecificFrequencyMode(cellMeasuredResult,-1,aReadIndex);
			}
			
			return cellMeasuredResult;
	}

void RMmCustomAPI::FillFDDSpecificFrequencyMode(TMmCellInfo::TCellMeasuredResult& aCellMeasuredResult,TInt aOption,TInt aReadIndex) 
	{
		TMmCellInfo::TFddInfo fddInfo;
		if(aOption == 0)
			{
				fddInfo.iPrimaryCPICH = KPrimaryCPICH;
				fddInfo.iCpichEcN0 = KCpichEcN0;
				fddInfo.iCichRscp = KCichRscp;
				fddInfo.iPathloss = KPathloss;
		
#ifdef _READ_FROM_FILE_
		fddInfo.iPrimaryCPICH = GetParameterValue(22,aReadIndex);
		fddInfo.iCpichEcN0 = GetParameterValue(23,aReadIndex);
		fddInfo.iCichRscp = GetParameterValue(24,aReadIndex);
		fddInfo.iPathloss = GetParameterValue(25,aReadIndex);
#endif		
			}
		else
			{
				fddInfo.iPrimaryCPICH = -1;
				fddInfo.iCpichEcN0 = -1;
				fddInfo.iCichRscp = -1;
				fddInfo.iPathloss = -1;
			}	
		
		aCellMeasuredResult.iFddInfo = fddInfo;
	}


void RMmCustomAPI::FillTDDSpecificFrequencyMode(TMmCellInfo::TCellMeasuredResult& aCellMeasuredResult,TInt aOption,TInt aReadIndex) 
	{
		TMmCellInfo::TTddInfo tddInfo;
		
		if(aOption == 0)
			{
		   		tddInfo.iCellParamID = KCellParamID;	
		   		tddInfo.iProposedTGSN= KProposedTGSN;	
		   		tddInfo.iPrimaryCcpchRscp = KPrimaryCcpchRscp;
		   		tddInfo.iPathloss = KPathloss; 		
		   		tddInfo.iProposedTGSN= -1;	
		   		tddInfo.iPrimaryCcpchRscp = -1;
		   		tddInfo.iPathloss = -1; 		

#ifdef _READ_FROM_FILE_
   		tddInfo.iCellParamID = GetParameterValue(26,aReadIndex);	
   		tddInfo.iProposedTGSN= GetParameterValue(27,aReadIndex);	
   		tddInfo.iPrimaryCcpchRscp = GetParameterValue(28,aReadIndex);
   		tddInfo.iPathloss = GetParameterValue(29,aReadIndex); 		
#endif
		   		for(TInt i = 0;i < KMaxTimeSlotIscpAmount; i++ )
		   		    {

#ifdef _READ_FROM_FILE_	
		   		    tddInfo.iTimeslotISCP[ i ] = GetParameterValue(30,i+1);
#endif

		   		    }
			}
		else
			{
		   		tddInfo.iCellParamID = -1;	
		   		tddInfo.iProposedTGSN= -1;	
		   		tddInfo.iPrimaryCcpchRscp = -1;
		   		tddInfo.iPathloss = -1; 		
		   		for(TInt i = 0;i < KMaxTimeSlotIscpAmount; i++ )
		   			{
		   				tddInfo.iTimeslotISCP[ i ] = -1;
		   			}
			}	
   			
		aCellMeasuredResult.iTddInfo = tddInfo;
		
	}

void RMmCustomAPI::FillFrequencyMode(TMmCellInfo::TFrequencyInfo& aFrequencyInfo,TInt aUsedFor,TInt aInnerOuter) 
	{
		if( iSUPLFrequecyMode == EFDD)
			{
				aFrequencyInfo.iFddUL = KFddUL;		// 0..16383		In case of fdd( iFddUL, iFddDL),
				aFrequencyInfo.iFddUL = -1;
				aFrequencyInfo.iFddDL = KFddDL;		// 0..16383 	iFddUL is optional while iFddDL is mandatory. 
				
#ifdef _READ_FROM_FILE_				
				if(aInnerOuter == 0)
					{
						aFrequencyInfo.iFddUL = GetParameterValue(12,aUsedFor);
						aFrequencyInfo.iFddDL = GetParameterValue(13,aUsedFor);
					}
				else
					{
						aFrequencyInfo.iFddUL = GetParameterValue(17,aUsedFor);
						aFrequencyInfo.iFddDL = GetParameterValue(18,aUsedFor);
					}	
#endif				
			}
		else
			{
				aFrequencyInfo.iTddNt = KTddDt;		// 0..16383		
				aFrequencyInfo.iFddUL = -1;
				aFrequencyInfo.iFddDL = -1;		
				
#ifdef _READ_FROM_FILE_				
				if(aInnerOuter == 0)
					aFrequencyInfo.iTddNt = GetParameterValue(14,aUsedFor);
				else
					aFrequencyInfo.iTddNt = GetParameterValue(19,aUsedFor);			
#endif				
			}	
	}

void RMmCustomAPI::CancelAsyncRequest()
	{
		iRTimer.Cancel();
	}
 
TInt RMmCustomAPI::GetParameterValue(TInt aLineNumber,TInt aIndex ) 
{
		
		if ( KErrNone != iFileError)
		 	return -1;
		
		RFile fread;	
		TInt lError = fread.Open(iFs,KMyFile,EFileRead); 	
		RFileReadStream rfs(fread);
		
		TChar delmeter(10); 
		TInt ret = -9999;
		TInt err;
		if ( KErrNone == lError )
		    {
		    for(TInt i = 0; i < aLineNumber; i++ )
		    	{
					TRAP(err,rfs.ReadL(iLineBuffer,delmeter));
					if(err != KErrNone)
						{
							rfs.Close();	
							fread.Close();	    
							return -1;
						}
					iLineBuffer.Copy(iLineBuffer.Left(iLineBuffer.Length() - 2 ));
		    	}
		    }
		    
	TBuf8<64> restBuffer;  
	TInt pos = KErrNotFound;
	TChar c(44); // 44 means ',' 
	
	if(aIndex != -1 )
		{
			for(TInt i = 0; i < aIndex; i++ )
				{
					pos = iLineBuffer.Locate(c);    
					if(pos == KErrNotFound)
						break;
					restBuffer.Copy(iLineBuffer.Left(pos ));
					iLineBuffer.Copy(iLineBuffer.Mid( pos + 1));
				}
			if(pos != KErrNotFound)	
				iLineBuffer.Copy(restBuffer);
		}
		
	if(ret == -9999)	    
		{
			pos = iLineBuffer.Locate(c);    
			if(pos != KErrNotFound)
				{
					restBuffer.Copy(iLineBuffer.Left(pos ));
					iLineBuffer.Copy(restBuffer);
				}
			TLex8 input(iLineBuffer);		    
			input.Val(ret);
		}
	
	rfs.Close();	
	fread.Close();	    
	return ret;
	

}	

//  End of File
