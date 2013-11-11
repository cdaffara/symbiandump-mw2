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
* Description:   SUPL Settings Notifier Interface definition
*
*/


#ifndef EPOS_OMASUPLNOTIFIERINTERFACE_HRH
#define EPOS_OMASUPLNOTIFIERINTERFACE_HRH

/**
 * Interfaces for using the SUPL Settings notifier when the user is in
 * his home network.
 */
const TUid KSuplHomeNotifier            = {0x10009C00};
const TUid KSuplHomeNotifierChannel     = {0x00000601};

/**
 * Interfaces for using the SUPL Settings notifier when the user is not
 * in his home network.
 */
const TUid KSuplRoamingNotifier         = {0x10009C01};
const TUid KSuplRoamingNotifierChannel  = {0x00000602};

/**
 * Interfaces for using the SUPL Settings notifier when the SUPL connection
 * times out.
 */
const TUid KSuplTimeoutNotifier         = {0x10009C02};
const TUid KSuplTimeoutNotifierChannel  = {0x00000603};

#endif // EPOS_OMASUPLNOTIFIERINTERFACE_HRH
