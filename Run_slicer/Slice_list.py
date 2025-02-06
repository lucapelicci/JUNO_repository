import os
import re
import argparse

parser = argparse.ArgumentParser(description="Code that slices a list file")

parser.add_argument("-run", help="Run number", required=True)
parser.add_argument("-slice",help="Length of each list file",default=10)
parser.add_argument("-MM",help="Use to slice MM trigger runs",action="store_true")
parser.add_argument("-launch",help="Launch the run after slicing",action="store_true")
parser.add_argument("-reco",help="Launch the reco on the sliced files",action="store_true")
#parser.add_argument("-single",help="Use to create only a list with the first path")
args = parser.parse_args()

additional_launch_arguments = ""

if (args.reco) :
    additional_launch_arguments += " -reco"
if (args.MM) :
    additional_launch_arguments += " -MM -name _MMtrigger" 
    file_to_slice = "rawfile_RUN" + args.run + "_MMtrigger.list"
else :
    file_to_slice = "rawfile_RUN" + args.run + ".list"

filepath = "/junofs/users/claudiocoletta/Light_off/J25.1.0/list/" + file_to_slice

if not os.path.exists(filepath):
    print(f"ERROR: file to slice {filepath} does not exist")
    exit(1)

store_lines = []
with open(filepath) as file :
    for line in file :
        store_lines.append(line)
j = 0
for i in range(0, len(store_lines), args.slice):
    chunk = store_lines[i:i + args.slice]

    if (args.MM) :
        filename = f"/junofs/users/claudiocoletta/Light_off/J25.1.0/list/rawfile_RUN{args.run}_{j:03}_MMtrigger.list"
    else :
        filename = f"/junofs/users/claudiocoletta/Light_off/J25.1.0/list/rawfile_RUN{args.run}_{j:03}.list"
    
    with open(filename, "w") as fileout:
        for item in chunk:
            fileout.write(str(item))  # Write each element on a new line

    print (f"python Launch_rtraw2root.py -RUN {args.run} -cpTrees -fileNumber _{j:03} {additional_launch_arguments}")
    #os.system(f"python Launch_rtraw2root.py -RUN {args.run} -cpTrees -reco -fileNumber _{j:03} {additional_launch_arguments}")

    j = j + 1
