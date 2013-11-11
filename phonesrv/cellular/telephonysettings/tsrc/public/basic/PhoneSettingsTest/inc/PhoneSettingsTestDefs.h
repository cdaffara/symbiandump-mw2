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
* Description: This class is virtual abstract class for setting/cancelling and
*              inquiring divert(s).
*       
*
*
*/



#ifndef     PHONESETTINGSTESTDEFS_H
#define     PHONESETTINGSTESTDEFS_H

_LIT( KTestDefaultNumber, "12345123450" ); 
_LIT( KTestDataNumber, "1234567890" ); 
_LIT( KTestFaxNumber, "9876543210" ); 
_LIT( KTestCountryCode, "255" ); 
_LIT( KTestNetworkCode, "50" ); 
_LIT( KNetworkShortName, "TNW" ); 
_LIT( KNetworkLongName, "Test network" ); 

const TInt KPhoneSettingsTestMaxTimerValueLength = 20; 
const TInt KPhoneSettingsTestMaxPhoneNumberLength = 40; 
const TInt KPhoneSettingsTestMaxVmbxNumberLength = 100; 
const TInt KPhoneSettingsTestMaxNumbers = 20; 
const TInt KPhoneSettingsTestAsyncTimeout = 5000000;

#endif