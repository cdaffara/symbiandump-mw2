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
* Description:  Header Information for MTP Logger
*
*/


#ifndef MMMTPDPLOGGER_H
#define MMMTPDPLOGGER_H

#include <e32debug.h>

// ---------------------------------------------------------------------------
#if !defined( PRINT3 ) 
  #ifdef  _DEBUG 
      #define PRINT( txt )                          RDebug::Print( txt );
      #define PRINT1( txt, arg1 )                   RDebug::Print( txt, arg1 );
      #define PRINT2( txt, arg1, arg2 )             RDebug::Print( txt, arg1, arg2 );
      #define PRINT3( txt, arg1, arg2, arg3 )       RDebug::Print( txt, arg1, arg2, arg3 );
      #define PRINT4( txt, arg1, arg2, arg3, arg4 ) RDebug::Print( txt, arg1, arg2, arg3, arg4 );
  
  // PRINT_FRQ macros are used for logging of frequently occurring events,
  // for example draws done every time a viewfinder frame arrives.
  // By default, logging these is disabled.  
  #undef LOG_FREQUENT
  
  #ifdef LOG_FREQUENT      
      #define PRINT_FRQ( txt )                          RDebug::Print( txt );
      #define PRINT_FRQ1( txt, arg1 )                   RDebug::Print( txt, arg1 );
      #define PRINT_FRQ2( txt, arg1, arg2 )             RDebug::Print( txt, arg1, arg2 );
      #define PRINT_FRQ3( txt, arg1, arg2, arg3 )       RDebug::Print( txt, arg1, arg2, arg3 );
      #define PRINT_FRQ4( txt, arg1, arg2, arg3, arg4 ) RDebug::Print( txt, arg1, arg2, arg3, arg4 );
  #else
    #define PRINT_FRQ( txt )
      #define PRINT_FRQ1( txt, arg1 )
      #define PRINT_FRQ2( txt, arg1, arg2 )
      #define PRINT_FRQ3( txt, arg1, arg2, arg3 )
      #define PRINT_FRQ4( txt, arg1, arg2, arg3, arg4 )           
  #endif // LOG_FREQUENT
            
  #else
      #define PRINT( txt )
      #define PRINT1( txt, arg1 )
      #define PRINT2( txt, arg1, arg2 )
      #define PRINT3( txt, arg1, arg2, arg3 )
      #define PRINT4( txt, arg1, arg2, arg3, arg4 ) 
      
      #define PRINT_FRQ( txt )
      #define PRINT_FRQ1( txt, arg1 )
      #define PRINT_FRQ2( txt, arg1, arg2 )
      #define PRINT_FRQ3( txt, arg1, arg2, arg3 )
      #define PRINT_FRQ4( txt, arg1, arg2, arg3, arg4 )       
      
  #endif
#endif
// ---------------------------------------------------------------------------
#endif // MMMTPDPLOGGER_H
