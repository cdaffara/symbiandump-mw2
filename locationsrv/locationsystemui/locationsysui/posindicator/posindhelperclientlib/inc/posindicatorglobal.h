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
* Description: Positioning Indicators export/import declarations
*
*/


#ifndef POSINDICATOR_GLOBAL_H
#define POSINDICATOR_GLOBAL_H

#ifdef BUILD_POSINDICATOR
    #define POSINDICATOR_EXPORT Q_DECL_EXPORT
#else
    #define POSINDICATOR_EXPORT Q_DECL_IMPORT
#endif


#endif  // POSINDICATOR_GLOBAL_H

// End of file
    


