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



#ifndef MCLFEVENTHANDLEROBSERVER_H
#define MCLFEVENTHANDLEROBSERVER_H

//  INCLUDES
#include <e32std.h>

// CLASS DECLARATION

/**
*  Event handler observer interface.
*
*  @lib ContentListingFramework.lib
*  @since Series 60 3.0
*/
class MCLFEventHandlerObserver
    {
    public: // New functions
    
        /**
        *  Handle event.
        *  @param aError system-wide error code
        */
        virtual void HandleUpdateEndEventL( TInt aError ) = 0;

        virtual void HandleUpdateStartEventL( TInt aError ) = 0;

        
    protected:
        
        // Destructor
        virtual ~MCLFEventHandlerObserver() {}
    };

#endif      // MCLFEVENTHANDLEROBSERVER_H

// End of File
