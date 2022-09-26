#!/usr/bin/python3

file_input_path = ["util/word3.txt", "util/word1.txt", "util/word2.txt"]


for path in file_input_path:

        hex_list = list()

        #print()
        with open(path) as f:
                data = f.readlines()

        for d in data:
                d = d.replace("\n","")
                hex_data = int(d, 2) #hex()
                hex_list.append(hex_data)
                #print(d)
        print()

        for h in hex_list:
                hexv = '{0:04x}'.format(h)
                print("0x" + hexv, end=', ')
                #print(h)
        #print() 
