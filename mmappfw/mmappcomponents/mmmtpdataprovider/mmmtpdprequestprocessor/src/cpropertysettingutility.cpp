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
#include <mtp/cmtptypestring.h>
#include <mtp/cmtptypearray.h>
#include <mtp/tmtptypeuint8.h>
#include <mtp/tmtptypeuint16.h>
#include <mtp/tmtptypeuint32.h>
#include <mtp/mtpdatatypeconstants.h>

#include "cpropertysettingutility.h"
#include "cmmmtpdpmetadataaccesswrapper.h"
#include "mmmtpdpconfig.h"
#include "mmmtpdputility.h"
#include "mmmtpdplogger.h"

EXPORT_C CPropertySettingUtility::CPropertySettingUtility()
    {
    // Do nothing
    }

EXPORT_C CPropertySettingUtility::~CPropertySettingUtility()
    {
    // Do nothing
    }

// -----------------------------------------------------------------------------
// CPropertySettingUtility::SetMetaDataToWrapper
//
// -----------------------------------------------------------------------------
//
EXPORT_C TMTPResponseCode CPropertySettingUtility::SetMetaDataToWrapper( MMmMtpDpConfig& aDpConfig,
    const TUint16 aPropCode,
    MMTPType& aNewData,
    const CMTPObjectMetaData& aObjectMetaData )
    {
    TMTPResponseCode responseCode = EMTPRespCodeOK;
    TRAPD( err, aDpConfig.GetWrapperL().SetObjectMetadataValueL( aPropCode,
        aNewData,
        aObjectMetaData ) );

    PRINT1( _L("MM MTP <> CPropertySettingUtility::SetMetaDataToWrapper err = %d"), err);

    if ( err == KErrNone )
        responseCode = EMTPRespCodeOK;
    else // Other errors are not SetMetaDataToWrapper related, should be respond before this calling
        responseCode = EMTPRespCodeGeneralError;

    PRINT1( _L( "MM MTP <= CPropertySettingUtility::SetMetaDataToWrapper resCode = 0x%x" ), responseCode );

    return responseCode;
    }

// end of file
