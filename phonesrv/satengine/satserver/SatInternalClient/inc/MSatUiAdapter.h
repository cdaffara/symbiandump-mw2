/*
* Copyright (c) 2002-2008 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:  The adapter interface for the Sat Client. Used by clients to
*                send envelope commands to SIM applications.
*
*/



#ifndef MSATUIADAPTER_H
#define MSATUIADAPTER_H

//  INCLUDES
#include <e32base.h>
#include <e32svr.h>

// CLASS DECLARATION

/**
 *  An abstract adapter interface.
 *  Use to send SAT envelope commands to the SIM.
 *
 *  @lib SatClient.lib
 *  @since Series 60 2.0
 */
class MSatUiAdapter
    {

    public: // New functions

        /**
         * Response to the Set Up Menu command.
         * @param aMenuItem The selected menu item.
         * @param aHelpRequested Indicates whether help was requested.
         */
        virtual void MenuSelection(
            TInt aMenuItem,
            TBool aHelpRequested ) = 0;

        virtual void SessionTerminated( TInt8 aType ) = 0;

    };

#endif // MSATUIADAPTER_H

// End of File
