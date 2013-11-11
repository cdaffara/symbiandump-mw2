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
* Description:  Definition of CLbtCellIdMvmtDet class.
*
*/



#include <ecom.h>
#include <e32math.h>
#include <implementationproxy.h>
#include "lbtcellidmvmtdet.h"
#include "lbtlogger.h"


// ECOM implementation specifics
static const TImplementationProxy implTable[] =
	{
    IMPLEMENTATION_PROXY_ENTRY(0x2002130D, CLbtCellIdMvmtDet::NewL)
	}; 


EXPORT_C const TImplementationProxy* ImplementationGroupProxy(TInt& aTableCount)
    {
    aTableCount = sizeof(implTable) / sizeof(TImplementationProxy);

    return implTable;
    }
 

// Class methods

//------------------------------------------------------------
// CLbtCellIdMvmtDet::NewL
// Symbian Two - phase constructor
//------------------------------------------------------------
//
CLbtCellIdMvmtDet* CLbtCellIdMvmtDet::NewL()
	{
	FUNC_ENTER("CLbtCellIdMvmtDet::NewL");
	CLbtCellIdMvmtDet* self = new (ELeave) CLbtCellIdMvmtDet();
	CleanupStack::PushL(self);
	self->ConstructL();
	CleanupStack::Pop();
	return self;
	}


//------------------------------------------------------------
// CLbtCellIdMvmtDet::ConstructL
//------------------------------------------------------------
//
void CLbtCellIdMvmtDet::ConstructL()
	{
	FUNC_ENTER("CLbtCellIdMvmtDet::ConstructL");
	InitializeL();
	
	iCellIdMvmtDetGetCellData=CLbtCellIdMvmtDetGetCellData::
									NewL( *this ,iCApi );
	iPreviousNetworkMode = RMmCustomAPI::TMmCellInfo::EUnknown;
	iCurrentCellInfo = NULL;
	iWcdmaCellInfo = NULL;	
	iInitialCellInfo = NULL;
	iPrevCellInfo = NULL;
	}


//------------------------------------------------------------
// CLbtCellIdMvmtDet::CLbtCellIdMvmtDet
// C++ Default constructor
//------------------------------------------------------------
//
CLbtCellIdMvmtDet::CLbtCellIdMvmtDet()
	{
	}


//------------------------------------------------------------
// CLbtCellIdMvmtDet::~CLbtCellIdMvmtDet
// Destructor
//------------------------------------------------------------
//
CLbtCellIdMvmtDet::~CLbtCellIdMvmtDet()
	{
	FUNC_ENTER("CLbtCellIdMvmtDet::~CLbtCellIdMvmtDet");
	delete iCellIdMvmtDetGetCellData;
    delete iPrevCellInfo;
    delete iCurrentCellInfo;
    delete iWcdmaCellInfo;
	iCApi.Close();
    iMPhone.Close();
   	iTelServer.Close();
   	iVarianceOfED.Close();
   	iED.Close();
   	iSumOfSd.Close();
   	iVarianceOfRssi.Close();
	}


//------------------------------------------------------------
// CLbtCellIdMvmtDet::RequestNotificationL
//------------------------------------------------------------
//	
void CLbtCellIdMvmtDet::RequestNotificationL( MLbtMovementObserver* aObserver ,
                                                                TInt aSleepDuration )
	{
	FUNC_ENTER("CLbtCellIdMvmtDet::RequestNotificationL");
	iObserver = aObserver;
	iSleepDuration = aSleepDuration;
	iStartTime.UniversalTime();
	GetCellInfo();	
	}

	
//------------------------------------------------------------
// CLbtCellIdMvmtDet::GetCellInfo
//------------------------------------------------------------
//	
void CLbtCellIdMvmtDet::GetCellInfo()
	{
	FUNC_ENTER("CLbtCellIdMvmtDet::GetCellInfo");
   	iCellIdMvmtDetGetCellData->Start();
	}


//------------------------------------------------------------
// CLbtCellIdMvmtDet::HandleCellDataL
//------------------------------------------------------------
//	
void CLbtCellIdMvmtDet::HandleCellDataL( RMmCustomAPI::TMmCellInfo& aCellInfo, TInt& aError )
	{
	FUNC_ENTER("CLbtCellIdMvmtDet::HandleCellDataL");
	if( KErrNone == aError )
	    {
	    if( iPreviousNetworkMode != aCellInfo.iMode && 
	        iPreviousNetworkMode != RMmCustomAPI::TMmCellInfo::EUnknown )
	        {
	        LOG("Network change movement");
	        iObserver->HandleDetectorNotification( MLbtMovementObserver::EEventMovement ); 
	        iPreviousNetworkMode = RMmCustomAPI::TMmCellInfo::EUnknown;
	        if( iPreviousNetworkMode == RMmCustomAPI::TMmCellInfo::EWCDMA )
	            {
	            iVarianceOfRssi.Reset();
                iSumOfSd.Reset();
                }
	        else if( iPreviousNetworkMode == RMmCustomAPI::TMmCellInfo::EGSM )
	            {
	            iVarianceOfED.Reset();
	            iED.Reset();
	            }
	        return;
	        }
	    iPreviousNetworkMode = aCellInfo.iMode;    
	    switch( aCellInfo.iMode )
	        {
	        case RMmCustomAPI::TMmCellInfo::EGSM:
	            HandleGsmDataL( aCellInfo );
	            break;
	        
	        case RMmCustomAPI::TMmCellInfo::EWCDMA:
	            HandleWcmdaDataL( aCellInfo );
	            break;
	        
	        default:
	            break;    
	        }
	    }
	else
	    {
	    iObserver->HandleDetectorNotification(MLbtMovementObserver::EEventDetectorFailed); 
	    iVarianceOfED.Reset();
	    iED.Reset();
        LBT_TRACE( KLbtLogCellIdMovementDetector|KLbtLogVerbose, __FILE__, __LINE__, "Movement detection failed" ); 
	    }    
	}

//------------------------------------------------------------
// CLbtCellIdMvmtDet::HandleGsmDataL
//------------------------------------------------------------
//	
void CLbtCellIdMvmtDet::HandleGsmDataL( RMmCustomAPI::TMmCellInfo& aCellInfo )
	{
	FUNC_ENTER("CLbtCellIdMvmtDet::HandleGsmDataL");
	CLbtCellInfo* cellInfo = CLbtCellInfo::NewL( aCellInfo );
    if( NULL != iPrevCellInfo )     // First iteration
        {
        // This part of algo detects short movement
        if( iSleepDuration < 20 )
            {
            if( iCurrentCellInfo )
                {
                delete iCurrentCellInfo;
                iCurrentCellInfo = NULL;
                }
            iCurrentCellInfo = cellInfo;
            DetectShortGsmMovement();
            return;
            }
        else
            {
            // Check whether sleep interval has expired
            TTime currentTime;
            currentTime.UniversalTime();
            TTimeIntervalSeconds interval;
            currentTime.SecondsFrom( iStartTime,interval );
            if( interval.Int() >= iSleepDuration )
                {
                // Clear all data
                iVarianceOfED.Reset();
    	        iED.Reset();
    	        
                delete iPrevCellInfo;
                iPrevCellInfo = cellInfo;
                CheckBsicVariation();
                return;
                }
                
            // if there are less than 3 visible BTSs then
            // discard the reading and try acquiring new 
            // info without timer.
            if( cellInfo->Count() < 3 )
                {
                delete cellInfo;
                return;
                }
            else
                {
                TReal ed = 0.0;
                if( KErrNotFound != ComputeED( ed, cellInfo, iPrevCellInfo ) )
                    {
                    TInt error = iED.Append( ed );
                    if( error != KErrNone )
                        {
                        LOG1("Failed to append ed to the array:%d",error);
                        }
                    iSumOfED += ed;
                    if( iED.Count() == 5 )
                        {
                        ComputeVarianceOfED();
                        if( iAvgOfVariance > 5.0 )
                            {
                            LOG1("iAvgOfVariance:%f",iAvgOfVariance);
                            iAvgOfVariance = 0.0;   // reset variance
                            iObserver->HandleDetectorNotification( MLbtMovementObserver::EEventMovement ); 
                            iVarianceOfED.Reset();
    	                    iED.Reset();
                            LBT_TRACE( KLbtLogCellIdMovementDetector|KLbtLogVerbose, __FILE__, __LINE__, "Movement detected" );
                            delete cellInfo;
                            return;
                            }
                        }
                    }
                else    // No matches found!!
                    {
                    iObserver->HandleDetectorNotification( MLbtMovementObserver::EEventMovement ); 
                    iVarianceOfED.Reset();
    	            iED.Reset();
                    LBT_TRACE( KLbtLogCellIdMovementDetector|KLbtLogVerbose, __FILE__, __LINE__, "Possible movement" );
                    delete cellInfo;
                    return;
                    }
                }
            
            delete iPrevCellInfo;
            iPrevCellInfo = NULL;
            }    
        }
    else
        {
        iInitialCellInfo = CLbtCellInfo::NewL( aCellInfo );
        }
    iPrevCellInfo = cellInfo;
    }


//------------------------------------------------------------
// CLbtCellIdMvmtDet::HandleWcmdaDataL
//------------------------------------------------------------
//	
void CLbtCellIdMvmtDet::HandleWcmdaDataL( RMmCustomAPI::TMmCellInfo& aCellInfo )
    {
    FUNC_ENTER("CLbtCellIdMvmtDet::HandleWcmdaDataL");
    if( !iWcdmaCellInfo )
        {
        iWcdmaCellInfo = CLbtCellInfo::NewL( aCellInfo );
        }
    else
        {
        iWcdmaCellInfo->AddNMRData( aCellInfo );
        if( iSleepDuration < 20 )
            {
            DetectShortWcdmaMovement();
            return;
            }
        
        // If the total number of cell change is greater than two,we predict movement 
        // irrespective of other NMR readings.
        if( iWcdmaCellInfo->CellChangeCount() == 2 )
            {
            LOG("Cell change movement");
            iObserver->HandleDetectorNotification( MLbtMovementObserver::EEventMovement ); 
            LBT_TRACE( KLbtLogCellIdMovementDetector|KLbtLogVerbose, __FILE__, __LINE__, "Movement detected" );
            iVarianceOfRssi.Reset();
            iSumOfSd.Reset();
            return;
            }
        // Check for movement based on calculation for every 10 readings
        else if( iWcdmaCellInfo->Count() == 10 )
            {
            TReal carrierRSSISD = 0.0;
        	TReal cpichEcN0SD= 0.0;
        	TReal cpichRscpSD= 0.0;
        	TReal pathlossSD= 0.0;
        	TInt cellChangeCount;
            CalculateSDForWcdmaNmr( carrierRSSISD,cpichEcN0SD,cpichRscpSD,pathlossSD,cellChangeCount );
            
            // Test log
            LOG1("carrierRSSISD:%f",carrierRSSISD);
            LOG1("cpichEcN0SD:%f",cpichEcN0SD);
            LOG1("cpichRscpSD:%f",cpichRscpSD);
            LOG1("pathlossSD:%f",pathlossSD);
            LOG1("cellChangeCount:%d",cellChangeCount);
            // Test logs
            
            // There is a possibility that network fails and it transmits stored values, it is 
            // not possible for movement detector to predict anything from the network information.Hence 
            // movement detection fails.
            if( !cpichEcN0SD || !cpichRscpSD || !pathlossSD)
                {
                iObserver->HandleDetectorNotification( MLbtMovementObserver::EEventDetectorFailed ); 
                iVarianceOfRssi.Reset();
                iSumOfSd.Reset();
                return;
                }
            
            if( MovementDetected( carrierRSSISD,cpichEcN0SD,cpichRscpSD,pathlossSD,cellChangeCount ) )
                {
                iObserver->HandleDetectorNotification( MLbtMovementObserver::EEventMovement ); 
                iVarianceOfRssi.Reset();
                iSumOfSd.Reset();
                }
            }
        }    
    }

//------------------------------------------------------------
// CLbtCellIdMvmtDet::MovementDetected
//------------------------------------------------------------
//	
TBool CLbtCellIdMvmtDet::MovementDetected( TReal& aCarrierRSSISD,TReal& aCpichEcN0SD,
                                           TReal& aCpichRscpSD,TReal& aPathlossSD,
                                           TInt& /*aCellChangeCount*/  )    
    {
    FUNC_ENTER("CLbtCellIdMvmtDet::MovementDetected");
    TReal sumOfSd = aCpichEcN0SD + aCpichRscpSD + aPathlossSD;
    
    if( // These 3 parameters vary for movement
        aCpichEcN0SD > 5 || 
        aCpichRscpSD > 5 ||
        aPathlossSD  > 5 ||
        aCarrierRSSISD > 3 || 
        sumOfSd > 10   ||
        ( aCarrierRSSISD > 2 && sumOfSd > 6 ) )
        {
        LOG("Reason:1");
        return ETrue;
        }
        
    TInt error = iSumOfSd.Append( sumOfSd );
    if( error != KErrNone )
        {
        LOG1("Append sumOfSd to the array:%d",error);
        }
    error = iVarianceOfRssi.Append( aCarrierRSSISD );
    if( error != KErrNone )
        {
        LOG1("Append aCarrierRSSISD to the array:%d",error);
        }
    if( iSumOfSd.Count() == 3 )
        {
        TReal sumAvg = 0.0;
        TReal rssiAvg = 0.0;
        for( TInt i=0;i<iSumOfSd.Count();i++ )
            {
            sumAvg+= iSumOfSd[i];
            rssiAvg+= iVarianceOfRssi[i];  
            }
        sumAvg/= iSumOfSd.Count();
        rssiAvg/= iVarianceOfRssi.Count();  
        
        // Remove the old values
        iSumOfSd.Remove( 0 );
        iVarianceOfRssi.Remove( 0 );  
        
        if( ( sumAvg>6 && rssiAvg>2 ) ||
            ( sumAvg>8 && rssiAvg>1.75 ) )
            {
            LOG("Reason:2");
            return ETrue;
            }
        }
    return EFalse;    
    }

//------------------------------------------------------------
// CLbtCellIdMvmtDet::CalculateVariance
//------------------------------------------------------------
//	
void CLbtCellIdMvmtDet::CalculateSDForWcdmaNmr(TReal& aCarrierRSSISD,TReal& aCpichEcN0SD,
                                               TReal& aCpichRscpSD,TReal& aPathlossSD,
                                               TInt& aCellChangeCount )
    {
    FUNC_ENTER("CLbtCellIdMvmtDet::CalculateSDForWcdmaNmr");
    TReal carrierRSSIMean = 0.0;
	TReal cpichEcN0Mean= 0.0;
	TReal cpichRscpMean= 0.0;
	TReal pathlossMean= 0.0;
	
	// Calculate the mean for NMR values. 
	CLbtCellInfo::TCellInfoArrayIterator iter = iWcdmaCellInfo->Begin();
	TWcdmaNMR wcdmaNMR;
	while( iter.Next( wcdmaNMR ) )
	    {
	    carrierRSSIMean+= wcdmaNMR.iCarrierRSSI;
	    cpichEcN0Mean+= wcdmaNMR.iCpichEcN0;
	    cpichRscpMean+= wcdmaNMR.iCpichRscp;
	    pathlossMean+= wcdmaNMR.iPathloss;
	    }
	carrierRSSIMean/= iWcdmaCellInfo->Count();
    cpichEcN0Mean/= iWcdmaCellInfo->Count();
    cpichRscpMean/= iWcdmaCellInfo->Count();
    pathlossMean/= iWcdmaCellInfo->Count();
    
    // Calculate the variance for NMR values. 
    
    iter.Reset();
	while( iter.Next( wcdmaNMR ) )
        {
        aCarrierRSSISD+= ( carrierRSSIMean - wcdmaNMR.iCarrierRSSI ) * ( carrierRSSIMean - wcdmaNMR.iCarrierRSSI );
        aCpichEcN0SD+= ( cpichEcN0Mean - wcdmaNMR.iCpichEcN0 ) * ( cpichEcN0Mean - wcdmaNMR.iCpichEcN0 );
        aCpichRscpSD+= ( cpichRscpMean - wcdmaNMR.iCpichRscp ) * ( cpichRscpMean - wcdmaNMR.iCpichRscp );
        aPathlossSD+= ( pathlossMean - wcdmaNMR.iPathloss ) * ( pathlossMean - wcdmaNMR.iPathloss );
        }
    aCarrierRSSISD/= iWcdmaCellInfo->Count();
    aCpichEcN0SD/= iWcdmaCellInfo->Count();
    aCpichRscpSD/= iWcdmaCellInfo->Count();
    aPathlossSD/= iWcdmaCellInfo->Count();
    
    // Calculate the standard deviation for NMR values.    
    Math::Sqrt( aCarrierRSSISD,aCarrierRSSISD );
    Math::Sqrt( aCpichEcN0SD,aCpichEcN0SD );
    Math::Sqrt( aCpichRscpSD,aCpichRscpSD );
    Math::Sqrt( aPathlossSD,aPathlossSD );
    
    aCellChangeCount = iWcdmaCellInfo->CellChangeCount();
    iWcdmaCellInfo->Reset();
    }

	
//------------------------------------------------------------
// CLbtCellIdMvmtDet::ComputeED
//------------------------------------------------------------
//	
TInt CLbtCellIdMvmtDet::ComputeED( TReal& aED, CLbtCellInfo* aCurrCellInfo, CLbtCellInfo* aPrevCellInfo )
    {
    FUNC_ENTER("CLbtCellIdMvmtDet::ComputeED");
    // Find matches and calculate ED
    // Compare cellinfo with the previous readings. If there are 
    // less than 3 and more than 1 matches substitute with the lower 
    // readings of RxLEV. If there is just 1 or no matches then notify 
    // movement.
    CLbtCellInfo::TCellInfoArrayIterator iter = aCurrCellInfo->Begin();
    TGsmNMR nmr;
    TInt pos = 0;
    TInt leastRxLEV = -1;
    TReal diffSqr;
    RArray< TUint > diffArray;
    RArray< TUint > posArray;
    while( iter.Next( nmr ) )
        {
        // Find least of RxLEVs in the recent measurement.
        if( -1 == leastRxLEV || nmr.RxLEV < leastRxLEV )
            {
            leastRxLEV = nmr.RxLEV;
            }
            
        if( aPrevCellInfo->Find( pos, nmr ) )
            {
            // Calculate difference in RxLEV and the corresponding square
            TInt diff = 
                ( ( aPrevCellInfo->GetNMR() )[pos].RxLEV ) - ( nmr.RxLEV );
            Math::Pow( diffSqr, diff, 2 );
            TInt error = diffArray.Append( diffSqr );
            if( error != KErrNone )
                {
                LOG1("Failed to append to diff array:%d",error);
                }
            error = posArray.Append( pos );
            if( error !=KErrNone )
                {
                LOG1("Failed to append to pos array:%d",error);
                }
            }
        else
            {
            TInt diff = nmr.RxLEV - leastRxLEV;
            Math::Pow( diffSqr, diff, 2 );
            diffArray.Append( diffSqr );
            }
        }
    
    if( posArray.Count() > 1 )  // There was at least 1 match between the 2 readings
        {
        // Check for other lost entries in previously obtained
        // cellinfo
        RArray< TGsmNMR >& prevNMR = aPrevCellInfo->GetNMR();
        for( TInt index = 0; index < prevNMR.Count(); index++ )
            {
            if( KErrNotFound == posArray.Find( index ) )
                {
                TInt diff = prevNMR[index].RxLEV - leastRxLEV;
                Math::Pow( diffSqr, diff, 2 );
                diffArray.Append( diffSqr );
                }
            }
           
        // Calculate ED
        TUint sum = 0;
        for( TInt index = 0; index < diffArray.Count(); index++ )
            {
            sum += diffArray[index];
            }
            
        Math::Sqrt( aED, sum );
        diffArray.Close();
        posArray.Close();
        return KErrNone;
        }
        
    return KErrNotFound;    // no matches found
    }
    

//------------------------------------------------------------
// CLbtCellIdMvmtDet::ComputeVarianceOfED
//------------------------------------------------------------
//	
void CLbtCellIdMvmtDet::ComputeVarianceOfED()
    {
    FUNC_ENTER("CLbtCellIdMvmtDet::ComputeVarianceOfED");
    // Determine mean
    TReal mean = iSumOfED / 5;  // As the window size is set to 5
    iSumOfED = 0.0;

    // Calculate distance from mean
    TReal dist = 0.0;
    TReal sqr = 0.0;
    TReal sum = 0.0;
    for( TInt i = 0; i < iED.Count(); i++ )
        {
        dist = iED[i] - mean;
        Math::Pow( sqr, dist, 2 );
        sum += sqr;
        }

    iED.Reset();
    TReal variance = sum / 5;
    TInt error = iVarianceOfED.Append( variance );
    if( error != KErrNone )
        {
        LOG1("Failed to append variance:%d",error);
        }
    
    
    // Check if variance is > 5. If true then check the next set of
    // 5 variances. If there are more than two variance readings > 5
    // then compute average of the 5 readings and check if this value
    // is > 5. If true then notify movement.
    if( iVarianceOfED.Count() == 3 )
        {
        // Find average of variances
        TInt8 varianceCount = 0;
        sum = 0.0;
        for( TInt i = 0; i < iVarianceOfED.Count(); i++ )
            {
            sum += iVarianceOfED[i];
            if( iVarianceOfED[i] > 5.0 )
                {
                ++varianceCount;
                }
            }
        if( varianceCount >= 2 )
            {
            iAvgOfVariance = sum / 3;   // average of 5 variance readings
            }
        else
            {
            iAvgOfVariance = 0.0;
            }
        
        iVarianceOfED.Reset();
        }
    }
    

//------------------------------------------------------------
// CLbtCellIdMvmtDet::StopNotification
//------------------------------------------------------------
//		
void CLbtCellIdMvmtDet::StopNotification()
	{
	FUNC_ENTER("CLbtCellIdMvmtDet::StopNotification");
	iCellIdMvmtDetGetCellData->Stop();
	delete iPrevCellInfo;
	iPrevCellInfo = NULL;
    delete iCurrentCellInfo;
    iCurrentCellInfo = NULL;
    delete iInitialCellInfo;
    iInitialCellInfo = NULL;
    delete iWcdmaCellInfo;
    iWcdmaCellInfo = NULL;
	}


//------------------------------------------------------------
// CLbtCellIdMvmtDet::DetectShortGsmMovement
//------------------------------------------------------------
//
void CLbtCellIdMvmtDet::DetectShortGsmMovement()
    {    
    LOG("=>CLbtCellIdMvmtDet::DetectShortMovement");
    // Compare BSIC and ARFCN of current reading with the intial value.
    RArray< TGsmNMR >& intialData = iInitialCellInfo->GetNMR();
    RArray< TGsmNMR >& currentData = iCurrentCellInfo->GetNMR();
    // Test
    LOG1("Initialdata count:%d",intialData.Count() );
    LOG1("Current data count:%d",currentData.Count() );
    // Test
    
    TReal ratioOfBsicVisible = ( TReal ) currentData.Count() / ( TReal ) intialData.Count();
    if( ratioOfBsicVisible < 0.5 )
        {
        LOG1("ratioOfBsicVisible:%f",ratioOfBsicVisible);
        iObserver->HandleDetectorNotification( MLbtMovementObserver::EEventMovement ); 
        return;
        }
    
    TReal euclidianDist = 0.0;
    TInt currentMatch = 0;
    
    for( TInt i=0;i<currentData.Count();i++ )
        for( TInt j=0;j<intialData.Count();j++ )
        {
        if( currentData[i].BSIC == intialData[j].BSIC &&
            currentData[i].ARFCN == intialData[j].ARFCN )
            {
            currentMatch++;
            euclidianDist += ( currentData[i].RxLEV - intialData[j].RxLEV )*
	    				        ( currentData[i].RxLEV - intialData[j].RxLEV );  
	    	}
        }
    Math::Sqrt( euclidianDist,euclidianDist );    
    
    TReal ratio = ( TReal )currentMatch / ( TReal )intialData.Count();
    
    LOG1("Euclidian distance:%f",euclidianDist);
    LOG1("ratio:%f",ratio)
    if( ratio <= 0.8 && euclidianDist >= 15 )
        {
        iObserver->HandleDetectorNotification( MLbtMovementObserver::EEventMovement );
        LOG("Movement detected");
        }
    LOG("CLbtCellIdMvmtDet::DetectShortMovement=>");    
    }

//------------------------------------------------------------
// CLbtCellIdMvmtDet::DetectShortWcdmaMovement
//------------------------------------------------------------
//
void CLbtCellIdMvmtDet::DetectShortWcdmaMovement()
    {
    FUNC_ENTER("CLbtCellIdMvmtDet::DetectShortWcdmaMovement");
    LOG("CLbtCellIdMvmtDet::DetectShortWcdmaMovement");
    // If cell change has happened, notify movement
    if( iWcdmaCellInfo->CellChangeCount() == 2 )
        {
        iObserver->HandleDetectorNotification( MLbtMovementObserver::EEventMovement ); 
        LOG("Small cell change movement");
        return;
        }
    
    // Compare initial NMR readings with the current readings
    RArray<TWcdmaNMR>& wcdmaNMR = iWcdmaCellInfo->GetWcdmaNMR();
    
    TInt count = wcdmaNMR.Count();
    if( count == 0 )
        {
        iObserver->HandleDetectorNotification( MLbtMovementObserver::EEventDetectorFailed ); 
        return;
        }
        
    TInt cRssiDiff = Abs( wcdmaNMR[0].iCarrierRSSI - wcdmaNMR[count-1].iCarrierRSSI );
    TInt cPRscpDiff = Abs( wcdmaNMR[0].iCpichRscp - wcdmaNMR[count-1].iCpichRscp );
    TInt pathLossDiff = Abs( wcdmaNMR[0].iPathloss - wcdmaNMR[count-1].iPathloss );
    
    //Test Logs
    LOG1("cRssiDiff:%d",cRssiDiff);
    LOG1("cPRscpDiff:%d",cPRscpDiff);
    LOG1("pathLossDiff:%d",pathLossDiff);
    //Test Logs
    
    if( cRssiDiff >= 2 || ( cPRscpDiff >= 5 && pathLossDiff >= 5 ) )
        {
        iObserver->HandleDetectorNotification( MLbtMovementObserver::EEventMovement );
        LOG("Short movement detected"); 
        }
    }
//------------------------------------------------------------
// CLbtCellIdMvmtDet::InitialiseL
//------------------------------------------------------------
//
void CLbtCellIdMvmtDet::CheckBsicVariation()
    {
    FUNC_ENTER("CLbtCellIdMvmtDet::CheckBsicVariation");
    RArray< TGsmNMR >& intialData = iInitialCellInfo->GetNMR();
    RArray< TGsmNMR >& currentData = iPrevCellInfo->GetNMR();
    
    // Test
    LOG1("Initialdata count:%d",intialData.Count() );
    LOG1("Current data count:%d",currentData.Count() );
    // Test
    
    TInt totalMatch = 0;
    for( TInt i=0;i<currentData.Count();i++ )
        for( TInt j=0;j<intialData.Count();j++ )
        {
        if( currentData[i].BSIC == intialData[j].BSIC &&
            currentData[i].ARFCN == intialData[j].ARFCN )
            {
            totalMatch++;
            }
        }
    TReal ratio = ( TReal ) totalMatch / ( TReal ) intialData.Count();
    
    TReal comparisionFactor = CalculateComparisionFactor( iSleepDuration );
    LOG1("ratio:%f",ratio);
    LOG1("Comaparision factor:%f",comparisionFactor);
    if( ratio <= comparisionFactor )
        {
        iObserver->HandleDetectorNotification( MLbtMovementObserver::EEventMovement );
        LOG("Movement detected");
        }
    }


//------------------------------------------------------------
// CLbtCellIdMvmtDet::CalculateComparisionFactor
//------------------------------------------------------------
//
TReal CLbtCellIdMvmtDet::CalculateComparisionFactor( TInt aSleepDuration )
    {
    if( aSleepDuration > 20 && aSleepDuration < 30 )
        {
        return 0.75;
        }
    else if( aSleepDuration > 30 && aSleepDuration < 40 )
        {
        return 0.65;
        }
    else if( aSleepDuration > 40 && aSleepDuration < 50 )
        {
        return 0.60;
        }
    else
        {
        return 0.50;
        }
    }

//------------------------------------------------------------
// CLbtCellIdMvmtDet::InitialiseL
//------------------------------------------------------------
//
void CLbtCellIdMvmtDet::InitializeL()
    {
    FUNC_ENTER("CLbtCellIdMvmtDet::InitializeL");
    CCommsDatabase* const db = CCommsDatabase::NewL( ETrue );
    CleanupStack::PushL( db ); 

    TUint32 modemId = 0; 
    db->GetGlobalSettingL( TPtrC( MODEM_PHONE_SERVICES_SMS ), modemId ); 
    CCommsDbTableView* const view =
        db->OpenViewMatchingUintLC( TPtrC( MODEM ), TPtrC( COMMDB_ID ), modemId ); 

    TInt err = view->GotoFirstRecord();
    if( err != KErrNone )
        {
        User::Leave( err );
        }

    HBufC* nameBuf = NULL;
    nameBuf = view->ReadLongTextLC( TPtrC( MODEM_TSY_NAME ) );

    User::LeaveIfError( iTelServer.Connect() );
    err = iTelServer.LoadPhoneModule( *nameBuf );
    if( err != KErrNone )
        {
        User::Leave( err );
        }

    // For the phone information
    RTelServer::TPhoneInfo info;
    iTelServer.GetPhoneInfo( 0, info ); 
    err = iMPhone.Open( iTelServer, info.iName );
    if( err != KErrNone )
        {
        User::Leave( err );
        }

    // initialise etel multimode custom api
    err = iCApi.Open( iMPhone );
    if( err != KErrNone )
        {
        User::Leave( err );
        }

    CleanupStack::PopAndDestroy( 3 ); // nameBuf, view and db    
    }
