/*
* Copyright (c) 2005 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:  Definition of the S60 Audio Stream Source custom command msg cods.
*
*/


#ifndef __S60STREAMINGSOURCE_H__
#define __S60STREAMINGSOURCE_H__

// Standard EPOC32 includes required by this header file
#include <e32base.h>
#include <e32std.h>
// DevSound includes
//#include <d32snd.h>
#include <e32hal.h>
#include <e32svr.h>
#include <implementationproxy.h> // ECom

#include "S60StreamingSourceUIDs.hrh"

// Public Media Server includes
#include <mmfdatasink.h>
#include <mmfdatasource.h>
#include <mmfdatapath.h>
#include <mmfdatasourcesink.hrh>
#include <mmfaudioiointerfaceuids.hrh>

#include <mmffourcc.h>

// FORWARD DECLARATIONS
class CDataBufferQueueItem;
class CReadRequest;

// CLASS DECLARATION

class CS60StreamingSource : public MDataSource
	{
	public:
		IMPORT_C static CS60StreamingSource* NewL(TUid aType);
		IMPORT_C static CS60StreamingSource* NewLC(TUid aType);

		virtual void ConstructSourceL(  const TDesC8& /*aInitData */);

		virtual TUid DataSourceType() const;
		virtual TFourCC SourceDataTypeCode(TMediaId /*aMediaId*/);
		virtual TInt SetSourceDataTypeCode(TFourCC /*aSourceFourCC*/, TMediaId /*aMediaId*/);
		virtual void FillBufferL(CMMFBuffer* /*aBuffer*/, MDataSink* /*aConsumer*/,TMediaId /*aMediaId*/);
		virtual void BufferEmptiedL(CMMFBuffer* /*aBuffer*/); //called by MDataSink to pass back emptied buffer to the source
		virtual TBool CanCreateSourceBuffer();
		virtual CMMFBuffer* CreateSourceBufferL(TMediaId /*aMediaId*/, TBool &/*aReference*/);

		virtual CMMFBuffer* CreateSourceBufferL(TMediaId aMediaId, CMMFBuffer& /*aSinkBuffer*/, TBool &aReference);


		virtual void SourceStopL();

		virtual void SourceCustomCommand(TMMFMessage& aMessage);

		void HandleFillBuffer(CMMFBuffer* aBuffer, MDataSink* aConsumer);

		IMPORT_C TBool IsSeekingSupported();

		IMPORT_C TBool IsRandomSeekingSupported();

	private:
		CS60StreamingSource(TUid aType);
		~CS60StreamingSource(void);
		void ConstructL (void);

	private: // Data

        // Queue of data buffers to be processed
        TSglQue<CDataBufferQueueItem>* iDataBufferQueue;

        TSglQue<CReadRequest>* iReadRequestQueue;

		TBool iHandlingRequest;
        CReadRequest* iCurrentRequest;

        CDataBufferQueueItem* iSrcBuffer;
	};

#endif // __S60STREAMINGSOURCE_H__

//  End of File
