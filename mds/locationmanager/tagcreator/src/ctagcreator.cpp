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
* Description:  A class for creating country and city tags for media files
*
*/

#include "ctagcreator.h"
#include "locationmanagerdebug.h"

#include "mdccommon.h"
#include "mdeobjectdef.h"
#include "mdepropertydef.h"
#include "mderelation.h"
#include "mdequery.h"
#include "mdeconstants.h"

using namespace MdeConstants;

_LIT ( KCountry, "country:");   // country:finland
_LIT ( KCity, "city:");         // city:helsinki|country:finland
_LIT ( KSeparator, "|" );       // Separator for city tag. Separate city and country.
_LIT ( KUnMappedLocation, " " ); //a blank title for geocoordinates that could not be mapped to a valid location

// --------------------------------------------------------------------------
// CTagCreator::NewL
// --------------------------------------------------------------------------
//
EXPORT_C CTagCreator* CTagCreator::NewL( )
    {
    LOG( "CTagCreator::NewL(), begin" );
    CTagCreator* self = new (ELeave) CTagCreator( );
    CleanupStack::PushL( self );
    self->ConstructL();
    CleanupStack::Pop( self );
    LOG( "CTagCreator::NewL(), end" );
    return self;
    }
        
// --------------------------------------------------------------------------
// CTagCreator::CTagCreator
// --------------------------------------------------------------------------
//  
CTagCreator::CTagCreator(  ) 
    {

    }

// --------------------------------------------------------------------------
// CTagCreator::ConstructL
// --------------------------------------------------------------------------
//    
void CTagCreator::ConstructL()
    {
    
    }
    
// --------------------------------------------------------------------------
// CTagCreator::~CTagCreator
// --------------------------------------------------------------------------
//    
CTagCreator::~CTagCreator()
    {
  		
    }

// --------------------------------------------------------------------------
// CTagCreator::SetMdeSession()
// --------------------------------------------------------------------------
//
EXPORT_C void CTagCreator::SetSession(CMdESession* aSession)
    {
    iMdeSession = aSession;
    }

// ---------------------------------------------------------------------------
// CTagCreator::CreateTagForCountryL() 
// ---------------------------------------------------------------------------
//
void CTagCreator::CreateTagForCountryL( const TPtrC& aCountry, 
                        TItemId& aCountryTagId )
    {
    LOG( "CTagCreator::CreateTagForCountryL - begin" );

        // Crete new string for uri, -> country:India.
        TUint totalLength = ( aCountry.Length() + KCountry().Length() );
        HBufC* buf = HBufC::NewLC( totalLength );
        TPtr uriPtr = buf->Des();
        _LIT(KCountryUriFormat, "%S%S");
        uriPtr.Format(KCountryUriFormat, &(KCountry()), &aCountry); 
        
        // Check if there is allready tag for current country.
        aCountryTagId = TagExistsL( uriPtr );
           
        if( !aCountryTagId )
           {
            // No tag, create tag for country.
            if(aCountry.Length()> 0)
                {
                	//Valid country name
                aCountryTagId = CreateTagL( aCountry, uriPtr, MdeConstants::Tag::ESystemDefineCountryTags);
                }
            else
                {
                	//No valid country name. So create a tag of type EUnMappedLocationTags
                aCountryTagId = CreateTagL( aCountry, uriPtr, MdeConstants::Tag::EUnMappedLocationTags);
                }
           }  
          
        CleanupStack::PopAndDestroy( buf );


    LOG( "CTagCreator::CreateTagForCountryL - end" );
    }

// ---------------------------------------------------------------------------
// CTagCreator::CreateTagForCityL() 
// ---------------------------------------------------------------------------
//
void CTagCreator::CreateTagForCityL( const TPtrC& aCountry, const TPtrC& aCity, TItemId& aCityTagId )
    {
    LOG( "CTagCreator::CreateTagForCityL - begin" );
        // Crete new string for uri, -> city:bangalore|country:india
        TUint totalLength = ( aCity.Length() + KCity.iTypeLength +
            aCountry.Length() + KCountry.iTypeLength + KSeparator.iTypeLength );
        
        HBufC* buf = HBufC::NewLC( totalLength );
        TPtr uriPtr = buf->Des();
        _LIT(KCityCountryUriFormat, "%S%S%S%S%S");
        uriPtr.Format(KCityCountryUriFormat,
                        &(KCity()),
                        &aCity,
                        &(KSeparator()),
                        &(KCountry()),
                        &aCountry);
            
        // Check if there is allready tag for current city.
        aCityTagId = TagExistsL( uriPtr );
        
        if( !aCityTagId )
            {
               // No tag, create tag for city.
            if(aCity.Length() > 0)
                {
                	//Valid city name
                aCityTagId = CreateTagL( aCity, uriPtr, MdeConstants::Tag::ESystemDefineCityTags);
                }
            else
                {  
                	//No valid city name. So create a tag of type EUnMappedLocationTags
                aCityTagId = CreateTagL( aCity, uriPtr, MdeConstants::Tag::EUnMappedLocationTags);
                }
            }
        
        CleanupStack::PopAndDestroy( buf );
 
    LOG( "CTagCreator::CreateTagForCityL - end" );
    }

// ---------------------------------------------------------------------------
// CTagCreator::CreateTagL 
// ---------------------------------------------------------------------------
//
TUint32 CTagCreator::CreateTagL( const TPtrC& aTitle, 
        const TPtrC& aUri,
        MdeConstants::Tag::TTagType aTagType)
    {
    LOG( "CTagCreator::CreateTagL - begin" );

    CMdENamespaceDef& defaultNamespace = iMdeSession->GetDefaultNamespaceDefL();
    // Create tag object.
    CMdEObjectDef& tagObjectDef = defaultNamespace.GetObjectDefL( Tag::KTagObject );

	CMdEObject* tagObject = NULL;
    tagObject = iMdeSession->NewObjectLC( tagObjectDef, aUri );
	
	// Type defs.
	CMdEPropertyDef& creationDef = tagObjectDef.GetPropertyDefL(
		Object::KCreationDateProperty );
	CMdEPropertyDef& modifiedDef = tagObjectDef.GetPropertyDefL(
		Object::KLastModifiedDateProperty );
	CMdEPropertyDef& sizeDef = tagObjectDef.GetPropertyDefL(
		Object::KSizeProperty );
	CMdEPropertyDef& itemTypeDef = tagObjectDef.GetPropertyDefL(
		Object::KItemTypeProperty );
	CMdEPropertyDef& titleDef = tagObjectDef.GetPropertyDefL(
		Object::KTitleProperty );
	
	// Set propertys.
	TTime universalTime;
    universalTime.UniversalTime(); 

    // Creation time.
	tagObject->AddTimePropertyL( creationDef, universalTime );   

    // Modification time.
	tagObject->AddTimePropertyL( modifiedDef, universalTime );

    // Size 
	tagObject->AddUint32PropertyL( sizeDef, aTagType); 
	
	// ItemType.
	tagObject->AddTextPropertyL( itemTypeDef, Tag::KTagItemType );
	
	// Title, for country.
	if ( aTitle.Length() > 0 )
	    {
	    tagObject->AddTextPropertyL( titleDef, aTitle ); // Add title
	    }
    else
    	{
    	tagObject->AddTextPropertyL( titleDef,KUnMappedLocation); // Add empty title
	   	}
	
	// Finally add to database.
	TItemId itemId = iMdeSession->AddObjectL( *tagObject );
    
    CleanupStack::PopAndDestroy( tagObject );
    
    LOG( "CTagCreator::CreateTagL - end" );
    
    return itemId;
    }

// ---------------------------------------------------------------------------
// CTagCreator::TagExistsL 
// ---------------------------------------------------------------------------
//
TUint32 CTagCreator::TagExistsL( const TPtrC& aUri  )
    {
    LOG( "CTagCreator::TagExistsL - begin" );
  
    CMdEObject* mdeObject = NULL;
    mdeObject = iMdeSession->GetObjectL( aUri ) ; // Just to find out if it exists.
    
    if( mdeObject )
        {
        LOG( "CTagCreator::TagExistsL - returning object id" );
        return mdeObject->Id();
        }
    else
        {
        LOG( "CTagCreator::TagExistsL - No object, return 0" );
        return 0;
        } 
    }
    
// ---------------------------------------------------------------------------
// CGeoTagger::AttachTagsL 
// Create relationship to country and city tag
// ---------------------------------------------------------------------------
//
EXPORT_C void CTagCreator::AttachTagsL( const TUint32 aObjectId, const TItemId aCountryTagId, 
                             const TItemId aCityTagId )
    {
    LOG( "CGeoTagger::AttachTagsL" );
    CMdENamespaceDef& defaultNamespace = iMdeSession->GetDefaultNamespaceDefL();
    // contains relation definition
    CMdERelationDef& containsRelDef = defaultNamespace.GetRelationDefL(
        Relations::KContains );
    
    // Create relation between country tag and media object (image/video).
    CMdERelation* relationObject = NULL;
    if ( aCountryTagId )
        {
        relationObject = iMdeSession->NewRelationLC ( containsRelDef,
                aObjectId, aCountryTagId, 0 );
        
        if ( !relationObject )
            {
            LOG( "CGeoTagger::AttachTagsL - Couldn't create relation object!" );
            User::Leave ( KErrBadHandle );
            }
        
        iMdeSession->AddRelationL( *relationObject );
            
        CleanupStack::PopAndDestroy( relationObject );
        } 
    
    // Create relation between city tag and media object (image/video). 
    if ( aCityTagId )
        {
        relationObject = iMdeSession->NewRelationLC ( containsRelDef,
                    aObjectId, aCityTagId, 0 );
        
        if ( !relationObject)
            {
            LOG( "CGeoTagger::AttachTagsL - Couldn't create relation object!" );
            User::Leave ( KErrBadHandle );
            }
        
        iMdeSession->AddRelationL( *relationObject );
                
        CleanupStack::PopAndDestroy( relationObject );
        }      

    LOG( "CGeoTagger::AttachTagsL - end" );
    }

// ---------------------------------------------------------------------------
// CGeoTagger::CreateLocationTagsL 
// Create country and city tags
// ---------------------------------------------------------------------------
//
EXPORT_C void CTagCreator::CreateLocationTagsL( const TPtrC& aCountry, TItemId& aCountryTagId, 
                                               const TPtrC& aCity, TItemId& aCityTagId)
	{        
	LOG( "CGeoTagger::CreateLocationTagsL" );

    CreateTagForCountryL( aCountry, aCountryTagId );
    CreateTagForCityL( aCountry, aCity, aCityTagId );

	} 
   
// End of file
