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
* Description:  Custom command def for view utility
*
*/



#ifndef MPXVIEWUTILITYCOMMANDDEF_H
#define MPXVIEWUTILITYCOMMANDDEF_H

const TInt KMPXViewUtilityCustomCommand=0x10282959;

// ENUMS
    
enum TMPXViewUtilityCustomCommands
    {
    EMPXViewUtilCmdViewActivated,
    EMPXViewUtilCmdViewDeActivated // aData contains address of current view
    };

#endif  // MPXVIEWUTILITYCOMMANDDEF_H

// End of File
