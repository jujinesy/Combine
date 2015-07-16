import sqlite3


class WriteSqlite :
    def __init__(self, DbName) :
        self.DbName = DbName
        self.init()
        
    def init(self) :
        self.sqliteDB = sqlite3.connect(self.DbName)
        self.sqliteDB.text_factory = str
        self.cursor = self.sqliteDB.cursor()
        self.tableName = "None"
        
    def __del__(self) :
        self.sqliteDB.close()
    
    def write(self, table, lvTable = None) :
        self.tableName = "[" + table.getTableName() + "]"
        self.queryCreateTable(table)
        self.queryInsert(table, lvTable)
        
    def queryCreateTable(self, table) :
        columns = table.getColumns()
        columnIdList = columns.keys()
        columnIdList.sort()
        
        if len(columnIdList) == 0 :
            return 
        
        query = "CREATE TABLE IF NOT EXISTS " + self.tableName +" ("
        for columndId in columnIdList :
            name = columns[columndId].getName().replace("-","_")
            query += "_" + name + " "
            type = columns[columndId].getType()
            
            if type < 0 and type < 8 : 
                query += "int"
            else :
                query += "text"
                
            if columndId != columnIdList[len(columnIdList)-1] :  
                query += ", "
            
        query += ") "
        
        
        self.cursor.execute(query)
        self.sqliteDB.commit()
       
    def queryInsert(self, table, lvTable) :
        columns = table.getColumns()
        columnIdList = columns.keys()
        columnIdList.sort()
        
        if len(columnIdList) == 0 :
            return 
        
        query = "INSERT INTO " + self.tableName + " VALUES ("
        for columndId in columnIdList :
            query += "?"
            if columndId != columnIdList[len(columnIdList)-1] :  
                query += ","
        query += ")"
        
        
        fieldList = []
        lvRecordList = None
        lvDatas = {}
        if lvTable != None : 
            lvRecordList = lvTable.getNormalData()
            lvDatas = lvTable.getLvDatas(lvRecordList)
        
        recordList = table.getNormalData()    
        for record in recordList :
        
            datas = record.getAllDatas()
            datas = self.dictToSortedTuple(datas)
            field = ()
            for i in range(len(datas)) :
                if type(datas[i]) == type(()) : #tuple
                    if datas[i][0] == "value" :
                        field = field + (datas[i][1],)
                    else : #pointer
                        if datas[i][1] in lvDatas.keys() :
                            field = field + (lvDatas[datas[i][1]], )
                        else :
                            field = field + ("parsingError", )
                else :
                    field = field + (datas[i], )
            fieldList.append(field)

        self.cursor.executemany(query, fieldList)
        self.sqliteDB.commit()    
                
        print "\t [" + table.getTableName() + "] : " + str(len(recordList)) 
            
    def dictToSortedTuple(self, _dict):
        _tuple = ()
        keys = _dict.keys()
        keys.sort()
        for key in keys :
            _tuple = _tuple + (_dict[key],)
        
        return _tuple
    
    
                
                
                
        
        
        
        
        
        
        
            
            
            
        
    
    
    
        