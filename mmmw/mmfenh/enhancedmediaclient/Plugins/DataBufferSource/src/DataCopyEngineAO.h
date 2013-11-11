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
* Description:  Definition of the stream source reader active object class
*
*/


#ifndef DATACOPYENGINEAO_H
#define DATACOPYENGINEAO_H

// INCLUDES
#include <e32base.h>

class CSourceQueueItem;
class CSinkQueueItem;
class MDataCopyEngineObserver;


class CDataCopyEngineAO : public CActive
    {
    public: // Constructors and destructor
        
		/**
		* Two-phased constructor.
		*/
        static CDataCopyEngineAO* NewL( TSglQue<CSourceQueueItem>* aSourceQueue,
            TSglQue<CSinkQueueItem>* aSinkQueue,
            MDataCopyEngineObserver& aObserver );
        
		/**
		* Destructor.
		*/
        virtual ~CDataCopyEngineAO();
        
        // Called by DataBufferSource when a request is added to source queue
        void SourceQueueChanged();
        // Called by DataBufferSource when a request is added to sink queue
        void SinkQueueChanged();
        // Called by DataBufferSource to start the engine
        void Start();
        // Called by DataBufferSource to stop the engine
        void Stop();
        // Called by DataBufferSource to pause the engine
        void Pause();
        
        // Called by Active Object framework
        virtual void RunL();
        virtual void DoCancel();
        virtual TInt RunError(TInt aError);
    private:
        
    /**
    * C++ default constructor.
        */
        CDataCopyEngineAO( TSglQue<CSourceQueueItem>* aSourceQueue,
            TSglQue<CSinkQueueItem>* aSinkQueue,
            MDataCopyEngineObserver& aObserver );
        
            /**
            * By default Symbian 2nd phase constructor is private.
        */
        void ConstructL();
        
        // Used by this to keep the AO going
        void KickSignal();
        
    private:
        enum TDataCopyEngineState
            {
            EStopped,       // Stopped state
                EPaused,        // Paused state
                EExecuting,     // Copying data
                EWaitingForSourceQueueSignal,   // Waiting for item to be available in the source queue
                EWaitingForSinkQueueSignal
            };
        
        // Reference to Source queue passed in thru constructor
        TSglQue<CSourceQueueItem>*  iSourceQueue;
        // Reference to Sink queue passed in thru constructor
        TSglQue<CSinkQueueItem>*    iSinkQueue;
        // Reference to observer
        MDataCopyEngineObserver*    iObserver;
        // State of the engine
        TDataCopyEngineState        iState;
        // Holds true if source queue item is processed
        TBool                       iSourceQueueItemProcessed;
        // Holds true if sink queue item is processed
        TBool                       iSinkQueueItemProcessed;        
        // Temporary buffer containing data from source queue
        HBufC8*                     iSrcDataDes;
        // Data Position within the temporary buffer
        TInt                        iSrcDataPos;
    };

#endif      // DATACOPYENGINEAO_H

//  End of File
