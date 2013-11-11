/*
* Copyright (c) 2010 Nokia Corporation and/or its subsidiary(-ies).
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
* Description: Positioning Indicator Info Change Observer
*
*/

#ifndef POSIND_INFOCHANGEOBSERVER_H
#define POSIND_INFOCHANGEOBSERVER_H

#include <EPos_CPosLandmark.h>

class MPosIndInfoChangeObserver
    {
public:
    /*
     * It will be used to notify client that position information is changed
     * and new information is available.
     * 
     * Landmark object contains coordinate information when location fix is available
     * When reverse geo-code information is available, it contains address info.
     * 
     * @param[out] aLandmark - landmark object containing new position information
     * Ownership is NOT transferred to the observer.
     * @param[out] aErrCode - Error code if operation is failed
     *                        KErrNone if operation is successful.
     */
    virtual void NotifyPosInfoChange(CPosLandmark* aLandmark, TInt aErrCode) = 0;    
    };

#endif /* POSIND_INFOCHANGEOBSERVER_H */
