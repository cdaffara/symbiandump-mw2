/*
* Copyright (c) 2009 Nokia Corporation and/or its subsidiary(-ies).
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

#include <QByteArray>
#include "testutilities.h"

/*!
  Replaces global new operator for utilizing binary. Enables OOM 
  simulation and memory leak detection.
  
  Note that creation of CBase derived Symbian classes 
  are not tracked, because CBase overloads new operator. 
 */
void* operator new(std::size_t sz) throw(std::bad_alloc)
{
    return MemoryAllocator::alloc(sz);
}

/*!
  Replaces global delete operator for utilizing binary. Enables OOM 
  simulation and memory leak detection. 
 */
void operator delete(void* memoryAddr) throw()
{
    return MemoryAllocator::free(memoryAddr);
}

void* operator new(std::size_t sz, const std::nothrow_t&) throw()
{
    return qMalloc(sz);
}

void  operator delete(void* memoryAddress, const std::nothrow_t&) throw()
{
    if (NULL != memoryAddress) {
        qFree(memoryAddress);
    }
}

bool MemoryAllocator::m_isOomSimulationEnabled = false;
int MemoryAllocator::m_numOfAllocsSinceLastFail = 0;
int MemoryAllocator::m_allocFailIndex = 1;
QList<void*> MemoryAllocator::m_allocList;

/*!
  MemoryAllocator::enableOomSimulation
 */
void MemoryAllocator::enableOomSimulation()
{
    m_isOomSimulationEnabled = true;
    m_allocFailIndex = 1;
    m_numOfAllocsSinceLastFail = 0;
}

/*!
  MemoryAllocator::disableOomSimulation
 */
void MemoryAllocator::disableOomSimulation()
{
    m_isOomSimulationEnabled = false;
}

/*!
  MemoryAllocator::isOomSimulationEnabled
 */
bool MemoryAllocator::isOomSimulationEnabled()
{
    return m_isOomSimulationEnabled;
}

/*!
  MemoryAllocator::currentAllocFailIndex
 */
int MemoryAllocator::currentAllocFailIndex()
{
    return m_allocFailIndex;
}

/*!
  MemoryAllocator::alloc
 */
void* MemoryAllocator::alloc(std::size_t sz)
{
    if (isOomSimulationEnabled()) {
        m_numOfAllocsSinceLastFail++;
        if (m_allocFailIndex == m_numOfAllocsSinceLastFail) {
            m_allocFailIndex++;
            m_numOfAllocsSinceLastFail = 0;
#ifdef QT_NO_EXCEPTIONS
            return NULL;
#else
            throw std::bad_alloc();
#endif
        }
    }
    
    void *allocatedMemoryAddr = qMalloc(sz);
    m_allocList.append(allocatedMemoryAddr);
    return allocatedMemoryAddr;
}

/*!
  MemoryAllocator::free
 */
void MemoryAllocator::free(void *memoryAddress)
{
    if (memoryAddress) {
        int index = m_allocList.indexOf(memoryAddress);
        if (-1 != index) {
            m_allocList.removeAt(index);
        }
        qFree(memoryAddress);
    }
}

/*!
  MemoryAllocator::verifyMemoryAllocations
 */
void MemoryAllocator::verifyMemoryAllocations()
{
    int numOfUnfreedCells = m_allocList.count();
    if (0 != numOfUnfreedCells) {
        for ( int i = 0; i < numOfUnfreedCells; i++) {
            qDebug("UNFREED CELL: %x", reinterpret_cast<int>(m_allocList.at(i)));
        }
    
    m_allocList.clear();
    throw std::bad_alloc();
    }
    
    m_allocList.clear();
}


/*!
  OomTestExecuter::runTest
 */
void OomTestExecuter::runTest(
    QObject &testObject, const char *testMethod)
{
    qDebug() << "OomTestExecuter::runTest : IN :" << testMethod;
    
    MemoryAllocator::enableOomSimulation();

    bool exceptionCaught = false;
    do {
        exceptionCaught = false;
        int currentAllocFailIndex = MemoryAllocator::currentAllocFailIndex();
        
        try {
            try {
                QMetaObject::invokeMethod(
                    &testObject, "init", Qt::DirectConnection);
                QMetaObject::invokeMethod(
                    &testObject, testMethod, Qt::DirectConnection);
            } catch (const std::bad_alloc &ex) {
                exceptionCaught = true;
                QMetaObject::invokeMethod(
                    &testObject, "cleanup", Qt::DirectConnection);
            }
        // TODO: for some reason bad_alloc exception is corrupted to 
        // unknown exception and nested catch block is needed to be able to
        // handle situation. One level catch does not work for some reason.
        } catch (...) {
            exceptionCaught = true;
            QMetaObject::invokeMethod(
                &testObject, "cleanup", Qt::DirectConnection);
            if (currentAllocFailIndex == MemoryAllocator::currentAllocFailIndex()) {
                qDebug() << "OomTestExecuter::runTest, ERROR: unexpected exception!";
                throw;
            }
        }
    } while(exceptionCaught);
    
    QMetaObject::invokeMethod(&testObject, "cleanup", Qt::DirectConnection);
    MemoryAllocator::disableOomSimulation();
    qDebug() << "OomTestExecuter::runTest : OUT :" << testMethod;
}

/*!
  OomTestExecuter::runAllTests
 */
void OomTestExecuter::runAllTests(
    QObject &testObject, const char *callingTestMethod)
{
    const QMetaObject *metaObject = testObject.metaObject();
    
    int methodCount = metaObject->methodCount();
    for (int i = 0; i < methodCount; ++i) {
        QMetaMethod slotMethodCandidate = metaObject->method(i);
        if (!isValidSlot(slotMethodCandidate)) {
            continue;
        }
        
        QByteArray slotMethodName(slotMethodCandidate.signature());
        // remove parentheses
        slotMethodName = slotMethodName.left(slotMethodName.length() - 2);
        
        // Prevent from infinite loop and do not execute test method, which
        // has called runAllTests.
        if (slotMethodName != callingTestMethod) {
            runTest(testObject, slotMethodName);
        }
    }
}

/*!
  OomTestExecuter::isValidSlot
 */
bool OomTestExecuter::isValidSlot(const QMetaMethod &sl)
{
    if ((sl.access() != QMetaMethod::Private) || !sl.parameterTypes().isEmpty()
        || qstrlen(sl.typeName()) || (sl.methodType() != QMetaMethod::Slot)) {
        return false;
    }
    
    const char *sig = sl.signature();
    int len = qstrlen(sig);
    if (len < 2) {
        return false;
    }
    
    if (sig[len - 2] != '(' || sig[len - 1] != ')') {
        return false;
    }
    
    if (len > 7 && strcmp(sig + (len - 7), "_data()") == 0) {
        return false;
    }
    
    if ((strcmp(sig, "initTestCase()") == 0) || (strcmp(sig, "cleanupTestCase()") == 0)
        || (strcmp(sig, "cleanup()") == 0) || (strcmp(sig, "init()") == 0)) {
        return false;
    }
    
    return true;
}
