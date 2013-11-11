/*
* Copyright (c) 2008 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:  Observes service provider settings changes
*
*/



#ifndef MCCESPSOBSERVER_H
#define MCCESPSOBSERVER_H

/**
 *  Observes service provider settings changes
 *  
 *
 *  @lib cce.dll
 *  @since S60 v3.2
 */
class MCCESPSObserver
    {
    
public:

    /**
     * Service enabled
     *
     * @since S60 v3.2
     * @param Implementation id of call
     */
    virtual void ServiceEnabledL( TUid aImplementationUid ) = 0;

    /**
     * Service disabled
     *
     * @since S60 v3.2
     * @param Implementation id of call
     */
    virtual void ServiceDisabledL( TUid aImplementationUid ) = 0;

    
    };

#endif      // MCCESPSOBSERVER_H

// End of File

