/*-----------------------------------------------------------------------------
 * CStream.h
 *-----------------------------------------------------------------------------
 * 
 *-----------------------------------------------------------------------------
 * All rights reserved by Noh Yong Hwan (fixbrain@gmail.com, unsorted@msn.com)
 *-----------------------------------------------------------------------------
 * Revision History:
 * Date					Who					What
 * ----------------		----------------	----------------
 * 22/03/2007			Noh Yong Hwan		birth
**---------------------------------------------------------------------------*/

#ifndef _INCLUDE_C_STREAM_H_
#define _INCLUDE_C_STREAM_H_

#include <windows.h>
#include <crtdbg.h>

//
// �ֻ��� ��Ʈ�� �������̽� 
// 
class CStream
{
private:	
protected:
	unsigned long m_size;
	unsigned long m_pos;

	// ChangeCursor() method ���� setPos() �� ȣ���ϸ� ��� ��ȿ�� �˻�� 
	// ChangeCursor() method ������ �����Ѵ�. 
	//
	unsigned long setPos(const unsigned long newPosition)
	{
		m_pos = newPosition;
		return m_pos;
	};


	virtual unsigned long SetSize(unsigned long newSize) = 0;
public:
	CStream(): m_pos(0),m_size(0){};
	virtual ~CStream(){};


	unsigned long GetSize() { return m_size; };
	unsigned long GetCurrentCusor() { return m_pos; };
	unsigned long ChangeCursor(const unsigned long offset, unsigned long from);


	// ��Ʈ���� ����� �ڿ��� �Ҹ��Ѵ�. 
	//	(�����̵�, �޸𸮵�...)
	// 
	virtual void ClearStream(void) = 0;

	// ��Ʈ������ ���� �����͸� �о ���ۿ� ����.
	// 
	virtual unsigned long ReadFromStream(void *Buffer, unsigned long Count) = 0;

	// ���۷κ��� �����͸� �о� ��Ʈ���� ���� �����ǿ� ����.
	//
	virtual unsigned long WriteToStream(const void *Buffer, unsigned long Count) = 0;	
};


//
// �޸� ��Ʈ�� Ŭ���� 
// 
// WARNING !
//		�� Ŭ�������� �ٸ� Ŭ������ �Ļ����� ����.
//		����/����/�Ҹ��� ���.. ���� �Ļ� ��ü�� ������� �ʾ���. ������ :-)
// 
class CMemoryStream : public CStream
{
private:
	char *m_pMemory;
	virtual unsigned long SetSize(unsigned long newSize);	
protected:
public:
	CMemoryStream():m_pMemory(NULL){};
	~CMemoryStream()			
	{
		if (NULL != m_pMemory)
		{
			// free and change cursor			
			//
			if (0 != SetSize(0))
			{
				_ASSERTE(!"SetSize() error");				
			}
		}
	};


	// ��Ʈ���� ����� �ڿ��� �Ҹ��Ѵ�. 
	//	(�����̵�, �޸𸮵�...)
	//	-- inline method �� - cpp ������ �ѱ��� ����
	//
	virtual void ClearStream(void)
	{
		// free and change cursor			
		//
		if (0 != SetSize(0))
		{
			_ASSERTE(!"SetSize() error");			
		}		
	}

	// ByteToRead ��ŭ ���� �� �ִ��� �˻�
	//
	//	-- inline method �� - cpp ������ �ѱ��� ����
	//
	BOOL CanReadFromStream(unsigned long ByteToRead)
	{
		if (m_pos + ByteToRead > m_size)
			return FALSE;
		else
			return TRUE;
	}

	// ��Ʈ�� ���۸� �̹� �Ҵ�� �޸� ���۷� �Ҵ��Ѵ�. 
	// 
	void SetStreamBuffer(void* Buffer, unsigned long BufferSize)
	{
		ClearStream();

		m_pos = 0;
		m_size = BufferSize;
		m_pMemory = (char*)Buffer;
	}

	// ��Ʈ������ ���� �����͸� �о ���ۿ� ����.
	// 
	virtual unsigned long ReadFromStream(void *Buffer, unsigned long Count);

	// ���۷κ��� �����͸� �о� ��Ʈ���� ���� �����ǿ� ����.
	//
	virtual unsigned long WriteToStream(const void *Buffer, unsigned long Count);

	// �޸� ����� ���� ������ �ϱ� ���� �޼ҵ�
	//
	const void *GetMemory() { return m_pMemory; };

};



#endif