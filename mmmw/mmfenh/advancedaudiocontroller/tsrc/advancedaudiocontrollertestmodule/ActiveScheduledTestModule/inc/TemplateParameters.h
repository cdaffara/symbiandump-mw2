/*
* Copyright (c) 2007 Nokia Corporation and/or its subsidiary(-ies).
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
* Description:  Active Scheduler Test Module
*
*/

// TemplateParameters.h: interface for the CTemplateParameters class.
//
//////////////////////////////////////////////////////////////////////

//#if !defined(AFX_TEMPLATEPARAMETERS_H__77EC4487_6A81_4A9B_9CAE_C4D9913B5017__INCLUDED_)
//#define AFX_TEMPLATEPARAMETERS_H__77EC4487_6A81_4A9B_9CAE_C4D9913B5017__INCLUDED_

//#if _MSC_VER > 1000
//#pragma once
//#endif // _MSC_VER > 1000

#ifndef __TEMPLATE_PARAMETERS
#define __TEMPLATE_PARAMETERS

#include "Parameters.h"

template <class DataKind>
class CTemplateParameters : public CParameters
{
protected:
	inline CTemplateParameters(TInt aAction);
public:
	inline static CTemplateParameters<DataKind>* NewL(TInt aAction, const DataKind &aData);
	inline static CTemplateParameters<DataKind>* NewLC(TInt aAction, const DataKind &aData);
	virtual ~CTemplateParameters();
	inline DataKind GetData();

private:
	inline void ConstructL(const DataKind &aData);
	DataKind iData;
};

template<class DataKind> void CTemplateParameters<DataKind>::ConstructL(const DataKind &aData)
	{
	iData = aData;
	}

template<class DataKind> CTemplateParameters<DataKind>* CTemplateParameters<DataKind>::NewLC(TInt aAction, const DataKind &aFileName)
	{
	CTemplateParameters *__self = new (ELeave) CTemplateParameters(aAction);
	CleanupStack::PushL(__self);
	__self->ConstructL(aFileName);
	return __self;
	}

template<class DataKind> CTemplateParameters<DataKind>* CTemplateParameters<DataKind>::NewL(TInt aAction, const DataKind &aData)
	{
	CTemplateParameters *__self = CTemplateParameters::NewLC(aAction, aData);
	CleanupStack::Pop(__self);
	return __self;
	}

template<class DataKind> CTemplateParameters<DataKind>::CTemplateParameters(TInt aAction) : CParameters(aAction) {}

template<class DataKind> CTemplateParameters<DataKind>::~CTemplateParameters()
	{
	//delete iFileName;
	}

template<class DataKind> DataKind CTemplateParameters<DataKind>::GetData()
	{
	return iData;
	}


//#endif // !defined(AFX_TEMPLATEPARAMETERS_H__77EC4487_6A81_4A9B_9CAE_C4D9913B5017__INCLUDED_)
#endif	//__TEMPLATE_PARAMETERS
