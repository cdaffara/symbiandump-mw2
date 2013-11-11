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
* Description:  An interface to Location Object Manipulator.
*
*/

#ifndef R_RLOCATIONOBJECTMANIPULATOR_H
#define R_RLOCATIONOBJECTMANIPULATOR_H

#include <locationdatatype.h>
#include <etel3rdparty.h>
#include <rlocationmanager.h>
#include <lbsposition.h> 

#include "mdccommon.h"

/**
 *  RLocationObjectManipulator class is used to create, edit, copy and delete 
 *  location context objects and tags and relationships between them 
 *  and media items in the MdE database.
 *
 *  @lib LocationManager.lib
 *  @since S60 3.2
 */
NONSHARABLE_CLASS( RLocationObjectManipulator ) : public RLocationManager
    {

public:	
	/**
	 * Creates a new location object in MdE database with given 
	 * location data. Relationship is created between the location 
	 * object and objects whose ID is given in the ID array.
	 * @since S60 3.2
     * @param aLocationData, Location data consist info for GPS 
     * 		  and network data
     * @param aObjectId, Create relationships with location data and 
     * 		  given media object ID
     * @return KErrNone if successful, otherwise one of the other 
     *         system-wide error codes.
	 */
	IMPORT_C TInt CreateLocationObject( TLocationData& aLocationData,
										TItemId aObjectId );
	
	/**
	 * Creates a new location object in MdE database.
	 * Relationship is created between the location objects whose ID is given.
	 * Location data is taken from location trail.
	 * @since S60 3.2
     * @param aObjectId, Create relationships with location data and 
     * 		  given media object ID
     * @return KErrNone if successful, otherwise one of the other 
     *         system-wide error codes.
	 */
	IMPORT_C TInt LocationSnapshot( TItemId aObjectId );
	
	/**
	 * Deletes the relationship between an object and 
	 * the location context object associated with it.
 	 * @since S60 3.2
     * @param aObjId, Remove relationship from given media object ID
     * @return KErrNone if successful, otherwise one of the other 
     *         system-wide error codes.
	 */
	IMPORT_C TInt RemoveLocationObject( TItemId aObjId );
	
	/**
	 * Copies location data of a media object to one or more 
	 * media objects. If the target media object doesn’t have 
	 * a location object associated to it, one will be created.
 	 * @since S60 3.2
     * @param aSourceId, source media object ID
     * @param aTargetIds, target media object ID
     * @param aStatus Asynchronous request status
	 */
	IMPORT_C void CopyLocationData (TItemId aSourceId, 
									RArray<TItemId>& aTargetIds,
									TRequestStatus& aStatus);
	/**
	 * Copies location data of a media object to one or more 
	 * media objects. If the target media object doesn’t have 
	 * a location object associated to it, one will be created.
  	 * @since S60 3.2
     * @param aSourceURI, source media object URI
     * @param aTargetURIs, target media object URI
     * @param aStatus Asynchronous request status
	 */
	IMPORT_C void CopyLocationData (TDesC& aSourceURI, 
									RPointerArray<TDesC>& aTargetURIs,
									TRequestStatus& aStatus);
    };
    
    
#endif // R_RLOCATIONOBJECTMANIPULATOR_H

//End of File
