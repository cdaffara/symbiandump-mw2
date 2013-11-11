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
* Description:  Declaration of CPosApiAsynchRequestor
*
*/


#ifndef TEST_POSMSGPLUGINAPI_H
#define TEST_POSMSGPLUGINAPI_H

#include <StifLogger.h>
#include <StifParser.h>
#include <Stiftestinterface.h>
#include <epos_comasuplposhandlerbase.h>   // interface
#include <epos_momasuplobserver.h>

const TUid KOMAPOSHandlerInterfaceUid = {0x102073D4};  
class COMASuplInfoRequestList;
class COMASuplPosition;
    
class MRequestCompleteObserver
	{
    public: // New functions         
    virtual void SaveStatus(TInt aStatus) = 0;
  };
    
class CPosApiAsynchRequestor : public CActive,public MOMASuplObserver
	{
			    CPosApiAsynchRequestor();
			    
			    CPosApiAsynchRequestor(CStifLogger* aLog,MRequestCompleteObserver* aObserver);
			    
			    void ConstructL();
public :    
					static CPosApiAsynchRequestor* NewL(CStifLogger *aLog,MRequestCompleteObserver* aObserver);
					
					~CPosApiAsynchRequestor();
					
					void COMASuplPosHandlerBase_InitializeL();
					
					TInt COMASuplPosHandlerBase_CreateNewSessionL();   
					
					void COMASuplPosHandlerBase_CancelInitializeL();    
					
					TInt COMASuplPosHandlerBase_VersionL();
					
					TInt COMASuplPosHandlerBase_ImplementationUID();
					
					
					void COMASuplPosSessionBase_InitializeL();
					
					void COMASuplPosSessionBase_CancelInitializeL();
					
					void COMASuplPosSessionBase_HandleSuplPosMessageL();
					
					void COMASuplPosSessionBase_GetSuplInfoL();
					
					void COMASuplPosSessionBase_CancelSuplInfoRequest();
					
					void COMASuplPosSessionBase_GetPositionL();
					
					void COMASuplPosSessionBase_CancelGetPosition();
					
					void COMASuplPosSessionBase_SessionEndL();
					
					void COMASuplPosSessionBase_SuplStateL();
					
					TInt GetResult() 
						{
							 return iLastResult;
						}
						
					TInt COMASuplPosHandlerBase_CreateNewSession_SuplObserverL();
					
public: //MOMASuplObserver
					TReal SuplVersion() { return 1.0; }
					void TerminateSession() { }

public: //CActive
	
					void RunL();
					
					void DoCancel();
					
					TInt RunError(TInt aError);

private:	
					CStifLogger* iLog;

					MRequestCompleteObserver* iObserver;

					//Pos Handler
					COMASuplPosHandlerBase *iPosHandler;

					TInt iLastResult;

					//POS Session 
					COMASuplPosSessionBase* iPOSSession; 
					
					TUid implementationUid;
					
					COMASuplInfoRequestList* iOMASuplInfoRequestList;
					
					COMASuplPosition* iPosition;
	};
	
#endif	