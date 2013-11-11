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

#include <mtp/cmtptypeinterdependentpropdesc.h>
#include <mtp/cmtptypeobjectpropdesc.h>
#include <mtp/cmtptypestring.h>

#include "cdescriptionutility.h"
#include "tobjectdescription.h"

#include "mmmtpdplogger.h"


_LIT( KMtpObjDescObjFileName,
    "[a-zA-Z!#\\$%&'\\(\\)\\-0-9@\\^_\\`\\{\\}\\~][a-zA-Z!#\\$%&'\\(\\)\\-0-9@\\^_\\`\\{\\}\\~ ]{0, 7}\\.[[a-zA-Z!#\\$%&'\\(\\)\\-0-9@\\^_\\`\\{\\}\\~][a-zA-Z!#\\$%&'\\(\\)\\-0-9@\\^_\\`\\{\\}\\~ ]{0, 2}]?" );
// RegEx is [a-zA-Z!#\$%&'\(\)\-0-9@\^_\`\{\}\~][a-zA-Z!#\$%&'\(\)\-0-9@\^_\`\{\}\~ ]{0, 7}\.[[a-zA-Z!#\$%&'\(\)\-0-9@\^_\`\{\}\~][a-zA-Z!#\$%&'\(\)\-0-9@\^_\`\{\}\~ ]{0, 2}]?

EXPORT_C CDescriptionUtility::CDescriptionUtility()
    {
    // Do nothing
    }

EXPORT_C CDescriptionUtility::~CDescriptionUtility()
    {
    // Do nothing
    }

CMTPTypeObjectPropDesc* CDescriptionUtility::NewCommonObjectPropertyL( TUint16 aPropCode )
    {
    CMTPTypeObjectPropDesc* propertyDesc = NULL;

    switch ( aPropCode )
        {
        case EMTPObjectPropCodeStorageID:
        case EMTPObjectPropCodeObjectFormat:
        case EMTPObjectPropCodeObjectSize:
        case EMTPObjectPropCodeParentObject:
        case EMTPObjectPropCodePersistentUniqueObjectIdentifier:
        case EMTPObjectPropCodeName:
        case EMTPObjectPropCodeDateAdded:
        case EMTPObjectPropCodeAlbumArtist:
            propertyDesc = CMTPTypeObjectPropDesc::NewL( aPropCode );
            break;

            // Protection Status (m)
        case EMTPObjectPropCodeProtectionStatus:
            propertyDesc = NewProtectionStatusPropDescL();
            break;

            // FileName
        case EMTPObjectPropCodeObjectFileName:
            propertyDesc = NewFileNamePropDescL();
            break;

            // Consumable (m)
        case EMTPObjectPropCodeNonConsumable:
            propertyDesc = NewNonConsumablePropDescL();
            break;

        case EMTPObjectPropCodeDateModified: // Date Modified
        case EMTPObjectPropCodeDateCreated: // Date Created
            {
            CMTPTypeObjectPropDesc::TPropertyInfo propertyInfo;
            propertyInfo.iDataType = EMTPTypeString;
            propertyInfo.iFormFlag = CMTPTypeObjectPropDesc::EDateTimeForm;
            propertyInfo.iGetSet = CMTPTypeObjectPropDesc::EReadOnly;
            propertyDesc = CMTPTypeObjectPropDesc::NewL( aPropCode,
                propertyInfo,
                NULL );
            }
            break;

        default:
            break;
        }
    return propertyDesc;
    }

// -----------------------------------------------------------------------------
// CDescriptionUtility::NewRangeFormDescriptionL
//
// -----------------------------------------------------------------------------
//
EXPORT_C CMTPTypeObjectPropDesc* CDescriptionUtility::NewRangeFormDescriptionL( TUint16 aPropertyCode,
    TUint32 aMinValue,
    TUint32 aMaxValue,
    TUint32 aStepValue,
    TBool aIsReadOnly )
    {
    PRINT1( _L( "MM MTP => CDescriptionUtility::NewRangeFormDescriptionL, aPropertyCode = 0x%x" ),
        aPropertyCode );
    CMTPTypeObjectPropDescRangeForm* form =
        CMTPTypeObjectPropDescRangeForm::NewLC( EMTPTypeUINT32 ); // + form

    // Set expected values
    form->SetUint32L( CMTPTypeObjectPropDescRangeForm::EMinimumValue, aMinValue );
    form->SetUint32L( CMTPTypeObjectPropDescRangeForm::EMaximumValue, aMaxValue );
    form->SetUint32L( CMTPTypeObjectPropDescRangeForm::EStepSize, aStepValue );

    CMTPTypeObjectPropDesc::TPropertyInfo propertyInfo;
    propertyInfo.iDataType = EMTPTypeUINT32;
    propertyInfo.iFormFlag = CMTPTypeObjectPropDesc::ERangeForm;
    propertyInfo.iGetSet = !aIsReadOnly;

    CMTPTypeObjectPropDesc* propertyDesc = CMTPTypeObjectPropDesc::NewL( aPropertyCode,
        propertyInfo,
        form );    // + propertyDesc

    CleanupStack::PopAndDestroy( form ); // - form
    PRINT( _L( "MM MTP <= CDescriptionUtility::NewRangeFormDescriptionL" ) );

    return propertyDesc;
    }

// -----------------------------------------------------------------------------
// CDescriptionUtility::GetGroupCode
// Get MTPTypeObjectPropDesc according to property code
// -----------------------------------------------------------------------------
//
TUint32 CDescriptionUtility::GetGroupCode( TUint32 aPropCode )
    {
    TInt count = sizeof( KPropGroupMapTable ) / sizeof( KPropGroupMapTable[0] );

    for( TInt i = 0; i < count; i++ )
        {
        if ( aPropCode == KPropGroupMapTable[i].iPropCode )
            return KPropGroupMapTable[i].iGroupCode;
        }
    return EGroupCodeNotDefined;
    }

// -----------------------------------------------------------------------------
// CDescriptionUtility::NewProtectionStatusPropDescL
// New MTPTypeObjectPropDesc protection status
// -----------------------------------------------------------------------------
//
CMTPTypeObjectPropDesc* CDescriptionUtility::NewProtectionStatusPropDescL()
    {
    PRINT( _L( "MM MTP => CDescriptionUtility::NewProtectionStatusPropDescL" ) );

    CMTPTypeObjectPropDescEnumerationForm* expectedForm =
        CMTPTypeObjectPropDescEnumerationForm::NewLC( EMTPTypeUINT16 ); // + expectedForm

    TUint16 values[] =
        {
        EMTPProtectionNoProtection,
        EMTPProtectionReadOnly
        };

    TInt numValues = sizeof ( values ) / sizeof ( values[0] ) ;
    for ( TInt i = 0; i < numValues; i++ )
        {
        TMTPTypeUint16 data( values[i] );
        expectedForm->AppendSupportedValueL( data );
        }

    CMTPTypeObjectPropDesc* propertyDesc  = CMTPTypeObjectPropDesc::NewL( EMTPObjectPropCodeProtectionStatus, *expectedForm );
    CleanupStack::PopAndDestroy( expectedForm ); // - expectedForm

    PRINT( _L( "MM MTP <= CDescriptionUtility::NewProtectionStatusPropDescL" ) );
    return propertyDesc;
    }

// -----------------------------------------------------------------------------
// CDescriptionUtility::NewFileNamePropDescL
// Get MTPTypeObjectPropDesc New file name
// -----------------------------------------------------------------------------
//
CMTPTypeObjectPropDesc* CDescriptionUtility::NewFileNamePropDescL()
    {
    PRINT( _L( "MM MTP => CDescriptionUtility::NewFileNamePropDescL" ) );
    CMTPTypeString* form = CMTPTypeString::NewLC( KMtpObjDescObjFileName ); // + form

    CMTPTypeObjectPropDesc* ret = CMTPTypeObjectPropDesc::NewL( EMTPObjectPropCodeObjectFileName,
        CMTPTypeObjectPropDesc::ERegularExpressionForm,
        form );

    CleanupStack::PopAndDestroy( form ); // - form
    PRINT( _L( "MM MTP <= CDescriptionUtility::NewFileNamePropDescL" ) );
    return ret;
    }

// -----------------------------------------------------------------------------
// CDescriptionUtility::NewFileNamePropDescL
// Get MTPTypeObjectPropDesc New file name
// -----------------------------------------------------------------------------
//
CMTPTypeObjectPropDesc* CDescriptionUtility::NewNonConsumablePropDescL()
    {
    PRINT( _L( "MM MTP => CDescriptionUtility::NewNonConsumablePropDescL" ) );
    CMTPTypeObjectPropDescEnumerationForm* expectedForm =
        CMTPTypeObjectPropDescEnumerationForm::NewLC( EMTPTypeUINT8 ); // + expectedForm
    TUint8 values[] =
        {
        EMTPConsumable,
        EMTPNonConsumable
        };

    TInt numValues = sizeof ( values ) / sizeof ( values[0] );
    for ( TInt i = 0; i < numValues; i++ )
        {
        TMTPTypeUint8 data( values[i] );
        expectedForm->AppendSupportedValueL( data );
        }

    CMTPTypeObjectPropDesc* ret = CMTPTypeObjectPropDesc::NewL( EMTPObjectPropCodeNonConsumable,
        CMTPTypeObjectPropDesc::EEnumerationForm,
        expectedForm );
    CleanupStack::PopAndDestroy( expectedForm ); // - expectedForm

    PRINT( _L( "MM MTP <= CDescriptionUtility::NewNonConsumablePropDescL" ) );
    return ret;
    }

// end of file
