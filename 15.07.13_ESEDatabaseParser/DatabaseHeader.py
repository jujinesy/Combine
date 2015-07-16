from Item import FixedItems
		
class DatabaseHeader :
	def __init__(self, path) :
		self.path = path
		self.init()
		
	def init(self) :
		self.signature = 0
		self.version = 0
		self.status = 0
		self.revision = 0
		self.pagesize = 0x8000
		self.pageHeaderSize = 0
		
	def parseDabaseHeader(self) :
		fixedItems = FixedItems(self.path, 0x00)	
		self.signature = fixedItems.setItem("signature", 4, 4)
		self.version = fixedItems.setItem("version", 8, 4)
		self.status = fixedItems.setItem("status", 52, 4)
		self.revision = fixedItems.setItem("revision", 232,4)
		self.pagesize = fixedItems.setItem("pagesize", 236,4)
		if self.pagesize >= 16384 :
			self.pageHeaderSize = 0x50
		else : 
			self.pageHeaderSize = 0x28
		fixedItems = None
		 
	def getSignature(self) :
		return self.signature
				
	def getVersion(self) :
		return self.version
		
	def getStatus(self) :
		if self.status == 2 :
			return "Dirty"
		elif self.status == 3 :
			return "Clean"
	
	def getRevision(self) :
		return self.revision
	
	def getPageSize(self) :
		return self.pagesize
	
	def getPageHeaderSize(self) :
		return self.pageHeaderSize
	
	






