#pragma once
#include <iterator>

template <typename T>
struct PointedType;

template <typename T>
struct PointedType<T*>
{
  typedef T value_type;
};

template <typename InputIterator>
struct DereferenceIterator
{
  typedef std::input_iterator_tag iterator_category;
  typedef typename PointedType<
			typename std::iterator_traits<InputIterator>::value_type>::value_type
			value_type;
  typedef typename std::iterator_traits<InputIterator>::difference_type
			difference_type;
  typedef value_type* pointer;
  typedef value_type& reference;

  public:
	explicit DereferenceIterator(const InputIterator& ii)
	  : it(ii) {}

	// Returns the object pointed by the object referenced by it
	reference operator*()  const { return **it; }
	pointer   operator->() const { return *it; }

	DereferenceIterator& operator++()
	{
		++it;
		return *this;
	}

	DereferenceIterator operator++(int)
	{
		DereferenceIterator tmp = *this;
		++it;
		return tmp;
	}

	bool equals(const DereferenceIterator<InputIterator> & di) const
	{
		return di.it == it;
	}

  private:
	InputIterator it;
};

// Equality functions

template <typename InputIterator>
inline bool operator==(const DereferenceIterator<InputIterator>& di1,
					   const DereferenceIterator<InputIterator>& di2)
{
  return di1.equals(di2);
}

template <typename InputIterator>
inline bool operator!=(const DereferenceIterator<InputIterator>& di1,
					   const DereferenceIterator<InputIterator>& di2)
{
  return ! (di1 == di2);
}

//Helper function

template <typename InputIterator>
DereferenceIterator<InputIterator> deref_iterator(const InputIterator& ii)
{
  return DereferenceIterator<InputIterator>(ii);
}
