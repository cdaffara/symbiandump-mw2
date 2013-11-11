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
* Description:  Data structure containing positioning and address info
*
*/

#include "posgeoaddress.h"
#include "posgeoaddress_p.h"
#include "posindicatorlogger.h"


// ---------------------------------------------------------------------------
// PosGeoAddress::PosGeoAddress()
// ---------------------------------------------------------------------------
PosGeoAddress::PosGeoAddress()
    {
    d_ptr = new PosGeoAddressPrivate;	
    }	

// ---------------------------------------------------------------------------
// PosGeoAddress::~PosGeoAddress()
// ---------------------------------------------------------------------------
PosGeoAddress::~PosGeoAddress()
    {
    delete d_ptr;	
    }
// ---------------------------------------------------------------------------
// PosGeoAddress::setLatitude()
// ---------------------------------------------------------------------------
void PosGeoAddress::setLatitude( double value )
    {
    FUNC("+ PosGeoAddress::setLatitude");
    d_ptr->mLatitude = value;
    FUNC("- PosGeoAddress::setLatitude");
    }

// ---------------------------------------------------------------------------
// PosGeoAddress::setLatitude()
// ---------------------------------------------------------------------------
double PosGeoAddress::latitude() const 
    {
    FUNC("+ PosGeoAddress::latitude");
    return d_ptr->mLatitude; 
    }

// ---------------------------------------------------------------------------
// PosGeoAddress::setLatitude()
// ---------------------------------------------------------------------------
void PosGeoAddress::setLongitude(double value)
    { 
    FUNC("+ PosGeoAddress::setLongitude");
    d_ptr->mLongitude = value; 
    FUNC("- PosGeoAddress::setLongitude");
    }

// ---------------------------------------------------------------------------
// PosGeoAddress::setLatitude()
// ---------------------------------------------------------------------------
double PosGeoAddress::longitude() const 
    {
    FUNC("+ PosGeoAddress::longitude");
    return d_ptr->mLongitude; 
    }

// ---------------------------------------------------------------------------
// PosGeoAddress::setLatitude()
// ---------------------------------------------------------------------------
void PosGeoAddress::sethAccuracy(float value)
    {
    FUNC("+ PosGeoAddress::sethAccuracy");
    d_ptr->mHAccuracy = value; 
    FUNC("- PosGeoAddress::sethAccuracy");
    }

// ---------------------------------------------------------------------------
// PosGeoAddress::setLatitude()
// ---------------------------------------------------------------------------
float PosGeoAddress::hAccuracy() const 
    {
    FUNC("+ PosGeoAddress::hAccuracy");
    return d_ptr->mHAccuracy; 
    }

// ---------------------------------------------------------------------------
// PosGeoAddress::setLatitude()
// ---------------------------------------------------------------------------
void PosGeoAddress::setvAccuracy(float value)
    {
    FUNC("+ PosGeoAddress::setvAccuracy");
    d_ptr->mVAccuracy = value;
    FUNC("- PosGeoAddress::setvAccuracy");
    }

// ---------------------------------------------------------------------------
// PosGeoAddress::setLatitude()
// ---------------------------------------------------------------------------
float PosGeoAddress::vAccuracy() const 
    {
    FUNC("+ PosGeoAddress::vAccuracy");
    return d_ptr->mVAccuracy; 
    }

// ---------------------------------------------------------------------------
// PosGeoAddress::setLatitude()
// ---------------------------------------------------------------------------
void PosGeoAddress::setAltitude(float value)
    {
    FUNC("+ PosGeoAddress::setAltitude");
    d_ptr->mAltitude = value; 
    FUNC("- PosGeoAddress::setAltitude");
    }

// ---------------------------------------------------------------------------
// PosGeoAddress::setLatitude()
// ---------------------------------------------------------------------------
float PosGeoAddress::altitude() const 
    {
    FUNC("+ PosGeoAddress::altitude");
    return d_ptr->mAltitude; 
    }

// ---------------------------------------------------------------------------
// PosGeoAddress::setLatitude()
// ---------------------------------------------------------------------------
void PosGeoAddress::setCountry(QString value)
    {
    FUNC("+ PosGeoAddress::setCountry");
    d_ptr->mCountry = value; 
    FUNC("- PosGeoAddress::setCountry");
    }

// ---------------------------------------------------------------------------
// PosGeoAddress::setLatitude()
// ---------------------------------------------------------------------------
QString PosGeoAddress::country() const 
    {
    FUNC("+ PosGeoAddress::country");
    return d_ptr->mCountry; 
    }

// ---------------------------------------------------------------------------
// PosGeoAddress::setLatitude()
// ---------------------------------------------------------------------------
void PosGeoAddress::setState(QString value)
    {
    FUNC("+ PosGeoAddress::setState");
    d_ptr->mState = value; 
    FUNC("- PosGeoAddress::setState");
    }

// ---------------------------------------------------------------------------
// PosGeoAddress::setLatitude()
// ---------------------------------------------------------------------------
QString PosGeoAddress::state() const 
    {
    FUNC("+ PosGeoAddress::state");
    return d_ptr->mState; 
    }

// ---------------------------------------------------------------------------
// PosGeoAddress::setLatitude()
// ---------------------------------------------------------------------------
void PosGeoAddress::setDistrict(QString value)
    {
    FUNC("+ PosGeoAddress::setDistrict");
    d_ptr->mDistrict = value; 
    FUNC("- PosGeoAddress::setDistrict");
    }

// ---------------------------------------------------------------------------
// PosGeoAddress::setLatitude()
// ---------------------------------------------------------------------------
QString PosGeoAddress::district() const 
    {
    FUNC("+ PosGeoAddress::district");
    return d_ptr->mDistrict; 
    }

// ---------------------------------------------------------------------------
// PosGeoAddress::setLatitude()
// ---------------------------------------------------------------------------
void PosGeoAddress::setCity(QString value)
    {
    FUNC("+ PosGeoAddress::setCity");
    d_ptr->mCity = value; 
    FUNC("- PosGeoAddress::setCity");
    }

// ---------------------------------------------------------------------------
// PosGeoAddress::setLatitude()
// ---------------------------------------------------------------------------
QString PosGeoAddress::city() const 
    {
    FUNC("+ PosGeoAddress::city");
    return d_ptr->mCity; 
    }

// ---------------------------------------------------------------------------
// PosGeoAddress::setLatitude()
// ---------------------------------------------------------------------------
void PosGeoAddress::setZip(QString value)
    {
    FUNC("+ PosGeoAddress::setZip");
    d_ptr->mZip = value; 
    FUNC("- PosGeoAddress::setZip");
    }

// ---------------------------------------------------------------------------
// PosGeoAddress::setLatitude()
// ---------------------------------------------------------------------------
QString PosGeoAddress::zip() const 
    {
    FUNC("+ PosGeoAddress::zip");
    return d_ptr->mZip; 
    }

// ---------------------------------------------------------------------------
// PosGeoAddress::setLatitude()
// ---------------------------------------------------------------------------
void PosGeoAddress::setStreet(QString value)
    {
    FUNC("+ PosGeoAddress::setStreet");
    d_ptr->mStreet = value; 
    FUNC("- PosGeoAddress::setStreet");
    }

// ---------------------------------------------------------------------------
// PosGeoAddress::setLatitude()
// ---------------------------------------------------------------------------
QString PosGeoAddress::street() const 
    {
    FUNC("+ PosGeoAddress::street");
    return d_ptr->mStreet; 
    }

// ---------------------------------------------------------------------------
// PosGeoAddress::setLatitude()
// ---------------------------------------------------------------------------
void PosGeoAddress::setNumber(QString value)
    {
    FUNC("+ PosGeoAddress::setNumber");
    d_ptr->mNumber = value; 
    FUNC("- PosGeoAddress::setNumber");
    }

// ---------------------------------------------------------------------------
// PosGeoAddress::setLatitude()
// ---------------------------------------------------------------------------
QString PosGeoAddress::number() const 
    {
    FUNC("+ PosGeoAddress::number");
    return d_ptr->mNumber; 
    }
