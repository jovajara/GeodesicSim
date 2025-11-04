# GeodesicSim - A Geodesic Calculator And Viewer

This is the final project repository for an elective Numerical Relativity course taken during my undergraduate physics degree at Universidad de Valparaíso.

The goal of this project was to numerically solve the Geodesic equation of a time-like particle in a Schwarzschild spacetime using the Leapfrog method and visualize it. 

Currently, it cannot compute a reliable solution, as angular momentum is not conserved. There are no current plans to resolve this issue.

C is used for the numerical computation, and Python generates an interactive 3D plot with Plotly.

#### Use
* To compile and run the program, run `make run` in the terminal. 
* To make the 3D plot make sure that Plotly is installed and then run  `make plot`.  It should open a browser window with your generated plot. If nothing happens, open the generated HTML file.
