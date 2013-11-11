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


#ifndef CDESCRIPTIONUTILITY_H
#define CDESCRIPTIONUTILITY_H

class CMTPTypeObjectPropDesc;
class CMTPTypeInterdependentPropDesc;
class MMmMtpDpConfig;

class CDescriptionUtility : public CBase
    {
protected:
    /**
     *
     */
    IMPORT_C CDescriptionUtility();

public:
    /**
     *
     */
    IMPORT_C virtual ~CDescriptionUtility();

public:
    /**
     *
     */
    CMTPTypeObjectPropDesc* NewCommonObjectPropertyL( TUint16 aPropCode );

    /**
     * New RangeForm Description
     * @param aMinValue The minimal value
     * @param aMaxValue The maximal value
     * @param aStepValue The step value
     * @return CMTPTypeObjectPropDesc
     */
    IMPORT_C CMTPTypeObjectPropDesc* NewRangeFormDescriptionL( TUint16 aPropCode,
        TUint32 aMinValue,
        TUint32 aMaxValue,
        TUint32 aStepValue,
        TBool aIsReadOnly = EFalse );

    /**
     * Get group code
     * @param aPropCode Specify the property code of which the group code needed
     * @return Group code of specified property
     */
    TUint32 GetGroupCode( TUint32 aPropCode );

    /**
     *
     */
    virtual CMTPTypeInterdependentPropDesc* NewInterdepentPropDescL( MMmMtpDpConfig& aDpConfig, TUint aFormatCode ) = 0;

    /**
     *
     */
    virtual CMTPTypeObjectPropDesc* NewSpecificPropDescL( TUint aFormatCode, TUint16 aPropCode ) = 0;

private:
    /**
     * New a property description of protection status
     * @return CMTPTypeObjectPropDesc
     */
    CMTPTypeObjectPropDesc* NewProtectionStatusPropDescL();

    /**
     * Get property description of file name
     * @return CMTPTypeObjectPropDesc
     */
    CMTPTypeObjectPropDesc* NewFileNamePropDescL();

    /**
     * New a property description of nonconsumable
     * @return CMTPTypeObjectPropDesc
     */
    CMTPTypeObjectPropDesc* NewNonConsumablePropDescL();

    };

#endif // CDESCRIPTIONUTILITY_H
