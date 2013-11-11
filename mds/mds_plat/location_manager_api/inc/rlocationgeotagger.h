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
* Description:  An interface to Location Trail.
*
*/

#ifndef R_RLOCATIONGEOTAGGER_H
#define R_RLOCATIONGEOTAGGER_H

#include <e32base.h>
#include <etel3rdparty.h>
#include <lbsposition.h>
#include <rlocationmanager.h>
#include <locationdatatype.h>
#include <geotagobserver.h>


/**
 *  RLocationManager class is used for creating a Location Manager session.
 *  Location Manager is used to start and stop the location trail, retrieve 
 *  location information and write the location information to images.
 *
 *  @lib LocationManager.lib
 *  @since S60 9.2
 */
NONSHARABLE_CLASS( RLocationGeoTagger ) : public RLocationManager
    {
public:
        
public:
	IMPORT_C RLocationGeoTagger();
	
	IMPORT_C ~RLocationGeoTagger();


    /**
     * Is there any geo tag pending state. (Asynchronous call)
     * @param aStatus - request status.
     * @param aTagPendingFlag - tag pending flag (ETrue if few entry is yet to be tagged else EFalse)
     * @return None
     */
    IMPORT_C void GeoTagStaus( TRequestStatus& aStatus, 
                                   TGeoTaggingSatus& aTagPendingFlag );
    
    /**
     * Cancel tag pending request
     * @return None
     */
    IMPORT_C void CancelTagPendingRequest();

    /**
     * Cancel geo tagging request
     * @return None
     */
    IMPORT_C void CancelGeoTaggingRequest();

    /**
     * Start geo tagging.
     * @param aConnectionOption - connection option (ESilent/EInteractive).
     * @return None
     */
    IMPORT_C void StartGeoTagging(TRequestStatus& aStatus, const TConnectionOption aConnectionOption );

private:
	TPckg<TGeoTaggingSatus> *iTagPendingData;
    };
    
#endif // R_RLOCATIONGEOTAGGER_H

//End of File
