import sys, os, struct
import datetime
import Error
class HexReader :
	def __init__(self, path) :
		self.path = path
		if not os.path.isfile(self.path) :
			Error.printError(0)
		self.f =  open(self.path,"rb")

	def __del__(self) :
		self.f.close()

	def readUnicode(self, offset, size) :
		self.f.seek(offset)
		dump = self.f.read(size)
		result = unicode( dump , "utf-16", "ignore")
		return result
	
	def readHexDump(self, offset, size) :
		self.f.seek(offset)
		result = self.f.read(size)
		return result
	
	def readNumber(self, offset, size) :
		self.f.seek(offset)
		result = self.f.read(size)
		for i in range(size, 8) :
			result = result + "\x00"
		result = struct.unpack("<q", result)[0]
		return result
	
	def readByte(self, offset) :
		self.f.seek(offset)
		result = struct.unpack("<B", self.f.read(1))[0]
		return result

	def readShort(self, offset) : #8 byte 
		self.f.seek(offset)
		result = struct.unpack("<h", self.f.read(2))[0]
		return result

	def readInteger(self, offset) : #4 byte 
		self.f.seek(offset)
		result = struct.unpack("<i", self.f.read(4))[0]
		return result

	def readLong(self, offset) : #8 byte 
		self.f.seek(offset)
		result = struct.unpack("<q", self.f.read(8))[0]
		return result
	
	def readUnsignedNumber(self, offset, size) :
		self.f.seek(offset)
		result = self.f.read(size)
		for i in range(size, 8) :
			result = result + "\x00"
		result = struct.unpack("<Q", result)[0]
		return result
	
	def readUnsignedShort(self, offset) : #2 byte 
		self.f.seek(offset)
		result = struct.unpack("<H", self.f.read(2))[0]
		return result

	def readUnsignedInteger(self, offset) : #4 byte 
		self.f.seek(offset)
		result = struct.unpack("<I", self.f.read(4))[0]
		return result

	def readUnsignedLong(self, offset) : #8 byte 
		self.f.seek(offset)
		result = struct.unpack("<Q", self.f.read(8))[0]
		return result
	
	def readUnsignedShortBig(self, offset) : #1 byte 
		self.f.seek(offset)
		result = struct.unpack(">H", self.f.read(2))[0]
		return result

	def readUnsignedIntegerBig(self, offset) : #4 byte 
		self.f.seek(offset)
		result = struct.unpack(">I", self.f.read(4))[0]
		return result

	def readUnsignedLongBig(self, offset) : #8 byte 
		self.f.seek(offset)
		result = struct.unpack(">Q", self.f.read(8))[0]
		return result
	
	def readUnsignedNumberBig(self, offset, size) :
		self.f.seek(offset)
		result = self.f.read(size)
		for i in range(size, 8) :
			result = "\x00" + result 
		result = struct.unpack(">Q", result)[0]
		return result
########################### cutome ###########################

	def setPageSize(self, pageSize) :
		self.pageSize = pageSize

	def readAsciiToNull(self, offset) : 
		result = self.readAscii(offset, offset - offset % self.pageSize + self.pageSize)
		return result[:result.find('\000')]

	def readUnicodeToNull(self, offset) : 
		result = self.readAscii(offset, offset - offset % self.pageSize + self.pageSize)
		return result[:result.find('\000\000')]

	def isEqual(self, offset, size, value) :
		self.f.seek(offset)
		result = self.f.read(size)
		if(result == value) :
			return True
		else : 
			return False

	def findOffset(self, offset, value) :
		result = self.readAscii(offset, offset - offset % self.pageSize + self.pageSize)
		return result.find(value)

	def findOffsetToSize(self, offset, value, size) :
		result = self.readAscii(offset, size)
		return result.find(value)	

	def getDatatime(self, filetime) :
		if filetime == 0 :
			return ''
		try :
			standareTime = datetime.datetime(1601, 1, 1, 0, 0, 0)
			return (standareTime + datetime.timedelta(microseconds=filetime/10)).strftime('%Y-%m-%d %H:%M:%S')
		except :
			return str(filetime)

	def getFileSize(self) :
		self.f.seek(0, os.SEEK_END)
		return self.f.tell()

