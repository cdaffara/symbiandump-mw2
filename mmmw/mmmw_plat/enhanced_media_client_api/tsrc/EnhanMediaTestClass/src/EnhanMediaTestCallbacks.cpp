/*
* Copyright (c) 2002 Nokia Corporation and/or its subsidiary(-ies).
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
* Description:  EMC Api Test App
*
*/



// INCLUDE FILES
#include "EnhanMediaTestClass.h"
#include "debug.h"
#include <StreamControlObserver.h>
#include <SourceControlObserver.h>


using namespace multimedia;

using multimedia::MSourceControlObserver;
using multimedia::MStreamControlObserver;





/************************************************************************************************************/

void CEnhanMediaTestClass::Event( MControl* aControl, TUint aEventType, TAny* aEventObject )
	{
	TInt controlTy;
   // TControlType controlType;
    controlTy = aControl->ControlType();
    RDebug::Print(_L("CEnhanMediaTestClass::Event [%d]"),controlTy);
	switch ( controlTy )
		{
		case EStreamControl:
			    {
		    	switch (aEventType)
			        {
			        case MStreamControlObserver::KStateChangedEvent:
			            {
			            MStreamControl* control1 = (MStreamControl*)(aControl);
			            RDebug::Print(_L("CEnhanMediaTestClass::Event:EStateChanged"));
			            switch(control1->GetState())
			                {
			                case MStreamControl::CLOSED:
								RDebug::Print(_L("CEnhanMediaTestClass::Event:EStateChanged[Closed]"));
			                    break;
			                case MStreamControl::INITIALIZED:
							    {
					            iLog->Log(_L("CEnhanMediaTestClass::Event:EStateChanged[Opened]"));
							    RDebug::Print(_L("CEnhanMediaTestClass::Event:EStateChanged[Opened]"));
							    }
			                    break;
			                case MStreamControl::PRIMED:
			                	{
					            iLog->Log(_L("CEnhanMediaTestClass::Event:EStateChanged[Primed]"));
			                	RDebug::Print(_L("CEnhanMediaTestClass::Event:EStateChanged[Primed]"));
			                	}
							    break;
			                case MStreamControl::EXECUTING:
			                	RDebug::Print(_L("CEnhanMediaTestClass::Event:EStateChanged[Playing]"));
			                    break;
			                case MStreamControl::BUFFERING:
			                	RDebug::Print(_L("CEnhanMediaTestClass::Event:EStateChanged[Buffering]"));
			                    break;
			                case MStreamControl::PAUSED:
							    RDebug::Print(_L("CEnhanMediaTestClass::Event:EStateChanged[Paused]"));
			                    break;
			                default:
			                    break;
			                };
			            }
			            break;

			        case MStreamControlObserver::KDurationChangedEvent:
			        	RDebug::Print(_L("CEnhanMediaTestClass::Event:KDurationChangedEvent"));
			            break;
					};
				}
        		break;

        case ESourceControl:
 			    {
 			    switch (aEventType)
			        {
			        case MSourceControlObserver::KBufferingTypesSupportChangedEvent:
			        	{
			            iLog->Log(_L("KBufferingTypesSupportChangedEvent"));
			        	}
			            break;
			        case MSourceControlObserver::KBufferProcessedEvent:
			        	{
			            MBufferProcessedEvent* evt = (MBufferProcessedEvent*)aEventObject;
			            RDebug::Print(_L("CEnhanMediaTestClass::Event:EBufferProcessed[AudioBuffer[0x%x]Reason[%d]]") , evt->GetDataBuffer(), evt->GetErrorCode() );
			            iLog->Log(_L("CEnhanMediaTestClass::Event:EBufferProcessed[AudioBuffer[0x%x]Reason[%d]]") , evt->GetDataBuffer(), evt->GetErrorCode() );
			        	}
			            break;
			        case MSourceControlObserver::KBitRateChangedEvent:
			        	{
		            	TUint rate;
			            iMFileSource->GetBitRate(rate);
			            RDebug::Print(_L("CEnhanMediaTestClass::Event:BitRateChanged[%d]"),rate);
			            iLog->Log(_L("CEnhanMediaTestClass::Event:BitRateChanged[%d]"),rate);
			        	}
			            break;
			        case MSourceControlObserver::KFileMoveCompleteEvent:
			            {
				            RDebug::Print(_L("CEnhanMediaTestClass::Event:KFileMoveCompleteEvent"));
				            iLog->Log(_L("CEnhanMediaTestClass::Event:KFileMoveCompleteEvent"));
			            }
			            break;
			        };
			    }
       			break;

       case ESinkControl:
			    {
                TUid controlUid = aControl->Type();
	            iLog->Log(_L("CEnhanMediaTestClass::Event Sink[%x] Event[%d]"),controlUid, aEventType);
                RDebug::Print(_L("CEnhanMediaTestClass::Event Sink[%x] Event[%d]"),controlUid, aEventType);
			    }
                break;

        case EEffectControl:
			    {
                TUid controlUid = aControl->Type();
	            iLog->Log(_L("CEnhanMediaTestClass::Event Effect[%x] Event[%d]"),controlUid, aEventType);
                RDebug::Print(_L("CEnhanMediaTestClass::Event Effect[%x] Event[%d]"),controlUid, aEventType);
			    }
			    break;
        default:
            break;
		};
	}

// End of File
