/*
* Copyright (c) 2009 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:  A class for creating country and city tags for media files
*
*/

//#include "locationmanagerdebug.h"
#include "cgeoconverter.h"
#include "locationmanagerdebug.h"



// --------------------------------------------------------------------------
// CGeoConverter::NewL
// --------------------------------------------------------------------------
//
EXPORT_C CGeoConverter* CGeoConverter::NewL( MGeoConverterObserver& aObserver )
    {
    LOG("CGeoConverter::NewL");
    CGeoConverter* self = new( ELeave ) CGeoConverter( aObserver );
    CleanupStack::PushL( self );
    self->ConstructL();
    CleanupStack::Pop(); // self
    return self;
    }

// --------------------------------------------------------------------------
// CGeoConverter::CGeoConverter()
// --------------------------------------------------------------------------
//    
CGeoConverter::CGeoConverter( MGeoConverterObserver& aObserver )
    :iObserver( aObserver ) 
#ifdef LOC_GEOTAGGING_CELLID    
    ,iLocConverter(NULL),
    iLocInfo(NULL),
    iGsmCellInfo(NULL),
    iWcdmaCellInfo(NULL)
#endif
    {
    }

// --------------------------------------------------------------------------
// CGeoConverter::~CGeoConverter
// --------------------------------------------------------------------------
//  
EXPORT_C CGeoConverter::~CGeoConverter()
    {
    LOG("CGeoConverter::~CGeoConverter, begin");
#ifdef LOC_GEOTAGGING_CELLID    
    delete iGsmCellInfo;
    iGsmCellInfo = NULL;    
    delete iWcdmaCellInfo;
    iWcdmaCellInfo = NULL;
    delete iLocInfo;
    iLocInfo = NULL;
    delete iLocConverter;
    iLocConverter = NULL;
#endif    
    LOG("CGeoConverter::~CGeoConverter, end");
    }

// --------------------------------------------------------------------------
// CGeoConverter::OnConversionComplete()
// --------------------------------------------------------------------------
//  
void CGeoConverter::OnConversionComplete( TInt aStatusCode )
    {
    LOG("CGeoConverter::OnConversionComplete, begin");
#ifdef LOC_GEOTAGGING_CELLID    
    //16. Retrieve position estimate that is updated by Location Monitor server
    //after a conversion operation is completed successfully.
    RLbsAreaInfoBaseArray posInfoList;
    TLocality position;
    
    
    LOG1("Status Code - %d",aStatusCode);
    
    if( KErrNone == aStatusCode )
        {
        TRAP_IGNORE(iLocInfo->GetAreaInfoL( posInfoList, CLbsLocationInfo::ELbsPosInfo );)
        
        //Get geo-coordinates. 
        
        LOG1("PposInfoList Count - %d",posInfoList.Count());
        if ( posInfoList.Count() )
            {
            static_cast<CLbsPositionInfo*>(posInfoList[0])->GetLocality(position);
            }
        else
            {
            aStatusCode = KErrNotFound;
            }     
        }
    TRAPD(err,iObserver.ConversionCompletedL( aStatusCode, position);)
    if(err != KErrNone)
    	{
		iObserver.HandleConversionError(err);
    	}
    posInfoList.Close();
#endif    
    LOG("CGeoConverter::OnConversionComplete, end");
    }

// --------------------------------------------------------------------------
// CGeoConverter::ConstructL()
// --------------------------------------------------------------------------
//  
void CGeoConverter::ConstructL()
    {
    LOG("CGeoConverter::ConstructL, begin");
#ifdef LOC_GEOTAGGING_CELLID    
    iLocInfo = CLbsLocationInfo::NewL();
    TUid pluginUid = TUid::Uid( 0x20027011 );   
    LOG("CGeoConverter::ConstructL");
    iLocConverter = CLbsLocationInfoConverter::NewL( *this, pluginUid );
#endif    
    LOG("CGeoConverter::ConstructL, end");
    }

// --------------------------------------------------------------------------
// CGeoConverter::ConvertL()
// --------------------------------------------------------------------------
//  
EXPORT_C void CGeoConverter::ConvertL( const CTelephony::TNetworkInfoV1& aNetworkInfo )
    {
    LOG("CGeoConverter::ConvertL, begin");
#ifdef LOC_GEOTAGGING_CELLID    
    
    //Set mobile country code
    TLex lexer( aNetworkInfo.iCountryCode );
    TUint countryCode = 0;
    
    User::LeaveIfError( lexer.Val( countryCode, EDecimal) );
    //Set mobile network code
    lexer = aNetworkInfo.iNetworkId;
    TUint networkCode = 0;
    User::LeaveIfError( lexer.Val( networkCode, EDecimal) );
    LOG1("Network mode - %d", aNetworkInfo.iMode);
    CLbsAreaInfoBase* areaInfoBase = NULL;
    switch(aNetworkInfo.iAccess)
        {
        case CTelephony::ENetworkAccessGsm:
        case CTelephony::ENetworkAccessGsmCompact:
            {
            LOG("2G network.");
            if(iGsmCellInfo == NULL)
                {
                iGsmCellInfo = CLbsGsmCellInfo::NewL();
                }
            iGsmCellInfo->SetMobileCountryCode( countryCode );
            
            iGsmCellInfo->SetMobileNetworkCode( networkCode );
            //Set cell id 
            iGsmCellInfo->SetCellId( aNetworkInfo.iCellId ); 

            if(aNetworkInfo.iAreaKnown)
                {
                //Set location area code
                iGsmCellInfo->SetLocationAreaCode( aNetworkInfo.iLocationAreaCode );
                }            
        
            areaInfoBase = iGsmCellInfo;
            }        
            break;
        case CTelephony::ENetworkAccessUtran:
            {
            LOG("3G network.");
            if(iWcdmaCellInfo == NULL)
                {
                iWcdmaCellInfo = CLbsWcdmaCellInfo::NewL();
                }
            iWcdmaCellInfo->SetMobileCountryCode(countryCode);
            iWcdmaCellInfo->SetMobileNetworkCode(networkCode);
            iWcdmaCellInfo->SetUniqueCellId(aNetworkInfo.iCellId);
            areaInfoBase = iWcdmaCellInfo;
            }
            break;
        default:
            LOG("Unknown network mode");
            User::Leave(KErrNotSupported);
        }
    //Add the cell information to the location info class. This class
    //would be sent to the Location Monitor server that would perform the
    //requested conversion and update the position estimate
    if(areaInfoBase != NULL) // self check
        {
        // reset previous one.. to clear the local info.
        iLocInfo->ResetAreaInfo(CLbsLocationInfo::ELbsAreaInfoAll);
        iLocInfo->AddAreaInfoL( areaInfoBase ); 
        //ownership is transferred.
        iWcdmaCellInfo = NULL;
        iGsmCellInfo = NULL;
        //Request conversion of GSM cell information to corresponding coordinate information
        iLocConverter->ConvertLocationInfoL( *iLocInfo, ELbsConversionSilent, 
                                                ELbsConversionOutputPosition  );
        }
#endif    
    LOG("CGeoConverter::ConvertL, end");
    }

// End of file

