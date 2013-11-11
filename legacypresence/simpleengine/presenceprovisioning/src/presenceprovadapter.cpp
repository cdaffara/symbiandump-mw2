/*
* Copyright (c) 2006 Nokia Corporation and/or its subsidiary(-ies).
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
* Description:    Implementation for main Adapter class
*
*/




// INCLUDE FILES
#include <CWPCharacteristic.h>
#include <WPAdapterUtil.h>
#include <presenceprovisioningtexts.rsg>
#include <pressettingsapi.h>

#include "presenceprovitem.h"
#include "presenceprovadapter.h"
#include "presenceprovlogger.h"


// ============================ MEMBER FUNCTIONS ===============================

// -----------------------------------------------------------------------------
// CPresProvAdapter::CPresProvAdapter
//
// -----------------------------------------------------------------------------
//
CPresProvAdapter::CPresProvAdapter()
    {    
    }

// -----------------------------------------------------------------------------
// CPresProvAdapter::ConstructL
// Symbian 2nd phase constructor can leave.
// -----------------------------------------------------------------------------
//
void CPresProvAdapter::ConstructL()
    {
    PRES_PROV_LOG(LOG_LIT8( "CPresProvItem::ConstructL"));
    // AdapterUtil needs the name of the resource file without the prefix and
    // the path returned by Dll::FileName. Note that Dll::FileName is an
    // undocumented method.
    //
    _LIT( KPresAdapterName, "PresenceProvisioningTexts");  //  presenceprovisioningtexts.rss
    TFileName fileName;
    Dll::FileName(fileName);
   
    iTitle       = WPAdapterUtil::ReadHBufCL( fileName,
                                              KPresAdapterName,
                                              R_QTN_OP_HEAD_PRESENCE);
    iDefaultName = WPAdapterUtil::ReadHBufCL( fileName,
                                              KPresAdapterName,
                                              R_QTN_OP_HEAD_PRESENCE);
                                              
    PRES_PROV_LOG(LOG_LIT8( " iTitle: %S"), &(*iTitle));
    PRES_PROV_LOG(LOG_LIT8( " iDefaultName: %S"), &(*iDefaultName));
    }

// -----------------------------------------------------------------------------
// CPresProvAdapter::NewL
// Two-phased constructor.
// -----------------------------------------------------------------------------
//
CPresProvAdapter* CPresProvAdapter::NewL()
    {
    PRES_PROV_LOG(LOG_LIT8( "CPresProvItem::NewL"));
    CPresProvAdapter* self = new(ELeave) CPresProvAdapter;
    CleanupStack::PushL(self);
    self->ConstructL();
    CleanupStack::Pop();
    return self;
    }

// -----------------------------------------------------------------------------
// CPresProvAdapter::~CPresProvAdapter
// Destructor
// -----------------------------------------------------------------------------
//
CPresProvAdapter::~CPresProvAdapter()
    {
    
    PRES_PROV_LOG(LOG_LIT8( "CPresProvAdapter::~CPresProvAdapter" ) );
    delete iTitle;
    delete iDefaultName;
    iProvItems.ResetAndDestroy();
    iProvItems.Close();
    }

// -----------------------------------------------------------------------------
// CPresProvAdapter::ItemCount
// -----------------------------------------------------------------------------
//
TInt CPresProvAdapter::ItemCount() const
    {
    PRES_PROV_LOG(LOG_LIT8( "CPresProvAdapter::ItemCount returns %d" ), iProvItems.Count());
    return iProvItems.Count();  
    }

// -----------------------------------------------------------------------------
// CPresProvAdapter::SummaryTitle
// -----------------------------------------------------------------------------
//
const TDesC16& CPresProvAdapter::SummaryTitle(TInt /*aIndex*/) const
    {
    PRES_PROV_LOG(LOG_LIT8( "CPresProvAdapter::SummaryTitle" ) );
    return *iTitle;
    }

// -----------------------------------------------------------------------------
// CPresProvAdapter::SummaryText
// -----------------------------------------------------------------------------
//
const TDesC16& CPresProvAdapter::SummaryText( TInt aIndex ) const
    {
    PRES_PROV_LOG(LOG_LIT8( "CPresProvAdapter::SummaryText" ) );
    if( aIndex < iProvItems.Count() )
        {
        if( iProvItems[aIndex]->Name().Length() > 0 )
            {
            return iProvItems[aIndex]->Name();
            }
        }
    return *iDefaultName;   
    }
    
// -----------------------------------------------------------------------------
// CPresProvAdapter::SaveL
// -----------------------------------------------------------------------------
//
void CPresProvAdapter::SaveL( TInt aItem )
    {
    PRES_PROV_LOG(LOG_LIT8( "CPresProvAdapter::SaveL done" ) );
    if ( aItem < iProvItems.Count() - 1 )
        {
        PRES_PROV_LOG(LOG_LIT8( " CPresProvAdapter::SaveL leaves with KErrArgument" ) );
        User::Leave ( KErrArgument );
        }
    iProvItems[aItem]->SaveL();
    PRES_PROV_LOG(LOG_LIT8( " CPresProvAdapter::SaveL done" ) );
    }

// -----------------------------------------------------------------------------
// CPresProvAdapter::CanSetAsDefault
// -----------------------------------------------------------------------------
//
TBool CPresProvAdapter::CanSetAsDefault( TInt /*aItem*/ ) const
    {
    PRES_PROV_LOG(LOG_LIT8( "CPresProvAdapter::CanSetAsDefault" ) );
    return EFalse;
    }

// -----------------------------------------------------------------------------
// CPresProvAdapter::SetAsDefaultL
// -----------------------------------------------------------------------------
//
void CPresProvAdapter::SetAsDefaultL( TInt /*aItem*/ )
    {
    PRES_PROV_LOG(LOG_LIT8( "CPresProvAdapter::SetAsDefaultL" ) );
    }

// -----------------------------------------------------------------------------
// CPresProvAdapter::DetailsL
//
// -----------------------------------------------------------------------------
//
TInt CPresProvAdapter::DetailsL( TInt /*aItem*/, MWPPairVisitor& /*aVisitor*/ )
    {
    PRES_PROV_LOG(LOG_LIT8( "CPresProvAdapter::DetailsL" ) );
    return KErrNotSupported;
    }

// -----------------------------------------------------------------------------
// CPresProvAdapter::VisitL
// -----------------------------------------------------------------------------
//
void CPresProvAdapter::VisitL( CWPCharacteristic& aCharacteristic )
    {
    PRES_PROV_LOG(LOG_LIT8( "CPresProvAdapter::VisitL( CWPCharacteristic& aCharacteristic)" ) );
    if( aCharacteristic.Type() == KWPApplication )
        {       
        CPresProvItem* item = CPresProvItem::NewL();
        CleanupStack::PushL( item );
        aCharacteristic.AcceptL( *item );
    
        if( item->Validate() )
            {
            iProvItems.AppendL( item );
            CleanupStack::Pop( item );
            }
        else
            {
            CleanupStack::PopAndDestroy( item );                                
            }
        }
    }
        
// -----------------------------------------------------------------------------
// CPresProvAdapter::VisitL
// -----------------------------------------------------------------------------
//
void CPresProvAdapter::VisitL( CWPParameter& /*aParameter*/ )
    {
    PRES_PROV_LOG(LOG_LIT8( "CPresProvAdapter::VisitL( CWPParameter& /*aParameter*/" ) );
    // Not used.
    }

// -----------------------------------------------------------------------------
// CPresProvAdapter::VisitLinkL
// -----------------------------------------------------------------------------
//
void CPresProvAdapter::VisitLinkL( CWPCharacteristic& /*aLink*/ )
    {
    PRES_PROV_LOG(LOG_LIT8( "CPresProvAdapter::VisitLinkL( CWPCharacteristic& aLink" ) );
    // Not used.
    }
    
// -----------------------------------------------------------------------------
// CPresProvAdapter::SettingsSavedL
// -----------------------------------------------------------------------------
void CPresProvAdapter::SettingsSavedL( const TDesC8& aAppIdOfSavingItem,
                                      const TDesC8& aAppRef, 
                                      const TDesC8& aStorageIdValue )
    {
    PRES_PROV_LOG(LOG_LIT8( "CPresProvAdapter::SettingsSavedL" ) );
    PRES_PROV_LOG(LOG_LIT8( "   aAppIdOfSavingItem: %S, aAppRef: %S, aStorageIdValue: %S " )
                                ,&aAppIdOfSavingItem, &aAppRef, &aStorageIdValue);

    // Convert aAppRef
    HBufC* appRef16 = HBufC::NewLC( aAppRef.Length() ); // << aAppRef16
    appRef16->Des().Copy( aAppRef );
    
    // Offer application references to all provisioned Pres settings
    // Only 2 possible, so performance doesnt matter
    for( TInt i(0); i < iProvItems.Count(); i++ )
        {
        CPresProvItem* item = iProvItems[i];
        const RArray<TPtrC>& toAppRefs = item->ToAppReferences();
    
        // Find application reference that is being offered
        for( TInt j(0); j < toAppRefs.Count(); j++ )
            {
            const TDesC& toAppRef = toAppRefs[j];
            if( !toAppRef.Compare( *appRef16 ) )
                {
                // Set correct application reference
                if( !aAppIdOfSavingItem.Compare(KPresProvisioningSipId) )
                    {
                    item->SetSipSetIdL( DesToInt(aStorageIdValue) );
                    }
                else if( !aAppIdOfSavingItem.Compare(KPresProvisioningXdmId) )
                    {
                    item->SetXDMSetIdL( DesToInt(aStorageIdValue) );
                    }   
                }               
            }
        }
    CleanupStack::PopAndDestroy( appRef16 );    // >>> appRef16
    }

// -----------------------------------------------------------------------------
// CPresProvAdapter::GetSavingInfoL
// -----------------------------------------------------------------------------
void CPresProvAdapter::GetSavingInfoL( TInt aIndex,
                                      RPointerArray<HBufC8>& aSavingInfo )
    {
    PRES_PROV_LOG(LOG_LIT8( "CPresProvAdapter::GetSavingInfoL" ) );
    
    if ( aIndex < iProvItems.Count() - 1 )
        {
    PRES_PROV_LOG(LOG_LIT8( " CPresProvAdapter::GetSavingInfoL leaves with KErrArgument" ) );

        User::Leave ( KErrArgument );
        }
    TInt length ( iProvItems[aIndex]->AppReference().Length() );
    // if the APPREF exists
    if ( length > 0 )
        {
        // APPID into place [0].
        TPtrC8 appId ( KPresProvisioningAppId8 );
        HBufC8* appIdBuf = appId.AllocLC();             // << appIdBuf
        aSavingInfo.Append( appIdBuf );
        CleanupStack::Pop( appIdBuf );                  // >> appIdBuf
        appIdBuf = NULL;
        
        // APPREF into place [1].
        HBufC8* appRefBuf = HBufC8::NewLC( length );    // << appRefBuf
        TPtr8 appRef = appRefBuf->Des();
        appRef.Copy( iProvItems[aIndex]->AppReference() );
        aSavingInfo.Append( appRefBuf );
        CleanupStack::Pop( appRefBuf );                 // >> appRefBuf
        appRefBuf = NULL;
        
        // Pres settings id into place [2]. 
        // 10 = max length of 32bit integer
        HBufC8* idBuf = HBufC8::NewLC( 10 );            // << idBuf
        TPtr8 id = idBuf->Des();
        id.Num( iProvItems[aIndex]->PresSetId() );
        aSavingInfo.Append( idBuf );
        CleanupStack::Pop( idBuf );                     // >> idBuf
        idBuf = NULL;
        }
        
    PRES_PROV_LOG(LOG_LIT8( " CPresProvAdapter::GetSavingInfoL done" ) );

    }
    
// -----------------------------------------------------------------------------
// CPresProvAdapter::SavingFinalizedL()
//
// -----------------------------------------------------------------------------
void CPresProvAdapter::SavingFinalizedL()
    {
    PRES_PROV_LOG(LOG_LIT8( "CPresProvAdapter::SavingFinalizedL" ) );
    }
    
// ----------------------------------------------------------------------------
// CPresProvAdapter::DesToInt
// ----------------------------------------------------------------------------
TInt CPresProvAdapter::DesToInt( const TDesC8& aSrcDes ) const
    {
    TLex8 lex( aSrcDes );
    TInt value ( KErrNotFound );
    lex.Val( value );
    return value;
    }    
    
//  End of File  
