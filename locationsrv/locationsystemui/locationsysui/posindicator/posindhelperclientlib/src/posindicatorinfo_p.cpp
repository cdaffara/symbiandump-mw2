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
* Description: Implementation of position indicator private class.
*
*/

#include "posindicatorinfo_p.h"
#include "posgeoaddress.h"
#include "posindicatorlogger.h"

// ---------------------------------------------------------------------------
// PosIndicatorInfoPrivate::PosIndicatorInfoPrivate()
// ---------------------------------------------------------------------------
PosIndicatorInfoPrivate::PosIndicatorInfoPrivate( PosIndicatorInfo *aPositionIndicator )
            :             q_ptr(aPositionIndicator)                        
    {
    FUNC("+ PosIndicatorInfoPrivate::PosIndicatorInfoPrivate");
    QT_TRAP_THROWING (
    qt_symbian_throwIfError(mPosIndicatorHelperServer.Connect());    
    qt_symbian_throwIfError(mPosIndicatorHelper.Open(mPosIndicatorHelperServer, this));    
    mPosGeoAddress = NULL;    
    )
    FUNC("- PosIndicatorInfoPrivate::PosIndicatorInfoPrivate");
    }

// ---------------------------------------------------------------------------
// PosIndicatorInfoPrivate::~PosIndicatorInfoPrivate()
// ---------------------------------------------------------------------------
PosIndicatorInfoPrivate::~PosIndicatorInfoPrivate()
    {       
    FUNC("+ PosIndicatorInfoPrivate::~PosIndicatorInfoPrivate");
    if(mPosGeoAddress)
      {
      delete mPosGeoAddress;
      mPosGeoAddress = NULL;
      } 
    mPosIndicatorHelper.CancelCurrentRequest();
    mPosIndicatorHelper.Close();
    mPosIndicatorHelperServer.Close();    
    FUNC("- PosIndicatorInfoPrivate::~PosIndicatorInfoPrivate");
    }

// ---------------------------------------------------------------------------
// PosIndicatorInfoPrivate::RequestPosInfo()
// ---------------------------------------------------------------------------
void PosIndicatorInfoPrivate::RequestPosInfo()
    {    
    FUNC("+ PosIndicatorInfoPrivate::RequestPosInfo");
    mPosIndicatorHelper.RequestPosInfo();
    FUNC("- PosIndicatorInfoPrivate::RequestPosInfo");
    }

// ---------------------------------------------------------------------------
// PosIndicatorInfoPrivate::CancelPosInfo()
// ---------------------------------------------------------------------------
void PosIndicatorInfoPrivate::CancelPosInfo()
    {    
    FUNC("+ PosIndicatorInfoPrivate::CancelPosInfo");
    mPosIndicatorHelper.CancelCurrentRequest();
    FUNC("- PosIndicatorInfoPrivate::CancelPosInfo");
    }

// ---------------------------------------------------------------------------
// PosIndicatorInfoPrivate::NotifyPosInfoChange()
// ---------------------------------------------------------------------------
void PosIndicatorInfoPrivate::NotifyPosInfoChange(CPosLandmark* aLandmark, TInt aErrCode)
    {
   FUNC("+ PosIndicatorInfoPrivate::NotifyPosInfoChange");
   
    if(mPosGeoAddress)
      {
      delete mPosGeoAddress;
      mPosGeoAddress = NULL;
      }
    mPosGeoAddress = new PosGeoAddress();
    
   if ( aErrCode != KErrNone )
       {
       LOG("Error in NotifyPosInfoChange = ",aErrCode);
       emit q_ptr->posInfoUpdated(*mPosGeoAddress , aErrCode );
       return;
       }
    
    // Coordinate info
    TLocality locality;
    TInt err = aLandmark->GetPosition(locality);
    
    if (err != KErrNone)
        {
        emit q_ptr->posInfoUpdated( *mPosGeoAddress, err );
        FUNC("Unable to get position, exitting with error");
        return;
        }
    else
        {
        mPosGeoAddress->setLatitude(locality.Latitude());
        mPosGeoAddress->setLongitude(locality.Longitude());
        mPosGeoAddress->sethAccuracy(locality.HorizontalAccuracy());
        mPosGeoAddress->setvAccuracy(locality.VerticalAccuracy());    
        }
    
    //reverse geo-code info
    TUint numOfPositionFields = aLandmark->NumOfAvailablePositionFields();
    if (numOfPositionFields>0)
        {
        FUNC("Retrieving Address");

        TPositionFieldId fieldId;
        TPtrC value;
        
        // Read fields input by reverse geocoding and add to address
        FUNC("Reading First Field");

        fieldId = aLandmark->FirstPositionFieldId();
        aLandmark->GetPositionField( fieldId, value );
        AddToAddress(fieldId, value);
        FUNC("Reading Fields");

        for( int i = 0; i<numOfPositionFields ; ++i )
            {
            FUNC("Reading Fields");
            fieldId = aLandmark->NextPositionFieldId(fieldId);
            aLandmark->GetPositionField( fieldId, value );
            AddToAddress(fieldId, value);
            }
        }
    
    emit q_ptr->posInfoUpdated(*mPosGeoAddress , aErrCode );
    FUNC("- PosIndicatorInfoPrivate::NotifyPosInfoChange");
    }

void PosIndicatorInfoPrivate::AddToAddress(TPositionFieldId aFieldId, TPtrC aValue)
    {
    FUNC("+ PosIndicatorInfoPrivate::AddToAddress");
    switch(aFieldId)
        {
        case EPositionFieldCountry:
            {
            mPosGeoAddress->setCountry(QString::fromUtf16(aValue.Ptr(),
                    aValue.Length()));
                    break;
            }
        case EPositionFieldState:
            {
            mPosGeoAddress->setState(QString::fromUtf16(aValue.Ptr(),
                    aValue.Length()));
                    break;
            }
        case EPositionFieldDistrict:
            {
            mPosGeoAddress->setDistrict(QString::fromUtf16(aValue.Ptr(),
                    aValue.Length()));
                    break;
            }

        case EPositionFieldCity:
            {
            mPosGeoAddress->setCity(QString::fromUtf16(aValue.Ptr(),
                    aValue.Length()));
                    break;
            }
            
        case EPositionFieldPostalCode:
            {
            mPosGeoAddress->setZip(QString::fromUtf16(aValue.Ptr(),
                    aValue.Length()));
                    break;
            }
            
        case EPositionFieldStreet:
            {
           mPosGeoAddress->setStreet(QString::fromUtf16(aValue.Ptr(),
                   aValue.Length()));
                   break;
            }
                  
        case EPositionFieldStreetExtension:
            {
           mPosGeoAddress->setNumber(QString::fromUtf16(aValue.Ptr(),
                   aValue.Length()));
                   break;
            }    
            
        }
    FUNC("- PosIndicatorInfoPrivate::AddToAddress");
    }
