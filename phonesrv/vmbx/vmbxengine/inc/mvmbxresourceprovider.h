/*
* Copyright (c) 2009-2010 Nokia Corporation and/or its subsidiary(-ies).
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
* Description:  Notifies when ALS line is changed
*  Interface   : Internal, MResourceProvider
*
*/


#ifndef M_VMBXRESOURCEPROVIDER_H
#define M_VMBXRESOURCEPROVIDER_H

// INCLUDES
#include "mvmbxuiutilities.h"
#include "mvmbxcenrephandler.h"

// CLASS DECLARATION

/**
 *  Resource provider
 *
 *  @lib vmbxengine.lib
 *
 */
NONSHARABLE_CLASS ( MVmbxResourceProvider )
    {
public:

   /**
    * Gets handler of MVmbxUiUtilities
    * 
    * @return handler of MVmbxUiUtilities
    **/
    virtual MVmbxUiUtilities& VmbxUiUtilities() = 0;

   /**
    * Gets handler of MVmbxCenrepHandler
    * 
    * @return handler of MVmbxCenrepHandler
    **/
    virtual MVmbxCenrepHandler& VmbxCenRepHandler() = 0;

    };

#endif // M_VMBXRESOURCEPROVIDER_H
