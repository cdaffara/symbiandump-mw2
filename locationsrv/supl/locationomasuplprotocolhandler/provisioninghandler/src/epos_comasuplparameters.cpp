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
* Description:   Class to contain SUPL parameters
*
*/




#include <CWPCharacteristic.h>
#include <CWPParameter.h>
#include <commdb.h>

#include "epos_comasupltrace.h"
#include "epos_comasuplparameters.h"
#include "epos_omasuplprovhandler.pan"
#include "epos_omasuplprovhandler.hrh"


_LIT( KTraceSuplParams, "epos_comasuplparameters.cpp" );


// -----------------------------------------------------------------------------
// COMASuplParameters::NewL
// 
// -----------------------------------------------------------------------------
//
COMASuplParameters* COMASuplParameters::NewL( COMASuplTrace* aTrace )
    {
    COMASuplParameters* self = COMASuplParameters::NewLC( aTrace ); 
    CleanupStack::Pop( self );
    return self;
    }


// -----------------------------------------------------------------------------
// COMASuplParameters::NewLC
// 
// -----------------------------------------------------------------------------
//
COMASuplParameters* COMASuplParameters::NewLC( COMASuplTrace* aTrace )
    {
    COMASuplParameters* self = new ( ELeave ) COMASuplParameters ( aTrace );
    CleanupStack::PushL( self );
    self->ConstructL();
    return self;
    }


// -----------------------------------------------------------------------------
// COMASuplParameters::Destructor
// 
// -----------------------------------------------------------------------------
//
COMASuplParameters::~COMASuplParameters()
    {
    delete iSuplSettingParams;    
    }


// -----------------------------------------------------------------------------
// COMASuplParameters::ParameterValue
// Returns the parameter value
// -----------------------------------------------------------------------------
//
const TDesC& COMASuplParameters::ParameterValue( TInt aParamID ) const
    {
    TInt count = iSuplSettingParams->Count();
    for ( TInt i = 0; i < count; ++i )
        {
        if ( iSuplSettingParams->At( i )->ID() == aParamID )
            {
            return iSuplSettingParams->At( i )->Value();
            }
        }
    return KNullDesC;
    }


// -----------------------------------------------------------------------------
// COMASuplParameters::AssociatedIapId
// 
// -----------------------------------------------------------------------------
//
TUint32 COMASuplParameters::AssociatedIapId()
    {
    TPckgBuf<TUint32> pckg;
    pckg() = 0;

    if ( iNapDef )
        {
        const TDesC8& data = iNapDef->Data();
        pckg.Copy( data );
        }

    return pckg();
    }


// -----------------------------------------------------------------------------
// COMASuplParameters::AssociatedIapName
//
// -----------------------------------------------------------------------------
//
HBufC* COMASuplParameters::AssociatedIapNameL( TUint32 aWapId, 
                                                   CCommsDatabase* aCommsDb ) const
    {
	CCommsDbTableView* tableView = aCommsDb->OpenIAPTableViewMatchingBearerSetLC( 
	                        ECommDbBearerCSD|ECommDbBearerGPRS, 
	                        ECommDbConnectionDirectionOutgoing );
	                        
	TInt rc = tableView->GotoFirstRecord();
	HBufC* iapName = NULL;
	
   	TUint32 iapID;
    while ( rc == KErrNone ) 
   	    {
        tableView->ReadUintL( TPtrC( COMMDB_ID ), iapID );
        if ( iapID == aWapId ) 
            {
            iapName = tableView->ReadLongTextLC( TPtrC( COMMDB_NAME ) );
            CleanupStack::Pop();    // iapName
            break;
            }
        rc = tableView->GotoNextRecord();
        }

    CleanupStack::PopAndDestroy();  // tableView
    return iapName;
    }


// -----------------------------------------------------------------------------
// COMASuplParameters::IsValid
// 
// -----------------------------------------------------------------------------
//
TBool COMASuplParameters::IsValid() const
    {
    // Check APPID
    const TDesC& appID = ParameterValue( EWPParameterAppID );
    if ( appID == KApplicationSupl )
        {
        return ETrue;
        }
    return EFalse;
    }


// -----------------------------------------------------------------------------
// COMASuplParameters::VisitL
// 
// -----------------------------------------------------------------------------
//
void COMASuplParameters::VisitL( CWPCharacteristic& aCharacteristic )
    {
    iTrace->Trace( _L("COMASuplParameters::VisitL(CWPCharacteristic) Start"), KTraceSuplParams, __LINE__ );
    switch ( aCharacteristic.Type() )
        {	
        case KWPAppAddr:
            aCharacteristic.AcceptL( *this );
            break;

        default:
            break;
        }
    }


// -----------------------------------------------------------------------------
// COMASuplParameters::VisitL
// 
// -----------------------------------------------------------------------------
//
void COMASuplParameters::VisitL( CWPParameter& aParameter )
    {
    iTrace->Trace( _L("COMASuplParameters::VisitL(CWPParameter) Start"), 
                                    KTraceSuplParams, __LINE__ );    
    switch( aParameter.ID() )
        {
        case EWPParameterAppID:
        
        case EWPParameterName:
        
        case EWPParameterProviderID:
        
        case EWPParameterAddr:
            iSuplSettingParams->AppendL( &aParameter );
            break;

        default:
            break;
        }
    }


// -----------------------------------------------------------------------------
// COMASuplParameters::VisitLinkL
// 
// -----------------------------------------------------------------------------
//
void COMASuplParameters::VisitLinkL( CWPCharacteristic& aCharacteristic )
    {
    iTrace->Trace( _L("COMASuplParameters::VisitLinkL(CWPCharacteristic) Start"), 
                                    KTraceSuplParams, __LINE__ );    
    if ( aCharacteristic.Type() ==  KWPNapDef )
        {
        iNapDef = &aCharacteristic;
        }
    }


// -----------------------------------------------------------------------------
// COMASuplParameters::Constructor
// 
// -----------------------------------------------------------------------------
//
COMASuplParameters::COMASuplParameters( COMASuplTrace* aTrace )
    {
    iTrace = aTrace;
    }


// -----------------------------------------------------------------------------
// COMASuplParameters::ConstructL
// Symbian 2nd phase constructor can leave.
// -----------------------------------------------------------------------------
//
void COMASuplParameters::ConstructL()
    {
    // average setting contains 10 items
    iSuplSettingParams = new ( ELeave ) CArrayPtrFlat<CWPParameter>( 
    KArrayGranularity );
    }
