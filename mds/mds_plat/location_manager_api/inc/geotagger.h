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
* Description:  A class responsible for geotagging of media files in background.
*
*/

#ifndef GEOTAGGER_H_
#define GEOTAGGER_H_

#include <e32base.h>
#include <e32property.h>
#include <mdesession.h>
#include <geotagobserver.h>


/**
 *  GeoTagger converts a given cell ID(CGI Info) to geo-coordinates.
 *  Does reverse geocoding to get country and city name.
 *  Creates country and city tag for image/video files.  
 *  @since S60 9.2
 */
 
NONSHARABLE_CLASS(CGeoTagger) : public CBase
    {
public:  
    /**
     * 2-phased constructor.
     */
    IMPORT_C static CGeoTagger* NewL(MGeoTaggerObserver* aObserver, CMdESession* aMdeSession = NULL );
    
public:    //exported API
    /**
	   * Harvests location tags.
	   * @param aObserver, observer for geotagging.
	   * @return None.
	   */    
    virtual void CreateGeoTagsL( const TConnectionOption = ESilent ) = 0;

    /**
	   * Is there any untagged photos with location info?.
	   * @return None.
	   */    
    virtual void PendingGeoTagsL( TBool aTagInProgress ) = 0;
    };

#endif // GEOTAGGER_H_ 

// End of file.

