/*
* Copyright (c) 2002-2007 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:  Provides functions to read and write from/to long text columns.
*
*/


#ifndef POSLMLONGTEXTCOLHANDLER_H
#define POSLMLONGTEXTCOLHANDLER_H

#include <d32dbms.h>

/**
* Provides functions to read and write from/to long text columns.
*/
class PosLmLongTextColHandler
    {

    public:  // New functions

        /**
        * Reads the data of a long text column.
        *
        * @param[out] aLongText A buffer of proper size for the specified column.
        * @param[in] aView Prepared view.
        * @param[in] aCol The number of the column to read from.
        */
        IMPORT_C static void ReadFromLongTextColumnL(
            TDes& aLongText,
            RDbRowSet& aView,
            TDbColNo aCol
        );

        /**
        * Writes the specified text to a long text column.
        *
        * @param[in] aLongText The text to write.
        * @param[in] aView Prepared view.
        * @param[in] aCol The number of the column to write to.
        */
        IMPORT_C static void WriteToLongTextColumnL(
            const TDesC& aLongText,
            RDbRowSet& aView,
            TDbColNo aCol
        );

        /**
        * Reads the data of a long text column.
        *
        * @param[in] aView Prepared view.
        * @param[in] aCol The number of the column to read from.
        * @return Data from the column.
        */
        IMPORT_C static HBufC* ReadFromLongTextColumnLC(
            RDbRowSet& aView,
            TDbColNo aCol );
    };

#endif      // POSLMLONGTEXTCOLHANDLER_H
