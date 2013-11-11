/*
* Copyright (c) 2006 Nokia Corporation and/or its subsidiary(-ies).
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
* Description:   Constant data for SendUi Refresh API
*
*/



#ifndef R_CSENDUIPROPERTY_H
#define R_CSENDUIPROPERTY_H

const TUid KPSUidSendUiProperty = { 0x102072E4 };

enum TSendUiPropertyKeys
    {
    KSendUiUpdateRequest,
    KSendUiUpdateOperation
    };

enum TSendUiPropertyValues
    {
    KSendUiNotInitialized = -1,
    KSendUiUpdated,
    KSendUiUpdateRequested,
    KSendUiUpdateOperationRunning,
    KSendUiUpdateOperationDone
    };
    
#endif