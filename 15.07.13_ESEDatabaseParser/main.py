import ESEDatabaseParser

import sys, os
if __name__=="__main__":
    if len(sys.argv) != 3:
        print "Usage : ESEDatabaseParser [Input File Path] [Output File Path]"
        sys.exit()
    
    if not os.path.isfile(sys.argv[1]) :
        print "[Error] can't open the file"
        sys.exit()


    ESEDatabaseParser = ESEDatabaseParser.ESEDatabaseParser(sys.argv[1], sys.argv[2])
    ESEDatabaseParser.run()