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
* Description: Positioning Indicators Info - QT Wrapper
*
*/

#ifndef POSITIONINDICATORINFO_H
#define POSITIONINDICATORINFO_H

#include <QObject.h>

#include "posgeoaddress.h"
#include "posindicatorglobal.h"

class PosIndicatorInfoPrivate;

class POSINDICATOR_EXPORT PosIndicatorInfo :  public QObject
    {    
    Q_OBJECT
    
public :
    /*
     * Constructor
     */
     PosIndicatorInfo();
    
    /*
     * Destructor
     */
     ~PosIndicatorInfo();
    
    /**
     * Requests for positioning information from Positioning Indicator 
     * Helper Server.PosInfoUpdated() will be emitted when
     * position information is available.
     *                   
     */
     void requestPosInfo(); 
     
     /**
      * Cancels position info request.
      */
     void cancelPosInfo();
	 
private:
    signals:
    /*
     * Signal emitted when new position updates available
     * When coordinates are available, this object is 
     * populated with only Position attribute. 
     * When reverse geo-code address has been resolved, 
     * the only address fields will be populated.
     * 
     * @param[in] posGeoAddress - Object containing position information
     * @param[in] error - Any error code while retrieving position information
     *                  - KTimeOut - if time out during retrieval of position information
     */
    void posInfoUpdated(PosGeoAddress& posGeoAddress, int error);    

private:    
    Q_DECLARE_PRIVATE_D(d_ptr, PosIndicatorInfo)   
    
private:
    /**
     * Pointer to private implementation.
     * Owns
     */
    PosIndicatorInfoPrivate* d_ptr;
    };

#endif /* POSITIONINDICATORINFO_H */
