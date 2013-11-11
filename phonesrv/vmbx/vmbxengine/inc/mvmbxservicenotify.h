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
* Description:  public Service profile changed
*  Interface   : Internal, MServiceNotifyHandler
*
*/


#ifndef M_VMBXSERVICENOTIFY_H
#define M_VMBXSERVICENOTIFY_H

// INCLUDES

// FORWARD DECLARATION


// CLASS DECLARATION

/**
 *  MServiceNotifyHandler
 *
 *  @lib vmbxengine.lib
 *
 */
NONSHARABLE_CLASS ( MServiceNotifyHandler )
    {
public:
      /**
      * Called when a Service profile has been changed.
      */
      virtual void HandleServiceNotifyL() = 0;
    };

#endif // M_VMBXSERVICENOTIFY_H
