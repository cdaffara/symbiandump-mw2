/*
* Copyright (c) 2007 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:  CMnrpCoordConverter class definition
*
*/


#ifndef MNRP_COORDINATECONVERTER_H
#define MNRP_COORDINATECONVERTER_H

#include <mncoordinateconverterbase.h>

class CMnrpEngine;
class CMnrpMapModel;

class CMnrpCoordConverter : public CMnCoordinateConverterBase
    {
    public:
        static CMnrpCoordConverter* NewL();

        ~CMnrpCoordConverter();

    protected: // From CMnCoordinateConverterBase

        TInt GetImageCoordinate(
            const TCoordinate& aWorldCoordinate,
            TPoint& aImagePoint );

        TInt GetWorldCoordinate(
            const TPoint& aImagePoint,
            TCoordinate& aWorldCoordinate );

    protected:
        CMnrpCoordConverter();

    private:

        CMnrpEngine*            iEngine;
        CMnrpMapModel*          iModel;
    };

#endif /*MNTP_COORDINATECONVERTER_H*/
