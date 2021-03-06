#ifndef MARLIN_EVT_SEEDER_H
#define MARLIN_EVT_SEEDER_H 1

#include "EventSeeder/IEventSeeder.h"
#include <GaudiKernel/Service.h>
#include <vector>
#include <map>

/** Processor event seeder - provides independent pseudo-randomly generated seeds 
 *  for registered processors on an event by event basis.   
 * 
 *      This Class is used to provide individual pseudo-random numbers to Processors on
 *      an event-by-event and run-by-run basis. These may be used by Processors to 
 *	  seed their random number generator of choice. In order to use this service 
 *	  a Processor must register itself in the init method using:
 *
 *             Global::EVENTSEEDER->registerProcessor(this);
 *
 *      and should retrieve its allocated event seed during processEvent using:
 *
 *             int eventSeed = Global::EVENTSEEDER->getSeed(this);
 *
 *	  and include the header file:
 *
 *	  	#include "marlin/ProcessorEventSeeder.h"	      	
 *
 *      The global seed is used for a complete job and is set in the Global steering parameters thus:
 *     
 *             <parameter name="RandomSeed" value="1234567890"/>
 *
 *      Note that the value must be a positive integer, with max value 2,147,483,647
 *      A pseudo-random event seed is generated using a three step hashing function of unsigned ints,
 *	  in the following order: event_number, run_number, RandomSeed. The hashed int from each step 
 *	  in the above order is used as input to the next hash step. This is used to ensure that in 
 *	  the likely event of similar values of event_number, run_number and RandomSeed, different 
 *	  event seeds will be generated. 
 *    
 *	  The event seed is then used to seed rand via srand(seed) and then rand is used to 
 *	  generate one seed per registered processor.
 *
 *	  This mechanism ensures reproducible results for every event, regardless of the sequence 
 *	  in which the event is processed in a Marlin job, whilst maintaining the full 32bit range 
 *	  for event and run numbers.
 *   
 *      If a call is made to getSeed( Processor* ) preceededing a call to registerProcessor( Processor* )
 *      an exception will be thrown.
 *
 *  @author S.J. Aplin, DESY
 */

class MarlinEvtSeeder : public extends<Service, IEventSeeder>
{
  public:

    /** Constructor and Destructor */
    MarlinEvtSeeder(const std::string& name, ISvcLocator* svc);
    ~MarlinEvtSeeder() { } ;

    /** Called by Algorithms to register themselves for the seeding service. 
    */
    void registerAlg( Algorithm* alg ) override;

    /** Called by Algorithms to obtain seed assigned to it for the current event.
    */
    unsigned int getSeed(Algorithm* alg, int eventNumber, int runNumber) override;

    StatusCode initialize() override;
    StatusCode finalize() override;

  private:

    void refreshSeeds();

    int _evtNo;
    int _runNo;

    /** Global seed for current Job. Set in steering file.
    */
    Gaudi::Property<int> _globalSeed{this, "RandomSeed", 0};

    /** Vector to hold pair of pointers to the registered processors and their assigned seeds
    */
    std::vector< std::pair<Algorithm*, unsigned int> > _vAlgSeed;
} ;

#endif
