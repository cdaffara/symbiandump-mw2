/*
* Copyright (c) 2002 Nokia Corporation and/or its subsidiary(-ies).
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
* Description: MAlwaysOnlineStatusQueryInterface
*
*/

#ifndef __ALWAYSONLINESTATUSQUERYINTERFACE_H__
#define __ALWAYSONLINESTATUSQUERYINTERFACE_H__

#include <e32base.h>
//constants


/**
* CAlwaysOnlineManagerServer
* 
*/
class MAlwaysOnlineStatusQueryInterface
    {
public:
    virtual TAny* QueryStatusL( TInt aQuery ) = 0;
    };


#endif
//EOF
