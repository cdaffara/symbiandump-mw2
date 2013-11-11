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
* Description:  This file defines the interface which the trigger stores must
*                implement.
*
*/


#ifndef M_LISTTRIGGER_OBSERVER_H
#define M_LISTTRIGGER_OBSERVER_H







/**
 *  This M class defines the observer interface for the list trigger 
 *  asynchronous operation.
 *
 *  
 */
class MLbtContainerOpObserver
    {

public:
   

public: // Methods of the interface.
    /**
     * 
     *
     * 
     */
    virtual void ListTriggerOpComplete(TInt aErr) = 0;
    
    virtual void UpdateOpComplete(TInt aErr) = 0;
	 /**
     * pure virtual destructor
     */
     virtual ~MLbtContainerOpObserver() {};
    };


#endif // M_LISTTRIGGER_OBSERVER_H
