
import matplotlib.pyplot as plt
import math
from pyntcloud import PyntCloud
import numpy as np
import matplotlib.tri as mtri
import pandas as pd

file = open("D:\TEST2.txt", "r")
datadist = file.readlines()
file.close()

i = 0
while i < len(datadist):
    datadist[i] = datadist[i].strip()
    i += 1

datadist = [eval(i) for i in datadist]
i = 0

while i < len(datadist):
    datadist[i] = 22.0 - datadist[i]
    i += 1


spcirc = 200
spcirctemp = spcirc

zdata = []
rise = 0
for i in range(len(datadist)):
    zdata.append(rise)
    if i > spcirctemp:
        rise += 0.5  # change after built final
        spcirctemp += spcirc

xdata = []
theta = 0.0
for i in range(len(datadist)):
    xdata.append(datadist[i] * (math.cos(math.radians(theta))))
    theta += 1.8

ydata = []
theta = 0.0
for i in range(len(datadist)):
    ydata.append(datadist[i] * (math.sin(math.radians(theta))))
    theta += 1.8

zdata = np.asarray(zdata)
xdata = np.asarray(xdata)
ydata = np.asarray(ydata)

data = {"x": xdata, "y": ydata, "z": zdata}

cloud = PyntCloud(pd.DataFrame(data))
cloud.to_file("plydatapoints.ply")

triang = mtri.Triangulation(xdata, ydata)

scaned = PyntCloud.from_file("plydatapoints.ply")
convex_hull_id = scaned.add_structure("convex_hull")
convex_hull = scaned.structures[convex_hull_id]
scaned.mesh = convex_hull.get_mesh()
scaned.to_file("plydata_mesh.ply", also_save=["mesh"])


# IGNORE BELOW -------------

fig = plt.figure(figsize=plt.figaspect(0.5))
ax = fig.add_subplot(1, 2, 2, projection="3d")
# ax.plot_trisurf(xdata, ydata, zdata, triangles=triang.triangles, cmap='viridis', edgecolor='none')
ax.scatter(xdata, ydata, zdata, c=zdata, cmap="viridis", linewidth=0.5)
ax.scatter3D(xdata, ydata, zdata, c=zdata, cmap="Greens")
# plt.show()

"""
data = np.zeros(len(triang.triangles), dtype=mesh.Mesh.dtype)
mobius_mesh = mesh.Mesh(data, remove_empty_areas=False)
mobius_mesh.x[:] = xdata[triang.triangles]
mobius_mesh.y[:] = ydata[triang.triangles]
mobius_mesh.z[:] = zdata[triang.triangles]
mobius_mesh.save('scanned_obj1.stl')
"""

