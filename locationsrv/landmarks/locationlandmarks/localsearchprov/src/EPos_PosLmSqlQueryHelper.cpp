/*
* Copyright (c) 2005 Nokia Corporation and/or its subsidiary(-ies). 
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
*    This is a helper class for calculating progress during a search operation.
*
*/


// INCLUDE FILES
#include <epos_landmarkdatabasestructure.h>
#include <epos_poslmtrealcompare.h>
#include "EPos_PosLmSqlQueryHelper.h"

// CONSTANTS
const TReal64 KPosMinLongitude = -180;
const TReal64 KPosMaxEastLongitude = 180;

// ============================ MEMBER FUNCTIONS ===============================

// -----------------------------------------------------------------------------
// PosLmSqlQueryHelper::AppendAreaCriteriaToQueryL
//
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void PosLmSqlQueryHelper::AppendAreaCriteriaToQueryL(
    HBufC* aDbQuery,
    TReal64 aSouthLatitude,
    TReal64 aNorthLatitude,
    TReal64 aWestLongitude,
    TReal64 aEastLongitude)
    {
    _LIT(KPosLmSqlWhere, "WHERE");
    _LIT(KPosLmSqlAndOp, "AND");
    _LIT(KPosLmSqlOrOp, "OR");
    _LIT(KPosLmSqlGeOp, ">=");
    _LIT(KPosLmSqlLeOp, "<=");
    _LIT(KPosLmLeftPar, "(");
    _LIT(KPosLmRightPar, ")");
    _LIT(KPosLmSpace, " ");

    TRealFormat sqlRealFormat;
    // Disable character between 1 and 000 in 1000
    sqlRealFormat.iTriLen = 0;
    // Sql want integer value and decimal value separated by '.'
    sqlRealFormat.iPoint = '.';

    aDbQuery->Des().Append(KPosLmSpace);
    aDbQuery->Des().Append(KPosLmSqlWhere);
    aDbQuery->Des().Append(KPosLmSpace);

    // Latitude
    // Min latitude
    aDbQuery->Des().Append(KPosLmLatitudeCol);
    aDbQuery->Des().Append(KPosLmSqlGeOp);
    User::LeaveIfError(
        aDbQuery->Des().AppendNum(aSouthLatitude, sqlRealFormat));
    aDbQuery->Des().Append(KPosLmSpace);
    aDbQuery->Des().Append(KPosLmSqlAndOp);
    aDbQuery->Des().Append(KPosLmSpace);

    // Max latitude
    aDbQuery->Des().Append(KPosLmLatitudeCol);
    aDbQuery->Des().Append(KPosLmSqlLeOp);
    User::LeaveIfError(
        aDbQuery->Des().AppendNum(aNorthLatitude, sqlRealFormat));

    if (!PosLmTrealCompare::IsEqual(aWestLongitude, KPosMinLongitude) ||
        !PosLmTrealCompare::IsEqual(aEastLongitude, KPosMaxEastLongitude))
        {
        if (PosLmTrealCompare::IsEqual(aEastLongitude, KPosMaxEastLongitude))
            {
            aEastLongitude = KPosMinLongitude;
            }

        TBuf<3> lmSqlLongitudeOp(KPosLmSqlAndOp);
        TBool crossMeridian = EFalse;

        if (aEastLongitude < aWestLongitude)
            {
            crossMeridian = ETrue;
            lmSqlLongitudeOp = KPosLmSqlOrOp;
            }

        aDbQuery->Des().Append(KPosLmSpace);
        aDbQuery->Des().Append(KPosLmSqlAndOp);
        aDbQuery->Des().Append(KPosLmSpace);

        // Longitude
        if (crossMeridian)
            {
            aDbQuery->Des().Append(KPosLmLeftPar);
            }

        // Min longitude
        aDbQuery->Des().Append(KPosLmLongitudeCol);
        aDbQuery->Des().Append(KPosLmSqlGeOp);
        User::LeaveIfError(
            aDbQuery->Des().AppendNum(aWestLongitude, sqlRealFormat));
        aDbQuery->Des().Append(KPosLmSpace);
        aDbQuery->Des().Append(lmSqlLongitudeOp);
        aDbQuery->Des().Append(KPosLmSpace);

        // Max longitude
        aDbQuery->Des().Append(KPosLmLongitudeCol);
        aDbQuery->Des().Append(KPosLmSqlLeOp);
        User::LeaveIfError(
            aDbQuery->Des().AppendNum(aEastLongitude, sqlRealFormat));

        if (crossMeridian)
            {
            aDbQuery->Des().Append(KPosLmRightPar);
            }
        }

    }

// End of File
