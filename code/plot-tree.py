import urllib.request

filename = "tree.txt"

edges = ""

with open(filename, 'r', encoding='utf-8') as f:
    for line in f:
        line_processed = line.strip().split(" ")
        if len(line_processed)!=2:
            continue
        edges += line_processed[0]+"->"+line_processed[1]+";"

urllib.request.urlretrieve(f"https://chart.googleapis.com/chart?cht=gv&chl=digraph{{{edges}}}", "tree.png")