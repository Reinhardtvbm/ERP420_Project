import numpy as np
import matplotlib.pyplot as plt
from scipy.stats import norm

file = open("data.txt", "r")


data = list(map(lambda x: x.strip(), file.readlines()))
data = list(map(lambda x: x.replace('[', '').replace(']', ''), data))
data = list(map(lambda x: x.split(','), data))

new_data = [[], [], [], [], [], [], [], [], []] 

for i in range(9):
    for d in data:
        new_data[i].append(round(float(d[i]), 6))

data = new_data

means = [np.mean(x) for x in data]

for i in range(9):
    if i < 3:
        _range = 0.1
    elif i < 6:
        _range = 0.2
    else:
        _range = 0.03
#    data[i] = [x for x in data[i] if means[i] - _range <= x <= means[i] + _range]

titles = ["Magnetometer", "Gyroscope", "Accelerometer"]
axis_lims = [[-1, 1], [-1, 1], [-1, 1], [-0.1, 0.1], [-0.1, 0.1], [-0.1, 0.1], [-0.01, 0.01], [-0.005, 0.015], [1.0, 1.02]]

for i in range(0, 9, 3):
    print(titles[i//3])
    print("\tx:\n\t\tmean:\t\t" + str(np.mean(data[i])))
    print("\t\tvariance:\t" + str(np.var(data[i])))

    print("\ty:\n\t\tmean:\t\t" + str(np.mean(data[i + 1])))
    print("\t\tvariance:\t" + str(np.var(data[i + 1])))

    print("\tz:\n\t\tmean:\t\t" + str(np.mean(data[i + 2])))
    print("\t\tvariance:\t" + str(np.var(data[i + 2])))

print(len(data[0]))



for i in range(0, 9, 3):
    fig, axs = plt.subplots(3)
    fig.suptitle(titles[i//3])
    #axs[0].hist(data[i], bins = 1200, density=True)
    #axs[1].hist(data[i + 1], bins = 1200, density=True)
    #axs[2].hist(data[i + 2], bins = 1200, density=True)
    for j in range(3):
        hist, bins = np.histogram(data[i + j], bins=5000, density=True)
        bin_widths = bins[1:] - bins[:-1]
        normalized_hist = hist / len(data[i + j])

        # Calculate the mean and standard deviation of the data
        mean = np.mean(data[i + j])
        std_dev = np.std(data[i + j])

        # Create a range of x values for the Gaussian distribution plot
        x = np.linspace(min(data[i + j]), max(data[i + j]), 1000)

        # Calculate the estimated Gaussian distribution using mean and std_dev
        gaussian = norm.pdf(x, mean, std_dev)

        # Create a bar plot of the normalized histogram
        axs[j].bar(bins[:-1], normalized_hist, width=bin_widths, edgecolor='black', alpha=0.5, label='Histogram')

        # Plot the estimated Gaussian distribution
        axs[j].plot(x, gaussian / max(gaussian) / (1.0 / max(normalized_hist)), color='red', label='Gaussian Distribution')
        axs[j].set_title(['x', 'y', 'z'][j])
    

    plt.show()
