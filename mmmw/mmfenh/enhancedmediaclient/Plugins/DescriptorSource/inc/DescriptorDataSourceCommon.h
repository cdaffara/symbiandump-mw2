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
* Description:  Definition of the S60 DescriptorDataSource custom command msg cods.
*
*/


#ifndef DESCRIPTORDATASOURCECUSTOMCOMMANDS_H
#define DESCRIPTORDATASOURCECUSTOMCOMMANDS_H

// UID FIX ME - Allocate a UID for this. This is the ECom plugin uid
const TUid KDescriptorDataSourcePlugin = {0x10207B8B};

enum TDescriptorDataSourceCC
    {
    ESetConfig = 1,
    EFillDataResponse,
    ESeekResponse,
    EGetSourceEvent,
    EGetSourceBitRate
    };

class TCCAttributes
    {
public:
    // Attributes used for ESetConfig
    TBool iSeeking;
    TBool iRandomSeeking;
    TUint iSourceSizeInBytes;
    // Attributes used for EFillDataResponse
    TBool iLastBuffer;
    };
    
typedef TPckgBuf<TCCAttributes> TCCAttributesPckgBuf;

enum TDescriptorDataSourceEventType
    {
    EFillData = 1,
    ESeek,
    EBitRateChanged
    };

class TDescriptorDataSourceEvent
    {
public:
    TDescriptorDataSourceEventType iType;
    TUint iFillDataSize;
    TUint iSeekPos;
    TUint iBitRate;
    };

typedef TPckgBuf<TDescriptorDataSourceEvent> TDescriptorDataSourceEventPckgBuf;

#endif // DESCRIPTORDATASOURCECUSTOMCOMMANDS_H

//  End of File
