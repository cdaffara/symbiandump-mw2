/*
* Copyright (c) 2006 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:  Header of StreamControlCustomCommands interface.
*
*/


#ifndef STREAMCONTROLCUSTOMCOMMANDS_H
#define STREAMCONTROLCUSTOMCOMMANDS_H

#include <mmf/common/mmfcontroller.h>

// Client class to access state change event functionality.
class RStreamControlCustomCommands : public RMMFCustomCommandsBase
    {
    public:
        IMPORT_C RStreamControlCustomCommands(RMMFController& aController);
        IMPORT_C TInt EnableEvents(TBool aEnable);
        IMPORT_C TInt GetSeekingSupported(TBool& aSupported);
        IMPORT_C TInt GetRandomSeekingSupported(TBool& aSupported);
    };

#endif // STREAMCONTROLCUSTOMCOMMANDS_H

// End of file
