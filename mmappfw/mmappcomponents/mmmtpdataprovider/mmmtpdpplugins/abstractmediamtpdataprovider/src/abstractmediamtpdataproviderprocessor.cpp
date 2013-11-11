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
* Description:  AbstractMedia Dp processor handle
*
*/


#include <mtp/tmtptyperequest.h>

//Include the standard request processor headers
#include "crequestunknown.h"

#include "abstractmediamtpdataproviderprocessor.h"

// Include the required header for carrying out the specified request
#include "mmmtpdplogger.h"

#include "abstractmediamtpdataproviderconst.h"
#include "cgetobjectpropssupported.h"
#include "cgetobjectpropdesc.h"
#include "cgetinterdependentpropdesc.h"
#include "cgetobject.h"
#include "cgetobjectinfo.h"
#include "csendobject.h"
#include "cabstractmediamtpdataprovidergetobjectpropvalue.h"
#include "cabstractmediamtpdataprovidergetobjectproplist.h"
#include "cabstractmediamtpdataprovidersetobjectpropvalue.h"
#include "csetobjectproplist.h"
#include "cgetobjectreferences.h"
#include "csetobjectreferences.h"
#include "cdeleteobject.h"
#include "cabstractmediamtpdataprovidercopyobject.h"
#include "cabstractmediamtpdataprovidermoveobject.h"
#include "cgetpartialobject.h"
#include "cabstractmediamtpdataproviderabstractmediapreference.h"
#include "cgetformatcapabilities.h"


/** A mapping table from the operation code to the request processor factory method */
const TAbstractMediaMtpDataProviderRequestProcessorEntry KAbstractMediaMtpDataProviderRequestProcessorTable[] =
    {
        { EMTPOpCodeGetObjectPropsSupported, CGetObjectPropsSupported::NewL }, //Get Supported Props
        { EMTPOpCodeGetObjectPropDesc, CGetObjectPropDesc::NewL }, //Get Props Description
        { EMTPOpCodeGetInterdependentPropDesc, CGetInterdependentPropDesc::NewL },
        { EMTPOpCodeGetObjectInfo, CGetObjectInfo::NewL }, //GetObjectInfo
        { EMTPOpCodeGetObject, CGetObject::NewL }, //GetObject
        { EMTPOpCodeGetObjectPropValue, CAbstractMediaMtpDataProviderGetObjectPropValue::NewL },
        { EMTPOpCodeGetObjectPropList, CAbstractMediaMtpDataProviderGetObjectPropList::NewL }, //GetObjectPropList
        { EMTPOpCodeSendObjectInfo, CSendObject::NewL }, //SendObjectInfo (routed to SendObject)
        { EMTPOpCodeSendObject, CSendObject::NewL }, //SendObject
        { EMTPOpCodeSendObjectPropList, CSendObject::NewL }, // SendobjectPropList (routed to SendObject)
        { EMTPOpCodeSetObjectPropValue, CAbstractMediaMtpDataProviderSetObjectPropValue::NewL },
        { EMTPOpCodeSetObjectPropList, CSetObjectPropList::NewL }, //SetObjectPropList
        { EMTPOpCodeGetObjectReferences, CGetObjectReferences::NewL }, //Get Object References
        { EMTPOpCodeSetObjectReferences, CSetObjectReferences::NewL }, //Set Object References
        { EMTPOpCodeDeleteObject, CDeleteObject::NewL }, //DeleteObject
        { EMTPOpCodeCopyObject, CAbstractMediaMtpDataProviderCopyObject::NewL }, // Copy Object
        { EMTPOpCodeMoveObject, CAbstractMediaMtpDataProviderMoveObject::NewL }, // Move Object
        { EMTPOpCodeGetPartialObject, CGetPartialObject::NewL }, //GetPartialObject
        { EMTPExtOpCodeAbstractMediaPreference, CAbstractMediaMtpDataProviderAbstractMediaPreference::NewL }, // WMPReportSyncOnlyAbstractMedias
        { EMTPOpCodeGetFormatCapabilities, CGetFormatCapabilities::NewL } // GetFormatCapabilities
    };

// -----------------------------------------------------------------------------
// AbstractMediaMtpDataProviderProcessor::CreateL
// Create a request processor that matches the request
// -----------------------------------------------------------------------------
//
MMmRequestProcessor* AbstractMediaMtpDataProviderProcessor::CreateL( MMTPDataProviderFramework& aFramework,
    const TMTPTypeRequest& aRequest,
    MMTPConnection& aConnection,
    MMmMtpDpConfig& aDpConfig )
    {
    PRINT( _L( "MM MTP => AbstractMediaMtpDataProviderProcessor::CreateL" ) );

    TAbstractMediaMtpDataProviderRequestProcessorCreateFunc createFunc = NULL;
    TUint16 operationCode = aRequest.Uint16(
            TMTPTypeRequest::ERequestOperationCode );

    TInt count = sizeof( KAbstractMediaMtpDataProviderRequestProcessorTable )
            / sizeof( TAbstractMediaMtpDataProviderRequestProcessorEntry );

    for ( TInt i = 0; i < count; i++ )
        {
        if ( KAbstractMediaMtpDataProviderRequestProcessorTable[i].iOperationCode == operationCode )
            {
            createFunc = KAbstractMediaMtpDataProviderRequestProcessorTable[i].iCreateFunc;
            break;
            }
        }

    if ( !createFunc )
        {
        return CRequestUnknown::NewL( aFramework, aConnection );
        }

    PRINT( _L( "MM MTP <= AbstractMediaMtpDataProviderProcessor::CreateL" ) );

    return ( *createFunc )( aFramework, aConnection, aDpConfig );
    }

// end of file
