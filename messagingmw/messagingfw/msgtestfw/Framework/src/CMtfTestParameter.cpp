// Copyright (c) 2003-2009 Nokia Corporation and/or its subsidiary(-ies).
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

/**
 @file
*/
 
#include "CMtfTestParameter.h" 
#include "CMtfTestCase.h"

/** Ownership of aParameter is taken at the END. aParameter must be on the cleanup stack.
aParameter must not be NULL. */
CMtfTestParameter* CMtfTestParameter::NewL(const TDesC& aName, 
	TMtfTestParameterGeneralType aGeneralTypeId, TMtfTestParameterSpecificType aSpecificTypeId, TAny* aParameter)
{
	__ASSERT_ALWAYS(aParameter != NULL, CMtfTestCase::Panic(CMtfTestCase::EMtfInvalidParameter));
	CMtfTestParameter* self = new(ELeave) CMtfTestParameter(aGeneralTypeId,aSpecificTypeId);
	CleanupStack::PushL(self);
	self->ConstructL(aName);
	CleanupStack::Pop(self);
	self->iParameter = aParameter;
	return self;	
}

CMtfTestParameter::CMtfTestParameter(TMtfTestParameterGeneralType aGeneralTypeId,TMtfTestParameterSpecificType aSpecificTypeId)
:iGeneralTypeId(aGeneralTypeId),iSpecificTypeId(aSpecificTypeId)
{
}

void CMtfTestParameter::ConstructL(const TDesC& aName)
{
	iName = aName.AllocL();
}



//#include "CMTFTestCase.h"

//#include <e32base.h>
#include "test/testexecutelog.h"
#include "TMtfTestParameterType.h"

/** Depending on the parameter type, attempt to cast back to the associated
type and delete the parameter. */
CMtfTestParameter::~CMtfTestParameter()
	{
	delete iName;
	
	switch (iGeneralTypeId)
		{
		case EMtfValueType:
			if(iSpecificTypeId != EMtfTTime)
				{
				delete reinterpret_cast<HBufC8*>(iParameter);
				}
			break;
		case EMtfHBufCType:
		
			switch (iSpecificTypeId)
				{
				case EMtfHBufC:
					delete reinterpret_cast<HBufC*>(iParameter); 
					break;
				case EMtfHBufC8:
					delete reinterpret_cast<HBufC8*>(iParameter); 
					break;
				default: 
					User::Panic(KMtfUnrecognisedHBufCType,0); 
					break;
				}
				break;
				
		case EMtfCBaseType:
			if(iSpecificTypeId != EMtfTTime)
				{
				delete reinterpret_cast<CBase*>(iParameter);
				}	
			break;
			
		default:
			User::Panic(KMtfParameterGeneralTypeUnrecognised,-781);
			break;
		}
	}

const TDesC& CMtfTestParameter::Name() const
	{
	// guaranteed not to be NULL
	return *iName;
	}

TMtfTestParameterGeneralType CMtfTestParameter::GeneralTypeId() const
{
	return iGeneralTypeId;
}

TMtfTestParameterSpecificType CMtfTestParameter::SpecificTypeId() const
{
	return iSpecificTypeId;
}

TAny* CMtfTestParameter::Parameter() const
{
	return iParameter;
}

