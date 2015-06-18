#!/usr/bin/python

reg = {'eax' : 0, 'ebx' : 0,'ecx' : 0, 'edx' : 0, 'esi' : 0, 'edi' : 0, 'esp' : 0, 'ebp' : 0}
stack = []

def read(line, line_num):
    opcode = line.split()[0]
    if opcode == 'mov':
        mov(line)
    elif opcode == 'push':
        push(line)
    elif opcode == 'pop':
        pop(line)
    elif opcode == 'xor':
        xor(line)
    elif opcode == 'sub':
        sub(line)
    elif opcode == 'add':
        add(line)
    print "[(#" + str(line_num) + "] opcode: opcode_"+opcode

def mov(line):
    bHex = line.split()[1][0]
    fir = line.split()[1][1:-1]
    sec = line.split()[2][1:]
    if(bHex == '$'):
        if(line.split()[1][1] == '0'):
            reg[sec] = int(fir, 16)
        else:
            reg[sec] = int(fir)
    else:
        reg[sec] = reg[fir]
def push(line):
    fir = line.split()[1][1:]
    stack.append(reg[fir])
    reg['esp']-=4
def pop(line):
    fir = line.split()[1][1:]
    reg[fir] = stack.pop()
    reg['esp']+=4
def xor(line):
    bHex = line.split()[1][0]
    fir = line.split()[1][1:-1]
    sec = line.split()[2][1:]
    if(bHex == '$'):
        reg[sec] = (int(fir, 16) ^ int(reg[sec]))
    else:
        reg[sec] = (int(reg[fir]) ^ int(reg[sec]))
def sub(line):
    bHex = line.split()[1][0]
    fir = line.split()[1][1:-1]
    sec = line.split()[2][1:]
    if(bHex == '$'):
        if(line.split()[1][1] == '0'):
            reg[sec]-=int(fir, 16)
        else:
            reg[sec]-=int(fir)
    else:
        reg[sec]-=reg[fir]
def add(line):
    bHex = line.split()[1][0]
    fir = line.split()[1][1:-1]
    sec = line.split()[2][1:]
    if(bHex == '$'):
        if(line.split()[1][1] == '0'):
            reg[sec]+=int(fir, 16)
        else:
            reg[sec]+=int(fir)
    else:
        reg[sec]+=reg[fir]


fp = open('./homework_3.s')
line_num=0
while 1:
    line = fp.readline()
    if not line:
        break
    else:
        print line.split('\n')[0]
        read(line, line_num)
        line_num+=1
        print reg
        print