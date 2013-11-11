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
* Description:  Class definition of Cell data handler 
*
*/


#include <commdb.h>
#include <etel.h>
#include "lbtcelloperation.h"
#include "lbtgeogsmcell.h"
#include "lbtgeowcdmacell.h"
#include "lbtlogger.h"

// -----------------------------------------------------------------------------
// CLbtCellOperation::NewL
// 
// -----------------------------------------------------------------------------
//
CLbtCellOperation* CLbtCellOperation::NewL( TRequestStatus& aStatus, CLbtGeoAreaBase ** aArea )
    {
    CLbtCellOperation* self = new (ELeave) CLbtCellOperation(aStatus, aArea);
    CleanupStack::PushL(self);
    self->ConstructL( );
    CleanupStack::Pop();    
    return self;
    }

// -----------------------------------------------------------------------------
// CLbtCellOperation::CLbtCellOperation
// 
// -----------------------------------------------------------------------------
//
CLbtCellOperation::CLbtCellOperation(TRequestStatus& aStatus, CLbtGeoAreaBase ** aArea):iCommandId(-1),
																						      iNwInfo(),
                                            												  iNwInfoPckg(iNwInfo), 
                                            												  iLocArea(),iCellInfo(), 
                                            												  iCellPckg(iCellInfo),
                                            												  iClientStatus(aStatus),
												                                              iArea(aArea)
    {
    CActiveScheduler::Add( this );
    }

// -----------------------------------------------------------------------------
// CLbtCellOperation::~CLbtCellOperation
// 
// -----------------------------------------------------------------------------
//
CLbtCellOperation::~CLbtCellOperation()
    {
    Cancel();
    iMmApi.Close();
    iMPhone.Close();
    iTelServer.Close();    
    }

// -----------------------------------------------------------------------------
// CLbtCellOperation::ConstructL
// 
// -----------------------------------------------------------------------------
//
void CLbtCellOperation::ConstructL(  )
    {    
    InitialiseEtelL();
    }

// -----------------------------------------------------------------------------
// CLbtCellOperation::Cancel
// 
// -----------------------------------------------------------------------------
//
void CLbtCellOperation::DoCancel()
    {
    if( iCommandId == EMobilePhoneGetCurrentNetwork )
        {
        iMPhone.CancelAsyncRequest( iCommandId );
        }
    else if( iCommandId == ECustomGetCellInfoIPC )
        {
        iMmApi.CancelAsyncRequest( iCommandId );
        }
    }
    
// -----------------------------------------------------------------------------
// CLbtCellOperation::RunL
// 
// -----------------------------------------------------------------------------
//
void CLbtCellOperation::RunL()
    {
    LBT_TRACE( KLbtLogClientLib|KLbtLogVerbose, __FILE__, __LINE__, "CLbtCellOperation::RunL", iStatus.Int() );
    if( iStatus.Int() == KErrNone )
        {
        if( iCommandId == EMobilePhoneGetCurrentNetwork )
            {
            LBT_TRACE( KLbtLogClientLib|KLbtLogVerbose, __FILE__, __LINE__, "First RunL");
            iMmApi.GetCellInfo(iStatus, iCellPckg);
            iCommandId = ECustomGetCellInfoIPC;
            SetActive();
            return; // TODO: Test code remove it
            }
        else
            {
            LBT_TRACE( KLbtLogClientLib|KLbtLogVerbose, __FILE__, __LINE__, "Second time RunL");
            CLbtGeoCell* geoCell = NULL;
                        
            if(iCellInfo.iMode == RMmCustomAPI::TMmCellInfo::EGSM)
            	{
            	geoCell = CLbtGeoGsmCell::NewL();
            	}
            else
            	{
            	geoCell = CLbtGeoWcdmaCell::NewL();
            	}
            CleanupStack::PushL( geoCell );
            geoCell->SetNetworkType( iNetworkMode );
            geoCell->SetNetworkCountryCode( iNwInfo.iCountryCode );
            geoCell->SetNetworkIdentityCode( iNwInfo.iNetworkId );
            geoCell->SetLocationAreaCode( iLocArea.iLocationAreaCode );
            geoCell->SetCellId( iLocArea.iCellId );
            
            if( iNetworkMode == RMobilePhone::ENetworkModeGsm )
            	{
                CLbtGeoGsmCell* gsmCell = static_cast<CLbtGeoGsmCell*>(geoCell);
                RArray<TInt> bsicArray;                
                for( TInt i=0;i<15;i++ ) // 15 is  max available Nmr value
                    {
                    if( iCellInfo.iGsmCellInfo.iNmr[i].iBSIC != -1 )
                        {
                        bsicArray.AppendL( iCellInfo.iGsmCellInfo.iNmr[i].iBSIC );
                        }
                    }                
                gsmCell->SetBsic( bsicArray );
                geoCell = gsmCell;
            	}
            else if( iNetworkMode == RMobilePhone::ENetworkModeWcdma )
            	{
            	LBT_TRACE( KLbtLogClientLib|KLbtLogVerbose, __FILE__, __LINE__, "WCDMA Network");
            	// Create WCDMA cell
            	CLbtGeoWcdmaCell* wcdmaCell = static_cast<CLbtGeoWcdmaCell*>(geoCell);
            	
            	TLbtWcdmaCellInfo wcdmaCellInfo;
            	
            	// Copy the serving cell parameters
            	wcdmaCellInfo.iCell.iCarrierRssi = iCellInfo.iWcdmaCellInfo.iNwkMeasureReport[0].iCarrierRSSI;
            	wcdmaCellInfo.iCell.iRscp = iCellInfo.iWcdmaCellInfo.iNwkMeasureReport[0].iCellMeasuredResult[0].iFddInfo.iCpichRscp;
            	wcdmaCellInfo.iCell.iEcNo = iCellInfo.iWcdmaCellInfo.iNwkMeasureReport[0].iCellMeasuredResult[0].iFddInfo.iCpichEcN0;
            	wcdmaCellInfo.iCell.iPathLoss = iCellInfo.iWcdmaCellInfo.iNwkMeasureReport[0].iCellMeasuredResult[0].iFddInfo.iPathloss;
            	
            	// Copy the Rscp and Ecno values of the adjacent cells
            	for(TInt i=1;i<KMaxNetworkMeasureReports;++i)
            		{
            		for(TInt j=0;j<KMaxCellMeasuredResults;++j)
            			{
            			TInt8 rscp = iCellInfo.iWcdmaCellInfo.iNwkMeasureReport[i].iCellMeasuredResult[j].iFddInfo.iCpichRscp;
            			TInt8 ecNo = iCellInfo.iWcdmaCellInfo.iNwkMeasureReport[i].iCellMeasuredResult[j].iFddInfo.iCpichEcN0;
            			if( (rscp != -1) && (ecNo != -1) )
            				{
            				TLbtWcdmaCellInfo::TLbtCellMeasuredResult cellMsr;
            				cellMsr.iEcNo = ecNo;
            				cellMsr.iRscp = rscp;
            				wcdmaCellInfo.iMeasurementResult.AppendL(cellMsr);
            				}
            			}
            		}
            	wcdmaCell->SetWcdmaCellInfo(wcdmaCellInfo);
            	geoCell = wcdmaCell;
            	}
            CleanupStack::Pop( geoCell );
            *(iArea) = geoCell;
            TRequestStatus* status = &iClientStatus;
            User::RequestComplete( status, iStatus.Int() );
            }   
        }
    else
        {
        /*
        // -------------------------------------------------------------------------------------------------------
        // ----------- TEST CODE ----------------------------
        CLbtGeoCell* geoCell = NULL;
        CLbtGeoWcdmaCell* wcdmaCell = CLbtGeoWcdmaCell::NewL();
        wcdmaCell->SetNetworkType( RMobilePhone::ENetworkModeWcdma );
        RMobilePhone::TMobilePhoneNetworkCountryCode countryCode(_L("5"));
        wcdmaCell->SetNetworkCountryCode( countryCode );
        RMobilePhone::TMobilePhoneNetworkIdentity nid(_L("5"));
        wcdmaCell->SetNetworkIdentityCode( nid );
        wcdmaCell->SetLocationAreaCode( 5 );
        wcdmaCell->SetCellId( 5 );
        TLbtWcdmaCellInfo wcdmaCellInfo;
        	
    	// Copy the serving cell parameters
    	wcdmaCellInfo.iCell.iCarrierRssi = 5;
    	wcdmaCellInfo.iCell.iRscp = 5;
    	wcdmaCellInfo.iCell.iEcNo = 5;
    	wcdmaCellInfo.iCell.iPathLoss = 5;
    	
    	// Copy the Rscp and Ecno values of the adjacent cells
    	for(TInt i=0;i<1;++i)
    		{
    		for(TInt j=0;j<5;++j)
    			{
    			TInt8 rscp = 5;
    			TInt8 ecNo = 5;
    			if( (rscp != -1) && (ecNo != -1) )
    				{
    				TLbtWcdmaCellInfo::TLbtCellMeasuredResult cellMsr;
    				cellMsr.iEcNo = 5;
    				cellMsr.iRscp = 5;
    				wcdmaCellInfo.iMeasurementResult.Append(cellMsr);
    				}
    			}
    		}
    	wcdmaCell->SetWcdmaCellInfo(wcdmaCellInfo);
    	geoCell = wcdmaCell;
    	*(iArea) = geoCell;
        // ----------- TEST CODE END----------------------------
        // -------------------------------------------------------------------------------------------------------
        */
        TRequestStatus* status = &iClientStatus;
        User::RequestComplete( status, iStatus.Int() );        
        }    
    }    

// -----------------------------------------------------------------------------
// CLbtCellOperation::GetCurrentCellInformation
// 
// -----------------------------------------------------------------------------
//
void CLbtCellOperation::GetCurrentCellInformation( )
    {
    iClientStatus = KRequestPending;
    TInt error = iMPhone.GetCurrentMode( iNetworkMode );
    if( KErrNone != error )
        {
        TRequestStatus* status = &iClientStatus;
        User::RequestComplete( status, error );        
        return;  
        }
    iMPhone.GetCurrentNetwork(iStatus, iNwInfoPckg, iLocArea);
    iCommandId = EMobilePhoneGetCurrentNetwork;
    SetActive();
    }
    
// -----------------------------------------------------------------------------
// CLbtCellOperation::InitialiseEtelL
// 
// -----------------------------------------------------------------------------
//
void CLbtCellOperation::InitialiseEtelL()
    {
	CCommsDatabase* const db = CCommsDatabase::NewL(ETrue); 
	CleanupStack::PushL(db); 

	TUint32 modemId = 0; 
	db->GetGlobalSettingL(TPtrC(MODEM_PHONE_SERVICES_SMS), modemId); 
	CCommsDbTableView* const view =
	    db->OpenViewMatchingUintLC(TPtrC(MODEM), TPtrC(COMMDB_ID), modemId); 

	TInt err = view->GotoFirstRecord();
	if(err != KErrNone)
	    {
        User::Leave(err);
	    }

    HBufC* nameBuf = NULL;
    nameBuf = view->ReadLongTextLC(TPtrC(MODEM_TSY_NAME));

    User::LeaveIfError(iTelServer.Connect());
	err = iTelServer.LoadPhoneModule(*nameBuf);
	
	if(err != KErrNone)
	      {
        User::Leave(err);
	    }
		
	// For the phone information
	RTelServer::TPhoneInfo info;

	iTelServer.GetPhoneInfo(0, info); 
	err = iMPhone.Open(iTelServer, info.iName);
	
	if(err != KErrNone)
	    {
        User::Leave(err);
	    }
	    
	// initialise etel multimode custom api
    err = iMmApi.Open(iMPhone);

	if(err != KErrNone)
	    {
        User::Leave(err);
	    }    

  	CleanupStack::PopAndDestroy(3); // nameBuf, view and db 
    }    
    

// End of File

