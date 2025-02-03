This code contains a Sniper class (with the three default member initialize, execute and finalize) needed to make it work. It reads the event navigator
via a Sniper pointer called m_buf that can access all the header files.
Via the headers of different classes you can access the Evt trees and data. By default, this program creates a tree with Q/T pairs, timestamps and 
trigger types.

HOW TO LAUNCH:
1) Create the library Run_reader_cxx.so launching the command "root -l Run_reader.cxx++"
2) Launch the python code Export_run.py with options :
    python Export_run.py -inFile Data_rootfile_to_read -out Rootfile_containing_the_output_tree