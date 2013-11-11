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
* Description:  Harvests metadata from wm video file 
*
*/

#include <e32std.h>
#include <caf/caf.h>
#include <pathinfo.h>

#include "mdsutils.h"
#include "harvesterdata.h"
#include "harvesterlog.h"
#include "harvestercommon.h"
#include "harvesterwmvplugin.h"
#include "mdscommoninternal.h"
#include <mdenamespacedef.h>
#include <mdeobjectdef.h>
#include "mdeobject.h"
#include "mdetextproperty.h"
#include "mdeobjectwrapper.h"

CHarvesterWmvPluginPropertyDefs::CHarvesterWmvPluginPropertyDefs() : CBase(),
    iCreationDatePropertyDef( NULL )
	{
	}

void CHarvesterWmvPluginPropertyDefs::ConstructL( CMdEObjectDef& aObjectDef )
    {
    SetByObjectDefL( aObjectDef );
    }

CHarvesterWmvPluginPropertyDefs* CHarvesterWmvPluginPropertyDefs::NewL()
    {
    CHarvesterWmvPluginPropertyDefs* self = 
        new (ELeave) CHarvesterWmvPluginPropertyDefs();
    return self;
    }

void CHarvesterWmvPluginPropertyDefs::SetByObjectDefL( CMdEObjectDef& aObjectDef )
	{
	CMdENamespaceDef& nsDef = aObjectDef.NamespaceDef();
	
	// Common property definitions
	CMdEObjectDef& objectDef = nsDef.GetObjectDefL( MdeConstants::Object::KBaseObject );
	iCreationDatePropertyDef = &objectDef.GetPropertyDefL( MdeConstants::Object::KCreationDateProperty );
	iLastModifiedDatePropertyDef = &objectDef.GetPropertyDefL( MdeConstants::Object::KLastModifiedDateProperty );
	iSizePropertyDef = &objectDef.GetPropertyDefL( MdeConstants::Object::KSizeProperty );
	iItemTypePropertyDef = &objectDef.GetPropertyDefL( MdeConstants::Object::KItemTypeProperty );
    iTitlePropertyDef = &objectDef.GetPropertyDefL( MdeConstants::Object::KTitleProperty );
    iDefaultFolderPropertyDef = &objectDef.GetPropertyDefL( MdeConstants::Object::KInDefaultFolder );
	
    CMdEObjectDef& mediaDef = nsDef.GetObjectDefL( MdeConstants::MediaObject::KMediaObject );
    iDrmPropertyDef = &mediaDef.GetPropertyDefL( MdeConstants::MediaObject::KDRMProperty );
    iDescriptionPropertyDef = &mediaDef.GetPropertyDefL( MdeConstants::MediaObject::KDescriptionProperty );
    iAuthorPropertyDef = &mediaDef.GetPropertyDefL( MdeConstants::MediaObject::KAuthorProperty );
    iGenrePropertyDef = &mediaDef.GetPropertyDefL( MdeConstants::MediaObject::KGenreProperty );
	}

// ======== MEMBER FUNCTIONS ========

// ---------------------------------------------------------------------------
// Constructor
// ---------------------------------------------------------------------------
//
CHarvesterWMVPlugin* CHarvesterWMVPlugin::NewL()
    {
    CHarvesterWMVPlugin* self = new(ELeave) CHarvesterWMVPlugin();
    CleanupStack::PushL(self);
    self->ConstructL();
    CleanupStack::Pop(self);

    return self;
    }

// ---------------------------------------------------------------------------
// Destructor
// ---------------------------------------------------------------------------
//
CHarvesterWMVPlugin::~CHarvesterWMVPlugin()
    {
    WRITELOG( "CHarvesterWMVPlugin::~CHarvesterWMVPlugin()" );
    delete iPropDefs;
    iPropDefs = NULL;
    
    delete iPhoneVideosPath;
    iPhoneVideosPath = NULL;
    delete iMmcVideosPath;
    iMmcVideosPath = NULL;
    }

// ---------------------------------------------------------------------------
// Harvest file
// ---------------------------------------------------------------------------
//
void CHarvesterWMVPlugin::HarvestL( CHarvesterData* aHD )    
    {
    WRITELOG( "CHarvesterWMVPlugin::Harvest()" );
    CMdEObject& mdeObject = aHD->MdeObject();
    
    CHarvesterWmvClipDetails* clipDetails = CHarvesterWmvClipDetails::NewL();
    CleanupStack::PushL( clipDetails );
    
    TRAPD( error, GatherDataL( mdeObject, *clipDetails ) );
    if ( error == KErrNone || error == KErrCompletion )
    	{
    	TBool isNewObject( mdeObject.Id() == 0 );
        
        if ( isNewObject || mdeObject.Placeholder() )
            {
            TRAP( error, HandleObjectPropertiesL( *aHD, *clipDetails, ETrue ) );
            mdeObject.SetPlaceholder( EFalse );
            }
        else
            {
            TRAP( error, HandleObjectPropertiesL( *aHD, *clipDetails, EFalse ) );
            }

        if ( error != KErrNone )
            {
            WRITELOG1( "CHarvesterWMVPlugin::HarvestL() - Handling object failed: ", error );
            }
    	}
    else
        {
        TInt convertedError = KErrNone;
        MdsUtils::ConvertTrapError( error, convertedError );
        aHD->SetErrorCode( convertedError );
        }
    
    CleanupStack::PopAndDestroy( clipDetails );
    }

// ---------------------------------------------------------------------------
// CHarvesterWMVPlugin::GetMimeType (from CHarvesterPlugin)
// ---------------------------------------------------------------------------
//    
void CHarvesterWMVPlugin::GetMimeType( const TDesC& aUri, TDes& aMimeType )
    {
    aMimeType.Zero();
    
    ContentAccess::CContent* content = NULL;
    
    TRAPD( err, content = ContentAccess::CContent::NewL( aUri ) );
    if (err == KErrNone) 
        {
        err = content->GetStringAttribute( ContentAccess::EMimeType, aMimeType );
        delete content;
        content = NULL;
        }
    }

// ---------------------------------------------------------------------------
// Default constructor
// ---------------------------------------------------------------------------
//
CHarvesterWMVPlugin::CHarvesterWMVPlugin() : CHarvesterPlugin(), iPropDefs( NULL )
    {
    }

// ---------------------------------------------------------------------------
// 2nd phase constructor
// ---------------------------------------------------------------------------
//
void CHarvesterWMVPlugin::ConstructL()
    {
    WRITELOG( "CHarvesterWMVPlugin::ConstructL()" );
    
    iPropDefs = CHarvesterWmvPluginPropertyDefs::NewL();
    
    TFileName videos = PathInfo::VideosPath();
    
    TFileName phonePath = PathInfo::PhoneMemoryRootPath();
    phonePath.Append( videos );
    iPhoneVideosPath = phonePath.AllocL();

    TFileName mmcPath = PathInfo::MemoryCardRootPath();
    mmcPath.Append( videos );
    iMmcVideosPath = mmcPath.Right( mmcPath.Length() - 1 ).AllocL();
   
    SetPriority( KHarvesterPriorityHarvestingPlugin - 1 );
    }

// ---------------------------------------------------------------------------
// GatherDataL
// ---------------------------------------------------------------------------
//
void CHarvesterWMVPlugin::GatherDataL( CMdEObject& aMetadataObject, CHarvesterWmvClipDetails& aClipDetails )
    {
    WRITELOG( "CHarvesterWMVPlugin::GatherDataL()" );
       
    const TDesC& uri = aMetadataObject.Uri();

    TEntry* entry = new (ELeave) TEntry();
    CleanupStack::PushL( entry );

    User::LeaveIfError( iFs.Entry( uri, *entry ) );

    aClipDetails.iModifiedDate = entry->iModified;
    aClipDetails.iFileSize = (TUint)entry->iSize;
    
    CleanupStack::PopAndDestroy( entry );
    
    ContentAccess::CContent* content = ContentAccess::CContent::NewLC( uri );
    ContentAccess::CData* data = content->OpenContentLC( ContentAccess::EPeek );
   
    ContentAccess::RStringAttributeSet attrSet;
    CleanupClosePushL( attrSet );
    
    attrSet.AddL( ContentAccess::EDescription );
    attrSet.AddL( ContentAccess::EMimeType );
    attrSet.AddL( ContentAccess::ETitle );
    attrSet.AddL( ContentAccess::EAuthor );
    attrSet.AddL( ContentAccess::EGenre );

    User::LeaveIfError( data->GetStringAttributeSet(attrSet) );

    TInt err = attrSet.GetValue( ContentAccess::EMimeType, aClipDetails.iMimeType );
    if ( err != KErrNone)
        {
        WRITELOG1( "CHarvesterWMVPlugin::GatherDataL - ERROR: getting mimetype failed %d", err );
        }
        
    if ( aClipDetails.iMimeType.Length() <= 0 )
        {
        WRITELOG( "CHarvesterWMVPlugin::GatherDataL - no mimetype" );
        }
    
    err = attrSet.GetValue( ContentAccess::EDescription, aClipDetails.iDescription );
    if ( err != KErrNone)
        {
        WRITELOG1( "CHarvesterWMVPlugin::GatherDataL - ERROR: getting description failed %d", err );
        }
        
    if ( aClipDetails.iDescription.Length() <= 0 )
        {
        WRITELOG( "CHarvesterWMVPlugin::GatherDataL - no description" );
        }
    
    err = attrSet.GetValue( ContentAccess::ETitle, aClipDetails.iTitle );
    if ( err != KErrNone)
        {
        WRITELOG1( "CHarvesterWMVPlugin::GatherDataL - ERROR: getting title failed %d", err );
        }
        
    if ( aClipDetails.iTitle.Length() <= 0 )
        {
        WRITELOG( "CHarvesterWMVPlugin::GatherDataL - no title" );
        }
    
    err = attrSet.GetValue( ContentAccess::EAuthor, aClipDetails.iAuthor );
    if ( err != KErrNone)
        {
        WRITELOG1( "CHarvesterWMVPlugin::GatherDataL - ERROR: getting author failed %d", err );
        }
        
    if ( aClipDetails.iAuthor.Length() <= 0 )
        {
        WRITELOG( "CHarvesterWMVPlugin::GatherDataL - no author" );
        }

    err = attrSet.GetValue( ContentAccess::EGenre, aClipDetails.iGenre );
    if ( err != KErrNone)
        {
        WRITELOG1( "CHarvesterWMVPlugin::GatherDataL - ERROR: getting genre failed %d", err );
        }
        
    if ( aClipDetails.iGenre.Length() <= 0 )
        {
        WRITELOG( "CHarvesterWMVPlugin::GatherDataL - no genre" );
        }
    
    err = content->GetAttribute( ContentAccess::EIsProtected, aClipDetails.iDrmProtected );
    if ( err != KErrNone)
        {
        WRITELOG1( "CHarvesterWMVPlugin::GatherDataL - ERROR: getting protection info failed %d", err );
        }
        
    CleanupStack::PopAndDestroy( 3 ); // content, data, attrSet
    }

// ---------------------------------------------------------------------------
// Handle object properties
// ---------------------------------------------------------------------------
//
void CHarvesterWMVPlugin::HandleObjectPropertiesL( 
    CHarvesterData& aHD,
    CHarvesterWmvClipDetails& aClipDetails,
    TBool aIsAdd )
    {
    WRITELOG( "CHarvesterWMVPlugin::HandleObjectPropertiesL()" );
    
    CMdEObject& mdeObject = aHD.MdeObject();
    
    InitPropDefsL( mdeObject.Def() );
    
    if( ! mdeObject.Placeholder() )
    	{
        // Creation date
    	TTimeIntervalSeconds timeOffset = User::UTCOffset();
    	TTime localModifiedDate = aClipDetails.iModifiedDate + timeOffset;
    	CMdeObjectWrapper::HandleObjectPropertyL(mdeObject, 
    			*iPropDefs->iCreationDatePropertyDef, &localModifiedDate, aIsAdd );
    	// Last modified date
    	CMdeObjectWrapper::HandleObjectPropertyL(mdeObject, 
    			*iPropDefs->iLastModifiedDatePropertyDef, &aClipDetails.iModifiedDate, aIsAdd );
    	// File size
    	CMdeObjectWrapper::HandleObjectPropertyL(mdeObject, 
    			*iPropDefs->iSizePropertyDef, &aClipDetails.iFileSize, aIsAdd );
        // Mime Type
        CMdeObjectWrapper::HandleObjectPropertyL(mdeObject, 
                *iPropDefs->iItemTypePropertyDef, &aClipDetails.iMimeType, aIsAdd );

        const TDesC& uri = mdeObject.Uri();
        if( uri.FindF( iMmcVideosPath->Des()) != KErrNotFound ||
            uri.FindF( iPhoneVideosPath->Des()) != KErrNotFound ||
            uri.FindF( KDCIMFolder ) != KErrNotFound )
            {
            TBool inDefaultFolder( ETrue );
            CMdeObjectWrapper::HandleObjectPropertyL(mdeObject, *iPropDefs->iDefaultFolderPropertyDef, &inDefaultFolder, aIsAdd );
            }
        else
            {
            TBool inDefaultFolder( EFalse );
            CMdeObjectWrapper::HandleObjectPropertyL(mdeObject, *iPropDefs->iDefaultFolderPropertyDef, &inDefaultFolder, aIsAdd );    
            }
    	}
    
    // DRM protection
    if( aClipDetails.iDrmProtected )
        {
        CMdeObjectWrapper::HandleObjectPropertyL(mdeObject, 
                *iPropDefs->iDrmPropertyDef, &aClipDetails.iDrmProtected, aIsAdd );
        } 
    // Title (is set from URI by default)
    if( aClipDetails.iTitle.Length() > 0 && aClipDetails.iTitle.Length() < KMaxTitleFieldLength )
        {
        CMdeObjectWrapper::HandleObjectPropertyL(mdeObject, 
                *iPropDefs->iTitlePropertyDef, &aClipDetails.iTitle, EFalse );
        }
    // Description
    if( aClipDetails.iDescription.Length() > 0 && aClipDetails.iDescription.Length() < iMaxTextLength )
        {
        CMdeObjectWrapper::HandleObjectPropertyL(mdeObject, 
                *iPropDefs->iDescriptionPropertyDef, &aClipDetails.iDescription, aIsAdd );
        }   
    // Author
    if( aClipDetails.iAuthor.Length() > 0 && aClipDetails.iAuthor.Length() < iMaxTextLength )
        {
        CMdeObjectWrapper::HandleObjectPropertyL(mdeObject, 
                *iPropDefs->iAuthorPropertyDef, &aClipDetails.iAuthor, aIsAdd );
        }
    // Genre
    if( aClipDetails.iGenre.Length() > 0 && aClipDetails.iGenre.Length() < iMaxTextLength )
        {
        CMdeObjectWrapper::HandleObjectPropertyL(mdeObject, 
                *iPropDefs->iGenrePropertyDef, &aClipDetails.iGenre, aIsAdd );
        }   
    }

void CHarvesterWMVPlugin::InitPropDefsL(CMdEObjectDef& aObjectDef)
    {
    if( !iPropDefs->iCreationDatePropertyDef )
        {
        iPropDefs->SetByObjectDefL( aObjectDef );
        // Prefetch max text lengt for validity checking
        iMaxTextLength = iPropDefs->iGenrePropertyDef->MaxTextLengthL();
        }
    }

