/************************************************************
**
** file:	btreeio.cpp
** author:	Andreas Steffens
** license:	GPL v2
**
** description:
**
** This file contains code for the b-tree framework's
** abstract data layer class.
**
************************************************************/

#ifndef	BTREEIO_CPP
#define	BTREEIO_CPP

#include "btreeio.h"

using namespace std;

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
CBTreeIO<_t_nodeiter, _t_subnodeiter, _t_addresstype, _t_offsettype>::CBTreeIO
(
	uint32_t nBlockSize, 
	_t_subnodeiter nNodeSize, 
	uint32_t nNumDataPools, 
	CBTreeIOperBlockPoolDesc_t *psDataPools
)
	:	m_nNumDataPools (nNumDataPools)
	,	m_bCacheFreeze (false)
	,	m_bWriteThrough (false)
	,	m_nNodeSize (nNodeSize)
	,	m_nMaxNodes (0ULL)
{
	uint32_t			ui32;

	m_psDataPools = new CBTreeIOperBlockPoolDesc_t [m_nNumDataPools];

	BTREEDATA_ASSERT (m_psDataPools != NULL, "CBTreeIO::CBTreeIO: insufficient memory!");

	for (ui32 = 0; ui32 < m_nNumDataPools; ui32++)
	{
		m_psDataPools[ui32] = psDataPools[ui32];
	}

	m_pnPerBlockPoolOffset = new _t_offsettype [m_nNumDataPools];

	BTREEDATA_ASSERT (m_pnPerBlockPoolOffset != NULL, "CBTreeIO::CBTreeIO: insufficient memory!");
	
	m_nAlignedNodeSize = get_alignedDataOffsets ();

#if defined (USE_PERFORMANCE_COUNTERS)
	memset ((void *) &m_anHitCtrs, 0, sizeof (m_anHitCtrs));
	memset ((void *) &m_anMissCtrs, 0, sizeof (m_anMissCtrs));
#endif

#if defined (_DEBUG)

//	print_addresses ();

#endif
}

template <class _t_nodeiter, class _t_subnodeiter, class _t_addresstype, class _t_offsettype>
CBTreeIO<_t_nodeiter, _t_subnodeiter, _t_addresstype, _t_offsettype>::~CBTreeIO ()
{
	delete [] m_psDataPools;

	delete [] m_pnPerBlockPoolOffset;
}

template <class _t_nodeiter, class _t_subnodeiter, class _t_addresstype, class _t_offsettype>
_t_nodeiter CBTreeIO<_t_nodeiter, _t_subnodeiter, _t_addresstype, _t_offsettype>::get_maxNodes ()
{
	return (m_nMaxNodes);
}

template <class _t_nodeiter, class _t_subnodeiter, class _t_addresstype, class _t_offsettype>
_t_offsettype CBTreeIO<_t_nodeiter, _t_subnodeiter, _t_addresstype, _t_offsettype>::get_dataAlignment ()
{
	// The keyword 'long' has been purposely choosen, since it will make the compiler to return 4 bytes on 32 bit builds and 8 bytes on 64 bit builds, as oppose to uint32_t and uint64_t.
	return ((_t_offsettype) (sizeof (long)));
}

template <class _t_nodeiter, class _t_subnodeiter, class _t_addresstype, class _t_offsettype>
_t_offsettype CBTreeIO<_t_nodeiter, _t_subnodeiter, _t_addresstype, _t_offsettype>::get_alignedOffset (_t_offsettype nOffset)
{
	nOffset += get_dataAlignment () - 1;
	nOffset /= get_dataAlignment ();
	nOffset *= get_dataAlignment ();

	return (nOffset);
}

template <class _t_nodeiter, class _t_subnodeiter, class _t_addresstype, class _t_offsettype>
void CBTreeIO<_t_nodeiter, _t_subnodeiter, _t_addresstype, _t_offsettype>::get_performance_counters (uint64_t (&rHitCtrs)[PERFCTR_TERMINATOR], uint64_t (&rMissCtrs)[PERFCTR_TERMINATOR])
{
#if defined (USE_PERFORMANCE_COUNTERS)
	memcpy ((void *) &rHitCtrs, (void *) m_anHitCtrs, sizeof (m_anHitCtrs));
	memcpy ((void *) &rMissCtrs, (void *) m_anMissCtrs, sizeof (m_anMissCtrs));

	switch (m_ePresentationMode)
	{
	case presentationMODE_FILE		:	m_pFile->getPerfCtrs (rHitCtrs[PERFCTR_FILE_CACHE], rMissCtrs[PERFCTR_FILE_CACHE]);

										break;
	case presentationMODE_MEMORY	:	
										break;

	default							:	BTREEDATA_ASSERT (false, "CBTreeIO::get_performance_counters: bayer tree presentation mode not set or corrupted");

										break;
	}

#else
	memset ((void *) &rHitCtrs, 0, sizeof (rHitCtrs));
	memset ((void *) &rMissCtrs, 0, sizeof (rMissCtrs));
#endif
}

template <class _t_nodeiter, class _t_subnodeiter, class _t_addresstype, class _t_offsettype>
uint32_t CBTreeIO<_t_nodeiter, _t_subnodeiter, _t_addresstype, _t_offsettype>::get_perBlockPoolTotalBlockSize (uint32_t nPool)
{
#if defined (_DEBUG)
	BTREEDATA_ASSERT (nPool < m_nNumDataPools, "CBTreeIO::get_perBlockPoolTotalBlockSize: nPool exceeds available block pools!");
#endif

	return (m_psDataPools[nPool].nTotalSize);
}

template <class _t_nodeiter, class _t_subnodeiter, class _t_addresstype, class _t_offsettype>
uint32_t CBTreeIO<_t_nodeiter, _t_subnodeiter, _t_addresstype, _t_offsettype>::get_perBlockPoolRawEntrySize (uint32_t nPool)
{
#if defined (_DEBUG)
	BTREEDATA_ASSERT (nPool < m_nNumDataPools, "CBTreeIO::get_perBlockPoolRawEntrySize: nPool exceeds available block pools!");
#endif

	return (m_psDataPools[nPool].nEntrySize);
}

template <class _t_nodeiter, class _t_subnodeiter, class _t_addresstype, class _t_offsettype>
_t_offsettype CBTreeIO<_t_nodeiter, _t_subnodeiter, _t_addresstype, _t_offsettype>::get_perBlockPoolOffset (uint32_t nPool)
{
#if defined (_DEBUG)
	BTREEDATA_ASSERT (nPool < m_nNumDataPools, "CBTreeIO::get_perBlockPoolOffset: nPool exceeds available block pools!");
#endif

	return (m_pnPerBlockPoolOffset[nPool]);
}

template <class _t_nodeiter, class _t_subnodeiter, class _t_addresstype, class _t_offsettype>
_t_addresstype CBTreeIO<_t_nodeiter, _t_subnodeiter, _t_addresstype, _t_offsettype>::get_perBlockPoolAddr (uint32_t nPool, _t_nodeiter nNode)
{
#if defined (_DEBUG)
	BTREEDATA_ASSERT (nPool < m_nNumDataPools, "CBTreeIO::get_perBlockPoolAddr: nPool exceeds available block pools!");
#endif

	_t_addresstype		nAddr = get_nodeAddr (nNode);

	nAddr += get_perBlockPoolOffset (nPool);

	return (nAddr);
}

template <class _t_nodeiter, class _t_subnodeiter, class _t_addresstype, class _t_offsettype>
void CBTreeIO<_t_nodeiter, _t_subnodeiter, _t_addresstype, _t_offsettype>::set_cacheFreeze (bool bEnabled)
{
	m_bCacheFreeze = bEnabled;
}

template <class _t_nodeiter, class _t_subnodeiter, class _t_addresstype, class _t_offsettype>
uint32_t CBTreeIO<_t_nodeiter, _t_subnodeiter, _t_addresstype, _t_offsettype>::get_node_buffer_cache_size ()
{
	return (0);
}
/*
template <class _t_nodeiter, class _t_subnodeiter, class _t_addresstype, class _t_offsettype>
void CBTreeIO<_t_nodeiter, _t_subnodeiter, _t_addresstype, _t_offsettype>::showdump	(std::ofstream &ofs, _t_nodeiter nTreeSize, char *pAlloc)
{
	_t_nodeiter		ui64;
	uint32_t		ui32, uj32;

	set_cacheFreeze (true);
	{
		ofs << "<H1>data dump</H1>" << endl;
		ofs << "<br>" << endl;

		ofs << "m_nBlockSize: " << m_nBlockSize << " (0x" << hex << m_nBlockSize << dec << ")<br>" << endl;
		ofs << "m_nNodeSize: " << m_nNodeSize << " (0x" << hex << m_nNodeSize << dec << ")<br>" << endl;
		ofs << "m_nNodesPerBlock: " << m_nNodesPerBlock << " (0x" << hex << m_nNodesPerBlock << dec << ")<br>" << endl;
		ofs << "m_nNodesPerBlockVectorSize: " << m_nNodesPerBlockVectorSize << " (0x" << hex << m_nNodesPerBlockVectorSize << dec << ")<br>" << endl;
		ofs << "m_nAlignedNodeSize: " << m_nAlignedNodeSize << " (0x" << hex << m_nAlignedNodeSize << dec << ")<br>" << endl;
		
		ofs << "<br>" << endl;
		
		ofs << "m_nNumDataPools: " << m_nNumDataPools << "<br>" << endl;
		
		ofs << "<table>" << endl;
		
		ofs << "<tr>" << endl;
		ofs << "<td></td>" << endl;

		for (ui32 = 0; ui32 < m_nNumDataPools; ui32++)
		{
			ofs << "<td>Pool " << ui32 << "</td>" << endl;
		}

		ofs << "</tr>" << endl;

		ofs << "<tr>" << endl;
		ofs << "<td>m_psDataPools->nCacheVectorSize</td>" << endl;

		for (ui32 = 0; ui32 < m_nNumDataPools; ui32++)
		{
			ofs << "<td>" << m_psDataPools[ui32].nCacheVectorSize << "</td>" << endl;
		}

		ofs << "</tr>" << endl;
		
		ofs << "<tr>" << endl;
		ofs << "<td>m_psDataPools->nEntrySize</td>" << endl;

		for (ui32 = 0; ui32 < m_nNumDataPools; ui32++)
		{
			ofs << "<td>" << m_psDataPools[ui32].nEntrySize << "</td>" << endl;
		}

		ofs << "</tr>" << endl;

		ofs << "<tr>" << endl;
		ofs << "<td>m_psDataPools->nTotalSize</td>" << endl;

		for (ui32 = 0; ui32 < m_nNumDataPools; ui32++)
		{
			ofs << "<td>" << m_psDataPools[ui32].nTotalSize << "</td>" << endl;
		}

		ofs << "</tr>" << endl;
		
		ofs << "<tr>" << endl;
		ofs << "<td>m_pnPerBlockPoolCacheMaskes</td>" << endl;

		for (ui32 = 0; ui32 < m_nNumDataPools; ui32++)
		{
			ofs << "<td>0x" << hex << m_pnPerBlockPoolCacheMaskes[ui32] << dec << "</td>" << endl;
		}

		ofs << "</tr>" << endl;

		ofs << "<tr>" << endl;
		ofs << "<td>m_pnPerBlockPoolOffset</td>" << endl;

		for (ui32 = 0; ui32 < m_nNumDataPools; ui32++)
		{
			ofs << "<td>" << m_pnPerBlockPoolOffset[ui32] << "</td>" << endl;
		}

		ofs << "</tr>" << endl;
		
		ofs << "</table>" << endl;

		ofs << "<table>" << endl;

		for (ui64 = 0; ui64 < nTreeSize; )
		{
			for (ui32 = 0; ui32 < (nTreeSize / m_nNodesPerBlock); ui32++)
			{
				ofs << "<tr>" << endl;
				ofs << "<td>" << endl;

				ofs << "block: " << ui32 << endl;

				ofs << "</td><td></td>" << endl;
				ofs << "</tr>" << endl;

				for (uj32 = 0; uj32 < (1U << m_nNodesPerBlockVectorSize); uj32++, ui64++)
				{
					ofs << "<tr>" << endl;
					ofs << "<td></td><td>" << endl;

					ofs << "node: " << ui64 << endl;

					ofs << "</td><td></td>" << endl;
					ofs << "</tr>" << endl;

					{
						ofs << "<tr>" << endl;

						ofs << "<td></td><td></td><td>" << endl;

						uint32_t	uk32;

						for (uk32 = 0; uk32 < m_nNumDataPools; uk32++)
						{
							_t_addresstype	nAddr = get_perBlockPoolAddr (uk32, ui64);
							bool			bIsCached = is_dataCached (uk32, ui64);

							if (m_pMemory != NULL)
							{
								bIsCached = true;
							}

							ofs << "<td>" << endl;
							
							if (!bIsCached)
							{
								ofs << "<font color=\"#999999\">";
							}

							ofs << nAddr << ":" << endl;
							
							if (!bIsCached)
							{
								ofs << "</font>";
							}

							ofs << "</td>" << endl;

							ofs << "<td>" << endl;

							if (bIsCached)
							{
								uint32_t	ul32;
								uint8_t		*pb;
								uint32_t	nMaxPoolLen;

								ofs << "<font color=\"#4040FF\">";

								nMaxPoolLen = get_perBlockPoolTotalBlockSize (uk32);

								pb = new uint8_t [nMaxPoolLen];

								BTREEDATA_ASSERT (pb != NULL, "CBTreeIO<_t_nodeiter, _t_subnodeiter, _t_addresstype, _t_offsettype>::showdump: insufficient memory!");

								get_pooledData (uk32, ui64, nMaxPoolLen / get_perBlockPoolRawEntrySize (uk32), (void *) pb);
							
								for (ul32 = 0; ul32 < nMaxPoolLen; ul32++)
								{
									ofs << hex << setfill ('0') << setw (2) << (uint32_t) pb[ul32] << dec;

									if (((ul32 + 1) % get_perBlockPoolRawEntrySize (uk32)) == 0)
									{
										ofs << "<br>" << endl;
									}
									else if (((ul32 + 1) % 4) == 0)
									{
										ofs << " ";
									}
								}

								delete [] pb;

								ofs << "</font>";

								ofs << "<br>";
							}
							
							uint32_t	ul32;
							uint8_t		b;

							if (m_pFile != NULL)
							{
								ofs << "<font color=\"#999999\">";
								
								for (ul32 = 0; ul32 < get_perBlockPoolTotalBlockSize (uk32); ul32++)
								{
									m_pFile->memcpy ((void *) &b, nAddr + ul32, 1ULL);

									ofs << hex << setfill ('0') << setw (2) << (uint32_t) b << dec;

									if (((ul32 + 1) % get_perBlockPoolRawEntrySize (uk32)) == 0)
									{
										ofs << "<br>";
									}
									else if (((ul32 + 1) % 4) == 0)
									{
										ofs << " ";
									}
								}

								ofs << "</font>";
							}
							
							ofs << "</td>";
						}

						ofs << "</tr>" << endl;
					}
				}
			}
		}

		ofs << "</table>" << endl;
	}
	set_cacheFreeze (false);
}
*/
template <class _t_nodeiter, class _t_subnodeiter, class _t_addresstype, class _t_offsettype>
_t_addresstype CBTreeIO<_t_nodeiter, _t_subnodeiter, _t_addresstype, _t_offsettype>::get_blockAddr (_t_nodeiter nNode)
{
	_t_addresstype		nRetval;

	nRetval = (_t_addresstype) ((nNode >> m_nNodesPerBlockVectorSize) * m_nBlockSize);

	return (nRetval);
}

template <class _t_nodeiter, class _t_subnodeiter, class _t_addresstype, class _t_offsettype>
_t_offsettype CBTreeIO<_t_nodeiter, _t_subnodeiter, _t_addresstype, _t_offsettype>::get_poolOffset ()
{
	return ((_t_offsettype) 0);
}

template <class _t_nodeiter, class _t_subnodeiter, class _t_addresstype, class _t_offsettype>
_t_addresstype CBTreeIO<_t_nodeiter, _t_subnodeiter, _t_addresstype, _t_offsettype>::get_nodeAddr (_t_nodeiter nNode)
{
	_t_addresstype		nRetval;

	nRetval = get_blockAddr (nNode);
	nRetval += (nNode & (m_nNodesPerBlock - 1)) * m_nAlignedNodeSize;

	return (nRetval);
}

template <class _t_nodeiter, class _t_subnodeiter, class _t_addresstype, class _t_offsettype>
_t_offsettype CBTreeIO<_t_nodeiter, _t_subnodeiter, _t_addresstype, _t_offsettype>::get_alignedDataOffsets ()
{
	_t_offsettype	nBase = get_poolOffset ();
	uint32_t		ui32;

	nBase = get_alignedOffset (nBase);

	for (ui32 = 0; ui32 < m_nNumDataPools; ui32++)
	{
		m_pnPerBlockPoolOffset[ui32] = nBase;

		nBase += get_perBlockPoolTotalBlockSize (ui32);

		nBase = get_alignedOffset (nBase);
	}

	return (nBase);
}

template<class _t_nodeiter, class _t_subnodeiter, class _t_addresstype, class _t_offsettype>
void CBTreeIO<_t_nodeiter, _t_subnodeiter, _t_addresstype, _t_offsettype>::set_writeThrough (bool bWriteThrough)
{
	m_bWriteThrough = bWriteThrough;
}

template<class _t_nodeiter, class _t_subnodeiter, class _t_addresstype, class _t_offsettype>
void CBTreeIO<_t_nodeiter, _t_subnodeiter, _t_addresstype, _t_offsettype>::setup (uint32_t nBlockSize)
{
	uint32_t	nMultiple = 1;
	uint32_t	ui32;

	m_nBlockSize = nBlockSize * nMultiple;

	while ((nMultiple * nBlockSize) < get_alignedDataOffsets ())
	{
		nMultiple++;

		m_nBlockSize = nBlockSize * nMultiple;
	}

	m_nNodesPerBlock = m_nBlockSize / get_alignedDataOffsets ();

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
void CBTreeIO<_t_nodeiter, _t_subnodeiter, _t_addresstype, _t_offsettype>::print_addresses ()
{
	uint32_t	u;

	cout << endl;
	cout << "              " << cout.width (8) << cout.fill (' ') << get_perBlockPoolTotalBlockSize (0) << " " << get_perBlockPoolTotalBlockSize (1) << endl;

	for (u = 0; u < 128; u++)
	{
		cout << cout.width (3) << u << ": " << cout.width (8) << get_nodeAddr (u) << " " << get_perBlockPoolAddr (0, u) << " " << get_perBlockPoolAddr (1, u) << endl;
	}

	cout.width (0);
}

#endif // BTREEIO_CPP
