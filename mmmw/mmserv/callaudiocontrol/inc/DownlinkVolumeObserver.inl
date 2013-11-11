/*
* Copyright (c) 2007 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:  Inline implementaiton of CDownlinkVolumeObserver.
*
*/


#ifndef CALLAUDIOCONTROLLEROBSERVER_INL
#define CALLAUDIOCONTROLLEROBSERVER_INL

// INCLUDES

// CLASS DECLARATION
inline void CDownlinkVolumeObserver::DownlinkVolumeEvent(
                                        CCallAudioControl& /*aCallAudioControl*/,
                                        TUint aEvent )
    {
#ifdef _DEBUG    
	RDebug::Print(_L("CDownlinkVolumeObserver::DownlinkVolumeEvent Event: %d \n"), aEvent);
#endif
    }

#endif      // CALLAUDIOCONTROLLEROBSERVER_INL

// End of File
