/*
* Copyright (c) 2004 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:  Class definition for the SourceAdapter functions.
*
*/



#ifndef CDATASOURCEADAPTER_H
#define CDATASOURCEADAPTER_H

//  INCLUDES
#include <mmfbuffer.h>
#include <mmfutilities.h>
#include <e32base.h>
#include <mmfbase.h>
#include <caf/manager.h>

// CONSTANTS
const TUid KS60AudioStreamingSourceUid	= {0x10207AF3};
const TUid KMmdsProgDLSourceUid			= {0x10207B46};
const TUid KMmdsStreamingSourceUid		= {0x10207B7B};
const TUid KOldProgDLSourceUid			= {0x10207A7C};
const TUid KMmdsFileSourceUid			= {0x10207B89};
const TUid KMmdsDescriptorSourceUid		= {0x10207B8B};

const TInt KErrSourceAdapter            = -1001;

// FORWARD DECLARATIONS
class CMMFClip;
class CMMFDataBuffer;
class MDataSink;
class MDataSource;
class MAsyncEventHandler;
class CConfigurationComponentsFactory;
class CAudioOutputControlUtility;
class CDRMConfigIntfc;
class CMMFDevSound;
class CMMFBuffer;
class MMultimediaDataSourceObserver;        
class CMultimediaDataSource;
class CAudioOutputControlUtility;  
// CLASS DEFINITIONS

/**
*  This class provides CDataSourceAdapter functions.
*
*  @lib CDataSourceAdapter.lib
*  @since 3.2
*/
class CDataSourceAdapter : public CBase
    {
	public:  // Constructors and destructor

        /**
        * Two-phased constructor.
        */
        IMPORT_C static CDataSourceAdapter* NewL();

        /**
        * Destructor.
        */
        IMPORT_C virtual ~CDataSourceAdapter();

		// New (like MDataSource)
		IMPORT_C virtual TUid DataSourceType();
		IMPORT_C virtual void FillBufferL(CMMFBuffer* aBuffer, MDataSink* aConsumer, TMediaId aMediaId);
		IMPORT_C virtual void BufferEmptiedL(CMMFBuffer* aBuffer);
		IMPORT_C virtual TBool CanCreateSourceBuffer();
		IMPORT_C virtual CMMFBuffer* CreateSourceBufferL(TMediaId aMediaId, TBool &aReference);
		IMPORT_C virtual TInt SourceThreadLogon(MAsyncEventHandler& aEventHandler);
		IMPORT_C virtual void SourceThreadLogoff();
		IMPORT_C virtual void SourcePrimeL();
		IMPORT_C virtual void SourceStopL();
		IMPORT_C virtual void SourcePlayL();
        IMPORT_C virtual void Event(TUid aEvent);

		// New
		IMPORT_C virtual TInt SeekToPosition(TUint aPosition);
		IMPORT_C virtual TInt SeekToTime(TUint aTimeMs);
        IMPORT_C virtual TInt SeekToTime(TUint aTimeMs, TUint& aFoundTimeMs);
		IMPORT_C virtual TBool IsTimeSeekable();
		IMPORT_C virtual TBool IsPositonSeekable();
		IMPORT_C virtual void SetDataSourceL(MDataSource* aDataSource,
											MMultimediaDataSourceObserver* aMMultimediaDataSourceObserver,
											MAsyncEventHandler* aMAsyncEventHandler);
		IMPORT_C virtual TInt SourceSize();
		IMPORT_C virtual TInt IsProtectedL();
		IMPORT_C virtual void SetSourcePrioritySettings(const TMMFPrioritySettings& aPrioritySettings);
		IMPORT_C virtual TInt ExecuteIntent(ContentAccess::TIntent aIntent);
		IMPORT_C virtual TInt SetAgentProperty(ContentAccess::TAgentProperty aProperty, TInt aValue);
		IMPORT_C virtual TInt EvaluateIntent(ContentAccess::TIntent aIntent);
        IMPORT_C virtual TInt GetInterface(TUid aInterfaceId, TVersion& aVer, TAny*& aInterfaceImpl);
		IMPORT_C virtual TBool OnlyHeaderPresent();
		IMPORT_C virtual void ResetDataSource();
		
		//sets the AudioOutputControlUtitlity reference and sets Datasource to AudioOutputContolUtility
		IMPORT_C TInt SetAudioOutputControlUtil(CAudioOutputControlUtility* aAudioOutputControlUtility);
		IMPORT_C virtual TBool IsLocalPlayback();
	protected:

        /**
        * C++ default constructor.
        */
		IMPORT_C CDataSourceAdapter();

	private:

        /**
        * By default Symbian 2nd phase constructor is private.
        */
		void ConstructL();

	protected:
        class CAsyncProxyFillBuffer : public CActive
            {
            public: // may want to export constructor and fillbuffer for 3gpdatasourceadapter
                CAsyncProxyFillBuffer(MDataSource* aDataSource);
            	~CAsyncProxyFillBuffer();
            	void FillBuffer(CMMFBuffer* aBuffer, MDataSink* aConsumer, TMediaId aMediaId);
                void Reset();
            
            protected:	
                void RunL();
                TInt RunError(TInt aError);
                void DoCancel();

            private:
                TRequestStatus* iRequestStatus;
                MDataSource* iDataSource;
			    CMMFBuffer* iBuffer;
    			MDataSink* iConsumer;
    			TMediaId iMediaId;
				RPointerArray<CMMFBuffer> iQueuedAsyncBuffers;
				TInt			iReadPos;
            };

	protected:	// Data

		TUid			iSourceType;
		MDataSource*	iDataSource;
        CMultimediaDataSource* iMMDataSource;
        MAsyncEventHandler* iAsyncEventHandler;
		TInt			iSourceSize;
		TBool			iPosSeekable;
		TBool			iTimeSeekable;
		TBool			iIsProtected;
		CMMFDataBuffer* iZeroBuffer;
		CMMFDataBuffer* iHdrBuffer;
		CMMFClip*		iClip;
		CAsyncProxyFillBuffer*	iAsyncProxyFillBuffer;
		TBool			iHeaderOnly;
		TBool			iIsLocalPlayback;
		CAudioOutputControlUtility* iAudioOutputControlUtility;  
    };

#endif 		// CDATASOURCEADAPTER_H

// End of File
