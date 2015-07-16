from Item import FixedItems
from Item import VariableItems
from Item import TaggedDataItems
from Column import Column

from __builtin__ import False

class Record :
    def __init__(self, path, pageSize, pageOffset, recordOffset, recordSize, pageTagFlag) :
        self.path = path
        self.pageSize = pageSize
        self.pageOffset = pageOffset        
        self.recordOffset = recordOffset
        self.recordSize = recordSize
        self.pageTagFlag = pageTagFlag
        self.init()
        self.readRecordHeader()
    
    def init(self) :
        if self.pageSize >= 16384 :
            self.pageHeaderSize = 0x50
        else :
            self.pageHeaderSize = 0x28
        
        self.vFlag = False
        self.dFlag = False
        self.cFlag = False
        self.pageKeySize = 0
        
        self.lastFixedItemID = -1
        self.firstFixedItemOffset = -1
        
        self.lastVariableItemID = -1
        self.firstVariableItemOffset = -1
        
        self.firstTaggedDataItemOffset = -1
        self.columns = None
        self.datas = {}
        self.recordType = "Normal_Data"
        self.id = 0
        
    def readRecord(self, columns) :
        if not self.checkRecord() :
            return None
        self.columns = columns
        self.initRecord()
        
        if self.lastFixedItemID < 0 :
            self.readSingleItem()
            return self.datas
        
        if self.lastFixedItemID > 0 :
            self.readFxiedItem()
        
        if self.lastVariableItemID >= 128 :
            self.firstTaggedDataItemOffset = self.readVariableItem()
        else :
            self.firstTaggedDataItemOffset = self.firstFixedItemOffset-4 + self.firstVariableItemOffset
        
        if self.firstTaggedDataItemOffset < self.recordSize :
            self.readTaggedDataItem()
            
        return self.datas
    
    def initRecord(self) :
        for column in self.columns.values()  :
            if column.getId() < 128 :
                self.datas[column.getId()] = 0
            else :
                self.datas[column.getId()] = ""
                
    def readSingleItem(self) :
        dataSize = self.recordSize - self.firstFixedItemOffset
        if dataSize < 0 :
            return
        elif dataSize <= 8 :
            self.lastFixedItemID = 1
            self.readFxiedItem()
        elif dataSize <= 128 :
            self.lastVariableItemID = 128
            self.readVariableItem()
        else :
            self.readTaggedDataItem()
        return dataSize
        
    def readFxiedItem(self) :
        parsingOffset = 0
        if self.firstFixedItemOffset >= self.recordSize :
            return
        fixedItems = FixedItems(self.path, self.pageOffset + self.pageHeaderSize + self.recordOffset + self.firstFixedItemOffset)
        for i in range(1, self.lastFixedItemID+1) :
            size = self.columns[i].getSpaceUsage()
            id = self.columns[i].getId()
            type = self.columns[i].getType() 
            if type == 17 or type == 14 :
                self.datas[id] = fixedItems.setItemUnsigned(id, parsingOffset, size)
            elif type == 8 :
                self.datas[id] = fixedItems.convertLongToTime(fixedItems.setItem(id, parsingOffset, size))
            elif type == 9 :
                self.datas[id] = "0x" + str(fixedItems.setItem(id, parsingOffset, size)).encode("hex")
            else :
                self.datas[id] = fixedItems.setItem(id, parsingOffset, size)
                
            parsingOffset += size
            
        fixedItems = None

        return
        

        
        
    def readVariableItem(self) :
        sizeParsingOffset = 0
        parsingVariableItemsList = []
        fixedItems = FixedItems(self.path, self.pageOffset + self.pageHeaderSize + self.recordOffset + self.firstFixedItemOffset-4 + self.firstVariableItemOffset) 
        
        for id in range(128, self.lastVariableItemID+1) :
            size = fixedItems.setItem("VISize"+str(id), sizeParsingOffset+0, 1)
            flag = fixedItems.setItem("VIflag"+str(id), sizeParsingOffset+1, 1)
            if flag != 0x80 :
                parsingVariableItemsList.append((id, size))                 
            sizeParsingOffset += 2
        
        
        variableItems = VariableItems(self.path, self.pageOffset + self.pageHeaderSize + self.recordOffset + self.firstFixedItemOffset-4 + self.firstVariableItemOffset + sizeParsingOffset)
        dataParsingOffset = 0
        for parsingVariableItems in parsingVariableItemsList :
            id = parsingVariableItems[0]
            size = parsingVariableItems[1]
            value = variableItems.setItem(id, dataParsingOffset, size - dataParsingOffset)
            type  = 0
            if id in self.columns.keys() :
                type = self.columns[id].getType()
            else :
                type = 9
            
            if type == 10 :
                try :
                    self.datas[id] = unicode( value , "utf-8", "strict")
                except :
                    self.datas[id] = "0x" + str(value).encode("hex")
                    
            else :
                self.datas[id] = "0x" + str(value).encode("hex")
                           
            dataParsingOffset = size
            
        fixedItems = None
        variableItems = None
        
        
        return self.firstFixedItemOffset - 4 + self.firstVariableItemOffset + sizeParsingOffset + dataParsingOffset 
                
    def readTaggedDataItem(self) :
        parsingTaggedItemsList = []
        fixedItems = FixedItems(self.path, self.pageOffset + self.pageHeaderSize + self.recordOffset + self.firstTaggedDataItemOffset) 
        
        length = fixedItems.setItem("offset0", 2, 2) & (self.pageSize -1)
        index = 0
        for parsingOffset in range(0, length, 4) :
            id = fixedItems.setItem("id" + str(index), parsingOffset+0, 2)
            offset = fixedItems.getItem("offset" + str(index)) & (self.pageSize -1)
            if length == parsingOffset + 4 : # end
                size = self.recordSize - (self.firstTaggedDataItemOffset + offset)
            else : #not end
                size = fixedItems.setItem("offset" + str(index + 1), parsingOffset+6, 2) - offset


            parsingTaggedItemsList.append((id, offset, size))                 
            index += 1
        
        
        taggedItems = TaggedDataItems(self.path, self.pageOffset + self.pageHeaderSize + self.recordOffset + self.firstTaggedDataItemOffset)
        
        index = 0
        value = None
        for parsingTaggedItems in parsingTaggedItemsList :
            id = parsingTaggedItems[0]
            offset = parsingTaggedItems[1]
            size =  parsingTaggedItems[2]
            flag = fixedItems.setItem("flag" + str(index), offset, 1)
            type = 0
            if id in self.columns.keys() :
                type = self.columns[id].getType()
            else :
                type = 11
             
            if id >= 256 and (id in self.columns.keys()) and  self.columns[id].getSpaceUsage() <= 16 and self.columns[id].getSpaceUsage() > 0 :
                value = taggedItems.setItem("taggedItemValue" + str(index), offset, size)
                self.datas[id] = ("value", "0x" + str(value).encode("hex"))
            
            elif flag == 0 :
                value = taggedItems.setItem("taggedItemValue" + str(index), offset+1, size-1)
                self.datas[id] = ("value", "0x" + str(value).encode("hex"))
                
            elif flag == 1 :
                value = taggedItems.setTaggedDataItem("taggedItemValue" + str(index), offset+1, size-1)
                if type == 12 :
                    try :
                        self.datas[id] = ("value", unicode( value , "utf-16", "strict"))
                    except :
                        self.datas[id] = ("value", "0x" + str(value.encode("hex"))) 
                else : 
                    self.datas[id] = ("value", "0x" + str(value.encode("hex")))
                
            elif flag == 3 : #compress
                value = taggedItems.setTaggedDataItem("taggedItemValue" + str(index), offset+1, size-1)
                self.datas[id] = ("value", self.decompress(value[1:]).encode("utf-8"))
                
            else : #pointer
                value = fixedItems.setItem("taggedItemPointer"+str(index), offset+1, size-1)
                self.datas[id] = ("pointer", value)

            index += 1
        
        fixedItems = None
        taggedItems = None

    def checkRecord(self) :
        if self.pageKeySize >= self.recordSize :
            return False
        """
        if self.dFlag :
            return False
        """
        return True 
        
    def readRecordHeader(self) :
        self.setPageFlag(self.pageTagFlag)
        fixedItems = FixedItems(self.path, self.pageOffset + self.pageHeaderSize + self.recordOffset)
        parsingOffset = 0

        if self.cFlag :
            self.pageKeySize = fixedItems.setItem("pageKeySize", 2, 2)
            parsingOffset = 4 + self.pageKeySize 
        else :
            self.pageKeySize = fixedItems.setItem("pageKeySize", 0, 2) & 0x1FFF
            parsingOffset = 2 + self.pageKeySize
        
        
        self.lastFixedItemID = fixedItems.setItem("lastFixedItemID", parsingOffset + 0, 1)
        self.lastVariableItemID = fixedItems.setItem("lastVariableItemID", parsingOffset + 1, 1)
        self.firstVariableItemOffset = fixedItems.setItem("firstVariableItemOffset",  parsingOffset + 2, 1)
        self.firstFixedItemOffset = parsingOffset + 4
        parsingOffset = parsingOffset + 4 
 
                  
        fixedItems = None
    
    
    def setPageFlag(self, pageFlag) :
        if pageFlag & 0x20 == 0x20 : self.vFlag = True
        if pageFlag & 0x40 == 0x40 : self.dFlag = True
        if pageFlag & 0x80 == 0x80 : self.cFlag = True
        
    
    def getDatas(self, id) :
        if id in self.datas.keys() :
            return self.datas[id]
        else :
            return -1
    
    def getAllDatas(self) :
        return self.datas
        
    def setRecordType(self, type):
        self.type = type
    
    def getRecordType(self):
        return self.type
    
    def getRecordSize(self):
        return self.recordSize
    
    def getFirstFixedItemOffset(self) :
        return self.firstFixedItemOffset
    
    def getPageKeySize(self) : 
        return self.pageKeySize
    
    def decompress(self, compressedText) :
        decompressedText = ""
        temp1 = ""
        for i in range(0, len(compressedText)) :
            temp2 = str(bin(int(compressedText[i].encode("hex"),16)))
            temp2 = temp2[2:]
            for j in range(len(temp2), 8) :
                temp2 = "0" + temp2
            temp1 = temp2 + temp1
            while len(temp1) >= 7 :
                temp2 = temp1[len(temp1)-7:]
                decompressedText += chr(int(temp2, 2))
                temp1 = temp1[:len(temp1)-7]
        return decompressedText

class SingleDataRecord(Record) :
    def readRecordHeader(self) :
        self.setPageFlag(self.pageTagFlag)
        fixedItems = FixedItems(self.path, self.pageOffset + self.pageHeaderSize + self.recordOffset)
        parsingOffset = 0
        
        
        #self.pageKeySize = fixedItems.setItem("pageKeySize", 0, 2)
        #parsingOffset = 2 + self.pageKeySize
        
        if self.cFlag :
            self.pageKeySize = fixedItems.setItem("pageKeySize", 2, 2)
            parsingOffset = 4 + self.pageKeySize 
        else :
            self.pageKeySize = fixedItems.setItem("pageKeySize", 0, 2) & 0x1FFF
            parsingOffset = 2 + self.pageKeySize
        
        self.firstFixedItemOffset = parsingOffset      
        fixedItems = None
        
class LongValueRecord(Record) :
    def __init__(self, path, pageSize, pageOffset, recordOffset, recordSize, pageTagFlag) :
        Record.__init__(self, path, pageSize, pageOffset, recordOffset, recordSize, pageTagFlag)
    
    def init(self) :
        Record.init(self)
        self.LvPtrNumber = -1
        self.previousRecorededSize = -1
        self.firstRecordFlag = True
        self.privousRecordedSize = 0
        self.totalDataSize = 0
        
    def readRecordHeader(self) :
        self.setPageFlag(self.pageTagFlag)
        fixedItems = FixedItems(self.path, self.pageOffset + self.pageHeaderSize + self.recordOffset)
        parsingOffset = 0
        
        #get pageKeySize
        if self.cFlag :
            self.pageKeySize = fixedItems.setItem("pageKeySize", 2, 2)
            parsingOffset = 4 
        else : 
            self.pageKeySize = fixedItems.setItem("pageKeySize", 0, 2) & 0x1FFF
            parsingOffset = 2
        
                        
        if self.recordSize != parsingOffset + self.pageKeySize + 8 :
            self.firstRecordFlag = False
              
        #get lvPtrNumber 
        if (self.firstRecordFlag == False and self.pageKeySize == 0 ) : 
            self.LvPtrNumber = -1 #it exists in TAG0
            self.previousRecorededSize = -1 #it exists in TAG0
        
        elif (self.firstRecordFlag == False and self.pageKeySize <= 4 and self.pageKeySize > 0 ) :
            self.LvPtrNumber = -1 #it exists in TAG0
            self.previousRecorededSize = fixedItems.setItemBig("previousRecorededSize", parsingOffset, self.pageKeySize)
            parsingOffset += self.pageKeySize
                
        elif (self.firstRecordFlag == True and self.pageKeySize == 0) : 
            self.LvPtrNumber = -1 #it exists in TAG0
            parsingOffset += 4
            
        else :
            if not self.firstRecordFlag :
                self.LvPtrNumber = fixedItems.setItemBig("LvPtrNumber", parsingOffset, self.pageKeySize-4)
                self.previousRecorededSize = fixedItems.setItemBig("previousRecorededSize", parsingOffset+self.pageKeySize-4, 4)
                parsingOffset += self.pageKeySize
            else :
                self.LvPtrNumber = fixedItems.setItemBig("LvPtrNumber", parsingOffset, self.pageKeySize) 
                parsingOffset += self.pageKeySize + 4
                         
        
        
        self.firstFixedItemOffset = parsingOffset
        fixedItems = None
        
    def readRecord(self) :
        if not self.checkRecord() :
            return None
                
        if self.firstRecordFlag :
            id = "Normal_LvTotalSize"
            fixedItems = FixedItems(self.path, self.pageOffset + self.pageHeaderSize + self.recordOffset)
            value = fixedItems.setItem(id, self.firstFixedItemOffset, 4)
            self.datas[id] = value
            fixedItems = None
        
        else :
            id = "Normal_LvData"
            taggedDataItems = TaggedDataItems(self.path, self.pageOffset + self.pageHeaderSize + self.recordOffset)
            value = taggedDataItems.setTaggedDataItem(id, self.firstFixedItemOffset, self.recordSize - self.firstFixedItemOffset)
            self.datas[id] = value
            taggedDataItems = None
    
    def isRecordPointer(self) :
        return self.firstRecordFlag
    
    def getLvPtrNumber(self) :
        return self.LvPtrNumber
    
    def setLvPtrNumber(self, LvPtrNumber) :
        self.LvPtrNumber = LvPtrNumber
    
    def getLvData(self) :
        if self.firstRecordFlag :
            return self.datas["Normal_LvTotalSize"]
        else : 
            return self.datas["Normal_LvData"]
    