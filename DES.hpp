#ifndef ENCRYPT_DES_HPP
#define ENCRYPT_DES_HPP

#include <string.h>
#include <openssl/des.h>
#include "FileIO.hpp"


#define DES_BLOCK_SIZE					8


OPEN_NAMESPACE_SWITCHTOOL


class EncryptDES
{
public:
	EncryptDES(void)
	{
		m_pPlainText = NULL;
		m_pCipherText = NULL;
		m_lOraLen = 0;
		m_lFmtLen = 0;
		memset(m_Ivec, 0, sizeof(m_Ivec));
		memset(m_sKey, 0, sizeof(m_sKey));
		m_pFileIO = NULL;
	}

	static int GetDESFormatLength(int len)
	{
		return (len / DES_BLOCK_SIZE + 1) * DES_BLOCK_SIZE;
	}

	int SetPlainText(const unsigned char* pData, const unsigned long len)
	{
		if (pData == NULL) {
			return -1;
		}

		m_lOraLen = len;
		m_lFmtLen = (len / DES_BLOCK_SIZE + 1) * DES_BLOCK_SIZE;

		m_pPlainText = (unsigned char*)malloc(m_lFmtLen);
		memcpy(m_pPlainText, pData, m_lOraLen);
		memset(m_pPlainText + len, m_lFmtLen - m_lOraLen, m_lFmtLen - m_lOraLen);	// 填充补充数据

		return 0;
	}

	int SetPlainText(const char* filename)
	{
		int ret;
		unsigned char* pData;
		unsigned long len;

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
		ret = m_pFileIO->GetData(&pData, &len);
		if (ret != 0) {
			return -1;
		}

		return SetPlainText(pData, len);
	}

	int SetCipherText(const unsigned char* pData, const unsigned long len)
	{
		if (pData == NULL) {
			return -1;
		}

		m_lFmtLen = len;
		m_pCipherText = (unsigned char*)malloc(m_lFmtLen);
		memcpy(m_pCipherText, pData, m_lFmtLen);

		return 0;
	}

	int SetCipherText(const char* filename)
	{
		int ret;
		unsigned char* pData;
		unsigned long len;

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
		ret = m_pFileIO->GetData(&pData, &len);
		if (ret != 0) {
			return -1;
		}

		return SetCipherText(pData, len);
	}

	int SetKey(const unsigned char* pKey, const unsigned long len)
	{
		int minLen;

		if (pKey == NULL) {
			return -1;
		}

		minLen = len > DES_BLOCK_SIZE * 3 ? DES_BLOCK_SIZE * 3 : len;
		memset(m_sKey, 0, DES_BLOCK_SIZE * 3);
		memcpy(m_sKey, pKey, minLen);

		DES_set_key_unchecked((const_DES_cblock*)m_sKey, &m_Ks1);
		DES_set_key_unchecked((const_DES_cblock*)(m_sKey + DES_BLOCK_SIZE), &m_Ks2);
		DES_set_key_unchecked((const_DES_cblock*)(m_sKey + DES_BLOCK_SIZE * 2), &m_Ks3);

		return 0;
	}

	int SetIvec(const unsigned char* pIvec, const unsigned long len)
	{
		int i;

		if (pIvec == NULL) {
			return -1;
		}

		memset(m_Ivec, 0, DES_BLOCK_SIZE);
		for (i = 0; i < (int)len && i < DES_BLOCK_SIZE; ++i) {
			m_Ivec[i] = pIvec[i];
		}

		return 0;
	}

	int Encrypt(void)
	{
		DES_cblock ivec;

		m_pCipherText = (unsigned char*)malloc(m_lFmtLen);

		memcpy(ivec, m_Ivec, DES_BLOCK_SIZE);

		DES_ede3_cbc_encrypt(m_pPlainText, m_pCipherText, m_lFmtLen,
							&m_Ks1, &m_Ks2, &m_Ks3,
							&ivec, DES_ENCRYPT);

		return 0;
	}

	int Decrypt(void)
	{
		DES_cblock ivec;

		m_pPlainText = (unsigned char*)malloc(m_lFmtLen);

		memcpy(ivec, m_Ivec, DES_BLOCK_SIZE);

		DES_ede3_cbc_encrypt(m_pCipherText, m_pPlainText, m_lFmtLen,
							&m_Ks1, &m_Ks2, &m_Ks3,
							&ivec, DES_DECRYPT);

		return 0;
	}

	int GetPlainText(unsigned char** ppData, unsigned long* pLen)
	{
		unsigned long i;
		char ch;
		bool isComp;

		isComp = false;
		ch = m_pPlainText[m_lFmtLen - 1];
		if (ch > 0 && ch <= DES_BLOCK_SIZE) {
			for (i = m_lFmtLen - ch; i < m_lFmtLen - 1; ++i) {		// 去除补充数据
				if (m_pPlainText[i] != ch) {			// 没有补充数据
					break;
				}
			}
			if (i == m_lFmtLen - 1) {
				isComp = true;
			}
		}

		if (isComp) {
			m_lOraLen = m_lFmtLen - ch;
		} else {
			m_lOraLen = m_lFmtLen;
		}

		*ppData = m_pPlainText;
		*pLen = m_lOraLen;

		return 0;
	}

	int GetCipherText(unsigned char** ppData, unsigned long* pLen)
	{
		*ppData = m_pCipherText;
		*pLen = m_lFmtLen;

		return 0;
	}

	int WritePlainText2File(const char* filename)
	{
		unsigned char* pData;
		unsigned long len;

		if (filename == NULL) {
			return -1;
		}

		if (m_lOraLen == 0) {
			GetPlainText(&pData, &len);
		}

		EncryptFileIO io(filename);
		io.SetData(m_pPlainText, m_lOraLen);

		return io.Write();
	}

	int WriteCipherText2File(const char* filename)
	{
		if (filename == NULL) {
			return -1;
		}

		EncryptFileIO io(filename);
		io.SetData(m_pCipherText, m_lFmtLen);

		return io.Write();
	}

	~EncryptDES(void)
	{
		free(m_pPlainText);
		free(m_pCipherText);
		delete m_pFileIO;
	}

private:
	DES_key_schedule		m_Ks1;							// 密码的第一部分
	DES_key_schedule		m_Ks2;							// 密码的第二部分
	DES_key_schedule		m_Ks3;							// 密码的第三部分
	DES_cblock				m_Ivec;							// 向量
	unsigned char			m_sKey[DES_BLOCK_SIZE * 3];		// 原始密码
	unsigned char* 			m_pPlainText;					// 明文
	unsigned char*			m_pCipherText;					// 密文
	unsigned long			m_lOraLen;						// 原始长度
	unsigned long			m_lFmtLen;						// 补充数据后的长度/密文长度
	EncryptFileIO*			m_pFileIO;
};


CLOSE_NAMESPACE_SWITCHTOOL


#endif // ENCRYPT_DES_HPP
