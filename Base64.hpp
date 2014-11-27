#ifndef ENCRYPT_BASE64_HPP
#define ENCRYPT_BASE64_HPP

#include <string>
#include <openssl/evp.h>
#include <openssl/bio.h>
#include <openssl/buffer.h>
#include "FileIO.hpp"

OPEN_NAMESPACE_SWITCHTOOL


using namespace std;

class EncryptBase64
{
public:
	EncryptBase64(void)
	{
		m_pPlainText = NULL;
		m_lPlanLen = 0;
		m_pCipherText = NULL;
		m_lCipherLen = 0;
		m_pFileIO = NULL;
	}

	int SetPlainText(const unsigned char* pData, const unsigned long len)
	{
		if (pData == NULL)
		{
			return -1;
		}
		if (m_pPlainText)
		{
			free(m_pPlainText);
		}
		m_pPlainText = (unsigned char*)malloc(len);
		memcpy(m_pPlainText, pData, len);
		m_lPlanLen = len;

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
		if (pData == NULL)
		{
			return -1;
		}
		if (m_pCipherText)
		{
			free(m_pCipherText);
		}
		m_pCipherText = (unsigned char*)malloc(len);
		memcpy(m_pCipherText, pData, len);
		m_lCipherLen = len;

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

	int Encode(void)
	{
		if (m_pPlainText == NULL)
		{
			return -1;
		}

		BIO * bmem = NULL;
		BIO * b64 = NULL;
		BUF_MEM * bptr = NULL;

		b64 = BIO_new(BIO_f_base64());
		BIO_set_flags(b64, BIO_FLAGS_BASE64_NO_NL);
		bmem = BIO_new(BIO_s_mem());
		b64 = BIO_push(b64, bmem);
		BIO_write(b64, m_pPlainText, m_lPlanLen);
		BIO_flush(b64);
		BIO_get_mem_ptr(b64, &bptr);

		if (m_pCipherText)
		{
			free(m_pCipherText);
		}
		m_pCipherText = (unsigned char *)malloc(bptr->length);
		memcpy(m_pCipherText, bptr->data, bptr->length);
		m_lCipherLen = bptr->length;

		BIO_free_all(b64);

		return 0;
	}

	int Decode(void)
	{
		if (m_pCipherText == NULL)
		{
			return -1;
		}

		BIO * b64 = NULL;
		BIO * bmem = NULL;

		b64 = BIO_new(BIO_f_base64());
		BIO_set_flags(b64, BIO_FLAGS_BASE64_NO_NL);
		bmem = BIO_new_mem_buf(m_pCipherText, m_lCipherLen);
		b64 = BIO_push(b64, bmem);
		if (m_pPlainText)
		{
			free(m_pPlainText);
		}
		m_pPlainText = (unsigned char*)malloc(m_lCipherLen);
		m_lPlanLen = BIO_read(b64, m_pPlainText, m_lCipherLen);

		BIO_free_all(b64);

		return 0;
	}

	int GetPlainText(unsigned char** ppData, unsigned long* pLen)
	{
		if (ppData == NULL || pLen == NULL || m_pPlainText == NULL)
		{
            return -1;
		}
		*ppData = m_pPlainText;
		*pLen = m_lPlanLen;

		return 0;
	}

	int GetCipherText(unsigned char** ppData, unsigned long* pLen)
	{
		if (ppData == NULL || pLen == NULL || m_pCipherText == NULL)
		{
            return -1;
		}
		*ppData = m_pCipherText;
		*pLen = m_lCipherLen;

		return 0;
	}

	int WritePlainText2File(const char* filename)
	{
		if (filename == NULL || m_pPlainText == NULL) {
			return -1;
		}

		EncryptFileIO io(filename);
		io.SetData(m_pPlainText, m_lPlanLen);

		return io.Write();
	}

	int WriteCipherText2File(const char* filename)
	{
		if (filename == NULL || m_pCipherText == NULL) {
			return -1;
		}

		EncryptFileIO io(filename);
		io.SetData(m_pCipherText, m_lCipherLen);

		return io.Write();
	}

	~EncryptBase64(void)
	{
		free(m_pPlainText);
		free(m_pCipherText);
		delete m_pFileIO;
	}

private:
	unsigned char* 			m_pPlainText;					// 明文
	unsigned long 			m_lPlanLen;						// 明文长度
	unsigned char*			m_pCipherText;					// 密文
	unsigned long			m_lCipherLen;					// 密文长度
	EncryptFileIO*			m_pFileIO;
};


CLOSE_NAMESPACE_SWITCHTOOL


#endif // ENCRYPT_BASE64_HPP
