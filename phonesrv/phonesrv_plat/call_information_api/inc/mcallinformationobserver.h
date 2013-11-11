/*
* Copyright (c) 2007-2008 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:  Observer class for client to implement for receiving call info notification.
*
*/


#ifndef M_CALLINFORMATIONOBSERVER_H
#define M_CALLINFORMATIONOBSERVER_H


/**
 *  Defines function that client must implement in order to receive notification
 *  about changes in calls. 
 * 
 *
 *  @lib telephonyservice.lib
 *  @since S60 v5.1
 */
class MCallInformationObserver 
    {

public:
    
    /**
     * Signals that there are changes in ongoing calls. Client should fetch 
     * the call infomations after receiving the notification.
     *
     * @since S60 v5.1     
     * @return void
     */
    virtual void CallInformationChanged()  = 0;    
      
    };


#endif // M_CALLINFORMATIONOBSERVER_H
