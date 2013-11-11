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

#include <mtp/cmtptypeobjectproplist.h>
#include <mtp/mtpprotocolconstants.h>
#include <mtp/mtpdatatypeconstants.h>
#include <mtp/cmtptypearray.h>

#include "cabstractmediamtpdataproviderpropertysettingutility.h"
#include "mmmtpdplogger.h"

// -----------------------------------------------------------------------------
// CAbstractMediaMtpDataProviderPropertySettingUtility::NewL
// Two-phase construction method
// -----------------------------------------------------------------------------
//
CAbstractMediaMtpDataProviderPropertySettingUtility* CAbstractMediaMtpDataProviderPropertySettingUtility::NewL()
    {
    CAbstractMediaMtpDataProviderPropertySettingUtility* self =
        new ( ELeave ) CAbstractMediaMtpDataProviderPropertySettingUtility();

    return self;
    }

// -----------------------------------------------------------------------------
// CAbstractMediaMtpDataProviderPropertySettingUtility::~CAbstractMediaMtpDataProviderPropertySettingUtility
// Destructor
// -----------------------------------------------------------------------------
//
CAbstractMediaMtpDataProviderPropertySettingUtility::~CAbstractMediaMtpDataProviderPropertySettingUtility()
    {
    // Do nothing
    }

// -----------------------------------------------------------------------------
// CAbstractMediaMtpDataProviderPropertySettingUtility::CAbstractMediaMtpDataProviderPropertySettingUtility
// Standard C++ Constructor
// -----------------------------------------------------------------------------
//
CAbstractMediaMtpDataProviderPropertySettingUtility::CAbstractMediaMtpDataProviderPropertySettingUtility()
    {
    // Do nothing
    }

TMTPResponseCode CAbstractMediaMtpDataProviderPropertySettingUtility::SetSpecificObjectPropertyL( MMmMtpDpConfig& aDpConfig,
    TUint16 aPropCode,
    const CMTPObjectMetaData& aObject,
    const CMTPTypeObjectPropListElement& aElement )
    {
    PRINT( _L( "MM MTP => CAbstractMediaMtpDataProviderPropertySettingUtility::SetSpecificObjectPropertyL" ) );

    TMTPResponseCode responseCode = EMTPRespCodeOK;

    switch ( aPropCode )
        {
        case EMTPObjectPropCodeRepresentativeSampleData:
            {
            CMTPTypeArray*  desData = CMTPTypeArray::NewLC( EMTPTypeAUINT8 );
            desData->SetByDesL( aElement.ArrayL( CMTPTypeObjectPropListElement::EValue ) );
            responseCode = SetMetaDataToWrapper( aDpConfig, aPropCode, *desData, aObject );
            CleanupStack::PopAndDestroy( desData );
            }
            break;

        case EMTPObjectPropCodeRepresentativeSampleFormat:
        case EMTPObjectPropCodeRepresentativeSampleSize: 
        case EMTPObjectPropCodeRepresentativeSampleHeight:
        case EMTPObjectPropCodeRepresentativeSampleWidth:
            // no place to store, do nothing. reserve here for future usage
            break;

        default:
            // Should not happen, property code should have been checked before set to store.
            User::Leave( KErrNotSupported );
        }

    PRINT( _L( "MM MTP <= CAbstractMediaMtpDataProviderPropertySettingUtility::SetSpecificObjectPropertyL" ) );
    return responseCode;
    }

// end of file
