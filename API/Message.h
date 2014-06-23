/*!
 * \file Message.h
 *
 * \author Clément Bossut
 * \author Théo de la Hogue
 *
 * This code is licensed under the terms of the "CeCILL-C"
 * http://www.cecill.info
 */

#ifndef MESSAGE_H_
#define MESSAGE_H_

#include "StateElement.h"

namespace OSSIA {

class Address;

template <typename T>
class Message : public StateElement {

public:

  typedef T value_type;

  // Constructors, destructor, asssignment
  Message();
  Message(const Message&);
  virtual ~Message();
  Message & operator= (const Message&);

  // Lecture
  virtual void launch() const override;

  // Accessors
  virtual StateElementType getType() const override final
      { return MESSAGE_TYPE; };
  Address getAddress() const;
  void setAddress(const Address);
  value_type getValue() const;
  void setValue(const value_type);
  bool hasValue() const;

  // pimpl idiom
private:
  class Impl;
  Impl * pimpl;

};

}

#endif /* MESSAGE_H_ */
