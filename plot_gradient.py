import numpy as np
import plotly.graph_objs as go
import plotly.io as pio

# Read data
data = np.loadtxt('geodesic.txt')
tau, x, y, z = data.T

M = 1.0
r_horizon = 2 * M

# Text for initial conditions legend
initial_conditions_text = (
    f"Initial conditions:<br>"
    f"r = 5.5 M<br>"
    f"θ = π/4 rad<br>"
    f"φ = 0.00 rad<br>"
    f"v<sub>r</sub> = 0.0<br>"
    f"v<sub>θ</sub> = 0.1M<br>"
    f"v<sub>φ</sub> = -0.2M<br>"
)

# Make an empty trace for initial conditions
trace_conditions = go.Scatter3d(
    x=[None],  # No mostrar datos reales
    y=[None],
    z=[None],
    mode='markers',
    marker=dict(size=0),
    name=initial_conditions_text,
    showlegend=True,
    hoverinfo='none'
)

# Create the 3D trajectory with color gradient
trace_path = go.Scatter3d(
    x=x,
    y=y,
    z=z,
    mode='lines',
    line=dict(
        color=tau,
        colorscale='RdBu',  # Use a standard color scale
        width=6,
        colorbar=dict(title='Proper time τ'),
        cmin=np.min(tau),
        cmax=np.max(tau)
    ),
    name='Trajectory'
)

# Create the event horizon surface
u = np.linspace(0, 2 * np.pi, 50)
v = np.linspace(0, np.pi, 50)
X = r_horizon * np.outer(np.cos(u), np.sin(v))
Y = r_horizon * np.outer(np.sin(u), np.sin(v))
Z = r_horizon * np.outer(np.ones_like(u), np.cos(v))

trace_horizon = go.Surface(
    x=X,
    y=Y,
    z=Z,
    colorscale=[[0, 'darkgray'], [1, 'darkgray']],
    opacity=0.5,
    showscale=False,
    name='Event horizon (r=2M)',
    showlegend=True
)

# Create the figure
fig = go.Figure(data=[trace_path, trace_horizon, trace_conditions])

# Configure layout
fig.update_layout(
    title='Geodesic in Schwarzschild spacetime',
    scene=dict(
        xaxis_title='x [M]',
        yaxis_title='y [M]',
        zaxis_title='z [M]',
        aspectmode='data',
        camera=dict(
            up=dict(x=0, y=0, z=1),
            center=dict(x=0, y=0, z=0),
            eye=dict(x=1.5, y=1.5, z=0.5)
        ),
        xaxis=dict(backgroundcolor="gainsboro"),
        yaxis=dict(backgroundcolor="gainsboro"),
        zaxis=dict(backgroundcolor="gainsboro")
    ),
    margin=dict(l=0, r=0, b=0, t=30),
    autosize=True,
    legend=dict(
        x=0.02,  # Move left for better text visibility
        y=0.98,  # Vertical position at the top
        bgcolor='rgba(255,255,255,0.8)',  # More opaque background for better readability
        bordercolor='Black',
        borderwidth=1,
        font=dict(
            size=15  # Smaller font size for long text
        ),
        itemsizing='constant'  # For consistent symbol sizing
    )
)

# Save the figure with additional configuration
pio.write_html(
    fig, 
    file="geodesic_plot.html",
    auto_open=True,  # To open automatically in the browser
    include_plotlyjs='cdn',  # Use CDN for Plotly.js (reduces file size)
    full_html=True
)
