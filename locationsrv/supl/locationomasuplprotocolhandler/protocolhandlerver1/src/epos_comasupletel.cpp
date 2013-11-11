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
* Description:   Implementation of COMASuplEtel Class
*  Version     : %version: 5 % << Don't touch! Updated by Synergy at check-out.
*
*/



// INCLUDE FILES
#include "epos_comasupltrace.h"
#include "epos_comasupletel.h"
#include "epos_comasupllocationid.h"

_LIT(KTraceFileName,"SUPL_OMA_SESSION::EPos_COMASuplETel.cpp");

// ============================ MEMBER FUNCTIONS OF COMASuplETel ===============================

// -----------------------------------------------------------------------------
// COMASuplETel::COMASuplETel,constuctor
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//

COMASuplETel::COMASuplETel(MOMASuplETelObserver& aObserver,RMobilePhone& aPhone) 
					    : CActive(EPriorityStandard),
					      iPhone(aPhone),
					      iNetworkInfoPkg( iNetworkInfo),
					      iObserver(aObserver),
					      iCellInfoPckg( iMmCellInfo )
	{
		 CActiveScheduler::Add(this);            
	}

// -----------------------------------------------------------------------------
// COMASuplETel::ConstructL,two phase constuctor
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void COMASuplETel::ConstructL()
    {
        	iTrace = COMASuplTrace::NewL();	
        	iTrace->Trace(_L("COMASuplETel::ConstructL"), KTraceFileName, __LINE__); 
    }

// -----------------------------------------------------------------------------
// COMASuplETel::NewL,two phase constuctor
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
COMASuplETel* COMASuplETel::NewL(MOMASuplETelObserver& aObserver,
								 RMobilePhone& aPhone)
    {
    COMASuplETel* self = new( ELeave ) COMASuplETel(aObserver,aPhone);
    
    CleanupStack::PushL( self );
    self->ConstructL();
    CleanupStack::Pop(self);

    return self;
    }


// -----------------------------------------------------------------------------
// COMASuplETel::DoCancel
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void COMASuplETel::DoCancel()
    {
    	if(iCellMode == EOMASuplCellId || iCellMode == EOMASuplCompareCellId)
    		{
    			iPhone.CancelAsyncRequest( EMobilePhoneGetCurrentNetwork );            
    		}
    	else
    		{
    			#ifdef USE_E_CELL_ID_STUB
    				iCustomAPI.CancelAsyncRequest();
    			#else
    				iPhone.CancelAsyncRequest(ECustomGetCellInfoIPC) ;
    			#endif	
    		}	
    }            

// -----------------------------------------------------------------------------
// COMASuplETel::~COMASuplETel,Destructor
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
COMASuplETel::~COMASuplETel()
    {
    	Cancel();
    	iCustomAPI.Close();
    	delete iTrace;
    }
            
// -----------------------------------------------------------------------------
// COMASuplETel::GetLocationID,For retriving the cell id information
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void COMASuplETel::GetLocationID()
    {
	    iCellMode = EOMASuplCellId;
    iTrace->Trace(_L("COMASuplETel::GetLocationID..."),KTraceFileName, __LINE__); 							
    iPhone.GetCurrentNetwork( iStatus, iNetworkInfoPkg, iLocationInfo );
    SetActive();            
    }  
      
// -----------------------------------------------------------------------------
// COMASuplETel::GetECellID,For retriving the E-cell id information
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void COMASuplETel::GetECellID()      
	{
		iCellMode = EOMASuplECellId; 
		iTrace->Trace(_L("COMASuplETel::GetECellID..."),KTraceFileName, __LINE__); 
		TInt err = iCustomAPI.Open(iPhone);
		if(err == KErrNone)
			{
				iCustomAPI.GetCellInfo(iStatus,iCellInfoPckg );								
				SetActive();            
			}
		else
			{
				TRequestStatus* pStat = &iStatus;
				User::RequestComplete(pStat, KErrNone);//go ahead evenif it fails...
				SetActive();
			}	
	}
	
// -----------------------------------------------------------------------------
// COMASuplETel::GetComparisionLocationID,For retriving the cell id information
//                                        for comparision
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void COMASuplETel::GetComparisionLocationID()
    {
    iCellMode = EOMASuplCompareCellId;
    iTrace->Trace(_L("COMASuplETel::GetLocationID..."),KTraceFileName, __LINE__);                           
    iPhone.GetCurrentNetwork( iStatus, iNetworkInfoPkg, iLocationInfo );
    SetActive();            
    }
// COMASuplETel::RunL
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void COMASuplETel::RunL()
    {
    if (iStatus == KErrNone)
        {       
        	if(iCellMode == EOMASuplCellId || iCellMode == EOMASuplCompareCellId)
        		{
        			HandleLocationIdL();
        		}
        	else
        		{
        			HandleECellIdL();
        		}
        }
    else
        {
        	if(iCellMode == EOMASuplCellId)
	    		{
					// Error while processing
					iTrace->Trace(_L("Error in getting Location Id"),KTraceFileName, __LINE__); 							
					iObserver.LocationIDRequestCompletedL(NULL,iStatus.Int());    
	    		}
        	else if(iCellMode == EOMASuplCompareCellId)
        	    {
            	    // Error while processing
            	    iTrace->Trace(_L("Error in getting Comparision Location Id"),KTraceFileName, __LINE__);                             
            	    iObserver.ComparisionLocationIDRequestCompletedL(NULL,iStatus.Int());
            	}
	    	else
	    		{
					// Error while processing
					iTrace->Trace(_L("Error in getting ECell Id"),KTraceFileName, __LINE__); 							
					iObserver.LocationECellIdRequestCompletedL(iOMASuplECellInfo,iStatus.Int());    
	    		}	
        }                
    }

// -----------------------------------------------------------------------------
// COMASuplETel::HandleLocationIdL(),
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void COMASuplETel::HandleLocationIdL()
	{
         switch (iNetworkInfo.iMode) 
            {
            case RMobilePhone::ENetworkModeGsm:
                {
            //coverity[alloc_fn]
            //coverity[assign]
                // For GSM Cell Information
                COMASuplLocationId* locationId = COMASuplLocationId::NewL(); 
            //coverity[push]
            CleanupStack::PushL(locationId);
            COMASuplGSMCellInfo* gsmCellInfo = COMASuplGSMCellInfo::NewL();
            CleanupStack::PushL(gsmCellInfo);
            COMASuplLocationId::TOMASuplStatus lStatus =
                    COMASuplLocationId::ECurrent;

            TInt MCC = 0;
            TInt64 MNC = 0;
            TUint LAC, CellId;
            TLex LexMCC(iNetworkInfo.iCountryCode);
            TLex LexMNC(iNetworkInfo.iNetworkId);

                LexMCC.Val(MCC);
                LexMNC.Val(MNC);
                LAC = iLocationInfo.iLocationAreaCode;
                CellId = iLocationInfo.iCellId;

            gsmCellInfo->SetSuplGSMCellInfo(MNC, MCC, CellId, LAC);
            locationId->SetSuplLocationId(gsmCellInfo, lStatus);
            CleanupStack::Pop(gsmCellInfo);
            //coverity[pop]
            CleanupStack::Pop(locationId);

#if defined(_DEBUG)  //This is only for logging purpose. 
            TBuf<128> msg;

            msg.Copy(_L("It is GSM Network. MCC = "));
            msg.AppendNum(MCC);

            msg.Append(_L(" MNC = "));
            msg.AppendNum(MNC);


				msg.Append(_L(" CellId = "));					
				msg.AppendNum(CellId);
				

				msg.Append(_L(" LAC = "));					
				msg.AppendNum(LAC);
				
				iTrace->Trace(msg,KTraceFileName, __LINE__); 							
        		#endif
        		
				if(iCellMode == EOMASuplCellId)
				    iObserver.LocationIDRequestCompletedL(locationId,KErrNone);
				else if(iCellMode == EOMASuplCompareCellId)
				    iObserver.ComparisionLocationIDRequestCompletedL(locationId,KErrNone);
            //coverity[memory_leak]
                break;                             
                }                    
            case RMobilePhone::ENetworkModeWcdma:
                {
            //coverity[alloc_fn]
            //coverity[assign]
                // For WCDMA  Cell Information
                COMASuplLocationId* locationId = COMASuplLocationId::NewL(); 
            //coverity[push]
            CleanupStack::PushL(locationId);
            
            COMASuplCellInfo* wcdmaCellInfo = COMASuplCellInfo::NewL();
            CleanupStack::PushL(wcdmaCellInfo);

            COMASuplLocationId::TOMASuplStatus lStatus =
                    COMASuplLocationId::ECurrent;
            TInt MCC = 0;
            TInt64 MNC = 0;
            TUint UC;
            TLex LexMCC(iNetworkInfo.iCountryCode);
            TLex LexMNC(iNetworkInfo.iNetworkId);

                LexMCC.Val(MCC);
                LexMNC.Val(MNC);
                UC = iLocationInfo.iCellId;

                                
                wcdmaCellInfo->SetSuplCellInfo(MNC, MCC, UC);                
                locationId->SetSuplLocationId(wcdmaCellInfo,lStatus);
                CleanupStack::Pop(wcdmaCellInfo);
            //coverity[pop]
                CleanupStack::Pop(locationId);
                
           		#if defined(_DEBUG)  //This is only for logging purpose. 
				TBuf<128> msg;
				
				msg.Copy(_L("It is CDMA Network. MCC = "));					
				msg.AppendNum(MCC);
				
				msg.Append(_L(" MNC = "));					
				msg.AppendNum(MNC);

            msg.Append(_L(" UC = "));
            msg.AppendNum(UC);

            iTrace->Trace(msg, KTraceFileName, __LINE__);
#endif

				if(iCellMode == EOMASuplCellId)
				    iObserver.LocationIDRequestCompletedL(locationId,KErrNone);
				else if(iCellMode == EOMASuplCompareCellId)
				    iObserver.ComparisionLocationIDRequestCompletedL(locationId,KErrNone);
            //coverity[memory_leak]
                break;                             
                }                    
            default:            
            	{
            	#if _DEBUG 
            //coverity[alloc_fn]
            //coverity[assign]            
	            	// For GSM Cell Information
	            	COMASuplLocationId* locationId = COMASuplLocationId::NewL(); 
            //coverity[push]
            CleanupStack::PushL(locationId);
            
            COMASuplGSMCellInfo* gsmCellInfo = COMASuplGSMCellInfo::NewL();
            CleanupStack::PushL(gsmCellInfo);

            COMASuplLocationId::TOMASuplStatus lStatus = COMASuplLocationId::ECurrent;
            TInt MCC=0;
            TInt64 MNC=0;
            TUint LAC, CellId;
            TLex LexMCC(iNetworkInfo.iCountryCode);
            TLex LexMNC(iNetworkInfo.iNetworkId);

	                LexMCC.Val(MCC);
	                LexMNC.Val(MNC);
	                LAC = iLocationInfo.iLocationAreaCode;
	                CellId = iLocationInfo.iCellId;
	                
                
	           		gsmCellInfo->SetSuplGSMCellInfo(MNC, MCC,CellId,LAC);                
	                locationId->SetSuplLocationId(gsmCellInfo,lStatus);
	                CleanupStack::Pop(gsmCellInfo);
            //coverity[pop]
            CleanupStack::Pop(locationId);
                	
					#if defined(_DEBUG)  //This is only for logging purpose. 
					TBuf<128> msg;
					msg.Copy(_L("It is GSM Network. MCC = "));					
					msg.AppendNum(MCC);

					msg.Append(_L(" MNC = "));					
					msg.AppendNum(MNC);

					msg.Append(_L(" CellId = "));					
					msg.AppendNum(CellId);

					msg.Append(_L(" LAC = "));					
					msg.AppendNum(LAC);

					iTrace->Trace(msg,KTraceFileName, __LINE__); 							
					
					#endif
					if(iCellMode == EOMASuplCellId)
					    iObserver.LocationIDRequestCompletedL(locationId,KErrNone); 
					else if(iCellMode == EOMASuplCompareCellId)
					    iObserver.ComparisionLocationIDRequestCompletedL(locationId,KErrNone);					
                #else
                	iTrace->Trace(_L("Error in getting Location Id"),KTraceFileName, __LINE__); 							
                	if(iCellMode == EOMASuplCellId)
                	    iObserver.LocationIDRequestCompletedL(NULL,KErrNotSupported); 
                	else if(iCellMode == EOMASuplCompareCellId)
                	    iObserver.ComparisionLocationIDRequestCompletedL(NULL,KErrNotSupported);                 	
                #endif
            //coverity[memory_leak]
            break;
            	}                     
            }
		
	}

// -----------------------------------------------------------------------------
// COMASuplETel::HandleECellIdL(),
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void COMASuplETel::HandleECellIdL()
	{
	
		switch(iMmCellInfo.iMode)
			{
				case RMmCustomAPI::TMmCellInfo::EGSM:
					{
						FillECellGSMInfoL();
						break;	
					}
					
				case RMmCustomAPI::TMmCellInfo::EWCDMA:
					{
						FillECellWCDMAInfoL();
						break;	
					}
					
				default:break;

			}
	}
	
// -----------------------------------------------------------------------------
// COMASuplETel::RunError, Will be called by framework,if RunL leaves.
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
TInt COMASuplETel::RunError(TInt aError)
	{
		if(iCellMode == EOMASuplCellId)
		{
			// Error while processing
			iTrace->Trace(_L("RunL leaved for Cell id"),KTraceFileName, __LINE__); 							
			TRAP_IGNORE(iObserver.LocationIDRequestCompletedL(NULL,aError));    
		}
		else
		{

			// Error while processing
			iTrace->Trace(_L("RunL leaved for E - cell id"),KTraceFileName, __LINE__); 							
			TRAP_IGNORE(iObserver.LocationECellIdRequestCompletedL(iOMASuplECellInfo,aError));    
		}	
		
	 return KErrNone;
	}

// -----------------------------------------------------------------------------
// COMASuplETel::FillECellWCDMAInfoL, 
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void COMASuplETel::FillECellWCDMAInfoL()
	{
		iCustomWCDMACellInfo = iMmCellInfo.iWcdmaCellInfo;
		//memcpy(&iOMASuplwcdmaCellInfo,&iCustomWCDMACellInfo,sizeof(iCustomWCDMACellInfo)); Manish
		CopyWCDMAData();
		iOMASuplECellInfo.SetWCDMACellInfo(iOMASuplwcdmaCellInfo);
		iTrace->Trace(_L("Filled WCDMA Struct for E-CellId"),KTraceFileName, __LINE__); 							
		iObserver.LocationECellIdRequestCompletedL(iOMASuplECellInfo,KErrNone);    
	}
	
// -----------------------------------------------------------------------------
// COMASuplETel::FillECellGSMInfoL, 
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void COMASuplETel::FillECellGSMInfoL()
	{
		RMmCustomAPI::TMmCellInfo::TGSMCellInfo  gsmCellInfo = iMmCellInfo.iGsmCellInfo;
		TOMASuplGSMCellinfo suplgsmCellInfo; //Make sure both struct are same.....
		memcpy(&suplgsmCellInfo,&gsmCellInfo,sizeof(gsmCellInfo));

		iOMASuplECellInfo.SetGSMCellInfo(suplgsmCellInfo);
		iTrace->Trace(_L("Filled GSM Struct for E-CellId"),KTraceFileName, __LINE__); 							
		iObserver.LocationECellIdRequestCompletedL(iOMASuplECellInfo,KErrNone);    
	}

// -----------------------------------------------------------------------------
// COMASuplETel::CopyWCDMAData, 
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void COMASuplETel::CopyWCDMAData()
	{

	 if(KFddDLMin <= iCustomWCDMACellInfo.iFrequencyInfo.iFddDL && iCustomWCDMACellInfo.iFrequencyInfo.iFddDL <= KFddDLMax)
	 	{
	 		iOMASuplwcdmaCellInfo.iFrequencyInfo.iFddDL =  iCustomWCDMACellInfo.iFrequencyInfo.iFddDL;
	 	}
	 else
	 	{
	 		iOMASuplwcdmaCellInfo.iFrequencyInfo.iFddDL = -1;
	 	}	

	 
	 if(KFddULMin <= iCustomWCDMACellInfo.iFrequencyInfo.iFddUL && iCustomWCDMACellInfo.iFrequencyInfo.iFddUL <= KFddULMax)
	 	{
	 		iOMASuplwcdmaCellInfo.iFrequencyInfo.iFddUL =  iCustomWCDMACellInfo.iFrequencyInfo.iFddUL;
	 	}
	 else
	 	{
	 		iOMASuplwcdmaCellInfo.iFrequencyInfo.iFddUL = -1;
	 	}
	 	

	 if(KTddDtMin <= iCustomWCDMACellInfo.iFrequencyInfo.iTddNt && iCustomWCDMACellInfo.iFrequencyInfo.iTddNt <= KTddDtMax)
	 	{
	 		iOMASuplwcdmaCellInfo.iFrequencyInfo.iTddNt =  iCustomWCDMACellInfo.iFrequencyInfo.iTddNt;
	 	}
	 else
	 	{
	 		iOMASuplwcdmaCellInfo.iFrequencyInfo.iTddNt = -1;
	 	}	
	 
	 
	 iOMASuplwcdmaCellInfo.iPrimaryScrambilingCode = iCustomWCDMACellInfo.iPrimaryScrambilingCode;
	 
	 for(TInt i = 0; i < KOMASUPLMaxNetworkMeasureReports; i++ )
	 	{

			iOMASuplwcdmaCellInfo.iNwkMeasureReport[i].iFrequencyInfo.iFddDL =  -1;
			iOMASuplwcdmaCellInfo.iNwkMeasureReport[i].iFrequencyInfo.iFddUL =  -1;
			iOMASuplwcdmaCellInfo.iNwkMeasureReport[i].iFrequencyInfo.iTddNt =  -1;
			
			if(KFddDLMin <= iCustomWCDMACellInfo.iNwkMeasureReport[i].iFrequencyInfo.iFddDL  && iCustomWCDMACellInfo.iNwkMeasureReport[i].iFrequencyInfo.iFddDL <= KFddDLMax)
				{
					iOMASuplwcdmaCellInfo.iNwkMeasureReport[i].iFrequencyInfo.iFddDL =  iCustomWCDMACellInfo.iNwkMeasureReport[i].iFrequencyInfo.iFddDL;
				}
			
			if(KFddULMin <= iCustomWCDMACellInfo.iNwkMeasureReport[i].iFrequencyInfo.iFddUL && iCustomWCDMACellInfo.iNwkMeasureReport[i].iFrequencyInfo.iFddUL <= KFddULMax)
				{
					iOMASuplwcdmaCellInfo.iNwkMeasureReport[i].iFrequencyInfo.iFddUL =  iCustomWCDMACellInfo.iNwkMeasureReport[i].iFrequencyInfo.iFddUL;
				}

	 		if(KTddDtMin <= iCustomWCDMACellInfo.iNwkMeasureReport[i].iFrequencyInfo.iTddNt && iCustomWCDMACellInfo.iNwkMeasureReport[i].iFrequencyInfo.iTddNt <= KTddDtMax)
	 			{
	 				iOMASuplwcdmaCellInfo.iNwkMeasureReport[i].iFrequencyInfo.iTddNt =  iCustomWCDMACellInfo.iNwkMeasureReport[i].iFrequencyInfo.iTddNt;	
	 			}
	 			
	 		iOMASuplwcdmaCellInfo.iNwkMeasureReport[i].iCarrierRSSI = iCustomWCDMACellInfo.iNwkMeasureReport[i].iCarrierRSSI;
	 		
	 		for(TInt j = 0; j < KOMASUPLMaxCellMeasuredResults; j++ )
	 			{
	 				iOMASuplwcdmaCellInfo.iNwkMeasureReport[i].iCellMeasuredResult[j].iCID = iCustomWCDMACellInfo.iNwkMeasureReport[i].iCellMeasuredResult[j].iCID ;
	 				iOMASuplwcdmaCellInfo.iNwkMeasureReport[i].iCellMeasuredResult[j].iFddInfo.iPrimaryCPICH = iCustomWCDMACellInfo.iNwkMeasureReport[i].iCellMeasuredResult[j].iFddInfo.iPrimaryCPICH;
	 				iOMASuplwcdmaCellInfo.iNwkMeasureReport[i].iCellMeasuredResult[j].iFddInfo.iCpichEcN0 = iCustomWCDMACellInfo.iNwkMeasureReport[i].iCellMeasuredResult[j].iFddInfo.iCpichEcN0;
	 				iOMASuplwcdmaCellInfo.iNwkMeasureReport[i].iCellMeasuredResult[j].iFddInfo.iCichRscp = iCustomWCDMACellInfo.iNwkMeasureReport[i].iCellMeasuredResult[j].iFddInfo.iCpichRscp;
	 				iOMASuplwcdmaCellInfo.iNwkMeasureReport[i].iCellMeasuredResult[j].iFddInfo.iPathloss = iCustomWCDMACellInfo.iNwkMeasureReport[i].iCellMeasuredResult[j].iFddInfo.iPathloss;
	 				
	 				iOMASuplwcdmaCellInfo.iNwkMeasureReport[i].iCellMeasuredResult[j].iTddInfo.iCellParamID = iCustomWCDMACellInfo.iNwkMeasureReport[i].iCellMeasuredResult[j].iTddInfo.iCellParamID; 
	 				iOMASuplwcdmaCellInfo.iNwkMeasureReport[i].iCellMeasuredResult[j].iTddInfo.iProposedTGSN = iCustomWCDMACellInfo.iNwkMeasureReport[i].iCellMeasuredResult[j].iTddInfo.iProposedTGSN;
	 				iOMASuplwcdmaCellInfo.iNwkMeasureReport[i].iCellMeasuredResult[j].iTddInfo.iPrimaryCcpchRscp = iCustomWCDMACellInfo.iNwkMeasureReport[i].iCellMeasuredResult[j].iTddInfo.iPrimaryCcpchRscp;
	 				iOMASuplwcdmaCellInfo.iNwkMeasureReport[i].iCellMeasuredResult[j].iTddInfo.iPathloss = iCustomWCDMACellInfo.iNwkMeasureReport[i].iCellMeasuredResult[j].iTddInfo.iPathloss;
	 				
						for(TInt k = 0; k < KOMASUPLMaxTimeSlotIscpAmount; k++ )	 				
							{
								iOMASuplwcdmaCellInfo.iNwkMeasureReport[i].iCellMeasuredResult[j].iTddInfo.iTimeslotISCP[k] = iCustomWCDMACellInfo.iNwkMeasureReport[i].iCellMeasuredResult[j].iTddInfo.iTimeslotISCP[k];
							}
	 					
	 			}
	 		
	 	}
	 
	}
//  End of File  
