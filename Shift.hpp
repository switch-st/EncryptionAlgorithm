#ifndef ENCRYPT_SHIFT_HPP
#define ENCRYPT_SHIFT_HPP


#define SHIFT_LEFT(x, s, n)				((x) << (n)) | ((x) >> ((s) - (n)))
#define SHIFT_RIGHT(x, s, n)			((x) >> (n)) | ((x) << ((s) - (n)))


OPEN_NAMESPACE_SWITCHTOOL


/// 位移方向
enum EncryptShiftDirection
{
	DIR_SHIFT_LEFT,
	DIR_SHIFT_RIGHT
};


/// 位移类
/// 当前只支持将一个字符串的每一个字符移位bits位，不支持将整个字符串位移。
class EncryptShift
{
public:
	EncryptShift(void)
	{
		m_pSrc = NULL;
		m_pDst = NULL;
		m_lLen = 0;
		m_nBits = 0;
	}

	int SetData(const unsigned char* pData, unsigned long len)
	{
		if (pData == NULL) {
			return -1;
		}

		m_pSrc = (unsigned char*)pData;
		m_lLen = len;

		return 0;
	}

	int GetData(unsigned char** ppData, unsigned long* pLen)
	{
		if (ppData == NULL || pLen == NULL) {
			return -1;
		}

		*ppData = m_pDst;
		*pLen = m_lLen;

		return 0;
	}

	int Shift(unsigned int bits, EncryptShiftDirection dir)
	{
		int i;

		if (bits < 1 && bits > 7) {
			return -1;
		}

		if (!m_pDst) {
			m_pDst = (unsigned char*)malloc(m_lLen);
		}

		if (m_nBits == bits && m_Direction == dir) {
			return 0;
		}

		m_nBits = bits;
		m_Direction = dir;
		for (i = 0; i < (int)m_lLen; ++i) {
			m_pDst[i] = ShiftBase(m_pSrc[i]);
		}

		return 0;
	}

	~EncryptShift(void)
	{
		free(m_pDst);
	}

private:
	unsigned char ShiftBase(unsigned char ch)
	{
		if (m_Direction == DIR_SHIFT_LEFT) {
			return SHIFT_LEFT(ch, sizeof(ch) * 8, m_nBits);
		}

		if (m_Direction == DIR_SHIFT_RIGHT) {
			return SHIFT_RIGHT(ch, sizeof(ch) * 8, m_nBits);
		}

		return 0;
	}

private:
	unsigned char* 				m_pSrc;
	unsigned char*				m_pDst;
	unsigned long				m_lLen;
	unsigned int				m_nBits;			// 上次位移的位数
	EncryptShiftDirection		m_Direction;		// 上次位移的方向
};










CLOSE_NAMESPACE_SWITCHTOOL



#endif // ENCRYPT_SHIFT_HPP
