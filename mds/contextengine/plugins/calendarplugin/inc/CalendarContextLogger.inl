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
*/


/**
 * 
 */
inline void FPrint( const TRefByValue<const TDesC> aFmt, ... )
    {
    VA_LIST list;
    VA_START( list,aFmt );
    RFileLogger::WriteFormat( KLBENGLogDir, KLBENGLogFile, EFileLoggingModeAppend, aFmt, list );
    }

/**
 * 
 */
inline void FPrint( const TDesC& aDes )
    {
    RFileLogger::WriteFormat( KLBENGLogDir, KLBENGLogFile, EFileLoggingModeAppend, aDes );
    }

/**
 * 
 */
inline void FHex( const TUint8* aPtr, TInt aLen )
    {
    RFileLogger::HexDump( KLBENGLogDir, KLBENGLogFile, EFileLoggingModeAppend, 0, 0, aPtr, aLen );
    }

/**
 * 
 */
inline void FHex( const TDesC8& aDes )
    {
    FHex( aDes.Ptr(), aDes.Length() );
    }

/**
 * 
 */
inline void FCreate()
    {
    _LIT( KLogPath, "c:\\logs\\" );
    _LIT( KRootPath, "\\" );
    TFileName path( KLogPath );
    path.Append( KLBENGLogDir );
    path.Append( KRootPath );
    RFs fs;
    fs.Connect();
    fs.MkDirAll( path );
    fs.Close();
    RFileLogger::WriteFormat( KLBENGLogDir, KLBENGLogFile, EFileLoggingModeOverwrite, KLBENGLogBanner );
    }
