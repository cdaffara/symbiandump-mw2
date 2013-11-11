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
* Description:  Definition of the S60 File common parameters.
*
*/


#ifndef FILEDATASOURCECOMMMON_H
#define FILEDATASOURCECOMMMON_H

#include <MultimediaDataSource.h>

const TUid KFileDataSourcePlugin = {0x10207B89};

enum TFileDataSourceCC
    {
    EGetFileSourceEvent = 1,
    EGetBitRateEvent
    };


enum TFileDataSourceEventType
    {
    EFileBitRateChanged = 1,
    EFileSourceStateChanged,
    EFileSize
    };

class TFileDataSourceEvent
    {
public:
    TFileDataSourceEventType iType;
    TUint iBitRate;
    CMultimediaDataSource::TState iState;
    TUint iFileSize;
    };

typedef TPckgBuf<TFileDataSourceEvent> TFileDataSourceEventPckgBuf;

#endif // FILEDATASOURCECOMMMON_H

//  End of File
