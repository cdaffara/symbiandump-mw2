/*
* Copyright (c) 2006-2009 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:  A class for getting network cell id.
*
*/

#include <etel3rdparty.h>

#include "cnetworkinfo.h"
#include "locationmanagerdebug.h"




// --------------------------------------------------------------------------
// CNetworkInfo::NewL
// --------------------------------------------------------------------------
//
EXPORT_C CNetworkInfo* CNetworkInfo::NewL( MNetworkInfoObserver* aTrail )
    {
    LOG( "CNetworkInfo::NewL(), begin" );
    CNetworkInfo* self = new (ELeave) CNetworkInfo( aTrail );
    CleanupStack::PushL( self );
    self->ConstructL();
    CleanupStack::Pop( self );
    return self;
    }
        
// --------------------------------------------------------------------------
// CNetworkInfo::CNetworkInfo
// --------------------------------------------------------------------------
//  
CNetworkInfo::CNetworkInfo( MNetworkInfoObserver* aTrail ) 
    : CActive( CActive::EPriorityStandard ),
    iTrail ( aTrail ),
    iMobilePhone(aTrail->GetMobilePhone4NwInfo()),
    iState( ENetworkInfoNoOptState ),
    
    iMMNetworkInfoPckg( iMMNetworkInfo ) 
    {
    CActiveScheduler::Add( this );
    }

// --------------------------------------------------------------------------
// CNetworkInfo::ConstructL
// --------------------------------------------------------------------------
//    
void CNetworkInfo::ConstructL()
    {
	LOG( "CNetworkInfo::ConstructL(), begin" );
    StartNwInfoChangeNotifier();
    LOG( "CNetworkInfo::ConstructL(), end" );
    }
    
// --------------------------------------------------------------------------
// CNetworkInfo::~CNetworkInfo
// --------------------------------------------------------------------------
//    
EXPORT_C CNetworkInfo::~CNetworkInfo()
    {
    // we are not owning RMobilePhone obj.. DON'T CLOSE it.
    Cancel();
    }

// --------------------------------------------------------------------------
// CNetworkInfo::RunError
// --------------------------------------------------------------------------
//
TInt CNetworkInfo::RunError( TInt /*aError*/ )
    {
    return KErrNone;
    }    

// --------------------------------------------------------------------------
// CNetworkInfo::StartNwInfoChangeNotifier
// --------------------------------------------------------------------------
//    
EXPORT_C void CNetworkInfo::StartNwInfoChangeNotifier()
    { 
	LOG( "CNetworkInfo::StartNwInfoChangeNotifier(), begin" );   
   	if ( !IsActive())
    	{
    	// get current n/w info first and from RunL start notifier
        iState = ENetworkInfoOptGetState; 
        
        iMobilePhone.GetCurrentNetwork(iStatus, iMMNetworkInfoPckg, iMMArea);
        SetActive();
        }
    
    LOG( "CNetworkInfo::StartNwInfoChangeNotifier(), end" ); 
    }    

// --------------------------------------------------------------------------
// CNetworkInfo::StopNwInfoChangeNotifier
// --------------------------------------------------------------------------
//    
EXPORT_C void CNetworkInfo::StopNwInfoChangeNotifier()
    { 
	LOG( "CNetworkInfo::StopNwInfoChangeNotifier(), begin" );   
    Cancel();
    iState = ENetworkInfoNoOptState;
    LOG( "CNetworkInfo::StopNwInfoChangeNotifier(), end" ); 
    }    

// --------------------------------------------------------------------------
// CNetworkInfo::RunL
// --------------------------------------------------------------------------
//    
void CNetworkInfo::RunL()
    { 
	LOG( "CNetworkInfo::RunL(), begin" );   
	// reset the flag first
    iState = ENetworkInfoNoOptState;
    if(iStatus == KErrNone)
        {
        // convert the network info to CTelephony network info. just to give backward compatibility
        MapNetworkInfo(&iNetworkInfoV1, &iMMNetworkInfo, &iMMArea);
        }
    if(iTrail)
        {
        iTrail->NetworkInfo( iNetworkInfoV1, iStatus.Int() );
        }
    // listen to n/w info change
   	if ( !IsActive() )
    	{
    	iState = ENetworkInfoOptNotifyState;
        iMobilePhone.NotifyCurrentNetworkChange(iStatus,iMMNetworkInfoPckg, iMMArea);
        SetActive();
        }
    LOG( "CNetworkInfo::RunL(), end" ); 
    }    

// --------------------------------------------------------------------------
// CNetworkInfo::DoCancel
// --------------------------------------------------------------------------
// 
void CNetworkInfo::DoCancel()
    {
	LOG1( "CNetworkInfo::DoCancel(), begin. State - %d", iState);
	switch ( iState )    
        {   
        case ENetworkInfoOptGetState:
        	{
        	LOG( "Cancel get current n/w info change" );
        	iMobilePhone.CancelAsyncRequest( EMobilePhoneGetCurrentNetwork);
            break;
        	}
        case ENetworkInfoOptNotifyState:
    		{
        	LOG( "Cancel current n/w info change" );
        	iMobilePhone.CancelAsyncRequest( EMobilePhoneNotifyCurrentNetworkChange);
            break;
        	}
        default:
            break;
        }
    iState = ENetworkInfoNoOptState;
   	LOG( "CNetworkInfo::DoCancel(), end" );
    }

// --------------------------------------------------------------------------
// CNetworkInfo::MapNetworkInfo
// --------------------------------------------------------------------------
// 
EXPORT_C void CNetworkInfo::MapNetworkInfo(CTelephony::TNetworkInfoV1* aTelNetworkInfo, 
    RMobilePhone::TMobilePhoneNetworkInfoV2* aMMNetworkInfo, 
    RMobilePhone::TMobilePhoneLocationAreaV1* aMMArea)
	{
   	LOG( "CNetworkInfo::MapNetworkInfo(), begin" );
	switch(aMMNetworkInfo->iMode)
		{
		case RMobilePhone::ENetworkModeUnknown: 
			aTelNetworkInfo->iMode=CTelephony::ENetworkModeUnknown;
			break;
		case RMobilePhone::ENetworkModeUnregistered:
			aTelNetworkInfo->iMode=CTelephony::ENetworkModeUnregistered;
			break;
		case RMobilePhone::ENetworkModeGsm:
			aTelNetworkInfo->iMode=CTelephony::ENetworkModeGsm;
			break;
		case RMobilePhone::ENetworkModeAmps:
			aTelNetworkInfo->iMode=CTelephony::ENetworkModeAmps;
			break;			
		case RMobilePhone::ENetworkModeCdma95:
			aTelNetworkInfo->iMode=CTelephony::ENetworkModeCdma95;
			break;
		case RMobilePhone::ENetworkModeCdma2000:
			aTelNetworkInfo->iMode=CTelephony::ENetworkModeCdma2000;
			break;
		case RMobilePhone::ENetworkModeWcdma:
			aTelNetworkInfo->iMode=CTelephony::ENetworkModeWcdma;
			break;
		case RMobilePhone::ENetworkModeTdcdma:
			aTelNetworkInfo->iMode=CTelephony::ENetworkModeTdcdma;
			break;
		default:
			aTelNetworkInfo->iMode=CTelephony::ENetworkModeUnknown;
		}	

	switch(aMMNetworkInfo->iStatus)
		{
		case RMobilePhone::ENetworkStatusUnknown: 
			aTelNetworkInfo->iStatus=CTelephony::ENetworkStatusUnknown;
			break;
		case RMobilePhone::ENetworkStatusAvailable:
			aTelNetworkInfo->iStatus=CTelephony::ENetworkStatusAvailable;
			break;
		case RMobilePhone::ENetworkStatusCurrent:
			aTelNetworkInfo->iStatus=CTelephony::ENetworkStatusCurrent;
			break;
		case RMobilePhone::ENetworkStatusForbidden:
			aTelNetworkInfo->iStatus=CTelephony::ENetworkStatusForbidden;
			break;			
		default:
			aTelNetworkInfo->iStatus=CTelephony::ENetworkStatusUnknown;
		}	
			
	switch(aMMNetworkInfo->iBandInfo)
		{
		case RMobilePhone::EBandUnknown: 
			aTelNetworkInfo->iBandInfo=CTelephony::EBandUnknown;
			break;
		case RMobilePhone::E800BandA:
			aTelNetworkInfo->iBandInfo=CTelephony::E800BandA;
			break;
		case RMobilePhone::E800BandB:
			aTelNetworkInfo->iBandInfo=CTelephony::E800BandB;
			break;
		case RMobilePhone::E800BandC:
			aTelNetworkInfo->iBandInfo=CTelephony::E800BandC;
			break;			
		case RMobilePhone::E1900BandA:
			aTelNetworkInfo->iBandInfo=CTelephony::E1900BandA;
			break;
		case RMobilePhone::E1900BandB:
			aTelNetworkInfo->iBandInfo=CTelephony::E1900BandB;
			break;
		case RMobilePhone::E1900BandC:
			aTelNetworkInfo->iBandInfo=CTelephony::E1900BandC;
			break;
		case RMobilePhone::E1900BandD:
			aTelNetworkInfo->iBandInfo=CTelephony::E1900BandD;
			break;
		case RMobilePhone::E1900BandE:
			aTelNetworkInfo->iBandInfo=CTelephony::E1900BandE;
			break;
		case RMobilePhone::E1900BandF:
			aTelNetworkInfo->iBandInfo=CTelephony::E1900BandF;
			break;
		default:
			aTelNetworkInfo->iBandInfo=CTelephony::EBandUnknown;
		}			
	
	aTelNetworkInfo->iCountryCode.Copy(aMMNetworkInfo->iCountryCode);
	aTelNetworkInfo->iNetworkId.Copy(aMMNetworkInfo->iNetworkId);
	aTelNetworkInfo->iDisplayTag.Copy(aMMNetworkInfo->iDisplayTag);
	
	TInt maxLength = aTelNetworkInfo->iShortName.MaxLength();
	if (aMMNetworkInfo->iShortName.Length() > maxLength)
		{
		aTelNetworkInfo->iShortName.Copy(aMMNetworkInfo->iShortName.Ptr(), maxLength);
		}
	else
		{
		aTelNetworkInfo->iShortName.Copy(aMMNetworkInfo->iShortName);		
		}
	
	maxLength = aTelNetworkInfo->iLongName.MaxLength();

	if (aMMNetworkInfo->iLongName.Length() > maxLength)
		{
		aTelNetworkInfo->iLongName.Copy(aMMNetworkInfo->iLongName.Ptr(), maxLength);
		}
	else
		{
		aTelNetworkInfo->iLongName.Copy(aMMNetworkInfo->iLongName);	
		}
	
	aTelNetworkInfo->iCdmaSID.Copy(aMMNetworkInfo->iCdmaSID);
	
	if(aMMArea)
		{
		aTelNetworkInfo->iAreaKnown=aMMArea->iAreaKnown;
		aTelNetworkInfo->iLocationAreaCode=aMMArea->iLocationAreaCode;
		aTelNetworkInfo->iCellId=aMMArea->iCellId;	
		}
	else
		{
		aTelNetworkInfo->iAreaKnown=EFalse;
		aTelNetworkInfo->iLocationAreaCode=0;
		aTelNetworkInfo->iCellId=0;	
		}
	
	//check which paramater class version this is.		
	switch(aMMNetworkInfo->iAccess)
		{
		case RMobilePhone::ENetworkAccessUnknown: 
			aTelNetworkInfo->iAccess=CTelephony::ENetworkAccessUnknown;
			break;
		case RMobilePhone::ENetworkAccessGsm:
			aTelNetworkInfo->iAccess=CTelephony::ENetworkAccessGsm;
			break;
		case RMobilePhone::ENetworkAccessGsmCompact:
			aTelNetworkInfo->iAccess=CTelephony::ENetworkAccessGsmCompact;
			break;
		case RMobilePhone::ENetworkAccessUtran:
			aTelNetworkInfo->iAccess=CTelephony::ENetworkAccessUtran;
			break;			
		default:
			aTelNetworkInfo->iAccess=CTelephony::ENetworkAccessUnknown;
		}						
		
   	LOG( "CNetworkInfo::MapNetworkInfo(), end" );
	}

// ----------------------------------------------------------------------------
// CNetworkInfo::GetCurrentRegisterNw()
// ---------------------------------------------------------------------------- 
RMobilePhone::TMobilePhoneNetworkInfoV2& CNetworkInfo::GetCurrentRegisterNw()
    {
    return iMMNetworkInfo;
    }

// End of file

