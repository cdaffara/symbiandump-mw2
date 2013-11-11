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
* Description:  Definition of the ClientUtility class.
*  Version     : %version: bh1mmcf#8 %
*
*/


// ClientUtility.h
//
// Copyright (c) Symbian Software Ltd 2002-2006.  All rights reserved.
//
// Client utility functions

#ifndef CLIENTUTILITY_H
#define CLIENTUTILITY_H

#include <mda/common/audio.h>
#include <mda/common/gsmaudio.h>
#include <mda/client/utility.h>
#include <mmf/common/mmffourcc.h>
#include <mmfformatimplementationuids.hrh>
#include <mmf/common/mmfcontrollerpluginresolver.h>
#include <mmf/server/mmffile.h>
#include <mmf/server/mmfdes.h>
#include <mmf/common/mmfcontroller.h>
#include <f32file.h>
#include <caf/content.h>
#include <caf/data.h>
#include <mmf/common/mmfstandardcustomcommands.h>
#ifdef SYMBIAN_ENABLE_SPLIT_HEADERS
#include <mmf/common/mmfstandardcustomcommandsimpl.h>
#endif

class RMMFDRMPluginServerProxy;
namespace multimedia
    {
    class CMMSourceSink; // declared here.
    
    NONSHARABLE_CLASS( CMMSourceSink ): public CBase
        {
        public:
            static CMMSourceSink* NewL(TUid aUid, const TDesC8& aDescriptor);
            static CMMSourceSink* NewLC(TUid aUid, const TDesC8& aDescriptor);
            
            virtual ~CMMSourceSink();
            virtual TUid SourceSinkUid() const;
            virtual const TDesC8& SourceSinkData() const;
            
			virtual TBool CarryingFileHandle() const;
            virtual void EvaluateIntentL();
        protected:
            CMMSourceSink(TUid aUid);
            
            
        private:
            void ConstructL(const TDesC8& aDescriptor);
            
            const TUid iUid;
            HBufC8* iBuf;
        };
    
    
    
    
    class CMMFileSourceSink; // declared here.
    
    NONSHARABLE_CLASS( CMMFileSourceSink ): public CMMSourceSink
        {
        public:
            static CMMFileSourceSink* NewL(TUid aUid, const RFile& aFile);
            static CMMFileSourceSink* NewLC(TUid aUid, const RFile& aFile);
            
            static CMMFileSourceSink* NewL(TUid aUid, const TMMSource& aMMSource);
            static CMMFileSourceSink* NewLC(TUid aUid, const TMMSource& aMMSource);
            
            const TDesC& UniqueId() const {return *iUniqueId;}
            
            virtual ~CMMFileSourceSink();
            
            const TDesC& FileName() const {return *iFileName;}
            const TDesC8& SourceSinkData() const;
            
            void EvaluateIntentL(ContentAccess::TIntent aIntent);
            virtual void EvaluateIntentL();
            
			TBool CarryingFileHandle() const;
			TBool UsingFileHandle() const {return iUsingFileHandle;};
			const RFile& FileHandle() const {return iHandle;};

        protected:
            CMMFileSourceSink(TUid aUid);
            
        private:
            void ConstructL(const TMMSource& aSource);
            void DoCreateFileSourceConfigDataL();
            void ConstructL(const RFile& aFile);
            void DoCreateFileHandleSourceConfigDataL();
            
            TBool iUsingFileHandle;
            RFile iHandle;
            HBufC* iFileName;
            HBufC8* iSourceSinkData;
            HBufC* iUniqueId;
            ContentAccess::TIntent iIntent;
            TBool   iEnableUI;
        };
    
    
    
    class CMMFMdaObjectStateChangeObserverCallback; // declared here.
                                                    /**
                                                    Active object utility class to allow the callback to be called asynchronously.
                                                    This should help prevent re-entrant code in clients of the mediaframework.
    */
    NONSHARABLE_CLASS( CMMFMdaObjectStateChangeObserverCallback ): public CActive
        {
        public:
            static CMMFMdaObjectStateChangeObserverCallback* NewL(MMdaObjectStateChangeObserver& aCallback);
            virtual ~CMMFMdaObjectStateChangeObserverCallback();
            void CallBack(CBase* aObject, TInt aPreviousState, TInt aCurrentState, TInt aErrorCode);
        private:
            CMMFMdaObjectStateChangeObserverCallback(MMdaObjectStateChangeObserver& aCallback);
            void RunL();
            void DoCancel();
        private:
            MMdaObjectStateChangeObserver& iCallback;
            CBase* iObject;
            TInt iPreviousState;
            TInt iCurrentState;
            TInt iErrorCode;
        };
    
    class CMMFClientUtility; // declared here.
    
    NONSHARABLE_CLASS( CMMFClientUtility )
        {
        public:
            static TUid ConvertMdaFormatUidToECOMWrite(TUid aMdaFormatUid);
            static TUid ConvertMdaFormatUidToECOMRead(TUid aMdaFormatUid);
            static TFourCC ConvertMdaCodecToFourCC(TMdaPackage& aCodec);
            static TInt GetFileHeaderData(const TDesC& aFileName, TDes8& aHeaderData, TInt aMaxLength);
            static HBufC8* GetFileExtensionL(const TDesC& aFileName);
            static void PrioritiseControllersL(
                const RMMFControllerImplInfoArray& aControllers,
                const TDesC8& aHeaderDataPlayback,
                const TDesC8& aFileExtensionPlayback,
                const TDesC8& aHeaderDataRecord,
                const TDesC8& aFileExtensionRecord,
                RMMFControllerImplInfoArray& aPrioritisedControllers);
            static TInt GetBestMatchL(const CMMFFormatImplementationInformation* format, const TDesC8& aHeaderData, const TDesC8& aFileExtension);
            
        private:
            CMMFClientUtility();
        };
    
    class CMMFUtilityFileInfo; // declared here.
    
    NONSHARABLE_CLASS( CMMFUtilityFileInfo ): public CBase
        {
        public:
            
			static CMMFUtilityFileInfo* NewL(TMMSource& aSource, TBool aSecureDRMMode = EFalse);
			static CMMFUtilityFileInfo* NewLC(TMMSource& aSource, TBool aSecureDRMMode = EFalse);
            
            ~CMMFUtilityFileInfo();
            
            TBool GetFileMimeTypeL(TDes8& aMimeType);
            void  GetFileHeaderDataL(TDes8& aHeaderData, TInt aMaxLength);
            TInt EvaluateIntent(ContentAccess::TIntent aIntent);
            
        private:
            CMMFUtilityFileInfo();
            
			void ConstructL(const TMMSource& aSource, TBool aSecureDRMMode);
            
        private:
            ContentAccess::CData* iData;
			RMMFDRMPluginServerProxy* iDrmPluginServer;
        };
    
    inline CMMFUtilityFileInfo::CMMFUtilityFileInfo()
        {
        };
    
        /**
        * Mixin class that the user of the class CMMFFindAndOpenController must derive from.
        * @internalComponent
    */
    class MMMFFindAndOpenControllerObserver
        {
        public:
        /**
        * Callback function to indicate the success or failure
        * of an attempt to find and open a suitable controller and
        * to add a source and sink.
        * @see CMMFFindAndOpenController
        *
        * @param aError
        *        Indicates whether a controller has been opened sucessfully
        *        This is passed by reference, mainly for the audio recorder utility
        *        which opens two controllers: if the secondary controller (which is
        *        always opened first) fails to open, then the audio recorder utility
        *        may choose to set aError = KErrNone in the MfaocComplete() callback
        *        to indicate to CFindAndOpenControler() that it should continue
        *        to open the primary controller, even though opening the secondary failed.
        * @param aController
        *        A pointer to the controller that has been opened or has failed to open
        *        This is mainly for the audio recorder utility to indicate
        *        which controller (primary or secondary) has been opened.
        * @param aControllerUid
        *        the UID of the controller that has been opened
        * @param aSourceHandle
        *        a pointer to the source handle
        * @internalComponent
        *        a pointer to the sink handle
            */
            virtual void MfaocComplete(
                TInt& aError,
                RMMFController* aController,
                TUid aControllerUid = KNullUid,
                TMMFMessageDestination* aSourceHandle = NULL,
                TMMFMessageDestination* aSinkHandle = NULL) = 0;
        };
    
    
    class CMMFFindAndOpenController; // declared here.
                                     /**
                                     * Utility class used by the MMF client API classes.
                                     * Finds and opens a suitable controller and adds a source and a sink
                                     * asynchronously. Completion is indicated asynchronously
                                     * using the MMMFFindAndOpenControllerObserver mixin class.
                                     *
                                     * @internalComponent
    */
    NONSHARABLE_CLASS( CMMFFindAndOpenController ): public CActive, public MMMFAddDataSourceSinkAsyncObserver
        {
        public:
            enum TControllerMode
                {
                EPlayback,
                    ERecord,
                    EConvert
                };
            
            enum TControllerNumber
                {
                EPrimaryController,
                    ESecondaryController
                };
            
            class TSourceSink
                {
                public:
                    TSourceSink(TUid aUid, const TDesC8& aData = KNullDesC8);
                    TSourceSink(TUid aUid, const RFile& aFile);
                    
                    TUid iUid;
                    const TDesC8& iConfigData;
                    TBool iUseFileHandle;
                    RFile iFileHandle;
                };
            
            
            
            
        public:
            static CMMFFindAndOpenController* NewL(MMMFFindAndOpenControllerObserver& aObserver);
            virtual ~CMMFFindAndOpenController();
            
            // from CActive
            virtual void DoCancel();
            virtual void RunL();
            
            // from MMMFAddDataSourceSinkAsyncObserver
            virtual void MadssaoAddDataSourceSinkAsyncComplete(TInt aError, const TMMFMessageDestination& aHandle);
            
            void Configure(
                TUid aMediaId,
                TMMFPrioritySettings aPrioritySettings,
                CMMFPluginSelectionParameters::TMediaIdMatchType aMediaIdMatchType = CMMFPluginSelectionParameters::EAllowOnlySuppliedMediaIds);
            
            void ConfigureController(RMMFController& aController, CMMFControllerEventMonitor& aEventMonitor, TControllerMode aControllerMode = EPlayback);
            void ConfigureSecondaryController(RMMFController& aController, CMMFControllerEventMonitor& aEventMonitor, TControllerMode aControllerMode = EPlayback);
            
            void UseSharedHeap();
            
            void ConfigureSourceSink(
                TSourceSink aSource,
                TSourceSink aSink);
            void ConfigureSecondarySourceSink(
                TSourceSink aSource,
                TSourceSink aSink);
            
            void ConfigureSourceSink(
                const TMMSource& aSource,
                TSourceSink aSink);
            
            void OpenByFileSource(const TMMSource& aFileSource, const TDesC& aFileNameSecondary = KNullDesC);
            
            void OpenByFormatUid(TUid aFormatUid, TUid aFormatUidSecondary = KNullUid);
            void OpenByDescriptor(const TDesC8& aDescriptor);
            void OpenByUrl(const TDesC& aUrl, TInt aIapId, const TDesC8& aMimeType);
            void OpenByControllerUid(TUid aControllerUid, TUid aSecondaryControllerUid = KNullUid);
            void OpenByMimeType(const TDesC8& aMimeType);
            
            void FindByMimeTypeL(const TDesC8& aMimeType);
            void OpenController();
            
            void ReOpen();
            void Close();
            
            static TMMFFileConfig GetConfigFile(const TDesC& aFileName);
            static TMMFDescriptorConfig GetConfigDescriptor(const TDesC8& aDescriptor);
            static TInt GetConfigUrl(CBufFlat*& aUrlCfgBuffer, const TDesC& aUrl, TInt aIapId);
			void CloseConfig();
            
			/** made public to check for further selected controllers in the queue */
			inline TInt ControllerIndex() const;
			inline TInt ControllerCount() const;
			/** made public to stop checking for further selected controllers in the queue */
			inline TBool StopTryLoadController() const;
			void SetInitScreenNumber(TInt aScreenNumber, RMMFVideoSetInitScreenCustomCommands* aVideoSetInitScreenCustomCommands);

        private:
            class CConfig: public CBase
                {
                public:
                    CConfig();
                    ~CConfig();
                    void Close();
                public:
                    RMMFController* iController;                // not owned
                    CMMFControllerEventMonitor* iEventMonitor;  // not owned
                    
                                                                /** indicates whether this controller is being used for
                    playback, recording or converting */
                    TControllerMode iControllerMode;
                    
                    CMMSourceSink* iSource;
                    CMMSourceSink* iSink;
                    TUid iControllerUid;
                };
            
            CMMFFindAndOpenController(MMMFFindAndOpenControllerObserver& aObserver);
            void ConstructL();
            
            void Init();
            
            void ConfigureSourceSink(
                CConfig& config,
                TSourceSink aSource,
                TSourceSink aSink);
            
            void ConfigureSourceSink(
                CConfig& config,
                const TMMSource& aSource,
                TSourceSink aSink);
            
            
            void ConfigureController(
                CConfig& config,
                RMMFController& aController,
                CMMFControllerEventMonitor& aEventMonitor,
                TControllerMode aControllerMode);
            
            void CloseController();
            
            void OpenPrimaryController(void);
            
            void KickState();
            void Process();
			void SendError(TInt aError = KErrNone, TBool aForcedError = EFalse);
            void SchedSendError(TInt aError = KErrNone);
            void BuildControllerListFileNameL();
            void BuildControllerListDescriptorL();
            void BuildControllerListUrlL();
            void BuildControllerListFormatL();
            void BuildControllerListMimeTypeL();
            void TryNextController();
            
            CMMSourceSink* CreateSourceSinkL(const TSourceSink& aParams);
            CMMSourceSink* CreateSourceSinkL(const TMMSource& aSource);
            
			CMMFUtilityFileInfo* CreateFileInfoL(TBool aSecureDRMMode = EFalse);
		    void UseSecureDRMProcessL(TBool& aIsSecureDrmProcess);

        private:
            /** primary controller details */
            CConfig* iPrimaryConfig;
            /** secondary controller details */
            CConfig* iSecondaryConfig;
            /** points to either iPrimaryConfig or iSecondaryConfig */
            CConfig* iCurrentConfig;    // not owned
            
            enum TMode
                {
                EOpenByControllerUid,
                    EOpenByFileName,
                    EOpenByDescriptor,
                    EOpenByUrl,
                    EOpenByFormatUid,
                    EOpenByMimeType
                };
            TMode iMode;
            
            /** indicates what state the state machine is in */
            enum TState
                {
                EIdle,
                    EBuildControllerList,
                    EOpenController,
                    EAddSource,
                    EAddSink,
                    EWaitingForSource,
                    EWaitingForSink,
                    ESendError
                };
            TState iState;
            
            MMMFFindAndOpenControllerObserver& iObserver;
            CMMFAddDataSourceSinkAsync* iAddDataSourceSinkAsync;
            
            TInt iControllerIndex;
            TInt iControllerCount;
            
            TFileName iFileNameSecondary;   // for converting
            
            TFileName iFileName;
            TBool iUseFileHandle;
            TBool iUseFileSource;
            HBufC* iUniqueId;
            RFile iFileHandle;
            ContentAccess::TIntent iIntent;
            
            HBufC* iUrl;
            HBufC8* iMimeType;
            TPtr8 iDescriptor;
            TInt iIapId;
            TUid iFormatUid;
            TUid iFormatUidSecondary;   // for converting
            
            TUid iMediaId;
            TMMFPrioritySettings iPrioritySettings;
            CMMFPluginSelectionParameters::TMediaIdMatchType iMediaIdMatchType;
            
            RMMFControllerImplInfoArray iControllers;
            RMMFControllerImplInfoArray iPrioritisedControllers;
            
            // if this is non-null, then it points to an element in
            // either iControllers or iPrioritisedControllers
            CMMFControllerImplementationInformation* iControllerImplInfo;   // not owned
            
            TControllerMode iControllerMode;
            TBool iSourceSinkConfigured;
            TInt iError;
            
            TMMFMessageDestination iSourceHandle;
            TMMFMessageDestination iSinkHandle;
            TBool iEnableUi;
            
            TBool iUseSharedHeap; // should new controllers use shared heaps?
			TBool iOwnFileHandle;
			TBool iStopTryLoadController; //stop to try loading the selected conrollers from the list
			RMMFVideoSetInitScreenCustomCommands* iVideoSetInitScreenCustomCommands;
			TInt iScreenNumber;
			TBool iHasDrmCapability;
			TBool iUsingSecureDrmProcess;
        };

inline TInt CMMFFindAndOpenController::ControllerIndex() const
	{
	return iControllerIndex;
	}
	
inline TInt CMMFFindAndOpenController::ControllerCount() const
	{
	return iControllerCount;
	}
	
inline TBool CMMFFindAndOpenController::StopTryLoadController() const
	{
	return iStopTryLoadController;
	}
    } // namespace multimedia
    
#endif  //CLIENTUTILITY_H
    
    //  End of File
