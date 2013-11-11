/*!
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
* Description:  Definitions for key sequence recognition component.
*
*/

#ifndef KEYSEQUENCERECOGNITIONSERVICEDEFS_H
#define KEYSEQUENCERECOGNITIONSERVICEDEFS_H

#include <QString>

/*! code for showing software version number */
const QString KCodeSwVersion("*#0000#");

/*! code for launching reset ui */
const QString KCodeActivateRfsNormal("*#7780#");

/*! code for launching reset ui */
const QString KCodeActivateRfsDeep("*#7370#");

/*! code for showing bluetooth device address */
const QString KCodeBtAddress("*#2820#");

/*! code for enabling bluetooth test mode */
const QString KCodeRFLoopback("*#9990#");

/*! code for showing serial number */
const QString KCodeImei("*#06#");

/*! code for showing life timer data */
const QString KCodeLifeTimer("*#92702689#");

/*! code for changing pin code 1 */
const QString KCodeChangePin1("\\*\\*04\\*\\d{4,15}\\*\\d{4,15}\\*\\d{4,15}\\#");

/*! code for changing pin code 2 */
const QString KCodeChangePin2("\\*\\*042\\*\\d{4,15}\\*\\d{4,15}\\*\\d{4,15}#");

/*! code for unblocking pin code 1 */
const QString KCodeUnblockPin1("\\*\\*05\\**\\d{8,20}\\*\\d{4,15}\\*\\d{4,15}#");

/*! code for unblocking pin code 2 */
const QString KCodeUnblockPin2("\\*\\*052\\**\\d{8,20}\\*\\d{4,15}\\*\\d{4,15}#");

/* code for showing the local bluetooth device address in a dialog */
const QString KShowBTAddress("*#2820#");

/* code for activating the bluetooth device test mode (DUT mode) */
const QString KActivateBTTestMode("*#9990#");

/* code for toggling the bluetooth SSP debug mode */
const QString KToggleBTSSPDebugMode("*#2873#");

#endif // KEYSEQUENCERECOGNITIONSERVICEDEFS_H
