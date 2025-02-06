#It takes from input the RUN number

import os
import sys
import argparse
from argparse import RawTextHelpFormatter

prs = argparse.ArgumentParser(formatter_class=RawTextHelpFormatter)

prs.add_argument("-name","--NameRun", default="", help="Name of the run to add to the number of the run, e.g. _15ADC")
prs.add_argument("-t","--threshold", default=15,help="For Calib: fixed threshold for seeking a pulse (ADC)")
prs.add_argument("-RUN","--WhichRun",help="Number of the run to reco", required=True)
prs.add_argument("-MM","--isMMTrigger",help="Use the MM trigger list (ds-3)", action="store_true")
prs.add_argument("-reco","--rtraw2rec", action="store_true", help="Launch rtraw2rec")
prs.add_argument("-cpTrees","--copyTrees", action="store_true", help="Copy all rtraw trees in the results")
prs.add_argument("-cpWFTree","--copyWaveformTree", action="store_true", help="Copy the waveform tree in the results")
prs.add_argument("-mem", default=0, help="Set the memory usage")
prs.add_argument("-fileNumber",help="Set an additional number for the run (put _ at the beginning for formatting reasons)",default="")


args = prs.parse_args()
cwd=os.getcwd()
run=str(args.WhichRun)
ts=str(args.threshold)
name=str(args.NameRun)


if (args.isMMTrigger):
    list_name=cwd+"/list/rawfile_RUN"+run+args.fileNumber+"_MMtrigger.list"
else :
    list_name=cwd+"/list/rawfile_RUN"+run+args.fileNumber+".list"

if not os.path.exists(list_name):
    raise FileNotFoundError("The list file provided "+list_name+" does not exist")

name = args.fileNumber + name
sh_file=cwd+"/sh/rtraw2root_RUN"+run+name+".sh"
root_file=cwd+"/root/RUN"+run+name+".root"
root_file_calib=cwd+"/root/RUN"+run+name+"_calib.root"
user_file=cwd+"/root/user_RUN"+run+name+".root"
log_file=cwd+"/log/RUN"+run+name+".log"
err_file=cwd+"/err/RUN"+run+name+".err"
out_file=cwd+"/out/RUN"+run+name+".out"
mem_string = ""

if (args.mem != 0) :
    mem_string = " -mem " + args.mem

if (args.copyTrees) :
    copy_trees = " --output-stream /Event/WpElec_FPGA:on --output-stream /Event/WpElec_OECRaw:on --output-stream /Event/CdLpmtElec_OECRaw:on --output-stream /Event/CdLpmtElec_FPGA:on --output-stream /Event/WpTrigger:on --output-stream /Event/CdTrigger:on" 
else :
    copy_trees = ""

if (args.copyWaveformTree) :
    copy_trees = copy_trees + " --output_stream /Event/CdWaveform:on"

threshold=""
if (ts!="15"): threshold=" --COTIThreshold "+ts+" --COTIThresholdFlat "+ts

of=open(sh_file,"w")
of.write('#!/bin/bash\n')
of.write('import time\n')
of.write("source /cvmfs/juno.ihep.ac.cn/el9_amd64_gcc11/Release/J25.1.3/setup.sh\n")

of.write("time python ${TUTORIALROOT}/share/tut_rtraw2calib.py --evtmax -1 --input-list "+list_name+" --output "+root_file_calib+copy_trees+" --waverec-method cotiwaverec --Calib 1 --pmtcalibsvc-ReadDB 0 "+threshold+" >& " + log_file + "\n")
if (args.rtraw2rec):
    of.write('time.sleep(10)\n')
    of.write("time python ${TUTORIALROOT}/share/tut_calib2rec.py --evtmax -1 --input "+root_file_calib+" --output "+root_file+copy_trees+threshold+" --recMethod waterphase --recVersion OEC --waterPhase --method qctr >& " + log_file + "\n")


of.close()

os.system('chmod 774 ' + sh_file)

os.system("hep_sub -g juno" + mem_string +" -wt mid -o "+out_file+" -e "+err_file+" "+sh_file)

