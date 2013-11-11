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
* Description:
*    Declares functions needed to save message.
*
*/


#ifndef MSAVEMESSAGECONTROLLER_H
#define MSAVEMESSAGECONTROLLER_H

//  INCLUDES
#include    <CbsCommon.h>



// CLASS DECLARATION

/**
* Helper class for saving message.
*/
class MSaveMessageController
    {
    public: // New functions

        /**
        *   Saves a message (the saved message won't be deleted to make 
        *   room for new messages).
        *
        *   Return codes:
        *   KErrNone            Message is saved.
        *   KErrGeneral         Message not saved -- total maximum of saved 
        *                       messages reached.
        *   KErrNotFound        Message not saved -- no message associated
        *                       with the given handle.
        *   KErrDiskFull        Message not saved -- FFS out of space.
        *
        *   @param  aHandle     Handle to the message to be saved.
        *   @return             Return code.
        */
        virtual TInt SaveMessage( const TCbsMessageHandle& aHandle ) = 0;
    };

#endif // MSAVEMESSAGECONTROLLER_H

// End of file
