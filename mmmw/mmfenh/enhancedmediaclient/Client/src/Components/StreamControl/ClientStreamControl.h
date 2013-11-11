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
* Description:  Definition of the ClientStreamControl class.
*
*/


// ClientStreamControl.h

#ifndef CSTREAMCONTROL_H
#define CSTREAMCONTROL_H

#include <e32base.h>
#include <StreamControl.h>
#include <MCustomInterface.h>
#include <mmf/common/mmfcontroller.h>
#include "ClientUtility.h"
#include <Events.h>
#include <StreamControlCustomCommands.h>
#include <StreamControlObserver.h>
#include "EffectControlBase.h"

//class CMMFFindAndOpenController;
//class CMMFControllerEventMonitor;

namespace multimedia
    {
    class MSinkControl;
    class CEventNotifier;
    class CEventBase;
    class CMCustomCommand;
    
    class CStreamControl : public CBase,
        public MStreamControl,
        public MMMFControllerEventMonitorObserver,
        public MMMFFindAndOpenControllerObserver
        {
        public:
            CStreamControl();
            ~CStreamControl();
            TInt PostConstructor();
            // From MControl begins
            TInt AddObserver( MControlObserver& aObserver );
            TInt RemoveObserver( MControlObserver& aObserver );
            TUid Type();
            TControlType ControlType();
            
            // From MControl ends
            
            
            // From MStreamControl begins
            // Config msgs
            TInt AddSource( MSourceControl& aSource );
            TInt RemoveSource( MSourceControl& aSource );
            TInt AddSink( MSinkControl& aSink );
            TInt RemoveSink( MSinkControl& aSink );
            TInt AddEffect( MEffectControl& aEffect );
            TInt RemoveEffect( MEffectControl& aEffect );
            TInt SetPosition( TInt64& aPos );
            TInt SetPriority( TInt aPriority, TInt aPreference );
            
            // Query msgs
            TInt GetDuration( TInt64& aDuration );
            TInt GetPosition( TInt64& aPos);
            TInt GetSeekingSupport( TBool& aFlag );
            TInt GetRandomSeekingSupport( TBool& aFlag );
            TStreamState GetState();
            TAny* CustomInterface( TUid aCIUid );

            // Control msg
            TInt Open();
            TInt Prime();
            TInt Start();
            TInt Pause();
            TInt Stop();
            TInt Close();
            // From MStreamControl ends
            
            // From MMMFControllerEventMonitorObserver
            void HandleEvent( const TMMFEvent& aEvent );
            
            // From MMMFFindAndOpenControllerObserver
            void MfaocComplete( TInt& aError,
                RMMFController* aController,
                TUid aControllerUid,
                TMMFMessageDestination* aSourceHandle,
                TMMFMessageDestination* aSinkHandle);
            
            
        private:
            enum TStreamControlState
                {
                    EStreamClosed,
                    EStreamOpened,
                    EStreamOpening,
                    EStreamPrimed,
                    EStreamPriming,
                    EStreamPlaying,
                    EStreamStartingPlaying,
                    EStreamBuffering,
                    EStreamPaused,
                    EStreamPausing
                };
            
        private:
            void ConfigureControllerLoader(
                TUid aMediaType,
                CMMFFindAndOpenController::TControllerMode aMode);
            
            TInt LaunchController();
            
            void SetStreamState( TStreamControlState aState, TInt aReason );
            
            void SendEventToClient( TUint aEvent, CEventBase* aEventObject );
        	void SendEventToEffectControls(  TUint aEvent, CEventBase* aEventObject );
            
        private:
            CEventNotifier* iEventNotifier;
            TStreamControlState iStreamState;
            MSourceControl* iSourceControl;
            MSinkControl* iSinkControl;
            RMMFController iController;
            TBool iControllerLoaded;
            TUid iControllerUid;
            CMMFFindAndOpenController* iFindAndOpenController;
            CMMFControllerEventMonitor* iControllerEventMonitor;
            TMMFPrioritySettings iPrioritySettings;
            TTimeIntervalMicroSeconds iPosition;
            CMCustomCommand* iCMCustomCommand;
            RStreamControlCustomCommands iStreamControlCustomCommands;
            TStreamState iState;
            TBool iPrimeController;
        	CEventNotifier* iEventNotifierForEffects;
        	RPointerArray<CEffectControlBase> iAssociatedEffects;
        	TInt iError;
        };
    } // namespace multimedia
    
#endif // CSTREAMCONTROL_H
    
    // End of file
