
import numpy as np
import matplotlib.pyplot as plt

file = open("data.txt", "r")


data = list(map(lambda x: x.strip(), file.readlines()))
data = list(map(lambda x: x.replace('[', '').replace(']', ''), data))
data = list(map(lambda x: x.split(','), data))

new_data = [[], [], [], [], [], [], [], [], []] 

for i in range(9):
    for d in data:
        new_data[i].append(round(float(d[i]), 6))

data = new_data

x = data[0]
y = data[1]
z = data[2]

print(len(x))

fig = plt.figure()
ax = fig.add_subplot(111, projection='3d')


# Plot the points
ax.scatter(x, y, z, c='r', marker='o')
plt.show()
