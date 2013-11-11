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
* Description:    XDM Provisioning Adapter implementation.
*
*/





// INCLUDE FILES
#include <CWPCharacteristic.h>
#include <WPAdapterUtil.h>
#include <xdmprovisioningtexts.rsg>
#include <XdmSettingsApi.h>

#include "xdmprovitem.h"
#include "xdmprovadapter.h"


#ifdef _DEBUG
    #include <flogger.h>
#endif


// ============================ MEMBER FUNCTIONS ===============================

// -----------------------------------------------------------------------------
// CXdmProvAdapter::CXdmProvAdapter
//
// -----------------------------------------------------------------------------
//
CXdmProvAdapter::CXdmProvAdapter():
iDefaultName( NULL ),
iTitle( NULL ),
iProvItems( 1 )
    {    
    }

// -----------------------------------------------------------------------------
// CXdmProvAdapter::ConstructL
// Symbian 2nd phase constructor can leave.
// -----------------------------------------------------------------------------
//
void CXdmProvAdapter::ConstructL()
    {
    // AdapterUtil needs the name of the resource file without the prefix and
    // the path returned by Dll::FileName. Note that Dll::FileName is an
    // undocumented method.
    //
    _LIT( KXdmAdapterName, "xdmprovisioningtexts");  //  xdmprovisioningtexts.rss
    TFileName fileName;
    Dll::FileName(fileName);
   
    iTitle       = WPAdapterUtil::ReadHBufCL( fileName,
                                              KXdmAdapterName,
                                              R_QTN_SM_HEAD_CONNECTION);
    iDefaultName = WPAdapterUtil::ReadHBufCL( fileName,
                                              KXdmAdapterName,
                                              R_QTN_XDM_PROVISIONING_NAME);
    }

// -----------------------------------------------------------------------------
// CXdmProvAdapter::NewL
// Two-phased constructor.
// -----------------------------------------------------------------------------
//
CXdmProvAdapter* CXdmProvAdapter::NewL()
    {
#ifdef _DEBUG 
    WriteToLog( _L8( "CXdmProvAdapter::NewL" ) );
#endif        
    CXdmProvAdapter* self = new(ELeave) CXdmProvAdapter;
    CleanupStack::PushL(self);
    self->ConstructL();
    CleanupStack::Pop();
    return self;
    }

// -----------------------------------------------------------------------------
// CXdmProvAdapter::~CXdmProvAdapter
// Destructor
// -----------------------------------------------------------------------------
//
CXdmProvAdapter::~CXdmProvAdapter()
    {
#ifdef _DEBUG 
    WriteToLog( _L8( "CXdmProvAdapter::~CXdmProvAdapter" ) );
#endif   
    delete iTitle;
    delete iDefaultName;
    iProvItems.ResetAndDestroy();
    iProvItems.Close();
    }

// -----------------------------------------------------------------------------
// CXdmProvAdapter::ItemCount
// -----------------------------------------------------------------------------
//
TInt CXdmProvAdapter::ItemCount() const
    {
#ifdef _DEBUG 
    WriteToLog( _L8( "CXdmProvAdapter::ItemCount returns %d" ), iProvItems.Count());
#endif   
    return iProvItems.Count();  
    }

// -----------------------------------------------------------------------------
// CXdmProvAdapter::SummaryTitle
// -----------------------------------------------------------------------------
//
const TDesC16& CXdmProvAdapter::SummaryTitle(TInt /*aIndex*/) const
    {
#ifdef _DEBUG 
    WriteToLog( _L8( "CXdmProvAdapter::SummaryTitle" ) );
#endif   
    return *iTitle;
    }

// -----------------------------------------------------------------------------
// CXdmProvAdapter::SummaryText
// -----------------------------------------------------------------------------
//
const TDesC16& CXdmProvAdapter::SummaryText( TInt aIndex ) const
    {
#ifdef _DEBUG 
    WriteToLog( _L8( "CXdmProvAdapter::SummaryText" ) );
#endif  
    TInt count = iProvItems.Count();
    if( aIndex < count )
        {
        if( 0 < iProvItems[aIndex]->Name().Length() )
            {
            return iProvItems[aIndex]->Name();
            }
        }
    return *iDefaultName;   
    }
    
// -----------------------------------------------------------------------------
// CXdmProvAdapter::SaveL
// -----------------------------------------------------------------------------
//
void CXdmProvAdapter::SaveL( TInt aItem )
    {
    if ( aItem < iProvItems.Count() - 1 )
        {
#ifdef _DEBUG 
    WriteToLog( _L8( "CXdmProvAdapter::SaveL leaves with KErrArgument" ) );
#endif
        User::Leave ( KErrArgument );
        }
    iProvItems[aItem]->SaveL();
#ifdef _DEBUG 
    WriteToLog( _L8( "CXdmProvAdapter::SaveL done" ) );
#endif
    }

// -----------------------------------------------------------------------------
// CXdmProvAdapter::CanSetAsDefault
// -----------------------------------------------------------------------------
//
TBool CXdmProvAdapter::CanSetAsDefault( TInt /*aItem*/ ) const
    {
#ifdef _DEBUG 
    WriteToLog( _L8( "CXdmProvAdapter::CanSetAsDefault" ) );
#endif   
    return EFalse;
    }

// -----------------------------------------------------------------------------
// CXdmProvAdapter::SetAsDefaultL
// -----------------------------------------------------------------------------
//
void CXdmProvAdapter::SetAsDefaultL( TInt /*aItem*/ )
    {
#ifdef _DEBUG 
    WriteToLog( _L8( "CXdmProvAdapter::SetAsDefaultL" ) );
#endif   
    }

// -----------------------------------------------------------------------------
// CXdmProvAdapter::DetailsL
//
// -----------------------------------------------------------------------------
//
TInt CXdmProvAdapter::DetailsL( TInt /*aItem*/, MWPPairVisitor& /*aVisitor*/ )
    {
#ifdef _DEBUG 
    WriteToLog( _L8( "CXdmProvAdapter::DetailsL" ) );
#endif
    return KErrNotSupported;
    }

// -----------------------------------------------------------------------------
// CXdmProvAdapter::VisitL
// -----------------------------------------------------------------------------
//
void CXdmProvAdapter::VisitL( CWPCharacteristic& aCharacteristic )
    {
#ifdef _DEBUG 
    WriteToLog( _L8( "CXdmProvAdapter::VisitL( CWPCharacteristic& aCharacteristic)" ) );
#endif   
    if( KWPApplication == aCharacteristic.Type()  )
        {       
        CXdmProvItem* item = CXdmProvItem::NewL();
        CleanupStack::PushL( item );    // << item
        aCharacteristic.AcceptL( *item );
    
        if( item->Validate() )
            {
            iProvItems.AppendL( item );
            CleanupStack::Pop( item );  // >> item
            }
        else
            {
            CleanupStack::PopAndDestroy( item );    // >>> item                                
            }
        }
    }
        
// -----------------------------------------------------------------------------
// CXdmProvAdapter::VisitL
// -----------------------------------------------------------------------------
//
void CXdmProvAdapter::VisitL( CWPParameter& /*aParameter*/ )
    {
#ifdef _DEBUG 
    WriteToLog( _L8( "CXdmProvAdapter::VisitL( CWPParameter& /*aParameter*/" ) );
#endif
    // Not used.
    }

// -----------------------------------------------------------------------------
// CXdmProvAdapter::VisitLinkL
// -----------------------------------------------------------------------------
//
void CXdmProvAdapter::VisitLinkL( CWPCharacteristic& /*aLink*/ )
    {
#ifdef _DEBUG 
    WriteToLog( _L8( "CXdmProvAdapter::VisitLinkL( CWPCharacteristic& aLink" ) );
#endif
    // Not used.
    }
    
// -----------------------------------------------------------------------------
// CXdmProvAdapter::SettingsSavedL
//
// -----------------------------------------------------------------------------
void CXdmProvAdapter::SettingsSavedL( const TDesC8& aAppIdOfSavingItem,
                                      const TDesC8& aAppRef, 
                                      const TDesC8& aStorageIdValue )
    {
#ifdef _DEBUG 
    WriteToLog( _L8( "CXdmProvAdapter::SettingsSavedL" ) );
#endif
    // Convert aAppRef
    HBufC* appRef16 = HBufC::NewLC( aAppRef.Length() ); // << aAppRef16
    appRef16->Des().Copy( aAppRef );
    
    // Offer application references to all provisioned XDM settings
    TInt provItemsCount( iProvItems.Count() );
    for( TInt i(0); i < provItemsCount; i++ )
        {
        CXdmProvItem* item = iProvItems[i];
        const RArray<TPtrC>& toAppRefs = item->ToAppReferences();
    
        // Find application reference that is being offered
        TInt toAppRefsCount( toAppRefs.Count() );
        for( TInt j(0); j < toAppRefsCount; j++ )
            {
            const TDesC& toAppRef = toAppRefs[j];
            if( !toAppRef.Compare( *appRef16 ) )
                {
                // Set correct application reference
                if( !aAppIdOfSavingItem.Compare( KXdmProvSipIdentifier ) )
                    {
                    item->SetSipSettingsIdL( aStorageIdValue );
                    break;
                    }   
                }               
            }
        }
        CleanupStack::PopAndDestroy( appRef16 );    // >>> appRef16
    }

// -----------------------------------------------------------------------------
// CXdmProvAdapter::GetSavingInfoL
//
// -----------------------------------------------------------------------------
void CXdmProvAdapter::GetSavingInfoL( TInt aIndex,
                                      RPointerArray<HBufC8>& aSavingInfo )
    {
#ifdef _DEBUG 
    WriteToLog( _L8( "CXdmProvAdapter::GetSavingInfoL" ) );
#endif    
    if ( aIndex < iProvItems.Count() - 1 )
        {
#ifdef _DEBUG 
    WriteToLog( _L8( "CXdmProvAdapter::GetSavingInfoL leaves with KErrArgument" ) );
#endif
        User::Leave ( KErrArgument );
        }
    TInt length ( iProvItems[aIndex]->AppReference().Length() );
    // if the APPREF exists
    if ( 0 < length )
        {
        // APPID into place [0].
        TPtrC8 appId ( KXdmProvAppId8 );
        HBufC8* appIdBuf = appId.AllocLC();             // << appIdBuf
        aSavingInfo.AppendL( appIdBuf );
        CleanupStack::Pop( appIdBuf );                  // >> appIdBuf
        appIdBuf = NULL;
        
        // APPREF into place [1].
        HBufC8* appRefBuf = HBufC8::NewLC( length );    // << appRefBuf
        TPtr8 appRef = appRefBuf->Des();
        appRef.Copy( iProvItems[aIndex]->AppReference() );
        aSavingInfo.AppendL( appRefBuf );
        CleanupStack::Pop( appRefBuf );                 // >> appRefBuf
        appRefBuf = NULL;
        
        // Xdm settings id into place [2]. 
        HBufC8* idBuf = HBufC8::NewLC( KXdmProvIntMaxLength );            // << idBuf
        TPtr8 id = idBuf->Des();
        id.Num( iProvItems[aIndex]->XdmSettingsId() );
        aSavingInfo.AppendL( idBuf );
        CleanupStack::Pop( idBuf );                     // >> idBuf
        idBuf = NULL;
        }
        
#ifdef _DEBUG 
    WriteToLog( _L8( "CXdmProvAdapter::GetSavingInfoL done" ) );
#endif
    }
    
// -----------------------------------------------------------------------------
// CXdmProvAdapter::SavingFinalizedL()
//
// -----------------------------------------------------------------------------
void CXdmProvAdapter::SavingFinalizedL()
    {
#ifdef _DEBUG 
    WriteToLog( _L8( "CXdmProvAdapter::SavingFinalizedL" ) );
#endif   
    }
    
    
// ----------------------------------------------------
// CXdmProvAdapter::WriteToLog
// _DEBUG only
// ----------------------------------------------------
//
#ifdef _DEBUG
void CXdmProvAdapter::WriteToLog( TRefByValue<const TDesC8> aFmt,... )                                 
    {
    VA_LIST list;
    VA_START( list, aFmt );
    TBuf8<KXdmProvAdapterBufferMaxSize> buf;
    buf.FormatList( aFmt, list );
    RFileLogger::Write( KXdmProvAdapterLogDir, KXdmProvAdapterLogFile, EFileLoggingModeAppend, buf );
    }
#endif
//  End of File  
