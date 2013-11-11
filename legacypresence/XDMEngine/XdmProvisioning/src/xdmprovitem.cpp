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
* Description:    Provisioning item.
*
*/




// INCLUDE FILES
#include <CWPParameter.h>
#include <CWPCharacteristic.h>
#include <commdb.h>
#include <XdmSettingsCollection.h>
#include <XdmSettingsApi.h>

#include "xdmprovitem.h"

#ifdef _DEBUG
    #include "xdmprovadapter.h"
#endif
// ============================ MEMBER FUNCTIONS ===============================

// -----------------------------------------------------------------------------
// CXdmProvItem::CXdmProvItem
// 
// -----------------------------------------------------------------------------
//
CXdmProvItem::CXdmProvItem():
    iXdmSettingsId( KErrNotFound ),
    iAppId( KNullDesC ),
    iAppName( KNullDesC ),
    iProviderId( KNullDesC ),
    iSipReference( NULL ),
    iToNapId( KNullDesC ),
    iNapId( NULL ),
    iNapName( KNullDesC ),
    iAppRef( KNullDesC ),
    iXcapRootUri( KNullDesC ),
    iUserName( KNullDesC ),
    iPassword( KNullDesC ),
    iAuthType( KNullDesC ),
    iSettingsSaved( EFalse )
    {
    }

// -----------------------------------------------------------------------------
// CXdmProvItem::NewL
// Two-phased constructor.
// -----------------------------------------------------------------------------
//
CXdmProvItem* CXdmProvItem::NewL()
    {
    CXdmProvItem* self = new (ELeave) CXdmProvItem();
    return self;
    }

// -----------------------------------------------------------------------------
// CXdmProvItem::~CXdmProvItem
// -----------------------------------------------------------------------------
//
CXdmProvItem::~CXdmProvItem()
    {
    iToAppReferences.Close();
    delete iSipReference;
    delete iNapId;
    }    

// -----------------------------------------------------------------------------
// CXdmProvItem::Validate
// -----------------------------------------------------------------------------
//
TBool CXdmProvItem::Validate() const
    {
    if ( !iAppId.Length() )
        {
#ifdef _DEBUG 
    CXdmProvAdapter::WriteToLog( _L8( "CXdmProvItem::Validate  iAppId missing" ) );
#endif
        return EFalse;
        }
    else if ( iAppId.Compare( KXdmProvAppId ) )
        {
#ifdef _DEBUG 
    CXdmProvAdapter::WriteToLog( _L8( "CXdmProvItem::Validate wrong iAppId" ) );
#endif
        return EFalse;
        }
    else if ( !iAppName.Length() )
        {
#ifdef _DEBUG 
    CXdmProvAdapter::WriteToLog( _L8( "CXdmProvItem::Validate iAppName missing" ) );
#endif
        return EFalse;
        }
     else if ( !iToNapId.Length() )
        {
#ifdef _DEBUG 
    CXdmProvAdapter::WriteToLog( _L8( "CXdmProvItem::Validate iNapId missing" ) );
#endif
        return EFalse;
        }
     else if ( !iXcapRootUri.Length() )
        {
#ifdef _DEBUG 
    CXdmProvAdapter::WriteToLog( _L8( "CXdmProvItem::Validate iXcapRootUri missing" ) );
#endif
        return EFalse;
        }
    else
        {
#ifdef _DEBUG 
    CXdmProvAdapter::WriteToLog( _L8( "CXdmProvItem::Validate returns true" ) );
#endif
        return ETrue;   
        }
    }
        
// -----------------------------------------------------------------------------
// CXdmProvItem::Name
// -----------------------------------------------------------------------------
//  
const TDesC& CXdmProvItem::Name() const
    {
    return iAppName; 
    }

// -----------------------------------------------------------------------------
// CXdmProvItem::ApplicationId
// -----------------------------------------------------------------------------
//  
const TDesC& CXdmProvItem::ApplicationId() const
    {
    return iAppId;
    }
    
// -----------------------------------------------------------------------------
// CXdmProvItem::XdmSettingsId
// -----------------------------------------------------------------------------
//  
TInt CXdmProvItem::XdmSettingsId() const
    {
    return iXdmSettingsId;
    }
    
// -----------------------------------------------------------------------------
// CXdmProvItem::AppReference
// -----------------------------------------------------------------------------
//  
const TDesC& CXdmProvItem::AppReference() const
    {
    return iAppRef;
    }
    
// -----------------------------------------------------------------------------
// CXdmProvItem::SaveL
// -----------------------------------------------------------------------------
//  
TInt CXdmProvItem::SaveL()
    {
#ifdef _DEBUG 
    CXdmProvAdapter::WriteToLog( _L8( "CXdmProvItem::SaveL" ) );
#endif
 
    if( !iSettingsSaved )
        {
        CXdmSettingsCollection* settings = new (ELeave) CXdmSettingsCollection();
        CleanupStack::PushL( settings );            // << settings
        settings->AppendL( iAppId, EXdmPropAppId );
        CheckExistingNamesL();
        settings->AppendL( iAppName, EXdmPropName );
        settings->AppendL( iProviderId, EXdmPropProvId );
        FindAccessPointIdL();              
        settings->AppendL( *iNapId, EXdmPropToNapId );
        if( iSipReference )
            {
            settings->AppendL( *iSipReference, EXdmPropToAppRef ); 
            }
        settings->AppendL( iXcapRootUri, EXdmPropUri );
        settings->AppendL( iUserName, EXdmPropAuthName );
        settings->AppendL( iPassword, EXdmPropAuthSecret );
        settings->AppendL( iAuthType, EXdmPropAuthType );
        iXdmSettingsId = TXdmSettingsApi::CreateCollectionL( *settings );
        CleanupStack::PopAndDestroy( settings );    // >>> settings
        iSettingsSaved = ETrue;
#ifdef _DEBUG 
    CXdmProvAdapter::WriteToLog( _L8( "CXdmProvItem::SaveL settings saved" ) );
#endif
        }
    
    return KErrNone;
    }

// -----------------------------------------------------------------------------
// CXdmProvItem::ToAppReferences
// -----------------------------------------------------------------------------
//
const RArray<TPtrC>& CXdmProvItem::ToAppReferences() const
    {
    return iToAppReferences;
    }

// -----------------------------------------------------------------------------
// CXdmProvItem::VisitL
// -----------------------------------------------------------------------------
//
void CXdmProvItem::VisitL( CWPCharacteristic& aCharacteristic )
    {
    if ( KWPResource == aCharacteristic.Type( ) )
        {
         aCharacteristic.AcceptL( *this );
#ifdef _DEBUG 
    CXdmProvAdapter::WriteToLog( _L8( "CXdmProvItem::VisitL CWPCharacteristic" ) );
#endif
        }
     }
        
// -----------------------------------------------------------------------------
// CXdmProvItem::VisitL
// -----------------------------------------------------------------------------
//
void CXdmProvItem::VisitL( CWPParameter& aParameter )
    {
#ifdef _DEBUG 
    HBufC8* value = HBufC8::NewLC( aParameter.Value().Length() );   // << value
    TPtr8 bufferPtr = value->Des();
    bufferPtr.Copy( aParameter.Value() );
    CXdmProvAdapter::WriteToLog( _L8( "CXdmProvItem::VisitL CWPParameter aParameter.Value() =%S" ), &bufferPtr );
    CleanupStack::PopAndDestroy( value );                           // >>> value
#endif
    switch( aParameter.ID() )
        {
        case EWPParameterAppID:
            {
            iAppId.Set( aParameter.Value() );
#ifdef _DEBUG 
    CXdmProvAdapter::WriteToLog( _L8( "CXdmProvItem::EWPParameterAppID" ) );
#endif
            }
            break;
    
        case EWPParameterProviderID:
            {
            iProviderId.Set( aParameter.Value() );
#ifdef _DEBUG 
    CXdmProvAdapter::WriteToLog( _L8( "CXdmProvItem::EWPParameterProviderID" ) );
#endif
            }
            break;

        case EWPParameterName:
            {
            iAppName.Set( aParameter.Value() );
#ifdef _DEBUG 
    CXdmProvAdapter::WriteToLog( _L8( "CXdmProvItem::EWPParameterName" ) );
#endif
            }
            break;  
            
        case EWPParameterToAppRef:
            {
            iToAppReferences.Append( aParameter.Value() );  
#ifdef _DEBUG 
    CXdmProvAdapter::WriteToLog( _L8( "CXdmProvItem::EWPParameterToAppRef" ) );
#endif
            }
            break;
    
        case EWPParameterToNapID:
            {
            iToNapId.Set( aParameter.Value() );
#ifdef _DEBUG 
    CXdmProvAdapter::WriteToLog( _L8( "CXdmProvItem::EWPParameterToNapID" ) );
#endif
            }
            break; 
            
        case EWPParameterURI:
            {
            iXcapRootUri.Set( aParameter.Value() );
#ifdef _DEBUG 
    CXdmProvAdapter::WriteToLog( _L8( "CXdmProvItem::EWPParameterURI" ) );
#endif
            }
            break;

        case EWPParameterAAuthName:
            {
            iUserName.Set( aParameter.Value() );
#ifdef _DEBUG 
    CXdmProvAdapter::WriteToLog( _L8( "CXdmProvItem::EWPParameterAAuthName" ) );
#endif
            }
            break;  
            
        case EWPParameterAAuthSecret:
            {
            iPassword.Set( aParameter.Value() );
#ifdef _DEBUG 
    CXdmProvAdapter::WriteToLog( _L8( "CXdmProvItem::EWPParameterAAuthSecret" ) );
#endif
            }
            break;
      
        case EWPParameterAAuthType:
            {
            iAuthType.Set( aParameter.Value() );
#ifdef _DEBUG 
    CXdmProvAdapter::WriteToLog( _L8( "CXdmProvItem::EWPParameterAAuthType" ) );
#endif
            }
            break; 
            
        case EWPParameterAppRef:
            {
            iAppRef.Set( aParameter.Value() );
#ifdef _DEBUG 
    CXdmProvAdapter::WriteToLog( _L8( "CXdmProvItem::EWPParameterAppRef" ) );
#endif
            }
            break;   
        default:
            {
            DetermineNamedParameter( aParameter.Name(), aParameter.Value() );
            }
            break;
        }
    }

// -----------------------------------------------------------------------------
// CXdmProvItem::DetermineNamedParameter
// -----------------------------------------------------------------------------
//
void CXdmProvItem::DetermineNamedParameter( const TDesC& aName, const TDesC& aValue )
    {
    if ( !aName.Compare( KXdmProvAppRef ) )
        {
        iAppRef.Set( aValue );
        }
    else if ( !aName.Compare( KXdmProvToAppRef ) )
        {
        iToAppReferences.Append( aValue ); 
        }
    }
// -----------------------------------------------------------------------------
// CXdmProvItem::VisitLinkL
// -----------------------------------------------------------------------------
//
void CXdmProvItem::VisitLinkL( CWPCharacteristic& aLink )
    {
#ifdef _DEBUG 
    CXdmProvAdapter::WriteToLog( _L8( "CXdmProvItem::VisitLinkL" ) );
#endif
    if( KWPNapDef == aLink.Type() )
        {        
        // check that this is the correct one 
        CArrayFix<TPtrC>* napId = new (ELeave) CArrayFixFlat<TPtrC>(1);
        CleanupStack::PushL( napId );           // << napId
        aLink.ParameterL( EWPParameterNapID, napId );
        if ( 0 < napId->Count() )
            {
            if ( !napId->At(0).Compare( iToNapId ) )
                {
                SetNapDefL( aLink );
                }
            }      
        CleanupStack::PopAndDestroy( napId );   // >>> napId
        }
    }
    
// -----------------------------------------------------------------------------
// CXdmProvItem::SetNapDefL
// -----------------------------------------------------------------------------
//  
void CXdmProvItem::SetNapDefL ( CWPCharacteristic& aLink )
    {
#ifdef _DEBUG 
    CXdmProvAdapter::WriteToLog( _L8( "CXdmProvItem::SetNapDefL" ) );
#endif  
    // find the name of the access point 
    CArrayFix<TPtrC>* name = new (ELeave) CArrayFixFlat<TPtrC>(1);
    CleanupStack::PushL( name );        // << name
    aLink.ParameterL( EWPParameterName, name );
    if ( name->Count() > 0 )
        {
        iNapName.Set( name->At(0) );
        }
    CleanupStack::PopAndDestroy( name ); // >>> name
        
#ifdef _DEBUG 
    CXdmProvAdapter::WriteToLog( _L8( "CXdmProvItem::SetNapDefL done" ) );
#endif  
    }
// -----------------------------------------------------------------------------
// CXdmProvItem::SetSipSettingsIdL
// -----------------------------------------------------------------------------
//  
void CXdmProvItem::SetSipSettingsIdL( const TDesC8& aStorageIdValue )
    {
    delete iSipReference;
    iSipReference = NULL;
    iSipReference = HBufC::NewL( aStorageIdValue.Length() ); 
    iSipReference->Des().Copy( aStorageIdValue );
    }   

// -----------------------------------------------------------------------------
// CXdmProvItem::FindAccessPointIdL
// -----------------------------------------------------------------------------
// 
void CXdmProvItem::FindAccessPointIdL ()
    {       
            
    TInt result( KErrNone );
    TUint32 idNum;
    TBuf<KXdmProvNameLength> name;   
    // Get IAP names and ids from the database
    CCommsDatabase* commsDb = CCommsDatabase::NewL( EDatabaseTypeIAP ); 
    CleanupStack::PushL( commsDb );             // << commsDb
    commsDb->ShowHiddenRecords();    
    CCommsDbTableView* view = commsDb->OpenTableLC( TPtrC( IAP ) ); // << view    
    result = view->GotoFirstRecord();
    
    while( KErrNone == result )
        {
        view->ReadTextL( TPtrC( COMMDB_NAME ), name );
        if ( KErrNotFound != name.Find( iNapName ) )
            {
            view->ReadUintL( TPtrC( COMMDB_ID ), idNum );
#ifdef _DEBUG 
    CXdmProvAdapter::WriteToLog( _L8( "CXdmProvItem::FindAccessPointIdL IAP found id=%d" ), idNum );
#endif
            }
        result = view->GotoNextRecord();
        }
#ifdef _DEBUG 
    CXdmProvAdapter::WriteToLog( _L8( "CXdmProvItem::FindAccessPointIdL IAP using id=%d" ), idNum );
#endif
    CleanupStack::PopAndDestroy( 2, commsDb );  // >>> view, commsDb
    TBuf<KXdmProvIdLength> id;
    id.AppendNum( idNum, EDecimal );
    delete iNapId;
    iNapId = NULL;
    iNapId = id.AllocL();
    }

// -----------------------------------------------------------------------------
// CXdmProvItem::CheckExistingNamesL()
// -----------------------------------------------------------------------------
// 
void CXdmProvItem::CheckExistingNamesL( )
    {    
    TBool ready( EFalse );  
    RArray<TInt> settingIds;
    CleanupClosePushL( settingIds );                // << settingIds
    CDesCArray* settingNames = TXdmSettingsApi::CollectionNamesLC( settingIds ); // << settingNames
    TInt index( 0 );
    while ( !ready )
        {
        TBool found ( EFalse );
        TInt count = settingNames->MdcaCount();
        TBuf<KXdmProvBufLength> tempName;
        tempName.Copy( iAppName );
        if ( index > 0 )
            {
            tempName.Append( KXdmProvStartBracket );
            tempName.AppendNumFixedWidth( index, EDecimal, 2 );
            tempName.Append( KXdmProvCloseBracket );
            }
        for ( TInt i(0); i < count && !found; i++ )
            {
            if ( !settingNames->MdcaPoint(i).CompareF( tempName ) )
                {
                found = ETrue;
                index++;
                }
            }
        if ( !found )
            {
            ready = ETrue;
            iAppName.Set( tempName );
            }
        }
    // only number used in next function, type of RArray
    CleanupStack::PopAndDestroy( 2 );  // >>> settingNames, settingIds
    }


//  End of File  

