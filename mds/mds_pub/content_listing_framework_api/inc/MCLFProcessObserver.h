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
* Description: 
*
*/


#ifndef MCLFPROCESSOBSERVER_H
#define MCLFPROCESSOBSERVER_H

//  INCLUDES
#include <e32std.h>

// DATA TYPES
/**
* Content Listing Framework process events
*/
enum TCLFProcessEvent
    {
    /// CLF server starts update process
    ECLFUpdateStart = 0x0,
    /// CLF server update process is end
    ECLFUpdateStop = 0x1
    };

// FORWARD DECLARATIONS
class MCLFProcessObserverExt;

// CLASS DECLARATION

/**
*  Process observer interface for Content Listing Framework.
*  Implement this interface to get notified of process events. These process
*  events are generated when update process of CLF server is started
*  and finished.
*
*  @lib ContentListingFramework.lib
*  @since S60 3.1
*/
class MCLFProcessObserver
    {
    public: // New functions

        /**
        * Abstract method to handle CLF process event. This method is called
        * when process events are generated.
        * @since S60 3.1
        * @param aProcessEvent The process event that was generated.
        */
        virtual void HandleCLFProcessEventL( TCLFProcessEvent aProcessEvent ) = 0;

    protected:

        /**
        * Destructor.
        */
        virtual ~MCLFProcessObserver() {}

    private: // Extension interface

        /**
        * This member is internal and not intended for use.
        */
        virtual MCLFProcessObserverExt* Extension() { return NULL; }

    };

#endif      // MCLFProcessObserver_H

// End of File
