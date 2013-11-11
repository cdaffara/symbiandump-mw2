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
* Description:  Inline implementaiton of CRoutingObserver.
*
*/


#ifndef ROUTINGROBSERVER_INL
#define ROUTINGROBSERVER_INL

// INCLUDES

// CLASS DECLARATION
inline void CRoutingObserver::RoutingEvent(
                                CCallAudioControl& /*aCallAudioControl*/,
                                TUint aEvent )
    {
#ifdef _DEBUG    
	RDebug::Print(_L("CRoutingObserver::RoutingEvent Event: %d \n"), aEvent);
#endif
    }

inline void CRoutingObserver::RoutingEvent(
                                CCallAudioControl& /*aCallAudioControl*/,
                                TUint aEvent, TInt aError )
    {
#ifdef _DEBUG    
	RDebug::Print(_L("CRoutingObserver::RoutingEvent Event: Error: %d %d \n"), aEvent, aError);
#endif
    }

#endif      // ROUTINGROBSERVER_INL

// End of File
