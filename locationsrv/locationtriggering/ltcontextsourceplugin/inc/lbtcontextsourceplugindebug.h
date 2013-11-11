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
* Description:  Defines debug macros for lbtcontextsourceplugin
*
*/


#ifndef LBTCONTEXTSOURCEPLUGINDEBUG_H
#define LBTCONTEXTSOURCEPLUGINDEBUG_H

// INCLUDES
#include <e32std.h>
#include "lbtcontextsourceplugindebug.hrh"



// Macros

#define DEBUGLBTCONTEXTSOURCEPLUGIN(TEXT) 			 { \
									  		 _LIT( KText, #TEXT ); \
									   		 DebugLbtContextSourcePlugin(KText); \
									 		 }
									 
#define LOGLBTCONTEXTSOURCEPLUGIN(TEXT, ARG1) 		 { \
									         _LIT( KText, #TEXT ); \
									         DebugLbtContextSourcePlugin( KText, (ARG1) ); \
									         }


/**
* Logs messages into the c:\logs\lbtcontextsourceplugin\lbtcontextsourceplugin.txt file
* provided the folder c:\logs\lbtcontextsourceplugin\ exists.
*/
void DebugLbtContextSourcePlugin( TRefByValue<const TDesC> aText, ... );



/**
* Logging is enabled only when LOG_ENABLED_LBTCONTEXTSOURCEPLUGIN is defined
*/
#ifdef LOG_ENABLED_LBTCONTEXTSOURCEPLUGIN

#define LBTCONTEXTSOURCEPLUGINDEBUG(TEXT)                     DEBUGLBTCONTEXTSOURCEPLUGIN(TEXT)
#define LBTCONTEXTSOURCEPLUGINDEBUG1(TEXT, ARG1)              LOGLBTCONTEXTSOURCEPLUGIN(TEXT, ARG1)

#else

#define LBTCONTEXTSOURCEPLUGINDEBUG(TEXT) 
#define LBTCONTEXTSOURCEPLUGINDEBUG1(TEXT, ARG1) 

#endif


#endif      // LBTCONTEXTSOURCEPLUGINDEBUG_H



// End of file
