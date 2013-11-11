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
* Description:   SUPL Communication Manager is responsible for creating and
* 				 destroying SUPL Connections. It is also responsible for
* 				 handling the incoming SUPL packets from the network
*
*/



// INCLUDE FILES
//#include    <?include_file>
#include <e32std.h>
#include "epos_csuplconntrace.h"

#include "epos_csuplconnection.h"
#include "epos_msuplpacketreceiver.h"

#include    "epos_csuplcommunicationmanager.h"

const TInt KMaxSocketSupport = 15;

_LIT(KTraceFileName,"SUPL_PH_API::epos_csuplcommunicationmanager.cpp");


// ============================ MEMBER FUNCTIONS ===============================

CSuplCommunicationManager::CSuplCommunicationManager(MSuplConnectionMonitor& aConnMonitor):
	iServerConnected(EFalse), iConnMonitor(aConnMonitor)
    {
    }

void CSuplCommunicationManager::ConstructL()
    {
	iConnArray.Reset();
	iTrace = CSuplConnTrace::NewL();
	
	iTrace->Trace(_L("CSuplCommunicationManager::ConstructL"), KTraceFileName, __LINE__); 
    }

EXPORT_C CSuplCommunicationManager* CSuplCommunicationManager::NewL(MSuplConnectionMonitor& aConnMonitor)
    {
    CSuplCommunicationManager* self = new( ELeave ) CSuplCommunicationManager(aConnMonitor);
    
    CleanupStack::PushL( self );
    self->ConstructL();
    CleanupStack::Pop();

    return self;
    }
    
EXPORT_C CSuplCommunicationManager::~CSuplCommunicationManager()
    {
	DestroyAllConnections();
	
	if(iServerConnected)
		{
		iSocketServ.Close();	
		}	
	
	iConnArray.Close();
	
	iTrace->Trace(_L("CSuplCommunicationManager::Destructor"), KTraceFileName, __LINE__);
	
	delete iTrace;
    	
    }

// -----------------------------------------------------------------------------
// CSuplCommunicationManager::Initialize
// Connects to the Socket Server
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
EXPORT_C TInt CSuplCommunicationManager::Initialize()
    {
	TInt ret = iSocketServ.Connect(KMaxSocketSupport);
	if(KErrNone == ret)
		{
		iServerConnected = ETrue;
		}
	
	iTrace->Trace(_L("CSuplCommunicationManager Initialized"), KTraceFileName, __LINE__);
	
	return ret;
    }

// -----------------------------------------------------------------------------
// CSuplCommunicationManager::CreateConnectionL
// Creates the SUPL Connection to be used by SUPL Session
// This method checks if there is already a connection for the
// givem IP Adress and port. If so, it returns the existing object
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
EXPORT_C CSuplConnection *CSuplCommunicationManager::CreateConnectionL(const TDesC &aHostAddress, const TUint aPort,
																	   const TInt aIAPId)
    {
    // Log
    iTrace->Trace(_L("CSuplCommunicationManager::CreateConnectionL"), KTraceFileName, __LINE__);
    
    //Return KErrNotReady if Communication Manager not initialized
    
    if(!iServerConnected)
    	{
        // Log
    	iTrace->Trace(_L("CSuplCommunicationManager::CreateConnectionL Server Not Ready"), KTraceFileName, __LINE__);
    	User::Leave(KErrNotReady);
    	}
    	
      	
    CSuplConnection *suplConnection = NULL;
    
    if(NULL == (suplConnection = FindConnection(aHostAddress, aPort)))
    	{
    	suplConnection = CSuplConnection::NewL(iSocketServ, aHostAddress, aPort, aIAPId, this);	
    	iConnArray.AppendL(suplConnection);
    	iTrace->Trace(_L("CSuplCommunicationManager::CreateConnectionL New Connection Created"), KTraceFileName, __LINE__);
    	iConnMonitor.ConnectionOpened();
    }
    else
    	{
    	// Check if we have exceeded the max no. of sessions 
    	// for this conenction
        TBuf<64> msg(_L("RefCount in CreateConnectionL"));
        msg.AppendNum(suplConnection->RefCount());
	    iTrace->Trace(msg, KTraceFileName, __LINE__);		
    	if(suplConnection->RefCount() >= KMaxSessionPerConn)
    		{
    		suplConnection = CSuplConnection::NewL(iSocketServ, aHostAddress, aPort, aIAPId, this);
    		iConnArray.AppendL(suplConnection);
    		iTrace->Trace(_L("CSuplCommunicationManager::CreateConnectionL New Connection Created"), KTraceFileName, __LINE__);
        	iConnMonitor.ConnectionOpened();
    	}
    }
    
    // Increment the Ref Count
    suplConnection->IncRefCount();
    	
    return suplConnection;	
    }
    
// -----------------------------------------------------------------------------
// CSuplCommunicationManager::CreateConnectionL
// Creates the SUPL Connection to be used by SUPL Session
// This method checks if there is already a connection for the
// givem IP Adress and port. If so, it returns the existing object
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
//----------------------------ADDITIONAL CREATECONNECTIONL METHOD(TAKES TLS ALSO)-----------------------------

EXPORT_C CSuplConnection* CSuplCommunicationManager::CreateConnectionL(const TDesC &aHostAddress, TBool aTls,TBool aPskTls,
                                                                        const TUint aPort,const TInt aIAPId)
    {
    // Log
    iTrace->Trace(_L("CSuplCommunicationManager::CreateConnectionL"), KTraceFileName, __LINE__);
    
    //Return KErrNotReady if Communication Manager not initialized
    
    if(!iServerConnected)
    	{
        // Log
        iTrace->Trace(_L("CSuplCommunicationManager::CreateConnectionL Server Not Ready"), KTraceFileName, __LINE__);
        User::Leave(KErrNotReady);
    	}
        
        
    CSuplConnection *suplConnection = NULL;
    
    if(NULL == (suplConnection = FindConnection(aHostAddress, aPort)))
    	{
        suplConnection = CSuplConnection::NewL(iSocketServ, aHostAddress, aPort, aIAPId, aTls, aPskTls, this); 
        iConnArray.AppendL(suplConnection);
       	iConnMonitor.ConnectionOpened();
        iTrace->Trace(_L("CSuplCommunicationManager::CreateConnectionL New Connection Created"), KTraceFileName, __LINE__);
    	}
    else
    	{
        // Check if we have exceeded the max no. of sessions 
        // for this conenction
        TBuf<64> msg(_L("RefCount in CreateConnectionL"));
        msg.AppendNum(suplConnection->RefCount());
        iTrace->Trace(msg, KTraceFileName, __LINE__);       
        if(suplConnection->RefCount() >= KMaxSessionPerConn)
        	{
            suplConnection = CSuplConnection::NewL(iSocketServ, aHostAddress, aPort, aIAPId, aTls, aPskTls, this);
            iConnArray.AppendL(suplConnection);
        	iConnMonitor.ConnectionOpened();
            iTrace->Trace(_L("CSuplCommunicationManager::CreateConnectionL New Connection Created"), KTraceFileName, __LINE__);
        	}
    	}      
    
    // Increment the Ref Count
    suplConnection->IncRefCount();
        
    return suplConnection;  
    }

//--------------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------
// CSuplCommunicationManager::DestroyConnection
// Destroys the Specified SUPL Connection provided, the SUPL Connection
// is found in the list of Connections maintained by Communication Manager
// and the referece count on the Connection is 1.
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//	
EXPORT_C TInt CSuplCommunicationManager::DestroyConnection(CSuplConnection *aConnection)
    {
    // Log
    iTrace->Trace(_L("CSuplCommunicationManager::DestroyConnection"), KTraceFileName, __LINE__);
    
    // Check if the Connection is valid
    
    TInt index = FindConnection(aConnection);
    
    if(KErrNotFound == index)
    	{
    	return KErrArgument;
    	}
    
    //Check the RefCount on this object
    if(aConnection->RefCount() > 1)
    	{
    	iTrace->Trace(_L("CSuplCommunicationManager::DestroyConnection Decrement Ref Count and return"), KTraceFileName, __LINE__);
    	aConnection->DecrRefCount();
    	return KErrNone;
    	}
    	
    // Remove from Array
    iTrace->Trace(_L("CSuplCommunicationManager::DestroyConnection Remove Connection"), KTraceFileName, __LINE__);
    RemoveConnection(index);
    
    //Last Session invoking Destroy
    iTrace->Trace(_L("CSuplCommunicationManager::DestroyConnection Destroy Connection"), KTraceFileName, __LINE__);
    aConnection->Destroy();
    
    iConnMonitor.ConnectionClosed();
    
    return KErrNone;
    }

// -----------------------------------------------------------------------------
// CSuplCommunicationManager::SetPacketReceiver
// Sets the MSuplPacketReceiver
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
EXPORT_C void CSuplCommunicationManager::SetPacketReceiver(MSuplPacketReceiver *aReceiver)
    {
	// Log
	iTrace->Trace(_L("CSuplCommunicationManager::SetPacketReceiver"), KTraceFileName, __LINE__);
	
	iPktReceiver = aReceiver;
    }
    
    
// -----------------------------------------------------------------------------
// CSuplCommunicationManager::DestroyAllConnections
// Destroys all the SUPL Connections held by iConnArray
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void CSuplCommunicationManager::DestroyAllConnections()
    {    
    // Log
    iTrace->Trace(_L("CSuplCommunicationManager::DestroyAllConnections"), KTraceFileName, __LINE__);
    
    // Iterate through the arry of connections
    TInt arrLen = iConnArray.Count();
    
    while(iConnArray.Count() > 0)
    	{
    	CSuplConnection *conn = iConnArray[0];
    	
    	// Remove from Array
    	RemoveConnection(0);
    	
    	// Destroy SUPL Connection
       	conn->Destroy();
    	}
    }
    
// -----------------------------------------------------------------------------
// CSuplCommunicationManager::FindConnection
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
TInt CSuplCommunicationManager::FindConnection(const CSuplConnection *aConnection)
    {
    // Log
    iTrace->Trace(_L("CSuplCommunicationManager::FindConnection"), KTraceFileName, __LINE__);
    
    // Find for the connection in iConnArray
    
    return iConnArray.Find(aConnection);
    }
    
// -----------------------------------------------------------------------------
// CSuplCommunicationManager::RemoveConnection
// Removes the SUPL Connection from iConnArray
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void CSuplCommunicationManager::RemoveConnection(TInt aIndex)
    {
    // Log
    iTrace->Trace(_L("CSuplCommunicationManager::FindConnection"), KTraceFileName, __LINE__);
    
    if(aIndex >= 0 && aIndex < iConnArray.Count())
    	{
    	iConnArray.Remove(aIndex);
    	}
    }
    
// -----------------------------------------------------------------------------
// CSuplCommunicationManager::FindConnection
// Given the HostAddress and Port, this method determines
// if there is exists SUPL Connection. Returns the object
// pointer if found. Else returns NULL
// (other items were commented in a header).
// -----------------------------------------------------------------------------
// 
CSuplConnection *CSuplCommunicationManager::FindConnection(const TDesC &aHostAddress, const TUint aPort)
	{
	// Log 
	iTrace->Trace(_L("CSuplCommunicationManager::FindConnection"), KTraceFileName, __LINE__);
	
	TInt arrLen = iConnArray.Count();
	
	if(arrLen > 0)
		{
		CSuplConnection *connPtr;
		for(TInt i = 0; i < arrLen; i++)
			{
			connPtr = iConnArray[i];
		
			if(((aHostAddress.Compare(connPtr->HostAddress())) == 0) && (connPtr->Port() == aPort))
				{
				iTrace->Trace(_L("CSuplCommunicationManager::FindConnection - Connection Found"), KTraceFileName, __LINE__);
                if (connPtr->RefCount() < KMaxSessionPerConn)
                	{                        
				    iTrace->Trace(_L("Conn serving less no. of connection, returning conn"), KTraceFileName, __LINE__);
				    return connPtr;
                	}
				}
			}
		}
		
	// Log
	iTrace->Trace(_L("CSuplCommunicationManager::FindConnection - Connection Not Found"), KTraceFileName, __LINE__);
	
	return NULL;
	}


// -----------------------------------------------------------------------------
// CSuplCommunicationManager::HandlePacket
// Method to Handle incoming packets from the network
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void CSuplCommunicationManager::HandlePacket(const TDesC8 &aPacket,TUint aPortNum)
    {
    	// Log
    	iTrace->Trace(_L("CSuplCommunicationManager::HandlePacket"), KTraceFileName, __LINE__);
    	
    	if(iPktReceiver)
    	{
    		iPktReceiver->HandlePacket(aPacket,aPortNum);
    	}
    	
    }
    
// -----------------------------------------------------------------------------
// CSuplCommunicationManager::HandlePacketError
// Method to Handle incoming packets from the network
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void CSuplCommunicationManager::HandlePacketError(TInt aError)
    {
	// Log
	iTrace->Trace(_L("CSuplCommunicationManager::HandlePacketError"), KTraceFileName, __LINE__);
	
	if(iPktReceiver)
		{
		iPktReceiver->HandlePacketError(aError);
		}
    }

//  End of File  
