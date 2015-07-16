import sys

def printError(ErrorID) :
    if ErrorID == 0 :
        print "[Error] input file is not ESE Database"
    if ErrorID == 1 :
        print "[Error] size is minus"
    sys.exit()
        
        
    