/*
* Copyright (c) 2008 Nokia Corporation and/or its subsidiary(-ies).
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
* Description:  Class definition of Cell change Observer
*
*/


#ifndef M_LBTCELLCHANGEOBSERVER_H
#define M_LBTCELLCHANGEOBSERVER_H

/**
 * Observer interface for cell changes. This
 * interface is implemented by CLbtCellSupervisor.
 */
class MCellChangeObserver
    {
    public:
    
    /**
     * Handles cell change event. Deriving class must implement this.
     * 
     * @param aStatus status of event. KErrNone if everything is fine.
     * @param aNwInfo network information
     * @param aLocArea location information
     */
    virtual void HandleCellChangeEvent(
            const TInt aStatus,
            const RMobilePhone::TMobilePhoneNetworkInfoV1& aNwInfo,
            const RMobilePhone::TMobilePhoneLocationAreaV1& aLocArea) = 0;
    };
    
#endif //M_LBTCELLCHANGEOBSERVER_H  