/*
* Copyright (c) 2004-2008 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:  Default emergency numbers domain Central Repository keys
*
*/


#ifndef DEFAULTEMERGENCYNUMBERSCRKEYS_H
#define DEFAULTEMERGENCYNUMBERSCRKEYS_H

#include <e32std.h>

/******************************************************************************
* Default emergency numbers API
* Keys under this category are used in defining product specific emergency
* numbers.
******************************************************************************/
const TUid KCRUidDefaultEmergencyNumbers = { 0x102078F1 };

/**
* Contains a space separated list of product specific default emergency numbers.
* Can be empty, contain 1 number or more numbers.
* Varies in different product variants.
*/
const TUint32 KDefaultEmergencyNumbersList    = 0x00000001; 

#endif // DEFAULTEMERGENCYNUMBERSCRKEYS_H

// End of file