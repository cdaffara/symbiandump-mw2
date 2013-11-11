/*
* Copyright (c) 2009 Nokia Corporation and/or its subsidiary(-ies).
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
* Description: Private implementation of PosGeoAddress class
*
*/

#ifndef POSGEOADDRESSPRIVATE_H
#define POSGEOADDRESSPRIVATE_H

#include "posgeoaddress.h"

#include <QObject.h>

/**
* Private implementation of PosGeoAddress
*/
class PosGeoAddressPrivate
    {

public :

    double  mLatitude;
    double  mLongitude;
    float   mHAccuracy;
    float   mVAccuracy;
    float   mAltitude;
    
    QString  mCountry;
    QString  mState;
    QString  mDistrict;
    QString  mCity;
    QString  mZip;
    QString  mStreet;
    QString  mNumber;    

    PosGeoAddress* q_ptr;

    Q_DECLARE_PUBLIC(PosGeoAddress);

    };
#endif /* POSGEOADDRESSPRIVATE_H */
