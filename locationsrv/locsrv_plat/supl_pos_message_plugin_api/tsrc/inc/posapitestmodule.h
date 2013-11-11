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
* Description:  Declaration of CPosAPITestModule
*
*/



#ifndef POSAPITESTMODULE_H
#define POSAPITESTMODULE_H

//  INCLUDES
#include <StifLogger.h>
#include <TestScripterInternal.h>
#include <StifTestModule.h>

#include <e32std.h>
#include <e32base.h>
#include <epos_comasuplposhandlerbase.h>
#include <epos_comasuplpospayload.h>
#include <epos_comasuplinforequestlist.h>
#include <epos_comasuplposition.h>
#include <epos_comasuplvelocity.h>
#include <epos_comasuplsetcapabilities.h>
#include <epos_comasuplreqasstdata.h>



// Logging path
_LIT( KPosAPITestModuleLogPath, "\\logs\\testframework\\PosAPITestModule\\" ); 
// Log file
_LIT( KPosAPITestModuleLogFile, "PosAPITestModule.txt" ); 

// FORWARD DECLARATIONS
class CPosAPITestModule;

/////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////
// CLASS DECLARATION

/**
*  CPosAPITestModule test class for STIF Test Framework TestScripter.
*  ?other_description_lines
*
*  @lib 
*  @since S60 3.2
*/
NONSHARABLE_CLASS(CPosAPITestModule) : public CScriptBase
    {
    public:  // Constructors and destructor

        /**
        * Two-phased constructor.
        */
        static CPosAPITestModule* NewL( CTestModuleIf& aTestModuleIf );

        /**
        * Destructor.
        */
        virtual ~CPosAPITestModule();

    public: // New functions
	       /**
	        * COMASuplInfoRequestList_NewL method.
	        * @since S60_31
	        * @param aItem Script line containing parameters.
	        * @return Symbian OS error code.
	        */
			TInt COMASuplInfoRequestList_NewL( CStifItemParser& aItem );
	       /**
	        * COMASuplInfoRequestList_AppendL method.
	        * @since S60_31
	        * @param aItem Script line containing parameters.
	        * @return Symbian OS error code.
	        */
			TInt COMASuplInfoRequestList_AppendL( CStifItemParser& aItem );
	       /**
	        * COMASuplInfoRequestList_GetCountL method.
	        * @since S60_31
	        * @param aItem Script line containing parameters.
	        * @return Symbian OS error code.
	        */
			TInt COMASuplInfoRequestList_GetCountL( CStifItemParser& aItem );
	       /**
	        * COMASuplInfoRequestList_GetElementL method.
	        * @since S60_31
	        * @param aItem Script line containing parameters.
	        * @return Symbian OS error code.
	        */
			TInt COMASuplInfoRequestList_GetElementL( CStifItemParser& aItem );
			
			COMASuplInfoRequestList* COMASuplInfoRequestList_NewL();
			COMASuplPosition* COMASuplPosition_NewL();
			COMASuplSETCapabilities* COMASuplSETCapabilities_NewL();


	       /**
	        * COMASuplInfoRequest_NewL method.
	        * @since S60_31
	        * @param aItem Script line containing parameters.
	        * @return Symbian OS error code.
	        */
			TInt COMASuplInfoRequest_NewL( CStifItemParser& /*aItem*/ );
	       /**
	        * COMASuplInfoRequest_TypeL method.
	        * @since S60_31
	        * @param aItem Script line containing parameters.
	        * @return Symbian OS error code.
	        */
			TInt COMASuplInfoRequest_TypeL( CStifItemParser& /*aItem*/ );
	       /**
	        * COMASuplInfoRequest_StatusL method.
	        * @since S60_31
	        * @param aItem Script line containing parameters.
	        * @return Symbian OS error code.
	        */
			TInt COMASuplInfoRequest_StatusL( CStifItemParser& /*aItem*/ );
	       /**
	        * COMASuplInfoRequest_CloneL method.
	        * @since S60_31
	        * @param aItem Script line containing parameters.
	        * @return Symbian OS error code.
	        */
			TInt COMASuplInfoRequest_CloneL( CStifItemParser& /*aItem*/ );


	       /**
	        * TOMASuplAltitudeInfo_Set_Get method.
	        * @since S60_31
	        * @param aItem Script line containing parameters.
	        * @return Symbian OS error code.
	        */
			TInt TOMASuplAltitudeInfo_Set_Get( CStifItemParser& /*aItem*/ );
	       /**
	        * TOMASuplUncertainty_Set_Get method.
	        * @since S60_31
	        * @param aItem Script line containing parameters.
	        * @return Symbian OS error code.
	        */
			TInt TOMASuplUncertainty_Set_Get( CStifItemParser& /*aItem*/ );
	       /**
	        * TOMASuplPositionEstimate_Set_Get method.
	        * @since S60_31
	        * @param aItem Script line containing parameters.
	        * @return Symbian OS error code.
	        */
			TInt TOMASuplPositionEstimate_Set_Get( CStifItemParser& /*aItem*/ );
	       /**
	        * TOMASuplPositionEstimate_Set_Get_Confidance_AltitudeInfo method.
	        * @since S60_31
	        * @param aItem Script line containing parameters.
	        * @return Symbian OS error code.
	        */
			TInt TOMASuplPositionEstimate_Set_Get_Confidance_AltitudeInfo( CStifItemParser& /*aItem*/ );
	       /**
	        * TOMASuplUtcTime_Set_Get method.
	        * @since S60_31
	        * @param aItem Script line containing parameters.
	        * @return Symbian OS error code.
	        */
			TInt TOMASuplUtcTime_Set_Get( CStifItemParser& /*aItem*/ );
	       /**
	        * COMASuplPosition_Set_GetL method.
	        * @since S60_31
	        * @param aItem Script line containing parameters.
	        * @return Symbian OS error code.
	        */
			TInt COMASuplPosition_Set_GetL( CStifItemParser& /*aItem*/ );
	       /**
	        * COMASuplPosition_SetSuplVelocityL method.
	        * @since S60_31
	        * @param aItem Script line containing parameters.
	        * @return Symbian OS error code.
	        */
			
			TInt COMASuplPosition_SetSuplVelocityL( CStifItemParser& /*aItem*/ );
	       /**
	        * COMASuplPosition_CloneL method.
	        * @since S60_31
	        * @param aItem Script line containing parameters.
	        * @return Symbian OS error code.
	        */
			TInt COMASuplPosition_CloneL( CStifItemParser& /*aItem*/ );
	       /**
	        * COMASuplPosPayload_SuplPayloadTypeL method.
	        * @since S60_31
	        * @param aItem Script line containing parameters.
	        * @return Symbian OS error code.
	        */
			TInt COMASuplPosPayload_SuplPayloadTypeL( CStifItemParser& /*aItem*/ );
	       /**
	        * COMASuplPosPayload_CloneL method.
	        * @since S60_31
	        * @param aItem Script line containing parameters.
	        * @return Symbian OS error code.
	        */
			TInt COMASuplPosPayload_CloneL( CStifItemParser& /*aItem*/ );
			
	       /**
	        * COMASuplSetCapabilities_Set_GetL method.
	        * @since S60_31
	        * @param aItem Script line containing parameters.
	        * @return Symbian OS error code.
	        */
			TInt COMASuplSetCapabilities_Set_GetL( CStifItemParser& /*aItem*/ );
	       /**
	        * COMASuplSetCapabilities_CloneL method.
	        * @since S60_31
	        * @param aItem Script line containing parameters.
	        * @return Symbian OS error code.
	        */
			TInt COMASuplSetCapabilities_CloneL( CStifItemParser& aItem );
	       /**
	        * COMASuplVelocity_Set_GetL method.
	        * @since S60_31
	        * @param aItem Script line containing parameters.
	        * @return Symbian OS error code.
	        */
			TInt COMASuplVelocity_Set_GetL( CStifItemParser& aItem );
	       /**
	        * COMASuplVelocity_CloneL method.
	        * @since S60_31
	        * @param aItem Script line containing parameters.
	        * @return Symbian OS error code.
	        */
			TInt COMASuplVelocity_CloneL( CStifItemParser& aItem );
	       /**
	        * COMASuplHorizVelocity_Get_Set_HorizVelL method.
	        * @since S60_31
	        * @param aItem Script line containing parameters.
	        * @return Symbian OS error code.
	        */
			TInt COMASuplHorizVelocity_Get_Set_HorizVelL( CStifItemParser& aItem );
	       /**
	        * COMASuplHorizAndVertVelocity_Set_GetL method.
	        * @since S60_31
	        * @param aItem Script line containing parameters.
	        * @return Symbian OS error code.
	        */
			TInt COMASuplHorizAndVertVelocity_Set_GetL( CStifItemParser& aItem );
	       /**
	        * COMASuplHorizUncertVelocity_Set_GetL method.
	        * @since S60_31
	        * @param aItem Script line containing parameters.
	        * @return Symbian OS error code.
	        */
			TInt COMASuplHorizUncertVelocity_Set_GetL( CStifItemParser& aItem );
	       /**
	        * COMASuplHorizAndVertUncertVelocity_Set_GetL method.
	        * @since S60_31
	        * @param aItem Script line containing parameters.
	        * @return Symbian OS error code.
	        */
			TInt COMASuplHorizAndVertUncertVelocity_Set_GetL( CStifItemParser& aItem );

			TOMASuplSatelliteInfoElement TOMASuplSatelliteInfoElement_SatElement( TInt aSatId, TInt aIode, TInt& aError);
	       /**
	        * TOMASuplSatelliteInfoElement_Set_Get method.
	        * @since S60_31
	        * @param aItem Script line containing parameters.
	        * @return Symbian OS error code.
	        */
			TInt TOMASuplSatelliteInfoElement_Set_Get( CStifItemParser& aItem );
	       /**
	        * TOMASuplNavigationModel_Set_GetSatInfoElement_AllElements method.
	        * @since S60_31
	        * @param aItem Script line containing parameters.
	        * @return Symbian OS error code.
	        */
			TInt TOMASuplNavigationModel_Set_GetSatInfoElement_AllElements( CStifItemParser& aItem );
	       /**
	        * TOMASuplNavigationModel_Set_GetSatInfoElement_SpecificElement method.
	        * @since S60_31
	        * @param aItem Script line containing parameters.
	        * @return Symbian OS error code.
	        */
			TInt TOMASuplNavigationModel_Set_GetSatInfoElement_SpecificElement( CStifItemParser& aItem );
	       /**
	        * TOMASuplNavigationModel_Set_GetNavigationModel method.
	        * @since S60_31
	        * @param aItem Script line containing parameters.
	        * @return Symbian OS error code.
	        */
			TInt TOMASuplNavigationModel_Set_GetNavigationModel( CStifItemParser& aItem );
	       /**
	        * COMASuplReqAsstData_Set_GetNavigationDataL method.
	        * @since S60_31
	        * @param aItem Script line containing parameters.
	        * @return Symbian OS error code.
	        */
			TInt COMASuplReqAsstData_Set_GetNavigationDataL( CStifItemParser& aItem );
	       /**
	        * COMASuplReqAsstData_Set_GetReqAsstDataL method.
	        * @since S60_31
	        * @param aItem Script line containing parameters.
	        * @return Symbian OS error code.
	        */
			TInt COMASuplReqAsstData_Set_GetReqAsstDataL ( CStifItemParser& aItem );
	       /**
	        * COMASuplReqAsstData_CloneL method.
	        * @since S60_31
	        * @param aItem Script line containing parameters.
	        * @return Symbian OS error code.
	        */
			TInt COMASuplReqAsstData_CloneL(CStifItemParser& aItem );
	       /**
	        * COMASuplPosHandlerBase_InitializeL method.
	        * @since S60_31
	        * @param aItem Script line containing parameters.
	        * @return Symbian OS error code.
	        */
			TInt COMASuplPosHandlerBase_InitializeL(CStifItemParser& aItem );
	       /**
	        * COMASuplPosHandlerBase_CreateNewSessionL method.
	        * @since S60_31
	        * @param aItem Script line containing parameters.
	        * @return Symbian OS error code.
	        */
			TInt COMASuplPosHandlerBase_CreateNewSessionL(CStifItemParser& aItem );
	       /**
	        * COMASuplPosHandlerBase_CancelInitializeL method.
	        * @since S60_31
	        * @param aItem Script line containing parameters.
	        * @return Symbian OS error code.
	        */
			TInt COMASuplPosHandlerBase_CancelInitializeL(CStifItemParser& /*aItem*/);
	       /**
	        * COMASuplPosHandlerBase_VersionL method.
	        * @since S60_31
	        * @param aItem Script line containing parameters.
	        * @return Symbian OS error code.
	        */
			TInt COMASuplPosHandlerBase_VersionL(CStifItemParser& /*aItem*/);
	       /**
	        * COMASuplPosHandlerBase_ImplementationUIDL method.
	        * @since S60_31
	        * @param aItem Script line containing parameters.
	        * @return Symbian OS error code.
	        */
			TInt COMASuplPosHandlerBase_ImplementationUIDL(CStifItemParser& /*aItem*/);
			
			
	       /**
	        * COMASuplPosSessionBase_InitializeL method.
	        * @since S60_31
	        * @param aItem Script line containing parameters.
	        * @return Symbian OS error code.
	        */
			TInt COMASuplPosSessionBase_InitializeL(CStifItemParser& /*aItem*/);
	       /**
	        * COMASuplPosSessionBase_CancelInitializeL method.
	        * @since S60_31
	        * @param aItem Script line containing parameters.
	        * @return Symbian OS error code.
	        */
			TInt COMASuplPosSessionBase_CancelInitializeL(CStifItemParser& /*aItem*/);
	       /**
	        * COMASuplPosSessionBase_HandleSuplPosMessageL method.
	        * @since S60_31
	        * @param aItem Script line containing parameters.
	        * @return Symbian OS error code.
	        */
			TInt COMASuplPosSessionBase_HandleSuplPosMessageL(CStifItemParser& /*aItem*/);
	       /**
	        * COMASuplPosSessionBase_GetSuplInfoL method.
	        * @since S60_31
	        * @param aItem Script line containing parameters.
	        * @return Symbian OS error code.
	        */
			TInt COMASuplPosSessionBase_GetSuplInfoL(CStifItemParser& /*aItem*/);
	       /**
	        * COMASuplPosSessionBase_CancelSuplInfoRequestL method.
	        * @since S60_31
	        * @param aItem Script line containing parameters.
	        * @return Symbian OS error code.
	        */
			TInt COMASuplPosSessionBase_CancelSuplInfoRequestL(CStifItemParser& /*aItem*/);
	       /**
	        * COMASuplPosSessionBase_GetPositionL method.
	        * @since S60_31
	        * @param aItem Script line containing parameters.
	        * @return Symbian OS error code.
	        */
			TInt COMASuplPosSessionBase_GetPositionL(CStifItemParser& /*aItem*/);
	       /**
	        * COMASuplPosSessionBase_CancelGetPositionL method.
	        * @since S60_31
	        * @param aItem Script line containing parameters.
	        * @return Symbian OS error code.
	        */
			TInt COMASuplPosSessionBase_CancelGetPositionL(CStifItemParser& /*aItem*/);
			
	       /**
	        * COMASuplPosSessionBase_SessionEndL method.
	        * @since S60_31
	        * @param aItem Script line containing parameters.
	        * @return Symbian OS error code.
	        */
			TInt COMASuplPosSessionBase_SessionEndL(CStifItemParser& /*aItem*/);
	       /**
	        * COMASuplPosSessionBase_SuplStateL method.
	        * @since S60_31
	        * @param aItem Script line containing parameters.
	        * @return Symbian OS error code.
	        */
			TInt COMASuplPosSessionBase_SuplStateL(CStifItemParser& /*aItem*/);
	       /**
	        * COMASuplPosHandlerBase_CreateNewSession_SuplObserverL method.
	        * @since S60_31
	        * @param aItem Script line containing parameters.
	        * @return Symbian OS error code.
	        */
			TInt COMASuplPosHandlerBase_CreateNewSession_SuplObserverL(CStifItemParser& /*aItem*/);

	public: // Functions from base classes

        /**
        * From CScriptBase Runs a script line.
        * @since ?Series60_version
        * @param aItem Script line containing method name and parameters
        * @return Symbian OS error code
        */
        virtual TInt RunMethodL( CStifItemParser& aItem );

    private:

        /**
        * C++ default constructor.
        */
        CPosAPITestModule( CTestModuleIf& aTestModuleIf );

        /**
        * By default Symbian 2nd phase constructor is private.
        */
        void ConstructL();

        // Prohibit copy constructor if not deriving from CBase.
        // ?classname( const ?classname& );
        // Prohibit assigment operator if not deriving from CBase.
        // ?classname& operator=( const ?classname& );

        /**
        * Frees all resources allocated from test methods.
        * @since ?Series60_version
        */
        void Delete();

        /**
        * Test methods are listed below. 
        */

        /**
        * Example test method.
        * @since ?Series60_version
        * @param aItem Script line containing parameters.
        * @return Symbian OS error code.
        */
        virtual TInt ExampleL( CStifItemParser& aItem );

    public:     // Data

    protected:  // Data
        // ?one_line_short_description_of_data
        //?data_declaration;

    private:    // Data
        
        // ?one_line_short_description_of_data
        //?data_declaration;

        // Reserved pointer for future extension
        //TAny* iReserved;

    public:     // Friend classes
        //?friend_class_declaration;
    protected:  // Friend classes
        //?friend_class_declaration;
    private:    // Friend classes
        //?friend_class_declaration;

    };

#endif      // POSAPITESTMODULE_H

// End of File
