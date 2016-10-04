/************************************************************
**
** file:	btreearraytestprimitive.cpp
** author:	Andreas Steffens
** license:	GPL v2
**
** description:
**
** This file contains primitive code for the b-tree framework's
** test bench to support array data class testing.
**
************************************************************/

#include "btreearraytestprimitive.h"

#ifndef	BTREEARRAYTESTPRIMITIVE_CPP 
#define	BTREEARRAYTESTPRIMITIVE_CPP

template<class _t_container>
void arrayPrim_add (_t_container *pContainer, typename _t_container::size_type nEntries, btreetest_array_primitive_seek_e eWhere)
{
	typedef typename _t_container::const_iterator			citer_t;
	typedef typename _t_container::size_type				size_type;
	typedef typename _t_container::value_type				value_type;

	size_type		nPos;
	size_type		i;
	value_type		sData;
	citer_t			sCIter;

	for (i = 0; i < nEntries; i++)
	{
		::std::cout << "insert: " << i << " / " << nEntries << "\r" << ::std::flush;

		sData.nDebug = g_nDebug;
		sData.nData = generate_rand32 ();

		g_nDebug++;

		switch (eWhere)
		{
		case BTREETEST_ARRAY_PRIMITIVE_SEEK_BEGIN		:
			{
				nPos = 0;

				break;
			}

		case BTREETEST_ARRAY_PRIMITIVE_SEEK_END		:
			{
				nPos = pContainer->size ();
				
				break;
			}

		case BTREETEST_ARRAY_PRIMITIVE_SEEK_RANDOM	:
			{
				nPos = generate_rand64 () % (pContainer->size () + 1);

				break;
			}

		default									:
			{
				::std::cerr << "ERROR: arrayPrim_add: eWhere corrupted or not set!" << ::std::endl;

				exit (-1);

				break;
			}
		}

		sCIter = pContainer->cbegin ();

		::std::advance (sCIter, nPos);

		pContainer->insert (sCIter, sData);
	}

	::std::cout << "insert: " << i << " / " << nEntries << ::std::endl;
}

template<class _t_container>
void arrayPrim_remove (_t_container *pContainer, typename _t_container::size_type nEntries, btreetest_array_primitive_seek_e eWhere)
{
	typedef typename _t_container::const_iterator			citer_t;
	typedef typename _t_container::size_type				size_type;

	size_type		nPos;
	size_type		i;
	citer_t			sCIter;
	
	for (i = 0; i < nEntries; i++)
	{
		::std::cout << "remove: " << i << " / " << nEntries << "\r" << ::std::flush;

		switch (eWhere)
		{
		case BTREETEST_ARRAY_PRIMITIVE_SEEK_BEGIN		:
			{
				nPos = 0;

				break;
			}

		case BTREETEST_ARRAY_PRIMITIVE_SEEK_END		:
			{
				nPos = pContainer->size ();
				nPos--;
				
				break;
			}

		case BTREETEST_ARRAY_PRIMITIVE_SEEK_RANDOM	:
			{
				nPos = generate_rand64 () % pContainer->size ();

				break;
			}

		default									:
			{
				::std::cerr << "ERROR: arrayPrim_remove: eWhere corrupted or not set!" << ::std::endl;

				exit (-1);

				break;
			}
		}

		sCIter = pContainer->cbegin ();

		::std::advance (sCIter, nPos);

		pContainer->erase (sCIter);
	}

	::std::cout << "remove: " << i << " / " << nEntries << ::std::endl;
}

template<class _t_container>
void arrayPrim_replace (_t_container *pContainer, typename _t_container::size_type nEntries, btreetest_array_primitive_seek_e eWhere)
{
	typedef typename _t_container::const_iterator			citer_t;
	typedef typename _t_container::size_type				size_type;
	typedef typename _t_container::value_type				value_type;

	citer_t			sCIter;
	size_type		nPos;
	size_type		i;
	value_type		sData;

	for (i = 0; i < nEntries; i++)
	{
		::std::cout << "replace: " << i << " / " << nEntries << "\r" << ::std::flush;

		sData.nDebug = g_nDebug;
		sData.nData = generate_rand32 ();

		g_nDebug++;

		switch (eWhere)
		{
		case BTREETEST_ARRAY_PRIMITIVE_SEEK_BEGIN		:
			{
				nPos = 0;

				break;
			}

		case BTREETEST_ARRAY_PRIMITIVE_SEEK_END		:
			{
				nPos = pContainer->size ();
				nPos--;
				
				break;
			}

		case BTREETEST_ARRAY_PRIMITIVE_SEEK_RANDOM	:
			{
				nPos = generate_rand64 () % pContainer->size ();

				break;
			}

		default									:
			{
				::std::cerr << "ERROR: arrayPrim_replace: eWhere corrupted or not set!" << ::std::endl;

				exit (-1);

				break;
			}
		}

		sCIter = pContainer->cbegin ();

		::std::advance (sCIter, nPos);

		pContainer->replace (sCIter, sData);
	}

	::std::cout << "replace: " << i << " / " << nEntries << ::std::endl;
}

#endif // BTREEARRAYTESTPRIMITIVE_CPP
