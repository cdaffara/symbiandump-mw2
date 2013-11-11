/*
* Copyright (c) 2010 Nokia Corporation and/or its subsidiary(-ies).
* All rights reserved.
* This component and the accompanying materials are made available
* under the terms of "Eclipse Public License v1.0""
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

#ifndef POSSETTINGS_GLOBAL_H
#define POSSETTINGS_GLOBAL_H

#ifdef BUILD_POSSETTINGS
    #define POSSETTINGS_EXPORT Q_DECL_EXPORT
#else
    #define POSSETTINGS_EXPORT Q_DECL_IMPORT
#endif

#endif
