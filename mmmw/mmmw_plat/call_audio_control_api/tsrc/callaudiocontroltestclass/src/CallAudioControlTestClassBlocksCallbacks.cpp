/*
* Copyright (c) 2002-2008 Nokia Corporation and/or its subsidiary(-ies).
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
* Description: Call Audio Control
*
*/



// INCLUDE FILES
#include "CallAudioControlTestClass.h"
#include "debug.h"



// Necessary for Observers:
/*************************************************************************/

 void CCallAudioControlTestClass::DownlinkVolumeEvent(CCallAudioControl& /*aCallAudioControl*/, TUint aEvent)
 {

		iLog->Log(_L("CCallAudioControlTestClass::DownlinkVolumeEvent with event = %d"), aEvent);
		switch (aEvent)
    {
        case CDownlinkVolumeObserver::KDownlinkVolumeChanged:
            iLog->Log(_L("CCallAudioControlTestClass::DownlinkVolumeEvent::KDownlinkVolumeChanged"));
           // ProcessEvent(EDownlinkVolumeChanged, KErrNone);
            Signal();
            break;
            // KDownlinkMuteStateChanged is not defined in DownlinkVolumeObserver.h
     //   case CDownlinkVolumeObserver::KDownlinkMuteStateChanged:
     //       iLog->Log(_L("CCallAudioControlTestClass::DownlinkVolumeEvent::KDownlinkMuteStateChanged"));
      //      ProcessEvent(EDownlinkVolumeChanged, KErrNone);
      //      break;

        default:
            break;
     }

 }


 void CCallAudioControlTestClass::UplinkGainEvent(CCallAudioControl& /*aCallAudioControl*/, TUint aEvent)
 {
		iLog->Log(_L("CCallAudioControlTestClass::UplinkGainEvent with event = %d"), aEvent);

		switch (aEvent)
    {
        case CUplinkGainObserver::KUplinkMuteStateChanged:
            iLog->Log(_L("CCallAudioControlTestClass::CUplinkGainObserver::KUplinkMuteStateChanged"));
            ProcessEvent(EUplinkMuteStateChanged, KErrNone);
            break;
        default:
            break;
     }
 }

 void CCallAudioControlTestClass::RoutingEvent(CCallAudioControl& /*aCallAudioControl*/, TUint aEvent)
 {

		iLog->Log(_L("CCallAudioControlTestClass::RoutingEvent"));
		switch (aEvent)
    {
        case CRoutingObserver::KAvailableOutputsChanged:
            iLog->Log(_L("CCallAudioControlTestClass::RoutingEvent:KAvailableOutputsChanged"));
            // don't know how to triggle this event
        //    ProcessEvent(EAvailableOutputsChanged, KErrNone);
            break;
        case CRoutingObserver::KOutputChanged:
            iLog->Log(_L("CCallAudioControlTestClass::RoutingEvent:KOutputChanged"));
            ProcessEvent(EOutputChanged, KErrNone);
            break;
        default:
            break;
        }
 }

 void CCallAudioControlTestClass::RoutingEvent(CCallAudioControl& /*aCallAudioControl*/, TUint aEvent, TInt aErr)
 {
		iLog->Log(_L("CCallAudioControlTestClass::RoutingEvent with event = %d"), aEvent);
		switch (aEvent)
    {
        case CRoutingObserver::KSetOutputComplete:
            iLog->Log(_L("CCallAudioControlTestClass::RoutingEvent:KSetOutputComplete:aErr[%d]"),aErr);
            ProcessEvent(ESetOutputComplete, aErr);
            break;
        default:
            break;
     }
 }

/************************************************************************************************************/


