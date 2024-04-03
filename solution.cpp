#ifndef __PROGTEST__
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <cstdint>
#include <climits>
#include <cfloat>
#include <cassert>
#include <cmath>
#include <iostream>
#include <iomanip>
#include <algorithm>
#include <numeric>
#include <string>
#include <vector>
#include <array>
#include <iterator>
#include <set>
#include <list>
#include <map>
#include <unordered_set>
#include <unordered_map>
#include <compare>
#include <queue>
#include <stack>
#include <deque>
#include <memory>
#include <functional>
#include <thread>
#include <mutex>
#include <atomic>
#include <chrono>
#include <stdexcept>
#include <condition_variable>
#include <pthread.h>
#include <semaphore.h>
#include "progtest_solver.h"
#include "sample_tester.h"
using namespace std;
#endif /* __PROGTEST__ */

//-------------------------------------------------------------------------------------------------------------------------------------------------------------
mutex mutex3;
class CommunicationClass;
class Cpacker {
public:
    Cpacker(){

    }
    void AddtoQueue (shared_ptr<Cpacker> & item ) {
        unique_lock<mutex> ul (mutex3 );
        q.push_back (item );
        conditionVariable.notify_all();
    }
    explicit Cpacker(AProblemPack pPack) : m_ProblemPack(move(pPack)) {}
    void NotificationToALL() {
       unique_lock<mutex> lockGuard (mutex1 );
        conditionVariable.notify_all(); }
    bool isSolved() const {
        for (const auto &polygon : m_ProblemPack->m_ProblemsMin) {
            if (polygon->m_TriangMin == 0)
            {return false;

            }
        }
        for (const auto &polygon : m_ProblemPack->m_ProblemsCnt){
            if (polygon->m_TriangCnt==0){
                return false;
            }

        }
        return true;
    }

    auto returnfromqueueu () {
        unique_lock<mutex> uniqueLock (mutex3 );
        conditionVariable.wait (uniqueLock, [ this ] {return (! q.empty() && q.front() == nullptr ) || (! q.empty() && q.front()->isSolved() );} );
        shared_ptr<Cpacker> front  = q.front();
        q.pop_front();
       // NotificationToALL();
        return front;
    }

    deque< shared_ptr<Cpacker>> q;
    mutex mutex1;
    AProblemPack m_ProblemPack;
    condition_variable conditionVariable;
private:

};
class Buffer {
public:
    explicit Buffer (APolygon polygon, shared_ptr<Cpacker> PackofPolyg )
            : Polygonss (std::move (polygon ) ), m_ParentProblemPack (std::move (PackofPolyg ) ) {}
    const APolygon &getPolygonss() const {
        return Polygonss;
    }
private:
    APolygon Polygonss;
    shared_ptr<Cpacker> m_ParentProblemPack;
};
class Solver{
public:
    AProgtestSolver mySolver;
    explicit Solver (AProgtestSolver s )
            : mySolver (move (s ) ) {

    }
    size_t solve () { return mySolver->solve(); }
    bool addPolygonProblems (const shared_ptr<Buffer>& polygon ) {
        m_Problems.push_back (polygon );
        return mySolver->addPolygon(polygon->getPolygonss() );
    }
    bool hasFreeCapacity() {  return mySolver->hasFreeCapacity(); }
    vector<shared_ptr<Buffer>> m_Problems;

};

class COptimizer {
public:
    COptimizer ():
            CntSolver(make_shared<Solver>(createProgtestCntSolver())),MinSolver(make_shared<Solver>(createProgtestMinSolver())),
            counter(0),
            killer(false) {
        // cout << "COptimizer constructor called." << endl;
    }

    void addCompany(const ACompany &company) {
        m_Companies.push_back(make_shared<CommunicationClass>(company));
    }

    void start(int threadCount);
    void worker () {
      //  cout << "Worker thread started." << endl;
        while ( true ) {
            unique_lock<mutex> lk (mutex3 );
            Checker2.wait (lk, [ this ] { return !qforSolver.empty() || killer ; } );
            if (qforSolver.empty() && killer ) {
                break;
            }
            auto s = qforSolver.front();
            qforSolver.pop();
            lk.unlock();
            s->solve();
            Unlocker();
        }
        //  cout << "Worker thread ending." << endl;
    }
    void Unlocker ();
    bool SolverGetterCNT () {
        CntSolver = make_shared<Solver>(createProgtestCntSolver());
        if(CntSolver != nullptr){
            return true;
        }
        else{
            return false;
        }
    }
    bool SolverGetterMin () {
        MinSolver = make_shared<Solver>(createProgtestMinSolver());
        if(MinSolver != nullptr){
            return true;
        }
        else{
            return false;
        }
    }

    bool IsAllcomp () {
        if(counter.load() == m_Companies.size()){
            return true;
        }
        return false;
    }
    void Qpusher () {
        qforSolver.push (CntSolver );
    }
    void Qpusher2 () {
        qforSolver.push (MinSolver );
    }
    void stop() ;
    static bool usingProgtestSolver() {
        return true;
    }

    static void checkAlgorithmMin(APolygon p) {
        // Implement if not using ProgtestSolver
    }

    static void checkAlgorithmCnt(APolygon p) {
        // Implement if not using ProgtestSolver
    }
    mutex mutex2;
    shared_ptr<Solver> CntSolver;
    shared_ptr<Solver> MinSolver;
    queue<shared_ptr<Solver>> qforSolver;
    condition_variable Checker2;
    atomic_size_t counter;
    vector<thread> m_Workers;
    bool killer;

private:
    vector<shared_ptr<CommunicationClass>> m_Companies;
};

class CommunicationClass {
public:
    explicit CommunicationClass(ACompany company) : m_Company(std::move(company)) {}
    void GetCompany (COptimizer & optimizer  ) {
       // cout << "Receiver thread started for company." << endl;
        while (AProblemPack pPack = m_Company->waitForPack()) {
            auto packWrapPtr = make_shared<Cpacker>(pPack);
            m_ProblemPacks.AddtoQueue(packWrapPtr);

            unique_lock<mutex> lk(mutex3);//optimizer.mutex2);
            for (const auto &problem: pPack->m_ProblemsCnt) {
                if(optimizer.CntSolver->hasFreeCapacity()){
                    optimizer.CntSolver->addPolygonProblems(make_shared<Buffer>
                                                                    (Buffer(problem, packWrapPtr)));}
                if (!optimizer.CntSolver->hasFreeCapacity()) {
                    optimizer.Qpusher();
                    optimizer.SolverGetterCNT();
                }
            }
            for (const auto &problem: pPack->m_ProblemsMin) {
                if(optimizer.MinSolver->hasFreeCapacity()){
                    optimizer.MinSolver->addPolygonProblems(make_shared<Buffer>
                                                                    (Buffer(problem, packWrapPtr)));}
                if (!optimizer.MinSolver->hasFreeCapacity()) {
                    optimizer.Qpusher2();
                    optimizer.SolverGetterMin();
                }
            }

            lk.unlock();
        }
        shared_ptr<Cpacker> nullptrpack = nullptr;
        m_ProblemPacks.AddtoQueue(nullptrpack);
        optimizer.counter++;
        unique_lock<mutex> lk(mutex3);//optimizer.mutex2);
        if (optimizer.IsAllcomp()) {
            optimizer.Qpusher();
            optimizer.Qpusher2();
            optimizer.killer = true;
            optimizer.Checker2.notify_all();
        }
        // cout << "Receiver thread ended for company." << endl;
    }
    void SendCompany() {
          //cout << "Returner thread started for company." << endl;
        while ( auto pack = m_ProblemPacks.returnfromqueueu() ) {
            m_Company->solvedPack ( pack->m_ProblemPack );
        }
          // cout << "Returner thread ended for company." << endl;
    }
    void Communicationthread (COptimizer & optimizer ) {
        CommThread[0]=thread(&CommunicationClass::GetCompany, this, ref(optimizer) );
        CommThread[1]=thread(&CommunicationClass::SendCompany, this );
    }
    void joinThreads() {
        for (auto &thread : CommThread) {
            thread.join();
        }
    }
    thread CommThread[2];
    Cpacker m_ProblemPacks;
    ACompany m_Company;

};
void COptimizer::Unlocker () {
    for ( const auto & company : m_Companies )
        company->m_ProblemPacks.NotificationToALL();
}
void COptimizer::start ( int threadCount ) {
   // cout << "Starting COptimizer with " << threadCount << " worker threads." << endl;



    for ( const auto & company : m_Companies )
        company->Communicationthread(*this);
    for ( int i = 0; i < threadCount; i++ )
        m_Workers.emplace_back ( &COptimizer::worker, this );
}
void COptimizer::stop () {
  //   cout << "Stopping COptimizer." << endl;
    for ( auto & worker : m_Workers )
        worker.join();
    for (auto &company : m_Companies) {
        company->CommThread[0].join();
        company->CommThread[1].join();
    }
  //   cout << "COptimizer stopped." << endl;
}
// TODO: COptimizer implementation goes here
//-------------------------------------------------------------------------------------------------------------------------------------------------------------

#ifndef __PROGTEST__
void test () {
    COptimizer optimizer;
    std::vector <ACompanyTest> companies;

    for (int i = 0; i < 200 ; ++ i) {
        auto company = make_shared <CCompanyTest> ();
        companies .push_back (company);
        optimizer . addCompany (company);
    }

    optimizer . start (13);

    optimizer . stop ();
    for (auto & company : companies) {
        if (!company->allProcessed())
            throw logic_error("(some) problems were not correctly processed");
    }
}
int                                    main                                    ( void )
{
    COptimizer optimizer;
    ACompanyTest  company = std::make_shared<CCompanyTest> ();
    optimizer . addCompany ( company );
    optimizer . start ( 1);
    optimizer . stop  ();
    if ( ! company -> allProcessed () )
        throw std::logic_error ( "(some) problems were not correctly processsed" );
    test();
    return 0;
}
#endif /* __PROGTEST__ */
