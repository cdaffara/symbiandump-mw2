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
* Description:  Definition of the MmfAudioOutputSink class.
*
*/


#ifndef CMMFAUDIOOUTPUTSINK_H
#define CMMFAUDIOOUTPUTSINK_H

#include <e32base.h>
#include <SinkControl.h>
#include "SinkBase.h"
#include <ControlObserver.h>

namespace multimedia
    {

    class CMmfAudioOutputSink : public CSinkBase,
                        		public MSinkControl
        {
        public:
            CMmfAudioOutputSink();
            ~CMmfAudioOutputSink();
            TInt PostConstructor();
            
            // From MControl begins
            TInt AddObserver( MControlObserver& aObserver );
            TInt RemoveObserver( MControlObserver& aObserver );
            TUid Type();
            TControlType ControlType(); 
            // From MControl ends
            
            // From CSinkBase begins
            TUid GetSinkUid();
            // From CSourceBase ends
            
        private:
        };
    } // namespace multimedia

#endif // CMMFAUDIOOUTPUTSINK_H

// End of file
