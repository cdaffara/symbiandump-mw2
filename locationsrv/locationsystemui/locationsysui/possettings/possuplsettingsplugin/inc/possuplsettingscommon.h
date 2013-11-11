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
 * Description: This is a common header for SUPL Positioning Settings plugin 
 *							for Advanced View
 *
 */

#ifndef POSSUPLSETTINGS_COMMON_H
#define POSSUPLSETTINGS_COMMON_H

#include <qstring.h>

/**
 * Contains info related to each SUPL Servers
 * - SLP id
 * - Server Address
 * - Access Point
 * - Server Usage State(enabled/disabled)
 * - Sim Change Remove (Yes/No)
 * - Usage is Home Network(Yes/No)
 * - editable (Yes/No)
 */
class PosServerParams
    {
public:
    long slpId;
    QString serverAdd;
    QString iap;
    bool serverEnabled;
    };

#endif //POSSUPLSETTINGS_COMMON_H
