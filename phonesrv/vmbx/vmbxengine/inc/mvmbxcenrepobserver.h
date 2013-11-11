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
* Description:  Notifies when CenRep is changed
*  Interface   : Internal, MVmbxCenRepObserver
*
*/


#ifndef M_VMBXCENREPOBSERVER_H
#define M_VMBXCENREPOBSERVER_H

// INCLUDES
#include "voicemailboxdefsinternal.h"

// CLASS DECLARATION

/**
 *  MVmbxCenRepObserver
 *
 *  @lib vmbxengine.lib
 *
 */
NONSHARABLE_CLASS ( MVmbxCenRepObserver )
    {
public:

    /**
    * Does the action user wants when the CenRep value has been changed.
    * 
    * @param aId id related with the key of CenRep
    **/
    virtual void HandleCenRepChange( TVmbxCenRepKey aId ) = 0;

    };

#endif // M_VMBXCENREPOBSERVER_H
