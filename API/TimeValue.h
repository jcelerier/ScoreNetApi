/*!
 * \file TimeValue.h
 *
 * \author Clément Bossut
 * \author Théo de la Hogue
 *
 * This code is licensed under the terms of the "CeCILL-C"
 * http://www.cecill.info
 */

#ifndef TIMEVALUE_H_
#define TIMEVALUE_H_

namespace OSSIA {

class TimeValue {

	public:
  // Constructors, destructor, assignment
  TimeValue();
  TimeValue(const int);
  TimeValue & operator= (const int);

  // Cast
  operator int();
  // Soustraction?

  int date{};
};

}



#endif /* TIMEVALUE_H_ */
