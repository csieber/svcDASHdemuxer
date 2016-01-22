#define __STDC_FORMAT_MACROS

#include "NALFileStream.h"

#include <stdio.h>
#include <inttypes.h>
#include "../h264Error.hpp"

namespace H264 {

NALFileStream::NALFileStream()
{
	m_pFile    = NULL;
	m_iFileSize = 0;
}

NALFileStream::~NALFileStream()
{
	if (m_pFile != NULL) {

		fclose(m_pFile);
		m_pFile = NULL;
	}

}

/* ==========
 * Public
 * ==========
 */

/**
 * Opens a file for NAL reading.
 *
 * \param sFile Path of file to open.
 * \return success/failure
 */
bool NALFileStream::open(std::string sFile)
{
    m_pFile = fopen64(sFile.c_str(), "rb");

    if (m_pFile == NULL) {
        return false;
    }

    m_sFile = sFile;

	return true;
}

bool NALFileStream::close()
{
	if (!m_pFile)
		return true;

	fclose(m_pFile);

	m_pFile = NULL;

	return true;
}

bool NALFileStream::feature(feature_t feature)
{
	switch (feature) {
		case SEEKABLE_BACKWARD:
			return true;
		break;
		case SEEKABLE_FORWARD:
			return true;
		break;
		case INDEXABLE:
			return true;
		break;
		default:
			return false;
	}

	return false;
}

bool NALFileStream::rewind()
{
	fseek(m_pFile, 0L, SEEK_SET);
	m_iPosNAL = 0;

	return true;
}

bool NALFileStream::createIndex()
{
	const int32_t buff_size = 64000;

	char*   tmp      	 = new char[buff_size];
	int64_t iPos     	 = 0;
	int32_t iRead 	     = 0;
	int32_t iPrefixState = 0;
	int32_t iNALCounter  = 0;

	if (!rewind())
		return false;

	while (!feof(m_pFile)) {

		iRead = fread(tmp, 1, buff_size, m_pFile);

		for (int i = 0; i < iRead; i++) {

			if (tmp[i] == 0x00) {

				iPrefixState++;

		        if (iPrefixState > 3) {

		        	delete[] tmp;

		        	h264NALInvalidNALStream e;

		        	BOOST_THROW_EXCEPTION(e  << errinfo_message("prefix state > 3"));
		        }

			} else if (iPrefixState == 3 && tmp[i] == 0x01) {

				iPrefixState = 0;

				m_NALIndex[iNALCounter] = iPos + i - 3;

		        iNALCounter++;

			} else {
				iPrefixState = 0;
		    }
		}

		iPos += iRead;
	}

	m_iFileSize = iPos;

	delete[] tmp;

	return iNALCounter == 0 ? false : true;
}

bool NALFileStream::loadIndex()
{
	m_NALIndex.clear();

	FILE* pFile 	= NULL;
	int32_t iNAL 	= 0;
	int64_t iNALPos = 0;

	std::string sIndexFile = m_sFile + ".index";

	pFile = fopen(sIndexFile.c_str(), "r");

	if (pFile == NULL)
		return false;

	if (feof(pFile))
		return false;

	char fstrSize[16];
	sprintf(fstrSize, "%%%s\n", PRId64);

	fscanf(pFile, fstrSize, &m_iFileSize);

	sprintf(fstrSize, "%%d, %%%s\n", PRId64);

	while (fscanf(pFile, fstrSize, &iNAL, &iNALPos) != EOF) {

		m_NALIndex[iNAL] = iNALPos;
	}

	fclose(pFile);

	return true;
}

bool NALFileStream::saveIndex()
{
	if (!indexLoaded())
		return false;

	FILE* pIndex = NULL;

	std::string sIndexFile = m_sFile + ".index";

	pIndex = fopen(sIndexFile.c_str(), "w");

	if (pIndex == NULL)
		return false;

	char fstrSize[16];
	sprintf(fstrSize, "%%%s\n", PRId64);

	fprintf(pIndex, fstrSize, m_iFileSize);

	sprintf(fstrSize, "%%d, %%%s\n", PRId64);

	for (int i = 0; i < (int32_t)m_NALIndex.size(); i++) {

		fprintf(pIndex, fstrSize, i, m_NALIndex[i]);
	}

	fclose(pIndex);

	return true;
}

raw_data NALFileStream::next()
{
	raw_data res = read_NAL(m_iPosNAL);

	m_iPosNAL++;

	return res;
}

bool NALFileStream::indexLoaded()
{
	return m_NALIndex.size() != 0 ? true : false;
}

bool NALFileStream::seek(const int32_t iNAL)
{
	m_iPosNAL = iNAL;

	return true;
}

raw_data NALFileStream::getNAL(int32_t iNal)
{
	return read_NAL(iNal);
}

int32_t NALFileStream::nalIndexSize()
{
	return m_NALIndex.size();
}

/* ==========
 * Protected
 * ==========
 */


/* ==========
 * Private
 * ==========
 */

raw_data NALFileStream::read_NAL(int32_t iNAL)
{
	if (!indexLoaded())
		BOOST_THROW_EXCEPTION(h264IndexError() << errinfo_message("Index not loaded!"));

	if (m_NALIndex.count(iNAL) == 0)
		return raw_data(boost::shared_array<char>(), -1);

	if (!m_pFile)
		BOOST_THROW_EXCEPTION(
				h264FileStreamAccessError() << errinfo_message("Invalid file pointer!")
		);

	fseeko64(m_pFile, m_NALIndex[iNAL] + 4, 0);

	int toRead = 0;

	if (m_NALIndex.count(iNAL + 1) == 0)
		toRead = m_iFileSize - m_NALIndex[iNAL] - 4;
	else
		toRead = m_NALIndex[iNAL + 1] - m_NALIndex[iNAL] - 4;

	raw_data nal(boost::shared_array<char>(), -1);

	nal.get<0>() = boost::shared_array<char>(new char[toRead]);
	nal.get<1>() = fread(nal.get<0>().get(), 1, toRead, m_pFile);

	if (toRead != nal.get<1>())
		BOOST_THROW_EXCEPTION(
				h264FileStreamAccessError() << errinfo_message("Didn't read the amount of bytes expected!")
				);

	return nal;
}

}
