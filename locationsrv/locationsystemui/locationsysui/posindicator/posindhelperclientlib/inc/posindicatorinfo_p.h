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
* Description: Positioning Indicators Info private implementation
*
*/

#ifndef POSITIONINDICATORINFOPRIVATE_H
#define POSITIONINDICATORINFOPRIVATE_H

#include "posindicatorinfo.h"
#include "posindinfochangeobserver.h"
#include "posindicatorhelperserver.h"
#include "posindicatorhelper.h"

class PosGeoAddress;

class PosIndicatorInfoPrivate : public MPosIndInfoChangeObserver
    {
public :
    /**
     *  Constructor
     */
    PosIndicatorInfoPrivate(PosIndicatorInfo *aPositionIndicator);
    
    /**
     * Destructor 
     */
    ~PosIndicatorInfoPrivate();

    /**
     * Requests for positioning information from Positioning Indicator 
     * Helper Server.NotifyPosInfoChange() will be called when
     * position information is available.
     *                   
     */
   void RequestPosInfo(); 
   
   /**
    * Cancels positon info request.
    */
   void CancelPosInfo();

   /**
    *  Derived from MPosIndInfoChangeObserver
    */
    void NotifyPosInfoChange(CPosLandmark* aLandmark, TInt aErrCode);
    
private:
    void AddToAddress(TPositionFieldId aFieldId, TPtrC aValue);
    
private:
        
    // Handle to current server session
    RPosIndicatorHelperServer    mPosIndicatorHelperServer;

    // Handle to current subsession
    RPosIndicatorHelper         mPosIndicatorHelper;    

    /**
     * It contains the latest position information     
     * Owns  
     */
    PosGeoAddress*             mPosGeoAddress;
    
    /*
     * Handler to PosIndicatorInfo.
     * It is required to emit PosInfoUpdated() signal
     *     
     * 
     */
    PosIndicatorInfo*          q_ptr;
    
    Q_DECLARE_PUBLIC(PosIndicatorInfo);
    };


#endif /* POSITIONINDICATORINFOPRIVATE_H */
