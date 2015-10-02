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
    
    def write(self, table, fieldList, tableName) :
        self.tableName = tableName
        self.queryCreateTable(table)
        self.queryInsert(table, fieldList)
        
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
       
    def queryInsert(self, table, fieldList) :
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
        for field in fieldList :
            try :
                #self.cursor.executemany(query, fieldList)
                self.cursor.execute(query, field)
            except : 
                print field[0]
        
        self.sqliteDB.commit()    
    
        print "\t " + self.tableName + " : " + str(len(fieldList)) 
            

    
    
                
                
                
        
        
        
        
        
        
        
            
            
            
        
    
    
    
        