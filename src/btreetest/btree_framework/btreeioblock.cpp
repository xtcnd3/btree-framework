#ifndef	BTREEIOBLOCK_CPP
#define	BTREEIOBLOCK_CPP

#include "btreeioblock.h"

// block [0]
// |- node desciption[0]         -|- maintainence vector-|-algnm-|--- data[0][] ---|-algnm-|--- sub node numbers[0][] ---|-algnm-|--- serVector[0][] ---|-algnm-|	<-- node[0]
// |- node desciption[1]         -|- maintainence vector-|-algnm-|--- data[1][] ---|-algnm-|--- sub node numbers[1][] ---|-algnm-|--- serVector[1][] ---|-algnm-|	<-- node[1]
// |- node desciption[2]         -|- maintainence vector-|...
// ...
// |- node desciption[(2^n) - 1] -|- maintainence vector-|...
// data layer block alignment
// block [1]
// |- node desciption[2^n]       -|- maintainence vector-|-algnm-|--- data[0][] ---|-algnm-|--- sub node numbers[0][] ---|-algnm-|--- serVector[0][] ---|-algnm-|	<-- node[0]

template <class _t_nodeiter, class _t_subnodeiter, class _t_addresstype, class _t_offsettype>
CBTreeBlockIO<_t_nodeiter, _t_subnodeiter, _t_addresstype, _t_offsettype>::CBTreeBlockIO
(
	_t_addresstype nBlockSize, 
	_t_subnodeiter nNodeSize, 
	uint32_t nNumDataPools, 
	CBTreeIOperBlockPoolDesc_t *psDataPools
)
	:	CBTreeIO<_t_nodeiter, _t_subnodeiter, _t_addresstype, _t_offsettype> (nBlockSize, nNumDataPools, psDataPools)
	,	m_nNodeSize (nNodeSize)
	,	m_pnPerNodePoolOffset (NULL)
	,	m_psDescriptorVector (NULL)
	,	m_nDescriptorVectorSize (0)
	,	m_nPrevRootNode (~0)
	,	m_nMaxAccessCtr (127)
{
	m_pnPerNodePoolOffset = new _t_offsettype [this->m_nNumDataPools];

	BTREEDATA_ASSERT (m_pnPerNodePoolOffset != NULL, "CBTreeIO::CBTreeIO: insufficient memory!");
	
	m_nAlignedNodeSize = generate_pool_offsets ();
}

template <class _t_nodeiter, class _t_subnodeiter, class _t_addresstype, class _t_offsettype>
CBTreeBlockIO<_t_nodeiter, _t_subnodeiter, _t_addresstype, _t_offsettype>::~CBTreeBlockIO ()
{
	if (m_pnPerNodePoolOffset != NULL)
	{
		delete [] m_pnPerNodePoolOffset;
	}

	if (m_psDescriptorVector != NULL)
	{
		free ((void *) m_psDescriptorVector);
	}
}

template <class _t_nodeiter, class _t_subnodeiter, class _t_addresstype, class _t_offsettype>
_t_nodeiter CBTreeBlockIO<_t_nodeiter, _t_subnodeiter, _t_addresstype, _t_offsettype>::get_maxNodes ()
{
	return (this->m_nMaxNodes);
}

template <class _t_nodeiter, class _t_subnodeiter, class _t_addresstype, class _t_offsettype>
_t_offsettype CBTreeBlockIO<_t_nodeiter, _t_subnodeiter, _t_addresstype, _t_offsettype>::get_dataAlignment ()
{
	// The keyword 'long' has been purposely choosen, since it will make the compiler to return 4 bytes on 32 bit builds and 8 bytes on 64 bit builds, as oppose to uint32_t and uint64_t.
	return ((_t_offsettype) (sizeof (long)));
}

template <class _t_nodeiter, class _t_subnodeiter, class _t_addresstype, class _t_offsettype>
_t_offsettype CBTreeBlockIO<_t_nodeiter, _t_subnodeiter, _t_addresstype, _t_offsettype>::get_alignedOffset (_t_offsettype nOffset)
{
	nOffset += get_dataAlignment () - 1;
	nOffset /= get_dataAlignment ();
	nOffset *= get_dataAlignment ();

	return (nOffset);
}

template <class _t_nodeiter, class _t_subnodeiter, class _t_addresstype, class _t_offsettype>
_t_offsettype CBTreeBlockIO<_t_nodeiter, _t_subnodeiter, _t_addresstype, _t_offsettype>::get_per_node_pool_offset (uint32_t nPool)
{
#if defined (_DEBUG)

	BTREEDATA_ASSERT (nPool < this->m_nNumDataPools, "CBTreeBlockIO::get_per_node_pool_offset: nPool exceeds available block pools!");

#endif

	return (m_pnPerNodePoolOffset[nPool]);
}

template <class _t_nodeiter, class _t_subnodeiter, class _t_addresstype, class _t_offsettype>
_t_addresstype CBTreeBlockIO<_t_nodeiter, _t_subnodeiter, _t_addresstype, _t_offsettype>::get_pool_address (uint32_t nPool, _t_nodeiter nNode)
{
	_t_addresstype		nAddr = get_nodeAddr (nNode);

	nAddr += get_per_node_pool_offset (nPool);

	return (nAddr);
}

template <class _t_nodeiter, class _t_subnodeiter, class _t_addresstype, class _t_offsettype>
void CBTreeBlockIO<_t_nodeiter, _t_subnodeiter, _t_addresstype, _t_offsettype>::set_root_node (_t_nodeiter nRootNode)
{
	uint32_t		nCurrentRootContainingDesc;
	uint32_t		nNewRootContainingDesc = convert_node_to_descriptor (nRootNode);

	if (m_nPrevRootNode != ~0)
	{
		nCurrentRootContainingDesc = convert_node_to_descriptor (m_nPrevRootNode);
	}
	else
	{
		nCurrentRootContainingDesc = ~nNewRootContainingDesc;
	}

	if (nCurrentRootContainingDesc != nNewRootContainingDesc)
	{
#if defined (_DEBUG)

		BTREE_ASSERT (nNewRootContainingDesc < m_nDescriptorVectorSize, "ERROR: CBTreeBlockIO<_t_nodeiter, _t_subnodeiter, _t_addresstype, _t_offsettype>::set_root_node: requested new root exceeds descriptor vector size!");

#endif

		m_nPrevRootNode = nRootNode;
	}
}

template <class _t_nodeiter, class _t_subnodeiter, class _t_addresstype, class _t_offsettype>
_t_addresstype CBTreeBlockIO<_t_nodeiter, _t_subnodeiter, _t_addresstype, _t_offsettype>::get_blockAddr (_t_nodeiter nNode)
{
	_t_addresstype		nRetval;

	nRetval = (_t_addresstype) ((nNode >> m_nNodesPerBlockVectorSize) * m_nBlockSize);

	return (nRetval);
}

template <class _t_nodeiter, class _t_subnodeiter, class _t_addresstype, class _t_offsettype>
_t_offsettype CBTreeBlockIO<_t_nodeiter, _t_subnodeiter, _t_addresstype, _t_offsettype>::get_poolOffset ()
{
	return ((_t_offsettype) 0);
}

template <class _t_nodeiter, class _t_subnodeiter, class _t_addresstype, class _t_offsettype>
_t_addresstype CBTreeBlockIO<_t_nodeiter, _t_subnodeiter, _t_addresstype, _t_offsettype>::get_nodeAddr (_t_nodeiter nNode)
{
	_t_addresstype		nRetval;

	nRetval = get_blockAddr (nNode);
	nRetval += (nNode & (m_nNodesPerBlock - 1)) * m_nAlignedNodeSize;

	return (nRetval);
}

template <class _t_nodeiter, class _t_subnodeiter, class _t_addresstype, class _t_offsettype>
_t_offsettype CBTreeBlockIO<_t_nodeiter, _t_subnodeiter, _t_addresstype, _t_offsettype>::generate_pool_offsets ()
{
	_t_offsettype	nBase = this->get_poolOffset ();
	uint32_t		ui32;

	nBase = get_alignedOffset (nBase);

	for (ui32 = 0; ui32 < this->m_nNumDataPools; ui32++)
	{
		m_pnPerNodePoolOffset[ui32] = nBase;

		nBase += this->get_pool_total_size (ui32);

		nBase = get_alignedOffset (nBase);
	}

	return (nBase);
}

template<class _t_nodeiter, class _t_subnodeiter, class _t_addresstype, class _t_offsettype>
void CBTreeBlockIO<_t_nodeiter, _t_subnodeiter, _t_addresstype, _t_offsettype>::realloc_descriptor_vector (_t_nodeiter nMaxNodes)
{
	uint32_t	nOldDescriptorVectorSize = m_nDescriptorVectorSize;
	uint32_t	nNewDescriptorVectorSize;
	uint32_t	u;

	nNewDescriptorVectorSize = (uint32_t) ((nMaxNodes + m_nNodesPerBlock - 1) >> m_nNodesPerBlockVectorSize);

	if (m_psDescriptorVector == NULL)
	{
#if defined (_DEBUG)

		BTREE_ASSERT (m_nDescriptorVectorSize == 0, "ERROR: CBTreeBlockIO::realloc_descriptor_vector: on descriptor vector initialisation, vector size ought to be zero length!");

#endif

		m_psDescriptorVector = (btree_block_io_descriptor_t *) malloc (nNewDescriptorVectorSize * sizeof (btree_block_io_descriptor_t));

		BTREE_ASSERT (m_psDescriptorVector != NULL, "ERROR: CBTreeBlockIO::realloc_descriptor_vector: insufficient memory!");
	}
	else if (nNewDescriptorVectorSize != nOldDescriptorVectorSize)
	{
		m_psDescriptorVector = (btree_block_io_descriptor_t *) realloc ((void *) m_psDescriptorVector, nNewDescriptorVectorSize * sizeof (btree_block_io_descriptor_t));

		BTREE_ASSERT (m_psDescriptorVector != NULL, "ERROR: CBTreeBlockIO::realloc_descriptor_vector: insufficient memory! (realloc)");
	}

	for (u = nOldDescriptorVectorSize; u < nNewDescriptorVectorSize; u++)
	{
		m_psDescriptorVector[u].pBlockData = NULL;
		m_psDescriptorVector[u].nAccessCtr = 0;
	}

	m_nDescriptorVectorSize = nNewDescriptorVectorSize;
}

template<class _t_nodeiter, class _t_subnodeiter, class _t_addresstype, class _t_offsettype>
uint32_t CBTreeBlockIO<_t_nodeiter, _t_subnodeiter, _t_addresstype, _t_offsettype>::convert_node_to_descriptor (_t_nodeiter nNode, bool bRoundUp)
{
#if defined (_DEBUG)

	const uint32_t		uMask = ~0;
	_t_nodeiter			uRevNodeMask = (_t_nodeiter) uMask;

	uRevNodeMask = ~uRevNodeMask;

#endif

	uint32_t			nRslt;

	nNode >>= this->m_nNodesPerBlockVectorSize;

#if defined (_DEBUG)

	BTREE_ASSERT ((nNode & uRevNodeMask) == 0x0, "ERROR: CBTreeBlockIO<_t_nodeiter, _t_subnodeiter, _t_addresstype, _t_offsettype>::convert_node_to_descriptor: node value exceeds maximum descriptor vector size!");

#endif

	nRslt = (uint32_t) nNode;

	if (bRoundUp)
	{
		nRslt++;

#if defined (_DEBUG)

		BTREE_ASSERT (nRslt != 0, "ERROR: CBTreeBlockIO<_t_nodeiter, _t_subnodeiter, _t_addresstype, _t_offsettype>::convert_node_to_descriptor: requested size exceeds maximum descriptor vector size!");

#endif
	}

	return (nRslt);
}

template<class _t_nodeiter, class _t_subnodeiter, class _t_addresstype, class _t_offsettype>
void CBTreeBlockIO<_t_nodeiter, _t_subnodeiter, _t_addresstype, _t_offsettype>::setup (_t_addresstype nBlockSize)
{
	uint32_t	nMultiple = 1;
	uint32_t	ui32;

	BTREE_ASSERT (nBlockSize > 0, "ERROR: CBTreeBlockIO<_t_nodeiter, _t_subnodeiter, _t_addresstype, _t_offsettype>::setup: desired minimum block is zero!");

	m_nBlockSize = nBlockSize * nMultiple;

	while ((nMultiple * nBlockSize) < generate_pool_offsets ())
	{
		nMultiple++;

		m_nBlockSize = nBlockSize * nMultiple;
	}

	m_nNodesPerBlock = (uint32_t) (m_nBlockSize / generate_pool_offsets ());

	// make sure m_nNodesPerBlock is part of the 2^n series
	for (ui32 = 32; ui32 != 0x0; )
	{
		ui32--;

		if ((m_nNodesPerBlock & (1 << ui32)) == (1 << ui32))
		{
			m_nNodesPerBlock = 1 << ui32;
			m_nNodesPerBlockVectorSize = ui32;

			break;
		}
	}
}

template<class _t_nodeiter, class _t_subnodeiter, class _t_addresstype, class _t_offsettype>
void CBTreeBlockIO<_t_nodeiter, _t_subnodeiter, _t_addresstype, _t_offsettype>::increment_access_counter (uint32_t nDescriptor)
{
	this->m_psDescriptorVector[nDescriptor].nAccessCtr++;

	if (this->m_psDescriptorVector[nDescriptor].nAccessCtr >= this->m_nMaxAccessCtr)
	{
		uint32_t	ui32;

		for (ui32 = 0; ui32 < this->m_nDescriptorVectorSize; ui32++)
		{
			this->m_psDescriptorVector[nDescriptor].nAccessCtr >>= 1;
		}
	}
}

#endif // BTREEIOBLOCK_CPP
