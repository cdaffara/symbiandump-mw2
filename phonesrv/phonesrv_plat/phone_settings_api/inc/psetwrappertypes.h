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

#ifndef PSETWRAPPERTYPES_H
#define PSETWRAPPERTYPES_H

#include <qglobal.h>

const int maxBscNumber = 13;     //max. number of basic service groups
const int maxPsuiItemLength = 32;//max. length of text in PSUI note

// Status of Calling Identity.
enum PsCallLineIdentity
{
    ClirOn = 1,
    ClirOff,
    ClipOn,
    ClipOff,
    ColrOn,
    ColrOff,
    ColpOn,
    ColpOff,
    CliUnknown
};

//call forwarding statuses
enum PsCallDivertingStatus
{
    DivertingStatusActive,
    DivertingStatusInactive,
    DivertingStatusNotRegistered,
    DivertingStatusNotProvisioned,
    DivertingStatusUnknown
};

//call forwarding conditions
enum PsCallDivertingCondition
{
    DivertConditionUnknown,
    DivertConditionUnconditional,
    DivertConditionBusy,
    DivertConditionNoReply,
    DivertConditionNotReachable,
    DivertConditionAllCalls,
    DivertConditionAllConditionalCases
};

//call forwarding actions
enum PsCallDivertingSetting
{
    ActivateDiverting,
    CancelDiverting,
    RegisterDiverting,
    EraseDiverting,
    CheckStatus
};

// basic teleservice groups (ETSI Gsm 02.03 and 02.04)
enum PsService
{
    ServiceGroupUnknown = 0x00000000,
    ServiceGroupVoice = 0x00000001,
    ServiceGroupData = 0x00000002,
    ServiceGroupFax = 0x00000004,
    ServiceGroupAllTeleservices = ServiceGroupVoice | ServiceGroupData
            | ServiceGroupFax
};
typedef QFlags<PsService> PsServiceGroup;


#endif // PSETWRAPPERTYPES_H
