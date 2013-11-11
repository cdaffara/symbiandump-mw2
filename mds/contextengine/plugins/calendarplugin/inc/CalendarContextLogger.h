/*
* Copyright (c) 2002-2009 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:  Logging support for R&D purposes
*
*
*/


#ifndef CALENDARCONTEXTLOGGER_H_INCLUDED
#define CALENDARCONTEXTLOGGER_H_INCLUDED

// ========== INCLUDE FILES ================================

#include <eikenv.h>

// ========== CONSTANTS ====================================

#ifdef CALCON_LOGGING_ON

_LIT( KLBENGLogDir,               "CALCON");
_LIT( KLBENGLogFile,              "CALCON.txt");
_LIT( KLBENGLogBanner,            "CALCON 0.1.0");
_LIT( KLBENGLogEnterFn,           "CALCON: -> %S");
_LIT( KLBENGLogLeaveFn,           "CALCON: <- %S");
_LIT( KLBENGLogExit,              "CALCON: Application exit");
_LIT( KLBENGLogTimeFormatString,  "%H:%T:%S:%*C2");

// ========== MACROS =======================================

#define CALCONLOGGER_CREATE
#define CALCONLOGGER_DELETE
#define CALCONLOGGER_ENTERFN(a)         {_LIT(temp, a); RDebug::Print( KLBENGLogEnterFn, &temp);}
#define CALCONLOGGER_LEAVEFN(a)         {_LIT(temp, a); RDebug::Print( KLBENGLogLeaveFn, &temp);}
#define CALCONLOGGER_WRITE(a)           {_LIT(temp, a); RDebug::Print( temp);}
#define CALCONLOGGER_WRITE_TIMESTAMP(a) {_LIT(temp, a); TTime time; time.HomeTime(); TBuf<256> buffer; time.FormatL( buffer, KLBENGLogTimeFormatString ); buffer.Insert(0, temp); RDebug::Print( buffer); }
#define CALCONLOGGER_WRITEF             RDebug::Print

/**
 * 
 */
inline void FPrint( const TRefByValue<const TDesC> aFmt, ... );

/**
 * 
 */
inline void FPrint( const TDesC& aDes );

/**
 * 
 */
inline void FHex( const TUint8* aPtr, TInt aLen );

/**
 * 
 */
inline void FHex( const TDesC8& aDes );

/**
 * 
 */
inline void FCreate();

#include "CalendarContextLogger.inl"


#else // CALCON_LOGGING_ON

inline void FPrint( const TRefByValue<const TDesC> /*aFmt*/, ... ) { };

#define CALCONLOGGER_CREATE
#define CALCONLOGGER_DELETE
#define CALCONLOGGER_ENTERFN(a)
#define CALCONLOGGER_LEAVEFN(a)
#define CALCONLOGGER_WRITE(a)
#define CALCONLOGGER_WRITEF   1 ? ((void)0) : FPrint
#define CALCONLOGGER_WRITE_TIMESTAMP(a)

#endif // CALCON_LOGGING_ON

// ========== DATA TYPES ===================================

// ========== FUNCTION PROTOTYPES ==========================

// ========== FORWARD DECLARATIONS =========================

// ========== CLASS DECLARATION ============================

#endif // CALENDARCONTEXTLOGGER_H_INCLUDED