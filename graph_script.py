# graph_script.py
import networkx as nx
import numpy as np
import matplotlib.pyplot as plt

# Crear un grafo dirigido
G = nx.DiGraph()
G2 = nx.DiGraph()

def my_draw_networkx_edge_labels(
    G,
    pos,
    edge_labels=None,
    label_pos=0.5,
    font_size=10,
    font_color="k",
    font_family="sans-serif",
    font_weight="normal",
    alpha=None,
    bbox=None,
    horizontalalignment="center",
    verticalalignment="center",
    ax=None,
    rotate=True,
    clip_on=True,
    rad=0
):
    """Draw edge labels."""
    if ax is None:
        ax = plt.gca()
    if edge_labels is None:
        labels = {(u, v): d for u, v, d in G.edges(data=True)}
    else:
        labels = edge_labels
    text_items = {}
    for (n1, n2), label in labels.items():
        (x1, y1) = pos[n1]
        (x2, y2) = pos[n2]
        (x, y) = (
            x1 * label_pos + x2 * (1.0 - label_pos),
            y1 * label_pos + y2 * (1.0 - label_pos),
        )
        pos_1 = ax.transData.transform(np.array(pos[n1]))
        pos_2 = ax.transData.transform(np.array(pos[n2]))
        linear_mid = 0.5*pos_1 + 0.5*pos_2
        d_pos = pos_2 - pos_1
        rotation_matrix = np.array([(0,1), (-1,0)])
        ctrl_1 = linear_mid + rad*rotation_matrix@d_pos
        ctrl_mid_1 = 0.5*pos_1 + 0.5*ctrl_1
        ctrl_mid_2 = 0.5*pos_2 + 0.5*ctrl_1
        bezier_mid = 0.5*ctrl_mid_1 + 0.5*ctrl_mid_2
        (x, y) = ax.transData.inverted().transform(bezier_mid)

        if rotate:
            # in degrees
            angle = np.arctan2(y2 - y1, x2 - x1) / (2.0 * np.pi) * 360
            # make label orientation "right-side-up"
            if angle > 90:
                angle -= 180
            if angle < -90:
                angle += 180
            # transform data coordinate angle to screen coordinate angle
            xy = np.array((x, y))
            trans_angle = ax.transData.transform_angles(
                np.array((angle,)), xy.reshape((1, 2))
            )[0]
        else:
            trans_angle = 0.0
        # use default box of white with white border
        if bbox is None:
            bbox = dict(boxstyle="round", ec=(1.0, 1.0, 1.0), fc=(1.0, 1.0, 1.0))
        if not isinstance(label, str):
            label = str(label)  # this makes "1" and 1 labeled the same

        t = ax.text(
            x,
            y,
            label,
            size=font_size,
            color=font_color,
            family=font_family,
            weight=font_weight,
            alpha=alpha,
            horizontalalignment=horizontalalignment,
            verticalalignment=verticalalignment,
            rotation=trans_angle,
            transform=ax.transData,
            bbox=bbox,
            zorder=1,
            clip_on=clip_on,
        )
        text_items[(n1, n2)] = t

    ax.tick_params(
        axis="both",
        which="both",
        bottom=False,
        left=False,
        labelbottom=False,
        labelleft=False,
    )

    return text_items

def inicializar_grafo():
    edges_with_weights = [(0, 1, 10),
                          (0, 3, 5),
                          (1, 2, 1),
                          (1, 3, 2),
                          (2, 4, 4),
                          (3, 1, 3),
                          (3, 2, 9),
                          (3, 4, 2),
                          (4, 0, 7),
                          (4, 2, 6)]
    
    G.add_weighted_edges_from(edges_with_weights)
    pos = nx.spring_layout(G)
    plt.figure(figsize=(8, 6))
    nx.draw(G, pos, with_labels=True, node_size=700, node_color="skyblue", font_size=10, font_weight="bold", edge_color="blue", connectionstyle='arc3, rad=0.1')
    edge_labels = {(u, v): d['weight'] for u, v, d in G.edges(data=True)}
    my_draw_networkx_edge_labels(G, pos, edge_labels=edge_labels, label_pos=0.5, font_size=7, rad=0.1)
    plt.title("Visualización del mapa inicial")
    plt.show()
    return "Mapa mostrado correctamente"

def mostrar_grafo():
    pos = nx.spring_layout(G)
    plt.figure(figsize=(8, 6))
    edge_colors = ['red' if G[u][v].get('is_available') == False else 'blue' for u, v in G.edges()]
    nx.draw(G, pos, with_labels=True, node_size=700, node_color="skyblue", font_size=10, font_weight="bold", edge_color=edge_colors, connectionstyle='arc3, rad=0.1')
    edge_labels = {(u, v): d['weight'] for u, v, d in G.edges(data=True)}
    my_draw_networkx_edge_labels(G, pos, edge_labels=edge_labels, label_pos=0.5, font_size=7, rad=0.1)
    plt.title("Visualización del mapa")
    plt.savefig("mapa_actual.png")
    plt.show()
    return "Mapa mostrado y descargado correctamente"

def add_node(node):
    G.add_node(node)
    return f"Node {node} added."

def agregar_tuberia(u, v, weight=1.0):
    G.add_edge(u, v, weight=weight)
    return "Tuberia correctamente."

def establecer_disponibilidad(u, v):
    if G.has_edge(u, v):
        G[u][v]['is_available'] = False
    return "Tubería no disponible."
