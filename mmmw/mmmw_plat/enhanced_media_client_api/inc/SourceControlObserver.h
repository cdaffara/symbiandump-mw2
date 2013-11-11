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
* Description:  Header of SourceControlObserver interface.
*
*/


#ifndef SOURCECONTROLOBSERVER_H
#define SOURCECONTROLOBSERVER_H

#include <ControlObserver.h>

namespace multimedia
    {
    class MSourceControlObserver : public MControlObserver
        {
        public:    
    		//A constant that defines the event when buffer processed event occurs
    		static const TUint KBufferProcessedEvent                = KSpecificEventBase + 1;
    		//A constant that defines the event when buffering types supported changes
    		static const TUint KBufferingTypesSupportChangedEvent   = KSpecificEventBase + 2;
    		//A constant that defines the event when download status changes
    		static const TUint KDownloadStatusChangedEvent          = KSpecificEventBase + 3;
    		//A constant that defines the file move operation completed event
    		static const TUint KFileMoveCompleteEvent               = KSpecificEventBase + 4;
        	// A constant that defines the Percentage of Donwload change event
        	static const TUint KPercentageDownloadedChangedEvent    = KSpecificEventBase + 5;
        	// A constant that defines the BitRate Changed Event
        	static const TUint KBitRateChangedEvent                 = KSpecificEventBase + 6;
        };
    } // namespace multimedia

#endif // SOURCECONTROLOBSERVER_H

// End of file
