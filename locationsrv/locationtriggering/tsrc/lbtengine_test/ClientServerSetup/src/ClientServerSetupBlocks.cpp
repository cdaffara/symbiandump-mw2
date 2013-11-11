/*
* Copyright (c) 2002 - 2007 Nokia Corporation and/or its subsidiary(-ies).
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
* Description:    Test module to implement Server startup related test cases
*
*/





// INCLUDE FILES
#include <e32svr.h>
#include <StifParser.h>
#include <Stiftestinterface.h>
#include <Stiftestmodule.h>
#include "ClientServerSetup.h"
#include <lbt.h> 
#include <lbtserver.h>
#include <e32property.h>
//#include "..\..\..\..\lbt\inc\lbtserverconsts.h"
//#include "lbtstatussdkpskeys.h"
//#include "lbtserverconsts.h"


// EXTERNAL DATA STRUCTURES
//extern  ?external_data;

// EXTERNAL FUNCTION PROTOTYPES  
//extern ?external_function( ?arg_type,?arg_type );

// CONSTANTS
//const ?type ?constant_var = ?constant;

// MACROS
//#define ?macro ?macro_def

// LOCAL CONSTANTS AND MACROS
//const ?type ?constant_var = ?constant;
//#define ?macro_name ?macro_def

// MODULE DATA STRUCTURES
//enum ?declaration
//typedef ?declaration

// LOCAL FUNCTION PROTOTYPES
//?type ?function_name( ?arg_type, ?arg_type );

// FORWARD DECLARATIONS
//class ?FORWARD_CLASSNAME;

// ============================= LOCAL FUNCTIONS ===============================

// -----------------------------------------------------------------------------
// ?function_name ?description.
// ?description
// Returns: ?value_1: ?description
//          ?value_n: ?description_line1
//                    ?description_line2
// -----------------------------------------------------------------------------
//
/*
?type ?function_name(
    ?arg_type arg,  // ?description
    ?arg_type arg)  // ?description
    {

    ?code  // ?comment

    // ?comment
    ?code
    }
*/

// ============================ MEMBER FUNCTIONS ===============================

// -----------------------------------------------------------------------------
// CClientServerSetup::Delete
// Delete here all resources allocated and opened from test methods. 
// Called from destructor. 
// -----------------------------------------------------------------------------
//
void CClientServerSetup::Delete() 
    {

    }

// -----------------------------------------------------------------------------
// CClientServerSetup::RunMethodL
// Run specified method. Contains also table of test mothods and their names.
// -----------------------------------------------------------------------------
//
TInt CClientServerSetup::RunMethodL( 
    CStifItemParser& aItem ) 
    {

    static TStifFunctionInfo const KFunctions[] =
        {  
        // Copy this line for every implemented function.
        // First string is the function name used in TestScripter script file.
        // Second is the actual implementation member function. 
      	ENTRY( "EstablishAndCloseConnection", CClientServerSetup::EstablishAndCloseConnectionL ),
		ENTRY( "EstablishMultipleConnection", CClientServerSetup::EstablishMultipleConnectionL ),
		ENTRY( "MultipleServerMultipleConnection", CClientServerSetup::MultipleServerMultipleConnectionL ),
		ENTRY( "ErrorAlreadyConnected", CClientServerSetup::ErrorAlreadyConnectedL ),
		ENTRY( "ErrorServerNotRunning", CClientServerSetup::ErrorServerNotRunningL ),
		ENTRY( "ErrorAlreadyClosed", CClientServerSetup::ErrorAlreadyClosedL ),
		ENTRY( "ErrorCloseWithoutConnect", CClientServerSetup::ErrorCloseWithoutConnectL ),
		ENTRY( "OpenConnection", CClientServerSetup::OpenConnectionL ),
		ENTRY( "ErrorOpenMultipleConnection", CClientServerSetup::ErrorOpenMultipleConnectionL ),
		ENTRY( "MultipleClientMultipleConnection", CClientServerSetup::MultipleClientMultipleConnectionL ),
		ENTRY( "ErrorServerNotConnected", CClientServerSetup::ErrorServerNotConnectedL ),
		ENTRY( "ErrorSubsessionNotClosed", CClientServerSetup::ErrorSubsessionNotClosedL ),
		ENTRY( "ErrorSubsessionAlreadyClosed", CClientServerSetup::ErrorSubsessionAlreadyClosedL ),
		ENTRY( "GetLBTServerVersion", CClientServerSetup::GetLBTServerVersionL ),
		ENTRY( "ErrorGetLBTServerVersion", CClientServerSetup::ErrorGetLBTServerVersionL ),
		ENTRY( "FindServer", CClientServerSetup::FindServerL ),
		ENTRY( "StatusInformationSupervisionNotActive", CClientServerSetup::StatusInformationSupervisionNotActiveL ),
		ENTRY( "RequestStatusChangeNotification", CClientServerSetup::RequestStatusChangeNotificationL ),
		ENTRY( "killLBTServer", CClientServerSetup::killLBTServerL ),
        };

    const TInt count = sizeof( KFunctions ) / 
                        sizeof( TStifFunctionInfo );

    return RunInternalL( KFunctions, count, aItem );

    }

// -----------------------------------------------------------------------------
// CClientServerSetup::EstablishConnection
// Description  :: This Method is Testing Connect 
// 				   functanality of RLbtServer.
// Return Value :: On Success KErrNone else System wide error code.   
// -----------------------------------------------------------------------------
//
TInt CClientServerSetup::EstablishConnectionL( CStifItemParser& aItem )
    {

    // Print to UI
    TestModuleIf().Printf( 0, _L("CClientServerSetup"),_L("EstablishConnection"));      
    //collecting logs
    iLog->Log(_L("Start Connection"));     
    //Create instance of RLbtServer
    RLbtServer	lbtServ;
    //RLbtServer clientObject;//testing
    //Connect to RLbtServer
    TInt retConnect = lbtServ.Connect();
    if(retConnect!=KErrNone)
    	{
    	iLog->Log(_L("Connection to RLbtServer Failed %d"),retConnect);     
    	return retConnect;
    	}
    CleanupClosePushL(lbtServ);	
    iLog->Log(_L("Connection to RLbtServer Passed ")); 	
    
   return KErrNone;

    }
    
// -----------------------------------------------------------------------------
// CClientServerSetup::EstablishAndCloseConnection
// Description  :: This Method is Testing Connect and
// 				   Close functanality of RLbtServer.
// Return Value :: On Success KErrNone else System wide error code.   
// -----------------------------------------------------------------------------
//
TInt CClientServerSetup::EstablishAndCloseConnectionL( CStifItemParser& aItem )
    {

    // Print to UI
    TestModuleIf().Printf( 0, _L("CClientServerSetup"),_L("EstablishAndCloseConnection"));      
    //collecting logs
    iLog->Log(_L("Start Connection"));     
    //Create instance of RLbtServer
    RLbtServer lbtServ;
       //Connect to RLbtServer
    TInt retConnect = lbtServ.Connect();
    if(retConnect!=KErrNone)
    	{
    	iLog->Log(_L("Connection to RLbtServer Failed %d"),retConnect);     
    	return retConnect;
    	}
    CleanupClosePushL(lbtServ);		
    iLog->Log(_L("Connection to RLbtServer Passed ")); 	
    
    //Close connection to RLbtServer
    lbtServ.Close();    
    CleanupStack::PopAndDestroy(&lbtServ);
    iLog->Log(_L("close Connection"));
    //Return value			
    return KErrNone;

    }
    
    
    // -----------------------------------------------------------------------------
// CClientServerSetup::EstablishMultipleConnection
// Description  :: This Method is Testing Establishing and closing 
// 				     connections to RLbtServer multiple times.
// Return Value :: On Success KErrNone else System wide error code.   
// -----------------------------------------------------------------------------
//
TInt CClientServerSetup::EstablishMultipleConnectionL( CStifItemParser& aItem )
    {

    // Print to UI
    TestModuleIf().Printf( 0, _L("CClientServerSetup"),_L("EstablishConnection"));      
    //collecting logs
    iLog->Log(_L("Start Connection"));     
    //Create instance of RLbtServer
    RLbtServer	lbtServ;
       //Connect to RLbtServer
    for(TInt i=0;i<10;i++)
    {
     TInt retConnect = lbtServ.Connect();
	    if(retConnect!=KErrNone)
	    	{
	    	iLog->Log(_L("Connection to RLbtServer Failed %d"),retConnect);     
	    	return retConnect;
	    	}
	    iLog->Log(_L("Connection to RLbtServer Passed ")); 	
	    CleanupClosePushL(lbtServ);
	    //Close connection to RSuplNetworkServer
	    lbtServ.Close(); 
	    CleanupStack::PopAndDestroy(&lbtServ);   
	    iLog->Log(_L("close Connection"));	
    }
	   
    //Return value			
    return KErrNone;

    }
    
        // -----------------------------------------------------------------------------
// CClientServerSetup::MultipleServerMultipleConnection
// Description  :: This Method is Testing Connect and
// 				   Close functanality to multiple server sessions of RLbtServer.
// Return Value :: On Success KErrNone else System wide error code.   
// -----------------------------------------------------------------------------
//
TInt CClientServerSetup::MultipleServerMultipleConnectionL( CStifItemParser& aItem )
    {

    // Print to UI
    TestModuleIf().Printf( 0, _L("CClientServerSetup"),_L("EstablishConnection"));      
    //collecting logs
    iLog->Log(_L("Creating 10 Instances of Server"));     
    //Create instance of RLbtServer
    RLbtServer	lbtServ[5];
       //Connect to RLbtServer
    for(TInt i=0;i<5;i++)
    {
     TInt retConnect = lbtServ[i].Connect();
	    if(retConnect!=KErrNone)
	    	{
	    	iLog->Log(_L("Connection to RLbtServer Failed %d"),retConnect);     
	    	return retConnect;
	    	}
	    CleanupClosePushL(lbtServ[i]);	
	    iLog->Log(_L("Connection to RLbtServer %d Passed "),i); 	
 	    
    }
    for(TInt i=0;i<5;i++)
    {
    	lbtServ[i].Close();    
	    iLog->Log(_L("close Connection of %d"),i);
	   // CleanupStack::PopAndDestroy(&lbtServ[i]);  		
    }
    CleanupStack::PopAndDestroy(5); 
       
    //Return value			
    return KErrNone;

    }
    
    
    // -----------------------------------------------------------------------------
// CClientServerSetup::ErrorAlreadyConnected
// Description  :: This Method is trying to connect to server when
//				   it is already connected 	
// Return Value :: On failure System wide error code.  
// Panics		:: panics with ELbtServerHandleNotClosed
// -----------------------------------------------------------------------------
//
TInt CClientServerSetup::ErrorAlreadyConnectedL( CStifItemParser& aItem )
    {

    // Print to UI
    TestModuleIf().Printf( 0, _L("CClientServerSetup"),_L("ErrorAlreadyConnected"));      
    //collecting logs
    iLog->Log(_L("Start Connection"));     
    //Create instance of RLbtServer
    RLbtServer	lbtServ;
        //Connect to RLbtServer
    TInt retConnect = lbtServ.Connect();
    if(retConnect!=KErrNone)
    	{
    	iLog->Log(_L("Connection to RLbtServer Failed %d"),retConnect);     
    	return retConnect;
    	}
    CleanupClosePushL(lbtServ);	
    iLog->Log(_L("Connection to RLbtServer Passed "));
    iLog->Log(_L("Trying to connect again ")); 
    TestModuleIf().SetExitReason( CTestModuleIf::EPanic, 1);  	
    TInt err= lbtServ.Connect();
    if(err!=KErrNone)
    	{
    	iLog->Log(_L("Connection to RLbtServer Failed %d"),retConnect); 
       
    	}
 
    return KErrNone;

    }
    
// -----------------------------------------------------------------------------
// CClientServerSetup::ErrorServerNotRunning
// Description  :: This Method is trying to connect to server 
//				   when server is not running.	
// Return Value :: KErrNotSupported if server not running On failure System wide error code.  
//  -----------------------------------------------------------------------------
//
    
    TInt CClientServerSetup::ErrorServerNotRunningL( CStifItemParser& aItem )
    {

    // Print to UI
    TestModuleIf().Printf( 0, _L("CClientServerSetup"),_L("EstablishConnection"));      
    //collecting logs
    iLog->Log(_L("Start Connection"));     
    //Create instance of RLbtServer
    RLbtServer	lbtServ;
        //Connect to RLbtServer
    TInt retConnect = lbtServ.Connect();
    if(retConnect!=KErrNotSupported)
    	{
    	iLog->Log(_L("Connection to RLbtServer Failed %d"),retConnect);     
    	return KErrNone;
    	}
    iLog->Log(_L("Connection to RLbtServer Passed ")); 	
    
    //Return value			
    return -99;

    }
    
    
    // -----------------------------------------------------------------------------
// CClientServerSetup::ErrorAlreadyClosed
// Description  :: This Method is trying to close connection when it is already closed
// Return Value :: On Success KErrNone else System wide error code.   
// -----------------------------------------------------------------------------
//
TInt CClientServerSetup::ErrorAlreadyClosedL( CStifItemParser& aItem )
    {

    // Print to UI
    TestModuleIf().Printf( 0, _L("CClientServerSetup"),_L("EstablishConnection"));      
    //collecting logs
    iLog->Log(_L("Start Connection"));     
    //Create instance of RLbtServer
    RLbtServer	lbtServ;
    //RLbtServer clientObject;//testing
    //Connect to RLbtServer
    TInt retConnect = lbtServ.Connect();
    if(retConnect!=KErrNone)
    	{
    	iLog->Log(_L("Connection to RLbtServer Failed %d"),retConnect);     
    	return retConnect;
    	}
    iLog->Log(_L("Connection to RLbtServer Passed ")); 	
    CleanupClosePushL(lbtServ);	
    //Close connection to RLbtServer
    lbtServ.Close();    
    iLog->Log(_L("close Connection"));
    CleanupStack::PopAndDestroy(&lbtServ);    
   
   iLog->Log(_L("close Connection again"));
    lbtServ.Close();    
    iLog->Log(_L(" Connection closed again"));
    //Return value			
    return KErrNone;

    }
    
    
    
        // -----------------------------------------------------------------------------
// CClientServerSetup::ErrorCloseWithoutConnect
// Description  :: This Method is trying to close connection without  
//					opening the connection.
// Return Value :: On Success KErrNone else System wide error code.   
// -----------------------------------------------------------------------------
//
TInt CClientServerSetup::ErrorCloseWithoutConnectL( CStifItemParser& aItem )
    {

    // Print to UI
    TestModuleIf().Printf( 0, _L("CClientServerSetup"),_L("EstablishConnection"));      
    //collecting logs
    iLog->Log(_L("Start Connection"));     
    //Create instance of RLbtServer
    RLbtServer	lbtServ;
    //RLbtServer clientObject;//testing
    
    lbtServ.Close();    
    iLog->Log(_L("close Connection without connect"));
    
    //Return value			
    return KErrNone;

    }
    
    
    // -----------------------------------------------------------------------------
// CClientServerSetup::OpenConnection
// Description  :: This Method is Testing Open and
// 				   Close functanality of RLbt client.
// Return Value :: On Success KErrNone else System wide error code.   
// -----------------------------------------------------------------------------
//
TInt CClientServerSetup::OpenConnectionL( CStifItemParser& aItem )
    {
    // Print to UI
    TestModuleIf().Printf( 0, _L("CClientServerSetup"),_L("OpenConnection"));      
    //collecting logs
    iLog->Log(_L("Start Connection"));     
    //Create instance of RLbtServer
    RLbtServer	lbtServ;
    RLbt lbt;
        //Connect to RLbtServer
    TInt retConnect = lbtServ.Connect();
    CleanupClosePushL(lbtServ);	
    iLog->Log(_L("Connection to RLbtServer Passed ")); 	
    
    TInt retOpen = lbt.Open(lbtServ);
    if(retOpen!=KErrNone)
    	{
    	iLog->Log(_L("Opening subsession to RLbtServer Failed %d"),retOpen);     
    	return retOpen;
    	}
    CleanupClosePushL(lbt);	
    //Closes the subsession
    lbt.Close();
   //CleanupStack::PopAndDestroy(&lbt); 
    //Close connection to RLbtServer
    lbtServ.Close(); 
    CleanupStack::Pop(&lbt); 
    CleanupStack::PopAndDestroy(&lbtServ);       
    iLog->Log(_L("close Connection"));
    //Return value			
    return KErrNone;

    }
    
    
    
        // -----------------------------------------------------------------------------
// CClientServerSetup::ErrorOpenMultipleConnection
// Description  :: This Method is trying to open subsession when it is already opened.
// Return Value :: On Success KErrNone else System wide error code.   
// -----------------------------------------------------------------------------
//
TInt CClientServerSetup::ErrorOpenMultipleConnectionL( CStifItemParser& aItem )
    {

    // Print to UI
    TestModuleIf().Printf( 0, _L("CClientServerSetup"),_L("OpenConnection"));      
    //collecting logs
    iLog->Log(_L("Start Connection"));     
    //Create instance of RLbtServer
    RLbtServer	lbtServ;
    RLbt lbt;
        //Connect to RLbtServer
    TInt retConnect = lbtServ.Connect();
    CleanupClosePushL(lbtServ);	
    
    TInt retOpen = lbt.Open(lbtServ);
	if(retOpen!=KErrNone)
	{
	iLog->Log(_L("Opening subsession to RLbtServer Failed %d"),retOpen);     
	return retOpen;
	}
	CleanupClosePushL(lbt);
	TestModuleIf().SetExitReason( CTestModuleIf::EPanic, 2); 
	retOpen = lbt.Open(lbtServ);
	if(retOpen!=KErrNone)
	{
	iLog->Log(_L("Opening subsession to RLbtServer Failed %d"),retOpen); 
	 return KErrNone;   
    }
    	
	        
        //Closes the subsession
	    lbt.Close();	
   		CleanupStack::Pop(&lbt); 
    
    //Close connection to RLbtServer
    lbtServ.Close();
    CleanupStack::PopAndDestroy(&lbtServ);      
    iLog->Log(_L("close Connection"));
    //Return value			
    return KErrNone;

    }
    
    
            // -----------------------------------------------------------------------------
// CClientServerSetup::MultipleClientMultipleConnection
// Description  :: This Method is Testing multiple client interfaces 
//					opening and closing subsessions
// Return Value :: On Success KErrNone else System wide error code.   
// -----------------------------------------------------------------------------
//
TInt CClientServerSetup::MultipleClientMultipleConnectionL( CStifItemParser& aItem )
    {

    // Print to UI
    TestModuleIf().Printf( 0, _L("CClientServerSetup"),_L("OpenConnection"));      
    //collecting logs
    iLog->Log(_L("Start Connection"));     
    //Create instance of RLbtServer
    RLbtServer	lbtServ;
    RLbt lbt[5];
    //RLbtServer serverObject;//testing
    //Connect to RLbtServer
    TInt retConnect = lbtServ.Connect();
    CleanupClosePushL(lbtServ);	
    iLog->Log(_L("Connection to RLbtServer Passed ")); 	
    for(TInt i =0;i<5;i++)
    {
    	TInt retOpen = lbt[i].Open(lbtServ);
    	if(retOpen!=KErrNone)
    	{
    	iLog->Log(_L("Opening subsession to RLbtServer Failed %d"),retOpen);     
    	return retOpen;
    	}
	    CleanupClosePushL(lbt[i]);
    }
    
   //Closes the subsessions
   for(TInt i =0;i<5;i++)
   {
   	 lbt[i].Close();
   	 	
   }
	   CleanupStack::Pop(5); 
   
    //Close connection to RLbtServer
    lbtServ.Close(); 
    CleanupStack::PopAndDestroy(&lbtServ);     
    iLog->Log(_L("close Connection"));
    //Return value			
    return KErrNone;

    }
    
    
       // -----------------------------------------------------------------------------
// CClientServerSetup::ErrorServerNotConnected
// Description  :: This Method is trying to open subsession
// 				   when serve is not connected.
// Return Value :: On Success KErrNone else System wide error code.   
// -----------------------------------------------------------------------------
//
TInt CClientServerSetup::ErrorServerNotConnectedL( CStifItemParser& aItem )
    {

    // Print to UI
    TestModuleIf().Printf( 0, _L("CClientServerSetup"),_L("OpenConnection"));      
    //collecting logs
    iLog->Log(_L("Start Connection"));     
    //Create instance of RLbtServer
    RLbtServer	lbtServ;
    RLbt lbt;
    //RLbtServer serverObject;//testing
    //Connect to RLbtServer
   // TInt retConnect = lbtServ.Connect();
    
   // iLog->Log(_L("Connection to RLbtServer Passed ")); 
   TestModuleIf().SetExitReason( CTestModuleIf::EPanic, 2); 	
    TInt retOpen = lbt.Open(lbtServ);
    if(retOpen!=KErrNone)
    	{
    	iLog->Log(_L("Opening subsession to RLbtServer Failed %d"),retOpen);     
    	return retOpen;
    	}
    	CleanupClosePushL(lbt);
    //Closes the subsession
    lbt.Close();
    CleanupStack::PopAndDestroy(&lbt); 
    //Close connection to RLbtServer
   // lbtServ.Close();    
   // iLog->Log(_L("close Connection"));
    //Return value			
    return KErrNone;

    }
    
    
// -----------------------------------------------------------------------------
// CClientServerSetup::ErrorSubsessionNotClosed
// Description  :: This Method is trying to close session
//				   when all the subsessions not closed.	
// Return Value :: On Success KErrNone else System wide error code.   
// -----------------------------------------------------------------------------
//
TInt CClientServerSetup::ErrorSubsessionNotClosedL( CStifItemParser& aItem )
    {

    // Print to UI
    TestModuleIf().Printf( 0, _L("CClientServerSetup"),_L("OpenConnection"));      
    //collecting logs
    iLog->Log(_L("Start Connection"));     
    //Create instance of RLbtServer
    RLbtServer	lbtServ;
    RLbt lbt;
    //RLbtServer serverObject;//testing
    //Connect to RLbtServer
    TInt retConnect = lbtServ.Connect();
    CleanupClosePushL(lbtServ);	
   // iLog->Log(_L("Connection to RLbtServer Passed ")); 	
    TInt retOpen = lbt.Open(lbtServ);
    if(retOpen!=KErrNone)
    	{
    	iLog->Log(_L("Opening subsession to RLbtServer Failed %d"),retOpen);     
    	return retOpen;
    	}
    	CleanupClosePushL(lbt);
    //Closes the subsession
   // clientobject.Close();
    //Close connection to RLbtServer
  //  TestModuleIf().SetExitReason( CTestModuleIf::EPanic, 4); 
    lbtServ.Close(); 
    CleanupStack::Pop(&lbt);     
    CleanupStack::PopAndDestroy(&lbtServ);     
    iLog->Log(_L("close Connection"));
    //Return value			
    return KErrNone;

    }
    
    
      // -----------------------------------------------------------------------------
// CClientServerSetup::ErrorSubsessionAlreadyClosed
// Description  :: This Method is trying to close subsession
//				   when there is no opened subsession	
// Return Value :: On Success KErrNone else System wide error code.   
// -----------------------------------------------------------------------------
//
    
    
    TInt CClientServerSetup::ErrorSubsessionAlreadyClosedL( CStifItemParser& aItem )
    {

    // Print to UI
    TestModuleIf().Printf( 0, _L("CClientServerSetup"),_L("OpenConnection"));      
    //collecting logs
    iLog->Log(_L("Start Connection"));     
    //Create instance of RLbtServer
    RLbtServer	lbtServ;
    RLbt lbt;
    //RLbtServer serverObject;//testing
    //Connect to RLbtServer
    TInt retConnect = lbtServ.Connect();
    CleanupClosePushL(lbtServ);	
   // iLog->Log(_L("Connection to RLbtServer Passed ")); 	
    TInt retOpen = lbt.Open(lbtServ);
    if(retOpen!=KErrNone)
    	{
    	iLog->Log(_L("Opening subsession to RLbtServer Failed %d"),retOpen);     
    	return retOpen;
    	}
    CleanupClosePushL(lbt);	
    //Closes the subsession
    lbt.Close();
   
    //Closes the subsession again
    lbt.Close();
     CleanupStack::PopAndDestroy(&lbt); 
    //Close connection to RLbtServer
    lbtServ.Close(); 
    CleanupStack::PopAndDestroy(&lbtServ);     
    iLog->Log(_L("close Connection"));
    //Return value			
    return KErrNone;

    }
    
    
    // -----------------------------------------------------------------------------
// CClientServerSetup::GetLBTServerVersion
// Description  :: This Method is Testing the retrieval of LBTServer version
//					information.
// Return Value :: On Success KErrNone else System wide error code.   
// -----------------------------------------------------------------------------
//
TInt CClientServerSetup::GetLBTServerVersionL( CStifItemParser& aItem )
    {

    // Print to UI
    TestModuleIf().Printf( 0, _L("CClientServerSetup"),_L("EstablishConnection"));      
    //collecting logs
    iLog->Log(_L("GetLBTServerVersion"));     
    //Create instance of RLbtServer
    RLbtServer	lbtServ;
    //RLbtServer clientObject;//testing
       
    TVersion LBTServerVersion= lbtServ.Version(); 	
    //Print version Number to UI
    TestModuleIf().Printf(1, _L("VersionMajor VersionMinor VersionBuild"),
    				 _L("%d:%d:%d"),LBTServerVersion.iBuild,LBTServerVersion.iMajor,
    				 				LBTServerVersion.iMinor);
	//Store version number in log file.    				 				
    iLog->Log(_L("VersionMajor =%d VersionMinor =%d VersionBuild =%d"),
    			LBTServerVersion.iBuild,LBTServerVersion.iMajor,LBTServerVersion.iMinor); 
    
    //Close connection to RLbtServer
    lbtServ.Close();    
    iLog->Log(_L("close Connection"));
    //Return value			
    return KErrNone;

    }
    
    
       // -----------------------------------------------------------------------------
// CClientServerSetup::ErrorGetLBTServerVersion
// Description  :: This Method is Testing Connect and
// 				   Close functanality of RLbtServer.
// Return Value :: On Success KErrNone else System wide error code.   
// -----------------------------------------------------------------------------
//
TInt CClientServerSetup::ErrorGetLBTServerVersionL( CStifItemParser& aItem )
    {

    // Print to UI
    TestModuleIf().Printf( 0, _L("CClientServerSetup"),_L("EstablishConnection"));      
    //collecting logs
    iLog->Log(_L("Get Version Information"));     
    //Create instance of RLbtServer
    RLbtServer	lbtServ;
    
    TVersion LBTServerVersion= lbtServ.Version(); 	
    //Print version Number to UI
    TestModuleIf().Printf(1, _L("VersionMajor VersionMinor VersionBuild"),
    				 _L("%d:%d:%d"),LBTServerVersion.iBuild,LBTServerVersion.iMajor,
    				 				LBTServerVersion.iMinor);
	//Store version number in log file.    				 				
    iLog->Log(_L("VersionMajor =%d VersionMinor =%d VersionBuild =%d"),
    			LBTServerVersion.iBuild,LBTServerVersion.iMajor,LBTServerVersion.iMinor); 
    
    //Close connection to RLbtServer
   // serverObject.Close();    
   // iLog->Log(_L("close Connection"));
    //Return value			
    return KErrNone;

    }
    
    
        
        // -----------------------------------------------------------------------------
// CClientServerSetup::FindServer
// Description  :: This Method is Testing Connect and
// 				   Close functanality of RLbtServer.
// Return Value :: On Success KErrNone else System wide error code.   
// -----------------------------------------------------------------------------
//
TInt CClientServerSetup::FindServerL( CStifItemParser& aItem )
    {

    // Print to UI
    TestModuleIf().Printf( 0, _L("CClientServerSetup"),_L("EstablishConnection"));      
    //collecting logs
    iLog->Log(_L("Find Server"));     
    _LIT(KProcessName,"lbtserver.exe") ;   
    TFindServer findLBTServer(KProcessName);
    TInt result;
    TFullName name;
	result = findLBTServer.Next(name);
	if (result == KErrNone)
	{
	// Server already running
//	RProcess serverprocess;
//	TInt error =serverprocess.Open(KProcessName);
//	TInt priority = serverprocess.Priority();
	return KErrNone;
	}
	else
	{
		return result;
	}	
    

    }
    
    
    // -----------------------------------------------------------------------------
// CClientServerSetup::StatusInformationSupervisionNotActive
// Example test method function.
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
TInt CClientServerSetup::StatusInformationSupervisionNotActiveL( CStifItemParser& aItem )
    {
/*
    RProperty iProperty;
    
    TInt ltSupervisionStatus;
    
    //Get location triggering supervision status
    User::LeaveIfError(iProperty.Get(
        KPSUidLbtStatusInformation, 
        KLbtLocationTriggeringSupervisionStatus, 
        ltSupervisionStatus));
    //Use following code for change notification.
    
    //Listen for change event of location triggering 
    //supervision status.
    //Attached to the key
    User::LeaveIfError(iProperty.Attach(
        KPSUidLbtStatusInformation, 
        KLbtLocationTriggeringSupervisionStatus));
        
    TRequestStatus status;
    iProperty.Subscribe(status);
    User::WaitForRequest(status);
    
    //Get the new value
    User::LeaveIfError(status.Int());
    User::LeaveIfError(iProperty.Get(
        KPSUidLbtStatusInformation, 
        KLbtLocationTriggeringSupervisionStatus, 
        ltSupervisionStatus));
    
    iProperty.Close();

    //Return value			
    return KErrNone;
*/
return KErrNone;
    }
    
    
       // -----------------------------------------------------------------------------
// CClientServerSetup::RequestStatusChangeNotification
// Example test method function.
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
TInt CClientServerSetup::RequestStatusChangeNotificationL( CStifItemParser& aItem )
    {

   /* RProperty iProperty;
    
    TInt ltSupervisionStatus;
    
    //Get location triggering supervision status
    User::LeaveIfError(iProperty.Get(
        KPSUidLbtStatusInformation, 
        KLbtLocationTriggeringSupervisionStatus, 
        ltSupervisionStatus));
    //Use following code for change notification.
    
    //Listen for change event of location triggering 
    //supervision status.
    //Attached to the key
    User::LeaveIfError(iProperty.Attach(
        KPSUidLbtStatusInformation, 
        KLbtLocationTriggeringSupervisionStatus));
        
    TRequestStatus status;
    iProperty.Subscribe(status);
    User::WaitForRequest(status);
    
    //Get the new value
    User::LeaveIfError(status.Int());
    User::LeaveIfError(iProperty.Get(
        KPSUidLbtStatusInformation, 
        KLbtLocationTriggeringSupervisionStatus, 
        ltSupervisionStatus));
    
    iProperty.Close();

    //Return value			
    return KErrNone;*/
return KErrNone;
    }
    
    
            // -----------------------------------------------------------------------------
// CClientServerSetup::FindServer
// Description  :: This Method is Testing Connect and
// 				   Close functanality of RLbtServer.
// Return Value :: On Success KErrNone else System wide error code.   
// -----------------------------------------------------------------------------
//
TInt CClientServerSetup::killLBTServerL( CStifItemParser& aItem )
    {
    // Print to UI
    TestModuleIf().Printf( 0, _L("CClientServerSetup"),_L("killLBTServer"));      
    //collecting logs
    iLog->Log(_L("Find Server")); 
    _LIT(KProcessName,"lbtserver.exe") ;     
    TFindServer findLBTServer(KProcessName);
    TInt result;
    TFullName name;
	result = findLBTServer.Next(name);
	
	if (result == KErrNone)
        {
        _LIT(KProcessName,"lbtserver.exe") ;
        RProcess aProcess;
        TInt res = aProcess.Open(KProcessName);
        aProcess.Kill(0);
        }

	result = findLBTServer.Next(name);
        if (result == KErrNone)
        {
        iLog->Log(_L("server started again"));  	
        }
	
    return KErrNone;
    }
    

    
    
 /**********************Code starts here**********************

 TFindProcess aProcess2(_L("*"));
 TFullName aResult;

 while(aProcess2.Next(aResult)==KErrNone){
        TInt idx = aPtr.Find(aResult);
        if(idx!=KErrNotFound){//found process
                RProcess aProcess;
                aProcess.Open(aProcess2);
                aProcess.Kill(0);
                aProcess.Close();
                InitProcessList();
                break;
        }
 }

**********************Code ends here************************/



    
    
    
// -----------------------------------------------------------------------------
// CClientServerSetup::?member_function
// ?implementation_description
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
/*
TInt CClientServerSetup::?member_function(
   CItemParser& aItem )
   {

   ?code

   }
*/

// ========================== OTHER EXPORTED FUNCTIONS =========================
// None

//  End of File
