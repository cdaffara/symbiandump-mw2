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
* Description:  Data transfer object for harvest client data
 *
*/


#ifndef HARVESTERCLIENTDATA_H
#define HARVESTERCLIENTDATA_H

// INCLUDES
#include <e32base.h>
#include <e32cmn.h>

#include <mdccommon.h>

// FORWARD DECLARATIONS

// ENUMERATIONS


/**
 * CHarvestClientData
 */
NONSHARABLE_CLASS( CHarvestClientData ) : public CBase
	{
    public:
        /**
         * New.
         * @return Pointer to newly created object.
         */
        IMPORT_C static CHarvestClientData* New();
        
        /**
         * NewL.
         * Leaving version of New().
         *
         * @return Pointer to newly created object.
         */
        IMPORT_C static CHarvestClientData* NewL();

        /**
         * Destructor.
         */
        ~CHarvestClientData();

        /**
         * Set album ids.
         *
         * @param aAlbumIds Array of album ids to set.
         */
        IMPORT_C void SetAlbumIds( const RArray<TItemId>& aAlbumIds );

    private:
        /**
         * Constructor.
         */
        CHarvestClientData();
        
    public: // data
        RArray<TItemId> iAlbumIds;
    };

#endif // HARVESTERCLIENTDATA_H

