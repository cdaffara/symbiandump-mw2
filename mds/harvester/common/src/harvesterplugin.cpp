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
* Description:  Base class for harvester plug-ins*
*/


#include <harvesterplugin.h>
#include <harvesterdata.h>
#include <mdeconstants.h>
#include <mdepropertydef.h>
#include <mdeobjectdef.h>
#include <mdeobject.h>
#include "harvesterlog.h"
#include "harvestercommon.h"
#include "harvesterblacklist.h"
#include "harvesterpluginfactory.h"
#include "mdsutils.h"

// ---------------------------------------------------------------------------
// NewL
// ---------------------------------------------------------------------------
//
EXPORT_C CHarvesterPlugin* CHarvesterPlugin::NewL(const TUid& aUid)
	{
	TAny* harvesterPlugin = REComSession::CreateImplementationL(
			aUid, _FOFF( CHarvesterPlugin, iDtor_ID_Key ) );
	CHarvesterPlugin* self = reinterpret_cast<CHarvesterPlugin*>(harvesterPlugin);
	CleanupStack::PushL( self );
	self->ConstructL();
	CleanupStack::Pop( self );
	return self;
	}

// ---------------------------------------------------------------------------
// ConstructL
// ---------------------------------------------------------------------------
//
void CHarvesterPlugin::ConstructL()
	{
	User::LeaveIfError( iFs.Connect() );
	iState = EHarvesterIdle;
	CActiveScheduler::Add( this );
	}

// ---------------------------------------------------------------------------
// Constructor
// ---------------------------------------------------------------------------
//
EXPORT_C CHarvesterPlugin::CHarvesterPlugin() : 
	CActive( KHarvesterPriorityHarvestingPlugin ),
	iState( EHarvesterIdle ),
	iQueue( NULL ),
	iBlacklist( NULL ),
    iDtor_ID_Key( KNullUid ),
    iOriginPropertyDef( NULL ),
    iTitlePropertyDef( NULL ),
    iHarvesting( EFalse ),
    iPaused( EFalse )
	{
	}

// ---------------------------------------------------------------------------
// ListImplementationsL
// ---------------------------------------------------------------------------
//
EXPORT_C void CHarvesterPlugin::ListImplementationsL(
		RImplInfoPtrArray& aImplInfoArray)
	{
	REComSession::ListImplementationsL(
			KCHarvesterPluginInterfaceUid, aImplInfoArray );
	}

// ---------------------------------------------------------------------------
// Destructor
// ---------------------------------------------------------------------------
//
EXPORT_C CHarvesterPlugin::~CHarvesterPlugin() // destruct - virtual
	{
	Cancel();
	
	iFs.Close();
	REComSession::DestroyedImplementation( iDtor_ID_Key );
	}

// ---------------------------------------------------------------------------
// AddQueue
// ---------------------------------------------------------------------------
//
EXPORT_C void CHarvesterPlugin::SetQueue( RPointerArray<CHarvesterData>& aQueue )
	{
	iQueue = &aQueue;
	}

// ---------------------------------------------------------------------------
// StartHarvest
// ---------------------------------------------------------------------------
//
EXPORT_C void CHarvesterPlugin::StartHarvest()
	{
	if( iState == EHarvesterIdle )
		{
		iPaused = EFalse;
		SetNextRequest( EHarvesterGathering );
		}
	}

// ---------------------------------------------------------------------------
// GetObjectType
// ---------------------------------------------------------------------------
//
EXPORT_C void CHarvesterPlugin::GetObjectType( const TDesC& /*aUri*/, TDes& aObjectType )
	{
	aObjectType.Zero();
	}

// ---------------------------------------------------------------------------
// RunL
// ---------------------------------------------------------------------------
//
EXPORT_C void CHarvesterPlugin::RunL()
    {
    switch( iState )
        {
        case EHarvesterIdle:
            {
            if( iHarvesting )
                {
                TRAP_IGNORE( iFactory->SendHarvestingStatusEventL( EFalse ) );
                iHarvesting = EFalse;     
                }
            }
            break;
            
        case EHarvesterGathering:
            {
            if( iQueue->Count() == 0 )
                {
                if( iHarvesting )
                    {
                    TRAP_IGNORE( iFactory->SendHarvestingStatusEventL( EFalse ) );
                    iHarvesting = EFalse;     
                    }
                iQueue->Compress();
                SetNextRequest( EHarvesterIdle );
                }
            else
            	{
                WRITELOG( "CHarvesterPlugin::RunL EHarvesterGathering - items in queue" );
                if ( !iHarvesting )
                    {
                    TRAP_IGNORE( iFactory->SendHarvestingStatusEventL( ETrue ) );
                    iHarvesting = ETrue;
                    }
            
            	CHarvesterData* hd = (*iQueue)[0];
            	iQueue->Remove( 0 );
            	if( !hd )
            	    {
            	    SetNextRequest( EHarvesterGathering );
            	    break;
            	    }
            	const TDesC& uri = hd->Uri();
            	TUint32 mediaId = hd->MdeObject().MediaId();
            	
            	if( hd->ObjectType() == EFastHarvest || hd->Origin() == MdeConstants::Object::ECamera )
            	    {
            	    iFastModeEnabled = ETrue;
            	    }
            	else if( iFastModeEnabled )
            	    {
                    iFastModeEnabled = EFalse;
                    SetPriority( KHarvesterPriorityHarvestingPlugin );
            	    }
            	
				if( iBlacklist )
					{
					WRITELOG( "CHarvesterPlugin::RunL - Adding URI to blacklist" );
					TTime modified ( 0 );
					
					if( hd->IsBinary() )
						{
						TInt err = iFs.Modified( hd->Uri(), modified );
						if ( err != KErrNone )
							{
							modified = 0;
							}
						}
					iBlacklist->AddFile( uri, mediaId, modified );
					}
				
				TRAP_IGNORE( SetDefaultPropertiesL( *hd ) );
				
				WRITELOG1("CHarvesterPlugin::RunL - Calling HarvestL for file: %S", &uri);  
                TRAPD( err, HarvestL( hd ) );
                
                if ( iBlacklist )
                    {
                    WRITELOG( "CHarvesterPlugin::RunL - Removing URI from blacklist" );
                    iBlacklist->RemoveFile( uri, mediaId );
                    }
                
                if( err )
                	{
                	WRITELOG1("CHarvesterPlugin::RunL - ERROR: harvesting failed: %d", err); 
                	hd->SetErrorCode( err );
                	}
                
	            hd->PluginObserver()->HarvestingCompleted( hd );
                SetNextRequest( EHarvesterGathering );
            	}
            }
            break;

        default:
            break;
        }
    }

// ---------------------------------------------------------------------------
// DoCancel
// ---------------------------------------------------------------------------
//
EXPORT_C void CHarvesterPlugin::DoCancel()
    {
    iState = EHarvesterIdle;
    }

// ---------------------------------------------------------------------------
// RunError
// ---------------------------------------------------------------------------
//
EXPORT_C  TInt CHarvesterPlugin::RunError( TInt /*aError*/ )
	{
	SetNextRequest( EHarvesterGathering );
	return KErrNone;
	}

// ---------------------------------------------------------------------------
// StartHarvest
// ---------------------------------------------------------------------------
//
EXPORT_C void CHarvesterPlugin::StopHarvest()
    {
    Cancel();
    iState = EHarvesterIdle;
    if( iHarvesting )
        {
        TRAP_IGNORE( iFactory->SendHarvestingStatusEventL( EFalse ) );
        iHarvesting = EFalse;     
        }
    iPaused = ETrue;
    }

// ---------------------------------------------------------------------------
// SetNextRequest
// ---------------------------------------------------------------------------
//
void CHarvesterPlugin::SetNextRequest( THarvesterState aState )
    {
    if ( !IsActive() && !iPaused )
        {
        iState = aState;
        SetActive();
        TRequestStatus* status = &iStatus;
        User::RequestComplete( status, KErrNone );
        }
    }


// ---------------------------------------------------------------------------
// SetDefaultProperties
// ---------------------------------------------------------------------------
//
void CHarvesterPlugin::SetDefaultPropertiesL(CHarvesterData& aData)
	{
	CMdEObject& mdeObject = aData.MdeObject();
	
	if( !iOriginPropertyDef )
		{
		CMdEObjectDef& objDef = mdeObject.Def();
		iOriginPropertyDef = &objDef.GetPropertyDefL( MdeConstants::Object::KOriginProperty );
		}

	CMdEProperty* prop = NULL;
	mdeObject.Property( *iOriginPropertyDef, prop );
    if ( prop )
    	{
    	TUint8 val = prop->Uint8ValueL();
    	if ( val == MdeConstants::Object::EOther )
    		{
#ifdef _DEBUG
    		WRITELOG2("CHarvesterPlugin::SetDefaultPropertiesL - URI: %S SET Origin: %d", &aData.Uri(), aData.Origin() );
#endif
    		prop->SetUint8ValueL( (TUint8) aData.Origin() );
    		}
    	}
    else
    	{
#ifdef _DEBUG
		WRITELOG2("CHarvesterPlugin::SetDefaultPropertiesL - URI: %S ADD Origin: %d", &aData.Uri(), aData.Origin() );
#endif
    	mdeObject.AddUint8PropertyL( *iOriginPropertyDef, (TUint8) aData.Origin() );
    	}
    
	if( !iTitlePropertyDef )
		{
		CMdEObjectDef& objDef = mdeObject.Def();
		iTitlePropertyDef = &objDef.GetPropertyDefL( MdeConstants::Object::KTitleProperty );
		}
    
    prop = NULL;
    mdeObject.Property( *iTitlePropertyDef, prop );
    if ( !prop )
    	{
    	TPtrC name;
    	TBool nameFound = MdsUtils::GetName( aData.Uri(), name );

     	if ( nameFound )
     		{
     		mdeObject.AddTextPropertyL( *iTitlePropertyDef, name );
     		}
    	}
	}

// ---------------------------------------------------------------------------
// SetBlacklist
// ---------------------------------------------------------------------------
//
EXPORT_C void CHarvesterPlugin::SetBlacklist( CHarvesterBlacklist& aBlacklist )
	{
	iBlacklist = &aBlacklist;
	}

// ---------------------------------------------------------------------------
// GetMimeType
// ---------------------------------------------------------------------------
//
EXPORT_C void CHarvesterPlugin::GetMimeType( const TDesC& /*aUri*/, TDes& aMimeType )
    {
    aMimeType.Zero();
    }

// ---------------------------------------------------------------------------
// SetHarvesterPluginFactory
// ---------------------------------------------------------------------------
//
EXPORT_C void CHarvesterPlugin::SetHarvesterPluginFactory( CHarvesterPluginFactory& aFactory )
    {
    iFactory = &aFactory;
    }

// ---------------------------------------------------------------------------
// PluginInIdleState
// ---------------------------------------------------------------------------
//
EXPORT_C TBool CHarvesterPlugin::PluginInIdleState()
    {
    if( iState == EHarvesterIdle )
        {
        return ETrue;
        }
    return EFalse;
    }

// ---------------------------------------------------------------------------
// E32Dll
// ---------------------------------------------------------------------------
//
GLDEF_C TInt E32Dll()
	{
	return(KErrNone);
	}
