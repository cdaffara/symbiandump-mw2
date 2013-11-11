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
* Description:  Header of DataCopyEngineObserver class.
*
*/


#ifndef DATACOPYENGINEOBSERVER_H
#define DATACOPYENGINEOBSERVER_H

class MDataCopyEngineObserver
    {
    public:
        // Called by iDataCopyEngineAO when item on source queue is processed
        virtual void SourceQueueItemProcessed() = 0;
        // Called by iDataCopyEngineAO when item on sink queue is processed
        virtual void SinkQueueItemProcessed() = 0;
    };

#endif      // DATACOPYENGINEOBSERVER_H

//  End of File
