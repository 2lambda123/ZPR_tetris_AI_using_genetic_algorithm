import json
import sys
import os
import matplotlib.pyplot as plt

def loadJSONData(path):
    x = []
    y = []
    with open(path) as json_f:
        data = json.load(json_f)

        x = [i for i in range(len(data))]

        for i in x:
            y.append(data[i]["score"])
    return x, y

json_files = [pos_json for pos_json in os.listdir(".") if pos_json.endswith('.json')]

plt.xlabel("numer generacji")
plt.ylabel("najwyższy wynik w generacji")
plt.xticks([i for i in range(25)])
for json_f in json_files:
    x, y = loadJSONData(json_f)
    line = plt.plot(x, y, label=json_f.replace('.json', ''))
    plt.legend()
plt.show()

