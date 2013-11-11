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
* Description:  Observer interface for supplementary services (SS) events.
*
*/


#ifndef MCCESSOBSERVER_H
#define MCCESSOBSERVER_H

/**
* TODO list: 
* Call waiting service status, should be available via phone settings UI.
* DnD status
* see ccp part
*/

/**
 * Supplementary services observer.
 *
 * @since S60 v3.2
 */

class MCCESsObserver
    {
public:

    /** Barring related event. */
    enum TCCESsBarringEvent
        {
        /** Incoming call is barred. (DoCoMo) */
        ECCESsIncomingCallBarred,   
        /** Anonymous call barring. */
        ECCESsAnonymousCallBarred,  
        /** Outgoing call barring */
        ECCESsOutgoingCallBarred,
        /** Barring active notification. */
        ECCESsActiveBarrings    
        };

    /** CLI (Call Line Information) related event. */
    enum TCCESsCLIEvent
        {
        /** Temporary CLIR suppression was unsuccessful */
        ECCESsTempClirSuppressUnsuccessful,     
        /** Temporary CLIR activation was unsuccessful*/
        ECCESsTempClirActivationUnsuccessful    
        };

    /** Call forward related events that are not call related to an existing call. */
    enum TCCESsCallForwardEvent
        {
        /** Incoming call is forwarded */
        ECCESsIncCallIsForw,    
        /** Incoming call was forwarded because of user own settings. */
        ECCESsIncCallForwToC,   
        /** Outgoing call was forwarded because of user own settings. */
        ECCESsOutCallForwToC,   
        /** Forward unconditional mode active */
        ECCESsForwardUnconditionalModeActive,   
        /** Forward unconditional mode deactive */
        ECCESsForwardUnconditionalModeNotActive,
        /** Forward conditional mode active */
        ECCESsForwardConditionallyModeActive,   
        /** Forward conditional mode deactive */
        ECCESsForwardConditionallyModeNotActive,
        /** Remote call waiting. TODO rename TCCESsCallForwardEvent to TCCESsCallEvent*/
        ESsCallWaiting
        };

    /** Closed user group (Cug) */
    enum TCCESsCugEvent
        {
        ECCESsShowIncCallGroupIndex,
        ECCESsRejectedCause
        };

public:

    /**
    * Supplementary service call barring event occurred.
    * @param aBarringEvent Event that occurred.
    * @return none
    * @since S60 v3.2
    */
    virtual void BarringEventOccurred( const MCCESsObserver::TCCESsBarringEvent aBarringEvent ) = 0;
                                  
    /**
    * Supplementary service call line identification event occurred.
    * @param aCallLineEvent Event that occurred.
    * @return none
    * @since S60 v3.2
    */
    virtual void CLIEventOccurred( const MCCESsObserver::TCCESsCLIEvent aCallLineEvent ) = 0;
                                  

    /**
    * Supplementary service related call forward event occurred.
    * @param aForwardMode Forward mode
    * @param aRemoteAddress Remote address.
    * @return none
    * @since S60 v3.2
    */
    virtual void CallForwardEventOccurred( 
                        const MCCESsObserver::TCCESsCallForwardEvent aCallForwardEvent,
                        const TDesC& aRemoteAddress ) = 0;

    /**
    * Supplementary service related active closed user group indications.
    * @param aCugEvent Event that occurred.
    * @return none
    * @since S60 v3.2
    */
    virtual void CallCugEventOccurred( const MCCESsObserver::TCCESsCugEvent aCugEvent ) = 0;

    /**
    * Notifies active ALS line. CS plugin will notify this every time line changes 
    * and at startup.
    * @param aLine Used ALS line.
    * @return none
    * @since S60 v3.2
    */
    virtual void NotifyCurrentActiveALSLine( TInt aLine ) = 0;

    };


#endif // MCCESSOBSERVER_H

