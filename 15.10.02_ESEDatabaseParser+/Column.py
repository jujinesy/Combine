        
class Column :
    def __init__(self, columnId, type, spaceUsage, name) :
        self.columnId = columnId
        self.type = type
        """
        if columnId < 128 :
            self.columnType = "fixedItem"
        elif columnId >= 128 and columnId < 256 :
            self.columnType = "variableItem"
        elif columnId >= 256 :
            self.columnType = "taggedDataItem"
        else : 
            self.columnType= "unKnown"
        """      
        self.spaceUsage = spaceUsage
        self.name = name
    
    def getId(self) :
        return self.columnId
    
    def getType(self) :
        return self.type
    
    def getColumnType(self) : #no use
        if self.type == 0 : return 'JET_coltypNil' #None
        elif self.type == 1 : return 'JET_coltypBit' #boolean
        elif self.type == 2 : return 'JET_coltypUnsignedByte' #byte
        elif self.type == 3 : return 'JET_coltypShort' #short
        elif self.type == 4 : return 'JET_coltypLong' #integer
        elif self.type == 5 : return 'JET_coltypCurrency' #Currency (64-bit)
        elif self.type == 6 : return 'JET_coltypIEEESingle' #float
        elif self.type == 7 : return 'JET_coltypIEEEDouble' #double
        elif self.type == 8 : return 'JET_coltypDateTime' #time
        elif self.type == 9 : return 'JET_coltypBinary' #binary
        elif self.type == 10 : return 'JET_coltypText' #text
        elif self.type == 11 : return 'JET_coltypLongBinary' #binary
        elif self.type == 12 : return 'JET_coltypLongText' #text
        elif self.type == 13 : return 'JET_coltypSLV' #lv
        elif self.type == 14 : return 'JET_coltypUnsignedLong' #unsigned integer
        elif self.type == 15 : return 'JET_coltypLongLong' #long 
        elif self.type == 16 : return 'JET_coltypGUID' #text
        elif self.type == 17 : return 'JET_coltypUnsignedShort' #unsigned short
        else : return "UnKnown" 
    
    def getSpaceUsage(self):
        return self.spaceUsage
    
    def getName(self) :
        return self.name
    
    