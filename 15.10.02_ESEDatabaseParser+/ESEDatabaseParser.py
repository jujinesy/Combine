import Error
import sys
from DatabaseHeader import DatabaseHeader
from HexReader import HexReader
from Page import Page
from MsysObject import MsysObject
from WriteSqlite import WriteSqlite

class ESEDatabaseParser :
    def __init__(self, inputPath, outputPath, swich) :
        self.inputPath = inputPath
        self.outputPath = outputPath
        self.swich = swich
        self.init()
    
    def init(self) :
        self.status = None
        self.pageSize = None
        self.version = None
        self.revision = None
        self.pageHeaderSize = None
        self.msysObject = None
        self.tables = None 
        self.hexReader = HexReader(self.inputPath)
        
    def run(self):
        self.parseDatabaseHeader()
        self.msysObject = MsysObject(self.inputPath, self.pageSize, self.swich)
        self.tables = self.msysObject.getTables()
        self.parseNormalPageList()
        self.writeSqlite = WriteSqlite(self.outputPath)
        self.printDatabaseInfo()
        if self.swich == True :
            self.parseAbnormalData()
        self.parseNormalData()
        
        print "\n--------------------------------------------------------\n"
    
    def parseNormalPageList(self) :
        for id in self.tables.keys() :
            sys.stdout.write("\r\t Parsing : " + str(self.tables[id].getTableName()))
            sys.stdout.flush()
            self.tables[id].findNormalPageList()
            sys.stdout.write("\r                                                                                   ")
            sys.stdout.flush()
            
    def parseNormalData(self) :
        for id in sorted(self.tables.keys()) :
            try :
                if self.tables[id].getTableName() == "LV" :
                    continue
    
                lvID = self.tables[id].getLongValueFDP()
                table = self.tables[id]
                lvTable = None
                if lvID != -1 :
                    lvTable = self.tables[lvID]
                
                fieldList = self.getFieldList("Normal", table, lvTable)
                self.writeSqlite.write(table, fieldList, "[" + table.getTableName() + "]")
            except :
                print "\t  Fail : " + self.tables[id].getTableName()
    
    def parseAbnormalData(self) :
        self.parseAbnormalPage()
        for id in sorted(self.tables.keys()) :

            if self.tables[id].getTableName() == "LV" :
                continue
            
            
            lvID = self.tables[id].getLongValueFDP()
            table = self.tables[id]
            lvTable = None
            if lvID != -1 :
                lvTable = self.tables[lvID]
            fieldList = self.getFieldList("Abnormal", table,  lvTable)
            if len(fieldList) == 0 :
                continue
            self.writeSqlite.write(table, fieldList, "[Carved_" + table.getTableName() + "]")
            
    def getFieldList(self, status, table, lvTable = None ) :
        fieldList = []
        recordList = []
        lvDatas = {}
        if lvTable != None : 
            lvDatas = lvTable.getLvDatas()
        
        if status == "Normal" :
            recordList = table.getNormalData() 
        elif status == "Abnormal" :
            recordList = table.getAbnormalData()
               
        for record in recordList :
        
            datas = record.getAllDatas()
            datas = self.dictToSortedTuple(datas)
            field = ()
            for i in range(len(datas)) :
                if type(datas[i]) == type(()) : #tuple
                    if datas[i][0] == "value" :
                        """
                        if type(datas[i][1]) == type("") and len(datas[i][1]) > self.pageSize :
                            field = field + (datas[i][1][:self.pageSize],) #parsing error : length
                        else :
                        """
                        field = field + (datas[i][1],)
                    else : #pointer
                        if datas[i][1] in lvDatas.keys() :
                            field = field + (lvDatas[datas[i][1]], )
                        else :
                            field = field + ("parsingError(PointerNum : " + str(datas[i][1]) + ")" , )
                else :
                    field = field + (datas[i], )
            if len(field) != len(table.getColumns()) : 
                continue
            fieldList.append(field)
        return fieldList
    
    def printDatabaseInfo(self) :
        print "\n"
        print "\t ESE Database Parser v0.0.1"
        print "\t by Jeonghyeon Kim (americano@korea.ac.kr)"
        print "\n--------------------------------------------------------\n"
        print "\t Input File : " + self.inputPath    
        print "\t Status : " + self.status
        print "\t Page Size : " + str(self.pageSize)
        print "\t Version : " + hex(self.version)
        print "\t Revision : " + hex(self.revision)
        print "\t [Total Tables] : " + str(len(self.tables.keys()))
        print ""
    
        
    def parseDatabaseHeader(self) :
        databaseHeader = DatabaseHeader(self.inputPath)
        databaseHeader.parseDabaseHeader()
        if databaseHeader.getSignature() != -1985229329 : #\xef\xcd\xab\x89
            Error.printError(0)
        
        self.status = databaseHeader.getStatus()
        self.pageSize = databaseHeader.getPageSize()
        self.version = databaseHeader.getVersion()
        self.revision = databaseHeader.getRevision()
        self.pageHeaderSize = databaseHeader.getPageHeaderSize()
        databaseHeader = None
    
    def parseAbnormalPage(self) :
        #get abnormal page list
        totalPageNumber = self.hexReader.getFileSize() / self.pageSize 
        for currentPageNumber in range(1, totalPageNumber-1) :
            sys.stdout.write("\r\t Carving page : " + str(currentPageNumber) +"/" + str(totalPageNumber-2)+ "")
            sys.stdout.flush()
            pageSize = self.pageSize
            offset = (currentPageNumber+1) * pageSize
            fatherDataPageNumber = self.hexReader.readInteger(offset + 24)
            if not fatherDataPageNumber in self.tables.keys() :
                continue
            else :
                self.tables[fatherDataPageNumber].addAbnormalPageList(currentPageNumber)
            sys.stdout.write("\r                                                                        ")
            sys.stdout.flush()
        print "\n"
        
    def dictToSortedTuple(self, _dict):
        _tuple = ()
        keys = _dict.keys()
        keys.sort()
        for key in keys :
            _tuple = _tuple + (_dict[key],)
        
        return _tuple
 
    
    
    