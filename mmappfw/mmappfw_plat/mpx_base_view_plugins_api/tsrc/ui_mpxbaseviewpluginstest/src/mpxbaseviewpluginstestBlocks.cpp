/*
* Copyright (c) 2002 - 2007 Nokia Corporation and/or its subsidiary(-ies).
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
* Description:  Cmpxbaseviewpluginstest block implementation for STIF Test Framework TestScripter
*
*/

// [INCLUDE FILES] - do not remove
#include <e32svr.h>
#include <StifParser.h>
#include <Stiftestinterface.h>
#include "mpxbaseviewpluginstest.h"

// -----------------------------------------------------------------------------
// Cmpxbaseviewpluginstest::Delete
// Delete here all resources allocated and opened from test methods. 
// Called from destructor. 
// -----------------------------------------------------------------------------
//
void Cmpxbaseviewpluginstest::Delete() 
    {
    // CMPXViewPlugin
    }

// -----------------------------------------------------------------------------
// Cmpxbaseviewpluginstest::RunMethodL
// Run specified method. Contains also table of test mothods and their names.
// -----------------------------------------------------------------------------
//
TInt Cmpxbaseviewpluginstest::RunMethodL( 
    CStifItemParser& aItem ) 
    {

    static TStifFunctionInfo const KFunctions[] =
        {  
        // Copy this line for every implemented function.
        // First string is the function name used in TestScripter script file.
        // Second is the actual implementation member function. 
        ENTRY( "Example", Cmpxbaseviewpluginstest::ExampleL ),
      
        ENTRY( "CMPXViewPluginNewL", Cmpxbaseviewpluginstest::CMPXViewPluginNewL ),
        ENTRY( "CMPXViewPluginDestruct", Cmpxbaseviewpluginstest::CMPXViewPluginDestruct ),
     
        ENTRY( "CMPXAknViewPluginDestruct", Cmpxbaseviewpluginstest::CMPXAknViewPluginDestruct ),
        ENTRY( "CMPXAknViewPluginCreateViewL", Cmpxbaseviewpluginstest::CMPXAknViewPluginCreateViewL ),
        ENTRY( "CMPXAknViewPluginDestroyView", Cmpxbaseviewpluginstest::CMPXAknViewPluginDestroyView ),
        ENTRY( "CMPXAknViewPluginActivateViewL", Cmpxbaseviewpluginstest::CMPXAknViewPluginActivateViewL ),
        ENTRY( "CMPXAknViewPluginSetAsDefaultViewL", Cmpxbaseviewpluginstest::CMPXAknViewPluginSetAsDefaultViewL ),
        ENTRY( "CMPXAknViewPluginDeactivateView", Cmpxbaseviewpluginstest::CMPXAknViewPluginDeactivateView ),
        
        ENTRY( "CMPXAknDialogPluginDestruct", Cmpxbaseviewpluginstest::CMPXAknDialogPluginDestruct ),
        ENTRY( "CMPXAknDialogPluginCreateViewL", Cmpxbaseviewpluginstest::CMPXAknDialogPluginCreateViewL ),
        ENTRY( "CMPXAknDialogPluginDestroyView", Cmpxbaseviewpluginstest::CMPXAknDialogPluginDestroyView ),
        ENTRY( "CMPXAknDialogPluginActivateViewL", Cmpxbaseviewpluginstest::CMPXAknDialogPluginActivateViewL ),
        ENTRY( "CMPXAknDialogPluginSetAsDefaultViewL", Cmpxbaseviewpluginstest::CMPXAknDialogPluginSetAsDefaultViewL ),
        ENTRY( "CMPXAknDialogPluginDeactivateView", Cmpxbaseviewpluginstest::CMPXAknDialogPluginDeactivateView ),
        ENTRY( "CDummyMPXAknViewPluginConstruct", Cmpxbaseviewpluginstest::CDummyMPXAknViewPluginConstruct ),
        ENTRY( "CDummyMPXAknDialogPluginConstruct", Cmpxbaseviewpluginstest::CDummyMPXAknDialogPluginConstruct ),
        ENTRY( "CDummyMPXViewPluginDesConstruct", Cmpxbaseviewpluginstest::CDummyMPXViewPluginDesConstruct ),
               
        
        //ADD NEW ENTRY HERE 
        //[test cases entries] - Do not remove

        };

    const TInt count = sizeof( KFunctions ) / 
                        sizeof( TStifFunctionInfo );

    return RunInternalL( KFunctions, count, aItem );

    }

// -----------------------------------------------------------------------------
// Cmpxbaseviewpluginstest::ExampleL
// Example test method function.
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
TInt Cmpxbaseviewpluginstest::ExampleL( CStifItemParser& aItem )
    {

    // Print to UI
    _LIT( Kmpxbaseviewpluginstest, "mpxbaseviewpluginstest" );
    _LIT( KExample, "In Example" );
    TestModuleIf().Printf( 0, Kmpxbaseviewpluginstest, KExample );
    // Print to log file
    iLog->Log( KExample );
    
    TInt i = 0;
    TPtrC string;
    _LIT( KParam, "Param[%i]: %S" );
    while ( aItem.GetNextString ( string ) == KErrNone )
        {
        TestModuleIf().Printf( i, Kmpxbaseviewpluginstest, 
                                KParam, i, &string );
        i++;
        }

    return KErrNone;

    }

// -----------------------------------------------------------------------------
// Cmpxbaseviewpluginstest::CMPXViewPluginNewL
// Create CMPXViewPlugin
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
TInt Cmpxbaseviewpluginstest::CMPXViewPluginNewL( CStifItemParser& /*aItem*/ )
    {
    iLog->Log(_L("Cmpxbaseviewpluginstest::CMPXViewPluginNewL"));
    TInt err = KErrNone;
    if ( iViewPlugin != NULL)
            {
            delete iViewPlugin;
            }
    iViewPlugin = NULL;

	TUid aUid = TUid::Uid(270531770);

    iViewPlugin = CMPXViewPlugin::NewL( aUid, this );
    if ( iViewPlugin == NULL)
        {
        err = KErrNotFound;
        }   
    iLog->Log(_L("Error: CMPXViewPluginNewL not created: %d"), err);
	return err;   
    }
// -----------------------------------------------------------------------------
// Cmpxbaseviewpluginstest::CMPXViewPluginDestruct
// Destruct CMPXViewPlugin
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
TInt Cmpxbaseviewpluginstest::CMPXViewPluginDestruct( CStifItemParser& /*aItem*/ )
    {
    iLog->Log(_L("Cmpxbaseviewpluginstest::CMPXViewPluginDestruct"));
    TInt err = KErrNone;
    if ( iViewPlugin )
           {
           delete iViewPlugin;
           REComSession::FinalClose();
           iViewPlugin = NULL;
           }
    if ( err != KErrNone)
    	{
         iLog->Log(_L("Cmpxbaseviewpluginstest::DeleteCMPXViewPlugin testing CMPXViewPlugin::~ end err=%d"), err);
    	}  	 
    return err;
    }

// -----------------------------------------------------------------------------
// Cmpxbaseviewpluginstest::CMPXAknViewPluginDestruct
// Destruct CMPXAknViewPlugin
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
TInt Cmpxbaseviewpluginstest::CMPXAknViewPluginDestruct( CStifItemParser& /*aItem*/ )
    {
    iLog->Log(_L("Cmpxbaseviewpluginstest::CMPXAknViewPluginDestruct"));
    TInt err = KErrNone;
    iViewPlugin = CMPXViewPlugin::NewL( KMPXAudioEffectsViewImplementationId, this );
    if ( iViewPlugin )
	      {
	       delete iViewPlugin;
	       REComSession::FinalClose();
	       iViewPlugin = NULL;
	      }
    if ( err != KErrNone )
    	 {
    	 iLog->Log(_L("Cmpxbaseviewpluginstest::DeleteCMPXAknViewPlugin testing CMPXAknViewPlugin::~ end err=%d"), err); 	 
    	 }
     return err;
    }
// -----------------------------------------------------------------------------
// Cmpxbaseviewpluginstest::CDummyMPXViewPluginConstruct
// CDummyMPXViewPlugin Construct
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
TInt Cmpxbaseviewpluginstest::CDummyMPXAknViewPluginConstruct( CStifItemParser& /*aItem*/ )
    {
    iLog->Log(_L("Cmpxbaseviewpluginstest::CMPXAknViewPluginCreateViewL"));
    TInt err = KErrNone;

   	idummyMPXViewPlugin = new CDummyMPXViewPlugin(KMPXAudioEffectsViewImplementationId,this);
   		
    if ( err != KErrNone )
	    {
         iLog->Log(_L("Cmpxbaseviewpluginstest::CMPXAknViewPluginCreateViewL err=%d"), err);
	    } 
    return err;
    }
// -----------------------------------------------------------------------------
// Cmpxbaseviewpluginstest::CDummyMPXViewPluginConstruct
// CDummyMPXViewPlugin Construct
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
TInt Cmpxbaseviewpluginstest::CDummyMPXAknDialogPluginConstruct( CStifItemParser& /*aItem*/ )
    {
    iLog->Log(_L("Cmpxbaseviewpluginstest::CMPXAknViewPluginCreateViewL"));
    TInt err = KErrNone;
 
   	idummyMPXViewPlugin = new CDummyMPXViewPlugin(KMPXMetadataEditorDialogImplementationId,this);
   		
    if ( err != KErrNone )
	    {
         iLog->Log(_L("Cmpxbaseviewpluginstest::CMPXAknViewPluginCreateViewL err=%d"), err);
	    } 
    return err;
    }
// -----------------------------------------------------------------------------
// Cmpxbaseviewpluginstest::CDummyMPXViewPluginDesConstruct
// CDummyMPXViewPlugin DesConstruct
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
TInt Cmpxbaseviewpluginstest::CDummyMPXViewPluginDesConstruct( CStifItemParser& /*aItem*/ )
    {
    iLog->Log(_L("Cmpxbaseviewpluginstest::CMPXAknViewPluginCreateViewL"));
    TInt err = KErrNone;
    if ( idummyMPXViewPlugin != NULL )
    	{
    	delete idummyMPXViewPlugin;
    	idummyMPXViewPlugin = NULL;
    	}
    	
    if ( err != KErrNone )
    	  {
           iLog->Log(_L("Cmpxbaseviewpluginstest::CMPXAknViewPluginCreateViewL err=%d"), err);
    	  } 
    return err;
    }
// -----------------------------------------------------------------------------
// Cmpxbaseviewpluginstest::CMPXAknViewPluginCreateViewL
// CMPXAknViewPlugin CreateViewL
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
TInt Cmpxbaseviewpluginstest::CMPXAknViewPluginCreateViewL( CStifItemParser& /*aItem*/ )
    {
    iLog->Log(_L("Cmpxbaseviewpluginstest::CMPXAknViewPluginCreateViewL"));
    TInt err = KErrNone;
    
    TRAP( err, idummyMPXViewPlugin->CreateViewL());

    if ( err != KErrNone )
    	  {
           iLog->Log(_L("Cmpxbaseviewpluginstest::CMPXAknViewPluginCreateViewL err=%d"), err);
    	  } 
    return err;
    }
// -----------------------------------------------------------------------------
// Cmpxbaseviewpluginstest::CMPXAknViewPluginDestroyView
// CMPXAknViewPlugin DestroyView
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
TInt Cmpxbaseviewpluginstest::CMPXAknViewPluginDestroyView( CStifItemParser& /*aItem*/ )
    {
    iLog->Log(_L("Cmpxbaseviewpluginstest::CMPXAknViewPluginDestroyView"));
    TInt err = KErrNone;
    
    idummyMPXViewPlugin->DestroyView();
    
    if ( err != KErrNone )
    	  {
           iLog->Log(_L("Cmpxbaseviewpluginstest::CMPXAknViewPluginDestroyView end err=%d"), err);
    	  }  
    
    return err;
    }
// -----------------------------------------------------------------------------
// Cmpxbaseviewpluginstest::CMPXAknViewPluginActivateViewL
// CMPXAknViewPlugin ActivateViewL
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
TInt Cmpxbaseviewpluginstest::CMPXAknViewPluginActivateViewL( CStifItemParser& /*aItem*/ )
    {
    iLog->Log(_L("Cmpxbaseviewpluginstest::CMPXAknViewPluginActivateViewL"));
    TInt err = KErrNone;

    _LIT( Kparam, "viewname" );
    const TDesC* KFrom = &Kparam;

    TRAP(err,idummyMPXViewPlugin->ActivateViewL());
    
    if ( err != KErrNone )
    	  {
           iLog->Log(_L("Cmpxbaseviewpluginstest::CMPXAknViewPluginActivateViewL err=%d"), err);
    	  }  	 
    return err;
    }
// -----------------------------------------------------------------------------
// Cmpxbaseviewpluginstest::CMPXAknViewPluginSetAsDefaultViewL
// CMPXAknViewPlugin SetAsDefaultViewL
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
TInt Cmpxbaseviewpluginstest::CMPXAknViewPluginSetAsDefaultViewL( CStifItemParser& /*aItem*/ )
    {
    iLog->Log(_L("Cmpxbaseviewpluginstest::CMPXAknViewPluginSetAsDefaultViewL"));
    TInt err = KErrNone;

    TRAP(err,idummyMPXViewPlugin->SetAsDefaultViewL());
    
    if ( err != KErrNone )
    	  {
           iLog->Log(_L("Cmpxbaseviewpluginstest::CMPXAknViewPluginSetAsDefaultViewL err=%d"), err);
    	  }  	 
    return err;
    }
// -----------------------------------------------------------------------------
// Cmpxbaseviewpluginstest::CMPXAknViewPluginDeactivateView
// CMPXAknViewPlugin DeactivateView
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
TInt Cmpxbaseviewpluginstest::CMPXAknViewPluginDeactivateView( CStifItemParser& /*aItem*/ )
    {
    iLog->Log(_L("Cmpxbaseviewpluginstest::CMPXAknViewPluginDeactivateView"));
    TInt err = KErrNone;

    idummyMPXViewPlugin->DeactivateView();
    
    if ( err != KErrNone )
    	  {
           iLog->Log(_L("Cmpxbaseviewpluginstest::CMPXAknViewPluginDeactivateView err=%d"), err);
    	  }  	 
    return err;
    }
// -----------------------------------------------------------------------------
// Cmpxbaseviewpluginstest::CMPXAknDialogPluginDestruct
// CMPXAknDialogPlugin Destruct
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
TInt Cmpxbaseviewpluginstest::CMPXAknDialogPluginDestruct( CStifItemParser& /*aItem*/ )
{
	iLog->Log(_L("Cmpxbaseviewpluginstest::CMPXAknDialogPluginDestruct"));
	TInt err = KErrNone;
	
	iViewPlugin = CMPXViewPlugin::NewL( KMPXMetadataEditorDialogImplementationId, this );
	if ( iViewPlugin )
	       {
	       TRAP(err , delete iViewPlugin);
	       REComSession::FinalClose();
	       iViewPlugin = NULL;
	       }
	if ( err != KErrNone )
		  {
	       iLog->Log(_L("Cmpxbaseviewpluginstest::CMPXAknDialogPluginDestruct err=%d"), err);
		  }  	 
	return err;
}
// -----------------------------------------------------------------------------
// Cmpxbaseviewpluginstest::CMPXAknDialogPluginCreateViewL
// CMPXAknDialogPlugin CreateViewL
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
TInt Cmpxbaseviewpluginstest::CMPXAknDialogPluginCreateViewL( CStifItemParser& /*aItem*/ )
    {
    iLog->Log(_L("Cmpxbaseviewpluginstest::CMPXAknDialogPluginCreateViewL"));
    TInt err = KErrNone;
    
    TRAP( err, idummyMPXViewPlugin->CreateViewL());

    if ( err != KErrNone )
    	  {
           iLog->Log(_L("Cmpxbaseviewpluginstest::CMPXAknDialogPluginCreateViewL err=%d"), err);
    	  } 
    return err;
    }
// -----------------------------------------------------------------------------
// Cmpxbaseviewpluginstest::CMPXAknDialogPluginDestroyView
// CMPXAknDialogPlugin DestroyView
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
TInt Cmpxbaseviewpluginstest::CMPXAknDialogPluginDestroyView( CStifItemParser& /*aItem*/ )
    {
    iLog->Log(_L("Cmpxbaseviewpluginstest::CMPXAknDialogPluginDestroyView"));
    TInt err = KErrNone;

    idummyMPXViewPlugin->DestroyView();
    
    if ( err != KErrNone )
    	  {
           iLog->Log(_L("Cmpxbaseviewpluginstest::CMPXAknDialogPluginDestroyView end err=%d"), err);
    	  }  
    
    return err;
    }
// -----------------------------------------------------------------------------
// Cmpxbaseviewpluginstest::CMPXAknDialogPluginActivateViewL
// CMPXAknDialogPlugin ActivateViewL
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
TInt Cmpxbaseviewpluginstest::CMPXAknDialogPluginActivateViewL( CStifItemParser& /*aItem*/ )
    {
    iLog->Log(_L("Cmpxbaseviewpluginstest::CMPXAknDialogPluginActivateViewL"));
    TInt err = KErrNone;

    TRAP(err,idummyMPXViewPlugin->ActivateViewL());
    
    if ( err != KErrNone )
    	  {
           iLog->Log(_L("Cmpxbaseviewpluginstest::CMPXAknDialogPluginActivateViewL err=%d"), err);
    	  }  	 
    return err;
    }
// -----------------------------------------------------------------------------
// Cmpxbaseviewpluginstest::CMPXAknDialogPluginSetAsDefaultViewL
// CMPXAknDialogPlugin SetAsDefaultViewL
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
TInt Cmpxbaseviewpluginstest::CMPXAknDialogPluginSetAsDefaultViewL( CStifItemParser& /*aItem*/ )
    {
    iLog->Log(_L("Cmpxbaseviewpluginstest::CMPXAknDialogPluginSetAsDefaultViewL"));
    TInt err = KErrNone;

    TRAP(err,idummyMPXViewPlugin->SetAsDefaultViewL());
    
    if ( err == KErrNotSupported )
    	  {
           iLog->Log(_L("Cmpxbaseviewpluginstest::CMPXAknDialogPluginSetAsDefaultViewL err=%d"), err);
    	   err = KErrNone;
    	  }  	 
    return err;
    }
// -----------------------------------------------------------------------------
// Cmpxbaseviewpluginstest::CMPXAknDialogPluginDeactivateView
// CMPXAknDialogPlugin DeactivateView
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
TInt Cmpxbaseviewpluginstest::CMPXAknDialogPluginDeactivateView( CStifItemParser& /*aItem*/ )
    {
    iLog->Log(_L("Cmpxbaseviewpluginstest::CMPXAknDialogPluginDeactivateView"));
    TInt err = KErrNone;

    idummyMPXViewPlugin->DeactivateView();
    
    if ( err != KErrNone )
    	  {
           iLog->Log(_L("Cmpxbaseviewpluginstest::CMPXAknDialogPluginDeactivateView err=%d"), err);
    	  }  	 
    return err;
    }

// -----------------------------------------------------------------------------
// Cmpxbaseviewpluginstest::HandleCustomCommand
// Handle CustomCommand
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void Cmpxbaseviewpluginstest::HandleCustomCommand( 
    const TUid& /*aUid*/,
    const TInt /*aCommand*/,
    const TDesC8& /*aData*/ )
    {
	iLog->Log(_L("CCommonTestClass::HandleCustomCommand CMPXViewPlugin::HandleCustomCommand is called"));
    }

/*
TInt Cmpxbaseviewpluginstest::?member_function(
   CItemParser& aItem )
   {

   ?code

   }
*/

// ========================== OTHER EXPORTED FUNCTIONS =========================
// None

//  [End of File] - Do not remove
