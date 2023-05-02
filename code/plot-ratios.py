import matplotlib.pyplot as plt
import os
import pathlib

directory = "./outputs"
plots_dir = "./plots/"
pathlib.Path(plots_dir).mkdir(exist_ok=True)

i = 1

for dir in os.listdir(directory):
    # if dir!='19': continue
    print(dir)
    filename = os.path.join(directory, dir, 'ratios.txt')
    print(filename)

    with open(filename, 'r' , encoding='utf-8') as f:
        total = 0
        ratio_slow = {}
        ratio_fast = {}
        ratio_lowCPU = {}
        ratio_highCPU = {}

        for line in f:
            line_split = line.strip().replace(':',' ').replace('(',' ').replace(')',' ').replace('|',' ').replace('  ',' ').split(' ')
            node_num = int(line_split[0])
            print(line_split)
            if line_split[-1]=='-1':
                line_split[-1] = '0'
            ratio = float(line_split[-1])
            if line_split[1]=='1':
                ratio_slow[node_num] = ratio
            else:
                ratio_fast[node_num] = ratio
            if line_split[2]=='1':
                # print("here!")
                ratio_lowCPU[node_num] = ratio
            else:
                ratio_highCPU[node_num] = ratio

        print(i, ratio_slow, ratio_fast, ratio_lowCPU, ratio_highCPU)

        fig, (ax1, ax2) = plt.subplots(1,2)

        ax1.scatter(ratio_slow.keys(), ratio_slow.values(), edgecolors='r', facecolors='none', label='slow')
        ax1.scatter(ratio_fast.keys(), ratio_fast.values(), edgecolors='b', facecolors='none', label='fast')
        ax2.scatter(ratio_lowCPU.keys(), ratio_lowCPU.values(), edgecolors='r', facecolors='none', label='lowCPU')
        ax2.scatter(ratio_highCPU.keys(), ratio_highCPU.values(), edgecolors='b', facecolors='none', label='highCPU')

        ax1.set_xticks(list(range(total)))
        ax2.set_xticks(list(range(total)))
        ax1.set_yticks([e/10 for e in list(range(11))])
        ax2.set_yticks([e/10 for e in list(range(11))])

        ax1.legend()
        ax2.legend()

        ax1.set_xlabel('Node')
        ax2.set_xlabel('Node')
        ax1.set_ylabel('Ratio')
        ax2.set_ylabel('Ratio')
        ax1.set_title('Slow vs Fast Nodes')
        ax2.set_title('Low CPU vs High CPU Nodes')

        fig.set_size_inches(15, 8)

        # plt.show()
        fig.savefig(os.path.join(plots_dir, f"{dir}.png"), bbox_inches="tight")

        i += 1