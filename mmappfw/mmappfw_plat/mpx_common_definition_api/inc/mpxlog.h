/*
* Copyright (c) 2006 Nokia Corporation and/or its subsidiary(-ies).
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
* Description:  Debug print macros
*
*/



#ifndef MPXLOG_H
#define MPXLOG_H

// INCLUDES
#include <e32std.h>
#include <e32svr.h>
#include <e32property.h>

#ifdef _DEBUG
#define MPX_PERFORMANCE_LOG
#endif

// MACROS
#ifdef _DEBUG
#define MPX_DEBUG1(s)               RDebug::Print(_L(#s))
#define MPX_DEBUG2(s, a)            RDebug::Print(_L(#s), a)
#define MPX_DEBUG3(s, a, b)         RDebug::Print(_L(#s), a, b)
#define MPX_DEBUG4(s, a, b, c)      RDebug::Print(_L(#s), a, b, c)
#define MPX_DEBUG5(s, a, b, c, d)   RDebug::Print(_L(#s), a, b, c, d)
#define MPX_FUNC(fn)                TFunctionTrace _s(_L(fn));
#define MPX_FUNC_EX(fn)             TFunctionTrace _s(_L(fn), this);
class TFunctionTrace
    {
    public:
        TFunctionTrace(TRefByValue<const TDesC> aName, TAny* aThis=NULL):
            iStr(aName), iThis(aThis)
            {
            iThis ? MPX_DEBUG3("-->%S, 0x%08x",&iStr, iThis) : MPX_DEBUG2("-->%S",&iStr);
            }
        ~TFunctionTrace()
            {
            iThis ? MPX_DEBUG3("<--%S, 0x%08x",&iStr, iThis) : MPX_DEBUG2("<--%S",&iStr);
            }
    private:
        TPtrC iStr;
        TAny* iThis;
    };

// print out (index, id) for each level and the first 15 ids at top level
#define MPX_DEBUG_PATH(aPath) { \
    TInt dlevels = (aPath).Levels();\
    TInt dcount = (aPath).Count();\
    MPX_DEBUG3("Collection Path debug: levels=  %d, count = %d", dlevels, dcount);\
    for (TInt i=0; i<dlevels;++i)\
        {\
        MPX_DEBUG4("Collection Path debug: Level[%d] = (index %d, id 0x%08x)", i, (aPath).Index(i), (aPath).Id(i).iId1);\
        }\
    if (dcount > 0)\
        {\
        TInt index = (aPath).Index();\
        MPX_DEBUG2("Collection Path debug current index %d", index);\
        if (index>=0 && index<dcount)\
            {\
            CMPXCollectionPath* dpath = const_cast<CMPXCollectionPath*>(&aPath);\
            dpath->SetToFirst();\
            TInt ditem(0);\
            do\
                {\
                MPX_DEBUG3("Collection Path debug top level: item %d = id 0x%08x", ditem, dpath->Id().iId1);\
                } while (++(*dpath) && ++ditem < 15 );\
            dpath->Set(index);\
            }\
        else\
            {\
            MPX_DEBUG1("Collection Path debug: Invalid current index");\
            }\
        }\
    }

#define MPX_DEBUG_THREAD(ss) { \
    RProcess process; \
    TFullName fullName; \
    process.FullName( fullName ); \
    RThread thisThread; \
    _LIT(_ss, ss); \
    MPX_DEBUG5("%S: process=%S, thread=%d, this=0x%08x", &_ss, &fullName, (TUint)thisThread.Id(), this); \
    }

_LIT(_KMPXErrorInfo, "##MPX Error##: error %d file %s line %d");
#define MPX_S(a) _S(a)
#define MPX_ERROR_LOG(aErr) {\
    if (aErr) RDebug::Print(_KMPXErrorInfo, aErr, MPX_S(__FILE__), __LINE__);\
    }
#else
#define MPX_DEBUG1(s)
#define MPX_DEBUG2(s, a)
#define MPX_DEBUG3(s, a, b)
#define MPX_DEBUG4(s, a, b, c)
#define MPX_DEBUG5(s, a, b, c, d)
#define MPX_FUNC(s)
#define MPX_FUNC_EX(fn)
#define MPX_DEBUG_PATH(aPath)
#define MPX_DEBUG_THREAD(ss)
// Copy constant _KMPXErrorInfo and Macros MPX_S and MPX_ERROR_LOG from
// _DEBUG section if intend to log error in release build.
#define MPX_ERROR_LOG(aErr)
#endif

#ifdef MPX_PERFORMANCE_LOG
#define MPX_PERF_CHECKPT(s)     RDebug::Print(_L("--- PERFORMANCE CHECK POINT ---"));\
                                RDebug::Print(_L(#s));
#define MPX_PERF_START(fn) TPerfTrace perf__##fn (_L(#fn)); perf__##fn.Start();
#define MPX_PERF_END(fn) perf__##fn.End();

#define MPX_PERF_START_EX(fn) TPerfTrace perf__##fn (_L(#fn)); perf__##fn.StartEx();
#define MPX_PERF_END_EX(fn) TPerfTrace perf__##fn (_L(#fn)); perf__##fn.EndEx();
#define KAppUidMusicPlayerPerf 300000
#define KMPXPerfTop32 1
#define KMPXPerfBottom32 2 

class TPerfTrace
    {
    public:
        TPerfTrace(TRefByValue<const TDesC> aName):
            iStr(aName)
            {
            }
        void TPerfTrace::Start()
            {
            iTime.UniversalTime();
            MPX_DEBUG2("-->%S",&iStr);
            }
        void TPerfTrace::End()
            {
            TTime now;
            now.UniversalTime();
            TInt delta = I64INT(now.MicroSecondsFrom(iTime).Int64());
            MPX_DEBUG3("<--%S, time taken %d us", &iStr, delta);
            }
        void TPerfTrace::StartEx()
            {
            MPX_DEBUG2(">>> MPXPerf started %S",&iStr);
            TTime time;
            time.UniversalTime();
            TInt64 start = time.Int64();

            TInt err( RProperty::Define(TUid::Uid(KAppUidMusicPlayerPerf), KMPXPerfTop32, RProperty::EInt ) );
            if ( err == KErrNone || err == KErrAlreadyExists)
                {
                err = RProperty::Define(TUid::Uid(KAppUidMusicPlayerPerf), KMPXPerfBottom32, RProperty::EInt );
                }
                
            if ( err == KErrNone || err == KErrAlreadyExists )
                {
                TInt top32( start >> 32 );
                TInt bottom32( start & 0xFFFFFFFF );
                err = RProperty::Set( TUid::Uid(KAppUidMusicPlayerPerf),
                            KMPXPerfTop32,
                            top32 );
                err = RProperty::Set( TUid::Uid(KAppUidMusicPlayerPerf),
                            KMPXPerfBottom32,
                            bottom32 );
                }
            }
            
            
        void TPerfTrace::EndEx()
            {
            TTime end;
            end.UniversalTime();
            TInt ret( 0 );
            TInt64 s( 0 );
            TInt err( RProperty::Get( TUid::Uid(KAppUidMusicPlayerPerf), KMPXPerfTop32, ret ) );
            if ( err == KErrNone )
                {
                s = TInt64( ret ) << 32;
                err = RProperty::Get( TUid::Uid(KAppUidMusicPlayerPerf), KMPXPerfBottom32, ret );
                if ( err == KErrNone )
                    {
                    s += ret;
                    }
                else
                    {
                    s = 0;
                    }
                }
            TTime start(s);
            TInt delta = I64INT(end.MicroSecondsFrom(start).Int64());
            MPX_DEBUG3(">>> MPXPerf %S, time taken %d us", &iStr, delta);
            }            
    private:
        TPtrC iStr;
        TTime iTime;
    };
    
   
#else
#define MPX_PERF_CHECKPT(s)
#define MPX_PERF_START(fn)
#define MPX_PERF_END(fn)
#define MPX_PERF_START_EX(fn)
#define MPX_PERF_END_EX(fn)
#endif

#define MPX_TRAP(_r, _s) TRAP(_r,_s);MPX_ERROR_LOG(_r);
#define MPX_TRAPD(_r, _s) TRAPD(_r,_s);MPX_ERROR_LOG(_r);

#endif  // MPXLOG_H

// End of File
