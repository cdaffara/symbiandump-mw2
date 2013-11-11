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
* Description:  CCP supplementary services (ss) observer.
*
*/


#ifndef MCCPSSOBSERVER_H
#define MCCPSSOBSERVER_H

#include <e32std.h>


/**
* TODO list: 
* Call waiting service status, should be available via phone settings UI.
* DnD status
*/

/**
 * Supplementary services observer.
 *
 * @since S60 v3.2
 */

class MCCPSsObserver
    {
public:

    /** Barring related event.*/
    enum TCCPSsBarringEvent
        {
        /** Incoming call is barred. (DoCoMo) */
        ECCPSsIncomingCallBarred,   
        /** Anonymous call barring. */
        ECCPSsAnonymousCallBarred,  
        /** Outgoing call barring */
        ECCPSsOutgoingCallBarred,
        /** Barring active notification. */
        ECCPSsActiveBarrings    
        };

    /** CLI (Call Line Information) related event. */
    enum TCCPSsCLIEvent
        {
        /** Temporary CLIR suppression was unsuccessful */
        ECCPSsTempClirSuppressUnsuccessful,     
        /** Temporary CLIR activation was unsuccessful*/
        ECCPSsTempClirActivationUnsuccessful    
        };

    /** Call forward related events that are not call related to an existing call. */
    enum TCCPSsCallForwardEvent
        {
        /** Incoming call is forwarded */
        ECCPSsIncCallIsForw,    
        /** Incoming call was forwarded because of user own settings. */
        ECCPSsIncCallForwToC,   
        /** Outgoing call was forwarded because of user own settings. */
        ECCPSsOutCallForwToC,
        /** Forward unconditional mode active */
        ECCPSsForwardUnconditionalModeActive,   
        /** Forward unconditional mode deactive */
        ECCPSsForwardUnconditionalModeNotActive,
        /** Forward conditional mode active */
        ECCPSsForwardConditionallyModeActive,   
        /** Forward conditional mode deactive */
        ECCPSsForwardConditionallyModeNotActive,
        /** Remote call waiting. TODO rename TCCPSsCallForwardEvent to TCCPSsCallEvent*/
        ESsCallWaiting
        };


    /** Closed user group (Cug) 08052007 added */
    enum TCCPSsCugEvent
        {
        ECCPSsShowIncCallGroupIndex,
        ECCPSsRejectedCause
        };

public:

    /**
    * Supplementary service call barring event occurred.
    * @param aBarringEvent Event that occurred.
    * @return none
    * @since S60 v3.2
    */
    virtual void BarringEventOccurred( const MCCPSsObserver::TCCPSsBarringEvent aBarringEvent ) = 0;
                                  
    /**
    * Supplementary service call line identification event occurred.
    * @param aCallLineEvent Event that occurred.
    * @return none
    * @since S60 v3.2
    */
    virtual void CLIEventOccurred( const MCCPSsObserver::TCCPSsCLIEvent aCallLineEvent ) = 0;

    /**
    * Supplementary service related call forward event occurred. 
    * @param aCallForwardEvent Event that occurred.
    * @param aRemoteAddress Remote address.
    * @return none
    * @since S60 v3.2
    */
    virtual void CallForwardEventOccurred( 
                    const MCCPSsObserver::TCCPSsCallForwardEvent aCallForwardEvent,
                    const TDesC& aRemoteAddress ) = 0;

    /**
    * Supplementary service related active closed user group indications.
    * @param aCugEvent Event that occurred.
    * @return none
    * @since S60 v3.2
    */
    virtual void CallCugEventOccurred( const MCCPSsObserver::TCCPSsCugEvent aCugEvent ) = 0;

    /**
    * Notifies active ALS line. CS plugin must notify this every time line changes 
    * and at startup.
    * @param aLine Used ALS line.
    * @return none
    * @since S60 v3.2
    */
    virtual void NotifyCurrentActiveALSLine( TInt aLine ) = 0;
    };


#endif // MCCPSSOBSERVER_H
