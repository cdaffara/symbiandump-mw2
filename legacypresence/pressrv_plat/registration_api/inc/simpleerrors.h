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
* Description:    SIMPLE factory
*
*/




#ifndef SIMPLEERRORS_H
#define SIMPLEERRORS_H

// Simple Engine error codes.

/** 
 * Subscribe requets is accepted by server but pending 
 */
const TInt KSimpleErrPending = -35000;

/** 
 * The subscription has been terminated.
 * Application SHOULD re-subscribe later. 
 */
const TInt KSimpleErrDeactivated = -35001;
    
/** 
 * The subscription has been terminated.
 * Application SHOULD re-subscribe later. 
 */
const TInt KSimpleErrProbation = -35002;
    
/** 
 * The subscription has been terminated due to change in
 * authorization policy.
 * Application SHOULD NOT attempt to re-subscribe. 
 */
const TInt KSimpleErrRejected = -35003;
    
/** 
 * The subscription has been terminated because it was not
 * refreshed before it expired.  Application MAY re-subscribe
 * later. 
 */
const TInt KSimpleErrTimeout = -35004;
    
/** 
 * The subscription has been terminated because the notifier
 * could not obtain authorization in a timely fashion.
 * Application MAY re-subscribe later. 
 */
const TInt KSimpleErrGiveup = -35005;
    
/** 
 * The subscription has been terminated because the resource
 * state which was being monitored no longer exists.
 * Application SHOULD NOT attempt to re-subscribe. 
 */
const TInt KSimpleErrNoresource = -35006;
    
/** 
 * The operation has exceeded SIMPLE Presence Settings limit,
 * e.g. CLIENT-OBJ-DATA-LIMIT or SOURCE-THROTLLE-PUBLISH.
 */
const TInt KSimpleErrSettingsLimit = -35007;

/** 
 * The operation has received SIP error that indicates a
 * temporary problem and the application MAY retry 
 * the same request later on.
 */
const TInt KSimpleErrTemporary = -35008;

/** 
 * The operation has received SIP error that indicates a
 * permanent problem and the application SHOULD NOT retry
 * the same request later on.
 */
const TInt KSimpleErrPermanent = -35009;

/** 
 * The operation has received SIP error that indicates an
 * authorization or authentication problem.
 */
const TInt KSimpleErrAuthorization = -35010;

/** 
 * The operation has received SIP error that indicates 
 * that the user does not exist at the domain specified 
 * in the Request-URI. This is SIP error 404.
 */
const TInt KSimpleErrNotFound = -35011;


#endif      

