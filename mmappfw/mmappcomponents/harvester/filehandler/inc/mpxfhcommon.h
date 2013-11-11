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
* Description:  Common utility functions that have been factored out
*
*/


#ifndef CMPXHARVESTERCOMMON_H
#define CMPXHARVESTERCOMMON_H

#include <badesca.h>

/*
* Extract tokens
* @param aString to Parse
* @param aArray On return, will be filled with the tokens separated by |
*/
inline void ExtractTokensL( const TDesC& aString, RArray<TPath>& aArray );

/*
* Extract tokens
* @param aString to Parse
* @param aArray On return, will be filled with the tokens seperated by |
*/
inline void ExtractTokensL( const TDesC& aString, CDesCArray& aArray );

/*
* Returns the drive for a file
*/
inline TDriveNumber ExtractDrive( const TDesC& aPath );

#include "mpxfhcommon.inl"

#endif // CMPXHARVESTERCOMMON_H    
    