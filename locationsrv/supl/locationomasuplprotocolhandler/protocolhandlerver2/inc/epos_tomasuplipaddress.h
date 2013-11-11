/*
* Copyright (c) 2002-2005 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:   class for holding IPAddress. 
*
*/



#ifndef T_TOMASUPLIPADDRESS_H
#define T_TOMASUPLIPADDRESS_H

#include <e32std.h>

/**
*   TIPAddress for holding IPAddress & Port No.
*  @lib eposomasuplprotocolhandler.lib
*  @since S60 v3.1u
*/

class TIPAddress
{

public:  // Constructor 

			/**
			* C++ default constructor.
			*/
			TIPAddress();
			
			/**
			* C++ constructor.
			*/
			TIPAddress(const TDesC& aIPAddress,TInt aPortNo);
			
public: //Setter methods			
			/**
			* SetIPAddressPort Method.
			* @since S60 v3.1u
			* @param aIPAddress , IPAddress to set.
			* @param aPortNo, Port No.
			* @return None 
			*/
			void SetIPAddressPort(const TDesC& aIPAddress,TInt aPortNo);
			
public: //Getter methods						
			/**
			* IPAddress Method.
			* @since S60 v3.1u
			* @param None
			* @return IP Address 
			*/
			const TDesC& IPAddress() const;
			/**
			* PortNo Method.
			* @since S60 v3.1u
			* @param None
			* @return Port Number
			*/
			TInt PortNo() const;

private:

			// IP Address
			TBuf<64> iIPAddress;
			
			// Port Number
			TInt  	 iPortNo;
			
};

#endif // M_TOMASUPLIPADDRESS_H
