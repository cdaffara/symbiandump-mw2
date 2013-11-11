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
* Description: Data structure containing positioning and address info
*
*/

#ifndef POSGEOADDRESS_H
#define POSGEOADDRESS_H

#include <QObject.h>

#include "posindicatorglobal.h"

class PosGeoAddressPrivate;

/**
* PosGeoAddress represents a Location information.
*/
class POSINDICATOR_EXPORT PosGeoAddress : public QObject
    {
      Q_OBJECT
      
      Q_PROPERTY(double latitude READ latitude WRITE setLatitude)
      Q_PROPERTY(double longitude READ longitude WRITE setLongitude)
      Q_PROPERTY(float hAccuracy READ hAccuracy WRITE sethAccuracy)
      Q_PROPERTY(float vAccuracy READ vAccuracy WRITE setvAccuracy)
      Q_PROPERTY(float altitude READ altitude WRITE setAltitude)
      
      Q_PROPERTY(QString country READ country WRITE setCountry)
      Q_PROPERTY(QString state READ state WRITE setState)
      Q_PROPERTY(QString city READ district WRITE setDistrict)
      Q_PROPERTY(QString city READ city WRITE setCity)
      Q_PROPERTY(QString zip READ zip WRITE setZip)
      Q_PROPERTY(QString street READ street WRITE setStreet)
      Q_PROPERTY(QString number READ number WRITE setNumber)
    
public:
	   PosGeoAddress();
    ~PosGeoAddress();
    void setLatitude( double value );
    double latitude() const;
    void setLongitude(double value);
    double longitude() const;
    void sethAccuracy(float value);
    float hAccuracy() const;
    void setvAccuracy(float value);
    float vAccuracy() const;
    void setAltitude(float value);
    float altitude() const ;
    void setCountry(QString value);
    QString country() const;
    void setState(QString value);
    QString state() const;
    void setDistrict(QString value);
    QString district() const ;
    void setCity(QString value);
    QString city() const;
    void setZip(QString value);
    QString zip() const;
    void setStreet(QString value);
    QString street() const;
    void setNumber(QString value);
    QString number() const;

private:    
    Q_DECLARE_PRIVATE_D(d_ptr, PosGeoAddress)   
    
private:
    /**
     * Pointer to private implementation.
     * Owns
     */
    PosGeoAddressPrivate* d_ptr;	
    };
#endif /* POSGEOADDRESS_H */
