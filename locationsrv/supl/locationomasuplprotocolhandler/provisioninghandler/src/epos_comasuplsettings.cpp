/*
* Copyright (c) 2005 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:   SUPL settings class
*
*/




#include <CWPParameter.h>
#include <WPAdapterUtil.h>
//#include <ApUtils.h>

#include "epos_comasupltrace.h"
#include "epos_comasuplsettings.h"
#include "epos_comasuplsettingsrepository.h"
#include "epos_comasuplparameters.h"
#include "epos_omasuplprovhandler.pan"
#include "epos_omasuplprovhandler.hrh"
#include <epos_omasuplprovhandlerres.rsg>

#include <cmmanagerext.h>
#include <cmdestinationext.h>
#include <cmconnectionmethodext.h>
#include <cmconnectionmethoddef.h>


_LIT( KTraceSuplSettings, "epos_comasuplsettings.cpp" );


// -----------------------------------------------------------------------------
// COMASuplSettings::NewL
// Factory method that creates an instance of COMASuplSettings
// -----------------------------------------------------------------------------
//
COMASuplSettings* COMASuplSettings::NewL( COMASuplParameters* aItem, COMASuplTrace* aTrace )
    {
    COMASuplSettings* self = new ( ELeave ) COMASuplSettings( aItem, aTrace );
    CleanupStack::PushL( self );
    self->ConstructL();
    CleanupStack::Pop();
    return self;
    }


// -----------------------------------------------------------------------------    
// Destructor
//
// -----------------------------------------------------------------------------
//
COMASuplSettings::~COMASuplSettings()
    {
    delete iDefaultName;
    delete iItem;
    }


// -----------------------------------------------------------------------------
// COMASuplSettings::SaveL
//
// -----------------------------------------------------------------------------
//
void COMASuplSettings::SaveL( COMASuplSettingsRepository* aRepository )
    {
    iTrace->Trace( _L( "COMASuplSettings::SaveL() Start" ), KTraceSuplSettings, __LINE__ );
     
     HBufC* iapn = NULL;                                       
     _LIT(KNull, "");
     TUint32 iapID = 0;                                      
    // Retrieve APN from Comms DB
    TRAPD(err, iapID = IapIdFromWapIdL( iItem->AssociatedIapId() ) );
    if(err == KErrNone)
    {
    	iapn = iItem->AssociatedIapNameL( iapID, iCommsDb );
    	if ( iapn == NULL )
        {
        iTrace->Trace( _L( "Error Retrieving APN from CommsDB" ), 
                                            KTraceSuplSettings, __LINE__ );
    	User::Leave(KErrNotFound);    
        }
    
    	
    }
    
    // Retrieve HSLP Address
    const TDesC& hslp = iItem->ParameterValue( EWPParameterAddr );
    
    if(iapn!=NULL)
    {
    CleanupStack::PushL( iapn );
    aRepository->SaveL(iapn->Des(), hslp);
    CleanupStack::PopAndDestroy();
  	}
		else
		aRepository->SaveL(KNull, hslp);
	
	    
    }


// -----------------------------------------------------------------------------
// COMASuplSettings::SummaryText
//
// -----------------------------------------------------------------------------
//
const TDesC& COMASuplSettings::SummaryText()
    {    
    return *iDefaultName;
    }

// -----------------------------------------------------------------------------
// COMASuplSettings::SetDefaultNameL
//
// -----------------------------------------------------------------------------
//
void COMASuplSettings::SetDefaultNameL( const TDesC& aDefaultName )
    {
    if ( aDefaultName.Length() )
    	{
	    HBufC* temp = aDefaultName.AllocL();
	    delete iDefaultName;
	    iDefaultName = temp;
    	}
    }

// -----------------------------------------------------------------------------
// COMASuplSettings::SetCommsDataBase
// -----------------------------------------------------------------------------
//
void COMASuplSettings::SetCommsDataBase( CCommsDatabase& aCommsDataBase )
    {
    iCommsDb = &aCommsDataBase;
    }
    

// -----------------------------------------------------------------------------
// COMASuplSettings::IapIdFromWapIdL
//
// -----------------------------------------------------------------------------
//
TUint32 COMASuplSettings::IapIdFromWapIdL( TUint32 aWapId ) const
    {
    /*CApUtils* apUtils = CApUtils::NewLC( *iCommsDb );
    TUint32 iapId = NULL;
    iapId = apUtils->IapIdFromWapIdL( aWapId );
    __ASSERT_ALWAYS( iapId, Panic( SuplAdapterInvalidId ) );
    CleanupStack::PopAndDestroy( apUtils );
    return iapId;
    return ;*/
    
    RCmManagerExt cmManager;
    cmManager.OpenLC(); // CS:1
    RArray<TUint32> iapIds;
    TUint32 iapId( 0 );

    // First get all free IAP ID's.
    cmManager.ConnectionMethodL( iapIds );
    CleanupClosePushL( iapIds ); // CS:2

    // Then get IAP ID's from all destinations.
    RArray<TUint32> destIds;
    cmManager.AllDestinationsL( destIds );
    CleanupClosePushL( destIds ); // CS:3
    TInt destCount = destIds.Count();
    for ( TInt destIndex = 0; destIndex < destCount; destIndex++ )
        {
        RCmDestinationExt dest = cmManager.DestinationL( 
            destIds[destIndex] );
        CleanupClosePushL( dest ); // CS:4
        TInt cmCount = dest.ConnectionMethodCount();
        for ( TInt cmIndex = 0; cmIndex < cmCount; cmIndex++ )
            {
            TUint32 apId = dest.ConnectionMethodL( 
                cmIndex ).GetIntAttributeL( CMManager::ECmIapId );
            iapIds.AppendL( apId );
            }
        CleanupStack::PopAndDestroy( &dest ); // CS:3
        }
    // Finally, go through all connection methods and find correct IAP ID.
    const TInt cmCount = iapIds.Count();
    for ( TInt counter = 0; counter < cmCount; counter++ )
        {
        TUint32 id = cmManager.GetConnectionMethodInfoIntL( 
            iapIds[counter], CMManager::ECmWapId );
        if ( id == aWapId )
            {
            iapId = iapIds[counter];
            // No need to go through rest of IAPs.
            break;
            }
        }
    // PopAndDestroy destIds, iapIds, cmManager.
    CleanupStack::PopAndDestroy( 3, &cmManager ); // CS:0
    return iapId;
    
    }


// -----------------------------------------------------------------------------
// COMASuplSettings::COMASuplSettings
// C++ default constructor can NOT contain any code, that
// might leave.
// -----------------------------------------------------------------------------
//
COMASuplSettings::COMASuplSettings( COMASuplParameters* aItem, COMASuplTrace* aTrace )
                                    :iItem( aItem )
    {
    iTrace = aTrace;
    }
    
    
// -----------------------------------------------------------------------------
// COMASuplSettings::ConstructL
// Symbian 2nd phase constructor can leave.
// -----------------------------------------------------------------------------
//
void COMASuplSettings::ConstructL()
    { 
    TFileName fileName;
    Dll::FileName( fileName );
    iDefaultName = WPAdapterUtil::ReadHBufCL( fileName, KSuplAdapter, 
                                R_QTN_SM_DEFAULT_SUPL_NAME );
    }
