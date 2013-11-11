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
* Description:  Notifies when MBDN file is refreshed
*  Interface   : Internal, MVmbxSatRefreshObserver
*
*/


#ifndef M_VMBXSATREFRESHOBSERVER_H
#define M_VMBXSATREFRESHOBSERVER_H

// INCLUDES

// CLASS DECLARATION

/**
 *  Notifier for sim file refreshed
 *
 *  @since S60 v5.2
 *  @lib vmbxengine.lib
 *
 */
NONSHARABLE_CLASS ( MVmbxSatRefreshObserver )
    {
public:

    /**
    * Does the action user wants when the sim file is refreshed.
    * 
    * @param 
    **/
    virtual void HandleSatRefresh() = 0;

    };

#endif // M_VMBXSATREFRESHOBSERVER_H
