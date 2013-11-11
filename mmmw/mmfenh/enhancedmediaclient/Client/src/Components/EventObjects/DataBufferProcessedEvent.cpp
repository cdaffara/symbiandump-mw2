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
* Description:  Implementation of the DataBufferProcessedEvent class
*
*/


#include "DataBufferProcessedEvent.h"

using namespace multimedia;

CDataBufferProcessedEvent::CDataBufferProcessedEvent(
MDataBuffer* aBuffer,
TInt aReason):
iDataBuffer(aBuffer),
iReason(aReason)
    {
    // No impl
    }

CDataBufferProcessedEvent::~CDataBufferProcessedEvent()
    {
    }

TAny* CDataBufferProcessedEvent::GetInterface()
    {
    MBufferProcessedEvent* interface = this;
    return interface;
    }

MDataBuffer* CDataBufferProcessedEvent::GetDataBuffer()
    {
    return iDataBuffer;
    }

TInt CDataBufferProcessedEvent::GetErrorCode()
    {
    return iReason;
    }

// End of file
