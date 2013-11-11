// Copyright (c) 1998-2009 Nokia Corporation and/or its subsidiary(-ies).
// All rights reserved.
// This component and the accompanying materials are made available
// under the terms of "Eclipse Public License v1.0"
// which accompanies this distribution, and is available
// at the URL "http://www.eclipse.org/legal/epl-v10.html".
//
// Initial Contributors:
// Nokia Corporation - initial contribution.
//
// Contributors:
//
// Description:
//



#ifndef __MSVENHANCESEARCHSORTUTIL_H__
#define __MSVENHANCESEARCHSORTUTIL_H__

#include <mtclbase.h>
#include <msvsearchsortoperation.h>

/** Used to pass condtional variables
@publishedPartner
@released
*/	

enum TMsvEnhanceSearchSortSetting 
		{
		EMessagePartToSort        		= 0x00000001,
		EMessagePartCcSort        		= 0x00000002,
		EMessagePartBccSort       		= 0x00000004,
		EMessagePartFromSort      		= 0x00000008,
		EMessagePartSubjectSort   		= 0x00000010,
		EMessagePartSearch  	    	= 0x00000020,
		EMessagePartSearchSort 		   	= 0x00000040, 
		EMessagePartSort          		= 0x00000080,
		EMessagePartLastQueryOption 	= 0x00000100, 
		EMessagePartInvalidSortField    = 0x00000200,
		EMessagePartHeaderLoaded        = 0x00000400,
		EMessagePartBodyLoaded          = 0x00000800
		};

/**
* Class: TMsvEnhanceSearchSortUtil
* 
* TMsvEnhanceSearchSortUtil is used to pass conditional variables to perform search sort operation.
* 
* @publishedPartner
* @released
*/
class TMsvEnhanceSearchSortUtil
	{
public:
	
	IMPORT_C TMsvEnhanceSearchSortUtil();
	IMPORT_C TInt32 GetSearchSortSetting(); // Get the integer value iPartList which contains the conditional flags.
	IMPORT_C void SetSearchSortSetting(TMsvEnhanceSearchSortSetting  aPartList); // Sets the flags in iPartList variable
	void  RemoveSearchSortSetting(TMsvEnhanceSearchSortSetting  aPartList);	// Removes flags in iPartList

private:
	TInt32 iPartList;	
	TAny*  iExtensionData;  //Future extension data
};

#endif