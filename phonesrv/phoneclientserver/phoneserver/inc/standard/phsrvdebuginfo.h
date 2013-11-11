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
* Description:  Definition of debug information.
*
*/


#ifndef CPHSRVGDEBUGINFO_H
#define CPHSRVGDEBUGINFO_H

// INCLUDES

#include <e32base.h>            // TInt, cbase
#include "kphsrvconfigure.h" // Debugging feature defines 

// MACROS
//----------------------------------------------------------------------------
// These macros are for debugging purposes. 
// Can be enabled or disabled from KPhSrvConfigure.h
// If disabled, macros are empty and no additionall resources used.
// If Enabled. This class PhSrvDebugInfo is additional load.
// Operations are quite heavy weight because use of debug prints and
// threads local storage.
//----------------------------------------------------------------------------
#ifdef __PHSRV_DEBUG_INFO__
#pragma message("\\PhoneServer\\Inc\\Standard\\KPhSrvConfigure.h(107): Warning: ** DEBUG PRINTS ARE ENABLED! ** ") 
// For printing only text.
#define _DPRINT(l,t) { PhSrvDebugInfo::Print((l), _L(t) ); }
// For printing text folloved by number.
#define _DDPRINT(l,t,n) { \
    TBuf<60> buf; \
    buf.Append( _L(t) ); \
    buf.AppendNum( (TInt)n ); \
    PhSrvDebugInfo::Print((l), buf ); }
// For print text followed by text
#define _DSPRINT(l,t,s) { \
    TBuf<60> buf; \
    buf.Append( _L(t) ); \
    buf.Append( s.Right(Min(s.Length(), 30 )) ); \
    PhSrvDebugInfo::Print( (l), buf ); }
#else
#define _DPRINT(l,t)
#define _DDPRINT(l,t,n)
#define _DSPRINT(l,t,s)
#endif

#if defined __PHSRV_DEBUG_TESTER__ && defined __PhSrv_DEBUG_INFO__
// Init & uninit
#ifdef __PHSRV_DEBUG_TESTER_ALWAYS__
#define PHSRV_TESTER_INIT() PhSrvDebugInfo::SetTestMonitorL( ETrue )
#define PHSRV_TESTER_UNINIT() PhSrvDebugInfo::SetTestMonitorL( EFalse )
#else
#define PHSRV_TESTER_INIT()
#define PHSRV_TESTER_UNINIT()
#endif

// Enable & disable
#define PHSRV_TESTER_ENABLE() PhSrvDebugInfo::SetTestMonitorL( ETrue )
#define PHSRV_TESTER_DISABLE() PhSrvDebugInfo::SetTestMonitorL( EFalse )

// Leave if set.
#define PHSRV_TESTER_LEAVE( c, e ) \
    { \
    PhSrvDebugInfo::TPhSrvDebugTestData _i; PhSrvDebugInfo::GetTestData( _i ); \
    if ( _i.iCase == (c) ) \
        { \
        _i.iDone++; \
        PhSrvDebugInfo::SetTestData( _i ); \
        User::Leave( (e) ); \
        } \
    }

// Leave if set, reset also.
#define PHSRV_TESTER_LEAVERESET( c, e ) \
    { \
    PhSrvDebugInfo::TPhSrvDebugTestData _i; PhSrvDebugInfo::GetTestData( _i ); \
    if ( _i.iCase == (c) ) \
        { \
        _i.iCase = _i.iDone = 0; \
        PhSrvDebugInfo::SetTestData( _i ); \
        User::Leave( (e) ); \
        } \
    }

// Leave if set, reset if amounts reached
#define PHSRV_TESTER_LEAVERESET3( c, e, a ) \
    { \
    PhSrvDebugInfo::TPhSrvDebugTestData _i; PhSrvDebugInfo::GetTestData( _i ); \
    if ( _i.iCase == (c) ) \
        { \
        _i.iDone++; \
        if ( _i.iDone > (a) ) \
            { \
            _i.iCase = _i.iDone = 0; \
            PhSrvDebugInfo::SetTestData( _i ); \
            } \
        User::Leave( (e) ); \
        } \
    }

// Set failnext if set, reset
#define PHSRV_TESTER_FAILNEXT( c ) \
    { \
    PhSrvDebugInfo::TPhSrvDebugTestData _i; PhSrvDebugInfo::GetTestData( _i ); \
    if ( _i.iCase == (c) ) \
        { \
        User::__DbgSetAllocFail( RHeap::EUser, RHeap::EFailNext, 1 ); \
        } \
    }

// Reset fail next
#define PHSRV_TESTER_FAILNEXT_RESET() User::__DbgSetAllocFail( RHeap::EUser, RHeap::ENone, 1 )

#else

// Init & uninit
#define PHSRV_TESTER_INIT()
#define PHSRV_TESTER_UNINIT()

// Enable & disable
#define PHSRV_TESTER_ENABLE()
#define PHSRV_TESTER_DISABLE()

// Leave
#define PHSRV_TESTER_LEAVE( c, e )
#define PHSRV_TESTER_LEAVERESET( c, e )
#define PHSRV_TESTER_LEAVERESET3( c, e, a )
#define PHSRV_TESTER_FAILNEXT( c )
#define PHSRV_TESTER_FAILNEXT_RESET()

#endif

// FORWARD DECLARATIONS
class RFs;
class RFile;
//class CPhSrvDebugTestMonitor;

// CLASS DECLARATION

/**
*   Debugging level. From 0-. 
*   00   None 
*   01   PhoneEngine, Call     
*   02   PhoneEngine, Audio
*   03   PhoneEngine, Indicators, Services & others
*   04   PhoneServer
*   05   Phone, logging
*   06   Phone, autom. redial.
*   07   Phone, 
*   08   Phone, 
*   09   
*   10   Phone, recovery system & indicators
*   11   Phone, ui 
*
*  @lib phoneengine
*  @since 1.2
*/
class PhSrvDebugInfo : public CBase
    {
    private:
        NONSHARABLE_CLASS( CPhSrvDebugData ) : public CBase
            {
            public: 
               ~CPhSrvDebugData();
              
                void CreateFile();
                void ReleaseFile();
                TBool IsFile() const;
                void WriteFile( const TDesC8& aDes );
               
            private:
                RFs* iFs;
                RFile* iFile;
             
            };

    public:  // Constructors and destructor
    
    
        /**
        * Prints text if area set.
        *
        * @param aArea area of text.
        * @param aText text to be printed.
        */
        static void Print( TInt aArea, const TDesC& aText );
    
        

    private:
        
        // Creates debug data.
        static CPhSrvDebugData* CreateData();

     

    };
#endif         
// End of File
