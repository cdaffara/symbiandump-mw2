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
* Description:  Constant values for the Phone Setting UI Notes
*
*/

#ifndef CPSUICONSTANTS_H
#define CPSUICONSTANTS_H
 
const TInt KPSUIMaxBscNumber = 13;     //max. number of basic service groups
const TInt KPSUIMaxPSUIItemLength = 32;//max. length of text in PSUI note

// Status of Calling Identity.
enum TPsuiCli
    {
    EPsuiClirOn = 1,
    EPsuiClirOff,
    EPsuiClipOn,
    EPsuiClipOff,
    EPsuiColrOn,
    EPsuiColrOff,
    EPsuiColpOn,
    EPsuiColpOff,
    EPsuiCliUnknown
    };

#endif //CPSUICONSTANTS_H


