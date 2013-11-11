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
* Description:   Class for handling SUPL provisioning message
*
*/




#include <WPAdapterUtil.h> 
#include <CWPAdapter.h>
#include <CWPCharacteristic.h>
#include <CWPParameter.h>
#include <centralrepository.h>
#include <commdb.h>


#include "epos_omasuplconfigurationkeys.h"
#include "epos_comasupltrace.h"
#include "epos_comasuplprovhandler.h"
#include "epos_comasuplparameters.h"
#include "epos_comasuplsettings.h"
#include "epos_comasuplsettingsrepository.h"
#include "epos_omasuplprovhandler.pan"
#include "epos_omasuplprovhandler.hrh" 
#include <epos_omasuplprovhandlerres.rsg>



_LIT( KTraceProvHandler, "epos_comasuplprovhandler.cpp" );


// ======== MEMBER FUNCTIONS ========

// -----------------------------------------------------------------------------
// COMASuplProvHandler::NewL
// Two-phased constructor.
// -----------------------------------------------------------------------------
//
COMASuplProvHandler* COMASuplProvHandler::NewL()
    {
    COMASuplProvHandler* self = new ( ELeave ) COMASuplProvHandler; 
    CleanupStack::PushL( self );
    self->ConstructL();
    CleanupStack::Pop( self );
    return self;
    }


// -----------------------------------------------------------------------------
// Destructor
// -----------------------------------------------------------------------------
//
COMASuplProvHandler::~COMASuplProvHandler()
    {
    delete iCommsDb;
    delete iRepository;    
    delete iCurrentItem;
    if ( iTitle )
        {
        delete iTitle;
        }
    delete iTrace;
    if ( iSuplSettings )
        {
        iSuplSettings->ResetAndDestroy();
        delete iSuplSettings;
        }
    }
    

// -----------------------------------------------------------------------------
// COMASuplProvHandler::ItemCount
// -----------------------------------------------------------------------------
//
TInt COMASuplProvHandler::ItemCount() const
    {
    if ( iSuplSettings )
        {
        return iSuplSettings->Count();
        }
    return 0;
    }    


// -----------------------------------------------------------------------------
// COMASuplProvHandler::SummaryTitle
// -----------------------------------------------------------------------------
//
const TDesC16& COMASuplProvHandler::SummaryTitle( TInt /*aIndex*/ ) const
    {
    if ( ItemCount() == 0 )
        {
        return KNullDesC;
        }    
    return *iTitle;
    }


// -----------------------------------------------------------------------------
// COMASuplProvHandler::SummaryText
// -----------------------------------------------------------------------------
//
const TDesC16& COMASuplProvHandler::SummaryText( TInt aIndex ) const
    {
    if ( ItemCount() == 0 )
        {
        return KNullDesC;
        }
    return iSuplSettings->At( aIndex )->SummaryText();
    }
    

// -----------------------------------------------------------------------------
// COMASuplProvHandler::SaveL
// -----------------------------------------------------------------------------
//
void COMASuplProvHandler::SaveL( TInt aItem )
    {
    iTrace->Trace( _L("COMASuplProvHandler::SaveL() Start"), 
                                     KTraceProvHandler, __LINE__ );
    __ASSERT_DEBUG( aItem < ItemCount() && 
    aItem >= KFirstArrayIndex, Panic( SuplAdapterArgument ) );

    // Create the commsdb when the save is initiated first time.
    // This stays alive until the adapter is destroyed.
    if ( !iCommsDb )
        {
        iCommsDb = CCommsDatabase::NewL( EDatabaseTypeIAP );
        }    
    User::LeaveIfNull( iCommsDb );
    iSuplSettings->At( aItem )->SetCommsDataBase( *iCommsDb );
    iSuplSettings->At( aItem )->SaveL( iRepository );
    iTrace->Trace( _L("COMASuplProvHandler::SaveL() End"), 
                                    KTraceProvHandler, __LINE__ );
    }
    

// -----------------------------------------------------------------------------
// COMASuplProvHandler::CanSetAsDefault
// -----------------------------------------------------------------------------
//
TBool COMASuplProvHandler::CanSetAsDefault( TInt /*aItem*/ ) const
    {
    return EFalse;
    }


// -----------------------------------------------------------------------------
// COMASuplProvHandler::SetAsDefaultL
// -----------------------------------------------------------------------------
//
void COMASuplProvHandler::SetAsDefaultL( TInt /*aItem*/ )
    {
    }
    

// -----------------------------------------------------------------------------
// COMASuplProvHandler::DetailsL
// -----------------------------------------------------------------------------
//
TInt COMASuplProvHandler::DetailsL( TInt /*aItem*/, MWPPairVisitor& /*aVisitor*/ )
    {
    return KErrNotSupported;
    }


// -----------------------------------------------------------------------------
// COMASuplProvHandler::VisitL
// -----------------------------------------------------------------------------
//
void COMASuplProvHandler::VisitL( CWPCharacteristic& aCharacteristic )
    {
    __ASSERT_VARIANT_ENABLED ( iVariantEnabled )
    
    iTrace->Trace( _L("COMASuplProvHandler::VisitL() Start"), 
                                        KTraceProvHandler, __LINE__ );
    TInt type = aCharacteristic.Type();

    // Check only for APPLICATION specific information
    if ( type == KWPApplication ) 
        {
        iCurrentItem = COMASuplParameters::NewL( iTrace );
        aCharacteristic.AcceptL( *iCurrentItem );

        if( AcceptedSettingGroup() )
            {
            AddSuplSettingsL( iCurrentItem );
            }
        else
            {
            delete iCurrentItem;
            }
        iCurrentItem = NULL;
        }
    }


// -----------------------------------------------------------------------------
// COMASuplProvHandler::VisitL
// -----------------------------------------------------------------------------
//
void COMASuplProvHandler::VisitL( CWPParameter& /*aParameter*/)
    {
    // parameters are handled by COMASuplParameters.
    }


// -----------------------------------------------------------------------------
// COMASuplProvHandler::VisitLinkL
// -----------------------------------------------------------------------------
//
void COMASuplProvHandler::VisitLinkL( CWPCharacteristic& /*aParameter*/)
    {
    // visit link is handled by COMASuplParameters.
    }
   
      
// -----------------------------------------------------------------------------
// COMASuplProvHandler::COMASuplProvHandler
// C++ default constructor can NOT contain any code, that
// might leave.
// -----------------------------------------------------------------------------
//
COMASuplProvHandler::COMASuplProvHandler() : CWPAdapter()
    {
    }


// -----------------------------------------------------------------------------
// COMASuplProvHandler::ConstructL
// Symbian 2nd phase constructor can leave.
// -----------------------------------------------------------------------------
//
void COMASuplProvHandler::ConstructL()
    {
    SetVariation();
    
    __ASSERT_VARIANT_ENABLED ( iVariantEnabled )
    
    iSuplSettings = 
    new ( ELeave ) CArrayPtrFlat<COMASuplSettings>( KArrayGranularity );
    
    iTrace = COMASuplTrace::NewL();

    TFileName fileName;
    Dll::FileName( fileName );
    iTitle = WPAdapterUtil::ReadHBufCL( fileName, KSuplAdapter, 
                                R_QTN_SM_TITLE_SUPL_SETTINGS );
                                
    iRepository = COMASuplSettingsRepository::NewL();
    }


// -----------------------------------------------------------------------------
// COMASuplProvHandler::AcceptedSettingGroup
// -----------------------------------------------------------------------------
//
TBool COMASuplProvHandler::AcceptedSettingGroup()
    {
    return ( iCurrentItem->IsValid() );
    }


// -----------------------------------------------------------------------------
// COMASuplProvHandler::AddSuplSettingsL
// -----------------------------------------------------------------------------
//
void COMASuplProvHandler::AddSuplSettingsL( COMASuplParameters* aItem )
    {
    COMASuplSettings* suplSetting = COMASuplSettings::NewL( aItem, iTrace );
    const TDesC& slpName = aItem->ParameterValue( EWPParameterName );
    suplSetting->SetDefaultNameL( slpName );
    iSuplSettings->AppendL( suplSetting );
    }


// -----------------------------------------------------------------------------
// COMASuplProvHandler::SetVariation
// Initializes the local variation flag
// -----------------------------------------------------------------------------
//
void COMASuplProvHandler::SetVariation()
    {
    iVariantEnabled = ETrue;
    CRepository *repository = NULL;
    TRAPD( err, repository = CRepository::NewL( KSUPLVarRepositoryUid ) );
    if ( KErrNone != err )
        {
        return;        
        }
    TInt val;
    TInt rc = repository->Get( KOMASuplClientProvHandlerVariant, val );
    if ( KErrNone == rc && val == 0 )
        {
        iVariantEnabled = EFalse;
        }
    delete repository;
    }
