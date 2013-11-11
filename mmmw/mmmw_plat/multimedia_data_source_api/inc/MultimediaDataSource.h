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
* Description:  Header of MultimediaDataSource interface.
*
*/


#ifndef MULTIMEDIADATASOURCE_H
#define MULTIMEDIADATASOURCE_H

#include <e32base.h>
#include <MCustomInterface.h>
#include <mmf/common/mmfutilities.h>
#include <mmf/common/mmfbase.h>
#include <caf/manager.h>


// Forward declaration
class CMMFBuffer;


//Observer
class MMultimediaDataSourceObserver
    {
    public:
        // Called on the observer when buffer is filled. aBuffer is guarenteed to
        // contain data of size CMMFBuffer::RequestSize() unless it is the last
        // buffer in the media stream.
        // When end of media stream is reached, aBuffer will have last buffer flag
        // set and data size may be less then or equal to CMMFBuffer::RequestSize().
        // When observer receives last buffer, any outstanding FillBuffer() requests
        // will be cancelled by the source automatically. The observer is free to use
        // the buffers that were associated with those outstanding requests.
        virtual void BufferFilled( CMMFBuffer* aBuffer ) = 0;
        
        // KStreamControlEventSeekingSupportChanged
        // KStreamControlEventRandomSeekingSupportChanged
        // KMultimediaDataSourceObserverEventSourceSizeChanged
        virtual void Event( TUid aEvent ) = 0;
        // Returns BITRATE. This is used for calculation how much of data need to be
        // buffered by the source before filling observer buffers.
        virtual TInt GetBitRate( TUint& aBitRate ) = 0;
    };


// Class
// States (CLOSED, STOPPED, PRIMED, EXECUTING, BUFFERING)
// Factory takes a observer reference and MDataSource reference and creates
// concrete implementation class instance.
// All APIs are synchronous, FillBuffer() can trigger a BufferFilled() callback.

class CMultimediaDataSource :/* public CBase,*/
public MCustomInterface
    {
    public:
        inline virtual ~CMultimediaDataSource();
        // Sets the reference observer who will receive callbacks.
        // Can be called in CLOSED state.
        virtual TInt SetObserver( MMultimediaDataSourceObserver& aObserver ) = 0;
        virtual TInt GetObserver( MMultimediaDataSourceObserver*& aObserver ) = 0;
        
        // Called when an event occurs that could affect functioning of Source.
        // BITRATECHANGED
        virtual void Event( TUid aEvent ) = 0;
        
        // The source does not know any information about the 4CC, the observer
        // needs to set the 4CC.
        // Can be called in any state.
        virtual TInt SetDataTypeCode(TFourCC aSourceFourCC ) = 0;
        virtual TInt GetDataTypeCode(TFourCC& aSourceFourCC ) = 0;
        
        // Sets the priority settings. This is needed if in the future the source
        // have some kind of source policy they need to go through for fulfilling
        // observer requests.
        inline virtual TInt SetPrioritySettings(
            const TMMFPrioritySettings& aPrioritySettings );
        
        // Returns the size of source in bytes.
        // (Max value 4294967295 bytes. Approximately 3.999 GB)
        // Can be called in PRIMED/EXECUTING/BUFFERING state.
        virtual TInt GetSize( TUint& aSize ) = 0;
        
        /* DRM Interface */
        // Can be called in PRIMED/EXECUTING/BUFFERING state.
        inline virtual TInt GetDRMProtection( TBool& aIsProtected );
        inline virtual TInt EvaluateIntent( ContentAccess::TIntent aIntent );
        inline virtual TInt ExecuteIntent( ContentAccess::TIntent aIntent );
        inline virtual TInt SetAgentProperty(
            ContentAccess::TAgentProperty aProperty,
            TInt aValue);
        
        
        /* State Machine Interface */
        enum TState
            {
                ECLOSED,
                ESTOPPED,
                EPRIMED,
                EEXECUTING,
                EBUFFERING
            };
        // Return current state
        inline virtual void GetState( TState& aState );
        // Signals the source to allocate resources and trigger moving to STOPPED
        // state.
        // KErrNone if source moves to STOPPED state succesfully, systemwide error
        // code otherwise.
        // Can be called in CLOSED state. Calling it in other states return
        // KErrNotReady.
        virtual TInt Open() = 0;
        // Signals the source to deallocate resources and trigger moving to CLOSED
        // state.
        // KErrNone if source moves to CLOSED state succesfully, systemwide error
        // code otherwise.
        // Can be called in PRIMED/STOPPED/EXECUTING/BUFFERING state. When called in 
        // PRIMED/EXECUTING/BUFFERING state, source releases reference to observer
        // buffers (observers will not get BufferFilled()).
        virtual TInt Close() = 0;
        // Signals source to move to PRIMED state. Intent is to reduce the start
        // up time.
        // Connects to data source (file) if needed.
        // Accepts FillBuffer() requests. Buffers data if needed. Does not call
        // BufferFilled() on the Observer.
        // Can be called in STOPPED/PRIMED state. If it is called in PRIMED state,
        // source releases references to observer buffers that were received via
        // FillBuffer() before Prime() was called. Observers will not get
        // BufferFilled() for those de-referenced buffers.
        // KErrNone if source moves to PRIMED state succesfully, systemwide error
        // code otherwise. KErrNotReady if source is in CLOSED/EXECUTING/BUFFERING
        // state.
        virtual TInt Prime() = 0;
        // Signals source to go to EXECUTING state.
        // Returns buffer to observer via BufferFilled() if enough data is buffered
        // up. Accepts FillBuffer() requests.
        // Can be called in PRIMED/EXECUTING state.
        // KErrNone if source moves to EXECUTING state succesfully, systemwide error
        // code otherwise. KErrNotReady if source is in STOPPED/CLOSED state. If
        // the source is already in EXECUTING state, the call is ignored.
        // If the last buffer is sent to observer, source automatically moves to
        // STOPPED state.
        virtual TInt Play() = 0;
        // Signals source to go to STOPPED state.
        // Will stop receiving FillBuffer() requests from observer. Cancels any
        // outstanding FillBuffer() requests and de-references observer buffers.
        // Can be called in PRIMED/BUFFERING/EXECUTING state.
        // Current read position is reset to zero. Buffered data size is reset to
        // zero.
        // Disconnects from the data source (file) if needed.
        // KErrNone if source moves to STOPPED state succesfully, systemwide 
        // error code otherwise. KErrNotReady if source is in CLOSED state. If the
        // source is already in STOPPED state, this call will be ignored.
        virtual TInt Stop() = 0;
        
        /* OutputBuffers Interface */
        // Requests source to fill data of size aBuffer->RequestSize() in to aBuffer
        // asynchronously. After source fills data into aBuffer, it will notify the
        // observer via BufferFilled() observer callback.
        // Source saves a temporary reference to aBuffer after the call is returned
        // with KErrNone. Once the buffer is given to source, Observer should not try
        // to use the aBuffer until it receives the buffer back through 
        // BufferFilled() callback or the source moves to STOPPED state.
        // Valid in PRIMED/EXECUTING/BUFFERING state. Returns KErrNotReady when
        // called in other states.
        virtual TInt FillBuffer( CMMFBuffer* aBuffer) = 0;
        
        /* Seeking Interface */
        // Returns true if byte position can be reset to zero.
        // Valid in all states
        inline virtual TInt GetSeekingSupport( TBool& aSeekSupport );
        // Returns true if byte position can be set anywhere between zero and
        // GetSize().
        // Valid in all states
        inline virtual TInt GetRandomSeekingSupport( TBool& aSeekSupport );
        // Moves read position pointer to byte location requested by the observer
        // between zero and GetSize(). If the size specified is out of range, this
        // function returns KErrArgument.
        inline virtual TInt Seek( TUint aPosInBytes );
        
        // Extension mechanism
        inline virtual TInt GetInterface(
                                TUid aInterfaceId,
                                TVersion& aVer,
                                TAny*& aInterfaceImpl );
        
protected:
    // Holds the current state.
    TState iState;
    };
    
#include "MultimediaDataSource.inl"
    
#endif // MULTIMEDIASOURCE_H
    
    // End of file
