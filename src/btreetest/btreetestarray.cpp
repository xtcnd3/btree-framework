/************************************************************
**
** file:	btreetestarray.cpp
** author:	Andreas Steffens
** license:	GPL v2
**
** description:
**
** This file contains array application class code derived
** from the b-tree framework's array data class to allow
** for the array data class being tested by having
** the array application class act as a wrapper.
**
************************************************************/

#ifndef BTREETESTARRAY_CPP
#define	BTREETESTARRAY_CPP

#include "btreetestarray.h"

template <class _t_sizetype, class _t_nodeiter, class _t_subnodeiter, class _t_datalayerproperties, class _t_datalayer>
CBTreeArrayTest<_t_sizetype, _t_nodeiter, _t_subnodeiter, _t_datalayerproperties, _t_datalayer>::CBTreeArrayTest 
	(
		_t_datalayerproperties &rDataLayerProperties, 
		bayerTreeCacheDescription_t *psCacheDescription, 
		_t_subnodeiter nNodeSize
	)
	:	CBTreeArray<arrayEntry_t, _t_sizetype, _t_nodeiter, _t_subnodeiter, _t_datalayerproperties, _t_datalayer>
	(
		rDataLayerProperties, 
		psCacheDescription, 
		nNodeSize
	)
{
	m_pClRef = new std::list<arrayEntry_t> ();
	
	BTREE_ASSERT (m_pClRef != NULL, "CBTreeArrayTest<_t_sizetype, _t_nodeiter, _t_subnodeiter, _t_datalayerproperties, _t_datalayer>::CBTreeArrayTest (bool, char *, bayerTreeCacheDescription_t *, _t_subnodeiter): insufficient memory!");
}

template <class _t_sizetype, class _t_nodeiter, class _t_subnodeiter, class _t_datalayerproperties, class _t_datalayer>
CBTreeArrayTest<_t_sizetype, _t_nodeiter, _t_subnodeiter, _t_datalayerproperties, _t_datalayer>::CBTreeArrayTest (CBTreeArrayTest<_t_sizetype, _t_nodeiter, _t_subnodeiter, _t_datalayerproperties, _t_datalayer> &rBT)
	:	CBTreeArray<arrayEntry_t, _t_sizetype, _t_nodeiter, _t_subnodeiter, _t_datalayerproperties, _t_datalayer>
	(
		rBT, false
	)
{
	m_pClRef = new std::list<arrayEntry_t> ();

	BTREE_ASSERT (m_pClRef != NULL, "CBTreeArrayTest<_t_sizetype, _t_nodeiter, _t_subnodeiter, _t_datalayerproperties, _t_datalayer>::CBTreeArrayTest (CBTreeArrayTest<_t_sizetype, _t_nodeiter, _t_subnodeiter, _t_datalayerproperties, _t_datalayer> &): insufficient memory!");

	CBTreeArray<arrayEntry_t, _t_sizetype, _t_nodeiter, _t_subnodeiter, _t_datalayerproperties, _t_datalayer>::assign (rBT);

	*m_pClRef = *(rBT.m_pClRef);

	test ();
}

template <class _t_sizetype, class _t_nodeiter, class _t_subnodeiter, class _t_datalayerproperties, class _t_datalayer>
CBTreeArrayTest<_t_sizetype, _t_nodeiter, _t_subnodeiter, _t_datalayerproperties, _t_datalayer>::~CBTreeArrayTest ()
{
	delete m_pClRef;
}

template <class _t_sizetype, class _t_nodeiter, class _t_subnodeiter, class _t_datalayerproperties, class _t_datalayer>
template <class _t_iterator>
void CBTreeArrayTest<_t_sizetype, _t_nodeiter, _t_subnodeiter, _t_datalayerproperties, _t_datalayer>::assign (_t_iterator sItFirst, _t_iterator sItLast)
{
	m_pClRef->assign<_t_iterator> (sItFirst, sItLast);

	CBTreeArray<arrayEntry_t, _t_sizetype, _t_nodeiter, _t_subnodeiter, _t_datalayerproperties, _t_datalayer>::template assign< _t_iterator > (sItFirst, sItLast);

	test ();
}

template <class _t_sizetype, class _t_nodeiter, class _t_subnodeiter, class _t_datalayerproperties, class _t_datalayer>
void CBTreeArrayTest<_t_sizetype, _t_nodeiter, _t_subnodeiter, _t_datalayerproperties, _t_datalayer>::assign (_t_sizetype nNewSize, const arrayEntry_t& rVal)
{
	m_pClRef->assign ((size_t) nNewSize, rVal);

	CBTreeArray<arrayEntry_t, _t_sizetype, _t_nodeiter, _t_subnodeiter, _t_datalayerproperties, _t_datalayer>::assign (nNewSize, rVal);

	test ();
}

template <class _t_sizetype, class _t_nodeiter, class _t_subnodeiter, class _t_datalayerproperties, class _t_datalayer>
void CBTreeArrayTest<_t_sizetype, _t_nodeiter, _t_subnodeiter, _t_datalayerproperties, _t_datalayer>::push_back (const arrayEntry_t& rData)
{
	m_pClRef->push_back (rData);

	CBTreeArray<arrayEntry_t, _t_sizetype, _t_nodeiter, _t_subnodeiter, _t_datalayerproperties, _t_datalayer>::push_back (rData);

	test ();
}

template <class _t_sizetype, class _t_nodeiter, class _t_subnodeiter, class _t_datalayerproperties, class _t_datalayer>
void CBTreeArrayTest<_t_sizetype, _t_nodeiter, _t_subnodeiter, _t_datalayerproperties, _t_datalayer>::pop_back ()
{
	m_pClRef->pop_back ();

	CBTreeArray<arrayEntry_t, _t_sizetype, _t_nodeiter, _t_subnodeiter, _t_datalayerproperties, _t_datalayer>::pop_back ();

	test ();
}

template <class _t_sizetype, class _t_nodeiter, class _t_subnodeiter, class _t_datalayerproperties, class _t_datalayer>
template <class _t_iterator>
typename CBTreeArrayTest<_t_sizetype, _t_nodeiter, _t_subnodeiter, _t_datalayerproperties, _t_datalayer>::iterator
	CBTreeArrayTest<_t_sizetype, _t_nodeiter, _t_subnodeiter, _t_datalayerproperties, _t_datalayer>::insert
	(
		typename CBTreeArrayTest<_t_sizetype, _t_nodeiter, _t_subnodeiter, _t_datalayerproperties, _t_datalayer>::const_iterator	sCIterPos, 
		_t_iterator																													sItFirst, 
		_t_iterator																													sItLast
	)
{
	typedef typename CBTreeArrayTest<_t_sizetype, _t_nodeiter, _t_subnodeiter, _t_datalayerproperties, _t_datalayer>::iterator			iter_t;
	typedef typename CBTreeArrayTest<_t_sizetype, _t_nodeiter, _t_subnodeiter, _t_datalayerproperties, _t_datalayer>::const_iterator	citer_t;

	typedef typename ::std::list<arrayEntry_t>::iterator																				listiter_t;

	listiter_t			sListIter = m_pClRef->begin ();
	_t_sizetype			nDiff = sCIterPos - this->cbegin ();

	::std::advance<listiter_t> (sListIter, (size_t) nDiff);

	m_pClRef->insert<_t_iterator> (sListIter, sItFirst, sItLast);

	iter_t			sRetval (this, false);

	sRetval = CBTreeArray<arrayEntry_t, _t_sizetype, _t_nodeiter, _t_subnodeiter, _t_datalayerproperties, _t_datalayer>::template insert<_t_iterator> (sCIterPos, sItFirst, sItLast);

	test ();

	return (sRetval);
}	

template <class _t_sizetype, class _t_nodeiter, class _t_subnodeiter, class _t_datalayerproperties, class _t_datalayer>
typename CBTreeArrayTest<_t_sizetype, _t_nodeiter, _t_subnodeiter, _t_datalayerproperties, _t_datalayer>::iterator
	CBTreeArrayTest<_t_sizetype, _t_nodeiter, _t_subnodeiter, _t_datalayerproperties, _t_datalayer>::insert
	(
		typename CBTreeArrayTest<_t_sizetype, _t_nodeiter, _t_subnodeiter, _t_datalayerproperties, _t_datalayer>::const_iterator sCIterPos, 
		const arrayEntry_t& rData
	)
{
	typedef typename CBTreeArrayTest<_t_sizetype, _t_nodeiter, _t_subnodeiter, _t_datalayerproperties, _t_datalayer>::iterator			iter_t;
	typedef typename CBTreeArrayTest<_t_sizetype, _t_nodeiter, _t_subnodeiter, _t_datalayerproperties, _t_datalayer>::const_iterator	citer_t;

	typedef typename ::std::list<arrayEntry_t>::iterator																				listiter_t;

	listiter_t			sListIter = m_pClRef->begin ();
	_t_sizetype			nDiff = sCIterPos - this->cbegin ();

	::std::advance<listiter_t> (sListIter, (size_t) nDiff);

	m_pClRef->insert (sListIter, rData);

	iter_t			sRetval (this, false);

	sRetval = CBTreeArray<arrayEntry_t, _t_sizetype, _t_nodeiter, _t_subnodeiter, _t_datalayerproperties, _t_datalayer>::insert (sCIterPos, rData);

	test ();

	return (sRetval);
}

template <class _t_sizetype, class _t_nodeiter, class _t_subnodeiter, class _t_datalayerproperties, class _t_datalayer>
typename CBTreeArrayTest<_t_sizetype, _t_nodeiter, _t_subnodeiter, _t_datalayerproperties, _t_datalayer>::iterator
	CBTreeArrayTest<_t_sizetype, _t_nodeiter, _t_subnodeiter, _t_datalayerproperties, _t_datalayer>::insert
	(
		typename CBTreeArrayTest<_t_sizetype, _t_nodeiter, _t_subnodeiter, _t_datalayerproperties, _t_datalayer>::const_iterator sCIterPos, 
		const _t_sizetype nLen, 
		const arrayEntry_t& rData
	)
{
	typedef typename CBTreeArrayTest<_t_sizetype, _t_nodeiter, _t_subnodeiter, _t_datalayerproperties, _t_datalayer>::iterator			iter_t;
	typedef typename CBTreeArrayTest<_t_sizetype, _t_nodeiter, _t_subnodeiter, _t_datalayerproperties, _t_datalayer>::const_iterator	citer_t;

	typedef typename ::std::list<arrayEntry_t>::iterator																				listiter_t;

	listiter_t			sListIter = m_pClRef->begin ();
	_t_sizetype			nDiff = sCIterPos - this->cbegin ();

	::std::advance<listiter_t> (sListIter, (size_t) nDiff);

	m_pClRef->insert (sListIter, (size_t) nLen, rData);

	iter_t			sRetval (this, false);

	sRetval = CBTreeArray<arrayEntry_t, _t_sizetype, _t_nodeiter, _t_subnodeiter, _t_datalayerproperties, _t_datalayer>::insert (sCIterPos, nLen, rData);

	test ();

	return (sRetval);
}

template <class _t_sizetype, class _t_nodeiter, class _t_subnodeiter, class _t_datalayerproperties, class _t_datalayer>
typename CBTreeArrayTest<_t_sizetype, _t_nodeiter, _t_subnodeiter, _t_datalayerproperties, _t_datalayer>::iterator
	CBTreeArrayTest<_t_sizetype, _t_nodeiter, _t_subnodeiter, _t_datalayerproperties, _t_datalayer>::erase
	(
		typename CBTreeArrayTest<_t_sizetype, _t_nodeiter, _t_subnodeiter, _t_datalayerproperties, _t_datalayer>::const_iterator sCIterPos
	)
{
	typedef typename CBTreeArrayTest<_t_sizetype, _t_nodeiter, _t_subnodeiter, _t_datalayerproperties, _t_datalayer>::iterator			iter_t;
	
	typedef typename ::std::list<arrayEntry_t>::iterator																				listiter_t;

	listiter_t			sListIter = m_pClRef->begin ();
	_t_sizetype			nDiff = sCIterPos - this->cbegin ();

	::std::advance<listiter_t> (sListIter, (size_t) nDiff);

	m_pClRef->erase (sListIter);

	iter_t			sRetval (this, false);

	sRetval = CBTreeArray<arrayEntry_t, _t_sizetype, _t_nodeiter, _t_subnodeiter, _t_datalayerproperties, _t_datalayer>::erase (sCIterPos);

	test ();

	return (sRetval);
}

template <class _t_sizetype, class _t_nodeiter, class _t_subnodeiter, class _t_datalayerproperties, class _t_datalayer>
typename CBTreeArrayTest<_t_sizetype, _t_nodeiter, _t_subnodeiter, _t_datalayerproperties, _t_datalayer>::iterator
	CBTreeArrayTest<_t_sizetype, _t_nodeiter, _t_subnodeiter, _t_datalayerproperties, _t_datalayer>::erase
	(
		typename CBTreeArrayTest<_t_sizetype, _t_nodeiter, _t_subnodeiter, _t_datalayerproperties, _t_datalayer>::const_iterator sCIterFirst, 
		typename CBTreeArrayTest<_t_sizetype, _t_nodeiter, _t_subnodeiter, _t_datalayerproperties, _t_datalayer>::const_iterator sCIterLast
	)
{
	typedef typename CBTreeArrayTest<_t_sizetype, _t_nodeiter, _t_subnodeiter, _t_datalayerproperties, _t_datalayer>::iterator			iter_t;
	
	typedef typename ::std::list<arrayEntry_t>::iterator																				listiter_t;

	listiter_t			sListIterFirst = m_pClRef->begin ();
	listiter_t			sListIterLast = m_pClRef->begin ();

	_t_sizetype			nDiff = sCIterFirst - this->cbegin ();

	::std::advance<listiter_t> (sListIterFirst, (size_t) nDiff);

	nDiff = sCIterLast - this->cbegin ();

	::std::advance<listiter_t> (sListIterLast, (size_t) nDiff);

	m_pClRef->erase (sListIterFirst, sListIterLast);

	iter_t			sRetval (this, false);

	sRetval = CBTreeArray<arrayEntry_t, _t_sizetype, _t_nodeiter, _t_subnodeiter, _t_datalayerproperties, _t_datalayer>::erase (sCIterFirst, sCIterLast);

	test ();

	return (sRetval);
}

template <class _t_sizetype, class _t_nodeiter, class _t_subnodeiter, class _t_datalayerproperties, class _t_datalayer>
void CBTreeArrayTest<_t_sizetype, _t_nodeiter, _t_subnodeiter, _t_datalayerproperties, _t_datalayer>::swap
	(
		CBTreeArrayTest<_t_sizetype, _t_nodeiter, _t_subnodeiter, _t_datalayerproperties, _t_datalayer> &rArray
	)
{
	CBTreeArray<arrayEntry_t, _t_sizetype, _t_nodeiter, _t_subnodeiter, _t_datalayerproperties, _t_datalayer>	&rBtrArray = 
		dynamic_cast <CBTreeArray<arrayEntry_t, _t_sizetype, _t_nodeiter, _t_subnodeiter, _t_datalayerproperties, _t_datalayer> &> (rArray);

	m_pClRef->swap (*(rArray.m_pClRef));

	CBTreeArray<arrayEntry_t, _t_sizetype, _t_nodeiter, _t_subnodeiter, _t_datalayerproperties, _t_datalayer>::swap (rBtrArray);

	test ();
}

template <class _t_sizetype, class _t_nodeiter, class _t_subnodeiter, class _t_datalayerproperties, class _t_datalayer>
_t_sizetype CBTreeArrayTest<_t_sizetype, _t_nodeiter, _t_subnodeiter, _t_datalayerproperties, _t_datalayer>::insert_at (const _t_sizetype nPos, arrayEntry_t const &rData)
{
	_t_sizetype									nRslt;
	typename std::list<arrayEntry_t>::iterator	itRef = m_pClRef->begin ();
	_t_sizetype									nFForward;

	for (nFForward = 0; nFForward < nPos; nFForward++)
	{
		itRef++;
	}

	m_pClRef->insert (itRef, rData);

	nRslt = CBTreeArray<arrayEntry_t, _t_sizetype, _t_nodeiter, _t_subnodeiter, _t_datalayerproperties, _t_datalayer>::insert_at (nPos, rData);

	test ();

	return (nRslt);
}

template <class _t_sizetype, class _t_nodeiter, class _t_subnodeiter, class _t_datalayerproperties, class _t_datalayer>
_t_sizetype CBTreeArrayTest<_t_sizetype, _t_nodeiter, _t_subnodeiter, _t_datalayerproperties, _t_datalayer>::replace_at (const _t_sizetype nStart, const _t_sizetype nLen, arrayEntry_t *pSrcData)
{
	_t_sizetype									nRslt;
	typename std::list<arrayEntry_t>::iterator	itRef = m_pClRef->begin ();
	_t_sizetype									nFForward;

	for (nFForward = 0; nFForward < nStart; nFForward++)
	{
		itRef++;
	}

	for (; nFForward < (nStart + nLen); nFForward++)
	{
		m_pClRef->insert (itRef, pSrcData[nStart - nFForward]);
		
		itRef++;
		
		m_pClRef->erase (itRef);
	}

	nRslt = CBTreeArray<arrayEntry_t, _t_sizetype, _t_nodeiter, _t_subnodeiter, _t_datalayerproperties, _t_datalayer>::replace_at (nStart, nLen, pSrcData);

	test ();

	return (nRslt);
}

template <class _t_sizetype, class _t_nodeiter, class _t_subnodeiter, class _t_datalayerproperties, class _t_datalayer>
_t_sizetype CBTreeArrayTest<_t_sizetype, _t_nodeiter, _t_subnodeiter, _t_datalayerproperties, _t_datalayer>::remove_at (const _t_sizetype nPos)
{
	_t_sizetype									nRslt;
	typename std::list<arrayEntry_t>::iterator	itRef = m_pClRef->begin ();
	_t_sizetype									nFForward;

	for (nFForward = 0; nFForward < nPos; nFForward++)
	{
		itRef++;
	}

	m_pClRef->erase (itRef);

	nRslt = CBTreeArray<arrayEntry_t, _t_sizetype, _t_nodeiter, _t_subnodeiter, _t_datalayerproperties, _t_datalayer>::remove_at (nPos);

	test ();

	return (nRslt);
}

template <class _t_sizetype, class _t_nodeiter, class _t_subnodeiter, class _t_datalayerproperties, class _t_datalayer>
void CBTreeArrayTest<_t_sizetype, _t_nodeiter, _t_subnodeiter, _t_datalayerproperties, _t_datalayer>::clear ()
{
	m_pClRef->clear ();

	CBTreeBase<CBTreeArrayPos <_t_sizetype>, arrayEntry_t, _t_sizetype, _t_nodeiter, _t_subnodeiter, _t_datalayerproperties, _t_datalayer>::clear ();

	test ();
}

template <class _t_sizetype, class _t_nodeiter, class _t_subnodeiter, class _t_datalayerproperties, class _t_datalayer>
bool CBTreeArrayTest<_t_sizetype, _t_nodeiter, _t_subnodeiter, _t_datalayerproperties, _t_datalayer>::set_at (const _t_sizetype nPos, const arrayEntry_t &rData)
{
	bool										bRslt;
	typename std::list<arrayEntry_t>::iterator	itRef = m_pClRef->begin ();
	_t_sizetype									nFForward;

	for (nFForward = 0; nFForward < nPos; nFForward++)
	{
		itRef++;
	}

	(*itRef) = rData;

	bRslt = CBTreeArray<arrayEntry_t, _t_sizetype, _t_nodeiter, _t_subnodeiter, _t_datalayerproperties, _t_datalayer>::set_at (nPos, rData);

	test ();

	return (bRslt);
}

template <class _t_sizetype, class _t_nodeiter, class _t_subnodeiter, class _t_datalayerproperties, class _t_datalayer>
bool CBTreeArrayTest<_t_sizetype, _t_nodeiter, _t_subnodeiter, _t_datalayerproperties, _t_datalayer>::get_at (const _t_sizetype nPos, arrayEntry_t &rData)
{
	return (CBTreeArray<arrayEntry_t, _t_sizetype, _t_nodeiter, _t_subnodeiter, _t_datalayerproperties, _t_datalayer>::get_at (nPos, rData));
}

template <class _t_sizetype, class _t_nodeiter, class _t_subnodeiter, class _t_datalayerproperties, class _t_datalayer>
bool CBTreeArrayTest<_t_sizetype, _t_nodeiter, _t_subnodeiter, _t_datalayerproperties, _t_datalayer>::operator== (CBTreeArrayTest &rArray)
{
	if (this == &rArray)
	{
		return (true);
	}

	if (this->size () != rArray.size ())
	{
		return (false);
	}

	_t_sizetype			i;

	for (i = 0; i < this->size (); i++)
	{
		arrayEntry_t	sThisData = this->at (i);
		arrayEntry_t	sArrayData = rArray.at (i);

		if (sThisData.nData != sArrayData.nData)
		{
			return (false);
		}
	}

	return (true);
}

template <class _t_sizetype, class _t_nodeiter, class _t_subnodeiter, class _t_datalayerproperties, class _t_datalayer>
bool CBTreeArrayTest<_t_sizetype, _t_nodeiter, _t_subnodeiter, _t_datalayerproperties, _t_datalayer>::operator!= (CBTreeArrayTest &rArray)
{
	return ( ! (*this == rArray));
}

template <class _t_sizetype, class _t_nodeiter, class _t_subnodeiter, class _t_datalayerproperties, class _t_datalayer>
void CBTreeArrayTest<_t_sizetype, _t_nodeiter, _t_subnodeiter, _t_datalayerproperties, _t_datalayer>::test ()
{
	typedef typename CBTreeArrayTest<_t_sizetype, _t_nodeiter, _t_subnodeiter, _t_datalayerproperties, _t_datalayer>::iterator	iter_t;

	if (!this->test_integrity ())
	{
		cerr << endl;
		cerr << "integrity test failed" << endl;

		cerr << "creating integrity.html..." << endl;

		this->show_integrity ("integrity.html");

		cerr << "finished!" << endl;

		exit (-1);
	}

	if (this->size () != (_t_sizetype) m_pClRef->size ())
	{
		cerr << endl;
		cerr << "size mismatches" << endl;
		cerr << "size: " << this->size () << endl;
		cerr << "reference: " << m_pClRef->size () << endl;

		cerr << "creating size.html..." << endl;

		this->show_integrity ("size.html");

		cerr << "finished!" << endl;

		exit (-1);
	}

	typename std::list<arrayEntry_t>::iterator		itRef = m_pClRef->begin ();
	_t_sizetype										it;
	arrayEntry_t									sData;
	arrayEntry_t									sRef;
	iter_t											sIter;

	sIter = this->begin ();

	for (it = 0; it < this->size (); it++, itRef++, sIter++)
	{
		sData = *sIter;

		sRef = *itRef;

		if (memcmp ((void *) &sData, (void *) &sRef, sizeof (arrayEntry_t)) != 0)
		{
			cerr << endl;
			cerr << "data mismatch at " << it << endl;
			
			cerr << "creating error.html..." << endl;

			this->show_integrity ("error.html");

			cerr << "finished!" << endl;

			exit (-1);
		}
	}
}

template <class _t_sizetype, class _t_nodeiter, class _t_subnodeiter, class _t_datalayerproperties, class _t_datalayer>
bool CBTreeArrayTest<_t_sizetype, _t_nodeiter, _t_subnodeiter, _t_datalayerproperties, _t_datalayer>::show_data (std::ofstream &ofs, std::stringstream &rstrData, std::stringstream &rszMsg, const _t_nodeiter nNode, const _t_subnodeiter nSubPos)
{
	uint32_t				rData;
	arrayEntry_t			*psData;
	
	try
	{
		psData = this->get_data (nNode, nSubPos);
		
		rData = psData->nData;
		rData = (rData >> 16) | (rData << 16);
		rData = ((rData >> 8) & 0xFF00FF) | ((rData << 8) & 0xFF00FF00);

		rstrData.clear ();
		rstrData << "data: " << hex << setfill ('0') << setw (8) << rData << dec << "<br>debug: " << psData->nDebug;
	}
	catch (exception *pE)
	{
		if (!ofs.is_open ())
		{
			return (false);
		}

		rszMsg << "<br>data: corruption (" << pE->what () << ")";

		rstrData.clear ();
		rstrData << "data: ---<br>debug: ---";
	}

	return (true);
}

//template <class _t_sizetype, class _t_nodeiter, class _t_subnodeiter, class _t_datalayerproperties, class _t_datalayer>
//bool CBTreeArrayTest<_t_sizetype, _t_nodeiter, _t_subnodeiter, _t_datalayerproperties, _t_datalayer>::show_node (std::ofstream &ofs, const _t_nodeiter nNode, const _t_subnodeiter nSubPos)
//{
//	std::stringstream		buf;
//	std::stringstream		aszMsg;
//
//	if (!this->show_data (ofs, buf, aszMsg, nNode, nSubPos))
//	{
//		if (!ofs.is_open ())
//		{
//			return (false);
//		}
//	}
//
//	if (ofs.is_open ())
//	{
//		ofs << buf.str ().c_str () << aszMsg.str ().c_str ();
//	}
//	
//	return (true);
//}
//
template <class _t_sizetype, class _t_nodeiter, class _t_subnodeiter, class _t_datalayerproperties, class _t_datalayer>
CBTreeArrayTest<_t_sizetype, _t_nodeiter, _t_subnodeiter, _t_datalayerproperties, _t_datalayer> &CBTreeArrayTest<_t_sizetype, _t_nodeiter, _t_subnodeiter, _t_datalayerproperties, _t_datalayer>::operator=
	(CBTreeArrayTest<_t_sizetype, _t_nodeiter, _t_subnodeiter, _t_datalayerproperties, _t_datalayer> &rBT)
{
	if (this != &rBT)
	{
		CBTreeArray<arrayEntry_t, _t_sizetype, _t_nodeiter, _t_subnodeiter, _t_datalayerproperties, _t_datalayer>			*pThisArray;
		CBTreeArray<arrayEntry_t, _t_sizetype, _t_nodeiter, _t_subnodeiter, _t_datalayerproperties, _t_datalayer>		*pRefArray;

		pThisArray = reinterpret_cast <CBTreeArray<arrayEntry_t, _t_sizetype, _t_nodeiter, _t_subnodeiter, _t_datalayerproperties, _t_datalayer> *> (this);
		pRefArray = reinterpret_cast <CBTreeArray<arrayEntry_t, _t_sizetype, _t_nodeiter, _t_subnodeiter, _t_datalayerproperties, _t_datalayer> *> (&rBT);

		*pThisArray = *pRefArray;

		test ();
	}

	return (*this);
}

#endif // BTREETESTARRAY_CPP
