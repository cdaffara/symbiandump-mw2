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
* Description:  A class for creating country and city tags for media files.
*
*/


#ifndef C_TAGCREATOR_H
#define C_TAGCREATOR_H

#include "mdesession.h"
#include "mdenamespacedef.h"
#include <mdeconstants.h>

/**
 *
 *  @since S60 9.2
 */
class CTagCreator : public CBase
    {
public:  
    /**
     * 2-phased constructor.
     */
    IMPORT_C static CTagCreator* NewL( );

    /**
     * C++ destructor.
     */    
    IMPORT_C virtual ~CTagCreator();
    
    /**
     * Attach country and city tags to an image file/object.
     * @param aObjectId, object Id for image/video to be tagged.
     * @param aCountryTagId, a country tag Id.
     * @param aCityTagId, a city tag Id.
	   * @return None.
     */
   IMPORT_C void AttachTagsL( const TUint32 aObjectId, const TItemId aCountryTagId, 
                              const TItemId aCityTagId );
    /**
     * Creates country and city tags.
     * @param aCountry, a country name.
     * @param aCountryTagId, reference to a country tag Id.
     * @param aCity, a city name.
     * @param aCityTagId, a reference to a city tag Id.
	   * @return None.
     */
    IMPORT_C void CreateLocationTagsL( const TPtrC& aCountry, TItemId& aCountryTagId, 
                                      const TPtrC& aCity, TItemId& aCityTagId );

	/**
	  * Sets the CMdESession for tag creation
	  * @param aSession  the CMdESession object
	  */
    IMPORT_C void SetSession( CMdESession* aSession );

public:

	/**
	  * Creates a tag for the country
	  * @param aCountry country name
	  * @param aCity city name
	  * @param aCountryTagId   The country tag Id
	  */    
    void CreateTagForCountryL( const TPtrC& aCountry, 
                TItemId& aCountryTagId );

	/**
	  * Creates a tag for the city
	  * @param aCountry The country name
	  * @param aCity   The city name
	  *	@param aCityTagId  The city tag Id
	  */		
    void CreateTagForCityL( const TPtrC& aCountry, const TPtrC& aCity, TItemId& aCityTagId);
    
private:
    /**
     * C++ constructor.
     */  
    CTagCreator( );
    
    /**
     * 2nd phase constructor.
     */
    void ConstructL();

	/**
	  * Creates a tag object and adds it to the database
	  * @param aTitle  The title associated with the tag
	  * @param aUri  The uri associated with the tag
	  * @return The itemId of the newly created tag in the database	
	  */	
    TUint32 CreateTagL( const TPtrC& aTitle, 
        const TPtrC& aUri, 
        MdeConstants::Tag::TTagType aTagType );
	/**
	  * Finds if a tag with the given uri exists
	  * @param  aUri The uri by which the tag is to be searched
	  * @return  The ItemId of the tag if it exists,0 otherwise
	  */	
    
    TUint32 TagExistsL( const TPtrC& aUri  );
    
private:

	/**
	  *  iMdeSession
	  *  an instance of the CMdESession 
	  */		
    CMdESession* iMdeSession;
    
    };

#endif // C_TAGCREATOR_H

// End of file.
