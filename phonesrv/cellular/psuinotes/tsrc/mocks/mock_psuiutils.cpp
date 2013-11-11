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
* Description:
*
*/
#include <QDebug>
#include <smcmockclassincludes.h>
#include "psuiutils.h"

// ============================ MEMBER FUNCTIONS ===============================

// -----------------------------------------------------------------------------
// PsUiUtils::errorCodeTextMapping
// -----------------------------------------------------------------------------
//
bool PsUiUtils::errorCodeTextMapping( 
        const int errorcode,
        QString & errorText )
    {
    SMC_MOCK_METHOD2( bool, const int, errorcode, 
        QString &, errorText )
    }


// -----------------------------------------------------------------------------
// PsUiSettingsWrapper::PsUiSettingsWrapper
// -----------------------------------------------------------------------------
//
PsUiSettingsWrapper::PsUiSettingsWrapper( 
        QObject * parent )
    //:
    //QObject( /*parent*/ )
    {
    
    }


// -----------------------------------------------------------------------------
// PsUiSettingsWrapper::~PsUiSettingsWrapper
// -----------------------------------------------------------------------------
//
PsUiSettingsWrapper::~PsUiSettingsWrapper(  )
    {
    
    }


// -----------------------------------------------------------------------------
// PsUiSettingsWrapper::isFeatureCallWaitingDistiquishNotProvisionedEnabled
// -----------------------------------------------------------------------------
//
bool PsUiSettingsWrapper::isFeatureCallWaitingDistiquishNotProvisionedEnabled(  )
    {
    SMC_MOCK_METHOD0( bool )
    }


// -----------------------------------------------------------------------------
// PsUiSettingsWrapper::isPhoneOffline
// -----------------------------------------------------------------------------
//
bool PsUiSettingsWrapper::isPhoneOffline(  ) const
    {
    SMC_MOCK_METHOD0( bool )
    }


// -----------------------------------------------------------------------------
// PsUiSettingsWrapper::numberGroupingSupported
// -----------------------------------------------------------------------------
//
bool PsUiSettingsWrapper::numberGroupingSupported(  ) const
    {
    SMC_MOCK_METHOD0( bool )
    }


