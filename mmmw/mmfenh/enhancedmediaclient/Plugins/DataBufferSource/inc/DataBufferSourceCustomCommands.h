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
* Description:  Definition of the S60 DataBufferSource custom command msg cods.
*
*/


#ifndef DATABUFFERSOURCECUSTOMCOMMANDS_H
#define DATABUFFERSOURCECUSTOMCOMMANDS_H

enum TDataBufferSourceCC
    {
        EProcessBuffer = 1,
        ECancel,
        ESetSize,
        EGetBitRate,
        EGetCurrentBufferingConfig,
        ESetBufferingConfig,
        EGetBufferingConfigSupported,
        ESetDRMConfig,
    };

#endif // DATABUFFERSOURCECUSTOMCOMMANDS_H

//  End of File
