#ifndef __ILDTPCDETECTOR__
#define __ILDTPCDETECTOR__

/** TPC to be used for ILD DBD studies 
 *
 * @author S.Aplin DESY
 */

#include "kaltest/TVKalDetector.h"

class TNode;

namespace gear{
  class GearMgr ;
}

class IGeoSvc;

class ILDTPCKalDetector : public TVKalDetector {
public:
  
  /** Initialize the TPC from GEAR */
  ILDTPCKalDetector( const gear::GearMgr& gearMgr, IGeoSvc* geoSvc=0 );
  
  
private:
  
};

#endif
