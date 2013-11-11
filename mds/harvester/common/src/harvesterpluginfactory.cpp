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
* Description:  Harvester plugin factory
 *
*/


#include <e32base.h>
#include <e32std.h>
#include <apmrec.h>
#include <harvesterplugin.h>
#include <mdeobject.h>
#include <harvesterdata.h>
#include "harvestereventmanager.h"
#include "harvesterpluginfactory.h"
#include "harvesterplugininfo.h"
#include "mdsutils.h"
#include "harvesterlog.h"

// for CleanupResetAndDestroyPushL
#include <mmf/common/mmfcontrollerpluginresolver.h>

const TInt KCacheItemCountForEventCaching = 1;

// ---------------------------------------------------------------------------
// Constructor
// ---------------------------------------------------------------------------
//
CHarvesterPluginFactory::CHarvesterPluginFactory() :
    iBlacklist( NULL ),
    iHarvesting( EFalse ),
    iHarvesterEventManager( NULL )
	{
	WRITELOG( "CHarvesterPluginFactory::CHarvesterPluginFactory()" );
	}

// ---------------------------------------------------------------------------
// NewL
// ---------------------------------------------------------------------------
//
EXPORT_C CHarvesterPluginFactory* CHarvesterPluginFactory::NewL()
	{
	WRITELOG( "CHarvesterPluginFactory::NewL()" );
	CHarvesterPluginFactory* self = new (ELeave) CHarvesterPluginFactory();
	CleanupStack::PushL ( self);
	self->ConstructL ();
	CleanupStack::Pop ( self);

	return self;
	}

// ---------------------------------------------------------------------------
// Destructor
// ---------------------------------------------------------------------------
//
CHarvesterPluginFactory::~CHarvesterPluginFactory()
	{
	WRITELOG( "CHarvesterPluginFactory::~CHarvesterPluginFactory()" );
	
	delete iLastConfirmedContainerExt;
	iLastConfirmedContainerExt = NULL;
	
	delete iLastConfirmedSupportedExt;
	iLastConfirmedSupportedExt = NULL;
	
    if (iHarvesterEventManager)
        {
        iHarvesterEventManager->ReleaseInstance();
        }
	
	iHarvesterPluginInfoArray.ResetAndDestroy();
	iHarvesterPluginInfoArray.Close();
	REComSession::FinalClose();
	}

// ---------------------------------------------------------------------------
// ConstructL
// ---------------------------------------------------------------------------
//
void CHarvesterPluginFactory::ConstructL()
	{
	WRITELOG( "CHarvesterPluginFactory::ConstructL()" );
	iHarvesterEventManager = CHarvesterEventManager::GetInstanceL();
	SetupHarvesterPluginInfoL();
	}

// ---------------------------------------------------------------------------
// GetObjectDef
// ---------------------------------------------------------------------------
//
EXPORT_C void CHarvesterPluginFactory::GetObjectDefL( CHarvesterData& /*aHD*/, TDes& aObjectDef )
	{
	aObjectDef.Zero();
	}
	
// ---------------------------------------------------------------------------
// GetMimeType
// ---------------------------------------------------------------------------
//
EXPORT_C void CHarvesterPluginFactory::GetMimeType(const TDesC& aUri, TDes& aMimeType)
	{
    TPtrC extPtr;
    if( MdsUtils::GetExt( aUri, extPtr ) )
        {
        RPointerArray<CHarvesterPluginInfo> supportedPlugins;
        TRAP_IGNORE( GetSupportedPluginsL( supportedPlugins, extPtr ) );
        
        for( TInt i = supportedPlugins.Count() - 1; i >=0; i-- )
            {
            CHarvesterPluginInfo* info = supportedPlugins[i];
            if ( !(info->iPlugin) )
                {
                TRAPD( error, info->iPlugin = CHarvesterPlugin::NewL( info->iPluginUid ) );
                if( error != KErrNone )
                    {
                    aMimeType.Zero();
                    return;
                    }
                info->iPlugin->SetQueue( info->iQueue );
                info->iPlugin->SetHarvesterPluginFactory( *this );  
                info->iPlugin->SetBlacklist( *iBlacklist );
                }
            info->iPlugin->GetMimeType( aUri, aMimeType );
            if( aMimeType.Length() > 0 )
                {
                break;
                }
            }
        supportedPlugins.Close();
        }
    else
        {
        aMimeType.Zero();
        }
	}

// ---------------------------------------------------------------------------
// HarvestL
// ---------------------------------------------------------------------------
//
EXPORT_C TInt CHarvesterPluginFactory::HarvestL( CHarvesterData* aHD )
	{
#ifdef _DEBUG
	WRITELOG1("CHarvesterPluginFactory::HarvestL - aHD->Uri: %S", &aHD->Uri() );
#endif
	CHarvesterPluginInfo* hpi = aHD->HarvesterPluginInfo();
	
	if( !hpi )
	    {
	    SetPluginInfo( aHD );
	    hpi = aHD->HarvesterPluginInfo();
	    }
	
	if ( hpi )
		{
		if ( ! hpi->iPlugin )
	   		{
	   		hpi->iPlugin = CHarvesterPlugin::NewL( hpi->iPluginUid );
	   		hpi->iPlugin->SetQueue( hpi->iQueue );
	   		hpi->iPlugin->SetHarvesterPluginFactory( *this );  
	   		hpi->iPlugin->SetBlacklist( *iBlacklist );
	   		}
			
		if( aHD->ObjectType() == EFastHarvest || aHD->Origin() == MdeConstants::Object::ECamera )
		   	{
		   	hpi->iQueue.InsertL( aHD, 0 );
		   	
		   	if( !hpi->iPlugin->IsActive() )
		   	    {
	            hpi->iPlugin->SetPriority( KHarvesterPriorityHarvestingPlugin + 1 );
		   	    }
		    }
	    else
			{
			hpi->iQueue.AppendL( aHD );
			}
		    
		hpi->iPlugin->StartHarvest();
			
		return KErrNone;
		}
		
	return KErrNotFound;
	}

// ---------------------------------------------------------------------------
// GetPluginInfos
// ---------------------------------------------------------------------------
//
EXPORT_C RPointerArray<CHarvesterPluginInfo>& CHarvesterPluginFactory::GetPluginInfos()
	{
	return iHarvesterPluginInfoArray;
	}

// ---------------------------------------------------------------------------
// SetBlacklist
// ---------------------------------------------------------------------------
//
EXPORT_C void CHarvesterPluginFactory::SetBlacklist( CHarvesterBlacklist& aBlacklist )
	{
	WRITELOG( "CHarvesterPluginFactory::SetBlacklist()" );
	iBlacklist = &aBlacklist;
	
	const TInt count = iHarvesterPluginInfoArray.Count();
	for ( TInt i = count; --i >= 0; )
		{
	    if( iHarvesterPluginInfoArray[i]->iPlugin )
	        {
	        iHarvesterPluginInfoArray[i]->iPlugin->SetBlacklist( *iBlacklist );
	        }
		}	
	}

// ---------------------------------------------------------------------------
// SetupHarvesterPluginInfoL
// ---------------------------------------------------------------------------
//
void CHarvesterPluginFactory::SetupHarvesterPluginInfoL()
    {
    WRITELOG( "CHarvesterPluginFactory::SetupHarvesterPluginInfo()" );
    
    RImplInfoPtrArray infoArray;
    TCleanupItem cleanupItem( MdsUtils::CleanupEComArray, &infoArray );
    CleanupStack::PushL( cleanupItem );
 
    CHarvesterPlugin::ListImplementationsL( infoArray );

    for ( TInt i = infoArray.Count(); --i >= 0; )
        {
        // Parse the MIME types and resolve plug-in's uids from infoArray to iDataTypeArray
        CImplementationInformation* info = infoArray[i];
        const TBufC8<KMaxFileName>& type = info->DataType();
        const TBufC8<KMaxFileName>& opaque = info->OpaqueData();
        TUid implUID = info->ImplementationUid(); 
                
        AddNewPluginL( type, opaque, implUID );   
        }
    
    CleanupStack::PopAndDestroy( &infoArray );
    }

// ---------------------------------------------------------------------------
// AddNewPluginL
// ---------------------------------------------------------------------------
//
void CHarvesterPluginFactory::AddNewPluginL( const TDesC8& aType,
		const TDesC8& aOpaque, TUid aPluginUid )
    {      
    WRITELOG( "CHarvesterPluginFactory::AddNewPluginL" );

    CHarvesterPluginInfo* pluginInfo = new (ELeave) CHarvesterPluginInfo;
    CleanupStack::PushL( pluginInfo );

    // get file extensions
    TLex8 lex( aOpaque );
    while ( !lex.Eos() )
        {
        /* Tokenizing file extensions using TLex8 */
        lex.SkipSpaceAndMark();

        TPtrC8 extToken = lex.NextToken();

        HBufC* str = HBufC::NewLC( extToken.Length() );
        str->Des().Copy( extToken );
        pluginInfo->iExtensions.AppendL( str );
        CleanupStack::Pop( str );
        }

    // get object types
    TLex8 lexObjectTypes( aType );
    while ( !lexObjectTypes.Eos() )
        {
        /* Tokenizing object types using TLex8 */
        lexObjectTypes.SkipSpaceAndMark();
        
        TPtrC8 objectTypeToken = lexObjectTypes.NextToken();

        HBufC* str = HBufC::NewLC( objectTypeToken.Length() );
        str->Des().Copy( objectTypeToken );
        pluginInfo->iObjectTypes.AppendL( str );
        CleanupStack::Pop( str );
        }
    
    pluginInfo->iPluginUid = aPluginUid;
    
    pluginInfo->iPlugin = CHarvesterPlugin::NewL( pluginInfo->iPluginUid );
    pluginInfo->iPlugin->SetQueue( pluginInfo->iQueue );
    pluginInfo->iPlugin->SetHarvesterPluginFactory( *this );
    
    iHarvesterPluginInfoArray.AppendL( pluginInfo );
    CleanupStack::Pop( pluginInfo );
    }

void CHarvesterPluginFactory::GetSupportedPluginsL(
		RPointerArray<CHarvesterPluginInfo>& aSupportedPlugins, const TDesC& aExt )
	{
    CleanupResetAndDestroyPushL( aSupportedPlugins );
    
	const TInt pluginInfoCount = iHarvesterPluginInfoArray.Count();
	TInt extCount = 0;
	for ( TInt i = pluginInfoCount; --i >= 0; )
        {
        CHarvesterPluginInfo* info = iHarvesterPluginInfoArray[i];
        
        extCount = info->iExtensions.Count();
        for ( TInt k = extCount; --k >= 0; )
            {
            TDesC* ext = info->iExtensions[k];
            
            // checking against supported plugin file extensions
            const TInt result = ext->CompareF( aExt );
            if ( result == 0 )
                {
                aSupportedPlugins.AppendL( info );
                break;
                }
            }
        }	
	
	CleanupStack::Pop( &aSupportedPlugins );
	}

EXPORT_C TBool CHarvesterPluginFactory::IsSupportedFileExtension( const TDesC& aFileName )
	{
	TPtrC extPtr;
	if( MdsUtils::GetExt( aFileName, extPtr ) )
		{
        if( iLastConfirmedSupportedExt &&
            extPtr.CompareF( iLastConfirmedSupportedExt->Des() ) == 0 )
            {
            // Extension has previously been confirmed to be 
            // supported file extension, no need to ask from plugins
            return ETrue;
            }
		TInt pluginInfoCount = iHarvesterPluginInfoArray.Count();
		TInt extCount = 0;
		for ( TInt i = pluginInfoCount; --i >= 0; )
			{
			CHarvesterPluginInfo* info = iHarvesterPluginInfoArray[i];
			extCount = info->iExtensions.Count();
			for ( TInt k = extCount; --k >= 0; )
				{
				TDesC* ext = info->iExtensions[k];
				// checking against supported plugin file extensions
				TInt result = MdsUtils::Compare( *ext, extPtr );
				if ( result == 0 )
					{
                    delete iLastConfirmedSupportedExt;
                    iLastConfirmedSupportedExt = NULL;
                    iLastConfirmedSupportedExt = extPtr.Alloc();				    
					return ETrue;
					}
				}
			}
		}
	return EFalse;
	}

EXPORT_C TBool CHarvesterPluginFactory::IsContainerFileL( const TDesC& aURI )
	{
	TBool isContainerFile = EFalse;
	TPtrC extPtr;
	
	if( MdsUtils::GetExt( aURI, extPtr ) )
		{
	    if( iLastConfirmedContainerExt &&
	        extPtr.CompareF( iLastConfirmedContainerExt->Des() ) == 0 )
	        {
	        // Extension has previously been confirmed to be 
	        // container file extension, no need to ask from plugins
	        return ETrue;
	        }
	
		RPointerArray<CHarvesterPluginInfo> supportedPlugins;
		CleanupClosePushL( supportedPlugins );
		GetSupportedPluginsL( supportedPlugins, extPtr );
		for( TInt i = supportedPlugins.Count() - 1; i >=0; i-- )
			{
			CHarvesterPluginInfo* info = supportedPlugins[i];
			if( info->iObjectTypes.Count() >  1 )
				{
				isContainerFile = ETrue;
				delete iLastConfirmedContainerExt;
				iLastConfirmedContainerExt = NULL;
				iLastConfirmedContainerExt = extPtr.Alloc();
				break;
				}
			}
		CleanupStack::PopAndDestroy( &supportedPlugins );
		}
	return isContainerFile;
	}

void CHarvesterPluginFactory::SetPluginInfo( CHarvesterData* aHD )
    {
    TPtrC extPtr;
    if( MdsUtils::GetExt( aHD->Uri(), extPtr ) )
        {
        RPointerArray<CHarvesterPluginInfo> supportedPlugins;
        TRAP_IGNORE( GetSupportedPluginsL( supportedPlugins, extPtr ) );
        
        if( supportedPlugins.Count() > 0 )
            {
            CHarvesterPluginInfo* info = supportedPlugins[0];
            aHD->SetHarvesterPluginInfo( info );
            }

        supportedPlugins.Close();
        }
    }

EXPORT_C void CHarvesterPluginFactory::SendHarvestingStatusEventL( TBool aStarted )
    {
    WRITELOG( "CHarvesterPluginFactory::SendHarvestingStatusEventL" );
    const TInt pluginInfoCount = iHarvesterPluginInfoArray.Count();
    TBool itemsLeft( EFalse );
    TBool allPluginsOnIdle( ETrue );
    for ( TInt i = pluginInfoCount; --i >= 0; )
        {
        CHarvesterPluginInfo* info = iHarvesterPluginInfoArray[i];
        if( info && info->iQueue.Count() )
            {
            itemsLeft = ETrue;
            if( aStarted )
                {
                // Idle state is only checked if finished event is sent
                break;
                }
            }
        if( info && info->iPlugin && !(info->iPlugin->PluginInIdleState()) )
            {
            allPluginsOnIdle = EFalse;
            }
        }
    
    if( !iHarvesting && itemsLeft && aStarted )
        {
        WRITELOG( "CHarvesterPluginFactory::SendHarvestingStatusEventL - overall started" );
        iHarvesting = ETrue;
        iHarvesterEventManager->SendEventL( EHEObserverTypeOverall, EHEStateStarted );
        // This next line is for caching the harvester started event for observers registering
        // after harvesting has already started
        iHarvesterEventManager->IncreaseItemCount( EHEObserverTypeOverall, KCacheItemCountForEventCaching );
        return;
        }
    else if( iHarvesting && (!itemsLeft || allPluginsOnIdle) && !aStarted )
        {
        WRITELOG( "CHarvesterPluginFactory::SendHarvestingStatusEventL - overall finished" );
        iHarvesting = EFalse;                       
        iHarvesterEventManager->SendEventL( EHEObserverTypeOverall, EHEStateFinished );
        iHarvesterEventManager->DecreaseItemCountL( EHEObserverTypeOverall, KCacheItemCountForEventCaching );
        }
    }

EXPORT_C void CHarvesterPluginFactory::PauseHarvester( TBool aPaused )
    {
    const TInt count = iHarvesterPluginInfoArray.Count();
    for ( TInt i = count; --i >= 0; )
        {
        if( iHarvesterPluginInfoArray[i]->iPlugin && aPaused )
            {
            iHarvesterPluginInfoArray[i]->iPlugin->StopHarvest();
            }
        else if( iHarvesterPluginInfoArray[i]->iPlugin )
            {
            iHarvesterPluginInfoArray[i]->iPlugin->StartHarvest();
            }
        }
    }

EXPORT_C void CHarvesterPluginFactory::GetObjectDefL( CHarvesterData* aHD, TDes& aObjectDef )
    {
    TPtrC extPtr;
    if( MdsUtils::GetExt( aHD->Uri(), extPtr ) )
        {
        RPointerArray<CHarvesterPluginInfo> supportedPlugins;
        CleanupClosePushL( supportedPlugins );
        GetSupportedPluginsL( supportedPlugins, extPtr );
        
        const TInt sCount = supportedPlugins.Count();
        if( sCount == 1 )
            {
            CHarvesterPluginInfo* info = supportedPlugins[0];
            if( info->iObjectTypes.Count() == 1 )
                {
                aObjectDef.Copy( *(info->iObjectTypes[0]) );
                aHD->SetHarvesterPluginInfo( info );
                CleanupStack::PopAndDestroy( &supportedPlugins );
                return;
                }
            }
        for( TInt i = sCount - 1; i >=0; i-- )
            {
            CHarvesterPluginInfo* info = supportedPlugins[i];
            if ( !(info->iPlugin) )
                {
                info->iPlugin = CHarvesterPlugin::NewL( info->iPluginUid );
                info->iPlugin->SetQueue( info->iQueue );
                info->iPlugin->SetHarvesterPluginFactory( *this );  
                info->iPlugin->SetBlacklist( *iBlacklist );
                }
            info->iPlugin->GetObjectType( aHD->Uri(), aObjectDef );
            // It is possible for unmount to occure while we are waiting
            // for GetObjectType to return, thus check aHD for validity
            if( aHD && aObjectDef.Length() > 0 )
                {
                aHD->SetHarvesterPluginInfo( info );
                break;
                }
            else if( !aHD )
                {
                break;
                }
            }
        CleanupStack::PopAndDestroy( &supportedPlugins );
        }
    else
        {
        aObjectDef.Zero();
        }
    }


