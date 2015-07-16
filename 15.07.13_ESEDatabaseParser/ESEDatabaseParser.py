import Error
from DatabaseHeader import DatabaseHeader
from Page import Page
from MsysObject import MsysObject
from WriteSqlite import WriteSqlite
class ESEDatabaseParser :
    def __init__(self, inputPath, outputPath) :
        self.inputPath = inputPath
        self.outputPath = outputPath
        self.init()
    
    def init(self) :
        self.status = None
        self.pageSize = None
        self.version = None
        self.revision = None
        self.pageHeaderSize = None
        self.msysObject = None
        self.tables = None 
        
    def run(self):
        self.parseDatabaseHeader()
        self.msysObject = MsysObject(self.inputPath, self.pageSize)
        self.tables = self.msysObject.getTables()
        writeSqlite = WriteSqlite(self.outputPath)
        self.printDatabaseInfo()
        for id in sorted(self.tables.keys()) :
            
            if self.tables[id].getTableName() == "LV" :
                continue
            lvID = self.tables[id].getLongValueFDP()
            if lvID != -1 :
                writeSqlite.write(self.tables[id], self.tables[lvID])
            else :
                writeSqlite.write(self.tables[id])
            
            #recordList = self.tables[id].getNormalData()
            #print id, len(recordList)
        
        #self.page = Page(self.inputPath, self.pageSize, 0x70000, 1)
        print "\n--------------------------------------------------------\n"
    
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
        

 
    
    
    