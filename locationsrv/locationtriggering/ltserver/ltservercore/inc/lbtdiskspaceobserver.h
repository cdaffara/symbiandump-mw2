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
* Description:  Class declaration of CLbtDiskSpaceObserver
*
*/


#ifndef M_LBTDISKSPACEOBSERVER_H
#define M_LBTDISKSPACEOBSERVER_H

/**
 *  This is observer class for PSY change.This is implemented by
 *  CLbtCoordinateSupervisor.
 *
 */
class MLbtDiskSpaceObserver 
    {
public:
    /**
     * Handles PSY change event.Deriving class must implement this
     *
     */
    virtual void HandleDiskSpaceChange()=0; 
    };


#endif // M_LBTDISKSPACEOBSERVER_H
