#ifndef ENCRYPT_FILEIO_HPP
#define ENCRYPT_FILEIO_HPP

#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>
#include <errno.h>
#include <string.h>

OPEN_NAMESPACE_SWITCHTOOL


using namespace std;

class EncryptFileIO
{
public:
	EncryptFileIO(void)
	{
		m_bInited = false;
		m_bAlloced = false;
		m_sFileName = "";
		m_pData = NULL;
		m_lLen = 0;
	}

	EncryptFileIO(const char* filename)
	{
		m_bInited = false;
		m_bAlloced = false;
		m_sFileName = filename;
		m_pData = NULL;
		m_lLen = 0;
	}

	int Read(const char* filename = NULL)
	{
		int ret, len, pos;
		struct stat fileStatus;
		int fileDesc;

		if (m_bInited) {
			return -1;
		}

		if (filename != NULL && m_sFileName != "") {
			return -1;
		}

		if (filename == NULL && m_sFileName == "") {
			return -1;
		}

		if (filename != NULL) {
			m_sFileName = filename;
		}

		ret = stat(m_sFileName.c_str(), &fileStatus);
		if (ret == -1) {
			return -1;
		}
		m_lLen = fileStatus.st_size;
		m_pData = (unsigned char*)malloc(m_lLen);

		fileDesc = open(m_sFileName.c_str(), O_RDONLY);
		if (fileDesc == -1) {
			return -1;
		}

		pos = 0;
		len = m_lLen;
		memset(m_pData, 0, m_lLen);
		while (1) {
			ret = read(fileDesc, m_pData + pos, len);
			if (ret == -1) {
				if (errno != EINTR) {		// error
					close(fileDesc);
					free(m_pData);
					return -1;
				}
			} else if (ret == 0) {
				break;
			} else {
				pos += ret;
				len -= ret;
			}
		}

		close(fileDesc);
		m_bInited = true;
		m_bAlloced = true;

		return 0;
	}

	int Write(const char* filename = NULL)
	{
		int ret, len, pos;
		int fileDesc;

		if (!m_bInited) {
			return -1;
		}

		if (filename == NULL && m_sFileName == "") {
			return -1;
		}

		if (filename == NULL) {
			fileDesc = open(m_sFileName.c_str(), O_WRONLY | O_CREAT | O_TRUNC, 0644);
		} else {
			fileDesc = open(filename, O_WRONLY | O_CREAT | O_TRUNC, 0644);
		}
		if (fileDesc == -1) {
			return -1;
		}

		pos = 0;
		len = m_lLen;
		while (1) {
			ret = write(fileDesc, m_pData + pos, len);
			if (ret == -1) {
				if (errno != EINTR) {		// error
					close(fileDesc);
					return -1;
				}
			} else if (ret == 0) {
				break;
			} else {
				pos += ret;
				len -= ret;
			}
		}

		close(fileDesc);

		return 0;
	}

	int GetData(unsigned char** ppData, unsigned long* pLen)
	{
		if (!m_bInited) {
			return -1;
		}

		if (ppData == NULL || pLen == NULL) {
			return -1;
		}

		*ppData = m_pData;
		*pLen = m_lLen;

		return 0;
	}

	int SetData(const unsigned char* pData, const unsigned long len)
	{
		if (m_bInited) {
			return -1;
		}

		if (pData == NULL) {
			return -1;
		}

		m_pData = (unsigned char*)pData;
		m_lLen = len;
		m_bInited = true;

		return 0;
	}

	~EncryptFileIO(void)
	{
		if (m_bAlloced) {
			free(m_pData);
		}
	}

private:
	string				m_sFileName;
	unsigned char*		m_pData;
	unsigned long		m_lLen;
	bool 				m_bInited;
	bool				m_bAlloced;
};


CLOSE_NAMESPACE_SWITCHTOOL


#endif // ENCRYPT_FILEIO_HPP
