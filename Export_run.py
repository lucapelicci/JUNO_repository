import sys
import Sniper
import argparse

parser = argparse.ArgumentParser()
parser.add_argument('-inFile',help='name of the input file',required=True)
parser.add_argument('-out',help='name of the output file',required=True)

args = parser.parse_args()

Sniper.loadDll("Run_reader_cxx.so")
#Sniper.loadDll("libSimEvent.so")

inputFileList = [args.inFile]
outfilename = args.out

task = Sniper.Task("task")
task.setLogLevel(0)

alg = task.createAlg("Run_reader")

import BufferMemMgr
bufMgr = task.createSvc("BufferMemMgr")

import RootWriter
task.property("svcs").append("RootWriter")
rw = task.find("RootWriter")
rw.property("Output").set({"tree":outfilename})

import RootIOSvc
riSvc = task.createSvc("RootInputSvc/InputSvc")
riSvc.property("InputFile").set(inputFileList)

task.setEvtMax(-1)
task.show()
task.run()
