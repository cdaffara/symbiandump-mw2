/*
* Copyright (c) 2008-2008 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:  Common definitions
*
*/


#ifndef PHONEPARSERCOMMON_H
#define PHONEPARSERCOMMON_H

/**  Dtmf pause */
const TInt KPhoneDtmfPause = 'p';
/**  Dtmf wait */
const TInt KPhoneDtmfWait = 'w';
/**  Hash mark */
const TInt KPhoneNumberHash = '#';
/**  Asterisk mark */
const TInt KPhoneNumberAsterisk = '*';
/**  Plus mark */
const TInt KPhonePlus = '+';
/**  Valid Dtmf Chars */
_LIT( KValidDtmfChars, "0123456789pw*+#" );


#endif // PHONEPARSERCOMMON_H
