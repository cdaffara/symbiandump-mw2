/*
* Copyright (c) 2006 Nokia Corporation and/or its subsidiary(-ies).
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
* Description:  Utility class to create instances of geographical area.
*
*/


#ifndef LBTOPERATION_H
#define LBTOPERATION_H

#include <e32base.h>
#include <lbtgeoareabase.h>


/**
 *
 * Utility class to create instances of geographical area. The service allows
 * only current cell based geographical area instances to be created. 
 *
 * @lib lbt.lib
 *
 * @since S60 5.1
 */
class CLBTOperation  : public CActive
    {
public: 
    
    /**
     * Allocates and constructs a new operation instance.
     *
     * @param[out] aStatus Contains the error code when the 
     * request is completed.
     * - KErrNone If the operation was successful.
     * - KErrNotSupported If the type specified is not ECellular
     * - Other standard Symbian error code, such as KErrNoMemory,
     * KErrServerBusy, KErrGeneral, etc.
     * @param[in] aTechType The type of geographical area instance to be 
     * created. Currently only ECellular type is supported.
     * @param[out] aArea The geographical area containing the current cell ID
     * information
     * @return Pointer to the new operation instance.
     */
    IMPORT_C static CLBTOperation* CreateGeoAreaForCurrentLocationL( 
                                TRequestStatus &aStatus,
    		                    CLbtGeoAreaBase::TGeoAreaType aTechType,
    		                    CLbtGeoAreaBase ** aArea );
    /**
     * Destructor
     */
	IMPORT_C virtual ~CLBTOperation();
	
protected:
    
    /**
     * Default constructor
     */
	CLBTOperation();
    };

#endif // LBTOPERATION_H
