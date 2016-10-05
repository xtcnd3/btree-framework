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

#include "testbench/application_classes/regression/btreetestarray.h"

template<class _t_datalayerproperties>
CBTreeTestArray<_t_datalayerproperties>::CBTreeTestArray 
	(
		_t_datalayerproperties &rDataLayerProperties, 
		const bayerTreeCacheDescription_t *psCacheDescription, 
		typename _t_datalayerproperties::sub_node_iter_type nNodeSize, 
		typename CBTreeTestArray<_t_datalayerproperties>::reference_t *pClRef
	)
	:	CBTreeArray<typename CBTreeTestArray<_t_datalayerproperties>::value_type, _t_datalayerproperties>
	(
		rDataLayerProperties, 
		psCacheDescription, 
		nNodeSize
	)
	,	m_pClRef (pClRef)
	,	m_bAtomicTesting (true)
{
}

template<class _t_datalayerproperties>
CBTreeTestArray<_t_datalayerproperties>::CBTreeTestArray (CBTreeTestArray<_t_datalayerproperties> &rBT, bool bAssign)
	:	CBTreeArray<typename CBTreeTestArray<_t_datalayerproperties>::value_type, _t_datalayerproperties>
	(
		rBT, false
	)
	,	m_pClRef (rBT.m_pClRef)
	,	m_bAtomicTesting (false)
{
	if (bAssign)
	{
		this->_assign (rBT);
	}

	this->set_atomic_testing (true);
}

template<class _t_datalayerproperties>
CBTreeTestArray<_t_datalayerproperties>::~CBTreeTestArray ()
{
}

template<class _t_datalayerproperties>
template<class _t_iterator>
void CBTreeTestArray<_t_datalayerproperties>::assign (_t_iterator sItFirst, _t_iterator sItLast)
{
	CBTreeArray_t::assign (sItFirst, sItLast);
}

template<class _t_datalayerproperties>
void CBTreeTestArray<_t_datalayerproperties>::assign (typename _t_datalayerproperties::size_type nNewSize, const typename CBTreeTestArray<_t_datalayerproperties>::value_type& rVal)
{
	CBTreeArray_t::assign (nNewSize, rVal);

	test ();
}

template<class _t_datalayerproperties>
void CBTreeTestArray<_t_datalayerproperties>::push_back (const typename CBTreeTestArray<_t_datalayerproperties>::value_type& rData)
{
	CBTreeArray_t::push_back (rData);

	test ();
}

template<class _t_datalayerproperties>
void CBTreeTestArray<_t_datalayerproperties>::pop_back ()
{
	CBTreeArray_t::pop_back ();

	test ();
}

template<class _t_datalayerproperties>
template<class _t_iterator>
typename CBTreeTestArray<_t_datalayerproperties>::iterator
	CBTreeTestArray<_t_datalayerproperties>::insert
	(
		typename CBTreeTestArray<_t_datalayerproperties>::const_iterator	sCIterPos, 
		_t_iterator															sItFirst, 
		_t_iterator															sItLast
	)
{
	iterator		sRetval = CBTreeArray_t::insert (sCIterPos, sItFirst, sItLast);

	return (sRetval);
}	

template<class _t_datalayerproperties>
typename CBTreeTestArray<_t_datalayerproperties>::iterator
	CBTreeTestArray<_t_datalayerproperties>::insert
	(
		typename CBTreeTestArray<_t_datalayerproperties>::const_iterator sCIterPos, 
		const typename CBTreeTestArray<_t_datalayerproperties>::value_type& rData
	)
{
	iterator		sRetval = CBTreeArray_t::insert (sCIterPos, rData);

	test ();

	return (sRetval);
}

template<class _t_datalayerproperties>
typename CBTreeTestArray<_t_datalayerproperties>::iterator
	CBTreeTestArray<_t_datalayerproperties>::insert
	(
		typename CBTreeTestArray<_t_datalayerproperties>::const_iterator sCIterPos, 
		const typename _t_datalayerproperties::size_type nLen, 
		const typename CBTreeTestArray<_t_datalayerproperties>::value_type& rData
	)
{
	iterator		sRetval = CBTreeArray_t::insert (sCIterPos, nLen, rData);

	test ();

	return (sRetval);
}

template<class _t_datalayerproperties>
typename CBTreeTestArray<_t_datalayerproperties>::iterator
	CBTreeTestArray<_t_datalayerproperties>::erase
	(
		typename CBTreeTestArray<_t_datalayerproperties>::const_iterator sCIterPos
	)
{
	iterator		sRetval = CBTreeArray_t::erase (sCIterPos);

	test ();

	return (sRetval);
}

template<class _t_datalayerproperties>
typename CBTreeTestArray<_t_datalayerproperties>::iterator
	CBTreeTestArray<_t_datalayerproperties>::erase
	(
		typename CBTreeTestArray<_t_datalayerproperties>::const_iterator sCIterFirst, 
		typename CBTreeTestArray<_t_datalayerproperties>::const_iterator sCIterLast
	)
{
	iterator		sRetval = CBTreeArray_t::erase (sCIterFirst, sCIterLast);

	test ();

	return (sRetval);
}

template<class _t_datalayerproperties>
void CBTreeTestArray<_t_datalayerproperties>::swap
	(
		CBTreeTestArray<_t_datalayerproperties> &rArray
	)
{
	CBTreeArray_t	&rBtrArray = dynamic_cast <CBTreeArray_t &> (rArray);

	CBTreeArray_t::swap (rBtrArray);

	test ();
}

template<class _t_datalayerproperties>
void CBTreeTestArray<_t_datalayerproperties>::clear ()
{
	CBTreeBaseDefaults_t::clear ();

	test ();
}

template<class _t_datalayerproperties>
bool CBTreeTestArray<_t_datalayerproperties>::operator== (const CBTreeTestArray &rArray) const
{
	if (this == &rArray)
	{
		return (true);
	}

	if (this->size () != rArray.size ())
	{
		return (false);
	}

	const_iterator		sCIterThis = this->cbegin ();
	const_iterator		sCIterArray = rArray.cbegin ();

	while (sCIterThis != this->cend ())
	{
		if ((*sCIterThis).nData != (*sCIterArray).nData)
		{
			return (false);
		}

		sCIterThis++;
		sCIterArray++;
	}

	return (true);
}

template<class _t_datalayerproperties>
bool CBTreeTestArray<_t_datalayerproperties>::operator!= (const CBTreeTestArray &rArray) const
{
	return ( ! (*this == rArray));
}

template<class _t_datalayerproperties>
void CBTreeTestArray<_t_datalayerproperties>::test () const
{
	typedef typename CBTreeTestArray<_t_datalayerproperties>::iterator	iter_t;

	if (!this->m_bAtomicTesting)
	{
		return;
	}

	if (!this->test_integrity ())
	{
		::std::cerr << ::std::endl;
		::std::cerr << "integrity test failed" << ::std::endl;

		::std::cerr << "creating integrity.html..." << ::std::endl;

		this->show_integrity ("integrity.html");

		::std::cerr << "finished!" << ::std::endl;

		exit (-1);
	}

	if (this->size () != (size_type) m_pClRef->size ())
	{
		::std::cerr << ::std::endl;
		::std::cerr << "size mismatches" << ::std::endl;
		::std::cerr << "size: " << this->size () << ::std::endl;
		::std::cerr << "reference: " << m_pClRef->size () << ::std::endl;

		::std::cerr << "creating size.html..." << ::std::endl;

		this->show_integrity ("size.html");

		::std::cerr << "finished!" << ::std::endl;

		exit (-1);
	}

	typename reference_t::iterator		itRef = m_pClRef->begin ();
	size_type							it;
	value_type							sData;
	value_type							sRef;
	iter_t								sIter;

	sIter = this->begin ();

	for (it = 0; it < this->size (); it++, itRef++, sIter++)
	{
		sData = *sIter;

		sRef = *itRef;

		if (memcmp ((void *) &sData, (void *) &sRef, sizeof (sData)) != 0)
		{
			::std::cerr << ::std::endl;
			::std::cerr << "data mismatch at " << it << ::std::endl;
			
			::std::cerr << "creating error.html..." << ::std::endl;

			this->show_integrity ("error.html");

			::std::cerr << "finished!" << ::std::endl;

			exit (-1);
		}
	}
}

template<class _t_datalayerproperties>
void CBTreeTestArray<_t_datalayerproperties>::set_reference (reference_t *pReference)
{
	this->m_pClRef = pReference;
}

template<class _t_datalayerproperties>
void CBTreeTestArray<_t_datalayerproperties>::set_atomic_testing (bool bEnable)
{
	this->m_bAtomicTesting = bEnable;
}

template<class _t_datalayerproperties>
CBTreeTestArray<_t_datalayerproperties> &CBTreeTestArray<_t_datalayerproperties>::operator=
	(const CBTreeTestArray<_t_datalayerproperties> &rBT)
{
	if (this != &rBT)
	{
		*(this->m_pClRef) = *(rBT.m_pClRef);

		CBTreeArray_t			&rThisArray = reinterpret_cast <CBTreeArray_t &> (*this);
		const CBTreeArray_t		&rBTArray = reinterpret_cast <const CBTreeArray_t &> (rBT);

		rThisArray = rBTArray;

		test ();
	}

	return (*this);
}

template<class _t_datalayerproperties>
bool CBTreeTestArray<_t_datalayerproperties>::show_data (std::ofstream &ofs, std::stringstream &rstrData, std::stringstream &rszMsg, const typename _t_datalayerproperties::node_iter_type nNode, const typename _t_datalayerproperties::sub_node_iter_type nSubPos) const
{
	uint32_t			nData;
	value_type			*psData;
	
	try
	{
		psData = this->get_data (nNode, nSubPos);
		
		nData = psData->nData;
		nData = (nData >> 16) | (nData << 16);
		nData = ((nData >> 8) & 0xFF00FF) | ((nData << 8) & 0xFF00FF00);

		if (ofs.is_open ())
		{
			rstrData.clear ();
			rstrData << "<table border=\"0\" cellspacing=\"1\" width=\"220\"><tr><td>";
			rstrData << "data: " << ::std::hex << ::std::setfill ('0') << ::std::setw (8) << nData << ::std::dec << "<br>debug: " << psData->nDebug;
			rstrData << "</td>";

			rstrData << "<td align=\"top\">";
		}

		size_type		nOffset;
		node_iter_type	nTmpNode = nNode;

		if (this->is_leaf (nNode))
		{
			nOffset = nSubPos;
		}
		else
		{
			size_type		*pnSerVector = this->get_serVector (nNode);

			nOffset = pnSerVector[nSubPos];
		}
		
		while (nTmpNode != this->m_nRootNode)
		{
			node_iter_type		nParentNode = this->get_parent (nTmpNode);
			sub_node_iter_type	nTmpSubPos = this->find_sub_node_offset (nParentNode, nTmpNode);

			if (nTmpSubPos > 0)
			{
				size_type		*pnSerVector = this->get_serVector (nParentNode);

				nOffset += pnSerVector[nTmpSubPos - 1] + 1;
			}

			nTmpNode = nParentNode;
		}

		if (nOffset < this->m_pClRef->size ())
		{
			typename reference_t::const_iterator	sCIter = this->m_pClRef->cbegin ();
			value_type								sData;

			::std::advance (sCIter, nOffset);

			sData = *sCIter;

			nData = sData.nData;
			nData = (nData >> 16) | (nData << 16);
			nData = ((nData >> 8) & 0xFF00FF) | ((nData << 8) & 0xFF00FF00);

			if (ofs.is_open ())
			{
				if (psData->nData != sData.nData)
				{
					rstrData << "<font color=\"#FF0000\">";
				}
				else
				{
					rstrData << "<font color=\"#00BB00\">";
				}

				rstrData << "data: " << ::std::hex << ::std::setfill ('0') << ::std::setw (8) << nData << ::std::dec << "<br>debug: " << sData.nDebug;
				rstrData << "</font>";
			}
		}
		else
		{
			if (ofs.is_open ())
			{
				rstrData << "<font color=\"#FF0000\">";
				rstrData << "reference<br>";
				rstrData << "out of<br>";
				rstrData << "range";
				rstrData << "</font>";
			}
		}

		if (ofs.is_open ())
		{
			rstrData << "</td></tr>";
			rstrData << "</table>" << ::std::endl;
		}
	}
	catch (::std::exception *pE)
	{
		if (!ofs.is_open ())
		{
			return (false);
		}
		else
		{
			rszMsg << "<br>data: corruption (" << pE->what () << ")";

			rstrData.clear ();
			rstrData << "data: ---<br>debug: ---";
		}
	}

	return (true);
}

template<class _t_datalayerproperties>
bool CBTreeTestArray<_t_datalayerproperties>::set_at (const typename _t_datalayerproperties::size_type nPos, const typename CBTreeTestArray<_t_datalayerproperties>::value_type &rData)
{
	bool								bRslt;
	typename reference_t::iterator		itRef = m_pClRef->begin ();
	size_type							nFForward;

	for (nFForward = 0; nFForward < nPos; nFForward++)
	{
		itRef++;
	}

	(*itRef) = rData;

	bRslt = CBTreeArray_t::set_at (nPos, rData);

	test ();

	return (bRslt);
}

template<class _t_datalayerproperties>
bool CBTreeTestArray<_t_datalayerproperties>::get_at (const typename _t_datalayerproperties::size_type nPos, typename CBTreeTestArray<_t_datalayerproperties>::value_type &rData) const
{
	return (CBTreeArray_t::get_at (nPos, rData));
}

#endif // BTREETESTARRAY_CPP
