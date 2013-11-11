/*
* Copyright (c) 2009 Nokia Corporation and/or its subsidiary(-ies).
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
* Description:  CLBTContextSourcePlugin DLL source
*
*/

//  Include Files  

#define __INCLUDE_CAPABILITY_NAMES__
#include <e32capability.h>

#include "lbtcontextsourceplugin.h"	// CLBTContextSourcePlugin
#include "lbtcontextsourceplugindebug.h"


// constants
_LIT (KTriggerName, "name");
_LIT (KTriggerLocation, "TriggerLocation");
_LIT (KLatitude, "latitude");
_LIT (KLongitude, "longitude");
_LIT (KRadius, "radius");
_LIT (KRegion, "region");
_LIT (KSource, "setName");
_LIT (KDirection, "Direction");
_LIT (KCurrentState, "CurrentState");

// ---------------------------------------------------------------------------
// CLBTContextSourcePlugin::NewLC
// ---------------------------------------------------------------------------
//
CLBTContextSourcePlugin* CLBTContextSourcePlugin::NewLC( TContextSourceInitParams* aParams )
    {
    CLBTContextSourcePlugin* self = new (ELeave) CLBTContextSourcePlugin( aParams );
    CleanupStack::PushL(self);
    self->ConstructL();
    return self;
    }

// ---------------------------------------------------------------------------
// CLBTContextSourcePlugin::NewL
// ---------------------------------------------------------------------------
//
CLBTContextSourcePlugin* CLBTContextSourcePlugin::NewL(TContextSourceInitParams* aParams)
    {
    CLBTContextSourcePlugin* self = CLBTContextSourcePlugin::NewLC( aParams );
    CleanupStack::Pop(self);
    return self;
    }

// ---------------------------------------------------------------------------
// CLBTContextSourcePlugin::ConstructL
// ---------------------------------------------------------------------------
//
void CLBTContextSourcePlugin::ConstructL()
    {
    
    // Start listening to trigger fire events
        iLocationTrigger = CLocationTriggerAdapter::NewL(*this);
        
    // Start monitoring Management UI events
    iLbtManagerObserver = CLocationTriggerManager::NewL();
    iLbtManagerObserver->StartManagerObserver();
    }

// ---------------------------------------------------------------------------
// CLBTContextSourcePlugin::CLBTContextSourcePlugin
// ---------------------------------------------------------------------------
//
CLBTContextSourcePlugin::CLBTContextSourcePlugin( 
TContextSourceInitParams* aParams ):
CCFContextSourcePlugIn( aParams ),
iTriggersCreated(FALSE)
    {
    }

// ---------------------------------------------------------------------------
// CLBTContextSourcePlugin::~CLBTContextSourcePlugin
// ---------------------------------------------------------------------------
//
CLBTContextSourcePlugin::~CLBTContextSourcePlugin()
    {
    iTriggerList.ResetAndDestroy();
    iTriggerList.Close();
    
    iTriggerNodeList.ResetAndDestroy();
    iTriggerNodeList.Close();
    
    if( iLocationTrigger )
        {
        delete iLocationTrigger;
        iLocationTrigger = NULL;
        }
    
    if( iSettingList )
        {
        delete iSettingList;
        iSettingList = NULL;
        }
    
    if( iLbtManagerObserver )
        {
        delete iLbtManagerObserver;
        iLbtManagerObserver = NULL;
        }
    }

// ---------------------------------------------------------------------------
// CLBTContextSourcePlugin::HandleSettingL
// ---------------------------------------------------------------------------
//
void CLBTContextSourcePlugin::HandleSettingL(
        CCFContextSourceSettingArray* aSettingList )
    {
    
    // Cancel all active requests if any
    if (iLocationTrigger->IsActive())
        {
        iLocationTrigger->Cancel();
        }
    
    if( !iSettingList )
        {
        delete iSettingList;
        iSettingList = NULL;
        }
    // take ownership of this object
    iSettingList = aSettingList;
    
    // Clear contents of previous valid trigger specifications
    iTriggerList.ResetAndDestroy();
    iTriggerList.Close();
    
    // Clear contents of array of created triggers
    iTriggerNodeList.ResetAndDestroy();
    iTriggerNodeList.Close();

    // Read from XML file(s) and save onto array
    for ( TInt i = 0; i < aSettingList->Count(); i++ )
        {
        const CCFContextSourceSetting& item = aSettingList->Item( i );
	TInt error = KErrNone;
        TRAP( error, ParseTriggerInformationL( item ) );
        LBTCONTEXTSOURCEPLUGINDEBUG1( " error on ParseTriggerInformationL %d", error );
        }

    if( iTriggerNodeList.Count()>0 )
        {
            iCurrentTriggerIndex = 0;
            TInt err= KErrNone;
            TRAP(err, CreateTriggersL( ));
            while(err != KErrNone)
                {
                iCurrentTriggerIndex++;
                if( iCurrentTriggerIndex <= iTriggerNodeList.Count()-1 )
                    {
                    TRAP( err, CreateTriggersL( ) );
                    }
                }
        }

    }

// ---------------------------------------------------------------------------
// CLBTContextSourcePlugin::ParseTriggerInformationL
// ---------------------------------------------------------------------------
//
void CLBTContextSourcePlugin::ParseTriggerInformationL( const CCFContextSourceSetting& aItem )
    {
    // Flages used to check if the tags are present & also to check repetition of tags
    TBool triggerNameSet = EFalse;
    TBool latitudeSet = EFalse;
    TBool longitudeSet = EFalse;
    TBool regionSet = EFalse;
    TBool radiusSet = EFalse;
    TBool contextSourceSet = EFalse;
    TBool directionSet = EFalse;
    TBool currentStateSet = EFalse;
    
    
    LBTCONTEXTSOURCEPLUGINDEBUG( "CLBTContextSourcePlugin::ParseTriggerInformationL : IN " );

    // Add check for improper syntax of XML file
    if ( aItem.Name() != KTriggerLocation )     
    {
    User::Leave( KErrGeneral );
    }
    
    TTriggerNode *triggerNode = new(ELeave) TTriggerNode();
    
    
    const RKeyValueArray& paramAttr = aItem.Attributes();
    for( TInt k = 0; k < paramAttr.Count(); ++k )
        {
            {
            TPtrC key( paramAttr[k]->Key() );
            TPtrC value( paramAttr[ k ]->Value() );

            if ( ! (key.Compare( KTriggerName ))) 
                {
                // INdicate that the name tag has been parsed
                if( !triggerNameSet )
                    {
                    triggerNameSet = ETrue;
                    LBTCONTEXTSOURCEPLUGINDEBUG1( "SetTriggerNameL = %S", &value);
                    triggerNode->SetTriggerNameL( value );
                    }
                }
            else if(!(key.CompareF( KLatitude ) ))
                {
                // Indicate that the latitude tag has been parsed
                if( !latitudeSet )
                    {
                    latitudeSet = ETrue;
                    LBTCONTEXTSOURCEPLUGINDEBUG1( "SetLatitudeL = %S", &value);
                    triggerNode->SetLatitudeL( value );
                    }
                }
            
            else if( !(key.CompareF( KLongitude ) ))
                {
                // Indicate that the longitude tag has been parsed
                if( !longitudeSet )
                    {
                    longitudeSet = ETrue;
                    LBTCONTEXTSOURCEPLUGINDEBUG1( "SetLongitudeL = %S", &value);
                    triggerNode->SetLongitudeL( value );
                    }
                }
            else if (!(key.CompareF( KRegion ) ))
                {
                // Indicate that the region tag has been parsed
                if( !regionSet )
                    {
                    regionSet = ETrue;
                    LBTCONTEXTSOURCEPLUGINDEBUG1( "SetRegionL = %S", &value);
                    triggerNode->SetRegionL( value );
                    }
                }
            
            else if ( !(key.CompareF( KRadius ) ))
                {
                // Indicate that the radius tag has been parsed
                if( !radiusSet )
                    {
                    radiusSet = ETrue;
                    LBTCONTEXTSOURCEPLUGINDEBUG1( "SetRadiusL = %S", &value);
                    triggerNode->SetRadiusL( value );
                    }
                }
            else if ( !(key.CompareF( KSource ) ) )
                {
                // Indicate that the contextsource tag has been parsed
                if( !contextSourceSet )
                    {
                    contextSourceSet = ETrue;
                    LBTCONTEXTSOURCEPLUGINDEBUG1( "SetContextSource = %S", &value);
                    triggerNode->SetContextSource( value );
                    }
                }
            else if ( !(key.CompareF( KDirection ) ) )
                {
                // Indicate that the direction tag has been parsed
                if( !directionSet )
                    {
                    directionSet = ETrue;
                    LBTCONTEXTSOURCEPLUGINDEBUG1( "SetDirection = %S", &value); 
                    triggerNode->SetDirectionL( value );
                    }
                }
            else if ( !(key.CompareF(KCurrentState) ) )
                {
                // Indicate that the current state tag has been parsed
                if( !currentStateSet )
                    {
                    currentStateSet = ETrue;
                    LBTCONTEXTSOURCEPLUGINDEBUG1( "SetCurrentState = %S", &value);    
                    triggerNode->SetCurrentStateL( value );
                    }
                }
            }
        }
    // Check if mandatory tags are present before appending to the trigger node list
    if( triggerNameSet && latitudeSet && longitudeSet && regionSet && radiusSet )
        {
        // Check if optional tags are present,if not set default values
        if( !contextSourceSet )
            {
            TPtrC defaultContextSource( KDefaultContextSourceName );
            triggerNode->SetContextSource( defaultContextSource );
            }
        if( !directionSet )
            {
            TPtrC defaultdirection( KNullDesC );
            triggerNode->SetDirectionL( defaultdirection );
            }
        if( !currentStateSet )
            {
            TPtrC defaultCurrentState ( KNullDesC );
            triggerNode->SetCurrentStateL( defaultCurrentState );
            }
        // Add to triggernode list
        iTriggerNodeList.AppendL( triggerNode );
        }
    else
        {
        // delete triggerNode
        delete triggerNode;
        User::Leave( KErrGeneral );
        }
    
    LBTCONTEXTSOURCEPLUGINDEBUG( "CLBTContextSourcePlugin::ParseTriggerInformationL : OUT " );

    }


// ---------------------------------------------------------------------------
// CLBTContextSourcePlugin::CreateTriggersL
// ---------------------------------------------------------------------------
//
void CLBTContextSourcePlugin::CreateTriggersL( )
    {
    if( iCurrentTriggerIndex<iTriggerNodeList.Count())
        {
        iLocationTrigger->CreateContextTriggerL( *iTriggerNodeList[iCurrentTriggerIndex] );
        }
    }

// ---------------------------------------------------------------------------
// CLBTContextSourcePlugin::InitializeL
// ---------------------------------------------------------------------------
//
void CLBTContextSourcePlugin::InitializeL()
    {
    
    }

// ---------------------------------------------------------------------------
// CLBTContextSourcePlugin::DefineContextsL
// ---------------------------------------------------------------------------
//
void CLBTContextSourcePlugin::DefineContextsL()
    {
    if(iTriggersCreated)
        {
            DefineTriggerContexts();
        }
    }

// ---------------------------------------------------------------------------
// CLBTContextSourcePlugin::DefineTriggerContextsL
// ---------------------------------------------------------------------------
//
void CLBTContextSourcePlugin::DefineTriggerContexts( )
    {
    TSecurityPolicy policy( ECapabilityLocalServices );

    for(int i=0; i<iTriggerList.Count() ; i++)
        {
        TInt err = iCF.DefineContext( iTriggerList[i]->contextName, KContextType, policy);
        LBTCONTEXTSOURCEPLUGINDEBUG1( "Defining ContextSource = %S", &iTriggerList[i]->contextName);
        }
    }

// ---------------------------------------------------------------------------
// CLBTContextSourcePlugin::ContextTriggerFiredL
// ---------------------------------------------------------------------------
//
void CLBTContextSourcePlugin::ContextTriggerFiredL( TLbtTriggerId aTriggerId )
    {
    LBTCONTEXTSOURCEPLUGINDEBUG( "ContextTriggerFiredL :IN");
    TPtrC contextSource(KNullDesC);
    TPtrC contextName(KNullDesC);
    
    for(int i=0; i<iTriggerList.Count(); i++)
        {
        if (iTriggerList[i]->iTriggerId == aTriggerId)
            {
            contextSource.Set(iTriggerList[i]->contextName);
            contextName.Set(iTriggerList[i]->triggerName);
            break;
            }
        }
        
    CCFContextObject* context = CCFContextObject::NewLC();

    context->SetSourceL( contextSource );
    context->SetTypeL( KContextType );
    context->SetValueL( contextName );   
                    
    // Publish new context value
    RThread thread;
    CleanupClosePushL( thread );
    iCF.PublishContext( *context, thread );
    LBTCONTEXTSOURCEPLUGINDEBUG1( "Publishing Context = %S", &contextSource);
    LBTCONTEXTSOURCEPLUGINDEBUG1( "Context Value = %S", &contextName);
    thread.Close();
    
    CleanupStack::Pop( &thread );
    CleanupStack::PopAndDestroy( context );
    LBTCONTEXTSOURCEPLUGINDEBUG( "ContextTriggerFiredL :OUT");
    }

// ---------------------------------------------------------------------------
// CLBTContextSourcePlugin::TriggerCreatedL
// ---------------------------------------------------------------------------
//
void CLBTContextSourcePlugin::TriggerCreatedL( TLbtTriggerId& aTrigId , 
                                                TPtrC& aContextName, 
                                                TPtrC& aTriggerName )
    {
    TriggerList* newTrigger;
    newTrigger = new (ELeave) TriggerList;
    CleanupStack::PushL(newTrigger);
    newTrigger->iTriggerId = aTrigId;
    newTrigger->contextName.Set( aContextName );
    newTrigger->triggerName.Set( aTriggerName );
        
    iTriggerList.AppendL(newTrigger);
    iCurrentTriggerIndex++;
    if( iCurrentTriggerIndex <= iTriggerNodeList.Count()-1 )
        {            
        TInt err = KErrNone;
        TRAP(err, CreateTriggersL( ));
        while(err != KErrNone)
            {
            iCurrentTriggerIndex++;
            if( iCurrentTriggerIndex <= iTriggerNodeList.Count()-1 )
                {
                TRAP( err, CreateTriggersL() );
                }
            }
        }
    else
        {
        iTriggersCreated = TRUE;
        DefineContextsL();
        }
    CleanupStack::Pop(newTrigger);
    }

// ---------------------------------------------------------------------------
// CLBTContextSourcePlugin::NotifyError
// ---------------------------------------------------------------------------
//
void CLBTContextSourcePlugin::NotifyError( TInt /*aError*/ )
    {
    }


// ---------------------------------------------------------------------------
// CLBTContextSourcePlugin::Extension
// ---------------------------------------------------------------------------
//
TAny* CLBTContextSourcePlugin::Extension( const TUid& aExtensionUid ) const
    {
    // Check the extension
    if ( aExtensionUid == KCFContextSourceInterfaceUid )
            {
            return ( ( MCFContextSource* ) this );
            }
        else if ( aExtensionUid == KCFContextSourceUpgradeUid )
            {
            return ( ( MCFContextSourceUpgrade* ) this );
            }
        else
            {
            return NULL;
            }
    }
    
// ---------------------------------------------------------------------------
// CLBTContextSourcePlugin::UpdateSettingsL
// ---------------------------------------------------------------------------
//
void CLBTContextSourcePlugin::UpdateSettingsL( CCFContextSourceSettingArray* aSettingList )
    {
        LBTCONTEXTSOURCEPLUGINDEBUG( "UpdateSettingsL() : IN");
        
        // Reset list index number to zero.
        iCurrentTriggerIndex = 0;
        // Stop monitoring the triggers to avoid a call back for this
        // delete
        iLbtManagerObserver->Cancel();
        
        // Delete all existing triggers created by the plugin
        LBTCONTEXTSOURCEPLUGINDEBUG( "DeleteAllTriggersL");
        iTriggersCreated = FALSE;
        TRAPD(error, iLocationTrigger->DeleteAllTriggersL() ); 
        // CHeck if the delete is leaving because there is no triggers 
        // to be created(i.e KErrNotFound),if yes then go ahead & create 
        // the triggers
        if( !error || error == KErrNotFound )
            {
            // start observer
            iLbtManagerObserver->StartManagerObserver();
            // Read updated setting files and re-create triggers
            HandleSettingL( aSettingList );
            }
        LBTCONTEXTSOURCEPLUGINDEBUG( "UpdateSettingsL() : OUT");
    }

// ---------------------------------------------------------------------------
// CLBTContextSourcePlugin::CheckValidity
// ---------------------------------------------------------------------------
//
TInt CLBTContextSourcePlugin::CheckValidity( RThread& aClientThread, 
        const CCFContextSourceSettingArray& aSettingList )
    {
    TBool validFile = EFalse;
    
    // Check capabilities of client application
    if ( !aClientThread.HasCapability( ECapabilityLocation ) )
        {
        return KErrPermissionDenied;
        }
    
    // Check that the file contains atleast a single valid node,else
    // leave with KErrNotSupported
    for ( TInt i = 0; i < aSettingList.Count(); i++ )
        {
        const CCFContextSourceSetting& item = aSettingList.Item( i );

        // Add check for improper syntax of XML file
        if ( item.Name() != KTriggerLocation )     
            {
            validFile = EFalse; 
            }
        else
            {
            validFile = ETrue;
            break;
            }
        }
    
    if ( validFile )
        {
        LBTCONTEXTSOURCEPLUGINDEBUG( "CheckValidity() : OUT , returning KErrNone");
        return KErrNone;
        }
    else
        {
        LBTCONTEXTSOURCEPLUGINDEBUG( "CheckValidity() : OUT , returning KErrNotSupported" );
        return KErrNotSupported;
        }
    }
