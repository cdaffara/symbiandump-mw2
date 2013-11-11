/*
* Copyright (c) 2010 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description: This file contains testclass implementation.
*
*/

// [INCLUDE FILES] - do not remove
#include <e32svr.h>
#include <StifParser.h>
#include <StifTestInterface.h>
#include <StifTestEventInterface.h>
#include "systemtoneservicestif.h"



TInt CSystemToneServiceStif::RunMethodL(CStifItemParser& aItem )
    {

    static TStifFunctionInfo const KFunctions[] =
        {
        // Copy this line for every implemented function.
        // First string is the function name used in TestScripter script file.
        // Second is the actual implementation member function.
        ENTRY( "Create", CSystemToneServiceStif::CreateSystemToneService ),
        ENTRY( "Delete", CSystemToneServiceStif::DeleteSystemToneService ),
        ENTRY( "PlayTone", CSystemToneServiceStif::PlayTone ),
        ENTRY( "PlayAlarm", CSystemToneServiceStif::PlayAlarm ),
        ENTRY( "StopAlarm", CSystemToneServiceStif::StopAlarm ),
        ENTRY( "PlayToneWithStop", CSystemToneServiceStif::PlayToneWithStop ),	
        


        };

    const TInt count = sizeof( KFunctions ) /
                        sizeof( TStifFunctionInfo );

    return RunInternalL( KFunctions, count, aItem );

    }





void CSystemToneServiceStif::Delete()
    {
				iSts = NULL;
				iCallbackController = NULL;
    }



// -----------------------------------------------------------------------------
// CSystemToneServiceStif::ExampleL
// Example test method function.
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
TInt CSystemToneServiceStif::ExampleL( CStifItemParser& aItem )
    {

    // Print to UI
    _LIT( Ksystemtoneservicestif, "systemtoneservicestif" );
    _LIT( KExample, "In Example" );
    TestModuleIf().Printf( 0, Ksystemtoneservicestif, KExample );
    // Print to log file
    iLog->Log( KExample );

    TInt i = 0;
    TPtrC string;
    _LIT( KParam, "Param[%i]: %S" );
    while ( aItem.GetNextString ( string ) == KErrNone )
        {
        TestModuleIf().Printf( i, Ksystemtoneservicestif, 
                                KParam, i, &string );
        i++;
        }

    return KErrNone;

    }

TInt CSystemToneServiceStif::CreateSystemToneService(  )
    {
    // Print to UI
        _LIT( Ksystemtoneservicestif, "systemtoneservicestif" );
        _LIT( KPrint, "In CreateSystemToneService" );
        TestModuleIf().Printf( 0, Ksystemtoneservicestif, KPrint );
        // Print to log file
        iLog->Log( KPrint );
        
        iSts = CSystemToneService::Create();    
        
        iLog->Log(_L("CreateSystemToneService complete"));
        return KErrNone;
    }

TInt CSystemToneServiceStif::DeleteSystemToneService( )
    {
    // Print to UI
        _LIT( Ksystemtoneservicestif, "systemtoneservicestif" );
        _LIT( KPrint, "In DeleteSystemToneService" );
        TestModuleIf().Printf( 0, Ksystemtoneservicestif, KPrint );
        // Print to log file
        iLog->Log( KPrint );
        
        CSystemToneService::Delete(iSts);  
        return KErrNone;
    }
    
    
TInt CSystemToneServiceStif::PlayTone( CStifItemParser& aItem )
    {
        // Print to UI
            _LIT( Ksystemtoneservicestif, "systemtoneservicestif" );
            _LIT( KPrint, "In PlaySystemToneService" );
            TestModuleIf().Printf( 0, Ksystemtoneservicestif, KPrint );
            // Print to log file
            iLog->Log( KPrint );
            
               TInt lRetVal = KErrNone;
               TInt toneType = 0;

               lRetVal = aItem.GetNextInt(toneType);
              
        if ( lRetVal != KErrNone )
        {
            iLog->Log(_L("Csystemtoneservicestif::PlaySystemToneService tone type missing in config file "));
            iLog->Log(_L("Playing Default Tone"));
            iSts->PlayTone(CSystemToneService::EDefaultBeep);
            lRetVal = KErrNone;
        }
				else
        {
            iSts->PlayTone(CSystemToneService::TToneType(toneType));
            iLog->Log(_L("CSystemToneService::TToneType(toneType) %d"),CSystemToneService::TToneType(toneType) );
        }
        
        return lRetVal;
}


TInt CSystemToneServiceStif::PlayAlarm( CStifItemParser& aItem )
    {
        // Print to UI
            _LIT( Ksystemtoneservicestif, "systemtoneservicestif" );
            _LIT( KPrint, "In PlaySystemToneServiceWithContext" );
            TestModuleIf().Printf( 0, Ksystemtoneservicestif, KPrint );
            // Print to log file
            iLog->Log( KPrint );
            
               TInt lRetVal = KErrNone;
               TInt alarmType = 0;

               lRetVal = aItem.GetNextInt(alarmType);

               if ( lRetVal != KErrNone )
                {
                   iLog->Log(_L("CSystemToneServiceStif::PlaySystemToneService tone type missing in config file "));
                   iLog->Log(_L("Playing Default Tone"));
                   
            iSts->PlayAlarm(CSystemToneService::EClockAlarm, iCurrentContext, *this);
        }
			  else
        {
            //iSts->PlayTone(CSystemToneService::EClockAlarm, iCurrentContext);
            iSts->PlayAlarm(CSystemToneService::TAlarmType(alarmType), iCurrentContext, *this);
            iLog->Log(_L("CSystemToneService::TAlarmType(alarmType) %d"),CSystemToneService::TAlarmType(alarmType) );
        }
        
        return lRetVal;       
            
        }


TInt  CSystemToneServiceStif::StopAlarm(CStifItemParser& aItem  )
    {
    // Print to UI
          _LIT( Ksystemtoneservicestif, "systemtoneservicestif" );
          _LIT( KPrint, "In StopSystemToneService" );
          TestModuleIf().Printf( 0, Ksystemtoneservicestif, KPrint );
          // Print to log file
          iLog->Log( KPrint );
          
    TPtrC StopType;
		TInt error = KErrNone;
		
    error = aItem.GetNextString(StopType);
    
    
    if (StopType == KTrue)
    {
				iLog->Log(_L("Stop Tone with recognized context number"));
          
        iSts->StopAlarm(iCurrentContext);
		}
		else
		{
		    iLog->Log(_L("Stop Tone with unrecognized context number"));
          
        iSts->StopAlarm(iCurrentContext+1);
    }
    
    return error;
}


TInt CSystemToneServiceStif::PlayToneWithStop( CStifItemParser& aItem )
    {
        // Print to UI
            _LIT( Ksystemtoneservicestif, "systemtoneservicestif" );
            _LIT( KPrint, "In PlaySystemToneServiceWithContext" );
            TestModuleIf().Printf( 0, Ksystemtoneservicestif, KPrint );
            // Print to log file
            iLog->Log( KPrint );
            
               TInt lRetVal = KErrNone;
               TInt alarmType = 0;

               lRetVal = aItem.GetNextInt(alarmType);

               if ( lRetVal != KErrNone )
                {
                   iLog->Log(_L("CSystemToneServiceStif::PlaySystemToneService tone type missing in config file "));
                   iLog->Log(_L("Playing Default Tone"));
                   
            iSts->PlayAlarm(CSystemToneService::EClockAlarm, iCurrentContext, *this);
        }
			  else
        {
            //iSts->PlayTone(CSystemToneService::EClockAlarm, iCurrentContext);
            iSts->PlayAlarm(CSystemToneService::TToneType(alarmType), iCurrentContext, *this);
            iLog->Log(_L("CSystemToneService::TAlarmType(alarmType) %d"),CSystemToneService::TAlarmType(alarmType) );
        }
        
        return lRetVal;       
            
        }



void CSystemToneServiceStif::HandleEvent()
	{
		TEventIf event( TEventIf::ESetEvent, _L("Event_PlayAlarmComplete") );
    TestModuleIf().Event( event );
	}

	
    
void CSystemToneServiceStif::PlayAlarmComplete(unsigned int aAlarmContext)
    {
    if (aAlarmContext == iCurrentContext)
        {
        	
          _LIT( Ksystemtoneservicestif, "STS" );
          _LIT( KPrint, "PlayAlarmComplete" );
          TestModuleIf().Printf( 0, Ksystemtoneservicestif, KPrint );
                 	
        	iPlayState = EStopped;
        	iCallbackController->Cancel();
        	//HandleEvent();
        }
    }  

