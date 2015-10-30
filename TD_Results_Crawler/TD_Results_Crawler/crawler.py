import os
import re

directory = ".\\logs"

cnt = 0
strats = dict()
epochs = dict()

for root, dirs, files in os.walk(directory):
    for dir in dirs:
        epochs[0] = 0
        epochs[1] = 0
        epochs[2] = 0
        epochs[3] = 0
        epochs[4] = 0
        epochs[5] = 0
        epochs[6] = 0
        epochs[7] = 0
        epochs[8] = 0
        epochs[9] = 0

        if dir.startswith("Q"):
            for root, dirs, files in os.walk(directory):
                for file in files:
                    if file.startswith(dir):                
                        f=open(directory + "\\" + dir + "\\" + file, 'r')
                        line_cnt = 0
                        
                        for line in f:
                            m = re.search(r"\((?P<score>\d+)\)", line)
                            if m:
                                epoch = line_cnt/100
                                if epoch == 10:
                                    epoch = 9
                                epochs[epoch] = epochs[epoch] + float(m.group("score"))/(100*100*101*42)
                                line_cnt = line_cnt + 1
                                if line_cnt >= 1000:
                                    line_cnt = 0

                        f.close()
                        cnt = cnt + 1
                        if cnt % 9 == 0:
                            print cnt/(42.0*42.0)

            strats[dir] = epochs.values()
print "Analysis complete"

for strat in strats.keys():
    line = strat
    for score in strats[strat]:
        line = line + "\t" + str(score)

    print line
