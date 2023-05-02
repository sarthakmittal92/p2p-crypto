import urllib.request

filename = "peers.txt"

edges = ""
count = 0

with open(filename, 'r', encoding='utf-8') as f:
    for line in f:
        line_processed = line.strip().split(" ")
        if len(line_processed)!=2:
            urllib.request.urlretrieve(f"https://chart.googleapis.com/chart?cht=gv&chl=digraph{{{edges}}}", f"tree-files/peer{count}.png")
            count += 1
            edges = ""
            continue
        edges += line_processed[1].split('.')[0]+"->"+line_processed[0].split('.')[0]+";"
    urllib.request.urlretrieve(f"https://chart.googleapis.com/chart?cht=gv&chl=digraph{{{edges}}}", f"tree-files/peer{count}.png")