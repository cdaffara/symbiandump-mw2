/**
* Copyright (c) 2003-2009 Nokia Corporation and/or its subsidiary(-ies).
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
*
*/



/**
 @file
*/


#ifndef __CMTF_TEST_ACTION_PARAMETERS_H__
#define __CMTF_TEST_ACTION_PARAMETERS_H__

#include <e32base.h>
class CMtfTestAction;

/** Represents a set of test action parameters. Indivudual parameters are accessed 
via an index. An instance with up to 8 parameters can be created. Instances requiring
more than 8 parameters can be created by creating an instance and then adding parameters to it. */
class CMtfTestActionParameters : public CBase
{
public:	
	virtual ~CMtfTestActionParameters();
	
	static CMtfTestActionParameters* NewL();
	static CMtfTestActionParameters* NewL(const TDesC& aParam1);
	static CMtfTestActionParameters* NewL(const TDesC& aParam1,const TDesC& aParam2);	
	static CMtfTestActionParameters* NewL(const TDesC& aParam1,const TDesC& aParam2,const TDesC& aParam3);
	static CMtfTestActionParameters* NewL(const TDesC& aParam1,const TDesC& aParam2,const TDesC& aParam3,const TDesC& aParam4);	
	static CMtfTestActionParameters* NewL(const TDesC& aParam1,const TDesC& aParam2,const TDesC& aParam3,const TDesC& aParam4,
						const TDesC& aParam5);
	static CMtfTestActionParameters* NewL(const TDesC& aParam1,const TDesC& aParam2,const TDesC& aParam3,const TDesC& aParam4,
						const TDesC& aParam5,const TDesC& aParam6);
	static CMtfTestActionParameters* NewL(const TDesC& aParam1,const TDesC& aParam2,const TDesC& aParam3,const TDesC& aParam4,
						const TDesC& aParam5,const TDesC& aParam6,const TDesC& aParam7);
	static CMtfTestActionParameters* NewL(const TDesC& aParam1,const TDesC& aParam2,const TDesC& aParam3,const TDesC& aParam4,
						const TDesC& aParam5,const TDesC& aParam6,const TDesC& aParam7,const TDesC& aParam8);
	
	static CMtfTestActionParameters* NewLC();
	static CMtfTestActionParameters* NewLC(const TDesC& aParam1);
	static CMtfTestActionParameters* NewLC(const TDesC& aParam1,const TDesC& aParam2);	
	static CMtfTestActionParameters* NewLC(const TDesC& aParam1,const TDesC& aParam2,const TDesC& aParam3);
	static CMtfTestActionParameters* NewLC(const TDesC& aParam1,const TDesC& aParam2,const TDesC& aParam3,const TDesC& aParam4);	
	static CMtfTestActionParameters* NewLC(const TDesC& aParam1,const TDesC& aParam2,const TDesC& aParam3,const TDesC& aParam4,
						const TDesC& aParam5);
	static CMtfTestActionParameters* NewLC(const TDesC& aParam1,const TDesC& aParam2,const TDesC& aParam3,const TDesC& aParam4,
						const TDesC& aParam5,const TDesC& aParam6);
	static CMtfTestActionParameters* NewLC(const TDesC& aParam1,const TDesC& aParam2,const TDesC& aParam3,const TDesC& aParam4,
						const TDesC& aParam5,const TDesC& aParam6,const TDesC& aParam7);
	static CMtfTestActionParameters* NewLC(const TDesC& aParam1,const TDesC& aParam2,const TDesC& aParam3,const TDesC& aParam4,
						const TDesC& aParam5,const TDesC& aParam6,const TDesC& aParam7,const TDesC& aParam8);
	
	void AddParameterL(const TDesC& aParameter);
	TInt Count() const;
	const TDesC& Parameter(TInt aParameterIndex) const;
	   		
private:
	RPointerArray<HBufC> 	iParameters;
};


#endif

