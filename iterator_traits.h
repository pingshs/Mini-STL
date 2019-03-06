
namespace MiniSTL{

#ifndef _ITERATROR_TRAITS_
#define _ITERATROR_TRAITS_

struct input_iterator_tag{};
struct output_iterator_tag{};
struct forward_iterator_tag : public input_iterator_tag {};
struct bidirectional_iterator_tag : public forward_iterator_tag {};
struct random_access_iterator_tag : bidirectional_iterator_tag {};

template<typename IterT>
class iterator_traits{
public:
	typedef typename IterT::iterator_category iterator_category; 	
};

template<typename IterT>
class iterator_traits<IterT*>{
public:
	typedef random_access_iterator_tag iterator_category;
};

#endif

}
