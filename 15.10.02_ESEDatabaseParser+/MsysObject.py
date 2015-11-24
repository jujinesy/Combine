from Table import Table
from Column import Column
from Page import Page

class MsysObject :
    def __init__(self, path, pageSize, swich) :
        self.path = path
        self.pageSize = pageSize
        self.swich = swich
        self.init()
        
    def init(self):
        self.tables = {}
        self.deletedTables = {}
        self.table = Table(self.path, self.pageSize)
        self.table.setTableFDP(4)
        self.table.setTableNumber(2)
        self.setMsysObjectColumns()
        self.table.findNormalPageList()
        recordList = self.table.getNormalData()
        self.makeTables(recordList, "")
        if self.swich == True :
            recordList = self.table.getAbnormalData()
            self.makeTables(recordList, "Deleted_")
        
    def getTables(self) :
        return self.tables
    
    def setMsysObjectColumns(self) :
        self.table.addColumn(Column(1, 4, 4, "ObjidTable"))
        self.table.addColumn(Column(2, 3, 2, "Type"))
        self.table.addColumn(Column(3, 4, 4, "Id"))
        self.table.addColumn(Column(4, 4, 4, "ColtypOrPgnoFDP"))
        self.table.addColumn(Column(5, 4, 4, "SpaceUsage"))
        self.table.addColumn(Column(6, 4, 4, "Flags"))
        self.table.addColumn(Column(7, 4, 4, "PagesOrLocale"))
        self.table.addColumn(Column(8, 1, 1, "RootFlag"))
        self.table.addColumn(Column(9, 3, 2, "RecordOffset"))
        self.table.addColumn(Column(10, 4, 4, "LCMapFlags"))
        self.table.addColumn(Column(11, 17, 2, "KeyMost"))
        self.table.addColumn(Column(128, 10, 255, "Name"))
        self.table.addColumn(Column(129, 9, 255, "Stats"))
        self.table.addColumn(Column(130, 10, 255, "TemplateTable"))
        self.table.addColumn(Column(131, 9, 255, "DefaultValue"))
        self.table.addColumn(Column(132, 9, 255, "KeyFldIDs"))
    
    def makeTables(self, recordList, status) :
        for record in recordList :
            objidTable = record.getDatas(1)
            type = record.getDatas(2)
            id = record.getDatas(3)
            coltypOrPgnoFDP = record.getDatas(4)
            spaceUsage = record.getDatas(5)
            #flags = record.getDatas(6)
            #pagesOrLocale = record.getDatas(7)
            #rootFlag = record.getDatas(8)
            #recordOffset = record.getDatas(9)
            #lCMapFlags = record.getDatas(10)
            #keyMost = record.getDatas(11)
            name = record.getDatas(128)
            #stats = recordv129)
            #templateTable = record.getDatas(130)
            #defaultValue = record.getDatas(131)
            #keyFldIDs = record.getDatas(132)
            
            if status == "Deleted_" and objidTable in self.tables.keys() :
                if not "Deleted_" in self.tables[objidTable].getTableName() :
                    continue
            
            if not objidTable in self.tables.keys() :
                self.tables[objidTable] = Table(self.path, self.pageSize)
                
            if type ==1 :
                self.tables[objidTable].setTableNumber(id)
                self.tables[objidTable].setTableFDP(coltypOrPgnoFDP)
                self.tables[objidTable].setTableName(status+name)
                
            elif type ==2 :
                if objidTable == 2 :
                    self.table.addColumn(Column(id, coltypOrPgnoFDP, spaceUsage, name))
                    
                self.tables[objidTable].addColumn(Column(id, coltypOrPgnoFDP, spaceUsage, name))
                
            elif type ==4 :
                if not id in self.tables.keys() :
                    self.tables[id] = Table(self.path, self.pageSize)
                self.tables[id].setTableNumber(id)
                self.tables[id].setTableFDP(coltypOrPgnoFDP)
                self.tables[id].setTableName(name)
                self.tables[objidTable].setLongValueFDP(id)
            

             

    

        
        