/*
* Copyright (c) 2002 Nokia Corporation and/or its subsidiary(-ies).
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
* Description:   Const parameters for brightness/constrast enhancement
*                The tuning has 9 steps, each represented by 3 parameters.
*                The smaller indices in the table generate "lighter" pictures.
*
*/



#ifndef     __BRIGHTNESSCONTRAST_H__
#define     __BRIGHTNESSCONTRAST_H__


const TInt KMinBrightnessContrastIndex = 0;
const TInt KMedBrightnessContrastIndex = 10;
const TInt KMaxBrightnessContrastIndex = 20;

const TInt KMaxBCInputIndex = 100;

struct SEnhParam {
    TReal   a;
    TReal   b;
    TReal   g;
};

const struct SEnhParam KBrightnessContrastEnhParam[21] = {   
   {2.35, -0.1215, 0.5},
   {2.17, -0.0936, 0.55},
   {2, -0.07, 0.6},
   {1.84, -0.0504, 0.65},
   {1.69, -0.0345, 0.7},
   {1.55, -0.022, 0.75},
   {1.42, -0.0126, 0.8},
   {1.3, -0.006, 0.85},
   {1.19, -0.0019, 0.9},
   {1.09, 0, 0.95},
   {1, 0, 1},
   {1.04, -0.04, 1.05},
   {1.09, -0.0891, 1.1},
   {1.15, -0.147, 1.15},
   {1.22, -0.2134, 1.2},
   {1.3, -0.288, 1.25},
   {1.39, -0.3705, 1.3},
   {1.49, -0.4606, 1.35},
   {1.6, -0.558, 1.4},
   {1.72, -0.6624, 1.45},
   {1.85, -0.7735, 1.5}
};

#endif      //  __BRIGHTNESSCONTRAST_H__
            
// End of File
