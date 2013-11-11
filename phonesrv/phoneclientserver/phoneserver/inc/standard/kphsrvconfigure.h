/*
* Copyright (c) 2002-2004 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:  Build configuration file for Phone Server.
*
*/


#ifndef PHSRVCONFIGURE_H
#define PHSRVCONFIGURE_H


//////////////////////////////////////////////////////////////////////////////
//  DEBUGGING
//////////////////////////////////////////////////////////////////////////////
/** When defined debugging features are built in. */

//#define __PHSRV_DEBUG_INFO__

#ifdef __PHSRV_DEBUG_INFO__

/** When defined, phone server prints RDEBUG messages. */
#define __PHSRV_PRINT_DEBUG_INFO__

/** When defined, phone server writes log. 
*/
//#define __PHSRV_DEBUG_WRITE_LOG__

#endif


#endif
// End of File
