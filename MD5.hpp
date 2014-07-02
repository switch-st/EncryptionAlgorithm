#ifndef ENCRYPT_MD5_HPP
#define ENCRYPT_MD5_HPP

#include <string>
#include <openssl/md5.h>
#include "FileIO.hpp"

OPEN_NAMESPACE_SWITCHTOOL


using namespace std;

class EncryptMD5
{
public:
	EncryptMD5(void)
	{
		m_pData = NULL;
		m_lLen = 0;
		m_pFileIO = NULL;
		m_bIsCalc = false;
	}

	EncryptMD5(const unsigned char* pData, unsigned long len)
	{
		SetData(pData, len);
		m_pFileIO = NULL;
	}

	int SetData(const unsigned char* pData, unsigned long len)
	{
		m_pData = (unsigned char*)pData;
		m_lLen = len;
		m_bIsCalc = false;

		return 0;
	}

	int SetData(const char* filename)
	{
		int ret;

		if (filename == NULL) {
			return -1;
		}

		if (m_pFileIO) {
			delete m_pFileIO;
		}
		m_pFileIO = new EncryptFileIO(filename);
		ret = m_pFileIO->Read();
		if (ret != 0) {
			return -1;
		}
		ret = m_pFileIO->GetData(&m_pData, &m_lLen);
		if (ret != 0) {
			return -1;
		}

		return 0;
	}

	int GetData(unsigned char** ppData, unsigned long* pLen)
	{
		if (ppData == NULL || pLen == NULL) {
			return -1;
		}

		*ppData = m_pData;
		*pLen = m_lLen;

		return 0;
	}

	unsigned char* GetResult(void)
	{
		unsigned char* p;
		GetResult(&p);

		return p;
	}

	int GetResult(unsigned char** ppMd)
	{
		if (ppMd == NULL) {
			return -1;
		}

		if (!m_bIsCalc && m_pData == NULL) {
			*ppMd = NULL;
			return -1;
		}

		if (!m_bIsCalc && m_pData !=  NULL) {
			MD5_Init(&m_Ctx);
			MD5_Update(&m_Ctx, m_pData, m_lLen);
			MD5_Final(m_sMd, &m_Ctx);
			*ppMd = m_sMd;
			m_bIsCalc = true;

			return 0;
		}

		*ppMd = m_sMd;

		return 0;
	}

	int Write2File(const char* filename)
	{
		if (filename == NULL) {
			return -1;
		}

		if (!m_bIsCalc) {
			GetResult();
		}

		EncryptFileIO io(filename);
		io.SetData(m_sMd, 16);
		return io.Write();
	}

	~EncryptMD5(void)
	{
		if (m_pFileIO) {
			delete m_pFileIO;
		}
	}

private:
	MD5_CTX					m_Ctx;
	unsigned char*			m_pData;
	unsigned long 			m_lLen;
	unsigned char 			m_sMd[16];				// 信息摘要
	bool 					m_bIsCalc;				// 是否计算过MD5
	EncryptFileIO*			m_pFileIO;
};


CLOSE_NAMESPACE_SWITCHTOOL


#endif // ENCRYPT_MD5_HPP
