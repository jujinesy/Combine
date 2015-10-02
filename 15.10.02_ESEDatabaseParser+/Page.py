from Item import FixedItems
from Item import VariableItems
from Item import TaggedDataItems
from Record import Record
from Record import SingleDataRecord
from Record import LongValueRecord
from Column import Column
class Page :
    def __init__(self, path, pageSize, pageNumber, columns) :   
        #init value
        self.path= path
        self.pageSize = pageSize
        self.pageNumber = pageNumber
        self.columns = columns
        self.init()
        
    def init(self) :
        self.pageHeaderSize = 0
        if self.pageSize >= 16384 :
            self.pageHeaderSize= 0x50
        else :
            self.pageHeaderSize= 0x28
            
        self.pageOffset = (self.pageNumber+1) * self.pageSize
        
        self.recordList = []
        self.abnormalRecordList = []
        
        self.bitmap = []
        for i in range(0,self.pageSize/8) :
            if i < self.pageHeaderSize/8 :
                self.bitmap.append(0xFF)
            else :
                self.bitmap.append(0)
                
        #define page header item
        self.prePageNumber = 0
        self.nextPageNumber = 0
        self.fatherDataPageNumber = 0
        self.availableDataOffset = 0
        self.availablePageTag = 0
        self.pageFlags = 0
        
        #init pageFlag
        self.rootFlag = False
        self.leafFlag = False
        self.branchFlag = False
        self.emptyFlag = False
        self.spaceFlag = False
        self.indexFlag = False
        self.lvFlag = False  
        
        #parse page header area
        self.parsePageHeaderArea()
        
        #parse page tag flag area
        self.parsePageTagArea()
        
    def parsePageHeaderArea(self) :
        fixedItemsHeader = FixedItems(self.path, self.pageOffset)         
        self.prePageNumber = fixedItemsHeader.setItem("prePageNumber", 16, 4)
        self.nextPageNumber = fixedItemsHeader.setItem("nextPageNumber", 20, 4)
        self.fatherDataPageNumber = fixedItemsHeader.setItem("fatherDataPageNumber", 24, 4)
        self.availableDataOffset = fixedItemsHeader.setItem("availableDataOffset", 32,2)
        self.availablePageTag = fixedItemsHeader.setItem("availablePageTag", 34,2)
        self.pageFlags = fixedItemsHeader.setItem("pageFlags", 36,4)
        self.setPageFlag(self.pageFlags)
        fixedItemsHeader = None
        
    def parsePageTagArea(self) :
        fixedItemsTag = FixedItems(self.path, self.pageOffset)
        recordSize = None
        recordOffset = None
        pageTagFlag = None
        
        lvPointerNumber = 0
        
        #firstRecord
        if self.availablePageTag != 0 :
            firstRecordOffset = fixedItemsTag.setItem("firstRecordOffset", self.pageSize-2, 2)
            firstRecordSize = fixedItemsTag.setItem("firstRecordSize", self.pageSize-4, 2)
            self.setBitmap(self.pageSize-self.pageHeaderSize-4, 4)
            self.setBitmap(firstRecordOffset, firstRecordSize)
            
            if self.lvFlag == True and self.branchFlag == False :
                lvPointerNumber = fixedItemsTag.setItemBig("lvPointerNumber", self.pageHeaderSize + firstRecordOffset, 4)
            
        for i in range(1, self.availablePageTag) :
            tagOffset = self.pageSize - i*4
             
            #get Record Size
            recordSize = fixedItemsTag.setItem("recordSize"+str(i), tagOffset-4, 2) & (self.pageSize -1 )
            
            #get Record Offset
            recordOffset = fixedItemsTag.setItem("recordOffset"+str(i), tagOffset-2, 2) & (self.pageSize -1 )
                
            #set Bitmap
            self.setBitmap(recordOffset, recordSize)
            self.setBitmap(tagOffset-self.pageHeaderSize-4, 4)
            
            #get Page Tag Flag
            if self.pageSize >= 16384 : 
                pageTagFlag = fixedItemsTag.setItem("pageTagFlag"+str(i), self.pageHeaderSize + 1 + recordOffset , 1)
            else :
                pageTagFlag = (fixedItemsTag.setItem("pageTagFlag"+str(i), tagOffset-1 , 1) )
                
            record = None
            
            if self.branchFlag == True :             
                record = SingleDataRecord(self.path, self.pageSize, self.pageOffset, recordOffset, recordSize, pageTagFlag)
            elif self.lvFlag == True :
                record = LongValueRecord(self.path, self.pageSize, self.pageOffset, recordOffset, recordSize, pageTagFlag)
                if record.getLvPtrNumber() == -1 :
                    record.setLvPtrNumber(lvPointerNumber)
            else :
                record = Record(self.path, self.pageSize, self.pageOffset, recordOffset, recordSize, pageTagFlag)         
                if record.isDflag() :
                    self.abnormalRecordList.append(record)
                    continue
            self.recordList.append(record)
            
        fixedItemsTag = None

    def readBranchRecord(self) :
        columns = {}
        columns[1] = Column(1, 4, 4, "pageNumber")
        for record in self.recordList :
            record.setRecordType("Normal_Branch")
            record.readRecord(columns)
                
    def readDataRecord(self) :
        columns = self.columns
        for record in self.recordList :
            record.setRecordType("Normal_Data")
            record.readRecord(columns)

    def readAnnormalDataRecords(self) :

        fixedItemsTag = FixedItems(self.path, self.pageOffset)
        #carve tag
        recordOffset = 0
        recordSize = 0

        for i in range(self.availablePageTag, (self.pageSize - self.pageHeaderSize)/4) :
            tagOffset = self.pageSize - i*4
            
            #get Record Size
            recordSize = fixedItemsTag.setItem("recordSize"+str(i), tagOffset-4, 2) & (self.pageSize -1 )
            
            #get Record Offset
            recordOffset = fixedItemsTag.setItem("recordOffset"+str(i), tagOffset-2, 2) & (self.pageSize -1 )
            #check Record Size
            if recordSize <= 0 or recordSize > self.pageSize :
                break
            #check Record Offset
            if recordOffset < 0 or recordOffset > self.pageSize :
                break
            #check Record Bitmap
            if not self.checkBitmap(recordOffset, recordSize) :
                continue
            
            #get Page Tag Flag
            if self.pageSize >= 16384 : 
                pageTagFlag = fixedItemsTag.setItem("pageTagFlag"+str(i), self.pageHeaderSize + 1 + recordOffset , 1)
            else :
                pageTagFlag = (fixedItemsTag.setItem("pageTagFlag"+str(i), tagOffset-1 , 1) )
            
            record = Record(self.path, self.pageSize, self.pageOffset, recordOffset, recordSize, pageTagFlag)
            #check Record Header
            if not record.isNormalRecord(self.columns) :
                break
            
            self.abnormalRecordList.append(record)
            self.setBitmap(recordOffset, recordSize)
            self.setBitmap(tagOffset-self.pageHeaderSize-4, 4)
            
        fixedItemsTag = None
        
        #carve data
        
        recordInfo = self.carvingData(self.pageHeaderSize)
        recordOffset = recordInfo[0]
        pageTagFlag = recordInfo[1]
        minimumSize = recordInfo[2]
        while recordOffset < self.pageSize : #max page Size
            if minimumSize == 0 :
                break
            recordInfo = self.carvingData(recordOffset + minimumSize)
            
            tempRecordOffset = recordInfo[0]
            tempPageTagFlag = recordInfo[1]
            minimumSize = recordInfo[2]
            
            
            recordSize = tempRecordOffset - recordOffset
            recordOffset -= self.pageHeaderSize 

            record = Record(self.path, self.pageSize, self.pageOffset, recordOffset, recordSize , pageTagFlag)
            self.abnormalRecordList.append(record)
            self.setBitmap(recordOffset, recordSize)
            
            recordOffset = tempRecordOffset
            pageTagFlag = tempPageTagFlag
            
            
        
        #read record
        for record in self.abnormalRecordList :
            record.setRecordType("Abnormal_Data")
            try :
                record.readRecord(self.columns)
            except :
                continue
        
        return self.abnormalRecordList
    
    def carvingData(self, offset) :
        #bitInfo = self.getNextBitmap(offset)
        #blankOffset = bitInfo[0]
        #blankSize = bitInfo[1]   
        #if blankOffset == -1 and blankSize == 0 :
            #return (self.pageSize,0,0)
        
        blankOffset = offset
        blankSize = 0
        
        startOffset = 0
        lastFixedItemID = 0
        lastVariableItemID = 0
        firstVariableItemOffset = 0
        pageType = 1
        pageKeySize = 0
        pageFlag = 0
        
        if self.pageSize < 16384 :    
            pageType = 2 #I do not know pageFlag, so this program have to check twice
                
        for t in range(0, pageType) :
            while True :
                bitInfo = self.getNextBitmap(blankOffset+ blankSize)
                
                if bitInfo[0] == -1 or bitInfo[1] == 0 :
                    break
                blankOffset = bitInfo[0]
                blankSize = bitInfo[1]
                for i in range(0,blankSize) :
                    fixedItemsCarving = FixedItems(self.path, self.pageOffset+blankOffset + i)
                    
                    #Filtering
                    firstFilter = fixedItemsCarving.setItem("pageFlag", 0, 1)
                    if firstFilter == 0 :
                        continue
                    
                    if pageType == 1 :  #pageSize >= 16384        
                        pageFlag = fixedItemsCarving.setItem("pageFlag", 1, 1)
                        if pageFlag == 0 :
                            continue 
                        if pageFlag & 0x80 != 0 :
                            pageKeySize = fixedItemsCarving.setItem("pageKeySize", 2, 2) 
                            startOffset = pageKeySize + 4
                        else :
                            pageKeySize = fixedItemsCarving.setItem("pageKeySize", 0, 2) & 0x1FFF
                            startOffset = pageKeySize + 2 
                    
                    else : #pageSize < 16384
                        if t == 0 :
                            pageFlag = 0x80 
                            pageKeySize = fixedItemsCarving.setItem("pageKeySize", 2, 2) 
                            startOffset = pageKeySize + 4
                        else :  #t==1
                            pageKeySize = fixedItemsCarving.setItem("pageKeySize", 0, 2) & 0x1FFF
                            startOffset = pageKeySize + 2
                        
                    if pageKeySize < 0 or pageKeySize >= blankSize-i :
                        continue
                    
                    if pageKeySize > 512 : #no reason
                        continue
                    
                    lastFixedItemID = fixedItemsCarving.setItem("pageKeySize", startOffset, 1)
                    
                    if not lastFixedItemID in self.columns.keys() :
                        continue
                    if lastFixedItemID >= 128 :
                        continue
                    lastVariableItemID = fixedItemsCarving.setItem("pageKeySize", startOffset + 1, 1)
                    if not lastVariableItemID in self.columns.keys() and lastVariableItemID != 127 :
                        continue
                     
                    firstVariableItemOffset = fixedItemsCarving.setItem("pageKeySize", startOffset + 2, 2)
                    if firstVariableItemOffset > blankSize - i :
                        continue
                    if firstVariableItemOffset < 0 :    
                        continue
                
                    fixedItemsCarving = None
                    minimumSize = 0
                    for j in range(1, lastFixedItemID+1) :
                        if j > 128 :
                            break
                        minimumSize += self.columns[j].getSpaceUsage()
                    
                    if (firstVariableItemOffset - minimumSize) <= 7 and (firstVariableItemOffset - minimumSize) >= 4:
                        minimumSize = minimumSize + startOffset + 6
                    else :
                        continue
                        
                    return (blankOffset+i, pageFlag, minimumSize)
                
                
                        
                         
                        
                        
                fixedItemsCarving = None
            
        return (blankOffset + blankSize, 0, 0)
        
    def readLongvalueRecord(self) :
        for record in self.recordList :
            if record.getRecordSize() == record.getFirstFixedItemOffset() + 4 : # pointer record
                record.setRecordType("Normal_LvPointer")
            else :
                record.setRecordType("Normal_LvData")
            record.readRecord()
    
    def isBranchPage(self) :
        return self.branchFlag
    
    def setBitmap(self, recordOffset, recordSize) :
        offset = recordOffset+self.pageHeaderSize
        for i in range(offset, offset+recordSize) :
            self.bitmap[i/8] = self.bitmap[i/8] | (0x80 >> (i%8))
    
    def checkBitmap(self, recordOffset, recordSize) : #############
        offset = recordOffset+self.pageHeaderSize
        for i in range(offset, offset+recordSize, 8) :
            if i/8 >= len(self.bitmap) :
                return False
            if self.bitmap[i/8] != 0 :
                return False
        return True 
    
    def getNextBitmap(self, nowOffset) :
        blankOffset = -1
        blankSize = 0
        for i in range(nowOffset/8, len(self.bitmap)) :
            value = self.bitmap[i] 
            if value == 0xFF and blankOffset == -1 :
                continue
            else :
                nowBit = 0
                if i == nowOffset/8 :
                    nowBit = nowOffset%8
                for j in range(nowBit, 8) :
                    if blankOffset == -1 :
                        if value & (0x80 >> j) == 0 :
                            blankOffset = i * 8 + j
                            continue
                    else : 
                        if value & (0x80 >> j) != 0 :
                            blankSize = (i * 8 + j) - blankOffset
                            break
                        
            if blankSize != 0 :
                break;          
        
        
        return (blankOffset, blankSize)
        

    
    def setPageFlag(self, pageFlag) :
        if pageFlag & 0x00000001 == 0x00000001 : self.rootFlag = True
        if pageFlag & 0x00000002 == 0x00000002 : self.leafFlag = True
        if pageFlag & 0x00000004 == 0x00000004 : self.branchFlag = True
        if pageFlag & 0x00000008 == 0x00000008 : self.emptyFlag = True
        if pageFlag & 0x00000020 == 0x00000020 : self.spaceFlag = True
        if pageFlag & 0x00000040 == 0x00000040 : self.indexFlag = True
        if pageFlag & 0x00000080 == 0x00000080 : self.lvFlag = True
        if self.rootFlag == True and self.leafFlag == False and self.branchFlag == False and self.emptyFlag  == False and self.spaceFlag == False and self.indexFlag == False : 
            self.branchFlag = True
    
    def isEmptyPage(self) :
        return self.emptyFlag
    
    def isSpacePage(self) :
        return self.spaceFlag
    
    def isIndexPage(self) :
        return self.indexFlag
    
    def getPageNumber(self) :
        return self.pageNumber
    
    def getPrePageNumber(self) :
        return self.prePageNumber
    
    def getNextPageNumber(self) :
        return self.nextPageNumber
        
    def getFatherDataPageNumber(self) :
        return self.fatherDataPageNumber
    
    def getAvailableDataOffset(self) :
        return self.availableDataOffset
    
    def getAvailablePageTag(self) :
        return self.availablePageTag
    
    def getPageFlags(self) :
        return self.pageFlags
    
    def readRecords(self) :
        #parse page data area
        if self.spaceFlag == True :
            return []
        elif self.indexFlag == True :
            return []
        
        if self.branchFlag == True :
            self.readBranchRecord()
        elif self.lvFlag == True :
            self.readLongvalueRecord()
        else :
            self.readDataRecord()
        return self.recordList
    
    def readAnnormalRecords(self) :
        if self.spaceFlag == True :
            return [] 
        elif self.indexFlag == True :
            return []
        #if self.branchFlag == True :
            #self.readBranchRecord()
        elif self.lvFlag == True :
            return []
        else :
            self.readAnnormalDataRecords()
        return self.abnormalRecordList
    
    def readPageNumberList(self) :
        self.readRecords()
        pageNumberList = []
        if not self.branchFlag :
            return None
        for record in self.recordList :
            pageNumber = record.getDatas(1)
            if pageNumber > 0 :
                pageNumberList.append(pageNumber) 
            
        return pageNumberList
        
        
    
        
        
