/**
* Copyright (c) 2006-2009 Nokia Corporation and/or its subsidiary(-ies).
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
* This is the header file for CT_MsgUtilsConfigFileMachineName 
* 
*
*/



/**
 @file
*/


#ifndef __T_UTILS_CONFIG_FILE_MACHINE_NAME_H__
#define __T_UTILS_CONFIG_FILE_MACHINE_NAME_H__


// epoc includes
#include <e32base.h>


/**
CT_MsgUtilsConfigFileMachineName is an utility class, can be used 
to create automatic email acounts using the machine name.
*/
class CT_MsgUtilsConfigFileMachineName : public CBase
	{
public:
	IMPORT_C static CT_MsgUtilsConfigFileMachineName* NewL(const TDesC& aPtrEmailAdd);
	IMPORT_C static CT_MsgUtilsConfigFileMachineName* NewLC(const TDesC& aPtrEmailAdd);
   ~CT_MsgUtilsConfigFileMachineName();

	IMPORT_C const TDesC8& MachineName();
	IMPORT_C const TDesC& MachineNameEmail();

private:
	IMPORT_C CT_MsgUtilsConfigFileMachineName();
	IMPORT_C void ConstructL(const TDesC& aPtrEmailAdd); 

private:
	HBufC8* iMachineName;
	HBufC* iMachineNameEmail;
	};
#endif //__T_UTILS_CONFIG_FILE_MACHINE_NAME_H__
