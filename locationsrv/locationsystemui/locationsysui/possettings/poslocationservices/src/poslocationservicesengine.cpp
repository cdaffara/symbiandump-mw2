/*
* Copyright (c) 2010 Nokia Corporation and/or its subsidiary(-ies).
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
* Description: Implementation of PosLocationServicesEngine class.  
*
*/
#include "poslocationservicesengine.h"


//----------------------------------------------------------------------
// PosLocationServicesEngine::PosLocationServicesEngine
//----------------------------------------------------------------------
PosLocationServicesEngine::PosLocationServicesEngine()
    {
    d_ptr = new PosLocationServicesEnginePrivate( *this );
    }

//----------------------------------------------------------------------
// PosLocationServicesEngine::~PosLocationServicesEngine
//----------------------------------------------------------------------
PosLocationServicesEngine::~PosLocationServicesEngine()
    {
    delete d_ptr;
    d_ptr = NULL;
    }

//----------------------------------------------------------------------
// PosLocationServicesEngine::getLocationServices
//----------------------------------------------------------------------
int PosLocationServicesEngine::getLocationServices( 
                                            QList<PosLocationServiceInfo>& 
                                            locationServiceList )
    {
    return d_ptr->GetLocationServices( locationServiceList );
    }

//----------------------------------------------------------------------
// PosLocationServicesEngine::removeLocationService
//----------------------------------------------------------------------
int PosLocationServicesEngine::removeLocationService( int serviceId )
    {
    return d_ptr->RemoveLocationService( serviceId );
    }

//----------------------------------------------------------------------
// PosLocationServicesEngine::handleLocationServiceUpdate
//----------------------------------------------------------------------
void PosLocationServicesEngine::handleLocationServiceUpdate()
    {
    emit locationServiceUpdate();
    }

// End of file
