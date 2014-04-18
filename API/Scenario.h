/*!
 * \file Scenario.h
 *
 * \author Clément Bossut
 * \author Théo de la Hogue
 *
 * This code is licensed under the terms of the "CeCILL-C"
 * http://www.cecill.info
 */

#ifndef SCENARIO_H_
#define SCENARIO_H_

#include <set>

#include "TimeProcess.h"

namespace OSSIA {

class TimeBox;
class TimeNode;

class Scenario : public TimeProcess {

public:

  // Constructors, destructor, assignment
  Scenario();
  Scenario(const Scenario&);
  ~Scenario(); // virtual ?
  Scenario & operator= (const Scenario&);
  // Penser à faire pour Scenario&& aussi

  // Lecture
  virtual void play() const override;

  // Navigation
  std::set<TimeBox*> getTimeBoxes() const;
  std::set<TimeNode*> getTimeNodes() const;

  // pimpl idiom
private:
  class Impl;
  Impl * pimpl;
  //std::unique_ptr ici ?

};

}

#endif /* SCENARIO_H_ */
