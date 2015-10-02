from Page import Page
from __builtin__ import True

class Table(Page) :
    def __init__(self, path, pageSize) :
        self.path = path
        self.pageSize = pageSize
        self.init()
    
    def init(self) :
        self.tableName = "None"
        self.tableNumber = -1
        self.tableFDP = -1 #table ID
        self.longValueFDP = -1 #lv table ID
        self.columns = {}
        self.branchPage = None
        self.normalDataPageDict = {}
        self.abnormalDataPageDict = {}
        self.firstDataPageList = []
        
        
    def findNormalPageList(self):
        rootpage = Page(self.path, self.pageSize, self.tableFDP, self.columns)
        self.getPageList(rootpage)
        
    def addColumn(self, column):
        self.columns[column.getId()] = column
    
    def getColumnLength(self) :
        return len(self.columns)
    
    def getColumns(self) :
        return self.columns
    
    def checkColumn(self, id):
        if id in self.columns.keys() :
            return True
        else :
            return False
        
    def setTableFDP(self, tableFDP) :
        self.tableFDP = tableFDP
    
    def setTableNumber(self, tableNumber) :
        self.tableNumber = tableNumber
        
    def setTableName(self, tableName) :
        self.tableName = tableName
        
    def getTableName(self) :
        return self.tableName
    
    def setLongValueFDP(self, longValueFDP) :
        self.longValueFDP = longValueFDP
    
    def getLongValueFDP(self) :
        return self.longValueFDP
    
    def getPageList(self, page) :
        if page.isBranchPage() == True :
            self.abnormalDataPageDict[page.getPageNumber()] = page
            for pageNumber in page.readPageNumberList() :
                page = Page(self.path, self.pageSize, pageNumber, self.columns)
                self.getPageList(page) 
        else :
            pageNumber = page.getPageNumber()
            self.normalDataPageDict[pageNumber] = page
            if page.getPrePageNumber() == 0 :       
                self.firstDataPageList.append(pageNumber)
           
    def getNormalData(self) :
        #read records
        recordList = []
        for pageNumber in self.firstDataPageList :
            dataPage = self.normalDataPageDict[pageNumber]
            while True :
                recordList += dataPage.readRecords()
                nextPageNumber = dataPage.getNextPageNumber()
                if nextPageNumber == 0 :  
                    break;
                else :
                    dataPage = self.normalDataPageDict[nextPageNumber]
         
        return recordList
    
    def getAbnormalData(self) :
        #read records
        recordList = []
        for pageNumber in self.normalDataPageDict.keys() :
            normalPage = self.normalDataPageDict[pageNumber]
            recordList += normalPage.readAnnormalRecords()
        
        for pageNumber in self.abnormalDataPageDict.keys() :
            abnormalDataPage = self.abnormalDataPageDict[pageNumber]
            recordList += abnormalDataPage.readAnnormalRecords()
            
        return recordList
    
    def getLvDatas(self) :
        recordList = self.getNormalData()
        lvRecordList = {}
        lvID = 1
        for record in recordList :
            datas = record.getAllDatas()
            if record.isRecordPointer() :
                lvID = record.getLvPtrNumber()
                lvRecordList[lvID] = ""
            else :
                lvID = record.getLvPtrNumber()
                if not lvID in lvRecordList.keys() :
                    lvRecordList[lvID] = ""
                lvRecordList[lvID] += record.getLvData()
        
        return lvRecordList
    
    def addAbnormalPageList(self, pageNumber) :
        if pageNumber in self.normalDataPageDict.keys() :
            return
        page = Page(self.path, self.pageSize, pageNumber, self.columns)
        
        if page.isSpacePage() : 
            return
        
        if page.isIndexPage() : 
            return
        
        if page.isEmptyPage() :
            self.abnormalDataPageDict[pageNumber] = page
        
        return 
        
        
