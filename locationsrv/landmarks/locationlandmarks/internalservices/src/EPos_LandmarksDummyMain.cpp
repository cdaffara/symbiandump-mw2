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
*   is not used, but to be able to generate the resource file with global
*   categories during build, we need to create a dummy DLL.
*
*/


// INCLUDE FILES
#include <e32std.h>

// We need to export a function to be able to create a DLL. Note, this function
// is not used.
IMPORT_C void Dummy();
EXPORT_C void Dummy()
 {
 }

//  End of File
