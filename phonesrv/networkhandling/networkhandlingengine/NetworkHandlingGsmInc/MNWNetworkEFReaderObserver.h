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
* Description:  This file contains the header file of the 
*  MNWNetworkEFReaderObserver class.
*
*/



#ifndef MNWNETWORKHEFREADEROBSERVER_H
#define MNWNETWORKHEFREADEROBSERVER_H

//  INCLUDES
#include <e32property.h>
// CONSTANTS

// FORWARD DECLARATIONS

// CLASS DECLARATION

/**
*  Oberver class for CNWNetworkEFReader
*  
*
*  @lib networkhandling.lib
*  @since Series 60_3.0
*/

NONSHARABLE_CLASS( MNWNetworkEFReaderObserver )
    {
    public: 
        virtual void ReadCbCacheSync( TInt aStatus ) = 0;
    };

#endif      // MNWNETWORKHEFREADEROBSERVER_H
            
// End of File
