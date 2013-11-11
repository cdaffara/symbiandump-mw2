/*
* Copyright (c) 2005 Nokia Corporation and/or its subsidiary(-ies). 
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
*   Keeps common contants used by this component.
*
*/


#ifndef POSDBREGISTRYCONSTANTS_H
#define POSDBREGISTRYCONSTANTS_H

//CONSTANTS
const TInt KProtocolMaxLength = 16;

_LIT(KProtocol, "://");

// Table name
_LIT(KPosLmDbRegistryTable, "dbreg");

// Column names
_LIT(KPosLmProtocolCol, "prot");
_LIT(KPosLmFileNameCol, "name");
_LIT(KPosLmSettingsCol, "dbset");

_LIT(KPosLmDbRegIndex, "dbreg_index");

#endif //POSDBREGISTRYCONSTANTS_H

// End of File
