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
* Description:  Callback interface class.
*
*/


#ifndef MLPDRESULTHANDLER_H
#define MLPDRESULTHANDLER_H

// INCLUDES
#include <e32def.h>

// FORWARD DECLARATIONS
class CLpdQueryLauncherBase;


// CLASS DECLARATION

/**
*  Observer pattern for handling dialog result. Used by several
*  classes deriving from CLpdQueryLauncherBase.
*
*  @lib locverifierdlg.lib
*  @since 2.1
*/
class MLpdResultHandler
    {
    public: // New functions

        /**
        * Pure virtual observer method.
        * @param aResultCode
        */        
        virtual void HandleVerificationResultL( TInt aResultCode ) = 0;

        /**
        * Pure virtual observer method.
        * @param aResultCode
        */        
        virtual void HandleNotificationResultL( TInt aResultCode ) = 0;

        /**
        * Pure virtual observer method. Called if leave occurred in other
        * callback methods. Used because it is not acceptable to propagate
        * leave to AVKON dialog, plugin must clean up and continue with
        * handling of request queue.
        * @param aError code
        */        
        virtual void HandleLeave( TInt aError ) = 0;
    };

#endif      // MLPDRESULTHANDLER_H   
            
// End of File
