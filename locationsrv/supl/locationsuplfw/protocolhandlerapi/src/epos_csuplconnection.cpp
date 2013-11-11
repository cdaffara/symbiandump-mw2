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
 * Description:   SUPL Connection provides the Connection End point
 * 				 for SUPL Session to send/receive packets from the
 *				 network
 *
 */

// INCLUDE FILES
#ifdef SYMBIAN_ENABLE_SPLIT_HEADERS
#include <ssl_internal.h>
#endif
#include <commdbconnpref.h>
#include <centralrepository.h>

#include "epos_csuplserverprivatecrkeys.h"
#include "epos_csuplconnection.h"
#include "epos_csuplsocketreader.h"   
#include "epos_csuplsockettimer.h"
#include "epos_csuplconntrace.h"
#include "epos_csuplpacketinfo.h"
#include "epos_psktlsusageprivatecrkeys.h"
#include "epos_suplpskloger.h"

#include <extendedconnpref.h> //For OCC
const TInt KMaxGBAUsageLength = 4;
const TInt KMaxVerLength = 4;
//const TInt KMaxNewPasswordLength =  8;
const TInt KMaxNewPasswordLength = 16;
const TInt KMaxPskKeyLength = 256;

const TInt KMaxRootKeyLength = 32;
const TInt KMaxRootKeyXORLength = 16;
//const TInt KMaxRootKeyLength =  16;
const TInt KMaxPassKeyLength = 8;
//const TInt KMaxPassKeyLength =  8;
const TInt KMaxSaltLength = 128;
//const TInt KMaxSaltLength =  64;

const TInt KMaxPskIdentityHintLength = KMaxGBAUsageLength + KMaxVerLength
        + KMaxPassKeyLength;

// Constant definition

_LIT8(KWGBA,"WGBA");

_LIT(KTraceFileName,"SUPL_PH_API::epos_csuplconnection.cpp");

_LIT8(KSaltValue,"0x19,0x31,0x45,0x69,0x69,0x45,0x31,0x19");

void DoSelfXOR(TDes8& aDestination, const TDesC8& aSource)
    {
    aDestination.Zero();
    TInt half = aSource.Length() / 2;
    for (TInt i = 0; i < half; i++)
        {
        aDestination.Append(aSource[i] ^ aSource[half + i]);
        }
    }

void DoXOR(TDes8& aDestination, const TDesC8& aSource1,
        const TDesC8& aSource2)
    {
    aDestination.Zero();
    TInt srcLen1 = aSource1.Length();
    TInt srcLen2 = aSource2.Length();
    TInt j = 0;
    for (TInt i = 0; i < srcLen1; i++)
        {
        if (j >= srcLen2)
            {
            j = 0;
            }

        aDestination.Append(aSource1[i] ^ aSource2[j++]);
        }
    }

//0x18,0x32,0x34,0x55,0x55,0x34,0x32,0x18	
void FormSaltKey(TDes8& aDestination, const TDesC8& aSource)
    {
    TBuf8<2> seachText(_L8("0x"));
    aDestination.Zero();
    TBuf8<KMaxSaltLength> tempSaltBuffer(aSource);
    TUint32 hexNumber;
    TBuf<2> decNumber;
    TInt offSet = tempSaltBuffer.Find(seachText);

    while (offSet != KErrNotFound)
        {
        offSet += 2;
        decNumber.Copy(tempSaltBuffer.Mid(offSet, 2));
        tempSaltBuffer.Copy(tempSaltBuffer.Mid(offSet + 2));
        TLex lex(decNumber);
        lex.Val(hexNumber, EHex);
        aDestination.Append(hexNumber);
        offSet = tempSaltBuffer.Find(seachText);
        }
    }

//void get_key(unsigned char key[],unsigned int keylength);

void derive_key(const unsigned char pwd[], /* the PASSWORD     */
unsigned int pwd_len, /* and its length   */
const unsigned char salt[], /* the SALT and its */
unsigned int salt_len, /* length           */
unsigned int iter, /* the number of iterations */
unsigned char key[], /* space for the output key */
unsigned int key_len);/* and its required length  */

// ============================ MEMBER FUNCTIONS ===============================

CSuplConnection::CSuplConnection(RSocketServ &aSocketServ) :
    CActive(EPriorityStandard), iSocketServ(aSocketServ), iRefCount(0),
            iState(ENotConnected), iPacket(0, 0), iVariantEnabled(ETrue),
            iPskTLSUsage(EFalse), iStartedListen(EFalse), iConnectStarted(
                    EFalse), iSendInProgress(EFalse), iFirstPkt(EFalse),
            iListenCount(0), iPrompt(EFalse), iWlanOnly(EFalse)
    {
    iHostAddress.Zero();
    CActiveScheduler::Add(this);
    }

void CSuplConnection::ConstructL(const TDesC &aHostAddress,
        const TUint aPort, const TInt aIAPId, MSuplPacketNotifier *aNotifier)
    {

    iTrace = CSuplConnTrace::NewL();

    iTrace->Trace(_L("CSuplConnection::ConstructL"), KTraceFileName, __LINE__);
    SetVariation();
    iHostAddress.Append(aHostAddress);
    iPort = aPort;
    iIAPId = aIAPId;

    // Create the Secure Socket
    if (iVariantEnabled)
        {
        iTrace->Trace(_L("Using CSuplConnection::Secure Construction"),
                KTraceFileName, __LINE__);
        _LIT(KTLS1, "TLS1.0");
        iTlsSocket = CSecureSocket::NewL(iSocket, KTLS1);

        TBuf8<KMaxHostAddressLen> dNSName;
        dNSName.Copy(aHostAddress);
        iTlsSocket->SetOpt(KSoSSLDomainName, KSolInetSSL, dNSName);

        if (iPskTLSUsage)
            {
            iTrace->Trace(_L("Considering PSK Ciphersuites for Usage..."),
                    KTraceFileName, __LINE__);

            // PSK TLS Start
            TPckgBuf<MSoPskKeyHandler *> pskConfigPkg;
            pskConfigPkg() = this;
            User::LeaveIfError(iTlsSocket->SetOpt(KSoPskConfig, KSolInetSSL,
                    pskConfigPkg));
            // PSK TLS End
            }

        iTlsSocket->SetDialogMode(EDialogModeUnattended);

        iSocketReader = CSuplSocketReader::NewL(iTlsSocket, aNotifier,
                iVariantEnabled);
        }
    else
        {
        iTrace->Trace(_L("Using CSuplConnection::NonSecure Construction"),
                KTraceFileName, __LINE__);
        iSocketReader = CSuplSocketReader::NewL(iSocket, aNotifier,
                iVariantEnabled);
        }
    iTimer = CSuplSocketTimer::NewL(this);

    if (iPskTLSUsage)
        {
        // PSK TLS Start
#if WITH_GBA_OFFF
        iGbaClient = CGbaClientRequestHandler::NewL();
#endif /* WITH_GBA */
        // PSK TLS End
        }

    iConnArray.Reset();

    iPktArray.Reset();

    }
// -----------------------------------------------------------------------------
// CSuplConnection::ConstructL
// 
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
//-------------------------------------------------------------------------------------------------------------
void CSuplConnection::ConstructL(const TDesC &aHostAddress,
        const TUint aPort, const TInt aIAPId, TBool aTls, TBool aPskTls,
        MSuplPacketNotifier *aNotifier)
    {

    iTrace = CSuplConnTrace::NewL();

    iTrace->Trace(_L("CSuplConnection::ConstructL"), KTraceFileName, __LINE__);
    //SetVariation(); 
    iVariantEnabled = aTls;
    iPskTLSUsage = aPskTls;
    iHostAddress.Append(aHostAddress);
    iPort = aPort;
    iIAPId = aIAPId;

    // Create the Secure Socket
    if (iVariantEnabled)
        {
        iTrace->Trace(_L("Using CSuplConnection::Secure Construction"),
                KTraceFileName, __LINE__);
        _LIT(KTLS1, "TLS1.0");
        iTlsSocket = CSecureSocket::NewL(iSocket, KTLS1);

        TBuf8<KMaxHostAddressLen> dNSName;
        dNSName.Copy(aHostAddress);
        iTlsSocket->SetOpt(KSoSSLDomainName, KSolInetSSL, dNSName);

        if (iPskTLSUsage)
            {
            iTrace->Trace(_L("Considering PSK Ciphersuites for Usage..."),
                    KTraceFileName, __LINE__);
            // PSK TLS Start
            TPckgBuf<MSoPskKeyHandler *> pskConfigPkg;
            pskConfigPkg() = this;
            User::LeaveIfError(iTlsSocket->SetOpt(KSoPskConfig, KSolInetSSL,
                    pskConfigPkg));
            // PSK TLS End
            }

        iTlsSocket->SetDialogMode(EDialogModeUnattended);

        iSocketReader = CSuplSocketReader::NewL(iTlsSocket, aNotifier,
                iVariantEnabled);
        }
    else
        {
        iTrace->Trace(_L("Using CSuplConnection::NonSecure Construction"),
                KTraceFileName, __LINE__);
        iSocketReader = CSuplSocketReader::NewL(iSocket, aNotifier,
                iVariantEnabled);
        }
    iTimer = CSuplSocketTimer::NewL(this);

    if (iPskTLSUsage)
        {
        // PSK TLS Start
#if WITH_GBA_OFFF
        iGbaClient = CGbaClientRequestHandler::NewL();
#endif /* WITH_GBA */
        // PSK TLS End
        }

    iConnArray.Reset();

    iPktArray.Reset();

    }

CSuplConnection* CSuplConnection::NewL(RSocketServ &aSocketServ,
        const TDesC &aHostAddress, const TUint aPort, const TInt aIAPId,
        MSuplPacketNotifier *aNotifier)
    {
    CSuplConnection* self = new (ELeave) CSuplConnection(aSocketServ);

    CleanupStack::PushL(self);
    self->ConstructL(aHostAddress, aPort, aIAPId, aNotifier);
    CleanupStack::Pop();

    return self;
    }

// --------------------------------------------ADDED NEWL TAKING TLS ALSO-------------------------------------
// -----------------------------------------------------------------------------
// CSuplConnection::NewL
// 
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
CSuplConnection* CSuplConnection::NewL(RSocketServ &aSocketServ,
        const TDesC &aHostAddress, const TUint aPort, const TInt aIAPId,
        TBool aTls, TBool aPskTls, MSuplPacketNotifier *aNotifier)
    {
    CSuplConnection* self = new (ELeave) CSuplConnection(aSocketServ);

    CleanupStack::PushL(self);
    self->ConstructL(aHostAddress, aPort, aIAPId, aTls, aPskTls, aNotifier);
    CleanupStack::Pop();

    return self;
    }
//------------------------------------------------------------------------------------------------------------
CSuplConnection::~CSuplConnection()
    {

    // Log
    if (iTrace)
        {
        iTrace->Trace(_L("CSuplConnection::Destructor"), KTraceFileName,
                __LINE__);

        iTrace->Trace(_L("CSuplConnection::Destructor...Cancelling "),
                KTraceFileName, __LINE__);
        }
    // Cancel Any outstanding request
    Cancel();

    if (iTrace)
        iTrace->Trace(_L(
                "CSuplConnection::Destructor...Deleting Socket Reader"),
                KTraceFileName, __LINE__);
    //Delete the Socket Reader
    delete iSocketReader;

    // Close the resolver if required
    if (iState == ELookingUp)
        {
        if (iTrace)
            iTrace->Trace(
                    _L("CSuplConnection::Destructor...Closing Resolver"),
                    KTraceFileName, __LINE__);
        iResolver.Close();
        }
    if (iTrace)
        iTrace->Trace(_L(
                "CSuplConnection::Destructor...Deleting Secure Socket"),
                KTraceFileName, __LINE__);
    // Delete the TLS Socket
    delete iTlsSocket;

    // Close the Socket if required
    if (iState == EConnected || iState == EConnecting)
        {
        iTrace->Trace(_L("CSuplConnection::Destructor...Closing Socket"),
                KTraceFileName, __LINE__);
        iSocket.Close();
        }

#ifndef __WINS__
    // Close the RConnection
    if(iTrace)
    iTrace->Trace(_L("CSuplConnection::Destructor...Closing RConnection"), KTraceFileName, __LINE__);
    iConnection.Close();
#endif
    if (iTrace)
        iTrace->Trace(_L("CSuplConnection::Destructor...Deleting Timer"),
                KTraceFileName, __LINE__);
    // Delete timer
    delete iTimer;

    // PSK TLS Start
#if WITH_GBA_OFFF
    delete iGbaClient;
#endif /* WITH_GBA */
    // PSK TLS End

    iConnArray.Close();
    // Delete Trace
    delete iTrace;

    iPktArray.Close();
    }

// -----------------------------------------------------------------------------
// CSuplConnection::RunL
// This is the method to handle request completion for
// asynchronous requests made by CSuplConnection
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void CSuplConnection::RunL()
    {

    // Log
    TBuf<64> msg(_L("CSuplConnection::RunL: State is "));
    msg.AppendNum(iState);
    msg.Append(_L("  iStatus is "));
    msg.AppendNum(iStatus.Int());
    iTrace->Trace(msg, KTraceFileName, __LINE__);

    switch (iState)
        {
        case ERetriveIAP:
            {
            iTrace->Trace(_L("CSuplConnection::ERetriveIAP"), KTraceFileName,
                    __LINE__);
            if (iStatus.Int() == KErrNone)
                {
                ConnectIAP();
                }
            else
                {
                iTrace->Trace(_L("Error in Retriving IAP Id... "),
                        KTraceFileName, __LINE__);
                iConnection.Close();
                iTrace->Trace(_L("iConnection.Close();... "), KTraceFileName,
                        __LINE__);
                //User::RequestComplete(iReqStatus, iStatus.Int());
                CompleteConnectRequests(iStatus.Int());
                }
            break;
            }
            // Socket Connect has completed
        case EConnecting:
            iTimer->Cancel(); // Valid SUPL server, Cancel timer started for this purpose
            if (iStatus == KErrNone)
                {
                if (iVariantEnabled)
                    {
                    iTrace->Trace(
                            _L(
                                    "CSuplConnection::RunL Making Secure Connection"),
                            KTraceFileName, __LINE__);
                    iState = EMakingSecureConn;

                    MakeSecureConnection();
                    }
                else
                    {
                    // Log
                    iTrace->Trace(
                            _L(
                                    "CSuplConnection::RunL Non Secure Socket Connection Established"),
                            KTraceFileName, __LINE__);
                    // Change State
                    iState = EConnected;
                    //User::RequestComplete(iReqStatus, iStatus.Int());
                    CompleteConnectRequests(iStatus.Int());
                    }
                }
            else
                {
                // Log
                iTrace->Trace(_L(
                        "CSuplConnection::RunL Socket Connection Failed"),
                        KTraceFileName, __LINE__);

                iState = EFailure;
                iSocket.Close();
#ifndef __WINS__  // Bug Fix for close connection
                // Close the RConnection
                iConnection.Close();
#endif
                //User::RequestComplete(iReqStatus, iStatus.Int());
                CompleteConnectRequests(iStatus.Int());
                iTrace->Trace(_L("CSuplConnection::SocketConn Failed Exit"),
                        KTraceFileName, __LINE__);
                }
            break;

            // Secure Connection
        case ESecureHandShake:
            // Stop timer
            iTimer->Cancel();
            if (iStatus != KErrNone)
                {
                // Log
                iTrace->Trace(_L(
                        "CSuplConnection::RunL Secure Connection Failed"),
                        KTraceFileName, __LINE__);

                // Connection was not successful
                iState = EFailure;
                iSocket.Close();
#ifndef __WINS__  // Bug Fix for close connection
                // Close the RConnection
                iConnection.Close();
#endif
                }
            else
                {
                // Log
                iTrace->Trace(
                        _L(
                                "CSuplConnection::RunL Secure Connection Established"),
                        KTraceFileName, __LINE__);
                // Change State
                iState = EConnected;
                }

            //User::RequestComplete(iReqStatus, iStatus.Int());
            CompleteConnectRequests(iStatus.Int());
            break;

            // DNS Lookup has completed
        case ELookingUp:
            {
            // Log
            iTrace->Trace(_L("CSuplConnection::RunL DNS Lookup Complete"),
                    KTraceFileName, __LINE__);
            iTimer->Cancel();

            // Close the Resolver
            iResolver.Close();

            if (iStatus != KErrNone)
                {
                // Log
                iTrace->Trace(_L("CSuplConnection::RunL DNS Lookup Failed"),
                        KTraceFileName, __LINE__);

                iState = EFailure;
                iSocket.Close();
#ifndef __WINS__  // Bug Fix for close connection
                // Close the RConnection
                iConnection.Close();
#endif

                //User::RequestComplete(iReqStatus, iStatus.Int());
                CompleteConnectRequests(iStatus.Int());
                break;
                }

            // Log
            iTrace->Trace(_L("CSuplConnection::RunL DNS Lookup Successful"),
                    KTraceFileName, __LINE__);

            // Retrieve Address
            TInt addrFamily = iNameEntry().iAddr.Family();
            if (addrFamily == KAfInet)
                {
                //IPv4 Address
                iAddress.SetAddress(
                        TInetAddr::Cast(iNameEntry().iAddr).Address());

                }
            if (addrFamily == KAfInet6)
                {
                //IPv6 Address
                iAddress.SetAddress(
                        TInetAddr::Cast(iNameEntry().iAddr).Ip6Address());
                }

            // Set Port
            iAddress.SetPort(Port());

            // Connect to Remote Host
            Connect();

            break;
            }
        default:
            {
            if (iSendInProgress)
                {
                if (iPkt)
                    iPkt->CompleteRequest(iStatus.Int());
                CheckForOutstandingPackets();
                }
            else
                {
                iState = EFailure;
                //User::RequestComplete(iReqStatus, KErrGeneral);
                CompleteConnectRequests( KErrGeneral);
                }
            break;
            }

        }
    }

// -----------------------------------------------------------------------------
// CSuplConnection::CheckForOutstandingPackets
// Checks for outstanding packets in the send queue 
// and sends packets based on FIFO
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void CSuplConnection::CheckForOutstandingPackets()
    {
    if (iFirstPkt)
        {
        iFirstPkt = EFalse;
        }
    TInt index = iPktArray.Find(iPkt);
    if (index != KErrNotFound)
        iPktArray.Remove(index);
    delete iPkt;

    TInt count = iPktArray.Count();
    if (count == 0)
        {
        iSendInProgress = EFalse;
        }
    else
        {
        iPkt = REINTERPRET_CAST(CSuplPacketInfo*, iPktArray[0]);
        iPkt->GetPacketData(iPacket);

        TBuf<64> msg(_L("Sending data on port "));
        msg.AppendNum(iSocket.LocalPort());
        iTrace->Trace(msg, KTraceFileName, __LINE__);

        // Log
        if (iVariantEnabled)
            {
            iTrace->Trace(
                    _L(
                            "CSuplConnection::CheckForOutstandingPackets sending packet over Secure Connection"),
                    KTraceFileName, __LINE__);
            iTlsSocket->Send(iPacket, iStatus);
            iSendInProgress = ETrue;
            SetActive();
            }
        else
            {
            iTrace->Trace(
                    _L(
                            "CSuplConnection::CheckForOutstandingPackets sending packet over NonSecure Connection"),
                    KTraceFileName, __LINE__);
            iSocket.Send(iPacket, 0, iStatus);
            iSendInProgress = ETrue;
            SetActive();
            }
        }
    }

// -----------------------------------------------------------------------------
// CSuplConnection::CancelConnect
// Cancels connect request. 
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//

EXPORT_C void CSuplConnection::CancelConnect(TRequestStatus &aStatus)
    {
    // Log
    iTrace->Trace(_L("CSuplConnection::CancelConnect"), KTraceFileName,
            __LINE__);
    iTimer->Cancel(); // Cancel all timers
    if (iConnArray.Count() > 1)
        {
        CompleteConnectCancel(aStatus);
        return;
        }

    TBuf<64> buff;
    //buff.Copy(_L("iState = "));        
    buff.AppendNum(iState);
    iTrace->Trace(buff, KTraceFileName, __LINE__);

    switch (iState)
        {
        case ERetriveIAP:
            {
#ifndef __WINS__				
            //iConnection.Stop();
            //Earlier it was Stop()... but somehow functionality was not working after week 26..phone was restarting...Manish
            // Close is working fine at this place..
            iConnection.Close();
#endif
            break;
            }
        case ESecureHandShake:
            {
            iTlsSocket->CancelHandshake();
            break;
            }
        case ELookingUp:
            {
            iResolver.Cancel();
            break;
            }
        default:
            {
            if (iVariantEnabled)
                {
                iTrace->Trace(_L("CSuplConnection::Secure DoCancel"),
                        KTraceFileName, __LINE__);
                iTlsSocket->CancelAll();
                }
            else
                {
                iTrace->Trace(_L("CSuplConnection::NonSecure DoCancel"),
                        KTraceFileName, __LINE__);
                iSocket.CancelAll();
                }
            break;
            }
        }
    if (!(iState == ENotConnected || iState == ERetriveIAP))
        {
        iSocket.CancelAll();
        }

    CompleteConnectCancel(aStatus);
    //TRequestStatus *reqStatus = &aStatus;
    //User::RequestComplete(reqStatus, KErrNone);

    }
// -----------------------------------------------------------------------------
// CSuplConnection::DoCancel
// This is the method to cancel any request
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void CSuplConnection::DoCancel()
    {
    // Log
    iTrace->Trace(_L("CSuplConnection::DoCancel"), KTraceFileName, __LINE__);

    TBuf<64> buff;
    buff.Copy(_L("iState = "));
    buff.AppendNum(iState);
    iTrace->Trace(buff, KTraceFileName, __LINE__);

    if (!(iState == ENotConnected || iState == ERetriveIAP))
        {
        iSocket.CancelAll();
        }

    switch (iState)
        {
        case ERetriveIAP:
            {
#ifndef __WINS__				
            //iConnection.Stop();
            //Earlier it was Stop()... but somehow functionality was not working after week 26..phone was restarting...Manish
            // Close is working fine at this place..
            iConnection.Close();
#endif
            break;
            }
        case ESecureHandShake:
            {
            iTlsSocket->CancelHandshake();
            break;
            }
        case ELookingUp:
            {
            iResolver.Cancel();
            break;
            }
        default:
            {
            if (iVariantEnabled)
                {
                iTrace->Trace(_L("CSuplConnection::Secure DoCancel"),
                        KTraceFileName, __LINE__);
                iTlsSocket->CancelAll();
                }
            else
                {
                iTrace->Trace(_L("CSuplConnection::NonSecure DoCancel"),
                        KTraceFileName, __LINE__);
                iSocket.CancelAll();
                }
            break;
            }
        }

#if 0 
    if(iReqStatus)
        {
        User::RequestComplete(iReqStatus, KErrCancel);
        }
#endif
    CompleteConnectRequests( KErrCancel);
    return;
    }

// -----------------------------------------------------------------------------
// CSuplConnection::TimerExpired()
// This is the method to cancel any request
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void CSuplConnection::TimerExpired()
    {
    // Log
    iTrace->Trace(_L("CSuplConnection::TimerExpired"), KTraceFileName,
            __LINE__);

    TBuf<64> buff;
    buff.Copy(_L("iState = "));
    buff.AppendNum(iState);
    iTrace->Trace(buff, KTraceFileName, __LINE__);

    if (!(iState == ENotConnected || iState == ERetriveIAP))
        {
        iSocket.CancelAll();
        }

    switch (iState)
        {
        case ERetriveIAP:
            {
#ifndef __WINS__				
            //iConnection.Stop();
            //Earlier it was Stop()... but somehow functionality was not working after week 26..phone was restarting...Manish
            // Close is working fine at this place..
            iConnection.Close();
#endif
            break;
            }
        case ESecureHandShake:
            {
            iTlsSocket->CancelHandshake();
            break;
            }
        case ELookingUp:
            {
            iResolver.Cancel();
            break;
            }
        case EConnecting:
            {
            iState = EFailure;
            iSocket.Close();
#ifndef __WINS__  // Bug Fix for close connection
            // Close the RConnection
            iConnection.Close();
#endif
            break;
            }
        default:
            {
            if (iVariantEnabled)
                {
                iTrace->Trace(_L("CSuplConnection::Secure DoCancel"),
                        KTraceFileName, __LINE__);
                iTlsSocket->CancelAll();
                }
            else
                {
                iTrace->Trace(_L("CSuplConnection::NonSecure DoCancel"),
                        KTraceFileName, __LINE__);
                iSocket.CancelAll();
                }
            break;
            }
        }

#if 0 
    if(iReqStatus)
        {
        User::RequestComplete(iReqStatus, KErrCancel);
        }
#endif
    CompleteConnectRequests( KErrSuplTimeout);
    return;
    }

// -----------------------------------------------------------------------------
// CSuplConnection::Destroy
// This is the method through which the SUPL Connection
// can be destroyed. This is not Exported because no class
// other than CSuplCommunicationManager should destroy
// SUPL Connection
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void CSuplConnection::Destroy()
    {
    // Log
    iTrace->Trace(_L("CSuplConnection::Destroy"), KTraceFileName, __LINE__);

    delete this;
    }

// -----------------------------------------------------------------------------
// CSuplConnection::RefCount
// Returns the reference count
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
TInt CSuplConnection::RefCount()
    {
    // Log
    iTrace->Trace(_L("CSuplConnection::RefCount"), KTraceFileName, __LINE__);

    return iRefCount;
    }

// -----------------------------------------------------------------------------
// CSuplConnection::IncRefCount
// Increments the reference count
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void CSuplConnection::IncRefCount()
    {
    // Log
    iTrace->Trace(_L("CSuplConnection::IncRefCount"), KTraceFileName,
            __LINE__);

    iRefCount++;
    }

// -----------------------------------------------------------------------------
// CSuplConnection::DecrRefCount
// Decrements the reference count
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void CSuplConnection::DecrRefCount()
    {
    // Log
    iTrace->Trace(_L("CSuplConnection::DecrRefCount"), KTraceFileName,
            __LINE__);

    if (iRefCount > 0)
        iRefCount--;
    }

// -----------------------------------------------------------------------------
// CSuplConnection::HosrAddress
// Returns the IPAdress associated with this connection
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
const TDesC& CSuplConnection::HostAddress()
    {
    // Log
    iTrace->Trace(_L("CSuplConnection::HostAddress"), KTraceFileName,
            __LINE__);

    return iHostAddress;
    }

// -----------------------------------------------------------------------------
// CSuplConnection::Port
// Returns the Port associated with this connection
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
TUint CSuplConnection::Port()
    {
    return iPort;
    }

// -----------------------------------------------------------------------------
// CSuplConnection::Connect
// Makes a secure connection to Network
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
EXPORT_C void CSuplConnection::Connect(TRequestStatus &aStatus)
    {

    // Log
    iTrace->Trace(_L("CSuplConnection::Connect"), KTraceFileName, __LINE__);

    if (iState == EConnected)
        {
        aStatus = KRequestPending;
        TRequestStatus *reqStatus = &aStatus;
        User::RequestComplete(reqStatus, KErrNone);
        iConnectClients++;
        return;
        }

    // Check if Connect() is started.  If started, add status to Q
    //if (iConnectStarted)
        {
        aStatus = KRequestPending;
        TRequestStatus *reqStatus = &aStatus;
        TInt error = iConnArray.Append(reqStatus);
        if (error != KErrNone)
            {
            User::RequestComplete(reqStatus, error);
            return;
            }
        }

    if (!iConnectStarted)
        {
        iTrace->Trace(
                _L("CSuplConnection::Connect : Connection not started"),
                KTraceFileName, __LINE__);
        // Connect only if not already connected
        if (iState == ENotConnected || iState == EFailure)
            {
            iTrace->Trace(_L("CSuplConnection::Connect : ENotConnected"),
                    KTraceFileName, __LINE__);
            iConnectStarted = ETrue;

#ifndef __WINS__
            TExtendedConnPref OCCPrefs;
            TConnPrefList prefList;

            OCCPrefs.SetForcedRoaming(EFalse); //do not switch networks during an ongoing connection            
            OCCPrefs.SetNoteBehaviour(TExtendedConnPref::ENoteBehaviourDefault);

            if(iIAPId != -1) //if a IAP was configured

                {
                iTrace->Trace(_L("CSuplConnection::Connect : IAP configured"), KTraceFileName, __LINE__);
                OCCPrefs.SetIapId(iIAPId);
                // since IAP Id is set SNAP purpose should not be set - set to unknown
                OCCPrefs.SetSnapPurpose(CMManager::ESnapPurposeUnknown);
                OCCPrefs.SetBearerSet(TExtendedConnPref::EExtendedConnBearerUnknown);
                }
            else
                {
                OCCPrefs.SetSnapPurpose(CMManager::ESnapPurposeInternet);
                //WLAN and Cellular Networks allowed for connection
                OCCPrefs.SetBearerSet(TExtendedConnPref::EExtendedConnBearerWLAN | TExtendedConnPref::EExtendedConnBearerCellular);

                //if SNAP purpose is set IAP Id should be zero - zero by default not explicitly set
                iTrace->Trace(_L("CSuplConnection::Connect : IAP not configured"), KTraceFileName, __LINE__);
                }

            TRAP_IGNORE(prefList.AppendL(&OCCPrefs));

            TInt ret = iConnection.Open(iSocketServ);
            if(ret != KErrNone)
            iTrace->Trace(_L("RConnection Open returned error"), KTraceFileName, __LINE__);

            iTrace->Trace(_L("RConnection Start Called, State is ERetriveIAP"), KTraceFileName, __LINE__);
            // Start an Outgoing Connection with overrides
            iConnection.Start(prefList,iStatus);
            // Set state to ERetriveIAP
            iState = ERetriveIAP;
            SetActive();
#else
            ConnectIAP();
#endif
            }
        }
    else
        iTrace->Trace(
                _L(
                        "CSuplConnection::Connect : Conn in progress, request queued"),
                KTraceFileName, __LINE__);
    // Log
    iTrace->Trace(_L("CSuplConnection::Connect:Exit"), KTraceFileName,
            __LINE__);
    }

// -----------------------------------------------------------------------------
// CSuplConnection::Connect
// Makes a secure connection to Network
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//

EXPORT_C void CSuplConnection::Connect(TRequestStatus &aStatus,
        TBool aPrompt, TBool aWlanOnly)
    {

    // Log
    iTrace->Trace(_L("CSuplConnection::Connect"), KTraceFileName, __LINE__);
    iPrompt = aPrompt;
    iWlanOnly = aWlanOnly;

    aStatus = KRequestPending;
    TRequestStatus *reqStatus = &aStatus;

    if (iState == EConnected)
        {
        User::RequestComplete(reqStatus, KErrNone);
        iConnectClients++;
        return;
        }

    TInt error = iConnArray.Append(reqStatus);
    if (error != KErrNone)
        {
        User::RequestComplete(reqStatus, error);
        return;
        }

    if (!iConnectStarted)
        {
        iTrace->Trace(
                _L("CSuplConnection::Connect : Connection not started"),
                KTraceFileName, __LINE__);
        // Connect only if not already connected
        if (iState == ENotConnected || iState == EFailure)
            {
            iTrace->Trace(_L("CSuplConnection::Connect : ENotConnected"),
                    KTraceFileName, __LINE__);
            iConnectStarted = ETrue;

#ifndef __WINS__

            iTrace->Trace(_L("CSuplConnection::Connect ExtendedConnPref"), KTraceFileName, __LINE__);

            TExtendedConnPref OCCPrefs;
            TConnPrefList prefList;

            if(iWlanOnly)
                {
                iTrace->Trace(_L("CSuplConnection::Connect : iWLANOnly used"), KTraceFileName, __LINE__);
                OCCPrefs.SetBearerSet(TExtendedConnPref::EExtendedConnBearerWLAN);
                }
            else
                {
                iTrace->Trace(_L("CSuplConnection::Connect OCC GPRS Connection.."), KTraceFileName, __LINE__);
                OCCPrefs.SetBearerSet(TExtendedConnPref::EExtendedConnBearerCellular);
                }
            OCCPrefs.SetSnapPurpose(CMManager::ESnapPurposeInternet);

            if(!iPrompt)
                {
                iTrace->Trace(_L("CSuplConnection::Connect : silent connection"), KTraceFileName, __LINE__);
                OCCPrefs.SetNoteBehaviour(TExtendedConnPref::ENoteBehaviourConnSilent);
                }
            else
                {
                iTrace->Trace(_L("CSuplConnection::Connect : Dialog prompt"), KTraceFileName, __LINE__);
                OCCPrefs.SetNoteBehaviour(TExtendedConnPref::ENoteBehaviourDefault);
                }

            TRAP_IGNORE(prefList.AppendL(&OCCPrefs));

            TInt ret = iConnection.Open(iSocketServ);
            if( ret != KErrNone )
                {
                TRequestStatus* statusPtr = &aStatus;
                User::RequestComplete( statusPtr,ret );
                return;
                }
            // Start an Outgoing Connection with overrides
            iConnection.Start(prefList,iStatus);
            // Set state to ERetriveIAP
            iState = ERetriveIAP;
            SetActive();

#else
            ConnectIAP();
#endif
            }
        }
    else
        iTrace->Trace(
                _L(
                        "CSuplConnection::Connect : Conn in progress, request queued"),
                KTraceFileName, __LINE__);
    // Log
    iTrace->Trace(_L("CSuplConnection::Connect:Exit"), KTraceFileName,
            __LINE__);
    }

// -----------------------------------------------------------------------------
// CSuplConnection::CompleteConnectStatus
// Completes the request to cancel connection 
// (other items were commented in a header).
// -----------------------------------------------------------------------------
void CSuplConnection::CompleteConnectRequests(TInt aErr)
    {
    iTrace->Trace(_L("CSuplConnection::CompleteConnectRequests"),
            KTraceFileName, __LINE__);

    TInt statusCount = iConnArray.Count();

    TBuf<32> msg(_L("Number of conns :"));
    msg.AppendNum(statusCount);
    iTrace->Trace(msg, KTraceFileName, __LINE__);

    for (TInt loop = 0; loop < statusCount; loop++)
        {
        TRequestStatus* newStatus = REINTERPRET_CAST(TRequestStatus*, iConnArray[loop]);
        User::RequestComplete(newStatus, aErr);
        if (aErr == KErrNone)
            iConnectClients++;
        iTrace->Trace(
                _L(
                        "CSuplConnection::CompleteConnectRequests, request completed"),
                KTraceFileName, __LINE__);
        }
    iConnArray.Reset();
    iConnectStarted = EFalse;
    }

// -----------------------------------------------------------------------------
// CSuplConnection::CompleteConnectStatus
// Completes the request to cancel connection 
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void CSuplConnection::CompleteConnectCancel(TRequestStatus &aStatus)
    {
    TInt index = iConnArray.Find(&aStatus);

    if (index != KErrNotFound)
        {
        TRequestStatus *reqStatus = &aStatus;
        User::RequestComplete(reqStatus, KErrCancel);
        iConnArray.Remove(index);
        }
    }

// -----------------------------------------------------------------------------
// CSuplConnection::Send
// Sends a Packet out of the Secure Connection
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
EXPORT_C void CSuplConnection::Send(const TDesC8 &aPacket,
        TRequestStatus &aStatus)
    {
    // Log
    iTrace->Trace(_L("CSuplConnection::Send"), KTraceFileName, __LINE__);

    if (iState != EConnected || iState == EFailure)
        {
        // Log
        iTrace->Trace(_L("CSuplConnection::Send...Disconnected"),
                KTraceFileName, __LINE__);
        aStatus = KRequestPending;
        TRequestStatus *reqStatus = &aStatus;
        User::RequestComplete(reqStatus, KErrDisconnected);
        return;
        }

    // if send in progress Q it
    if (iSendInProgress)
        {
        TInt err = KErrNone;
        aStatus = KRequestPending;
        CSuplPacketInfo *packetInfo = NULL;

        TRAP(err, packetInfo = CSuplPacketInfo::NewL(aPacket, aStatus);
                iPktArray.AppendL(packetInfo));
        if (err != KErrNone)
            {
            TRequestStatus *reqStatus = &aStatus;
            User::RequestComplete(reqStatus, err);
            return;
            }
        }

    else
        {
        TInt err = KErrNone;
        aStatus = KRequestPending;
        TRAP(err, iPkt = CSuplPacketInfo::NewL(aPacket, aStatus));
        //iPkt->GetPacketData(iPacket, status);	
        if (err != KErrNone)
            {
            TRequestStatus *reqStatus = &aStatus;
            User::RequestComplete(reqStatus, err);
            return;
            }
        iFirstPkt = ETrue;
        TBuf<64> msg(_L("Sending data on port "));
        msg.AppendNum(iSocket.LocalPort());
        iTrace->Trace(msg, KTraceFileName, __LINE__);
        // Log
        if (iVariantEnabled)
            {
            iTrace->Trace(
                    _L(
                            "CSuplConnection::Send sending packet over Secure Connection"),
                    KTraceFileName, __LINE__);
            iTlsSocket->Send(aPacket, iStatus);
            iSendInProgress = ETrue;
            SetActive();
            }
        else
            {
            iTrace->Trace(
                    _L(
                            "CSuplConnection::Send sending packet over NonSecure Connection"),
                    KTraceFileName, __LINE__);
            iSocket.Send(aPacket, 0, iStatus);
            iSendInProgress = ETrue;
            SetActive();
            }
        }
    }

// -----------------------------------------------------------------------------
// CSuplConnection::CancelSend
// Method to cancel the Socket Send operation
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
EXPORT_C void CSuplConnection::CancelSend(TRequestStatus &aStatus)
    {
    if (iFirstPkt)
        {
        if (iVariantEnabled)
            {
            iTrace->Trace(_L("CSuplConnection::Secure Send Cancel"),
                    KTraceFileName, __LINE__);
            iTlsSocket->CancelSend();
            }
        else
            {
            iTrace->Trace(_L("CSuplConnection::Non Secure Send Cancel"),
                    KTraceFileName, __LINE__);
            iSocket.CancelSend();
            }
        delete iPkt;
        iPkt = NULL;
        TRequestStatus *reqStatus = &aStatus;
        User::RequestComplete(reqStatus, KErrCancel);
        }
    else
        {
        TInt count = iPktArray.Count();
        if (count == 1)
            {
            TInt index = iPktArray.Find(iPkt);
            if (index != KErrNotFound)
                iPktArray.Remove(index);
            delete iPkt;
            iPkt = NULL;
            TRequestStatus *reqStatus = &aStatus;
            User::RequestComplete(reqStatus, KErrCancel);
            }
        else
            {
            for (TInt loop = 0; loop < count; loop++)
                {
                iPkt = REINTERPRET_CAST(CSuplPacketInfo*, iPktArray[0]);
                if (iPkt->IsStatusMatches(aStatus))
                    {
                    iTrace->Trace(_L("Found packet, deleting"),
                            KTraceFileName, __LINE__);
                    TInt index = iPktArray.Find(iPkt);
                    if (index != KErrNotFound)
                        {
                        iTrace->Trace(_L("Found packet, deleted"),
                                KTraceFileName, __LINE__);
                        iPktArray.Remove(index);
                        TRequestStatus *reqStatus = &aStatus;
                        User::RequestComplete(reqStatus, KErrCancel);
                        }
                    delete iPkt;
                    iPkt = NULL;
                    break;
                    }
                }
            }
        }

    }

// -----------------------------------------------------------------------------
// CSuplConnection::Connect
// Makes a socket connection to Network
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void CSuplConnection::Connect()
    {
    // Log
    iTrace->Trace(_L("CSuplConnection::Connect(void)"), KTraceFileName,
            __LINE__);

    iState = EConnecting;
    iTimer->Start(4000000); // For DNS pass case , but not a valid SUPL server
    iSocket.Connect(iAddress, iStatus);
    if (!IsActive())
        {
        SetActive();
        }
    }

// -----------------------------------------------------------------------------
// CSuplConnection::ConnectIAP
// Makes a socket connection to Network
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//

void CSuplConnection::ConnectIAP()
    {
    iTrace->Trace(_L("CSuplConnection::ConnectIAP(void)"), KTraceFileName,
            __LINE__);
    // Connect only if not already connected
    if (iState == ENotConnected || iState == EFailure || iState
            == ERetriveIAP)
        {
        // Open the Socket
#ifndef __WINS__
        // Emulator can't handle RConnections
        TInt err = iSocket.Open( iSocketServ,
                KAfInet,
                KSockStream,
                KProtocolInetTcp,
                iConnection );
#else
        TInt err = iSocket.Open(iSocketServ, KAfInet, KSockStream,
                KProtocolInetTcp);
#endif

        if (KErrNone != err)
            {
            // Log
            iTrace->Trace(_L("CSuplConnection::Socket Open Failure"),
                    KTraceFileName, __LINE__);

            //User::RequestComplete(iReqStatus, err);
            CompleteConnectRequests(err);
            return;
            }

        // Set IP Address and Port
        if (iAddress.Input(iHostAddress) == KErrNone)
            {
            // Log
            iTrace->Trace(
                    _L(
                            "CSuplConnection::Connect Host Address is an IP Address"),
                    KTraceFileName, __LINE__);

            iAddress.SetPort(Port());
            Connect();
            }

        else
            {
            // DNS Lookup
            // Log
            iTrace->Trace(_L("CSuplConnection::Connect Perform DNS lookup"),
                    KTraceFileName, __LINE__);
#ifndef __WINS__
            err = iResolver.Open(iSocketServ, KAfInet, KProtocolInetUdp, iConnection);
#else
            err = iResolver.Open(iSocketServ, KAfInet, KProtocolInetUdp);
#endif				
            if (KErrNone != err)
                {
                //User::RequestComplete(iReqStatus, err);
                CompleteConnectRequests(err);
                return;
                }

            iState = ELookingUp;
            iResolver.GetByName(iHostAddress, iNameEntry, iStatus);
            iTimer->Start(10000000);
            if (!IsActive())
                {
                SetActive();
                }
            }

        }

    else
        {
        // Already Connected. Just return KErrNone
        //User::RequestComplete(iReqStatus, KErrNone);	
        CompleteConnectRequests( KErrNone);
        }

    }
// -----------------------------------------------------------------------------
// CSuplConnection::MakeSecureConnection
// Makes a secure connection to Network
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void CSuplConnection::MakeSecureConnection()
    {
    // Log
    iTrace->Trace(_L("CSuplConnection::MakeSecureConnection"),
            KTraceFileName, __LINE__);

    // Clear any pervious options
    //iTlsSocket->FlushSessionCache();

    iTlsSocket->StartClientHandshake(iStatus);
    iTimer->Start(10000000);
    iState = ESecureHandShake;
    if (!IsActive())
        {
        SetActive();
        }
    }

// -----------------------------------------------------------------------------
// CSuplConnection::ListenToMessages
// Start reading from the socket for packets from the network
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
EXPORT_C TInt CSuplConnection::ListenToMessages()
    {
    // Log
    iTrace->Trace(_L("CSuplConnection::ListenToMessages"), KTraceFileName,
            __LINE__);

    iListenCount++;

    if (iStartedListen)
        return KErrNone;

    if (iState != EConnected)
        {
        return KErrDisconnected;
        }

    if (iSocketReader && (!IsActive()))
        {
        iSocketReader->SetPortNum(GetPortNumberUsed());
        iSocketReader->IssueRead();
        iStartedListen = ETrue;
        return KErrNone;
        }

    return KErrGeneral;
    }

// -----------------------------------------------------------------------------
// CSuplConnection::GetLocalIPAddress
// Start reading from the socket for packets from the network
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//

EXPORT_C TInt CSuplConnection::GetLocalIPAddress(TInetAddr & aTInetAddress)
    {
    if (iState != EConnected)
        return KErrDisconnected;

    TSockAddr sockAddr;
    iSocket.LocalName(sockAddr);

    TInetAddr inetAddress(sockAddr);

    aTInetAddress = inetAddress;
    return KErrNone;
    }

// -----------------------------------------------------------------------------
// CSuplConnection::CancelListenToMessages
// Start reading from the socket for packets from the network
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//	
EXPORT_C void CSuplConnection::CancelListenToMessages()
    {
    if (iListenCount > 1)
        {
        iListenCount--;
        return;
        }
    if (iSocketReader)
        {
        iSocketReader->Cancel();
        }
    }

// -----------------------------------------------------------------------------
// COMASuplProvHandler::SetVariation
// Initializes the local variation flag
// -----------------------------------------------------------------------------
//
void CSuplConnection::SetVariation()
    {
#if 0
    CRepository *repository = NULL;
    TRAPD( err, repository = CRepository::NewL( KCRUidSuplConfiguration));
    if ( KErrNone != err )
        {
        return;
        }

    TInt val;
    TInt rc = repository->Get( KSuplTLSAuthenticationVariant,val );
    if ( KErrNone == rc && val == 0 )
        {
        iVariantEnabled = EFalse;
        }

    if ( KErrNone == rc && (val == 1 || val == 2 ))
        {
        iVariantEnabled = ETrue;
        if(val == 2 )
            {
            iPskTLSUsage = ETrue;
            }
        }

    delete repository;
#endif
    }

EXPORT_C void CSuplConnection::CloseConnection()
    {
    if (iTrace)
        iTrace->Trace(_L("CSuplConnection::CloseConnection"), KTraceFileName,
                __LINE__);

    // if connection request is from multiple clients and it is not yet
    // completed, just return. 
    if (iConnArray.Count() > 1)
        return;
#if 0
    TBuf<128> buf;
    buf.Copy(_L("CSuplConnection::CloseConnection, iConnectClients = "));
    buf.AppendNum(iConnectClients);
    iTrace->Trace(buf, KTraceFileName, __LINE__);

    buf.Copy(_L("CSuplConnection::CloseConnection, iState = "));
    buf.AppendNum(iState);
    iTrace->Trace(buf, KTraceFileName, __LINE__);
#endif

    if (iConnArray.Count() == 1)
        iConnArray.Reset();

    iConnectClients--;

    if (iConnectClients > 0) //earlier > 1
        return;

    iResolver.Close();
    CancelListenToMessages();
    // Close the Socket if required
    if (iState == EConnected || iState == EConnecting || iState
            == ESecureHandShake)
        {
        if (iTrace)
            iTrace->Trace(_L("CSuplConnection::...Closing Socket"),
                    KTraceFileName, __LINE__);
        if (iVariantEnabled)
            iTlsSocket->Close();
        iSocket.Close();
        }

#ifndef __WINS__
    // Close the RConnection
    if(iTrace)
    iTrace->Trace(_L("CSuplConnection::...Closing RConnection"), KTraceFileName, __LINE__);
    iConnection.Close();
#endif
    iState = ENotConnected;
    }

// -----------------------------------------------------------------------------
// CSuplConnection::GetPskL
// 
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//	

// PSK TLS Start 
void CSuplConnection::GetPskL(const HBufC8 * aPskIdentityHint,
        HBufC8 *& aPskIdentity, HBufC8 *& aPskKey)
    {

    iTrace->Trace(_L("CSuplConnection::GetPskL Start"), KTraceFileName,
            __LINE__);
    if (aPskIdentityHint != NULL && aPskIdentityHint->Length()
            >= KMaxPskIdentityHintLength)
        {
        iTrace->Trace(_L("aPskIdentityHint is not null..."), KTraceFileName,
                __LINE__);
        TBuf8<KMaxGBAUsageLength> gbaUsage;
        TBuf8<KMaxVerLength> ver;
        TBuf8<KMaxNewPasswordLength> newPassword;

        gbaUsage.Copy(aPskIdentityHint->Left(KMaxGBAUsageLength));
        ver.Copy(aPskIdentityHint->Mid(KMaxGBAUsageLength, KMaxVerLength));
        newPassword.Copy(aPskIdentityHint->Mid(KMaxGBAUsageLength
                + KMaxVerLength));

        gbaUsage.UpperCase();
        ver.UpperCase();

        if (gbaUsage.Compare(KWGBA) == 0) //We will proceed with key generation if its WGBA
            {
            iTrace->Trace(_L("Its WGBA..."), KTraceFileName, __LINE__);
            GeneratePsk_Identity_KeyL(ver, newPassword, aPskIdentity, aPskKey);
            }
        else
            {
            TBuf<32> buf;
            buf.Copy(gbaUsage);
            iTrace->Trace(_L("Received GBA field is ..."), KTraceFileName,
                    __LINE__);
            iTrace->Trace(buf, KTraceFileName, __LINE__);
            iTrace->Trace(_L("Leaving as of now,till GBA supports comes..."),
                    KTraceFileName, __LINE__);
            User::LeaveIfError( KErrNotSupported);
            }
        }
    else
        {
        iTrace->Trace(_L("Either Hint is Null OR Hint Length is exceeded."),
                KTraceFileName, __LINE__);
        User::LeaveIfError( KErrArgument);
        }

#ifdef _DEBUG
    LogPskKeyInfoL(aPskIdentityHint, aPskIdentity, aPskKey);
#endif
    iTrace->Trace(_L("CSuplConnection::GetPskL End"), KTraceFileName,
            __LINE__);

    }

// -----------------------------------------------------------------------------
// CSuplConnection::GeneratePsk_Identity_KeyL
// 
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//	
void CSuplConnection::GeneratePsk_Identity_KeyL(const TDesC8& aSLPKeyVer,
        const TDesC8& aPassKey, HBufC8 *& aPskIdentity, HBufC8 *& aPskKey)
    {

    iTrace->Trace(_L("Before CR..."), KTraceFileName, __LINE__);
    CRepository* repository = CRepository::NewL(
            KCRUidPskTLSUsageConfiguration);
    iTrace->Trace(_L("After CR..."), KTraceFileName, __LINE__);
    CleanupStack::PushL(repository);

    TInt keyVersion;

    TLex8 verStr(aSLPKeyVer);
    TInt SLPKeyVer;
    verStr.Val(SLPKeyVer);

    TBuf8<KMaxPskKeyLength> pskKey;

    TInt rc = repository->Get(KSuplPskTLSKeyVersion, keyVersion);
    if (rc == KErrNone && keyVersion == SLPKeyVer)
        {
        iTrace->Trace(_L("Version is matching with stored one..."),
                KTraceFileName, __LINE__);
        rc = repository->Get(KSuplPskTLSNewPassword, pskKey);
        if (rc == KErrNone)
            {
            aPskKey = HBufC8::NewL(KMaxPskKeyLength);
            aPskKey->Des().Copy(pskKey);
            iTrace->Trace(_L("Copied Psk Key"), KTraceFileName, __LINE__);
            PrintHex(pskKey, __LINE__);
            }
        else
            {
            iTrace->Trace(_L("KSuplPskTLSNewPassword Not Found"),
                    KTraceFileName, __LINE__);
            User::LeaveIfError(rc);
            }
        }
    else
        {
        iTrace->Trace(_L("Version is NOT matching with stored one..."),
                KTraceFileName, __LINE__);
        iTrace->Trace(_L("Generating new Key..."), KTraceFileName, __LINE__);
        GeneratePskKeyL(repository, aPassKey, aPskKey, SLPKeyVer);
        PrintHex(*aPskKey, __LINE__);
        }

    GeneratePskIdentityL(repository, SLPKeyVer, aPskIdentity);
    CleanupStack::PopAndDestroy(repository);

    }

// -----------------------------------------------------------------------------
// CSuplConnection::GeneratePskIdentity
// 
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//	
void CSuplConnection::GeneratePskIdentityL(CRepository* aRepository,
        TInt /*aKeyVer*/, HBufC8 *& aPskIdentity)
    {
    iTrace->Trace(_L("Start of GeneratePskIdentityL"), KTraceFileName,
            __LINE__);

    TInt crKeyId;
    TInt rc = aRepository->Get(KSuplPskTLSKeyId, crKeyId);

    if (rc == KErrNone)
        {
        aPskIdentity = HBufC8::NewL(KMaxPskKeyLength);
        aPskIdentity->Des().Append(crKeyId);
        aPskIdentity->Des().Append(13);//newline
        iTrace->Trace(_L("Copied PskIdentityL"), KTraceFileName, __LINE__);
TRACESTRING        ("PskIdentity : ")
        PrintHex(aPskIdentity->Des(),__LINE__);
        }
    else
        {
        iTrace->Trace(_L("KSuplPskTLSKeyId Not Found"), KTraceFileName, __LINE__);
        User::LeaveIfError(rc);
        }
    iTrace->Trace(_L("End of GeneratePskIdentityL"), KTraceFileName, __LINE__);
    }

// -----------------------------------------------------------------------------
// CSuplConnection::GeneratePskKeyL
// 
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//	
void CSuplConnection::GeneratePskKeyL(CRepository* aRepository,
        const TDesC8& aPassKey, HBufC8 *& aPskKey, TInt aSLPKeyVersion)
    {
    unsigned char newpassword[32];
    TBuf8<KMaxRootKeyLength> rootKey;
    TBuf8<KMaxRootKeyXORLength> rootXORedPassKey;
    TBuf8<KMaxSaltLength> finalSalt;

    iTrace->Trace(_L("Get rootKey from CR"), KTraceFileName, __LINE__);

    TBuf<KMaxRootKeyLength> rootKey16;
    TInt rc = aRepository->Get(KSuplPskTLSRootPassword, rootKey16);
    rootKey.Copy(rootKey16);

    TBuf<64> buf;
    TRACESTRING("Root Key from CenRep")
    TRACETEXT(rootKey)

    buf.Copy(_L("Get(KSuplPskTLSRootPassword,rootKey16)  "));
    buf.AppendNum(rc);
    buf.Append(_L(" Len of rootkey "));
    buf.AppendNum(rootKey.Length());
    iTrace->Trace(buf, KTraceFileName, __LINE__);
    User::LeaveIfError(rc);

    TRACESTRING("Pass Key from SLP")
    TRACETEXT(aPassKey)

    if(rc == KErrNone && rootKey.Length() != 0)
        {
        TBuf8<KMaxRootKeyLength> rootXORedKey;

        iTrace->Trace(_L("DoSelfXOR"), KTraceFileName, __LINE__);
        DoSelfXOR(rootXORedKey,rootKey);
        TRACESTRING("Self XOR")
        PrintHex(rootXORedKey,__LINE__);

        iTrace->Trace(_L("DoXOR"), KTraceFileName, __LINE__);
        DoXOR(rootXORedPassKey,aPassKey,rootXORedKey);
        TRACESTRING("DoXOR")
        PrintHex(rootXORedPassKey,__LINE__);

        iTrace->Trace(_L("FormSalt"), KTraceFileName, __LINE__);
        TRACETEXT(KSaltValue)
        FormSaltKey(finalSalt,KSaltValue);
        TRACESTRING("Salt Value in hex...")
        PrintHex(finalSalt,__LINE__);

        iTrace->Trace(_L("Call derive key"), KTraceFileName, __LINE__);

        buf.Copy(_L("Length of RootXORedPassKey "));
        buf.AppendNum(rootXORedPassKey.Length());
        TRACETEXT(buf)

        buf.Copy(_L("Length of Salt Key "));
        buf.AppendNum(finalSalt.Length());
        TRACETEXT(buf)

        derive_key(rootXORedPassKey.Ptr(),rootXORedPassKey.Length(),finalSalt.Ptr(),
                finalSalt.Length(),8,newpassword,32);

        iTrace->Trace(_L("Copy key into PskKey"), KTraceFileName, __LINE__);

        aPskKey = HBufC8::NewL(32);
        aPskKey->Des().Copy(newpassword,32);

        buf.Copy(_L("Length of Key "));
        buf.AppendNum(aPskKey->Des().Length());
        TRACETEXT(buf)

        TRACESTRING("Key...")
        PrintHex(aPskKey->Des(),__LINE__);

        aRepository->Set(KSuplPskTLSNewPassword,aPskKey->Des());
        aRepository->Set(KSuplPskTLSKeyVersion,aSLPKeyVersion);
        iTrace->Trace(_L("Copied PskKey"), KTraceFileName, __LINE__);
        }

    else
        {
        iTrace->Trace(_L("KSuplPskTLSRootPassword Not Found"), KTraceFileName, __LINE__);
        User::LeaveIfError(rc);
        }

    iTrace->Trace(_L("End GeneratePskKeyL"), KTraceFileName, __LINE__);
    }
// -----------------------------------------------------------------------------
// CSuplConnection::LogPskKeyInfoL
// 
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//	
void CSuplConnection::LogPskKeyInfoL(const HBufC8* /*aPskIdentityHint*/,
        HBufC8* /*aPskIdentity*/, HBufC8* /*aPskKey*/)
    {
#if 0
    TBuf<128> buf;
    buf.Copy(_L("PSK Key :"));
    iTrace->Trace(buf, KTraceFileName, __LINE__);
    HBufC* hintbuf = NULL;
    TInt err;
    TRAP(err, hintbuf = HBufC::NewL(aPskKey->Length()));
    err = 0;
    hintbuf->Des().Copy(aPskKey->Des());
    iTrace->Trace( hintbuf->Des(),KTraceFileName, __LINE__);
    delete hintbuf;
    hintbuf = NULL;

    buf.Copy(_L("PSK Identity :"));
    iTrace->Trace(buf, KTraceFileName, __LINE__);
    TRAP(err, hintbuf = HBufC::NewL(aPskIdentity->Length()));
    err = 0;
    hintbuf->Des().Copy(aPskIdentity->Des());
    iTrace->Trace( hintbuf->Des(),KTraceFileName, __LINE__);
    delete hintbuf;
    hintbuf = NULL;

    if (aPskIdentityHint != NULL)
        {
        buf.Copy(_L("Rcvd Identity Hint :"));
        iTrace->Trace(buf, KTraceFileName, __LINE__);
        TRAP(err, hintbuf = HBufC::NewL(aPskIdentityHint->Length()));
        err = 0;
        hintbuf->Des().Copy(*aPskIdentityHint);
        iTrace->Trace( hintbuf->Des(),KTraceFileName, __LINE__);
        delete hintbuf;
        hintbuf = NULL;
        }
    else
        {
        iTrace->Trace(_L("Rcvd Null Identity Hint"), KTraceFileName, __LINE__);
        }
#endif
    }

void CSuplConnection::PrintHex(const TDesC8&
#ifdef _DEBUG
        aBuffer
#endif
        , TInt
#ifdef _DEBUG
        aLine
#endif

)
    {
#ifdef _DEBUG
    TBuf<128> buffer;
    TBuf<8> buff;
    _LIT16(KFormat1, "%02x");
    TInt len = aBuffer.Length();
    for (TInt i = 0; i < len; i++)
        {
        buff.Zero();
        buff.Format(KFormat1, aBuffer[i]);
TRACETEXT        (buff)
        iTrace->Trace(buff, KTraceFileName, aLine);
        }
#endif
    }

EXPORT_C TInt CSuplConnection::GetPortNumberUsed()
    {
    iTrace->Trace(_L("CSuplConnection::GetPortNumberUsed"), KTraceFileName,
            __LINE__);
    if (iState == EConnected)
        {
        TBuf<64> msg(_L("Port Number used is "));
        msg.AppendNum(iSocket.LocalPort());
        iTrace->Trace(msg, KTraceFileName, __LINE__);
        return iSocket.LocalPort();
        }
    else
        return KErrGeneral;
    }

// PSK TLS End
//  End of File  
