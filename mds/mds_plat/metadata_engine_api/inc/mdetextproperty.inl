/*
* Copyright (c) 2008-2009 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:  CMdETextProperty's compatibility wrapper to CMdEProperty
*
*/

_LIT( KCMdETextPropertyGetValuePanic, "CMdETextProperty::Value" );
_LIT( KCMdETextPropertySetValuePanic, "CMdETextProperty::SetValueL" );

// ---------------------------------------------------------------------------
// Value
// ---------------------------------------------------------------------------
//
inline const TDesC& CMdETextProperty::Value() const
	{
	TDesC* des = NULL;

	TRAPD( err, des = CONST_CAST( TDesC*, &TextValueL() ) );
	if( err != KErrNone )
		{
#ifdef _DEBUG
		User::Panic( KCMdETextPropertyGetValuePanic, KErrNotSupported );
#endif
		return KNullDesC;
		}
	return *des;
	}

// ---------------------------------------------------------------------------
// SetValueL
// ---------------------------------------------------------------------------
//
inline void CMdETextProperty::SetValueL(const TDesC& aValue)
	{
	TRAPD( err, SetTextValueL( aValue ) );
	if( err != KErrNone )
		{
#ifdef _DEBUG		
		User::Panic( KCMdETextPropertySetValuePanic, KErrNotSupported );
#endif
		}
	}
	
