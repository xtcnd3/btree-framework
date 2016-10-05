/************************************************************
**
** file:	btreetestcommon.h
** author:	Andreas Steffens
** license:	GPL v2
**
** description:
**
** This file contains code for the b-tree framework's test bench
** common code.
**
************************************************************/

#ifndef	BTREETESTCOMMON_H
#define	BTREETESTCOMMON_H

#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#include <type_traits>
#include <set>
#include <list>
#include <vector>

#include <typeinfo>

#if defined(__GNUC__) || defined(__GNUG__)
 #include <cxxabi.h>
#endif

#include "common/btreecommon.h"


extern uint32_t		g_nDebug;

namespace std
{

	void swap (const uint32_t &_Left, const uint32_t &_Right);
	
};

template<class _t_key, class _t_map>
class CBTreePairTest	:	public ::std::pair < _t_key, _t_map >
{
public:

				CBTreePairTest<_t_key, _t_map> ()
					:	::std::pair<_t_key, _t_map> ()
				{

				};

				CBTreePairTest<_t_key, _t_map> (const CBTreePairTest<_t_key, _t_map> &rPair)
					:	::std::pair<_t_key, _t_map> ()
				{
					(typename ::std::remove_const <_t_key>::type &) this->first = rPair.first;
					(typename ::std::remove_const <_t_map>::type &) this->second = rPair.second;
				};

	explicit	CBTreePairTest<_t_key, _t_map> (const ::std::pair<_t_key, _t_map> &rPair)
					:	::std::pair<_t_key, _t_map> ()
				{
					(typename ::std::remove_const <_t_key>::type &) this->first = rPair.first;
					(typename ::std::remove_const <_t_map>::type &) this->second = rPair.second;
				};

				CBTreePairTest<_t_key, _t_map> (const _t_key &rFirst, const _t_map &rSecond)
					:	::std::pair<_t_key, _t_map> (rFirst, rSecond)
				{

				};
				
				~CBTreePairTest<_t_key, _t_map> ()
				{

				};

	CBTreePairTest	
				operator= (const CBTreePairTest<_t_key, _t_map> &rPair)
				{
					(typename ::std::remove_const <_t_key>::type &) this->first = rPair.first;
					(typename ::std::remove_const <_t_map>::type &) this->second = rPair.second;

					return (*this);
				};

	bool		operator== (const CBTreePairTest &rRhs) const
				{
					return ((this->first == rRhs.first) && (this->second.nData == rRhs.second.nData));
				};

	bool		operator!= (const CBTreePairTest &rRhs) const
				{
					return ((this->first != rRhs.first) || (this->second.nData != rRhs.second.nData));
				};
};

uint32_t generate_rand32 ();
uint64_t generate_rand64 ();

/************************************************
**	get_begin
************************************************/

template<class _t_container>
void get_begin (_t_container *pContainer, typename ::std::enable_if< !::std::is_same <typename _t_container::iterator, typename _t_container::const_iterator>::value, typename _t_container::iterator>::type &rIter)
{
	rIter = pContainer->begin ();
}

template<class _t_container>
void get_begin (_t_container *pContainer, typename _t_container::const_iterator &rIter)
{
	rIter = pContainer->cbegin ();
}

template<class _t_container>
void get_begin (_t_container *pContainer, typename ::std::enable_if< !::std::is_same <typename _t_container::reverse_iterator, typename _t_container::const_reverse_iterator>::value, typename _t_container::reverse_iterator>::type &rIter)
{
	rIter = pContainer->rbegin ();
}

template<class _t_container>
void get_begin (_t_container *pContainer, typename _t_container::const_reverse_iterator &rIter)
{
	rIter = pContainer->crbegin ();
}

/************************************************
**	get_end
************************************************/

template<class _t_container>
void get_end (_t_container *pContainer, typename ::std::enable_if< !::std::is_same <typename _t_container::iterator, typename _t_container::const_iterator>::value, typename _t_container::iterator>::type &rIter)
{
	rIter = pContainer->end ();
}

template<class _t_container>
void get_end (_t_container *pContainer, typename _t_container::const_iterator &rIter)
{
	rIter = pContainer->cend ();
}

template<class _t_container>
void get_end (_t_container *pContainer, typename ::std::enable_if< !::std::is_same <typename _t_container::reverse_iterator, typename _t_container::const_reverse_iterator>::value, typename _t_container::reverse_iterator>::type &rIter)
{
	rIter = pContainer->rend ();
}

template<class _t_container>
void get_end (_t_container *pContainer, typename _t_container::const_reverse_iterator &rIter)
{
	rIter = pContainer->crend ();
}

/************************************************
**	container_assign
************************************************/

template<class _t_iterator, class _t_container>
void container_assign (_t_container *pContainer, _t_iterator &rItBegin, _t_iterator &rItEnd)
{
	pContainer->assign (rItBegin, rItEnd);
}

#if defined(__GNUC__) || defined(__GNUG__)

template<class _t_iterator>
void container_assign (::std::list<typename _t_iterator::value_type> *pContainer, _t_iterator &rItBegin, _t_iterator &rItEnd)
{
	pContainer->clear ();
	pContainer->insert (pContainer->cbegin (), rItBegin, rItEnd);
}

template<class _t_iterator>
void container_assign (::std::vector<typename _t_iterator::value_type> *pContainer, _t_iterator &rItBegin, _t_iterator &rItEnd)
{
	_t_iterator		rIt;

	pContainer->clear ();

	for (rIt = rItBegin; rIt != rItEnd; ::std::advance (rIt, 1))
	{
		typename _t_iterator::value_type		sData ((typename _t_iterator::value_type) (*rIt));

		pContainer->push_back (sData);
	}
}

#endif

/************************************************
**	container_insert
************************************************/

template<class _t_iterator, class _t_container_iterator, class _t_container>
void container_insert (_t_container *pContainer, _t_container_iterator &rItPos, _t_iterator &rItBegin, _t_iterator &rItEnd)
{
	pContainer->insert (rItPos, rItBegin, rItEnd);
}

#if defined(__GNUC__) || defined(__GNUG__)

template<class _t_iterator, class _t_container_iterator>
void container_insert (::std::vector<::std::pair<typename ::std::add_const<typename _t_iterator::value_type::first_type>::type, typename _t_iterator::value_type::second_type>> *pContainer, _t_container_iterator &rItPos, _t_iterator &rItBegin, _t_iterator &rItEnd)
{
	container_insert (pContainer, (const _t_container_iterator &) rItPos, rItBegin, rItEnd);
}

template<class _t_iterator, class _t_container_iterator>
void container_insert (::std::vector<::std::pair<typename ::std::add_const<typename _t_iterator::value_type::first_type>::type, typename _t_iterator::value_type::second_type>> *pContainer, const _t_container_iterator &rItPos, _t_iterator &rItBegin, _t_iterator &rItEnd)
{
	typedef typename ::std::pair<typename ::std::add_const<typename _t_iterator::value_type::first_type>::type, typename _t_iterator::value_type::second_type>
																					pair_t;

	typedef typename ::std::vector<pair_t>::size_type								size_type;
	typedef typename _t_iterator::value_type										value_type_iter;
	typedef typename _t_container_iterator::value_type								value_type_cont;

	_t_iterator				rIt;
	size_type				nSize = pContainer->size ();
	_t_container_iterator	sContBegin;
	size_type				nOffset;
	size_type				nGrow;
	size_type				i;
	value_type_cont			*pData;

	get_begin (pContainer, sContBegin);
	nOffset = ::std::distance (sContBegin, rItPos);

	nGrow = ::std::distance (rItBegin, rItEnd);

	pContainer->resize (nSize + nGrow);

	pData = pContainer->data ();

	memmove ((void *) &(pData[nOffset + nGrow]), (void *) &(pData[nOffset]), sizeof (value_type_cont) * (nSize - nOffset));

	for (rIt = rItBegin, i = 0; rIt != rItEnd; ::std::advance (rIt, 1), i++)
	{
		value_type_iter		sData ((value_type_iter) (*rIt));

		entry_conversion (pData[nOffset + i], sData);
	}
}

/*
template<class _t_iterator>
void container_insert (::std::vector<typename _t_iterator::value_type> *pContainer, const typename ::std::vector<typename _t_iterator::value_type>::iterator &rItPos, _t_iterator &rItBegin, _t_iterator &rItEnd)
{
	typedef typename ::std::vector<typename _t_iterator::value_type>::size_type		size_type;
	typedef typename _t_iterator::value_type										value_type;

	_t_iterator		rIt;
	size_type		nSize = pContainer->size ();
	size_type		nOffset = ::std::distance (pContainer->begin (), rItPos);
	size_type		nGrow;
	size_type		i;
	value_type		*pData;

	nGrow = ::std::distance (rItBegin, rItEnd);

	pContainer->resize (nSize + nGrow);

	pData = pContainer->data ();

	memmove ((void *) &(pData[nOffset + nGrow]), (void *) &(pData[nOffset]), sizeof (value_type) * (nSize - nOffset));

	for (rIt = rItBegin, i = 0; rIt != rItEnd; ::std::advance (rIt, 1), i++)
	{
		value_type		sData ((value_type) (*rIt));

		memcpy ((void *) &(pData[nOffset + i]), (void *) &sData, sizeof (value_type));
	}
}

template<class _t_iterator>
void container_insert (::std::vector<typename _t_iterator::value_type> *pContainer, const typename ::std::vector<typename _t_iterator::value_type>::const_iterator &rItPos, _t_iterator &rItBegin, _t_iterator &rItEnd)
{
	typedef typename ::std::vector<typename _t_iterator::value_type>::size_type		size_type;
	typedef typename _t_iterator::value_type										value_type;

	_t_iterator		rIt;
	size_type		nSize = pContainer->size ();
	size_type		nOffset = ::std::distance (pContainer->cbegin (), rItPos);
	size_type		nGrow;
	size_type		i;
	value_type		*pData;

	nGrow = ::std::distance (rItBegin, rItEnd);

	pContainer->resize (nSize + nGrow);

	pData = pContainer->data ();

	memmove ((void *) &(pData[nOffset + nGrow]), (void *) &(pData[nOffset]), sizeof (value_type) * (nSize - nOffset));

	for (rIt = rItBegin, i = 0; rIt != rItEnd; ::std::advance (rIt, 1), i++)
	{
		value_type		sData ((value_type) (*rIt));

		memcpy ((void *) &(pData[nOffset + i]), (void *) &sData, sizeof (value_type));
	}
}

template<class _t_iterator>
void container_insert (::std::vector<typename _t_iterator::value_type> *pContainer, typename ::std::vector<typename _t_iterator::value_type>::iterator &rItPos, _t_iterator &rItBegin, _t_iterator &rItEnd)
{
	container_insert (pContainer, (const typename ::std::vector<typename _t_iterator::value_type>::iterator &) rItPos, rItBegin, rItEnd);
}

template<class _t_iterator>
void container_insert (::std::vector<typename _t_iterator::value_type> *pContainer, typename ::std::vector<typename _t_iterator::value_type>::const_iterator &rItPos, _t_iterator &rItBegin, _t_iterator &rItEnd)
{
	container_insert (pContainer, (const typename ::std::vector<typename _t_iterator::value_type>::const_iterator &) rItPos, rItBegin, rItEnd);
}
*/
#endif

/************************************************
**	is_const_iterator
************************************************/

template<class _t_container>
bool is_const_iterator (_t_container *pContainer, typename ::std::enable_if< !::std::is_same <typename _t_container::iterator, typename _t_container::const_iterator>::value, typename _t_container::iterator>::type &)
{
	return (false);
}

template<class _t_container>
bool is_const_iterator (_t_container *pContainer, typename _t_container::const_iterator &)
{
	return (true);
}

template<class _t_container>
bool is_const_iterator (_t_container *pContainer, typename ::std::enable_if< !::std::is_same <typename _t_container::reverse_iterator, typename _t_container::const_reverse_iterator>::value, typename _t_container::reverse_iterator>::type &)
{
	return (false);
}

template<class _t_container>
bool is_const_iterator (_t_container *pContainer, typename _t_container::const_reverse_iterator &)
{
	return (true);
}

/************************************************
**	is_reverse_iterator
************************************************/

template<class _t_container>
bool is_reverse_iterator (_t_container *pContainer, typename ::std::enable_if< !::std::is_same <typename _t_container::iterator, typename _t_container::const_iterator>::value, typename _t_container::iterator>::type &)
{
	return (false);
}

template<class _t_container>
bool is_reverse_iterator (_t_container *pContainer, typename _t_container::const_iterator &)
{
	return (false);
}

template<class _t_container>
bool is_reverse_iterator (_t_container *pContainer, typename ::std::enable_if< !::std::is_same <typename _t_container::reverse_iterator, typename _t_container::const_reverse_iterator>::value, typename _t_container::reverse_iterator>::type &)
{
	return (true);
}

template<class _t_container>
bool is_reverse_iterator (_t_container *pContainer, typename _t_container::const_reverse_iterator &)
{
	return (true);
}

template<class _t>
void get_typename (_t &rType, ::std::string &rString)
{
#if defined(__GNUC__) || defined(__GNUG__)

	int		nStatus;
	char	*pszTypeid = abi::__cxa_demangle (typeid (_t).name (), 0, 0, &nStatus);

	if (pszTypeid != NULL)
	{
		rString = pszTypeid;
		
		free ((void *) pszTypeid);
	}
	else
	{
		rString = typeid (_t).name ();
	}
	
#else

	rString = typeid (_t).name ();
	
#endif
}

#endif // BTREETESTCOMMON_H

