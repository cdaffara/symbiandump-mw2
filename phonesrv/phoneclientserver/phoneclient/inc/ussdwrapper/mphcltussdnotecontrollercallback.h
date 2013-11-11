/*
* Copyright (c) 2009 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:  Mixing class for CPhCltUssdNoteController. 
*
*/


#ifndef MPHCLTUSSDNOTECONTROLLERCALLBACK_H
#define MPHCLTUSSDNOTECONTROLLERCALLBACK_H

// INCLUDES
#include <e32std.h>


// CLASS DECLARATION

/**
*  Abstract interface for handling the notify events from note controller.
*  User derives his class from this and implements GlobalWaitNoteHidden().
*
*  @lib phoneclient.lib
*  @since 9.1
*/

class MPhCltUssdNoteControllerCallBack
    {
    
    public: // New functions

        /**
        * Called when global wait note is hidden, to background.
        * 
        * @param aError Error code.
        */    
        virtual void GlobalWaitNoteHidden() = 0;
    };


#endif //MPHCLTUSSDNOTECONTROLLERCALLBACK_H

// End of File
