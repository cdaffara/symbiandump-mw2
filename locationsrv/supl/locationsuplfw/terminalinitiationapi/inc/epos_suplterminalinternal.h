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
* Description:   SUPL Terminal Initiation internal class
*
*/

#ifndef __EPOS_SUPLTERMINALINTERNAL_H__
#define __EPOS_SUPLTERMINALINTERNAL_H__


#include <epos_suplterminal.h>

#include "epos_suplgeocellinfo.h"


// Forward declarations

/**
 * This class is used to make request to convert area information to geo-coordinates. 
 */
class RSuplTerminalInternal : public RSuplTerminalSubSession
	{
	
   
public:
    /**
     * Constructor for RSuplTerminalInternal
     *
     */
	IMPORT_C RSuplTerminalInternal(); 
	
	/**
     * This is an asynchronous request to SUPL Server to determine position 
     * information corresponding to given area information.
     * Client can have only one outstanding asynchronous request per sub-session.  
     * @param [OUT] aStatus to return result code 
     * @param[IN] aGeoCellInfo Cell information
     * @return aStatus will have 
     *         - KErrNone if request is completed sucessfully.  
     *         - KErrCancel if the request was successfully cancelled
     *         - KErrNotReady if SUPL server is not ready to determine position information, 
     *         			 client can retry after sometime.
     *         - KErrAlreadyExists if there is already an ongoing request on sub-session.
     *         - KErrServerTerminated if server got terminated due to some error
     *         - KErrNotSupported if function is called when Session is NOT Opened with SUPL_1_0 service
     *         - KErrGeneral for all other errors
     *
     */                    
    
    IMPORT_C void ConvertAreaInfoToCoordinates(
					TRequestStatus& aStatus, 
					TGeoCellInfo& aGeoCellInfo );
                    
    /**
     * Cancels an outstanding asynchronous conversion request issued via ConvertAreaInfoToCoordinates 
     *  Canceling requests is typically attempted when an client 
     * is closing down.
     *
     */
    IMPORT_C void CancelLocationConversion();
 
private:
    //Cell Information
	TGeoCellInfo iGeoCellInfo;
	
	//To check if conversion request has issued or not
	TBool iConversionRequestIssued;
	};

#endif // __EPOS_SUPLTERMINALINTERNAL_H__
