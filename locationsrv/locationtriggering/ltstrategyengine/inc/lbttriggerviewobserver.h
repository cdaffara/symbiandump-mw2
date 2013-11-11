/*
* Copyright (c) 2006, 2007 Nokia Corporation and/or its subsidiary(-ies).
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
* Description:  Class definition of Trigger View Observer
*
*/



#ifndef M_LBTTRIGGERVIEWOBSERVER_H
#define M_LBTTRIGGERVIEWOBSERVER_H


/**
 * Observer class used to report triggerview refresh completion events. This
 * interface is implemented by CLbtStrategyEngine.
 */
class MLbtTriggerViewObserver

    {
    
public:

    enum TOperation
        
        {
        
        EViewRefreshed = 1,
        
        EViewInitialized,
        
        EOperationError
        
        };

public:

    virtual void OperationComplete( TOperation aOperation ) = 0;
    
    };

#endif  // M_LBTTRIGGERVIEWOBSERVER_H
