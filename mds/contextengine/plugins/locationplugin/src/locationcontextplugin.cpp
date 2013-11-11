/*
* Copyright (c) 2007-2009 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:  A Context Engine Plug-in to get location context information.
*
*/


#include <e32std.h>
#include <uri16.h>
#include <e32math.h>
#include <lbsposition.h> 
#include <etel3rdparty.h>

#include "locationcontextplugin.h"
#include "mdenamespacedef.h"
#include "mdeobject.h"
#include "mdeharvestersession.h"
#include "mdeproperty.h"
#include "harvesterlog.h"
#include "mdsutils.h"
#include "mdeconstants.h"
#include "mdeobjectdef.h"
#include "harvesterdata.h"

using namespace MdeConstants;

// ======== MEMBER FUNCTIONS ========

// ---------------------------------------------------------------------------
// Default constructor for first phase construction.
// Don't export these - used only by functions in this DLL, eg our NewL().
// ---------------------------------------------------------------------------
//
CLocationContextPlugin::CLocationContextPlugin() :
    iObserver( NULL ), iMdeSession( NULL ), iLocationTrailConnected( EFalse )
    {
    }


// ---------------------------------------------------------------------------
// Second phase construction
// ---------------------------------------------------------------------------
//
void CLocationContextPlugin::ConstructL() // second-phase constructor
    {
    // No implementation required
    }


// ---------------------------------------------------------------------------
// The usual NewL()
// ---------------------------------------------------------------------------
//
CLocationContextPlugin* CLocationContextPlugin::NewL()
    {
    CLocationContextPlugin* self = new (ELeave) CLocationContextPlugin();
    CleanupStack::PushL( self );
    self->ConstructL();
    CleanupStack::Pop( self );
    return self;
    }

    
// ---------------------------------------------------------------------------
// Destructor
// ---------------------------------------------------------------------------
//
CLocationContextPlugin::~CLocationContextPlugin()
    {
    iLocationTrail.Close();
    iManipulator.Close();
    }


// ---------------------------------------------------------------------------
// From CContextPlugin.
// Initialization of Location Context Plugin.
// ---------------------------------------------------------------------------
//
void CLocationContextPlugin::Init( MContextPluginObserver& aObserver )
    {
    WRITELOG("CLocationContextPlugin::Init()");
    
    TInt err = iLocationTrail.Connect();
    if ( err == KErrNone )
    	{
    	err = iManipulator.Connect();
    	if (err == KErrNone)
    		{
    		iLocationTrailConnected = ETrue;
    		}
    	}
    else
    	{
    	iLocationTrailConnected = EFalse;
    	}
    aObserver.PluginInitializationStatus( KErrNone ); // notify the context framework
    }

// ---------------------------------------------------------------------------
// Set a pointer to MdESession.
// Session is used to store harvested context data.
// ---------------------------------------------------------------------------
//
void CLocationContextPlugin::SetMdeSession( CMdESession& aSession )
    {
    iMdeSession = &aSession;
    }

// ---------------------------------------------------------------------------
// From CContextPlugin.
// This method takes a location context snapshot for one metadata object
// and writes the information to the object received as parameter.
// The observer will be informed about the status of the operation.
// ---------------------------------------------------------------------------
//
void CLocationContextPlugin::ContextSnapshot( MContextPluginObserver& aObserver,
	CHarvesterData& aHD )
    {
    WRITELOG( "CLocationContextPlugin::ContextSnapshot()" );
    
    if( !aHD.AddLocation() )
    	{
        WRITELOG( "CLocationContextPlugin::ContextSnapshot() - aHd.AddLocation returned false" );
    	aHD.SetErrorCode( KErrNone );
        aObserver.PluginSnapshotStatus( &aHD );
    	return;
    	}
    
    if ( !iLocationTrailConnected ) 
    	{        
        WRITELOG( "CLocationContextPlugin::ContextSnapshot() - try to reconnect" );      
        iManipulator.Close();
        iLocationTrail.Close();        
        TInt err = iLocationTrail.Connect();
        if ( err == KErrNone )
            {         
            err = iManipulator.Connect();
            if( err == KErrNone )
                {
                iLocationTrailConnected = ETrue;         
                }
            }
        
        if (err != KErrNone )
            {
            // no trail, no snapshot            
            aHD.SetErrorCode( KErrDisconnected );
            aObserver.PluginSnapshotStatus( &aHD );
            return;
            }
    	}
    
    TInt ret = KErrNone;
    
    ret = iManipulator.LocationSnapshot( aHD.MdeObject().Id() );
    
    if( ret == KErrServerTerminated )
        {        
        iLocationTrailConnected = EFalse;
        }    
    
    if( ret != KErrNone )
        {
        ret = KErrCompletion;
        }
    
    aHD.SetErrorCode( ret );
    aObserver.PluginSnapshotStatus( &aHD );
    }

// ---------------------------------------------------------------------------
// From CContextPlugin.
// This method takes a location context snapshot for several metadata objects
// and writes the information to the all the objects received as parameter.
// The observer will be informed about the status of the operation.
// ---------------------------------------------------------------------------
//
void CLocationContextPlugin::ContextSnapshot( MContextPluginObserver& aObserver,
    RPointerArray<CHarvesterData>& aHDArray )
    {
	const TInt count( aHDArray.Count() );
    WRITELOG( "CLocationContextPlugin::ContextSnapshot()" );

    if ( !iLocationTrailConnected ) // no trail, no snapshot
    	{
    	for(TInt i = 0; i < count; i++)
    		{
    		CHarvesterData* hd = aHDArray[i];
    		hd->SetErrorCode( KErrDisconnected );
    		aObserver.PluginSnapshotStatus( hd );
    		}
    	return;
    	}

    TLocationData data;
    RLocationTrail::TTrailState state;
    CMdEProperty* timeProp = NULL;
    TInt ret = KErrNone;
    iLocationTrail.GetLocationTrailState( state );

    if ( state != RLocationTrail::ETrailStopped && state != RLocationTrail::ETrailStopping )
    	{
    	for (TInt i = 0; i < aHDArray.Count(); ++i)
    		{
    		CMdEObject& mdeObject = ((CHarvesterData*)aHDArray[i])->MdeObject();
    		TRAP( ret, GetMdeObjectCreationTimeL( mdeObject, timeProp ) );
    		if ( ret != KErrNone )
    			{
    			continue;
    			}
#ifdef _DEBUG
    		TRAP_IGNORE(
    		WRITELOG1( "CLocationContextPlugin::ContextSnapshot() - timeProp: %Ld", 
    				timeProp->TimeValueL().Int64() ) );
    		WRITELOG1( "CLocationContextPlugin::ContextSnapshot() - Location trail state: %d", 
    				state );
#endif
    				
    		TRAP_IGNORE(ret = iLocationTrail.RetrieveLocation( timeProp->TimeValueL(), data, state ));

#ifdef _DEBUG
    		WRITELOG1( "CLocationContextPlugin::ContextSnapshot() - Retrieve location returned: %d", ret );
    		WRITELOG1( "CLocationContextPlugin::ContextSnapshot() - Latitude was: %f", data.iPosition.Latitude() );
    		WRITELOG1( "CLocationContextPlugin::ContextSnapshot() - Longitude was: %f", data.iPosition.Longitude() );
    		WRITELOG1( "CLocationContextPlugin::ContextSnapshot() - Altitude was: %f", data.iPosition.Altitude() );
#endif
        
    		if (ret == KErrNone)
    			{
    			if ( !Math::IsNaN(data.iPosition.Latitude() ) ||
   	        		 !Math::IsNaN(data.iPosition.Longitude() ) || 
   	        		 !Math::IsNaN(data.iPosition.Altitude() ) ||
   	        		 data.iNetworkInfo.iCellId > 0 || data.iNetworkInfo.iLocationAreaCode > 0 ||
   	        		 data.iNetworkInfo.iCountryCode.Length() > 0 ||
   	        		 data.iCountry.Length() > 0 || data.iNetworkInfo.iNetworkId.Length() > 0 )
   	        		{
#ifdef _DEBUG
	    			WRITELOG1( "CLocationContextPlugin::ContextSnapshot() Create location object for id: %d", mdeObject.Id());
#endif
	    			ret = iManipulator.CreateLocationObject(data, mdeObject.Id());
	    			WRITELOG1( "CLocationContextPlugin::ContextSnapshot() Create location object returned: %d", ret);
   	        		}
    			}
	        else
	        	{
	        	ret = KErrNone;
	        	}
    		}
        }

    // Notify to context framework
    for(TInt i = 0; i < count; i++)
        {
        CHarvesterData* hd = aHDArray[i];
        hd->SetErrorCode( KErrNone );
        aObserver.PluginSnapshotStatus( hd );
        }
    }

// ---------------------------------------------------------------------------
// Get mde object creation date/time.
// ---------------------------------------------------------------------------
//
void CLocationContextPlugin::GetMdeObjectCreationTimeL( CMdEObject& aObject, 
	CMdEProperty*& aProperty )
    {
    User::LeaveIfNull( iMdeSession );
    CMdENamespaceDef& namespaceDef = iMdeSession->GetDefaultNamespaceDefL();

    CMdEObjectDef& objectDef = namespaceDef.GetObjectDefL( Object::KBaseObject );
    CMdEPropertyDef& creationDef = objectDef.GetPropertyDefL( Object::KLastModifiedDateProperty );

    CMdEProperty* property = NULL;
    aObject.Property( creationDef, property, 0 );
    if ( !property )
        {
        User::Leave( KErrNotFound );
        }
    aProperty = property;
    }
