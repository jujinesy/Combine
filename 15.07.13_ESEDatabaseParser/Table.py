from Page import Page

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
        
    def addColumn(self, column):
        self.columns[column.getId()] = column
    
    def getColumnLength(self) :
        return len(self.columns)
    
    def getColumns(self) :
        return self.columns
    
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
            for pageNumber in page.readPageNumberList() :
                page = Page(self.path, self.pageSize, pageNumber, self.columns)
                self.getPageList(page) 
        else :
            pageNumber = page.getPageNumber()
            self.normalDataPageDict[pageNumber] = page
            if page.getPrePageNumber() == 0 :       
                self.firstDataPageList.append(pageNumber)
        
        
    def getNormalData(self) :
        recordList = []
        rootpage = Page(self.path, self.pageSize, self.tableFDP, self.columns)
        
        #find page list
        self.getPageList(rootpage)
        """
        if rootpage.isBranchPage() :
            branchpage = rootpage
            while True :
                pageNumberList = rootpage.readPageNumberList()
                for pageNumber in pageNumberList : 
                    page = Page(self.path, self.pageSize, pageNumber, self.columns)
                    self.normalDataPageDict[pageNumber] = page
        else :
            self.normalDataPageDict[self.tableFDP] = rootpage
        """
        """
        for pageNumber in self.normalDataPageDict :
            dataPage = self.normalDataPageDict[pageNumber]
            recordList += dataPage.readRecords()
        """ 
        """
        #pop first pages in page list
        for pageNumber in self.normalDataPageDict : 
            page = self.normalDataPageDict[pageNumber]
            if page.getPrePageNumber() == 0 :       
                self.firstDataPageList.append(pageNumber)
        """
        
        #read records
        recordList = []
        for pageNumber in self.firstDataPageList :
            dataPage = self.normalDataPageDict.pop(pageNumber)
            while True :
                recordList += dataPage.readRecords()
                nextPageNumber = dataPage.getNextPageNumber()
                if nextPageNumber == 0 :  
                    break;
                else :
                    dataPage = self.normalDataPageDict.pop(nextPageNumber)
         
        return recordList
    
    def getLvDatas(self, recordList) :
        lvRecordList = {}
        lvID = 1
        for record in recordList :
            datas = record.getAllDatas()
            if record.isRecordPointer() :
                lvID = record.getLvPtrNumber()
                lvRecordList[lvID] = ""
            else :
                lvID = record.getLvPtrNumber()
                lvRecordList[lvID] += record.getLvData()
        
        return lvRecordList
        
    def addDataPageList(self, pageNumber):
        if not pageNumber in self.normalDataPageList :
            self.normalDataPageList.append(pageNumber)
        
