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
* Description:  Engine observer used by each session to complete events
*
*/


#ifndef M_MPXHARVESTERENGINEOBSERVER_H
#define M_MPXHARVESTERENGINEOBSERVER_H

#include "mpxhvsmsg.h"

class CMPXMedia;

/**
 *  Harvester Engine observer
 *
 *  Each session could have async requests, 
 *  this class allows the engine to callback to a particular session
 *
 *  @lib mpxharvesterserver.exe
 */
class MMPXHarvesterEngineObserver 
    {

public:

    /**
    * Handles the completion of an import event
    * @param aMedia media object to return to the client
    * @param aErr 
    * @param aMsg message to complete
    */
    virtual void HandleImportCompleteL( CMPXMedia* aMedia, 
                                        TInt aErr,
                                        RHvsMsg* aMsg ) = 0;
    
    /**
    * Handles the completion of an export event
    * @param aMedia media object to return to the client
    * @param aErr error code for the operation
    * @param aMsg message to complete
    */
    virtual void HandleExportCompleteL( CMPXMedia* aMedia, 
                                        TInt aErr,
                                        RHvsMsg* aMsg ) = 0;
        
    /**
    * Complete the async request, no need for packaging any returns
    * @param aMsg message to complete
    * @param aErr error code to complete the operation
    */
    virtual void CompleteTask( RHvsMsg* aMsg, TInt aErr ) = 0;
    };


#endif // M_MPXHARVESTERENGINEOBSERVER_H
