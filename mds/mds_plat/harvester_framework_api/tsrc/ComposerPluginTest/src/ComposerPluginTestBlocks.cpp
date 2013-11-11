/*
* Copyright (c) 2002-2009 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:  
*
*/


// [INCLUDE FILES] - do not remove
#include <e32svr.h>
#include <StifParser.h>
#include <StifTestEventInterface.h>
#include <StifTestInterface.h>
#include "ComposerPluginTest.h"

#include "mdsutils.h"
#include "mdeconstants.h"

using namespace MdeConstants;

_LIT( KOpenErr, "        ModifyObjects - Open error : %d" );
_LIT( KCommErr, "        ModifyObjects - Commit error : %d" );
_LIT( KNull, "        ModifyObjects - NULL object" );
_LIT( KCommit, "        ModifyObjects - Object committed" );
_LIT( KUri, "C:\\Data\\Images\\Temp%d.jpg" );

// ============================ MEMBER FUNCTIONS ===============================

// -----------------------------------------------------------------------------
// CComposerPluginTest::Delete
// Delete here all resources allocated and opened from test methods. 
// Called from destructor. 
// -----------------------------------------------------------------------------
//
void CComposerPluginTest::Delete() 
    {

    }

// -----------------------------------------------------------------------------
// CComposerPluginTest::RunMethodL
// Run specified method. Contains also table of test mothods and their names.
// -----------------------------------------------------------------------------
//
TInt CComposerPluginTest::RunMethodL( 
    CStifItemParser& aItem ) 
    {

    static TStifFunctionInfo const KFunctions[] =
        {  
        // Copy this line for every implemented function.
        // First string is the function name used in TestScripter script file.
        // Second is the actual implementation member function. 

        ENTRY( "ActiveWait", CComposerPluginTest::ActiveWait ),
        ENTRY( "BeginComposerSession", CComposerPluginTest::BeginComposerSessionL ),
        ENTRY( "EndComposerSession", CComposerPluginTest::EndComposerSession ),
        ENTRY( "LoadPlugins", CComposerPluginTest::LoadPluginsL ),
        ENTRY( "SetSessions", CComposerPluginTest::SetSessions ),
        ENTRY( "RemoveSessions", CComposerPluginTest::RemoveSessions ),
        ENTRY( "ModifyObjects", CComposerPluginTest::ModifyObjectsL ),
        ENTRY( "DeleteObject", CComposerPluginTest::DeleteObjectL ),
        ENTRY( "DeleteObjects", CComposerPluginTest::DeleteObjectsL ),
        ENTRY( "WaitComposingComplete", CComposerPluginTest::WaitComposingComplete ),

        };

    const TInt count = sizeof( KFunctions ) / 
                        sizeof( TStifFunctionInfo );

    return RunInternalL( KFunctions, count, aItem );

    }

// -----------------------------------------------------------------------------
// CComposerPluginTest::HandleSessionOpened
// -----------------------------------------------------------------------------
//
void CComposerPluginTest::HandleSessionOpened( CMdESession& /*aClient*/, TInt aError )
    {
    _LIT( KMsg, "CallBck HandleSessionOpened - Error code : %d" );
    TBuf <100> msg;
    msg.Format(KMsg, aError);
    iLog->Log( msg );     
    RDebug::Print(msg);
    
    // session event
    TEventIf event( TEventIf::ESetEvent, _L("Session") );
    TestModuleIf().Event( event );
    }

// -----------------------------------------------------------------------------
// CComposerPluginTest::HandleSessionError
// -----------------------------------------------------------------------------
//
void CComposerPluginTest::HandleSessionError( CMdESession& /*aClient*/, TInt aError )
    {
    _LIT( KMsg, "CallBck HandleSessionError - Error code : %d" );
    TBuf <100> msg;
    msg.Format(KMsg, aError);
    iLog->Log( msg );
    RDebug::Print(msg);
    
    // session event
    TEventIf event( TEventIf::ESetEvent, _L("Session") );
    TestModuleIf().Event( event );
    }   

// -----------------------------------------------------------------------------
// CComposerPluginTest::ActiveWait
// -----------------------------------------------------------------------------
//
TInt CComposerPluginTest::ActiveWait( CStifItemParser& /* aItem */ )
    {
    TTimeIntervalMicroSeconds32 timeout(2500000);
    RTimer timer;
    TRequestStatus status;

    timer.CreateLocal();
    timer.After(status,timeout);

    User::WaitForAnyRequest();
    
    timer.Close();
    
    _LIT( KMsg1, "Exit ActiveWait" );
    iLog->Log( KMsg1 );
    RDebug::Print( KMsg1 );
    
    return KErrNone;
    }

// -----------------------------------------------------------------------------
// CComposerPluginTest::BeginComposerSessionL
// -----------------------------------------------------------------------------
//
TInt CComposerPluginTest::BeginComposerSessionL( CStifItemParser& /* aItem */ )
    {
    iMdEClient = NULL;
    TRAPD( error, iMdEClient = CMdESession::NewL( *this ) );

    _LIT( KMsg, "Exit    BeginComposerSession - Error code : %d" );
    TBuf <100> msg;
    msg.Format(KMsg, error);
    iLog->Log( msg );
    RDebug::Print(msg);
    
    User::LeaveIfError(error);
    
    return KErrNone;
    }    

// -----------------------------------------------------------------------------
// CComposerPluginTest::EndComposerSession
// -----------------------------------------------------------------------------
//
TInt CComposerPluginTest::EndComposerSession( CStifItemParser& /* aItem */ )
	{    
	iPluginArray.ResetAndDestroy();
	iPluginArray.Close();
	    
	if ( iMdEClient )
		{
	    delete iMdEClient;
	    iMdEClient = NULL;
	    }
    
	REComSession::FinalClose();
	
    _LIT( KMsg, "Exit EndComposerSession" );
    iLog->Log( KMsg );  
    RDebug::Print( KMsg );
	
    return KErrNone;
    }

// -----------------------------------------------------------------------------
// CComposerPluginTest::LoadPluginsL
// -----------------------------------------------------------------------------
//
TInt CComposerPluginTest::LoadPluginsL( CStifItemParser& /* aItem */ )
    {
    _LIT( KMsg1, "Enter LoadPlugins" );
    iLog->Log( KMsg1 );
    RDebug::Print( KMsg1 );
    
    TUid imageComposerUID = { 0x20007185 }; // MDS image composer plugin
    
    RImplInfoPtrArray infoArray;
    	
	TCleanupItem cleanupItem( MdsUtils::CleanupEComArray, &infoArray );
	CleanupStack::PushL( cleanupItem );
	
	CComposerPlugin::ListImplementationsL( infoArray );
	TInt count( 0 );
	count = infoArray.Count();
	
	for ( TInt i=0; i < count; i++ )
		{
		TUid uid = infoArray[i]->ImplementationUid();    // Create the plug-ins
		if( uid == imageComposerUID )
		    {
		    iPluginArray.AppendL( CComposerPlugin::NewL( uid ) ); // and add them to array
		    }
		}
		
	CleanupStack::PopAndDestroy( &infoArray ); // infoArray, results in a call to CleanupEComArray
    
	if( iPluginArray.Count() == 0 )
	    {
	    return KErrNotFound;
	    }

	
    _LIT( KMsg2, "Exit LoadPlugins" );
    iLog->Log( KMsg2 );
    RDebug::Print( KMsg2 );
    
    return KErrNone;
    }

// -----------------------------------------------------------------------------
// CComposerPluginTest::SetSessions
// -----------------------------------------------------------------------------
//
TInt CComposerPluginTest::SetSessions( CStifItemParser&  /*aItem*/ )
	{    
    _LIT( KMsg1, "Enter SetSessions" );
    iLog->Log( KMsg1 );
    RDebug::Print( KMsg1 );
    
    if( !iMdEClient )
        {
        return KErrUnknown;
        }
    
	for ( TInt i = 0; i < iPluginArray.Count(); ++i )
		{
		iPluginArray[i]->SetSession( *iMdEClient );
		}
	
    _LIT( KMsg2, "Exit SetSessions" );
    iLog->Log( KMsg2 );
    RDebug::Print( KMsg2 );
	
    return KErrNone;
    }

// -----------------------------------------------------------------------------
// CComposerPluginTest::RemoveSessions
// -----------------------------------------------------------------------------
//
TInt CComposerPluginTest::RemoveSessions( CStifItemParser& /* aItem */ )
	{    
    _LIT( KMsg1, "Enter RemoveSessions" );
    iLog->Log( KMsg1 );
    RDebug::Print( KMsg1 );
	
	for(TInt i = 0; i < iPluginArray.Count(); ++i)
		{
		iPluginArray[i]->RemoveSession();
		}
	
    _LIT( KMsg2, "Exit RemoveSessions" );
    iLog->Log( KMsg2 );
    RDebug::Print( KMsg2 );
	
    return KErrNone;
    }

// -----------------------------------------------------------------------------
// CComposerPluginTest::ModifyObjectsL
// -----------------------------------------------------------------------------
//
TInt CComposerPluginTest::ModifyObjectsL( CStifItemParser&  aItem  )
	{   	
    _LIT( KMsg1, "Enter ModifyObjects" );
    iLog->Log( KMsg1 );
    RDebug::Print( KMsg1 );
	
	CMdENamespaceDef& defaultNamespace = iMdEClient->GetDefaultNamespaceDefL();
	CMdEObjectDef& imageDef = defaultNamespace.GetObjectDefL( Image::KImageObject );
	
	// Mandatory parameters for any object.
	CMdEPropertyDef& creationDef = imageDef.GetPropertyDefL( Object::KCreationDateProperty );
	CMdEPropertyDef& modifiedDef = imageDef.GetPropertyDefL( Object::KLastModifiedDateProperty );
	CMdEPropertyDef& sizeDef = imageDef.GetPropertyDefL( Object::KSizeProperty );
	CMdEPropertyDef& itemTypeDef = imageDef.GetPropertyDefL( Object::KItemTypeProperty );	
	
	CMdEPropertyDef& descriptionPropertyDef = imageDef.GetPropertyDefL( MediaObject::KDescriptionProperty );
	CMdEPropertyDef& widthDef = imageDef.GetPropertyDefL( MediaObject::KWidthProperty ); 
	CMdEPropertyDef& heightDef = imageDef.GetPropertyDefL( MediaObject::KHeightProperty ); 
	CMdEPropertyDef& commentDef = imageDef.GetPropertyDefL( MediaObject::KCommentProperty );
	CMdEPropertyDef& lastModifiedDateDef = imageDef.GetPropertyDefL( Image::KDateTimeProperty );
	CMdEPropertyDef& dateTimeDigitizedDef = imageDef.GetPropertyDefL( Image::KDateTimeDigitizedProperty );
	CMdEPropertyDef& makeDef = imageDef.GetPropertyDefL( Image::KMakeProperty );
	CMdEPropertyDef& modelDef = imageDef.GetPropertyDefL( Image::KModelProperty );
	CMdEPropertyDef& artistDef = imageDef.GetPropertyDefL( MediaObject::KArtistProperty ); // Ei toimi
	CMdEPropertyDef& orientationDef = imageDef.GetPropertyDefL( Image::KOrientationProperty );
	CMdEPropertyDef& yCbCrPosDef = imageDef.GetPropertyDefL( Image::KYCbCrPositioningProperty );
	CMdEPropertyDef& resolutionDef = imageDef.GetPropertyDefL( MediaObject::KResolutionUnitProperty );
	CMdEPropertyDef& isoSpeedDef = imageDef.GetPropertyDefL( Image::KISOSpeedRatingsProperty );
	CMdEPropertyDef& soundFileDef = imageDef.GetPropertyDefL( Image::KRelatedSoundFileProperty );
	CMdEPropertyDef& exposureTimeDef = imageDef.GetPropertyDefL( Image::KExposureTimeProperty );
	CMdEPropertyDef& apertureDef = imageDef.GetPropertyDefL(  Image::KApertureValueProperty );
	CMdEPropertyDef& colourSpaceDef = imageDef.GetPropertyDefL( Image::KColourSpaceProperty );
	CMdEPropertyDef& exposureBiasDef = imageDef.GetPropertyDefL( Image::KExposureBiasValueProperty );
	CMdEPropertyDef& meteringDef = imageDef.GetPropertyDefL( Image::KMeteringModeProperty );
	CMdEPropertyDef& exifVersionDef = imageDef.GetPropertyDefL( Image::KExifVersionProperty );
	CMdEPropertyDef& flashPixVersionDef = imageDef.GetPropertyDefL( Image::KFlashPixVersionProperty );
	CMdEPropertyDef& thumbXDef = imageDef.GetPropertyDefL( Image::KThumbXResolutionProperty );
	CMdEPropertyDef& thumbYDef = imageDef.GetPropertyDefL( Image::KThumbYResolutionProperty );
	CMdEPropertyDef& thumbResolutionUnitDef = imageDef.GetPropertyDefL( Image::KThumbResolutionUnitProperty );
	CMdEPropertyDef& thumbCompressionDef = imageDef.GetPropertyDefL( Image::KThumbCompressionProperty );
	CMdEPropertyDef& shutterSpeedDef = imageDef.GetPropertyDefL( Image::KShutterSpeedValueProperty );
	CMdEPropertyDef& componentsDef = imageDef.GetPropertyDefL( Image::KComponentsConfigurationProperty );
	CMdEPropertyDef& xResolutionDef = imageDef.GetPropertyDefL( Image::KXResolutionProperty );
	CMdEPropertyDef& yResolutionDef = imageDef.GetPropertyDefL( Image::KYResolutionProperty );
	CMdEPropertyDef& fNumberDef =  imageDef.GetPropertyDefL( Image::KFNumberProperty );
	CMdEPropertyDef& focalLengthDef = imageDef.GetPropertyDefL( Image::KFocalLengthProperty );
	CMdEPropertyDef& focalIn35Def = imageDef.GetPropertyDefL( Image::KFocalLengthIn35mmFilmProperty );
    
	TBuf <100> msg;

	TInt count( 0 );
	
	User::LeaveIfError( aItem.GetNextInt( count ) );
	
	if ( count == 1 )
		{	
		TItemId objectId(0);
		TTime currTime;
		currTime.HomeTime();
		TPtrC inputFile;
		    	
		User::LeaveIfError( aItem.GetNextString( inputFile ));
		
		// create object
		iMdEClient->RemoveObjectL(inputFile);
		CMdEObject* object = iMdEClient->NewObjectLC(imageDef, inputFile);
		
		object->AddTimePropertyL(creationDef, currTime);
		object->AddTimePropertyL(modifiedDef, currTime);
		object->AddUint32PropertyL( sizeDef, 0 );
		object->AddTextPropertyL( itemTypeDef, Image::KImageObject );
		
		objectId = iMdEClient->AddObjectL(*object);
		
		CleanupStack::PopAndDestroy( object );
		object = NULL;
		
		// open object for modification
		TRAPD( openError, object = iMdEClient->OpenObjectL(objectId) );
			
		if ( !object || openError != KErrNone )
			{
		    msg.Format(KOpenErr, openError);
		    iLog->Log( msg );
		    RDebug::Print(msg);
		    
		    User::LeaveIfError(openError);
			}

		CleanupStack::PushL( object );

   		CMdEProperty* property = NULL;
   		
   		
   		// Modified date
   		object->Property( lastModifiedDateDef, property );
   		
   		if ( property )
   			{
   			CMdETimeProperty* modTimeProperty = (CMdETimeProperty*)property;
   			
   			modTimeProperty->SetValueL( 1 );
   			}
   		else
   			{
   			object->AddTimePropertyL( lastModifiedDateDef, 1 );
   			}
   			
   		property = NULL;		
   		
   		// Date & time digitized
   		object->Property( dateTimeDigitizedDef, property );
   			
		if ( property )
			{
			CMdETimeProperty* digiTimeProperty = (CMdETimeProperty*)property;
			
			digiTimeProperty->SetValueL( 9959267210000000 );
			}
		else
			{
			object->AddTimePropertyL( dateTimeDigitizedDef, 9959267210000000 );
			}
   		
   		property = NULL;
   		
   		// Title
		_LIT( KDesc, "Description" );
		object->Property( descriptionPropertyDef, property );

		if ( property )
			{					
			property->SetTextValueL( KDesc );
			}
		else
			{
			object->AddTextPropertyL( descriptionPropertyDef, KDesc );
			}
  		
   		property = NULL;
   			
		// Comment
        _LIT( KNoComment, "Comment" );
		object->Property( commentDef, property );
		
		if ( property )
			{					
			property->SetTextValueL( KNoComment );
			}
		else
			{
			object->AddTextPropertyL( commentDef, KNoComment );
			}
		
		property = NULL;
		
		// Maker
		object->Property( makeDef, property );
		
        _LIT( KOnkia, "Maker" );
		if ( property )
			{
			property->SetTextValueL( KOnkia );
			}
		else
			{
			object->AddTextPropertyL( makeDef, KOnkia );
			}
			
		property = NULL;
			
		// Model
		object->Property( modelDef, property );
        _LIT( KModel, "Model" );
		if ( property )
			{					
			property->SetTextValueL( KModel );
			}
		else
			{
			object->AddTextPropertyL( modelDef, KModel );
			}
			
		property = NULL;
			
		// Width
		object->Property( widthDef, property );
		
		if ( property )
			{
			property->SetUint16ValueL( 100 );
			}
		else
			{
			object->AddUint16PropertyL( widthDef, 100 );
			}
		
		property = NULL;
			
		// Height
		object->Property( heightDef, property );
		
		if ( property )
			{
			property->SetUint16ValueL( 100 );
			}
		else
			{
			object->AddUint16PropertyL( heightDef, 100 );
			}
			
		property = NULL;
			
		// Orientation
		object->Property( orientationDef, property );
		
		if ( property )
			{
			property->SetUint16ValueL( 8 );
			}
		else
			{
			object->AddUint16PropertyL( orientationDef, 8 );
			}
			
		property = NULL;
			
		// YCbCr positioning
		object->Property( yCbCrPosDef, property );
		
		if ( property )
			{
			property->SetUint16ValueL( 2 );
			}
		else
			{
			object->AddUint16PropertyL( yCbCrPosDef, 2 );
			}
			
		property = NULL;
			
		// ISO speed
		object->Property( isoSpeedDef, property );
		
		if ( property )
			{
			property->SetUint16ValueL( 1600 );
			}
		else
			{
			object->AddUint16PropertyL( isoSpeedDef, 1600 );
			}
			
		property = NULL;
		
		// Related soundfile
		_LIT(KWavName, "Wav.wav");
		object->Property( soundFileDef, property);
		
		if(property)
			{					
			property->SetTextValueL( KWavName );
			}
		else
			{
			object->AddTextPropertyL( soundFileDef, KWavName );
			}
		
		property = NULL;
		
		// Exposure time
		object->Property( exposureTimeDef, property);
		
		if(property)
			{					
			property->SetReal32ValueL( 99.1f );
			}
		else
			{
			object->AddReal32PropertyL( exposureTimeDef, 99.1f );
			}
			
		property = NULL;
		
		// Aperture
		object->Property( apertureDef, property);
		
		if(property)
			{					
			property->SetReal32ValueL( 5.0f );
			}
		else
			{
			object->AddReal32PropertyL( apertureDef, 5.0f );
			}
			
		property = NULL;
			
		// Colour space
		object->Property( colourSpaceDef, property);
		
		if (property)
			{
			property->SetUint16ValueL( 65535 );
			}
		else
			{
			object->AddUint16PropertyL( colourSpaceDef, 65535 );
			}
			
		property = NULL;
			
		// Exposure bias
		object->Property( exposureBiasDef, property);
		
		if(property)
			{					
			property->SetReal32ValueL( 0.0f );
			}
		else
			{
			object->AddReal32PropertyL( exposureBiasDef, 0.0f );
			}
			
		property = NULL;
			
		// Metering mode
		object->Property( meteringDef, property);
		
		if (property)
			{
			property->SetUint16ValueL( 4 );
			}
		else
			{
			object->AddUint16PropertyL( meteringDef, 4 );
			}
			
		property = NULL;
		
		// Thumb X
		object->Property( thumbXDef, property);
		
		if(property)
			{					
			property->SetUint32ValueL( 64 );
			}
		else
			{
			object->AddUint32PropertyL( thumbXDef, 64 );
			}
			
		property = NULL;
		
		// Thumb Y
		object->Property( thumbYDef, property);
		
		if(property)
			{					
			property->SetUint32ValueL( 64 );
			}
		else
			{
			object->AddUint32PropertyL( thumbYDef, 64 );
			}
			
		property = NULL;
			
		// Thumbnail resolution unit
		object->Property( thumbResolutionUnitDef, property );
		
		if ( property )
			{
			property->SetUint16ValueL( 2 );
			}
		else
			{
			object->AddUint16PropertyL( thumbResolutionUnitDef, 2 );
			}
			
		property = NULL;
			
		// Thumbnail compression
		object->Property( thumbCompressionDef, property );
		
		if ( property )
			{
			property->SetUint16ValueL( 6 );
			}
		else
			{
			object->AddUint16PropertyL( thumbCompressionDef, 6 );
			}
			
		property = NULL;
			
		// Shutter speed
		object->Property( shutterSpeedDef, property );
		
		if ( property )
			{					
			property->SetReal32ValueL( 9.0f );
			}
		else
			{
			object->AddReal32PropertyL( shutterSpeedDef, 9.0f );
			}
			
		property = NULL;
			
		// Components configuration
		object->Property( componentsDef, property );
		
		if ( property )
			{					
			property->SetUint32ValueL( 197121 );
			}
		else
			{
			object->AddUint32PropertyL( componentsDef, 197121 );
			}
			
		property = NULL;
			
		// X resolution
		object->Property( xResolutionDef, property );
		
		if ( property )
			{					
			property->SetReal32ValueL( 300.0f );
			}
		else
			{
			object->AddReal32PropertyL( xResolutionDef, 300.0f );
			}
			
		property = NULL;
			
		// Y resolution
		object->Property( yResolutionDef, property );
		
		if ( property )
			{					
			property->SetReal32ValueL( 300.0f );
			}
		else
			{
			object->AddReal32PropertyL( yResolutionDef, 300.0f );
			}
			
		property = NULL;
		
		// F number
		object->Property( fNumberDef, property );
		
		if ( property )
			{					
			property->SetReal32ValueL( 2.8f );
			}
		else
			{
			object->AddReal32PropertyL( fNumberDef, 2.8f );
			}
			
		property = NULL;
			
		// Focal length
		object->Property( focalLengthDef, property );
		
		if ( property )
			{					
			property->SetReal32ValueL( 9.0f );
			}
		else
			{
			object->AddReal32PropertyL( focalLengthDef, 9.0f );
			}
			
		property = NULL;
		
		// Focal length in 35 mm film
		object->Property( focalIn35Def, property );
		
		if ( property )
			{
			property->SetUint16ValueL( 120 );
			}
		else
			{
			object->AddUint16PropertyL( focalIn35Def, 120 );
			}
						
		// Commit object
		TRAPD( commitError, iMdEClient->CommitObjectL(*object) );
		if ( commitError != KErrNone )
			{		
		    msg.Format(KCommErr, commitError);
		    iLog->Log( msg );
		    RDebug::Print(msg);
		    
		    User::LeaveIfError(commitError);
			}
		
		iLog->Log( KCommit );
		RDebug::Print(KCommit);
		
		CleanupStack::PopAndDestroy( object );
		}
	
	// modify more than one object
	else if ( count > 1 )
		{
		RPointerArray<CMdEObject> objectArray;
		CleanupClosePushL( objectArray );

		CMdEObject* object = NULL;
		TItemId objectId(0);
		TTime currTime;
	    TBuf <100> uri;
		
		for ( TInt i = 0; i < count; ++i )
			{			
		    uri.Format(KUri, i+1);
			
			// create object
		    iMdEClient->RemoveObjectL(uri);
			object = iMdEClient->NewObjectLC(imageDef, uri);
			
			currTime.HomeTime();
			object->AddTimePropertyL(creationDef, currTime);
			object->AddTimePropertyL(modifiedDef, currTime);
			object->AddUint32PropertyL( sizeDef, 0 );
			object->AddTextPropertyL( itemTypeDef, Image::KImageObject );
			
			objectId = iMdEClient->AddObjectL(*object);
			
			CleanupStack::PopAndDestroy(object);
			object = NULL;
			
			// open for modification
			TRAPD( openError, object = iMdEClient->OpenObjectL(objectId) )
			
			if ( !object || openError != KErrNone )
				{
				
				if (openError != KErrNone)
					{
				    msg.Format(KOpenErr, openError);
				    iLog->Log( msg );
				    RDebug::Print(msg);
				    
				    User::LeaveIfError(openError);
					}
				else 
					{
				    iLog->Log( KNull );
				    RDebug::Print(KNull);
					}

			    continue;
				}

	   		CMdEProperty* property = NULL;
	   		
	   		// Description
	   		_LIT( KDesc, "Description" );
			object->Property( descriptionPropertyDef, property );
			
			if ( property )
				{					
				property->SetTextValueL( KDesc );
				}
			else
				{
				object->AddTextPropertyL( descriptionPropertyDef, KDesc );
				}
			
			property = NULL;
			
			// Width
			object->Property( widthDef, property );
			
			if ( property )
				{
				property->SetUint16ValueL( 100 );
				}
			else
				{
				object->AddUint16PropertyL( widthDef, 100 );
				}
			
			property = NULL;
				
			// Height
			object->Property( heightDef, property );
			
			if ( property )
				{
				property->SetUint16ValueL( 100 );
				}
			else
				{
				object->AddUint16PropertyL( heightDef, 100 );
				}
			
			property = NULL;
			
			// Maker
			object->Property( makeDef, property );
			
	        _LIT( KOnkia, "Maker" );
			if ( property )
				{
				property->SetTextValueL( KOnkia );
				}
			else
				{
				object->AddTextPropertyL( makeDef, KOnkia );
				}
				
			property = NULL;
				
			// Model
			object->Property( modelDef, property );
	        _LIT( KModel, "Model" );
			if ( property )
				{					
				property->SetTextValueL( KModel );
				}
			else
				{
				object->AddTextPropertyL( modelDef, KModel );
				}

			objectArray.Append( object );
			}

		TRAPD( commitError, iMdEClient->CommitObjectsL(objectArray) );
		if ( commitError != KErrNone )
			{			
		    msg.Format(KCommErr, commitError);
		    iLog->Log( msg );
		    RDebug::Print(msg);
		    
		    User::LeaveIfError(commitError);
			}
			
		iLog->Log( KCommit );
		RDebug::Print(KCommit);
		
		objectArray.ResetAndDestroy();
		CleanupStack::PopAndDestroy( &objectArray );
		}
	
    _LIT( KMsg2, "Exit ModifyObjects" );
    iLog->Log( KMsg2 );
    RDebug::Print( KMsg2 );
	
    return KErrNone;
    }

// -----------------------------------------------------------------------------
// CComposerPluginTest::DeleteObject
// -----------------------------------------------------------------------------
//
TInt CComposerPluginTest::DeleteObjectL( CStifItemParser& aItem )
	{  
    _LIT( KMsg1, "Enter DeleteObject" );
    iLog->Log( KMsg1 );
    RDebug::Print( KMsg1 );
	
	TPtrC inputFile;
	User::LeaveIfError( aItem.GetNextString( inputFile ));
	
	iMdEClient->RemoveObjectL(inputFile);
	
    _LIT( KMsg2, "Exit DeleteObject" );
    iLog->Log( KMsg2 );
    RDebug::Print( KMsg2 );
	
    return KErrNone;
	}

// -----------------------------------------------------------------------------
// CComposerPluginTest::DeleteObjects
// -----------------------------------------------------------------------------
//
TInt CComposerPluginTest::DeleteObjectsL( CStifItemParser& aItem )
	{  
	TInt count(0);
    TBuf <100> uri;
	
	User::LeaveIfError( aItem.GetNextInt( count ) );
	
	for ( TInt i = 0; i < count; ++i )
		{			
	    uri.Format(KUri, i+1);
	    
	    iMdEClient->RemoveObjectL(uri);
		}
	
    _LIT( KMsg, "Exit    DeleteObjects" );
    iLog->Log( KMsg );
    RDebug::Print(KMsg);
	
    return KErrNone;
	}

// -----------------------------------------------------------------------------
// CComposerPluginTest::WaitComposingComplete
// -----------------------------------------------------------------------------
//
TInt CComposerPluginTest::WaitComposingComplete( CStifItemParser& aItem )
	{    
    _LIT( KMsg1, "Enter WaitComposingComplete" );
    iLog->Log( KMsg1 );
    RDebug::Print( KMsg1 );
    
	TBool complete = ETrue;
	TBool waitingForComposingToEnd = ETrue;
	
	while( waitingForComposingToEnd )
		{
		complete = ETrue;
		
		for(TInt i = 0; i < iPluginArray.Count(); ++i)
			{
			if( iPluginArray[i]->IsComposingComplete() == EFalse )
				{
				complete = EFalse;
				}
			}
	
		if( complete )
			{
			waitingForComposingToEnd = EFalse;
			}
		else 
			{
			ActiveWait( aItem );
			}
		}
	
    _LIT( KMsg2, "Exit WaitComposingComplete" );
    iLog->Log( KMsg2 );
    RDebug::Print( KMsg2 );
	
    return KErrNone;
    }

//  [End of File] - Do not remove
