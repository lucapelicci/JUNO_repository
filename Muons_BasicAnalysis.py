import numpy as np
import matplotlib.pyplot as plt
import sys
import uproot
import seaborn as sns
import corner 
import pandas as pd

def intersects_cap(volume, entry, exit, R=17700):
    
    # spherical cap height (this will be moved in a separate function, otherwise uselessly time consuming
    coeffs = [np.pi / 3, -np.pi * R, 0, volume]  
    roots = np.roots(coeffs)
    
    h_values = [r.real for r in roots if np.isreal(r) and 0 <= r.real <= 2 * R]
    
    h = min(h_values)
        
    # cap z
    z_min = R - h
    
    # Line parametric equation: P(t) = entry + t * (exit - entry)
    # Solve equation defining the intersection: |P(t)| = R
    p0 = np.array(entry)
    p1 = np.array(exit)
    d = p1 - p0  # Segment direction

    a = np.dot(d, d)
    b = 2 * np.dot(p0, d)
    c = np.dot(p0, p0) - R**2
    delta = b**2 - 4*a*c

    if delta < 0:
        return 0  # no intersections

    t1 = (-b - np.sqrt(delta)) / (2 * a)
    t2 = (-b + np.sqrt(delta)) / (2 * a)

    # Intersection points with the sphere
    p_int1 = p0 + t1 * d
    p_int2 = p0 + t2 * d

    # is any intersection point located in the spherical cap? (z >= z_min)
    if p_int1[2] >= z_min or p_int2[2] >= z_min:
        return 1  # intersection found

    return "0"  # no intersection

def distance_segment_sphere(x1, y1, z1, x2, y2, z2, xc, yc, zc, r):

    P1 = np.array([x1, y1, z1])
    P2 = np.array([x2, y2, z2])
    C = np.array([xc, yc, zc])
    
    d = P2 - P1 
    f = P1 - C
    
    t = -np.dot(f, d) / np.dot(d, d)
    t = np.clip(t, 0, 1)
    closest_point = P1 + t * d
    
    min_dist = np.linalg.norm(closest_point - C)
    
    return min_dist - r if min_dist > r else min_dist - r


xc, yc, zc, r = 0, 0, 0, 17700

filename = sys.argv[1]

with uproot.open(filename) as file:
    tree = file["MuonEvents"]
    
    evt_id = tree["EvtID"].array().to_numpy()
    fSec = tree["fSec"].array().to_numpy()
    fNanoSec = tree["fNanoSec"].array().to_numpy()
    pe_sum = tree["PeSum"].array().to_numpy()
    
    x_entry = tree["inPos/_0"].array().to_numpy()
    y_entry = tree["inPos/_1"].array().to_numpy()
    z_entry = tree["inPos/_2"].array().to_numpy()
    
    x_exit = tree["outPos/_0"].array().to_numpy()
    y_exit = tree["outPos/_1"].array().to_numpy()
    z_exit = tree["outPos/_2"].array().to_numpy()
    
    ux = tree["Direction/_0"].array().to_numpy()
    uy = tree["Direction/_1"].array().to_numpy()
    uz = tree["Direction/_2"].array().to_numpy()
   
    pos_quality = tree["PosQuality"].array().to_numpy()


bad_muons = sum(1 for x in x_exit if x == -999999)
print("Number of badly reco muons: " + str(bad_muons))

mask = x_exit != -999999  # remove muons without entry or exit points

absolute_time = fSec + fNanoSec * 1e-9
time = absolute_time - absolute_time[0]
time_diff = np.diff(time)

x_entry, y_entry, z_entry = x_entry[mask], y_entry[mask], z_entry[mask]
x_exit, y_exit, z_exit = x_exit[mask], y_exit[mask], z_exit[mask]
ux, uy, uz = ux[mask], uy[mask], uz[mask]

entry = (x_entry, y_entry, z_entry)
exit = (x_exit, y_exit, z_exit)

x_average = 0.5 * (x_exit + x_entry)
y_average = 0.5 * (y_exit + y_entry)
z_average = 0.5 * (z_exit + z_entry)

inward = x_average * ux + y_average * uy + z_average * uz

min_distances = np.array([
    distance_segment_sphere(x1, y1, z1, x2, y2, z2, xc, yc, zc, r)
    for x1, y1, z1, x2, y2, z2 in zip(x_entry, y_entry, z_entry, x_exit, y_exit, z_exit)
])

track_length = np.sqrt((x_exit - x_entry) ** 2 + (y_exit - y_entry) ** 2 + (z_exit - z_entry) ** 2) / 1000.

costheta = uz / np.sqrt(ux**2 + uy**2 + uz**2)
phi = np.sign(uy) * np.arccos(ux / np.sqrt(ux**2 + uy**2))

print(f"Clipping muons (track length < 10 m) : {len(track_length[track_length < 10])}")

print(f"Numero di eventi che intersecano la sfera: {len(min_distances[min_distances < 0])}")
print(f"Numero di eventi che NON intersecano la sfera: {len(min_distances[min_distances > 0])}, {len(min_distances[min_distances < 0]) / len(min_distances)}")

plots = 'true'

with open(sys.argv[2], "w") as f:
    for t, charge, x_e, y_e, z_e, x_x, y_x, z_x in zip(absolute_time, pe_sum, x_entry, y_entry, z_entry, x_exit, y_exit, z_exit):
        crossingLS = float(intersects_cap(40*1e9, (x_e, y_e, z_e), (x_x, y_x, z_x), R=17700))
        f.write(f"{t:.6f}\t{charge:.0f}\t{x_e:.2f}\t{y_e:.2f}\t{z_e:.2f}\t{x_x:.2f}\t{y_x:.2f}\t{z_x:.2f}\t{crossingLS:.0f}\n")



if(plots == 'true'):
	fig, axs = plt.subplots(3, 3, figsize=(14, 7))
	axs[0, 0].hist(x_entry, bins=50, color='blue', alpha=0.4)
	axs[0, 0].set_title('x_entry')
	axs[0, 1].hist(y_entry, bins=50, color='blue', alpha=0.4)
	axs[0, 1].set_title('y_entry')
	axs[0, 2].hist(z_entry, bins=50, color='blue', alpha=0.4)
	axs[0, 2].set_title('z_entry')
	axs[0, 0].hist(x_exit, bins=50, color='red', alpha=0.4)	
	axs[0, 0].set_title('x_exit')
	axs[0, 1].hist(y_exit, bins=50, color='red', alpha=0.4)
	axs[0, 1].set_title('y_exit')
	axs[0, 2].hist(z_exit, bins=50, color='red', alpha=0.4)
	axs[0, 2].set_title('z_exit')
	axs[1, 0].hist(ux, bins=50, color='forestgreen', alpha=0.4)
	axs[1, 0].set_title('u_x')
	axs[1, 1].hist(uy, bins=50, color='forestgreen', alpha=0.4)
	axs[1, 1].set_title('u_y')
	axs[1, 2].hist(costheta, bins=50, color='orange', alpha=0.4)
	axs[1, 2].set_title('cos $\\theta$')
	axs[2, 0].hist(time, bins=100, color='purple', alpha=0.4)
	axs[2, 0].set_title('time from run start [s]')
	axs[2, 1].hist(time_diff, bins=100, range=(0,10), color='purple', alpha=0.4)
	axs[2, 1].set_title('consecutive time diff. [s]')
	axs[2, 1].set_yscale('log')
	axs[2, 2].hist(phi, bins=50, color='orange', alpha=0.4)
	axs[2, 2].set_title('$\phi$')
	
	plt.tight_layout()
	plt.show()


	fig, axf = plt.subplots(2, 2, figsize=(14, 7))
	axf[0,0].hist(track_length, bins=50, color='blue', alpha=0.4)
	axf[0,0].set_title('Track Length [m]')
	axf[1,0].hist(min_distances / 1000., bins=50, color='blue', alpha=0.4)
	axf[1,0].set_title('Min. distance from CD center [m]')  
	axf[0,1].hist(pe_sum, bins=100, color='blue', alpha=0.4)
	axf[0,1].set_title('pe_sum [p.e.]')
	axf[1,1].hist(pos_quality, bins=100, color='blue', alpha=0.4)
	axf[1,1].set_title('Position Quality')  
	plt.tight_layout()
	plt.show()


	fig, axs = plt.subplots(1, 3, figsize=(10, 4))

	axs[0].scatter(phi, pe_sum, alpha=0.02, s=5, color="dodgerblue")
	axs[0].set_xlabel("$\phi$ (azimuth.)")
	axs[0].set_ylabel("peSum")
	axs[0].set_title("peSum vs $\phi$")

	axs[1].scatter(pe_sum, pos_quality, alpha=0.02, s=5, color="dodgerblue")
	axs[1].set_xlabel("peSum")
	axs[1].set_ylabel("PosQuality")
	axs[1].set_title("peSum vs PosQuality")

	axs[2].scatter(costheta, pe_sum, alpha=0.02, s=5, color="dodgerblue")
	axs[2].set_xlabel("cos$\\theta$ (azimuth.)")
	axs[2].set_ylabel("peSum")
	axs[2].set_title("peSum vs cos$\\theta$")

	plt.tight_layout()
	plt.show()

