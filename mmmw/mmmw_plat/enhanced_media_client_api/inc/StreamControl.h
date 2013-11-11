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
* Description:  Header of StreamControl interface.
*
*/


#ifndef STREAMCONTROL_H
#define STREAMCONTROL_H

#include <e32base.h>
#include <Control.h>
#include <MCustomInterface.h>

namespace multimedia
    {
    
    // Forward declaration
    class MSourceControl;
    class MSinkControl;
    class MEffectControl;
    
    // Uid identifying Stream Control
    const TUid KStreamControl = {0x10207B44};
    
    // Class declaration
    class MStreamControl : public MControl, public MCustomInterface
        {
        public:
            enum TStreamState
                {
                CLOSED,
                INITIALIZED,
                PRIMED,
                EXECUTING,
                BUFFERING,
                PAUSED
                };

        public:
            // Config msgs
            virtual TInt AddSource( MSourceControl& aSource ) = 0;
            virtual TInt RemoveSource( MSourceControl& aSource ) = 0;
            virtual TInt AddSink( MSinkControl& aSink ) = 0;
            virtual TInt RemoveSink( MSinkControl& aSink ) = 0;
            virtual TInt AddEffect( MEffectControl& aEffect ) = 0;
            virtual TInt RemoveEffect( MEffectControl& aEffect ) = 0;
            virtual TInt SetPosition( TInt64& aPos ) = 0;
            virtual TInt SetPriority( TInt aPriority, TInt aPreference ) = 0;
            
            // Query msgs
            virtual TInt GetDuration( TInt64& aDuration ) = 0;
            virtual TInt GetPosition( TInt64& aPos) = 0;
            virtual TInt GetSeekingSupport( TBool& aFlag ) = 0;
            virtual TInt GetRandomSeekingSupport( TBool& aFlag ) = 0;
            virtual TStreamState GetState() = 0;
            virtual TAny* CustomInterface( TUid aCIUid ) = 0;
            
            // Control msg
            virtual TInt Open() = 0;
            virtual TInt Prime() = 0;
            virtual TInt Start() = 0;
            virtual TInt Pause() = 0;
            virtual TInt Stop() = 0;
            virtual TInt Close() = 0;
            
        };
    } // namespace multimedia

#endif // STREAMCONTROL_H

// End of file
