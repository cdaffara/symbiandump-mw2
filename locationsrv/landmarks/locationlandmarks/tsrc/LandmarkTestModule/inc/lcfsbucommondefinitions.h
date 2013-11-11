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
* Description:
*
*/

#ifndef _LCF_SBU_COMMON_DEFINITIONS_H_
#define _LCF_SBU_COMMON_DEFINITIONS_H_

///////////////////////////////////////////////////////
//
// SBu Common Definitions
//
///////////////////////////////////////////////////////

/** 
    $File: LcfSBuCommonDefitions.h
    $Author: burnevsk $
    $Date: 2004/10/18 08:14:23 $
    $Revision: 1.6 $
*/

///////////////////////////////////////////////////////
//
// Constants
//
///////////////////////////////////////////////////////


#include <e32def.h>
#include <e32std.h>
#include <e32err.h>

#define DEF_LCFPSY1_MODULEID        0x03133002
#define DEF_LCFPSY2_MODULEID        0x03133004
#define DEF_LCFPSY3_MODULEID        0x03133006
#define DEF_LCFPSYSLOW4_MODULEID    0x03133008
#define DEF_LCFPSY6_MODULEID        0x0313300c

const TInt KLcfPsy1UidValue = DEF_LCFPSY1_MODULEID;
const TInt KLcfPsy3UidValue = DEF_LCFPSY3_MODULEID;
const TInt KLcfPsy6UidValue = DEF_LCFPSY6_MODULEID;

const TInt KSecondsToMicro = 1000*1000;

// Show service formt
_LIT(KShowServiceFormat, "Name: %S \nType: %d \n  Id: %S");

// time value formatting
_LIT(KTimeSpec, "%H:%T:%S:%C");

// default requestors
_LIT(KServiceName, "Lcf.Service");
_LIT(KServiceId, "Lcf.ServiceId");

_LIT(KContactName, "Lcf.Contact");
_LIT(KPhone, "+79217654321");

_LIT(KFieldNamePhone, "Phone");
_LIT(KFieldNameEmail, "Email");
_LIT(KFieldNameUrl, "Url");

///////////////////////////////////////////////////////
//
// Other text
//
///////////////////////////////////////////////////////

// Indicate not yet implemented
_LIT(KNotImplemented, "Not yet implemented");   

//_LIT(KBadRequestResult, "Wrong location request result (result=%d, expected=%d)");
//_LIT(KBadDiagNum, "Wrong number of events in Notifier (count=%d, expected=%d)");
//_LIT(KBadLcfResult, "Wrong notifier event result (result=%d, expected=%d)");

_LIT(KBadDiagVerifEventNum, "Wrong number of Verification events in Notifier");
_LIT(KBadDiagNotifEventNum, "Wrong number of Notification events in Notifier");

///////////////////////////////////////////////////////
//
// Here are macros for error logging 
// Those imply that are called from within of 
// CUtfwTestBase-derived class's function (they use iLog member)
//
///////////////////////////////////////////////////////

// LEAVES with _error_ if _condition_ is true
#define LEAVE(condition, error)             \
    if (condition) {                        \
        errStr.Format(_L("%s (line %d)"), _S(error), __LINE__);    \
        iLog->Log(errStr);			\
        User::Leave(KErrGeneral);           	\
    }

// if _error_ is descritor
#define LEAVE_DES(condition, error_des)     \
    if (condition) {                        \
        errStr.Format(_L("%S (line %d)"), &error_des, __LINE__);    \
        iLog->Log(errStr);					\
        User::Leave(KErrGeneral);            \
    }

// Must be included in the begging of function definition
// in order to use following ("ERROR") macros
// (The string buffer needs to be defined once, otherwise,
// in case when a lot of errors are checked, stack may be
// overflown, and copilation will fail)
#define DECLARE_ERROR_STR                   TBuf<160> errStr;
#define DECLARE_MESSAGE_STR                 TBuf<160> message;

#define DECLARE_ERROR_LOGGING               DECLARE_ERROR_STR   DECLARE_MESSAGE_STR

#define START_ERROR_LOGGING                 iErrorCounter = 0;

// Logs a message, which is defined as "..." constant
#define LOG(msg)                            \
    {                                       \
        errStr.Format(_L("%s (line %d)"), _S(msg), __LINE__);    \
        iLog->Log(errStr);                  \
    }
#define LOG_DES(msg_des)                    \
    {                                       \
        errStr.Format(_L("%S (line %d)"), &msg_des, __LINE__);    \
        iLog->Log(errStr);                  \
    }
    
#define LOG_FORMAT_1(_format, _param)       \
    message.Format(_format, _param);        \
    LOG_DES(message);
    
#define LOG_FORMAT_2(_format, _param1, _param2)       \
    message.Format(_format, _param, _param2);         \
    LOG_DES(message);

#define LOG_IIF(condition, msg1, msg2)      \
    if (condition) LOG(msg1) else LOG(msg2)

// Logs an error, which is defined as "..." constant
#define ERR(error)                          \
    {                                       \
        errStr.Format(_L("%s (line %d)"), _S(error), __LINE__);    \
        iLog->Log(errStr);             \
        iErrorCounter++;                    \
    }
#define ERR_DES(error)                      \
    {                                       \
        errStr.Format(_L("%S (line %d)"), &error, __LINE__);    \
        iLog->Log(errStr);             \
        iErrorCounter++;                    \
    }

// Logs an _error_ if _condition_ is true
#define ERROR(condition, error)             \
    if (condition) ERR(error)

// Logs an error, which defined as descriptor
#define ERROR_DES(condition, error_des)     \
    if (condition) ERR_DES(error_des)

#define ERROR_1(condition, msg_fmt_des, param1)  \
    message.Format(msg_fmt_des, param1);        \
    ERROR_DES(condition, message);

#define ERROR_2(condition, msg_fmt_des, param1, param2)  \
    message.Format(msg_fmt_des, param1, param2);        \
    ERROR_DES(condition, message);

#define ERROR_3(condition, msg_fmt_des, param1, param2, param3) \
    message.Format(msg_fmt_des, param1, param2, param3);        \
    ERROR_DES(condition, message);

#define IF_ERROR(error_code, msg)                               \
    { TInt32 _err = error_code;                                 \
    ERROR_2(_err != KErrNone, _L("%s (error=%d)"), _S(msg), _err); }

#define IF_ERROR_DES(error_code, msg_des)                       \
    { TInt32 _err = error_code;                                 \
    ERROR_2(_err != KErrNone, _L("%S (error=%d)"), &msg_des, _err); }

#define CHECK_EQUAL(_actual, _expected, _msg)                     \
    ERROR_3(_actual != _expected, _L("%s (actual=%d, expected=%d)"), _S(_msg), _actual, _expected);

#define CHECK_EQUAL_DES(_actual, _expected, _msg_des)           \
    ERROR_3(_actual != _expected, _L("%S (actual=%d, expected=%d)"), &_msg_des, _actual, _expected);

// Leaves if there were errors generated by ERROR macros
// (put it at the end of you test function)
#define LEAVE_IF_ERRORS()               \
    if (iErrorCounter > 0) {             \
        errStr.Format(_L("There were %d errors in the test, see previous messages!"), iErrorCounter);    \
        iLog->Log(errStr); \
          User::Leave(KErrGeneral);  \
    }

#define PAUSE_MS(_microseconds)                                 \
    User::After(TTimeIntervalMicroSeconds32(_microseconds));
#define PAUSE(_seconds)                                         \
    PAUSE_MS(_seconds * KSecondsToMicro)

class TWatch 
{
    public:
        TWatch() { RestartWatch(); };
        void RestartWatch() { iTime.UniversalTime(); iLastTick = iTime; };
        void Tick() { iLastTick.UniversalTime(); };
        TTimeIntervalMicroSeconds ElapsedTotal()
        {
            TTime now;
            now.UniversalTime();
            return now.MicroSecondsFrom(iTime);
        };
        TTimeIntervalMicroSeconds ElapsedFromTick()
        {
            TTime now;
            now.UniversalTime();
            return now.MicroSecondsFrom(iLastTick);
        };
        TTime Time() {return iTime; };
    private:
        TTime iTime;
        TTime iLastTick;
};

class TThreadUtility
{
    public:
        typedef void (*TThreadFunctionL)(TAny* aPtr);

        struct TOptions
        {
            TThreadFunctionL    ThreadFunctionL;
            TAny*               parameters; 
        };

        static TInt ThreadFunction(TAny* aPtr)
        {
            CTrapCleanup* cs = CTrapCleanup::New();
            if (cs == NULL)
            {
                return KErrNoMemory;
            }

            // new ActiveScheduler
            CActiveScheduler* scheduler = new(ELeave) CActiveScheduler;
            CActiveScheduler::Install(scheduler);

            TOptions* options = reinterpret_cast<TOptions*>(aPtr);
            TRAPD(err, (*options->ThreadFunctionL)(options->parameters));

            delete scheduler;
            delete cs;
            User::Exit(err);
            return err;
        };

        void CreateL(const TDesC& aThreadName, TOptions* aOptions)
        {
            iName.Set(aThreadName);
            User::LeaveIfError( iThread.Create(aThreadName,
                                TThreadUtility::ThreadFunction,
                                KDefaultStackSize,
                                KMinHeapSize,
                                10 * KMinHeapSize,
                                aOptions,
                                EOwnerProcess));
        };
        void CreateReuseHeapL(const TDesC& aThreadName, TOptions* aOptions)
        {
            iName.Set(aThreadName);
            User::LeaveIfError( iThread.Create(aThreadName,
                                TThreadUtility::ThreadFunction,
                                KDefaultStackSize,
                                &User::Heap(),
                                aOptions,
                                EOwnerProcess));
        };
        void Start()
        {
            iThread.Resume();
        };
        void Stop()
        {
            iThread.Suspend();
        };

        TInt Wait()
        {
            TRequestStatus threadStatus;
            if (iThread.ExitType() == EExitPending) // still executing
            {
                iThread.Logon(threadStatus);
                User::WaitForRequest(threadStatus);
            }
            return iThread.ExitReason();
        };

        const RThread Handle()
        {
            return iThread;
        };

        void Format(TDes& aMessage)
        {
            TExitCategoryName category = iThread.ExitCategory();
            _LIT(KThreadFormat, "Thread %S, Reason %d, ExitType %d, ExitName %S");
            aMessage.Format(KThreadFormat, 
                &iName, iThread.ExitReason(), iThread.ExitType(), &category);
        };

        // used if class destroyed
        void Close()
        {
            if (iThread.Handle())
            {
                if (iThread.ExitType() == EExitPending) // still executing
                {
                    iThread.Kill(KErrNone);
                }
                iThread.Close();
            }
        };

    protected:
        RThread iThread;
        TPtrC   iName;
};

#endif // _LCF_SBU_COMMON_DEFINITIONS_H_
