/************************************************************
**
** file:	btreebasedefaults.cpp
** author:	Andreas Steffens
** license:	GPL v2
**
** description:
**
** This file contains code for the b-tree frameworks base class.
**
************************************************************/

#ifndef	BTREEBASEDEFAULTS_CPP
#define	BTREEBASEDEFAULTS_CPP

#include "btreebasedefaults.h"

template <class _ti_pos, class _t_data, class _t_sizetype, class _t_nodeiter, class _t_subnodeiter, class _t_datalayerproperties, class _t_datalayer>
CBTreeBaseDefaults<_ti_pos, _t_data, _t_sizetype, _t_nodeiter, _t_subnodeiter, _t_datalayerproperties, _t_datalayer>::CBTreeBaseDefaults (_t_datalayerproperties &rDataLayerProperties, const bayerTreeCacheDescription_t *psCacheDescription, _t_subnodeiter nNodeSize)
	:	CBTreeBaseIf<_ti_pos, _t_data, _t_sizetype, _t_nodeiter, _t_subnodeiter> (psCacheDescription, nNodeSize)
	,	m_pData (NULL)
{
	m_pClDataLayerProperties = new _t_datalayerproperties (rDataLayerProperties);

	BTREE_ASSERT (m_pClDataLayerProperties != NULL, "CBTreeBaseDefaults<>::CBTreeBaseDefaults (_t_datalayerproperties &, bayerTreeCacheDescription_t *, _t_subnodeiter): insufficient memory!");
}

/*

Copy Constructor

rBT		-	reference to CBTreeBaseDefaults instance to be copied

*/

template <class _ti_pos, class _t_data, class _t_sizetype, class _t_nodeiter, class _t_subnodeiter, class _t_datalayerproperties, class _t_datalayer>
CBTreeBaseDefaults<_ti_pos, _t_data, _t_sizetype, _t_nodeiter, _t_subnodeiter, _t_datalayerproperties, _t_datalayer>::CBTreeBaseDefaults (const CBTreeBaseDefaults<_ti_pos, _t_data, _t_sizetype, _t_nodeiter, _t_subnodeiter, _t_datalayerproperties, _t_datalayer> &rBT)
	:	CBTreeBaseIf<_ti_pos, _t_data, _t_sizetype, _t_nodeiter, _t_subnodeiter> (dynamic_cast <const CBTreeBaseIf<_ti_pos, _t_data, _t_sizetype, _t_nodeiter, _t_subnodeiter> &> (rBT))
	,	m_pData (NULL)
{
	m_pClDataLayerProperties = new _t_datalayerproperties (*(rBT.m_pClDataLayerProperties));

	BTREE_ASSERT (m_pClDataLayerProperties != NULL, "CBTreeBaseDefaults<>::CBTreeBaseDefaults (CBTreeBaseDefaults<> &): insufficient memory!");
}

/*

Destructor

*/

template <class _ti_pos, class _t_data, class _t_sizetype, class _t_nodeiter, class _t_subnodeiter, class _t_datalayerproperties, class _t_datalayer>
CBTreeBaseDefaults<_ti_pos, _t_data, _t_sizetype, _t_nodeiter, _t_subnodeiter, _t_datalayerproperties, _t_datalayer>::~CBTreeBaseDefaults ()
{
	clear ();

	if (m_pData != NULL)
	{
		delete m_pData;

		m_pData = NULL;
	}

	if (m_pClDataLayerProperties != NULL)
	{
		delete m_pClDataLayerProperties;

		m_pClDataLayerProperties = NULL;
	}
}

/*

is_emtpy - is b-tree empty

This method returns true if this b-tree instance contains no data, otherwise false.

*/

template <class _ti_pos, class _t_data, class _t_sizetype, class _t_nodeiter, class _t_subnodeiter, class _t_datalayerproperties, class _t_datalayer>
bool CBTreeBaseDefaults<_ti_pos, _t_data, _t_sizetype, _t_nodeiter, _t_subnodeiter, _t_datalayerproperties, _t_datalayer>::empty () const
{
	bool	bRetval = CBTreeBase_t::size () == 0ULL;

	return (bRetval);
}

/*

size - get size

This method returns the number of data entries stored by this b-tree instance.

*/

template <class _ti_pos, class _t_data, class _t_sizetype, class _t_nodeiter, class _t_subnodeiter, class _t_datalayerproperties, class _t_datalayer>
_t_sizetype CBTreeBaseDefaults<_ti_pos, _t_data, _t_sizetype, _t_nodeiter, _t_subnodeiter, _t_datalayerproperties, _t_datalayer>::size () const
{
	if (m_pData == NULL)
	{
		return (0ULL);
	}

	return (get_max_index (this->m_nRootNode));
}

/*

clear - remove all data

This method removes all data from the b-tree at once.

If successful the method returns true, otherwise false.

*/

template <class _ti_pos, class _t_data, class _t_sizetype, class _t_nodeiter, class _t_subnodeiter, class _t_datalayerproperties, class _t_datalayer>
void CBTreeBaseDefaults<_ti_pos, _t_data, _t_sizetype, _t_nodeiter, _t_subnodeiter, _t_datalayerproperties, _t_datalayer>::clear ()
{
	if (m_pData != NULL)
	{
		delete m_pData;

		m_pData = NULL;
	}

	this->m_nTreeSize = 0ULL;
	this->m_nNextAlloc = 0ULL;
}

/*

unload - unload all data to disc

This method stores all data in this instance associated temporary file and frees
all temporary buffers and data caches. This is useful if the resources on a system are
scarce and it is known that this instance is not needed access to for a long time.
It is not required to reload the data, since this is done by any call trying to access
any data. However, even a simple call to size will cause the data to be reloaded.
Hence, if you do not want this instance to be realoded, then do not call any of its
methods.
Also do not frequently call the unload method. It will result in significant
performance drops.

*/

template <class _ti_pos, class _t_data, class _t_sizetype, class _t_nodeiter, class _t_subnodeiter, class _t_datalayerproperties, class _t_datalayer>
void CBTreeBaseDefaults<_ti_pos, _t_data, _t_sizetype, _t_nodeiter, _t_subnodeiter, _t_datalayerproperties, _t_datalayer>::unload ()
{
	// unload data container
	if (m_pData != NULL)
	{
		m_pData->unload ();
	}

	// destroy rebuild FIFO
	if (this->m_pRebuildFIFO != NULL)
	{
		delete [] this->m_pRebuildFIFO;
	}

	this->m_pRebuildFIFO = NULL;

	this->m_nRebuildFIFOwritePos = 0ULL;
	this->m_nRebuildFIFOreadPos = 0ULL;
	this->m_nRebuildFIFOSize = 0ULL;

#if defined (_DEBUG)
	// destroy node trace buffer
	if (this->m_pNodeTrace != NULL)
	{
		delete [] this->m_pNodeTrace;
	}

	this->m_pNodeTrace = NULL;

	this->m_nNodeTraceDepthMax = 1;
#endif
}

template <class _ti_pos, class _t_data, class _t_sizetype, class _t_nodeiter, class _t_subnodeiter, class _t_datalayerproperties, class _t_datalayer>
bool CBTreeBaseDefaults<_ti_pos, _t_data, _t_sizetype, _t_nodeiter, _t_subnodeiter, _t_datalayerproperties, _t_datalayer>::test_integrity () const
{
	if (m_pData == NULL)
	{
		return (true);
	}

	char				*pAlloc = new char[(size_t) this->m_nTreeSize];
	bool				retval = false;
	std::ofstream		ofs;
	
	if (pAlloc != NULL)
	{
		memset ((void *) pAlloc, 0, sizeof (*pAlloc) * (size_t) this->m_nTreeSize);

		try
		{
			retval = show_tree (ofs, this->m_nRootNode, this->m_nRootNode, pAlloc);
		}
		catch (exception *pE)
		{
			cerr << endl << "CRITICAL ERROR!" << endl;
			cerr << "exception has not been caught during integrity test" << endl;
			cerr << "message: " << pE->what () << endl;

			retval = false;
		}

		delete [] pAlloc;
	}
	else
	{
		cerr << "ERROR: CBTreeBaseDefaults<_ti_pos, _t_data, _t_sizetype, _t_nodeiter, _t_subnodeiter, _t_datalayerproperties, _t_datalayer>::test_integrity: insufficient memory!" << endl;

		retval = false;
	}

	return (retval);
}

template <class _ti_pos, class _t_data, class _t_sizetype, class _t_nodeiter, class _t_subnodeiter, class _t_datalayerproperties, class _t_datalayer>
void CBTreeBaseDefaults<_ti_pos, _t_data, _t_sizetype, _t_nodeiter, _t_subnodeiter, _t_datalayerproperties, _t_datalayer>::show_integrity (const char *pFilename) const
{
	char				*pAlloc = new char[(size_t) this->m_nTreeSize];
	std::ofstream		ofs ((const char *) pFilename, std::ofstream::out | std::ofstream::trunc);

	BTREE_ASSERT (pAlloc != NULL, "CBTreeBaseDefaults<_ti_pos, _t_data, _t_sizetype, _t_nodeiter, _t_subnodeiter, _t_datalayerproperties, _t_datalayer>::show_integrity: insufficient memory to generate integrty page");

#if defined (WIN32)

	DWORD	nErrCode = GetLastError ();

	BTREE_ASSERT ((nErrCode == ERROR_SUCCESS) || (nErrCode == ERROR_ALREADY_EXISTS), "CBTreeBaseDefaults<_ti_pos, _t_data, _t_sizetype, _t_nodeiter, _t_subnodeiter, _t_datalayerproperties, _t_datalayer>::show_integrity: unexpected error code");

	// under windows the std::ofstream constructor flags that a file already exists by setting the error code
	// returned by GetLastError (), to ERROR_ALREADY_EXISTS (0xb7), which is not an error per se.
	// trouble is, if CloseHandle () is used on a file mapping handle, then not only is the error code not reset, but
	// also the CloseHandle () has the tendency to return with a false error as its return value, although the operation
	// was successful. If CloseHandle flags a false error, then the only way to find out if the operation was successful
	// is to use GetLastError (), which would still return with ERROR_ALREADY_EXISTS. Therefore the error code is reset here.
	SetLastError (ERROR_SUCCESS);

#endif

	memset ((void *) pAlloc, 0, sizeof (*pAlloc) * (size_t) this->m_nTreeSize);

	ofs << "<!DOCTYPE HTML PUBLIC \"-//W3C//DTD HTML 4.01 Transitional//EN\" \"http://www.w3.org/TR/html4/loose.dtd\">" << endl;
	ofs << "<HTML>" << endl;
	ofs << "<head>" << endl;
	ofs << "<title>bayer tree integrity</title>" << endl;
	ofs << "<h1>bayer tree integrity</h1>" << endl;
	ofs << "</head>" << endl;
	ofs << "<body>" << endl;

	if (m_pData != NULL)
	{
		m_pData->set_cacheFreeze (true);
		{
			try
			{
				show_tree (ofs, this->m_nRootNode, this->m_nRootNode, pAlloc);
			}
			catch (exception *pE)
			{
				cerr << endl << "CRITICAL ERROR!" << endl;
				cerr << "exception has not been caught during integrity display" << endl;
				cerr << "message: " << pE->what () << endl;
			}

			try
			{
				shownodelist (ofs, pAlloc);
			}
			catch (exception *pE)
			{
				cerr << endl << "CRITICAL ERROR!" << endl;
				cerr << "exception has not been caught during node list display" << endl;
				cerr << "message: " << pE->what () << endl;
			}
		}
		m_pData->set_cacheFreeze (false);

		try
		{
			m_pData->showdump (ofs, this->m_nTreeSize, pAlloc);
		}
		catch (exception *pE)
		{
			cerr << endl << "CRITICAL ERROR!" << endl;
			cerr << "exception has not been caught during data dump display" << endl;
			cerr << "message: " << pE->what () << endl;
		}
	}
	else
	{
		ofs << "data layer not initialised!" << endl;
	}

	ofs << "</body>" << endl;
	ofs << "</HTML>" << endl;

	ofs.close ();

	delete [] pAlloc;
}

#if defined (BTREEFRAMEWORK_TEST_BENCH_ENABLED)

template <class _ti_pos, class _t_data, class _t_sizetype, class _t_nodeiter, class _t_subnodeiter, class _t_datalayerproperties, class _t_datalayer>
void CBTreeBaseDefaults<_ti_pos, _t_data, _t_sizetype, _t_nodeiter, _t_subnodeiter, _t_datalayerproperties, _t_datalayer>::test () const
{

}

template <class _ti_pos, class _t_data, class _t_sizetype, class _t_nodeiter, class _t_subnodeiter, class _t_datalayerproperties, class _t_datalayer>
void CBTreeBaseDefaults<_ti_pos, _t_data, _t_sizetype, _t_nodeiter, _t_subnodeiter, _t_datalayerproperties, _t_datalayer>::set_atomic_testing (bool bEnable)
{
	bEnable;
}

#endif

/*

operator= - assignment operator overload

rBT		- is a reference to a b-tree that will have its content be assigned to this instance

The result is a reference to this instance.

*/

template <class _ti_pos, class _t_data, class _t_sizetype, class _t_nodeiter, class _t_subnodeiter, class _t_datalayerproperties, class _t_datalayer>
CBTreeBaseDefaults<_ti_pos, _t_data, _t_sizetype, _t_nodeiter, _t_subnodeiter, _t_datalayerproperties, _t_datalayer> &CBTreeBaseDefaults<_ti_pos, _t_data, _t_sizetype, _t_nodeiter, _t_subnodeiter, _t_datalayerproperties, _t_datalayer>::operator=
	(const CBTreeBaseDefaults<_ti_pos, _t_data, _t_sizetype, _t_nodeiter, _t_subnodeiter, _t_datalayerproperties, _t_datalayer> &rBT)
{
	// if this is not the same instance as the referenced b-tree (rBT) ...
	if (this != &rBT)
	{
		CBTreeBaseIf_t			&rLowLevelThis = dynamic_cast<CBTreeBaseIf_t &> (*this);
		const CBTreeBaseIf_t	&rLowLevelBT = dynamic_cast<const CBTreeBaseIf_t &> (rBT);
		
		rLowLevelThis = rLowLevelBT;
	}

	return (*this);
}

/*

create_root

- creates the root node on creation of the tree or if split_node needs to generate a new root

*/

template <class _ti_pos, class _t_data, class _t_sizetype, class _t_nodeiter, class _t_subnodeiter, class _t_datalayerproperties, class _t_datalayer>
void CBTreeBaseDefaults<_ti_pos, _t_data, _t_sizetype, _t_nodeiter, _t_subnodeiter, _t_datalayerproperties, _t_datalayer>::create_root ()
{
	// if the tree structure has not been generated ...
	if (m_pData == NULL)
	{
		// ... then create the data layer
		
		m_pData = new _t_datalayer
								(
									*m_pClDataLayerProperties,
									this->m_sCacheDescription.nMinNumberOfBytesPerSuperBlock, 
									sizeof (node_t), 
									this->m_nNumPools, 
									this->m_pPoolDesc
								);

		// test if bring up was successful
		BTREE_ASSERT (m_pData != NULL, "CBTreeBaseDefaults<_ti_pos, _t_data, _t_sizetype, _t_nodeiter, _t_subnodeiter, _t_datalayerproperties, _t_datalayer>::create_root: data object not created");

		// allocate root node
		this->m_nRootNode = create_node ((_t_nodeiter) ~0x0);

		m_pData->set_root_node (this->m_nRootNode);
	}
}

/*

create_node

- allocates a new node and returns that nodes identifier number
- it also updates the node allocation table 

*/

template <class _ti_pos, class _t_data, class _t_sizetype, class _t_nodeiter, class _t_subnodeiter, class _t_datalayerproperties, class _t_datalayer>
_t_nodeiter CBTreeBaseDefaults<_ti_pos, _t_data, _t_sizetype, _t_nodeiter, _t_subnodeiter, _t_datalayerproperties, _t_datalayer>::create_node (_t_nodeiter nParent)
{
	node_t					*psNodeDesc;
	_t_nodeiter				ui64, retval;
	_t_nodeiter				uj64;

	// if tree structure has not been generated ...
	if (m_pData == NULL)
	{
		// ... then create root node
		create_root ();

		return (this->m_nRootNode);
	}

	do
	{
		// if the next node to be allocated exceeds the current tree size ...
		if (this->m_nNextAlloc >= this->m_nTreeSize)
		{
			// ... then extend available number of nodes
			this->m_nTreeSize += (_t_nodeiter) this->m_nGrowBy;

			// determine and set new data layer size size
			m_pData->set_size (this->m_nTreeSize);

			// unset allocation vector in newly created space
			for (uj64 = this->m_nTreeSize - (_t_nodeiter) this->m_nGrowBy; uj64 < this->m_nTreeSize; uj64++)
			{
				reset_reservation (uj64);
			}
		}

		// find the next available free node
		for (ui64 = this->m_nNextAlloc; ui64 < this->m_nTreeSize; ui64++)
		{
			// if node is available ...
			if (!get_reservation (ui64))
			{
				// ... then allocate
				set_reservation (ui64);

				// if parent node has not been determined (ie. root node is being created) ...
				if (nParent == (_t_nodeiter) ~0x0)
				{
					// update parent parameter to new root
					nParent = ui64;
				}

				// update start node for next allocation
				this->m_nNextAlloc = ui64 + 1;

				// set return value
				retval = ui64;

				break;
			}
		}

		// if it wasn't poosible to find a free node
		if (ui64 >= this->m_nTreeSize)
		{
			// ... then make m_nNextAlloc exceed tree size and try again
			this->m_nNextAlloc = this->m_nTreeSize;
		}
	} while (ui64 >= this->m_nTreeSize);

	psNodeDesc = get_node (retval);

	// set up new node description
	psNodeDesc->nMaxIdx = 0;
	psNodeDesc->nNumData = 0;
	psNodeDesc->nParent = nParent;

	return (retval);
}

/*

add_to_node

- recursively traces into the tree and adds one entry in a leave
- what will be added is determined the sort mode that has been set
- the method also splits nodes and traces back if need be
- if nodes had been split they also be rebuild

*/

template <class _ti_pos, class _t_data, class _t_sizetype, class _t_nodeiter, class _t_subnodeiter, class _t_datalayerproperties, class _t_datalayer>
_t_sizetype CBTreeBaseDefaults<_ti_pos, _t_data, _t_sizetype, _t_nodeiter, _t_subnodeiter, _t_datalayerproperties, _t_datalayer>::add_to_node (_ti_pos sPos, const _t_data &rData, _t_nodeiter nNode, uint32_t nDepth, _t_nodeiter &rnRsltNode, _t_subnodeiter &rnRsltSub, _t_sizetype *pnPos)
{
	_t_sizetype				retval = 0ULL;
	_t_nodeiter				*pnSubNode;
	_t_subnodeiter			nSubPos;
	_t_sizetype				*pnSerVector;

	// store node identifier on rebuild stack
	this->attache_traced_node (nNode);

	// identify next sub node's position
	nSubPos = this->find_next_sub_pos (nNode, sPos);

	// if node is not a leave ...
	if (!is_leaf (nNode))
	{
		// ... then search for next node to trace into

		if (pnPos != NULL)
		{
			if (nSubPos > 0)
			{
				pnSerVector = this->get_serVector (nNode);

				(*pnPos) += pnSerVector[nSubPos - 1] + 1;
			}
		}

		// obtain next sub node's id number
		pnSubNode = get_sub_node (nNode, nSubPos);

		// trace into next sub node
		retval += add_to_node (sPos, rData, *pnSubNode, nDepth + 1, rnRsltNode, rnRsltSub, pnPos);
		// sNodeDesc is not valid after this point, since it is a reference
		// and the content it was referring to is cached and may have
		// been modified during the last call also
		// add_to_node has potential to re-allocate locations and thus to modify base pointers
		// all pointers to data layer need to be re-initialised
		
		// if rebuild FIFO ran out of entries ...
		if (this->rebuild_FIFO_fillstate () == 0ULL)
		{
			// ... then quick auto-update this node by increasing amount of data contained in this tree branch by one
			this->rebuild_node (nNode, 1, nSubPos);
		}
	}
	else
	{
		// ... otherwise add data to leave
		insert_data_into_node (nNode, nSubPos, rData);

		// insert_data_into_node has potential to re-allocate locations and thus to modify base pointers
		// all pointers to data layer need to be re-initialised

		rnRsltNode = nNode;
		rnRsltSub = nSubPos;

		retval++;

		auto_shrink_dec ();

		if (pnPos != NULL)
		{
			(*pnPos) += (_t_sizetype) nSubPos;
		}
	}

	// if rebuild FIFO is not empty ...
	if (this->rebuild_FIFO_fillstate () > 1ULL)
	{
		// ... then run a full update on both this node and the newly created node on the same level
		this->rebuild_node (this->rebuild_FIFO_get ());
		this->rebuild_node (this->rebuild_FIFO_get ());
	}

	// if a new root node has been created ...
	if ((nDepth == 0) && (nNode != this->m_nRootNode))
	{
#if defined (_DEBUG)
		BTREE_ASSERT (this->rebuild_FIFO_fillstate () == 1ULL, "CBTreeBaseDefaults<_t_data, _t_datalayerproperties, _t_datalayer, _t_sizetype, _t_nodeiter, _t_subnodeiter>::add_to_node: FIFO corruption");
#endif

		// ... then update new root node
		this->rebuild_node (this->rebuild_FIFO_get ());
	}

#if defined (_DEBUG)
	
	if (nDepth == 0)
	{
		BTREE_ASSERT (this->rebuild_FIFO_fillstate () == 0UL, "CBTreeBaseDefaults<_t_data, _t_datalayerproperties, _t_datalayer, _t_sizetype, _t_nodeiter, _t_subnodeiter>::add_to_node: FIFO corruption");
	}

	BTREE_ASSERT (get_data_count (nNode) <= this->get_node_max_data_size (), "CBTreeBaseDefaults<_t_data, _t_datalayerproperties, _t_datalayer, _t_sizetype, _t_nodeiter, _t_subnodeiter>::add_to_node: node size exceeded 2 * t - 1 --> tree invalid");

#endif

	this->detache_traced_node ();

	if (nDepth == 0)
	{
		m_pData->terminate_access ();
	}

	return (retval);
}

/*

split_node

- tests if a new root node must be created
	+ if yes creates a new root node
- creates a new node on the same level
- copies all relevant information from upper half of node to be split to bottom of new node
- inserts middle data set into parent node, which can be the new root node

*/

template <class _ti_pos, class _t_data, class _t_sizetype, class _t_nodeiter, class _t_subnodeiter, class _t_datalayerproperties, class _t_datalayer>
_t_nodeiter CBTreeBaseDefaults<_ti_pos, _t_data, _t_sizetype, _t_nodeiter, _t_subnodeiter, _t_datalayerproperties, _t_datalayer>::split_node (_t_nodeiter nNode)
{
	_t_nodeiter					nRetval, nParent;
	_t_data						sData;
	bool						bNewRoot;
	_t_subnodeiter				nSubPos;
	node_t						*psNodeDesc;
	uint32_t					i;

	psNodeDesc = get_node (nNode);

#if defined (_DEBUG)
	BTREE_ASSERT (get_data_count (*psNodeDesc) == this->get_node_max_data_size (), "CBTreeBaseDefaults<_ti_pos, _t_data, _t_sizetype, _t_nodeiter, _t_subnodeiter, _t_datalayerproperties, _t_datalayer>::split_node: split node called with inconsistent node");
#endif

	bNewRoot = psNodeDesc->nParent == nNode;

	// if the root node has to be split ...
	if (bNewRoot)
	{
		// ... then prepare a new root node

		// allocate new node
		nParent = create_node ((_t_nodeiter) ~0);

		// create_node has potential to re-allocate locations and thus to modify base pointers
		// all pointers to data layer need to be re-initialised
		psNodeDesc = get_node (nNode);

		// update root node cache

		// read old root node from root cache
		node_t *psTempNode;
		
		// set new root
		this->m_nRootNode = nParent;

		// read new root from normal cache
		psTempNode = get_node (this->m_nRootNode);

		// set new root description data
		psTempNode->nParent = this->m_nRootNode;
		psTempNode->nMaxIdx = psNodeDesc->nMaxIdx;

		// set new childs parent to root
		psNodeDesc->nParent = this->m_nRootNode;

		m_pData->set_root_node (this->m_nRootNode);
	}
	else
	{
		// otherwise create a new node on the same level as the existing node to be split
		nParent = psNodeDesc->nParent;
	}

	// allocate space for the new node
	nRetval = create_node (psNodeDesc->nParent);

	// create_node has potential to re-allocate locations and thus to modify base pointers
	// all pointers to data layer need to be re-initialised
	psNodeDesc = get_node (nNode);

	// append node to be split and new node to rebuild FIFO
	this->rebuild_FIFO_put (nRetval);
	this->rebuild_FIFO_put (nNode);

	// if a new root node is to be added ...
	if (bNewRoot)
	{
		// ... then append the new root to the rebuild FIFO
		this->rebuild_FIFO_put (this->m_nRootNode);
	}

	{
		node_t		*psTempNewNode;
		_t_nodeiter	*pSubNodeBase, *pNewSubNodeBase;

		// load newly created node
		psTempNewNode = get_node (nRetval);

		if (!is_leaf (nNode))
		{
			// 
			pNewSubNodeBase = get_sub_node_buffer (nRetval);

			// load sub node vector of node to be split
			pSubNodeBase = get_sub_node_buffer (nNode);
					
			// copy sub node vector upper half from node to be split to bottom of new node
			memcpy ((void *) pNewSubNodeBase, (void *) &(pSubNodeBase[this->m_nNodeSize]), sizeof (*pSubNodeBase) * (size_t)this->m_nNodeSize);
		}

		_t_data		*pNodeData;
		_t_data		*pNewNodeData;

		// data from node to be split
		pNodeData = get_data_buffer (nNode);

		pNewNodeData = get_data_buffer (nRetval);
		
		// copy data of upper half from node to be split to bottom of new node
		memcpy ((void *) pNewNodeData, (void *) &(pNodeData[this->m_nNodeSize]), sizeof (*pNodeData) * (size_t)(this->m_nNodeSize - 1ULL));

		// copy data at middle position to object to be moved to parent node
		sData = pNodeData[this->m_nNodeSize - 1ULL];

		// if split happened on leaf node level ...
		if (is_leaf (nNode))
		{
			// ... then only update the data count of the node that was split as well as the new node
			psNodeDesc->nNumData = (0 - (this->m_nNodeSize - 1));
			psTempNewNode->nNumData = (0 - (this->m_nNodeSize - 1));
		}
		else
		{
			// ... otherwise update the data count of the node that was split as well as the new node
			psNodeDesc->nNumData = this->m_nNodeSize - 1;
			psTempNewNode->nNumData = this->m_nNodeSize - 1;

			// and update all sub nodes parent information which are addressed by the new node
			for (i = 0; i < this->m_nNodeSize; i++)
			{
				node_t		*psNodeDesc;
				
				psNodeDesc = get_node (pNewSubNodeBase[i]);

				psNodeDesc->nParent = nRetval;
			}
		}
	}

	// if a new root node is to be created ...
	if (bNewRoot)
	{
		// ... then the first data set written is always at the initial position
		nSubPos = 0;

		_t_nodeiter		*psSubNodes = get_sub_node (nParent, 0);

		psSubNodes[0] = nNode;
	}
	else
	{
		// determine the splitted node's position in its parent node
		nSubPos = find_sub_node_offset (nParent, nNode);

#if defined (_DEBUG)
		BTREE_ASSERT (nSubPos >= 0, "CBTreeBaseDefaults<_ti_pos, _t_data, _t_sizetype, _t_nodeiter, _t_subnodeiter, _t_datalayerproperties, _t_datalayer>::split_node: subPos not found in parent node");
#endif
	}

	// insert former middle positioned data into parent node
	insert_data_into_node (nParent, nSubPos, sData, nRetval, 1);

	// return newly created node identifier
	return (nRetval);
}

/*

remove_from_node

- 

*/

template <class _ti_pos, class _t_data, class _t_sizetype, class _t_nodeiter, class _t_subnodeiter, class _t_datalayerproperties, class _t_datalayer>
_t_sizetype CBTreeBaseDefaults<_ti_pos, _t_data, _t_sizetype, _t_nodeiter, _t_subnodeiter, _t_datalayerproperties, _t_datalayer>::remove_from_node (_ti_pos sPos, _t_nodeiter nNode, uint32_t nDepth)
{
	_t_sizetype				nRetval = 0;
	_t_subnodeiter			nSub;
	_t_subnodeiter			nSubData;
	_ti_pos					sNewPos;
	bool					bFound;
	
	// store node identifier on rebuild stack
	this->attache_traced_node (nNode);

	// determine position or sub node position of data set in question and copy position to variable "nSub"
	sNewPos = this->determine_position (sPos, nNode, nSub, nSubData, bFound);

	// if node is not a leaf ...
	if (!is_leaf (nNode))
	{
		// ... then determine what to do in order to remove the data set in question
		_t_sizetype			nLeftSize, nRightSize;
		_t_nodeiter			*pnLeftNode, *pnRightNode;
		_t_nodeiter			*pnNextNode;
		_t_subnodeiter		nNewSub;
		bool				bMergeNodes, bRotateNodes;
	
		do
		{
			bRotateNodes = false;
			bMergeNodes = false;
			
			pnLeftNode = get_sub_node (nNode, nSubData);
			pnRightNode = get_sub_node (nNode, nSubData + 1);

			nLeftSize = get_data_count (*pnLeftNode);
			nRightSize = get_data_count (*pnRightNode);

			// if left and right node have size t - 1 ...
			if ((nLeftSize == (this->m_nNodeSize - 1ULL)) && (nRightSize == (this->m_nNodeSize - 1ULL)))
			{
				// ... then mark them for a merge
				bMergeNodes = true;
			}
			// if one of the nodes has size t - 1 and the other is larger ...
			else if (((nLeftSize == (this->m_nNodeSize - 1ULL)) && (nRightSize >= this->m_nNodeSize)) ||
					((nRightSize == (this->m_nNodeSize - 1ULL)) && (nLeftSize >= this->m_nNodeSize)))
			{
				// ... then mark the for a rotate
				bRotateNodes = true;

				// - the following code covers a corner case where a rotate is potentially counter productive
				// - that is the case if data is rotated away from a leaf node in which the actual remove will 
				// happen in and would leave the tree in an undefined state
				
				// determine if data in question is in left node
				bool	bDataIsInLeftNode = ((*pnLeftNode) == (*get_sub_node (nNode, nSub)));

				// if data is in left node ...
				if (bDataIsInLeftNode)
				{
					// ... and left the left node larger than t - 1 ...
					if (nLeftSize >= this->m_nNodeSize)
					{
						// ... then a rotate is counter productive
						bRotateNodes = false;
					}
				}
				else
				{
					// if data is in right node and right node is larger ...
					if (nRightSize >= this->m_nNodeSize)
					{
						// ... then a rotate is also counter productive
						bRotateNodes = false;
					}
				}
			}

			// perform any previously determined operation
			if (bMergeNodes)
			{
				merge_node (nNode, nSubData);
			}
			else if (bRotateNodes)
			{
				rotate (nNode, nSubData, nNewSub, nLeftSize > nRightSize);
			}
			
			// if any operation took place ...
			if (bMergeNodes || bRotateNodes)
			{
				// ... then re-determine position
				sNewPos = this->determine_position (sPos, nNode, nSub, nSubData, bFound);
			}
		// it has to be checked if node (nNode) has become a leaf node, since merge_node can do that
		} while ((bMergeNodes || bRotateNodes) && !is_leaf (nNode));

		// if data in question has been found ...
		if (bFound)
		{
			// ... then remove data

			// if data is in a leaf node ...
			if (is_leaf (nNode))
			{
				// ... then remove from leaf
				nRetval = 1;

				remove_data_from_leaf (nNode, nSubData);
			}
			else
			{
				// ... otherwise take a copy from the nearest adjacent position
				// and remove the source of the copy

				nLeftSize = get_max_index (*pnLeftNode);
				nRightSize = get_max_index (*pnRightNode);

				// replace data
				replace_with_adjacent_entry (nNode, nSub, nLeftSize > nRightSize);

				// if left sub-tree is larger then right sub-tree ...
				if (nLeftSize > nRightSize)
				{
					// ... then determine nearest position on the left
					sNewPos = this->generate_prev_position (nNode, nSubData, sPos);
				}
				else
				{
					// ... otherwise determine nearest position on the right
					sNewPos = this->generate_next_position (nNode, nSubData, sPos);
				}

				// remove data from newly determined position, which is the source location of the copy from before
				nRetval = remove_from_node (sNewPos, nNode, nDepth + 1);

				this->rebuild_node (nNode);
			}
		}
		else
		{
			// ... otherwise determine sub-tree
			pnNextNode = get_sub_node (nNode, nSub);

			// and remove data from sub-tree
			nRetval = remove_from_node (sNewPos, *pnNextNode, nDepth + 1);

			this->rebuild_node (nNode);
		}
	}
	else
	{
		// if data is on a leaf node, then remove it from leaf
		nRetval = 1;

		remove_data_from_leaf (nNode, nSubData);
	}

	// remove node identifier from rebuild stack
	this->detache_traced_node ();

	if (nDepth == 0)
	{
		m_pData->terminate_access ();
	}

	return (nRetval);
}

template <class _ti_pos, class _t_data, class _t_sizetype, class _t_nodeiter, class _t_subnodeiter, class _t_datalayerproperties, class _t_datalayer>
_t_nodeiter CBTreeBaseDefaults<_ti_pos, _t_data, _t_sizetype, _t_nodeiter, _t_subnodeiter, _t_datalayerproperties, _t_datalayer>::merge_node (_t_nodeiter nNode, _t_subnodeiter nSub)
{
	node_t		*psNodeDesc;
	
	psNodeDesc = get_node (nNode);

#if defined (_DEBUG)
	BTREE_ASSERT (!is_leaf (*psNodeDesc), "CBTreeBaseDefaults<_ti_pos, _t_data, _t_sizetype, _t_nodeiter, _t_subnodeiter, _t_datalayerproperties, _t_datalayer>::merge_node: cannot merge from leaf node");
	BTREE_ASSERT (nSub < get_data_count (*psNodeDesc), "CBTreeBaseDefaults<_ti_pos, _t_data, _t_sizetype, _t_nodeiter, _t_subnodeiter, _t_datalayerproperties, _t_datalayer>::merge_node: cannot merge sub nodes beyond parents data size");
#endif

	_t_nodeiter				nRetval;
	_t_nodeiter				nLeftNode, nRightNode;
	_t_subnodeiter			nLeftSize, nRightSize;
	_t_data					*psData;

	nLeftNode = *(get_sub_node (nNode, nSub));
	nRightNode = *(get_sub_node (nNode, nSub + 1));

	nLeftSize = get_data_count (nLeftNode);
	nRightSize = get_data_count (nRightNode);

	// if node (node must be root) is not a leaf node and has only one entry left ...
	if (get_data_count (*psNodeDesc) == 1)
	{
		// ... then (special case applies) merge the two remaining sub-nodes and this node
		_t_data		*pLeftBuffer;
		_t_data		*pMergeBuffer;
		_t_data		*pRightBuffer;

#if defined (_DEBUG)
		BTREE_ASSERT (nNode == this->m_nRootNode, "CBTreeBaseDefaults<_ti_pos, _t_data, _t_sizetype, _t_nodeiter, _t_subnodeiter, _t_datalayerproperties, _t_datalayer>::merge_node: special case merge applies for root node only");
#endif

		// the merge result will be root
		nRetval = this->m_nRootNode;

		pLeftBuffer = get_data_buffer (nLeftNode);
		pMergeBuffer = get_data_buffer (nNode);
		pRightBuffer = get_data_buffer (nRightNode);

		// move data from initial entry to 'leftSize'-th entry in merge node
		memcpy ((void *) &(pMergeBuffer[nLeftSize]), (void *) pMergeBuffer, sizeof (*pMergeBuffer));

		// copy all left node data to the base of the merge node
		memcpy ((void *) pMergeBuffer, (void *) pLeftBuffer, sizeof (*pLeftBuffer) * nLeftSize);

		// copy all right node data to the top of the merge node
		memcpy ((void *) &(pMergeBuffer[nLeftSize + 1]), (void *) pRightBuffer, sizeof (*pRightBuffer) * nRightSize);

		// if the nodes being merged from were not leaf nodes ...
		if (!is_leaf (nLeftNode))
		{
			// ... then transfer sub node data as well
			_t_nodeiter			*pLeftSub;
			_t_nodeiter			*pMergeSub;
			_t_nodeiter			*pRightSub;
			_t_subnodeiter		ui32;

			// get sub node data left, merge and right node
			pLeftSub = get_sub_node_buffer (nLeftNode);
			pMergeSub = get_sub_node_buffer (nNode);
			pRightSub = get_sub_node_buffer (nRightNode);

			// copy all left node sub node data to bottom half of merge node
			memcpy ((void *) pMergeSub, (void *) pLeftSub, sizeof (_t_nodeiter) * (nLeftSize + 1));

			// copy all right node sub node data to top half of merge node
			memcpy ((void *) &(pMergeSub[nLeftSize + 1]), (void *) pRightSub, sizeof (_t_nodeiter) * (nRightSize + 1));

			// determine new size of merged node
			psNodeDesc->nNumData = nLeftSize + nRightSize + 1;

			// every node that was previously refering to the left or right node as their parent, will now refer to the merge node
			for (ui32 = 0UL; ui32 < (nLeftSize + nRightSize + 2); ui32++)
			{
				// load node descriptor
				node_t	*psNodeDesc;
				
				psNodeDesc = get_node (pMergeSub[ui32]);

				// update node descriptor
				psNodeDesc->nParent = nNode;
			}

			// update merge node
			this->rebuild_node (nNode);
		}
		else // if the nodes being merged from were leaf nodes ...
		{
			// ... then determine new size of merge node 
			psNodeDesc->nNumData = 0 - (_t_subnodeiter) psNodeDesc->nMaxIdx;
		}

		// remove left and right node from reservation list
		unset_reservation (nLeftNode);
		unset_reservation (nRightNode);

		// if the left node was smaller than the next node to allocate ...
		if (nLeftNode < this->m_nNextAlloc)
		{
			// ... then the former left node is the next to allocate from
			this->m_nNextAlloc = nLeftNode;
		}

		// if the right node was smaller than the next node to allocate ...
		if (nRightNode < this->m_nNextAlloc)
		{
			// ... then the former right node is the next to allocate from
			this->m_nNextAlloc = nRightNode;
		}

		return (nRetval);
	}

	// if node is a leaf node or has more than one entry then this code path is executed

	// mark left node as return value
	nRetval = nLeftNode;

	// create a copy of the entry to be removed during the merge
	psData = get_data (nNode, nSub);
	
	nLeftSize = get_data_count (nLeftNode);
	nRightSize = get_data_count (nRightNode);

	_t_data		*pBaseLeft;
	_t_data		*pBaseRight;

	// load left and right node's data
	pBaseLeft = get_data_buffer (nLeftNode);
	pBaseRight = get_data_buffer (nRightNode);

	// append previously created entry copy that has been removed from the merge node to left node's data
	pBaseLeft[nLeftSize] = *psData;

	// append right node's data to left node's data
	memcpy ((void *) &(pBaseLeft[nLeftSize + 1]), (void *) pBaseRight, sizeof (*pBaseLeft) * nRightSize);

	//  remove entry that has been merged
	remove_data_from_node (nNode, nSub, nSub + 1);
	// remove_data_from_node has potential to re-allocate locations and thus to modify base pointers
	// all pointers to data layer need to be re-initialised

	// if the node being merged to is not a leaf node ...
	if (!is_leaf (nLeftNode))
	{
		// ... then merge sub node data as well
		_t_nodeiter			*pLeftSub;
		_t_nodeiter			*pRightSub;
		_t_subnodeiter		ui32;

		// load left and right sub node's data
		pLeftSub = get_sub_node_buffer (nLeftNode);
		pRightSub = get_sub_node_buffer (nRightNode);

		// append right node's sub node data to left node's sub node data
		memcpy ((void *) &(pLeftSub[nLeftSize + 1]), (void *) pRightSub, sizeof (*pLeftSub) * (nRightSize + 1));

		// set right node's sub nodes to refer to the left node as their parent
		for (ui32 = 0UL; ui32 < (nRightSize + 1); ui32++)
		{
			// load next right node's sub node
			node_t	*psNodeDesc;
			
			psNodeDesc = get_node (pRightSub[ui32]);

#if defined (_DEBUG)
			BTREE_ASSERT (psNodeDesc->nParent == nRightNode, "CBTreeBaseDefaults<_ti_pos, _t_data, _t_sizetype, _t_nodeiter, _t_subnodeiter, _t_datalayerproperties, _t_datalayer>::merge_node: unexpected parent node found while merging");
#endif

			// update sub node to left node as its parent
			psNodeDesc->nParent = nLeftNode;
		}

		// load left node's descriptor
		node_t	*psNodeDesc;
		
		psNodeDesc = get_node (nLeftNode);

		// update left node's size after the merge
		psNodeDesc->nNumData = nLeftSize + nRightSize + 1;
	}
	else // if the node being merged to is a leaf node ...
	{
		// load left node's descriptor
		node_t	*psNodeDesc;
		
		psNodeDesc = get_node (nLeftNode);

		// update left node's size after the merge
		psNodeDesc->nNumData = 0 - (nLeftSize + nRightSize + 1);
	}

	// remove right node from reservation list
	unset_reservation (nRightNode);

	// if the right node was smaller than the next node to allocate ...
	if (nRightNode < this->m_nNextAlloc)
	{
		// ... then the former right node is the next to allocate from
		this->m_nNextAlloc = nRightNode;
	}

	// update left and merge node
	this->rebuild_node (nLeftNode);
	this->rebuild_node (nNode);

	return (nRetval);
}

template <class _ti_pos, class _t_data, class _t_sizetype, class _t_nodeiter, class _t_subnodeiter, class _t_datalayerproperties, class _t_datalayer>
void CBTreeBaseDefaults<_ti_pos, _t_data, _t_sizetype, _t_nodeiter, _t_subnodeiter, _t_datalayerproperties, _t_datalayer>::rotate (_t_nodeiter nNode, _t_subnodeiter nSub, _t_subnodeiter &newSub, bool bLeftToRight)
{
	_t_nodeiter				*pnLeftNode, *pnRightNode;
	_t_nodeiter				*pnSubNode;
	_t_nodeiter				nSrcNode, nDstNode;
	_t_subnodeiter			nSrcSub, nDstSub;
	_t_subnodeiter			nSrcData;
	_t_data					*psData;
	node_t					*psDescLeft; // *psDescRight;
	int						triMod;

	// determine left and right sub node
	pnLeftNode = get_sub_node (nNode, nSub);
	pnRightNode = get_sub_node (nNode, nSub + 1);

	// get left and right sub node's description
	psDescLeft = get_node (*pnLeftNode);

	// if rotate is set from right to left ...
	if (!bLeftToRight)
	{
		// ... then set up left as destination and right as source
		nDstNode = *pnLeftNode;
		nDstSub = get_data_count (*psDescLeft);
		nSrcNode = *pnRightNode;
		nSrcData = 0;
		nSrcSub = 0;
		triMod = 1;
	}
	else
	{
		// ... otherwise set up right as destination and left as source
		nDstNode = *pnRightNode;
		nDstSub = 0;
		nSrcNode = *pnLeftNode;
		nSrcData = get_data_count (*pnLeftNode);
		nSrcSub = nSrcData - 1;
		triMod = 0;
	}

	// set newSub return value
	newSub = nDstSub;

	// get data from rotate over position
	psData = get_data (nNode, nSub);

	// if next level is leaf layer ...
	if ((is_leaf (*pnLeftNode)) && (is_leaf (*pnRightNode)))
	{
		// ... then copy data into destination leaf node
		insert_data_into_node (nDstNode, nDstSub, *psData);

		// insert_data_into_node has potential to re-allocate locations and thus to modify base pointers
		// all pointers to data layer need to be re-initialised
	}
	else if ((!is_leaf (*pnLeftNode)) && (!is_leaf (*pnRightNode)))
	{
		// ... otherwise copy data into destination node
		pnSubNode = get_sub_node (nSrcNode, nSrcData);

		insert_data_into_node (nDstNode, nDstSub, *psData, *pnSubNode, triMod);
	}
	else
	{
		BTREE_ASSERT (false, "CBTreeBaseDefaults<_ti_pos, _t_data, _t_sizetype, _t_nodeiter, _t_subnodeiter, _t_datalayerproperties, _t_datalayer>::rotate: two nodes which have the same parent are not on the same level");
	}

	// get data from source position
	psData = get_data (nSrcNode, nSrcSub);

	// copy data into rotate over position
	replace_data (nNode, nSub, *psData);

	// if next level is leaf layer ...
	if ((is_leaf (*pnLeftNode)) && (is_leaf (*pnRightNode)))
	{
		// ... then remove data from source leaf node
		remove_data_from_leaf (nSrcNode, nSrcSub);
	}
	else if ((!is_leaf (*pnLeftNode)) && (!is_leaf (*pnRightNode)))
	{
		// ... otherwise remove data from source node
		remove_data_from_node (nSrcNode, nSrcSub, nSrcData);
	}
	else
	{
		BTREE_ASSERT (false, "CBTreeBaseDefaults<_ti_pos, _t_data, _t_sizetype, _t_nodeiter, _t_subnodeiter, _t_datalayerproperties, _t_datalayer>::rotate: two nodes which have the same parent are not on the same level");
	}

	// destination node and rotate over node need to have their indexing updated
	// the source node was already updated during the remove
	this->rebuild_node (nDstNode);
	
	this->rebuild_node (nNode);
}

template <class _ti_pos, class _t_data, class _t_sizetype, class _t_nodeiter, class _t_subnodeiter, class _t_datalayerproperties, class _t_datalayer>
void CBTreeBaseDefaults<_ti_pos, _t_data, _t_sizetype, _t_nodeiter, _t_subnodeiter, _t_datalayerproperties, _t_datalayer>::replace_with_adjacent_entry (_t_nodeiter nNode, _t_subnodeiter nSub, bool bFromLeft)
{
#if defined (_DEBUG)
	BTREE_ASSERT (!is_leaf (nNode), "CBTreeBaseDefaults<_ti_pos, _t_data, _t_sizetype, _t_nodeiter, _t_subnodeiter, _t_datalayerproperties, _t_datalayer>::replace_with_adjacent_entry: method has been used on a leaf node!");
#endif

	_t_data				*psData;
	_t_nodeiter			nAdjNode;
	_t_subnodeiter		nAdjSub;
	bool				bBounce;

	// if entry has to be copied from the nearest previous position ...
	if (bFromLeft)
	{
		// ... then determine nearest previous position
		move_prev (nNode, nSub, nAdjNode, nAdjSub, bBounce);
	}
	else
	{
		// ... otherwise determine nearest following position
		move_next (nNode, nSub, nAdjNode, nAdjSub, bBounce);
	}

#if defined (_DEBUG)
	BTREE_ASSERT (bBounce == false, "CBTreeBaseDefaults<_ti_pos, _t_data, _t_sizetype, _t_nodeiter, _t_subnodeiter, _t_datalayerproperties, _t_datalayer>::replace_with_adjacent_entry: While looking for an adjacent entry, the method unexpectedly hit a termination!"); 

	BTREE_ASSERT (is_leaf (nAdjNode), "CBTreeBaseDefaults<_ti_pos, _t_data, _t_sizetype, _t_nodeiter, _t_subnodeiter, _t_datalayerproperties, _t_datalayer>::replace_with_adjacent_entry: Adjacent node must be a leaf node!");
#endif
	
	// get data from previously determined nearest position
	psData = get_data (nAdjNode, nAdjSub);

	// copy data to input location
	replace_data (nNode, nSub, *psData);
}

template <class _ti_pos, class _t_data, class _t_sizetype, class _t_nodeiter, class _t_subnodeiter, class _t_datalayerproperties, class _t_datalayer>
_t_sizetype CBTreeBaseDefaults<_ti_pos, _t_data, _t_sizetype, _t_nodeiter, _t_subnodeiter, _t_datalayerproperties, _t_datalayer>::serialize (const _ti_pos nFrom, const _t_sizetype nLen, _t_data *pData, const bool bReadOpr) const
{
	if (m_pData == NULL)
	{
		return (0);
	}

	_t_sizetype						nRslt = 0;
	bool							bFound;
	bool							bBounce;
	_t_nodeiter						nNode = this->m_nRootNode;
	_t_subnodeiter					nSubPos, nSubData;
	_t_nodeiter						nNextNode;
	_t_subnodeiter					nNextSubPos;
	_ti_pos							sPos;
	_t_sizetype						i;
	_t_data							*psData;

	sPos = nFrom;

	// translate start position to node / sub-position
	do
	{
		sPos = this->determine_position (sPos, nNode, nSubPos, nSubData, bFound);

		if (!bFound)
		{
			nNode = *(this->get_sub_node (nNode, nSubPos));
		}
	} while (!bFound);

	// serialize data
	for (i = 0; i < nLen; )
	{
		if (this->is_leaf (nNode))
		{
			_t_subnodeiter	nRem = (_t_subnodeiter) (nLen - i);

			if ((nSubData == 0) && (nRem >= (_t_sizetype) this->get_data_count (nNode)))
			{
				nRem = this->get_data_count (nNode);

				psData = this->get_data_buffer (nNode);

				if (bReadOpr)
				{
					memcpy ((void *) (&(pData[i])), (void *) psData, sizeof (_t_data) * nRem);
				}
				else
				{
					memcpy ((void *) psData, (void *) (&(pData[i])), sizeof (_t_data) * nRem);
				}
			}
			else
			{
				psData = this->get_data_buffer (nNode);

				if ((this->get_data_count (nNode) - nSubData) < nRem)
				{
					nRem = this->get_data_count (nNode) - nSubData;
				}

				if (bReadOpr)
				{
					memcpy ((void *) &(pData[i]), (void *) &(psData[nSubData]), sizeof (_t_data) * nRem);
				}
				else
				{
					memcpy ((void *) &(psData[nSubData]), (void *) &(pData[i]), sizeof (_t_data) * nRem);
				}
			}

			nSubPos = this->get_data_count (nNode);

			this->move_next (nNode, nSubPos, nNextNode, nNextSubPos, bBounce);

			i += (_t_sizetype) nRem;
			nRslt += (_t_sizetype) nRem;
		}
		else
		{
			psData = this->get_data (nNode, nSubPos);

			if (bReadOpr)
			{
				pData[i] = *psData;
			}
			else
			{
				*psData = pData[i];
			}

			this->move_next (nNode, nSubPos, nNextNode, nNextSubPos, bBounce);

			i++;
			nRslt++;
		}

		if (bBounce)
		{
			break;
		}

		nSubData = 0;

		nNode = nNextNode;
		nSubPos = nNextSubPos;
	}

	return (nRslt);
}

template <class _ti_pos, class _t_data, class _t_sizetype, class _t_nodeiter, class _t_subnodeiter, class _t_datalayerproperties, class _t_datalayer>
_t_subnodeiter CBTreeBaseDefaults<_ti_pos, _t_data, _t_sizetype, _t_nodeiter, _t_subnodeiter, _t_datalayerproperties, _t_datalayer>::insert_data_into_node (_t_nodeiter nNode, _t_subnodeiter nSubPos, const _t_data &rData, _t_nodeiter nSubNode, int triMod)
{
	node_t					*psNodeDesc;
	_t_nodeiter				nNewNode;
	_t_subnodeiter			nNumData;
	
#if defined (_DEBUG)
	BTREE_ASSERT (get_data_count (nNode) <= this->get_node_max_data_size (), "CBTreeBaseDefaults<_ti_pos, _t_data, _t_sizetype, _t_nodeiter, _t_subnodeiter, _t_datalayerproperties, _t_datalayer>::insert_data_into_node: node exceeds possible data entries");
#endif

	// load node descriptor
	psNodeDesc = get_node (nNode);

	// determine node size
	nNumData = get_data_count (*psNodeDesc);

	// if the node's size has reached its maximum ...
	if (nNumData >= this->get_node_max_data_size ())
	{
		// ... then split node and divide it to node and nodeG
		// the bottom half will remain in the node referred by the variable 'node'
		// the top half will be copied to the node referred by the variable 'newNode'
		nNewNode = split_node (nNode);

		// split_node has potential to re-allocate locations and thus to modify base pointers
		// all pointers to data layer need to be re-initialised
		psNodeDesc = get_node (nNode);
	
		// if entry to be added is beyond the maximum node size half ...
		if (nSubPos >= this->m_nNodeSize)
		{
			// ... then add entry to the node newly created 'newNode'
			nNode = nNewNode;

			// load newNode descriptor
			psNodeDesc = get_node (nNode);

			// update sub position due to new node generated
			nSubPos -= (uint32_t)this->m_nNodeSize;
		}
		else // if entry to be added is not beyond the maximum node size half ...
		{
			// ... then load node descriptor
			psNodeDesc = get_node (nNode);
		}

		// re-determine node size after node split
		nNumData = get_data_count (*psNodeDesc);
	}

	_t_nodeiter		*pSubNodes;

	/*
	** triMod - three state modifier
	**
	** -1	- do not insert new sub node id
	** 0	- insert new sub node id prior to nSubPos
	** 1	- insert new sub node id subsequent to nSubPos
	*/

	// if new sub node id has to be inserted ...
	if (triMod != -1)
	{
		// ... then get sub node buffer
		pSubNodes = get_sub_node_buffer (nNode);

		node_t	*psSubNodeDesc;
		
		// update parent of inserted node
		psSubNodeDesc = get_node (nSubNode);

		psSubNodeDesc->nParent = nNode;
		
		// insert node into sub node buffer
		memmove ((void *) &(pSubNodes[nSubPos + triMod + 1]), (void *) &(pSubNodes[nSubPos + triMod]), sizeof (*pSubNodes) * ((nNumData + 1) - (nSubPos + triMod)));
		pSubNodes[nSubPos + triMod] = nSubNode;
	}
#if defined (_DEBUG)
	else
	{
		BTREE_ASSERT (is_leaf (*psNodeDesc), "CBTreeBaseDefaults<_ti_pos, _t_data, _t_sizetype, _t_nodeiter, _t_subnodeiter, _t_datalayerproperties, _t_datalayer>::insert_data_into_node: insert_data_into_node expects either subNodeBeforeData or subNodeAfterData to contain a valid node index when processing an inner node");
	}
#endif

	// insert one data entry into a node's buffer
	m_pData->template insert_dataIntoPool<_t_data> (this->m_nPoolIDdata, nNode, get_data_count (*psNodeDesc), nSubPos, 1, &rData);

	// if this node is not a leaf node ...
	if (triMod != -1)
	{
		// if this node used to be a leaf node ...
		if (is_leaf (*psNodeDesc))
		{
			// ... then convert to inner node
			psNodeDesc->nNumData = 0 - psNodeDesc->nNumData;
		}

		// increment node size
		psNodeDesc->nNumData++;
	}
	else // if this node is a leaf node ...
	{
		// absolute number contains number of data entries - if zero or negativ then leaf node

		// increment leaf node size
		psNodeDesc->nNumData--;
		psNodeDesc->nMaxIdx++;
	}

	this->update_time_stamp ();

	return (nSubPos);
}

template <class _ti_pos, class _t_data, class _t_sizetype, class _t_nodeiter, class _t_subnodeiter, class _t_datalayerproperties, class _t_datalayer>
void CBTreeBaseDefaults<_ti_pos, _t_data, _t_sizetype, _t_nodeiter, _t_subnodeiter, _t_datalayerproperties, _t_datalayer>::remove_data_from_leaf (_t_nodeiter nNode, _t_subnodeiter nSub)
{
#if defined (_DEBUG)
	node_t		*psNodeDesc;

	psNodeDesc = get_node (nNode);

	BTREE_ASSERT (is_leaf (*psNodeDesc), "CBTreeBaseDefaults<_ti_pos, _t_data, _t_sizetype, _t_nodeiter, _t_subnodeiter, _t_datalayerproperties, _t_datalayer>::remove_data_from_leaf: remove_data_from_leaf ought to be called on leaf nodes");
	BTREE_ASSERT (get_data_count (*psNodeDesc) > 0, "CBTreeBaseDefaults<_ti_pos, _t_data, _t_sizetype, _t_nodeiter, _t_subnodeiter, _t_datalayerproperties, _t_datalayer>::remove_data_from_leaf: remove_data_from_leaf ought not to be called on empty nodes");
#endif

	// remove data entry from node
	remove_data_from_node (nNode, nSub, ~0x0);
}

template <class _ti_pos, class _t_data, class _t_sizetype, class _t_nodeiter, class _t_subnodeiter, class _t_datalayerproperties, class _t_datalayer>
void CBTreeBaseDefaults<_ti_pos, _t_data, _t_sizetype, _t_nodeiter, _t_subnodeiter, _t_datalayerproperties, _t_datalayer>::remove_data_from_node (_t_nodeiter nNode, _t_subnodeiter nSubData, _t_subnodeiter nSubNode)
{
	node_t					*psNodeDesc;
	_t_subnodeiter			nNumData;

	psNodeDesc = get_node (nNode);

	// determine node's data size
	nNumData = get_data_count (*psNodeDesc);

#if defined (_DEBUG)
	BTREE_ASSERT (nSubData < nNumData, "CBTreeBaseDefaults<_ti_pos, _t_data, _t_sizetype, _t_nodeiter, _t_subnodeiter, _t_datalayerproperties, _t_datalayer>::remove_data_from_node: subData exceeds nodes data size");
	BTREE_ASSERT (nNumData > 0, "CBTreeBaseDefaults<_ti_pos, _t_data, _t_sizetype, _t_nodeiter, _t_subnodeiter, _t_datalayerproperties, _t_datalayer>::remove_data_from_node: cannot remove in an empty node");
#endif

	_t_data		*pBase;

	// load all data from current node
	pBase = get_data_buffer (nNode);

	// remove entry
	memmove ((void *) &(pBase[nSubData]), (void *) &(pBase[nSubData + 1L]), sizeof (*pBase) * (nNumData - nSubData - 1L));

	// if node is not a leaf node ...
	if (!is_leaf (nNode))
	{
		// ... then remove sub-node entry
#if defined (_DEBUG)
		BTREE_ASSERT (nSubNode <= nNumData, "CBTreeBaseDefaults<_ti_pos, _t_data, _t_sizetype, _t_nodeiter, _t_subnodeiter, _t_datalayerproperties, _t_datalayer>::remove_data_from_node: subNode exceeds nodes sub entry size");
#endif

		_t_nodeiter		*pSubNodes;
		
		// load all sub-node data from current node
		pSubNodes = get_sub_node_buffer (nNode);

		// remove sub-node entry
		memmove ((void *) &(pSubNodes[nSubNode]), (void *) &(pSubNodes[nSubNode + 1L]), sizeof (*pSubNodes) * (nNumData - nSubNode));

		// decrement node size
		psNodeDesc->nNumData--;
	}
	else
	{
		// ... otherwise only decrement node size
		psNodeDesc->nNumData++;
	}

	// update current node
	this->rebuild_node (nNode);

	auto_shrink_inc ();

	this->update_time_stamp ();
}

template <class _ti_pos, class _t_data, class _t_sizetype, class _t_nodeiter, class _t_subnodeiter, class _t_datalayerproperties, class _t_datalayer>
void CBTreeBaseDefaults<_ti_pos, _t_data, _t_sizetype, _t_nodeiter, _t_subnodeiter, _t_datalayerproperties, _t_datalayer>::replace_data (_t_nodeiter nNode, _t_subnodeiter nSub, const _t_data &rData)
{
	// replace data by overwriting old data
	_t_data		*psData = get_data (nNode, nSub);

	*psData = rData;
}

template <class _ti_pos, class _t_data, class _t_sizetype, class _t_nodeiter, class _t_subnodeiter, class _t_datalayerproperties, class _t_datalayer>
_t_subnodeiter CBTreeBaseDefaults<_ti_pos, _t_data, _t_sizetype, _t_nodeiter, _t_subnodeiter, _t_datalayerproperties, _t_datalayer>::find_sub_node_offset (_t_nodeiter nNode, _t_nodeiter nSubNode) const
{
	_t_subnodeiter		nRetval = ~0;
	_t_subnodeiter		i;
	_t_subnodeiter		nNumSub;
	_t_nodeiter			*pSubNodes;

#if defined (_DEBUG)
	BTREE_ASSERT (!is_leaf (nNode), "CBTreeBaseDefaults<_ti_pos, _t_data, _t_sizetype, _t_nodeiter, _t_subnodeiter, _t_datalayerproperties, _t_datalayer>::find_sub_node_offset: method was used on leaf node!");
#endif

	// if node is not a leaf node ...
	if (!is_leaf (nNode))
	{
		// ... then seek for sub node in sub node list

		// load sub node list
		pSubNodes = get_sub_node_buffer (nNode);

		// determine size of sub node list
		nNumSub = get_sub_node_count (nNode);
		
		for (i = 0; i < nNumSub; i++)
		{
			// if node was found ...
			if (pSubNodes[i] == nSubNode)
			{
				// ... then return with position
				nRetval = i;

				break;
			}
		}
	}

	return (nRetval);
}

template <class _ti_pos, class _t_data, class _t_sizetype, class _t_nodeiter, class _t_subnodeiter, class _t_datalayerproperties, class _t_datalayer>
void CBTreeBaseDefaults<_ti_pos, _t_data, _t_sizetype, _t_nodeiter, _t_subnodeiter, _t_datalayerproperties, _t_datalayer>::move_prev (_t_nodeiter nNode, _t_subnodeiter nSub, _t_nodeiter &nPrevNode, _t_subnodeiter &nPrevSubPos, bool &bBounce) const
{
#if defined (_DEBUG)
	BTREE_ASSERT (nNode < this->m_nTreeSize, "CBTreeBaseDefaults<_ti_pos, _t_data, _t_sizetype, _t_nodeiter, _t_subnodeiter, _t_datalayerproperties, _t_datalayer>::move_prev: requested node exceeds tree size");
#endif

	// search for nearest left or previous entry
	// and return result in nPrevNode, nPrevSubPos and bBounce

	node_t		*psNodeDesc;
	
	psNodeDesc = get_node (nNode);
	
	nPrevNode = nNode;
	nPrevSubPos = nSub;
	bBounce = false;

	// if leaf node ...
	if (is_leaf (*psNodeDesc))
	{
		// ... and the left most position has not been reached ...
		if (nSub > 0)
		{
			// ... then decrement sub position and return
			nSub--;

			nPrevNode = nNode;
			nPrevSubPos = nSub;
		}
		else
		{
			// ... otherwise if leaf node and left most position has been reached, then seek for the one parent's parent node that contains the previous data entry
			do
			{
				// if during the search the root node is reached ...
				if (psNodeDesc->nParent == nNode)
				{
					// ... then assert that the beginning of the list has been found and return
					bBounce = true;

					return;
				}

				// find this node's position in the parent node
				nPrevSubPos = find_sub_node_offset (psNodeDesc->nParent, nNode);
				nNode = psNodeDesc->nParent;

				psNodeDesc = get_node (nNode);

			// continue until a parent node has been found, where the current node's position is not on the left
			} while (nPrevSubPos == 0);

			// the found parent node becomes the return node value
			nPrevNode = nNode;

			// and the position before the found sub position beecomes the return sub position
			nPrevSubPos--;
		}
	}
	else
	{
		// ... otherwise if this is not a leaf node, then seek for the leaf node, which contains the nearest previous data entry
		do
		{
			// get next sub node
			nNode = *(get_sub_node (nNode, nSub));

			psNodeDesc = get_node (nNode);

			// determine most right position within node
			nSub = get_data_count (*psNodeDesc);
		} while (!is_leaf (nNode));

		// the found leaf node becomes the return node value
		nPrevNode = nNode;

		// the most right position within that leaf node becomes the return sub node position
		nPrevSubPos = get_data_count (*psNodeDesc) - 1;
	}
}

template <class _ti_pos, class _t_data, class _t_sizetype, class _t_nodeiter, class _t_subnodeiter, class _t_datalayerproperties, class _t_datalayer>
void CBTreeBaseDefaults<_ti_pos, _t_data, _t_sizetype, _t_nodeiter, _t_subnodeiter, _t_datalayerproperties, _t_datalayer>::move_next (_t_nodeiter nNode, _t_subnodeiter nSub, _t_nodeiter &nNextNode, _t_subnodeiter &nNextSubPos, bool &bBounce) const
{
#if defined (_DEBUG)
	BTREE_ASSERT (nNode < this->m_nTreeSize, "CBTreeBaseDefaults<_ti_pos, _t_data, _t_sizetype, _t_nodeiter, _t_subnodeiter, _t_datalayerproperties, _t_datalayer>::move_next: requested node exceeds tree size");
#endif

	// search for nearest left or previous entry
	// and return result in nNextNode, nNextSubPos and bBounce

	node_t		*psNodeDesc;
	
	psNodeDesc = get_node (nNode);

	nNextNode = nNode;
	nNextSubPos = nSub;
	bBounce = false;

	// if leaf node ...
	if (is_leaf (*psNodeDesc))
	{
		// ... and the right most position has not been reached ...
		if (nSub < (get_data_count (*psNodeDesc) - 1))
		{
			// ... then increment sub position and return
			nSub++;

			nNextNode = nNode;
			nNextSubPos = nSub;
		}
		else
		{
			// ... otherwise if leaf node and right most position has been reached, then seek for the one parent's parent node that contains the next data entry
			do
			{
				// if during the search the root node is reached ...
				if (psNodeDesc->nParent == nNode)
				{
					// ... then assert that the beginning of the list has been found and return
					bBounce = true;

					return;
				}

				// find this node's position in the parent node - this also becomes the return sub position
				nNextSubPos = find_sub_node_offset (psNodeDesc->nParent, nNode);
				nNode = psNodeDesc->nParent;

				psNodeDesc = get_node (nNode);

				BTREE_ASSERT (nNextSubPos <= get_data_count (*psNodeDesc), "CBTreeBaseDefaults<_ti_pos, _t_data, _t_sizetype, _t_nodeiter, _t_subnodeiter, _t_datalayerproperties, _t_datalayer>::move_next: move_next went beyond nodes max data entries");

			// continue until a parent node has been found, where the current node's position is not on the right
			} while (nNextSubPos == get_data_count (*psNodeDesc));

			// the found parent node becomes the return node value
			nNextNode = nNode;
		}
	}
	else
	{
		// ... otherwise if this is not a leaf node, then increment to the next position and seek for the leaf node, which contains the nearest subsequent data entry
		nSub++;

		do
		{
			// get next sub node
			nNode = *(get_sub_node (nNode, nSub));

			// set sub position to left most position
			nSub = 0;
		} while (!is_leaf (nNode));

		// the found leaf node becomes the return node value
		nNextNode = nNode;

		// that leaf node's most left sub position becomes the return sub position value
		nNextSubPos = 0;
	}
}

template <class _ti_pos, class _t_data, class _t_sizetype, class _t_nodeiter, class _t_subnodeiter, class _t_datalayerproperties, class _t_datalayer>
void CBTreeBaseDefaults<_ti_pos, _t_data, _t_sizetype, _t_nodeiter, _t_subnodeiter, _t_datalayerproperties, _t_datalayer>::set_reservation (_t_nodeiter nNode)
{
#if defined (_DEBUG)
	BTREE_ASSERT (nNode < this->m_nTreeSize, "CBTreeBaseDefaults<_ti_pos, _t_data, _t_sizetype, _t_nodeiter, _t_subnodeiter, _t_datalayerproperties, _t_datalayer>::set_reservation: requested reservation exceeds tree size");
#endif

	node_maintainence_t		*psMaintenance;

	// read maintenancy vector of node
	psMaintenance = m_pData->template get_pooledData<node_maintainence_t> (this->m_nPoolIDnodeMaintenance, nNode);

	// set reservation bit
	psMaintenance->uVector |= BAYERTREE_NODE_MAINTAINANCE_ALLOCATION;
}

template <class _ti_pos, class _t_data, class _t_sizetype, class _t_nodeiter, class _t_subnodeiter, class _t_datalayerproperties, class _t_datalayer>
bool CBTreeBaseDefaults<_ti_pos, _t_data, _t_sizetype, _t_nodeiter, _t_subnodeiter, _t_datalayerproperties, _t_datalayer>::get_reservation (_t_nodeiter nNode) const
{
#if defined (_DEBUG)
	BTREE_ASSERT (nNode < this->m_nTreeSize, "CBTreeBaseDefaults<_ti_pos, _t_data, _t_sizetype, _t_nodeiter, _t_subnodeiter, _t_datalayerproperties, _t_datalayer>::get_reservation: requested reservation information beyond tree size");
#endif

	node_maintainence_t		*psMaintenance;

	// read maintenancy vector of node
	psMaintenance = m_pData->template get_pooledData<node_maintainence_t> (this->m_nPoolIDnodeMaintenance, nNode);

	// return true if node is flagged as reserved, otherwise false
	return ((psMaintenance->uVector & BAYERTREE_NODE_MAINTAINANCE_ALLOCATION) != 0 ? true : false);
}

template <class _ti_pos, class _t_data, class _t_sizetype, class _t_nodeiter, class _t_subnodeiter, class _t_datalayerproperties, class _t_datalayer>
void CBTreeBaseDefaults<_ti_pos, _t_data, _t_sizetype, _t_nodeiter, _t_subnodeiter, _t_datalayerproperties, _t_datalayer>::unset_reservation (_t_nodeiter nNode)
{
#if defined (_DEBUG)
	BTREE_ASSERT (nNode < this->m_nTreeSize, "CBTreeBaseDefaults<_ti_pos, _t_data, _t_sizetype, _t_nodeiter, _t_subnodeiter, _t_datalayerproperties, _t_datalayer>::unset_reservation: requested reservation release exceeds tree size");
#endif

	node_maintainence_t		*psMaintenance;

	// read maintenancy vector of node
	psMaintenance = m_pData->template get_pooledData<node_maintainence_t> (this->m_nPoolIDnodeMaintenance, nNode);

	// unset reservation bit
	psMaintenance->uVector &= ~BAYERTREE_NODE_MAINTAINANCE_ALLOCATION;
}

template <class _ti_pos, class _t_data, class _t_sizetype, class _t_nodeiter, class _t_subnodeiter, class _t_datalayerproperties, class _t_datalayer>
void CBTreeBaseDefaults<_ti_pos, _t_data, _t_sizetype, _t_nodeiter, _t_subnodeiter, _t_datalayerproperties, _t_datalayer>::reset_reservation (_t_nodeiter nNode)
{
#if defined (_DEBUG)
	BTREE_ASSERT (nNode < this->m_nTreeSize, "CBTreeBaseDefaults<_ti_pos, _t_data, _t_sizetype, _t_nodeiter, _t_subnodeiter, _t_datalayerproperties, _t_datalayer>::reset_reservation: requested reservation release exceeds tree size");
#endif

	node_maintainence_t		*psMaintenance;

	// read maintenancy vector of node
	psMaintenance = m_pData->template get_pooledData<node_maintainence_t> (this->m_nPoolIDnodeMaintenance, nNode);

	// unset reservation bit
	psMaintenance->uVector = 0;
}

template <class _ti_pos, class _t_data, class _t_sizetype, class _t_nodeiter, class _t_subnodeiter, class _t_datalayerproperties, class _t_datalayer>
_t_subnodeiter CBTreeBaseDefaults<_ti_pos, _t_data, _t_sizetype, _t_nodeiter, _t_subnodeiter, _t_datalayerproperties, _t_datalayer>::get_data_count (const _t_nodeiter nNode) const
{// returns the present number of entries in a node
	node_t		*psNodeDesc;

	psNodeDesc = get_node (nNode);

	return (get_data_count (*psNodeDesc));
}

template <class _ti_pos, class _t_data, class _t_sizetype, class _t_nodeiter, class _t_subnodeiter, class _t_datalayerproperties, class _t_datalayer>
_t_subnodeiter CBTreeBaseDefaults<_ti_pos, _t_data, _t_sizetype, _t_nodeiter, _t_subnodeiter, _t_datalayerproperties, _t_datalayer>::get_data_count (const node_t &sNodeDesc) const
{// returns the present number of entries from a node descriptor
	_t_subnodeiter		nRetval;

	// if node descriptor is displaying a leaf node ...
	if (is_leaf (sNodeDesc))
	{
		// ... then return the inverted number of data entries, since leaf nodes store the their fill state as a 2's complement
		nRetval = (_t_subnodeiter) (0 - sNodeDesc.nNumData);
	}
	else
	{
		// ... otherwise return fill state
		nRetval = (_t_subnodeiter) sNodeDesc.nNumData;
	}

	return (nRetval);
}

template <class _ti_pos, class _t_data, class _t_sizetype, class _t_nodeiter, class _t_subnodeiter, class _t_datalayerproperties, class _t_datalayer>
_t_subnodeiter CBTreeBaseDefaults<_ti_pos, _t_data, _t_sizetype, _t_nodeiter, _t_subnodeiter, _t_datalayerproperties, _t_datalayer>::get_sub_node_count (const _t_nodeiter nNode) const
{// returns the present number of sub nodes
	node_t		*psNodeDesc;

	psNodeDesc = get_node (nNode);

	return (get_sub_node_count (*psNodeDesc));
}

template <class _ti_pos, class _t_data, class _t_sizetype, class _t_nodeiter, class _t_subnodeiter, class _t_datalayerproperties, class _t_datalayer>
_t_subnodeiter CBTreeBaseDefaults<_ti_pos, _t_data, _t_sizetype, _t_nodeiter, _t_subnodeiter, _t_datalayerproperties, _t_datalayer>::get_sub_node_count (const node_t &sNodeDesc) const
{
	_t_subnodeiter		nRetval;

	// if node descriptor is displaying a leaf node ...
	if (is_leaf (sNodeDesc))
	{
		// ... then return zero sub nodes
		nRetval = (_t_subnodeiter) 0;
	}
	else
	{
		// ... otherwise return number of sub nodes
		nRetval = (_t_subnodeiter) (sNodeDesc.nNumData + 1);
	}

	return (nRetval);
}

template <class _ti_pos, class _t_data, class _t_sizetype, class _t_nodeiter, class _t_subnodeiter, class _t_datalayerproperties, class _t_datalayer>
_t_sizetype CBTreeBaseDefaults<_ti_pos, _t_data, _t_sizetype, _t_nodeiter, _t_subnodeiter, _t_datalayerproperties, _t_datalayer>::get_max_index (const _t_nodeiter nNode) const
{// returns number of data entries in sub tree contain node (nNode)
	node_t		*psNodeDesc;

	psNodeDesc = get_node (nNode);

	return (get_max_index (*psNodeDesc));
}

template <class _ti_pos, class _t_data, class _t_sizetype, class _t_nodeiter, class _t_subnodeiter, class _t_datalayerproperties, class _t_datalayer>
_t_sizetype  CBTreeBaseDefaults<_ti_pos, _t_data, _t_sizetype, _t_nodeiter, _t_subnodeiter, _t_datalayerproperties, _t_datalayer>::get_max_index (const node_t &sNodeDesc) const
{// returns number of data entries in sub tree displayed by node descriptor
	return (sNodeDesc.nMaxIdx);
}

template <class _ti_pos, class _t_data, class _t_sizetype, class _t_nodeiter, class _t_subnodeiter, class _t_datalayerproperties, class _t_datalayer>
_t_nodeiter CBTreeBaseDefaults<_ti_pos, _t_data, _t_sizetype, _t_nodeiter, _t_subnodeiter, _t_datalayerproperties, _t_datalayer>::get_parent (const _t_nodeiter nNode) const
{
	node_t		*psNodeDesc;

	psNodeDesc = get_node (nNode);

	return (get_parent (*psNodeDesc));

}

template <class _ti_pos, class _t_data, class _t_sizetype, class _t_nodeiter, class _t_subnodeiter, class _t_datalayerproperties, class _t_datalayer>
_t_nodeiter CBTreeBaseDefaults<_ti_pos, _t_data, _t_sizetype, _t_nodeiter, _t_subnodeiter, _t_datalayerproperties, _t_datalayer>::get_parent (const node_t &sNodeDesc) const
{
	return (sNodeDesc.nParent);
}

template <class _ti_pos, class _t_data, class _t_sizetype, class _t_nodeiter, class _t_subnodeiter, class _t_datalayerproperties, class _t_datalayer>
bool CBTreeBaseDefaults<_ti_pos, _t_data, _t_sizetype, _t_nodeiter, _t_subnodeiter, _t_datalayerproperties, _t_datalayer>::is_root (const _t_nodeiter nNode) const
{
	return (nNode == this->m_nRootNode);
}

template <class _ti_pos, class _t_data, class _t_sizetype, class _t_nodeiter, class _t_subnodeiter, class _t_datalayerproperties, class _t_datalayer>
bool CBTreeBaseDefaults<_ti_pos, _t_data, _t_sizetype, _t_nodeiter, _t_subnodeiter, _t_datalayerproperties, _t_datalayer>::is_leaf (const _t_nodeiter nNode) const
{// returns if node (nNode) is a leaf node or an inner node
	node_t		*psNodeDesc;

	psNodeDesc = get_node (nNode);

	return (is_leaf (*psNodeDesc));
}

template <class _ti_pos, class _t_data, class _t_sizetype, class _t_nodeiter, class _t_subnodeiter, class _t_datalayerproperties, class _t_datalayer>
bool CBTreeBaseDefaults<_ti_pos, _t_data, _t_sizetype, _t_nodeiter, _t_subnodeiter, _t_datalayerproperties, _t_datalayer>::is_leaf (const node_t &nodeDesc) const
{
//	const _t_subnodeiter		nMask = ~(((_t_subnodeiter) ~0) >> 1);
//	bool						bRslt = nodeDesc.nNumData == 0;
//
//	bRslt = bRslt || ((nMask & nodeDesc.nNumData) != 0x0);
//
//	// returns if true if node descriptor displays a leaf node, otherwise false
//	return (bRslt);

	// equivalent source code above

	// code below equals:
	// return value = ((signed type) nodeDesc.nNumData) >= 0
	//
	// the code below rotates the input value nNumData from an unsinged space
	// to that of a singed space and therefore allows for a single compare
	// operation instead of two (see code above)
	const _t_subnodeiter		nOffset = ~(((_t_subnodeiter) ~0) >> 1);
	_t_subnodeiter				nRot = nodeDesc.nNumData + nOffset;

	return (nRot <= nOffset);
}

template <class _ti_pos, class _t_data, class _t_sizetype, class _t_nodeiter, class _t_subnodeiter, class _t_datalayerproperties, class _t_datalayer>
typename CBTreeBaseDefaults<_ti_pos, _t_data, _t_sizetype, _t_nodeiter, _t_subnodeiter, _t_datalayerproperties, _t_datalayer>::node_t*
	CBTreeBaseDefaults<_ti_pos, _t_data, _t_sizetype, _t_nodeiter, _t_subnodeiter, _t_datalayerproperties, _t_datalayer>::get_node (const _t_nodeiter nNode) const
{
#if defined (_DEBUG)

	BTREE_ASSERT (nNode < this->m_nTreeSize, "CBTreeBaseDefaults<_ti_pos, _t_data, _t_sizetype, _t_nodeiter, _t_subnodeiter, _t_datalayerproperties, _t_datalayer>::get_node: requested node exceeds tree size");

#endif

	// otherwise read node from IO layer
	return (m_pData->template get_pooledData <node_t> (this->m_nPoolIDnodeDesc, nNode));
}

template <class _ti_pos, class _t_data, class _t_sizetype, class _t_nodeiter, class _t_subnodeiter, class _t_datalayerproperties, class _t_datalayer>
_t_nodeiter* CBTreeBaseDefaults<_ti_pos, _t_data, _t_sizetype, _t_nodeiter, _t_subnodeiter, _t_datalayerproperties, _t_datalayer>::get_sub_node (const _t_nodeiter nNode, const _t_subnodeiter nEntry) const
{
#if defined (_DEBUG)
	node_t	*psNodeDesc;
	
	psNodeDesc = get_node (nNode);

	if (nNode != this->m_nRootNode)
	{
		BTREE_ASSERT (!is_leaf (nNode), "CBTreeBaseDefaults<_ti_pos, _t_data, _t_sizetype, _t_nodeiter, _t_subnodeiter, _t_datalayerproperties, _t_datalayer>::get_sub_node: get_sub_node cannot be used with leaf nodes");
	}

	BTREE_ASSERT (nEntry <= get_data_count (*psNodeDesc), "CBTreeBaseDefaults<_ti_pos, _t_data, _t_sizetype, _t_nodeiter, _t_subnodeiter, _t_datalayerproperties, _t_datalayer>::get_sub_node: requested sub position exceeds node size");
	BTREE_ASSERT (nNode < this->m_nTreeSize, "CBTreeBaseDefaults<_ti_pos, _t_data, _t_sizetype, _t_nodeiter, _t_subnodeiter, _t_datalayerproperties, _t_datalayer>::get_sub_node: requested node exceeds tree size");
#endif

	// read sub node ID from IO layer
	return (m_pData->template get_pooledData<_t_nodeiter> (this->m_nPoolIDsubNodes, nNode, nEntry));
}

template <class _ti_pos, class _t_data, class _t_sizetype, class _t_nodeiter, class _t_subnodeiter, class _t_datalayerproperties, class _t_datalayer>
_t_nodeiter* CBTreeBaseDefaults<_ti_pos, _t_data, _t_sizetype, _t_nodeiter, _t_subnodeiter, _t_datalayerproperties, _t_datalayer>::get_sub_node_buffer (const _t_nodeiter nNode) const
{
#if defined (_DEBUG)
	BTREE_ASSERT (nNode < this->m_nTreeSize, "CBTreeBaseDefaults<_ti_pos, _t_data, _t_sizetype, _t_nodeiter, _t_subnodeiter, _t_datalayerproperties, _t_datalayer>::get_sub_node_buffer: requested node exceeds tree size");
#endif

	// read sub node buffer from IO layer
	return (m_pData->template get_pooledData<_t_nodeiter> (this->m_nPoolIDsubNodes, nNode));

	// in the above call get_data_count is in use instead of get_sub_node_count
	// In the event a new node is created it has size zero and would therefore be deemed
	// as a leaf node. In that case get_sub_node_count would always return zero, which is incorrect here.
}

template <class _ti_pos, class _t_data, class _t_sizetype, class _t_nodeiter, class _t_subnodeiter, class _t_datalayerproperties, class _t_datalayer>
_t_data* CBTreeBaseDefaults<_ti_pos, _t_data, _t_sizetype, _t_nodeiter, _t_subnodeiter, _t_datalayerproperties, _t_datalayer>::get_data (const _t_nodeiter nNode, const _t_subnodeiter nEntry) const
{
#if defined (_DEBUG)
	BTREE_ASSERT (nNode < this->m_nTreeSize, "CBTreeBaseDefaults<_ti_pos, _t_data, _t_sizetype, _t_nodeiter, _t_subnodeiter, _t_datalayerproperties, _t_datalayer>::get_data: requested node exceeds tree size");

	BTREE_ASSERT (nEntry < this->get_node_max_data_size (), "CBTreeBaseDefaults<_ti_pos, _t_data, _t_sizetype, _t_nodeiter, _t_subnodeiter, _t_datalayerproperties, _t_datalayer>::get_data: requested entry exceeds possible node size");
#endif

	// read data entry from IO layer
	return (m_pData->template get_pooledData<_t_data> (this->m_nPoolIDdata, nNode, nEntry));
}

template <class _ti_pos, class _t_data, class _t_sizetype, class _t_nodeiter, class _t_subnodeiter, class _t_datalayerproperties, class _t_datalayer>
void CBTreeBaseDefaults<_ti_pos, _t_data, _t_sizetype, _t_nodeiter, _t_subnodeiter, _t_datalayerproperties, _t_datalayer>::set_data (const _t_nodeiter nNode, const _t_subnodeiter nEntry, _t_data sData)
{
#if defined (_DEBUG)
	BTREE_ASSERT (nNode < this->m_nTreeSize, "CBTreeBaseDefaults<_ti_pos, _t_data, _t_sizetype, _t_nodeiter, _t_subnodeiter, _t_datalayerproperties, _t_datalayer>::set_data: requested node exceeds tree size");
#endif

	// write data entry to IO layer
	_t_data		*psData = m_pData->template get_pooledData<_t_data> (this->m_nPoolIDdata, nNode);

	psData[nEntry] = sData;
}

template <class _ti_pos, class _t_data, class _t_sizetype, class _t_nodeiter, class _t_subnodeiter, class _t_datalayerproperties, class _t_datalayer>
_t_data* CBTreeBaseDefaults<_ti_pos, _t_data, _t_sizetype, _t_nodeiter, _t_subnodeiter, _t_datalayerproperties, _t_datalayer>::get_data_buffer (const _t_nodeiter nNode) const
{
#if defined (_DEBUG)
	BTREE_ASSERT (nNode < this->m_nTreeSize, "CBTreeBaseDefaults<_ti_pos, _t_data, _t_sizetype, _t_nodeiter, _t_subnodeiter, _t_datalayerproperties, _t_datalayer>::get_data_buffer: requested node exceeds tree size");
#endif

	// read all data entries of one node from IO layer
	return (m_pData->template get_pooledData<_t_data> (this->m_nPoolIDdata, nNode));
}

/*

get_serVector - get serial vector

This method retrieves the serial vector of one node and writes the result to the location
pVector is pointing at.

nNode		- specifies the node of which to retrieve the serial vector of
pVector		- specifies the pointer of the returned serial vector

Note:	The method assumes that the location pVector is pointing is big enough to
		contain all returned information.

*/

template <class _ti_pos, class _t_data, class _t_sizetype, class _t_nodeiter, class _t_subnodeiter, class _t_datalayerproperties, class _t_datalayer>
_t_sizetype* CBTreeBaseDefaults<_ti_pos, _t_data, _t_sizetype, _t_nodeiter, _t_subnodeiter, _t_datalayerproperties, _t_datalayer>::get_serVector (const _t_nodeiter nNode) const
{
#if defined (_DEBUG)
	BTREE_ASSERT (nNode < this->m_nTreeSize, "CBTreeBaseDefaults<>::get_serVector: requested node exceeds tree size");
#endif

	return (m_pData->template get_pooledData<_t_sizetype> (this->m_nPoolIDserialVector, nNode));
}

template <class _ti_pos, class _t_data, class _t_sizetype, class _t_nodeiter, class _t_subnodeiter, class _t_datalayerproperties, class _t_datalayer>
void CBTreeBaseDefaults<_ti_pos, _t_data, _t_sizetype, _t_nodeiter, _t_subnodeiter, _t_datalayerproperties, _t_datalayer>::auto_shrink_inc ()
{
	// if auto shrink is due ...
	if (this->m_nAutoShrinkCounter >= this->m_nAutoShrinkThreshold)
	{
		// ... then set back auto shrink counter and run auto shrink
		this->m_nAutoShrinkCounter -= this->m_nAutoShrinkThreshold;

		auto_shrink ();
	}
	else
	{
		// ... otherwise, increment auto shrink counter
		this->m_nAutoShrinkCounter++;
	}
}

template <class _ti_pos, class _t_data, class _t_sizetype, class _t_nodeiter, class _t_subnodeiter, class _t_datalayerproperties, class _t_datalayer>
void CBTreeBaseDefaults<_ti_pos, _t_data, _t_sizetype, _t_nodeiter, _t_subnodeiter, _t_datalayerproperties, _t_datalayer>::auto_shrink_dec ()
{
	// if auto shrink counter won't wrap on next decrement ...
	if (this->m_nAutoShrinkCounter > 0)
	{
		// ... then decrement counter
		this->m_nAutoShrinkCounter--;
	}
}

template <class _ti_pos, class _t_data, class _t_sizetype, class _t_nodeiter, class _t_subnodeiter, class _t_datalayerproperties, class _t_datalayer>
void CBTreeBaseDefaults<_ti_pos, _t_data, _t_sizetype, _t_nodeiter, _t_subnodeiter, _t_datalayerproperties, _t_datalayer>::auto_shrink ()
{
	_t_nodeiter		i;
	_t_nodeiter		nNewSize;

	// find node with highest id
	for (i = this->m_nTreeSize; i > 0; )
	{
		i--;

		if (get_reservation (i))
		{
			break;
		}
	}

	nNewSize = i;

	// unload any remaining data still present in cache
	for (i++; i < this->m_nTreeSize; i++)
	{
		m_pData->unload_from_cache (i);
	}

	// the previous step is required, since although if a node is de-allocted its data in the cache is still marked as valid
	// if another node is requested, which shares the same cache line, then the write back to of the de-allocated node's data
	// is an attempt to write to a location via the IO layer that is no longer there and will most likely result in a seg-fault
	// or hit an assert

	// update IO layer and tree size
	m_pData->set_size (nNewSize);

	this->m_nTreeSize = nNewSize + 1;
}

template <class _ti_pos, class _t_data, class _t_sizetype, class _t_nodeiter, class _t_subnodeiter, class _t_datalayerproperties, class _t_datalayer>
bool CBTreeBaseDefaults<_ti_pos, _t_data, _t_sizetype, _t_nodeiter, _t_subnodeiter, _t_datalayerproperties, _t_datalayer>::show_tree (std::ofstream &ofs, _t_nodeiter nNode, _t_nodeiter nParent, char *pAlloc) const
{
	node_t					*psNodeDesc, *psSubNodeDesc;
	uint64_t				nNumIdx;
	_t_subnodeiter			i;
	bool					bError = false;
	std::string				strError;
	_t_nodeiter				nSerVectorTest;
	_t_sizetype				*pnSerVector;

	/*
	**	If argument ofs is a stream that is not open, then this method will
	**	operate in integrity test mode and exit with false if anything is
	**	incorrect or an exception occures. The integrity test mode exits
	**	with true if everything is correct.
	**	If argument ofs is an open stream, then the node's data is output in
	**	HTML format to that stream and the result is always true. If an error
	**	happens in the process, then the data this method "thinks" is correct
	**	as well as a description of what went wrong is output to the stream.
	*/

	try
	{
		psNodeDesc = get_node (nNode);
	}
	catch (exception *pE)
	{
		strError = pE->what ();

		bError = true;
	}

	if (ofs.is_open ())
	{
		ofs << "<table cellspacing=\"0\"><tr><td>";
		ofs << "<table border=\"1\" cellspacing=\"0\">";
		ofs << "<tr align=center>" << endl;
		ofs << "<td>" << endl;
	}

	if (bError)
	{
		if (ofs.is_open ())
		{
			ofs << "<b><font color=\"#FF0000\">get_node failed on node " << nNode << "<br>Reason: " << strError << "<br><br></font></b>" << endl;
		}
		else
		{
			return (false);
		}
	}
	else if ((nNode == nParent) && (nNode != this->m_nRootNode))
	{	// if a node claims to be its own parent, then it also must be the root node, otherwise the node's state is invalid
		if (ofs.is_open ())
		{
			ofs << "<b><font color=\"#FF0000\">node claims to be root<br><br></font></b>" << endl;
		}
		else
		{
			return (false);
		}
	}
	else if (psNodeDesc->nParent != nParent)
	{	// if the node's parent state is not equal to the node it was recursively called into, then the node's state is invalid
		if (ofs.is_open ())
		{
			ofs << "<b><font color=\"#FF0000\">nodes parent state invalid<br><br></font></b>" << endl;
		}
		else
		{
			return (false);
		}
	}
	else if (pAlloc[nNode])
	{	// if this node was processed before, then the tree state is invalid
		if (ofs.is_open ())
		{
			ofs << "<b><font color=\"#FF0000\">node is linked to more than once<br><br></font></b>" << endl;
		}
		else
		{
			return (false);
		}
	}
	
	// if it was possible to call this node without any assertion (invalid states that may have been detected are being ignored) ...
	if (!bError)
	{
		// ... then process this node

		// flag this node being in use
		pAlloc[nNode] = 0x01;

		// test if nMaxIdx state is valid
		nNumIdx = get_data_count (nNode);

		if (!is_leaf (nNode))
		{
			for (i = 0L; i < get_sub_node_count (nNode); i++)
			{
				psSubNodeDesc = get_node (*(get_sub_node (nNode, i)));
				nNumIdx += psSubNodeDesc->nMaxIdx;
			}
		}

		// this node's number of data entries plus the nMaxIdx of all sub nodes combined must be equal to this node's maxIdx state
		if (nNumIdx != psNodeDesc->nMaxIdx)
		{
			if (ofs.is_open ())
			{
				ofs << "<b><font color=\"#FF0000\">nMaxIdx is inconsistent<br></font></b><br>" << endl;
			}
			else
			{
				return (false);
			}
		}
		
		if (ofs.is_open ())
		{
			ofs << "<a name=\"node_" << nNode << "\"></a>" << endl;
			ofs << "nMaxIdx: " << psNodeDesc->nMaxIdx << "<br>node: " << nNode << "<br>";
			ofs << "<a href=\"#node_" << psNodeDesc->nParent << "\">parent: " << psNodeDesc->nParent << "</a>";
		}
	}

	if (ofs.is_open ())
	{
		ofs << "</td>" << endl;
		ofs << "</tr>" << endl;
		ofs << "<tr align=center>" << endl;
		ofs << "<td>" << endl;
		ofs << "<table border=\"1\" cellspacing=\"0\">";
	}

	// only the root node is valid as an empty node
	if (get_data_count (*psNodeDesc) == 0L)
	{
		// hence, if this code path has been entered, then the current node must be the root node

		// size uses the root node's nMaxIdx to deteremine the tree size
		// if that is not zero, then the current node not the root node, which contradicts the above assumption
		// or the root node descriptor has been corrupted
		// both cases lead to the conclusion that the tree's state is invalid
		if (ofs.is_open ())
		{
			if (CBTreeBase_t::size () != 0)
			{
				ofs << "<tr align=center><td><b><font color=\"#FF0000\">no data</font></b></td></tr>" << endl;
			}
			else
			{
				ofs << "<tr align=center><td>no data</td></tr>" << endl;
			}
		}
		else
		{
			if (CBTreeBase_t::size () != 0)
			{
				return (false);
			}
		}
	}
	else
	{
		// ... otherwise this node's data must be displayed
		if (ofs.is_open ())
		{
			ofs << "<tr align=center>";

			if (!is_leaf (nNode))
			{
				ofs << "<td><font color=\"#CCCCCC\">linear offset:<br> 0 </font></td>" << endl;
			}
		}

		pnSerVector = this->get_serVector (nNode);

		nSerVectorTest = 0;

		// test or displayer node's data
		for (i = 0UL; i < get_data_count (nNode); i++)
		{
			if (ofs.is_open ())
			{
				ofs << "<td>" << endl;
			}
			
			// test or display one data item
			bool bPassed = show_node (ofs, nNode, i);

			if (ofs.is_open ())
			{
				// if show_node failed for any reason ...
				if (!bPassed)
				{
					// ... then the error message is already logged in the output stream

					// and this call returns immediately
					return (false);
				}
			}

			if (!is_leaf (nNode))
			{
				nSerVectorTest += (_t_nodeiter) this->get_max_index (*(this->get_sub_node (nNode, i)));

				bPassed = (nSerVectorTest + i) == pnSerVector[i];
			}
			else
			{
				bPassed = true;
			}

			if (ofs.is_open ())
			{
				ofs << "</td>" << endl;

				if (!is_leaf (nNode))
				{
					ofs << "<td>";

					if (!bPassed) ofs << "<font color=\"#FF0000\">";

					ofs << "linear offset:<br>" << pnSerVector[i];

					if (!bPassed) ofs << "</font>";
					
					ofs << "</td>" << endl;
				}
			}
			else if (!bPassed)
			{
				return (false);
			}
		}

		if (ofs.is_open ())
		{
			ofs << "</tr>" << endl;
		}

		// if this is not a leaf node ...
		if (!is_leaf (nNode))
		{
			// ... then all sub nodes must be test or displayed
			if (ofs.is_open ())
			{
				ofs << "<tr align=center>" << endl;
			}

			// test or display all sub nodes
			for (i = 0L; i < get_sub_node_count (nNode); i++)
			{
				if (ofs.is_open ())
				{
					ofs << "<td>" << endl;
				}

				// test or display one sub node
				bool bPassed = show_tree (ofs, *(get_sub_node (nNode, i)), nNode, pAlloc);

				if (ofs.is_open ())
				{
					// if any of the sub nodes encountered any error ...
					if (!bPassed)
					{
						// ... then the error message is already logged in the output stream

						// and this call returns immediately
						return (false);
					}
				}
			
				if (ofs.is_open ())
				{
					ofs << "</td>" << endl;

					ofs << "<td></td>" << endl;
				}
			}

			if (ofs.is_open ())
			{
				ofs << "</tr>" << endl;
			}
		}
	}

	if (ofs.is_open ())
	{
		ofs << "</table>" << endl;
		ofs << "</td>" << endl;
		ofs << "</tr>" << endl;
		ofs << "</table>" << endl;
		ofs << "</table>" << endl;
	}

	// if this call made it down here then it is either in display mode or did not find any error
	return (true);
}

template <class _ti_pos, class _t_data, class _t_sizetype, class _t_nodeiter, class _t_subnodeiter, class _t_datalayerproperties, class _t_datalayer>
void CBTreeBaseDefaults<_ti_pos, _t_data, _t_sizetype, _t_nodeiter, _t_subnodeiter, _t_datalayerproperties, _t_datalayer>::shownodelist (std::ofstream &ofs, char *pAlloc) const
{
	_t_nodeiter	ui64;
	node_t		*psNodeDesc;

	ofs << "<H1>node list</H1>" << endl;
	ofs << "<br>" << endl;
	ofs << "<table>" << endl;

	// display the entire node list - regradless if a node is in use or not
	for (ui64 = 0; ui64 < this->m_nTreeSize; ui64++)
	{
		bool	bInUse = get_reservation (ui64);
		
		psNodeDesc = get_node (ui64);
		
		ofs << "<tr>" << endl;
		ofs << "<td>" << endl;
		ofs << "<a name=\"node_list_" << ui64 << "\">";
		ofs << "node: " << ui64;
		ofs << "</a>" << endl;
		ofs << "</td>" << endl;
		ofs << "<td><a href=\"#node_" << ui64 << "\">tree</a></td>" << endl;
		ofs << "<td><a href=\"#node_dump_" << ui64 << "\">dump</a></td>" << endl;
		ofs << "</tr>" << endl;
		ofs << "<tr>" << endl;
		ofs << "<td></td><td>" << endl;

		if (!bInUse)
		{
			ofs << "<font color=\"#CCCCCC\">";
		}

		ofs << "<a href=\"#node_list_" << psNodeDesc->nParent << "\">";
		ofs << "parent: " << psNodeDesc->nParent;
		ofs << "</a>" << endl;
		
		if (!bInUse)
		{
			ofs << "</font>";
		}

		ofs << "</td>" << endl;
		ofs << "<td>" << endl;
		
		if (!bInUse)
		{
			ofs << "<font color=\"#CCCCCC\">";
		}

		ofs << "nNumData: " << psNodeDesc->nNumData << endl;
		
		if (!bInUse)
		{
			ofs << "</font>";
		}

		ofs << "</td>" << endl;

		ofs << "<td>" << endl;
		
		if (!bInUse)
		{
			ofs << "<font color=\"#CCCCCC\">";
		}

		ofs << "nMaxIdx: " << psNodeDesc->nMaxIdx << endl;
		
		if (!bInUse)
		{
			ofs << "</font>";
		}

		ofs << "</td>" << endl;

		ofs << "</tr>" << endl;
	}

	ofs << "</table>" << endl;
}

template <class _ti_pos, class _t_data, class _t_sizetype, class _t_nodeiter, class _t_subnodeiter, class _t_datalayerproperties, class _t_datalayer>
void CBTreeBaseDefaults<_ti_pos, _t_data, _t_sizetype, _t_nodeiter, _t_subnodeiter, _t_datalayerproperties, _t_datalayer>::_swap
	(
		CBTreeBaseDefaults<_ti_pos, _t_data, _t_sizetype, _t_nodeiter, _t_subnodeiter, _t_datalayerproperties, _t_datalayer> &rBT
	)
{
	fast_swap<_t_datalayer *> (this->m_pData, rBT.m_pData);
	fast_swap<_t_datalayerproperties *> (this->m_pClDataLayerProperties, rBT.m_pClDataLayerProperties);

	CBTreeBaseIf_t	&rLowLevel = dynamic_cast <CBTreeBaseIf_t &> (rBT);

	CBTreeBaseIf_t::_swap (rLowLevel);
}

#endif // BTREEBASEDEFAULTS_CPP
