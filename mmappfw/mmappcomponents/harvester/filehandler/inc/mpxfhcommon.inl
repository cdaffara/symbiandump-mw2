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
* Description:  inline functions
*
*/


// ---------------------------------------------------------------------------
// Parses a string for tokens separated by |
// ---------------------------------------------------------------------------
//
inline void ExtractTokensL( const TDesC& aString, RArray<TPath>& aArray )
    {
     // Parse the string and add it into array
    //
    TBool done(EFalse);
    TLex lex( aString );
    lex.Mark();
    while( !done )
        {
        // Found a token
        if( lex.Peek() =='|' || lex.Eos() )
            {
            TPtrC token = lex.MarkedToken();

            // Ignore null length tokens
            if( token.Length() )
                {
                aArray.AppendL( token );
                }

            // Next token
            if( lex.Eos() )
                {
                done = ETrue;
                }
            else
                {
                lex.Inc();
                lex.Mark();
                }
            }
        else
            {
            // Next character
            lex.Inc();
            }
        }
    }

// ---------------------------------------------------------------------------
// Parses a string for tokens separated by |
// ---------------------------------------------------------------------------
//
inline void ExtractTokensL( const TDesC& aString, CDesCArray& aArray )
    {
     // Parse the string and add it into array
    //
    TBool done(EFalse);
    TLex lex( aString );
    lex.Mark();
    while( !done )
        {
        // Found a token
        if( lex.Peek() =='|' || lex.Eos() )
            {
            TPtrC token = lex.MarkedToken();

            // Ignore null length tokens
            if( token.Length() )
                {
                aArray.InsertIsqL( token );
                }

            // Next token
            if( lex.Eos() )
                {
                done = ETrue;
                }
            else
                {
                lex.Inc();
                lex.Mark();
                }
            }
        else
            {
            // Next character
            lex.Inc();
            }
        }
    }

// ---------------------------------------------------------------------------
// Finds the drive number for a path
// ---------------------------------------------------------------------------
//
inline TDriveNumber ExtractDrive( const TDesC& aPath )
    {
    TParsePtrC parse( aPath );
    TDriveUnit drive( parse.Drive() );
    TInt num = drive;
    return TDriveNumber(num);
    }