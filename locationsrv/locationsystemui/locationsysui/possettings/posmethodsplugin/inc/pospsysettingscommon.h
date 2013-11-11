/*
 * Copyright (c) 2010 Nokia Corporation and/or its subsidiary(-ies).
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
 * Description: Contains the common data structures & Enum
 *
 */

#ifndef POSPSYSETTINGSCOMMON_H
#define POSPSYSETTINGSCOMMON_H

#include <e32cmn.h>
#include <qstring.h>

/**
 * Enumeration to indicate the state of the Psy
 */
enum PosPsyState
    {
    PsyEnable,

    PsyDisable
    };
/**
 * Struct that contains all the info related to each Psy
 * - uid of Psy
 * - name
 * - state of the Psy whether enabled/disabled
 */
struct PosPsyInfo
    {
    TUid mPsyModuelId;

    QString mPsyName;

    PosPsyState mPsyState;
    };

#endif /* POSPSYSETTINGSCOMMON_H */

