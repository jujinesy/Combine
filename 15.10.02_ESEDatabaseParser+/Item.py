import HexReader
import Error
import datetime
class Item :
    def __init__(self, offset, size, value):
        self.offset = offset
        self.size = size
        self.value = value
        
    def getOffset(self):
        return self.offset
    
    def getSize(self):
        return self.size
    
    def getValue(self):
        return self.value
    
class Items :
    def __init__(self, path, standdardOffset) :
        self.path = path
        self.standdardOffset = standdardOffset
        self.hexReader = HexReader.HexReader(path)
        self.items = {}
        
    def setItem(self, key, itemOffset, size):
        value = self.readItem(itemOffset, size)
        self.items[key] = Item(itemOffset, size, value)
        return value
    
    def setItemBig(self, key, itemOffset, size):
        value = self.readItemBig(itemOffset, size)
        self.items[key] = Item(itemOffset, size, value)
        return value
    
    def setItemUnsigned(self, key, itemOffset, size):
        value = self.readItemUnsigned(itemOffset, size)
        self.items[key] = Item(itemOffset, size, value)
        return value
    
    def convertLongToTime(self, filetime):
        if filetime == 0 :
            return ''
        try :
            standareTime = datetime.datetime(1601, 1, 1, 0, 0, 0)
            return (standareTime + datetime.timedelta(microseconds=filetime/10)).strftime('%Y-%m-%d %H:%M:%S')
        except :
            return str(filetime)
    def readItem(self, itemOffset, size): #inheritance method
        return -1
    
    def readItemBig(self, itemOffset, size): #inheritance method
        return -1
    
    def readItemUnsigned(self, itemOffset, size): #inheritance method
        return -1
    
    def getItem(self, key) :
        return self.items[key].getValue()
    
    def getSize(self, key) :
        return self.items[key].getSize()
    
    def getOffset(self, key) :
        return self.items[key].getOffset()
    
    def getLength(self) :
        return len(self.items)
        
        
class FixedItems(Items) :
    def readItem(self, itemOffset, size):
        value = None
        parsingOffset = itemOffset + self.standdardOffset
        if size < 0 :
            Error.printError(1)
            
        elif size == 0 :
            value = 0
        elif size == 1 :
            value = self.hexReader.readByte(parsingOffset)
        elif size == 2 :
            value = self.hexReader.readShort(parsingOffset)
        elif size == 4 :
            value = self.hexReader.readInteger(parsingOffset)
        elif size == 8 :
            value = self.hexReader.readLong(parsingOffset)
        elif size > 8 :
            value = self.hexReader.readHexDump(parsingOffset, size)
        else :    
            value = self.hexReader.readNumber(parsingOffset, size)
        
        return value  
    
    def readItemBig(self, itemOffset, size):
        value = None
        parsingOffset = itemOffset + self.standdardOffset
        if size < 0 :
            Error.printError(1)
        elif size == 0 :
            value = 0
        elif size == 1 :
            value = self.hexReader.readByte(parsingOffset)
        elif size == 2 :
            value = self.hexReader.readUnsignedShortBig(parsingOffset)
        elif size == 4 :
            value = self.hexReader.readUnsignedIntegerBig(parsingOffset)
        elif size == 8 :
            value = self.hexReader.readUnsignedLongBig(parsingOffset)
        else :
            value = self.hexReader.readUnsignedNumberBig(parsingOffset, size)
        return value
    
    def readItemUnsigned(self, itemOffset, size):
        value = None
        parsingOffset = itemOffset + self.standdardOffset
        if size < 0 :
            Error.printError(1)
        elif size == 0 :
            value = 0
        elif size == 1 :
            value = self.hexReader.readByte(parsingOffset)
        elif size == 2 :
            value = self.hexReader.readUnsignedShort(parsingOffset)
        elif size == 4 :
            value = self.hexReader.readUnsignedInteger(parsingOffset)
        elif size == 8 :
            value = self.hexReader.readUnsignedLong(parsingOffset)
        else :
            value = self.hexReader.readUnsignedNumber(parsingOffset, size)
        return value
            
class VariableItems(Items) :
    def readItem(self, itemOffset, size):
        if size < 0 and size >= 256 : #error
            return "parsing error"   
        parsingOffset = itemOffset + self.standdardOffset
        return self.hexReader.readHexDump(parsingOffset, size)
  
class TaggedDataItems(Items) :
    def readItem(self, itemOffset, size):
        value = None
        parsingOffset = itemOffset + self.standdardOffset
        if size < 0 :
            Error.printError(1)
        elif size == 0 :
            value = 0
        elif size == 1 :
            value = self.hexReader.readByte(parsingOffset)
        elif size == 2 :
            value = self.hexReader.readShort(parsingOffset)
        elif size == 4 :
            value = self.hexReader.readInteger(parsingOffset)
        elif size == 8 :
            value = self.hexReader.readLong(parsingOffset)
        else :
            value = self.hexReader.readHexDump(parsingOffset, size)
        return value
    
    def readItemUnsigned(self, itemOffset, size):
        value = None
        parsingOffset = itemOffset + self.standdardOffset
        if size < 0 :
            Error.printError(1)
        elif size == 0 :
            value = 0
        elif size == 1 :
            value = self.hexReader.readByte(parsingOffset)
        elif size == 2 :
            value = self.hexReader.readUnsignedShort(parsingOffset)
        elif size == 4 :
            value = self.hexReader.readUnsignedInteger(parsingOffset)
        elif size == 8 :
            value = self.hexReader.readUnsignedLong(parsingOffset)
        else :
            value = self.hexReader.readHexDump(parsingOffset, size)
        return value
    
    def setTaggedDataItem(self, key, itemOffset, size):
        value = self.readTaggedDataItem(itemOffset, size)
        self.items[key] = Item(itemOffset, size, value)
        return value
      
    def readTaggedDataItem(self, itemOffset, size):
        if size < 0 and size >= 65535 : #error
            return "parsing error"   
        parsingOffset = itemOffset + self.standdardOffset
        if size < 0 :
            return ""
        else :
            return self.hexReader.readHexDump(parsingOffset, size)
    
    """
    def convertUnicode(self, hexdump):
        if hexdump.length%2 == 1 : #odd
            hexdump.append(0x00)
        try :
            return unicode(hexdump , "utf-16", "ignore")
        except :
            return hexdump.decode("hex")
    """
    
    