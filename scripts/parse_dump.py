import re
import sys

if len(sys.argv) < 3:
    print("Pass file to parse and to output")
    sys.exit()

file_to_parse = sys.argv[1]
file_to_out = sys.argv[2]

f = open(file_to_parse, "r")
lines = f.readlines()


after_removed1 = [a for a in lines if (re.search(r'[a-f0-9]:\t[a-f0-9]', a))] 

after_removed2 = [a.split(':\t',1)[1] for a in after_removed1]

after_removed3 = [a.split('\t',1)[0] for a in after_removed2]

after_removed4 = [a.strip() for a in after_removed3]

f2 = open(file_to_out, "w")
for item in after_removed4:
    f2.write("%s\n" % item)

