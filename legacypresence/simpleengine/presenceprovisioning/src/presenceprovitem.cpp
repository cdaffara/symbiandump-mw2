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
* Description:    : Implementation for for presenceprovitem
*
*/




// INCLUDE FILES
#include <CWPParameter.h>
#include <CWPCharacteristic.h>
#include <StringLoader.h>
#include <badesca.h>

#include "presenceprovitem.h"
#include "pressettingsapi.h"
#include "presenceprovlogger.h"

// ============================ MEMBER FUNCTIONS ===============================

// -----------------------------------------------------------------------------
// CPresProvItem::CPresProvItem
// -----------------------------------------------------------------------------
//
CPresProvItem::CPresProvItem() :
    iPresSetId(KErrNotFound),
    iSettingsSaved( EFalse )
    {
    iSetName.Set(KNullDesC);
    iSipProfile = KErrNotFound;
    iXDMSetting = KErrNotFound;
    iObjectSize = KErrNotFound;
    iContSrvrAdd.Set(KNullDesC);
    iPublicationInt = KErrNotFound;
    iMaxSubscriptions = KErrNotFound;
    iMaxContactsInList = KErrNotFound;
    iDomainSyntax.Set(KNullDesC);
    iProviderID.Set(KNullDesC);
    }

// -----------------------------------------------------------------------------
// CPresProvItem::NewL
// Two-phased constructor.
// -----------------------------------------------------------------------------
//
CPresProvItem* CPresProvItem::NewL()
    {
    CPresProvItem* self = new (ELeave) CPresProvItem();
    return self;
    }

// -----------------------------------------------------------------------------
// CPresProvItem::~CPresProvItem
// -----------------------------------------------------------------------------
//
CPresProvItem::~CPresProvItem()
    {
    iToAppReferences.Close();
    }    

// -----------------------------------------------------------------------------
// CPresProvItem::Validate
// -----------------------------------------------------------------------------
//
TBool CPresProvItem::Validate() const
    {
    // Validate that compulsory settings are received
    PRES_PROV_LOG(LOG_LIT8( "CPresProvItem::Validate()"));
    
    if ( !iAppId.Length() )
        {
        PRES_PROV_LOG(LOG_LIT8( "   iAppId missing" ) );
        return EFalse;
        }
    else if ( iAppId.Compare( KPresProvisioningAppId) )
        {
        PRES_PROV_LOG(LOG_LIT8( "   Wrong iAppId" ) );
        return EFalse;
        }
    else if (iSetName == KNullDesC)
        {
        PRES_PROV_LOG(LOG_LIT8( "   iSetName is missing" ));
        return EFalse;
        }
    else if (iToAppReferences.Count() < 2 ) // if SIP and XDM references are not present
        {
        PRES_PROV_LOG(LOG_LIT8( "   XDM or SIP reference is missing" ));
        return EFalse;
        }
    else if (iObjectSize < NULL)
        {
        PRES_PROV_LOG(LOG_LIT8( "   iObjectSize is missing" ));
        return EFalse;
        }
    else if (iDomainSyntax == KNullDesC)
        {
        PRES_PROV_LOG(LOG_LIT8( "   iDomainSyntax is missing" ));
        return EFalse;
        }
    else if (iProviderID == KNullDesC)
        {
        PRES_PROV_LOG(LOG_LIT8( "   iProviderID is missing" ));
        return EFalse;
        }
    PRES_PROV_LOG(LOG_LIT8( "   return True" ) );
    return ETrue;   
    }
    
// -----------------------------------------------------------------------------
// CPresProvItem::Name
// -----------------------------------------------------------------------------
//  
const TDesC& CPresProvItem::Name() const
    {
    return iSetName;
    }

// -----------------------------------------------------------------------------
// CPresProvItem::ApplicationId
// -----------------------------------------------------------------------------
//  
const TDesC& CPresProvItem::ApplicationId() const
    {
    return KPresProvisioningAppId;
    }
    
// -----------------------------------------------------------------------------
// CPresProvItem::PresSetId
// -----------------------------------------------------------------------------
//  
TInt CPresProvItem::PresSetId() const
    {
    return iPresSetId;
    }
    
// -----------------------------------------------------------------------------
// CPresProvItem::AppReference
// -----------------------------------------------------------------------------
//  
const TDesC& CPresProvItem::AppReference() const
    {
    return iAppRef;
    }
    
// -----------------------------------------------------------------------------
// CPresProvItem::SaveL
// -----------------------------------------------------------------------------
//  
TInt CPresProvItem::SaveL()
    {
    PRES_PROV_LOG(LOG_LIT8( "CPresProvItem::SaveL" ) );
    
    TInt id(KErrNone);
 
    if( !iSettingsSaved )
        {
        TPresSettingsSet mySet;
        (mySet.iSetName).Copy(iSetName);
        mySet.iSipProfile = iSipProfile;
        mySet.iXDMSetting = iXDMSetting;
        mySet.iObjectSize = iObjectSize;
        (mySet.iContSrvrAdd).Copy(iContSrvrAdd);
        mySet.iPublicationInt = iPublicationInt;
        mySet.iMaxSubscriptions = iMaxSubscriptions;
        mySet.iMaxContactsInList = iMaxContactsInList;
        (mySet.iDomainSyntax).Copy(iDomainSyntax);
        (mySet.iProviderID).Copy(iProviderID);
        
        CheckDuplicateNameL(mySet.iSetName, iPresSetId);
        
        id = PresSettingsApi::CreateSetL(mySet);
        
        if(id>KErrNotFound)
            {
            iSettingsSaved = ETrue;
            iPresSetId = id;           
            }
            
        PRES_PROV_LOG(LOG_LIT8( "CPresProvItem::SaveL with Id:%d" ),id);
        }
    return KErrNone;
    }

// -----------------------------------------------------------------------------
// CPresProvItem::ToAppReferences
// -----------------------------------------------------------------------------
//
const RArray<TPtrC>& CPresProvItem::ToAppReferences() const
    {
    return iToAppReferences;
    }

// -----------------------------------------------------------------------------
// CPresProvItem::VisitL
// -----------------------------------------------------------------------------
//
void CPresProvItem::VisitL( CWPCharacteristic& aCharacteristic )
    {
    TInt type = aCharacteristic.Type();
    PRES_PROV_LOG(LOG_LIT8( "CPresProvItem::VisitL CWPCharacteristic %d" ), type );
    if (type == KWPResource)
        {
        aCharacteristic.AcceptL( *this );
        }    
    }
        
// -----------------------------------------------------------------------------
// CPresProvItem::VisitL
// -----------------------------------------------------------------------------
//
void CPresProvItem::VisitL( CWPParameter& aParameter )
    {
    PRES_PROV_LOG(LOG_LIT8( "CPresProvItem::VisitL" ) );
    PRES_PROV_LOG(LOG_LIT(" aParameter.Value(): %S"), &(aParameter.Value()));
    
    switch( aParameter.ID() )
        {
        case EWPParameterAppID:
            {
            iAppId.Set( aParameter.Value() );
            PRES_PROV_LOG(LOG_LIT8( "    EWPParameterAppID" ) );
            }
            break;
    
        case EWPParameterProviderID:
            {
            iProviderID.Set( aParameter.Value() );
            PRES_PROV_LOG(LOG_LIT8( "    EWPParameterProviderID" ) );
            }
            break;

        case EWPParameterName:
            {
            iSetName.Set( aParameter.Value() );
            PRES_PROV_LOG(LOG_LIT8( "    EWPParameterName" ) );
            }
            break;  
            
        case EWPParameterToAppRef:
            {
            PRES_PROV_LOG(LOG_LIT8( " EWPParameterToAppRef"));
            iToAppReferences.Append( aParameter.Value() );  
            }
            break;
    
        case EWPParameterAppRef:
            {
            iAppRef.Set( aParameter.Value() );
            PRES_PROV_LOG(LOG_LIT8( " EWPParameterAppRef"));
            }
            break;
        
        case EWPParameterService: // some setting tools put SERVICE-URI-TEMPLATE
            {                     // with this id
            iDomainSyntax.Set(aParameter.Value());
            PRES_PROV_LOG(LOG_LIT8( " EWPParameterService"));
            }
            break;
                                 
        default:
            {
            DetermineNamedParameter( aParameter.Name(), aParameter.Value() );
            }
            break;
        }
    PRES_PROV_LOG(LOG_LIT8( "   CPresProvItem::VisitL ends" ) );    
    }

// -----------------------------------------------------------------------------
// CPresProvItem::DetermineNamedParameter
// -----------------------------------------------------------------------------
//
void CPresProvItem::DetermineNamedParameter( const TDesC& aName, const TDesC& aValue )
    {
    PRES_PROV_LOG(LOG_LIT( "DetermineNamedParameter(%S, %S)"), &aName, &aValue);
    if ( !aName.Compare( KPresProvClientObjDataLim ) )
        {
        iObjectSize = DesToInt(aValue);
        }
    else if ( !aName.Compare( KPresProvContentSrvUri ) )
        {
        iContSrvrAdd.Set( aValue );
        }
    else if ( !aName.Compare( KPresProvSrcThPublish ) )
        {
        iPublicationInt = DesToInt( aValue );
        }
    else if ( !aName.Compare( KPresProvMaxPresSubs ) )
        {
        iMaxSubscriptions = DesToInt( aValue );
        }
    else if ( !aName.Compare( KPresProvMaxPresSubsInLists ) )
        {
        iMaxContactsInList = DesToInt( aValue );
        }
    else if ( !aName.Compare( KPresProvServiceUriTemp ) )
        {
        iDomainSyntax.Set(aValue);
        }
    else if (!aName.Compare(KPresIAppRef))
        {
        iAppRef.Set(aValue);
        }
    else if (!aName.Compare(KPresIToAppRef))
        {
        if(iToAppReferences.Count() < 2) // if any of SIP or XDM id is missing
            iToAppReferences.Append(aValue);        
        }
    PRES_PROV_LOG(LOG_LIT8( "   CPresProvItem::DetermineNamedParameter ends" ) );      
    }
// -----------------------------------------------------------------------------
// CPresProvItem::VisitLinkL
// -----------------------------------------------------------------------------
//
void CPresProvItem::VisitLinkL( CWPCharacteristic& /*aLink*/ )
    {
    PRES_PROV_LOG(LOG_LIT8( "CPresProvItem::VisitLinkL" ) );
    }
    
// -----------------------------------------------------------------------------
// CPresProvItem::SetSipSetIdL
// -----------------------------------------------------------------------------
//  
void CPresProvItem::SetSipSetIdL(TInt aSipId)
    {
    PRES_PROV_LOG(LOG_LIT8( "SetSipSetIdL(%d)" ), aSipId);
    if (iSipProfile == KErrNotFound)
        iSipProfile = aSipId;
    if(iPresSetId>KErrNotFound)
        PresSettingsApi::UpdatePropertyL( iPresSetId, EPresPropSIPProfile, iSipProfile);
    }
    
// -----------------------------------------------------------------------------
// CPresProvItem::SetXDMSetIdL
// -----------------------------------------------------------------------------
//  
void CPresProvItem::SetXDMSetIdL(TInt aXdmId)
    {
    PRES_PROV_LOG(LOG_LIT8( "SetXDMSetIdL(%d)" ), aXdmId);
    if (iXDMSetting == KErrNotFound)
        iXDMSetting = aXdmId;
    if(iPresSetId>KErrNotFound)
        PresSettingsApi::UpdatePropertyL( iPresSetId, EPresPropXDMSet, iXDMSetting);    
    }
    
// ---------------------------------------------------------------------------
// CPresProvItem::GetPrefixL()
// ---------------------------------------------------------------------------
// 
HBufC* CPresProvItem::GetPrefixL( HBufC* aName )
    {
    TPtrC prefix = aName->Des();
    TInt lastBrace = aName->LocateReverse('(');
    if ( lastBrace != KErrNotFound )
        {
        // aName looks like "<prefix><brace><something>".
        // See if <something> is an integer number and
        // <something> ends to a brace.
        TPtrC num = aName->Right( aName->Length() - lastBrace - 1 );
        TInt val;
        TLex lex( num );
        if ( lex.Val( val ) == KErrNone && num.Locate(')') == num.Length() - 1 )
            {
            // Yes, the trailer is an integer.
            prefix.Set( aName->Left( lastBrace ) );
            }
        }
    return prefix.AllocL();
    }    

// -----------------------------------------------------------------------------
// CPresProvAdapter::CheckDuplicateNameL
// Checks if duplicate named Presence profiles. Renames if same.
// -----------------------------------------------------------------------------
void CPresProvItem::CheckDuplicateNameL( TDes& aSetName, const TInt& aSetId)
    {
    PRES_PROV_LOG(LOG_LIT(" CheckDuplicateNameL(%S, %d)"),&aSetName, aSetId);
    
    _LIT( KPresProvNumbering, "(%N)" );
    const TInt KPresProvFormatExtraLen = 6;
    
    HBufC* newName = HBufC::NewLC(KPresSetNameMaxLength);
    newName->Des().Copy(aSetName);

    RArray<TInt> setIds;
    CleanupClosePushL( setIds );
    CDesCArray* names = PresSettingsApi::GetAllSetsNamesLC(setIds);
    TInt count(setIds.Count());
    
    newName = newName->ReAlloc( 
        newName->Length() + KPresProvFormatExtraLen );// enough for formatting

    HBufC* tempSetName = NULL;
    TInt pos = 0;// used as dummy
    TInt found = 0;
    TInt index = 1;
    TPtr ptr = newName->Des();
    
    // do the actual checking of name
    found = names->Find( *newName, pos );
    while ( found == 0 )
        {
        if ( !tempSetName )
            {
            tempSetName = GetPrefixL( newName );
            tempSetName = tempSetName->ReAlloc( 
                tempSetName->Length() + KPresProvFormatExtraLen );// enough for formatting
            CleanupStack::PushL( tempSetName );
            tempSetName->Des().Append( KPresProvNumbering );
            }
        StringLoader::Format( ptr, *tempSetName, -1, index );
        found = names->Find( *newName, pos );
        index++;
        }

    // cleanup
    if ( tempSetName )
        {
        CleanupStack::PopAndDestroy( tempSetName ); //tempSetName
        }
    
    CleanupStack::PopAndDestroy(names); // names
    CleanupStack::PopAndDestroy(1); // setIds 

    aSetName.Copy(newName->Des());
    CleanupStack::PopAndDestroy( newName );
    
    PRES_PROV_LOG(LOG_LIT(" CheckDuplicateNameL(%S): end"),&aSetName);
    }    
    
// ----------------------------------------------------------------------------
// CPresProvItem::DesToInt
// ----------------------------------------------------------------------------
TInt CPresProvItem::DesToInt( const TDesC& aSrcDes ) const
    {
    TLex16 lex( aSrcDes );
    TInt value ( KErrNotFound );
    lex.Val( value );
    return value;
    }

//  End of File  

