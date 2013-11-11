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
* Description:  Inline functions of the CPhoneGsmParserResult class.
*
*/


// ================= MEMBER FUNCTIONS =======================

// ---------------------------------------------------------
// CPhoneGsmParserResult::Uid
// ---------------------------------------------------------
//
inline TUint CPhoneGsmParserResult::Uid() const
    {
    return iUid;
    }

// ---------------------------------------------------------
// CPhoneGsmParserResult::AuxInformation
// ---------------------------------------------------------
//
inline TUint CPhoneGsmParserResult::AuxInformation() const
    {
    return iAuxInformation;
    }

// ---------------------------------------------------------
// CPhoneGsmParserResult::CountParameters
// ---------------------------------------------------------
//
inline TUint CPhoneGsmParserResult::CountParameters() const
    {
    return iParameters->Count();
    }

//  End of File  
