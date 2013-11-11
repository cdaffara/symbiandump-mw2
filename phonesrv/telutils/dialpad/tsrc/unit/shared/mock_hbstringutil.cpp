/** Copyright (c) 2010 Nokia Corporation and/or its subsidiary(-ies).
* All rights reserved.
* This component and the accompanying materials are made available
* under the terms of the License "Eclipse Public License v1.0"
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
#include <QDebug>
#include <smcmockclassincludes.h>
#include "hbstringutil.h"

// ============================ MEMBER FUNCTIONS ===============================

// -----------------------------------------------------------------------------
// HbStringUtil::matchC
// -----------------------------------------------------------------------------
//
int HbStringUtil::matchC( 
        const QString & strFrom,
        const QString & strToMatch,
        int maxLevel,
        Options flags,
        int wildChar,
        int wildSequenceChar,
        int escapeChar )
    {
    /*SMC_MOCK_METHOD7( int, const QString &, strFrom, 
        const QString &, strToMatch, 
        int, maxLevel, 
        Options, flags, 
        int, wildChar, 
        int, wildSequenceChar, 
        int, escapeChar )*/
    }


// -----------------------------------------------------------------------------
// HbStringUtil::compareC
// -----------------------------------------------------------------------------
//
int HbStringUtil::compareC( 
        const QString & string1,
        const QString & string2,
        int maxLevel,
        Options flags )
    {
    /*SMC_MOCK_METHOD4( int, const QString &, string1, 
        const QString &, string2, 
        int, maxLevel, 
        Options, flags )*/
    }


// -----------------------------------------------------------------------------
// HbStringUtil::findC
// -----------------------------------------------------------------------------
//
int HbStringUtil::findC( 
        const QString & strFrom,
        const QString & strToFind,
        int maxLevel )
    {
    SMC_MOCK_METHOD3( int, const QString &, strFrom, 
        const QString &, strToFind, 
        int, maxLevel )
    }


// -----------------------------------------------------------------------------
// HbStringUtil::matchF
// -----------------------------------------------------------------------------
//
int HbStringUtil::matchF( 
        const QString & strFrom,
        const QString & strToMatch )
    {
    SMC_MOCK_METHOD2( int, const QString &, strFrom, 
        const QString &, strToMatch )
    }


// -----------------------------------------------------------------------------
// HbStringUtil::compareF
// -----------------------------------------------------------------------------
//
int HbStringUtil::compareF( 
        const QString & string1,
        const QString & string2 )
    {
    SMC_MOCK_METHOD2( int, const QString &, string1, 
        const QString &, string2 )
    }


// -----------------------------------------------------------------------------
// HbStringUtil::findF
// -----------------------------------------------------------------------------
//
int HbStringUtil::findF( 
        const QString & strFrom,
        const QString & strToFind )
    {
    SMC_MOCK_METHOD2( int, const QString &, strFrom, 
        const QString &, strToFind )
    }


// -----------------------------------------------------------------------------
// HbStringUtil::convertDigits
// -----------------------------------------------------------------------------
//
QString HbStringUtil::convertDigits( 
        const QString str )
    {
    SMC_MOCK_METHOD1( QString, const QString, str )
    }


// -----------------------------------------------------------------------------
// HbStringUtil::convertDigitsTo
// -----------------------------------------------------------------------------
//
QString HbStringUtil::convertDigitsTo( 
        const QString str,
        const DigitType digitType )
    {
    SMC_MOCK_METHOD2( QString, const QString, str, 
        const DigitType, digitType )
    }


// -----------------------------------------------------------------------------
// HbStringUtil::sort
// -----------------------------------------------------------------------------
//
void HbStringUtil::sort( 
        QStringList & strList )
    {
    SMC_MOCK_METHOD1( void, QStringList &, strList )
    }


