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
        
        self.records = []

        
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
        if self.lvFlag == True and self.branchFlag == False :
            recordOffset = fixedItemsTag.setItem("recordOffset0", self.pageSize-2, 2)
            lvPointerNumber = fixedItemsTag.setItemBig("lvPointerNumber0", self.pageHeaderSize + recordOffset, 4)
      
             
            
        for i in range(1, self.availablePageTag) :
            tagOffset = self.pageSize - i*4
             
            #get Record Size
            recordSize = fixedItemsTag.setItem("recordSize"+str(i), tagOffset-4, 2) & (self.pageSize -1 )
            
            #get Record Offset
            recordOffset = fixedItemsTag.setItem("recordOffset"+str(i), tagOffset-2, 2) & (self.pageSize -1 )
                
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
            self.records.append(record)
            
        fixedItemsTag = None
    
    def parsePageDataArea(self, path) :
        if self.spaceFlag == True :
            return 
        elif self.indexFlag == True :
            return
        
        if self.branchFlag == True :
            self.readBranchRecord()
        elif self.lvFlag == True :
            self.readLongvalueRecord()
        else :
            self.readDataRecord()
               
    def readBranchRecord(self) :
        columns = {}
        columns[1] = Column(1, 4, 4, "pageNumber")
        for record in self.records :
            record.setRecordType("Normal_Branch")
            record.readRecord(columns)
                
    def readDataRecord(self) :
        columns = self.columns
        for record in self.records :
            record.setRecordType("Normal_Data")
            record.readRecord(columns)

    def readLongvalueRecord(self) :
        
        for record in self.records :
            if record.getRecordSize() == record.getFirstFixedItemOffset() + 4 : # pointer record
                record.setRecordType("Normal_LvPointer")
            else :
                record.setRecordType("Normal_LvData")
            record.readRecord()

        
    def isBranchPage(self) :
        return self.branchFlag
    
    def setPageFlag(self, pageFlag) :
        if pageFlag & 0x00000001 == 0x00000001 : self.rootFlag = True
        if pageFlag & 0x00000002 == 0x00000002 : self.leafFlag = True
        if pageFlag & 0x00000004 == 0x00000004 : self.branchFlag = True
        if pageFlag & 0x00000008 == 0x00000008 : self.emptyFlag = True
        if pageFlag & 0x00000020 == 0x00000020 : self.spaceFlag = True
        if pageFlag & 0x00000020 == 0x00000020 : self.indexFlag = True
        if pageFlag & 0x00000080 == 0x00000080 : self.lvFlag = True
        if self.rootFlag == True and self.leafFlag == False and self.branchFlag == False and self.emptyFlag  == False and self.spaceFlag == False and self.indexFlag == False : 
            self.branchFlag = True
            
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
        #parse page tag flag area
        self.parsePageTagArea()
        
        #parse page data area
        self.parsePageDataArea(self.path)
        return self.records

    
    def readPageNumberList(self) :
        self.readRecords()
        pageNumberList = []
        if not self.branchFlag :
            return None
        for record in self.records :
            pageNumber = record.getDatas(1)
            if pageNumber > 0 :
                pageNumberList.append(pageNumber) 
            
        return pageNumberList
        
        
    
        
        
