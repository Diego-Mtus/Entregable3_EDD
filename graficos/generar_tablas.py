import pandas as pd
import numpy as np
import matplotlib.pyplot as plt

# Código hecho con asistencia de gemini para generar las tablas leyendo el csv

df = pd.read_csv('resultados.csv', delimiter=';')

# Calcular Promedio y Desviación Estándar
df_stats = df.groupby(['estructura_de_datos', 'cantidad_consultas', 'dataset'])['tiempo_ejecucion'].agg(['mean', 'std']).reset_index()

# Mapeo de nombres de estructuras
mapa_estructuras = {
    'abierto': ('Hashing Abierto', 'hashing_abierto'),
    'lineal': ('Lineal', 'lineal'),
    'cuadratico': ('Cuadrático', 'cuadratico'),
    'doble': ('Hashing Doble', 'hashing_doble'),
    'unordered_map': ('std::unordered_map (STL)', 'unordered_map')
}

# generar una imagen por cada estructura
for est_codigo, (est_nombre, est_archivo) in mapa_estructuras.items():
    # Filtrar los datos de la estructura actual
    df_est = df_stats[df_stats['estructura_de_datos'] == est_codigo].copy()
    
    if df_est.empty:
        continue
        
    # Pivotar la tabla para poner los datasets en columnas contiguas
    df_pivot = df_est.pivot(index='cantidad_consultas', columns='dataset', values=['mean', 'std']).reset_index()
    
    # Aplanar las columnas
    df_pivot.columns = [
        'N', 
        'Promedio user_ids (s)', 'Promedio user_screen_names (s)', 
        'Desv. Est. user_ids', 'Desv. Est. user_screen_names'
    ]
    
    # Ordenar por el tamaño N
    df_pivot = df_pivot.sort_values(by='N')
    
    # --- CONVERSIÓN DE LA DESVIACIÓN ESTÁNDAR A MILISEGUNDOS (ms) ---
    df_pivot['Desv. Est. user_ids (ms)'] = df_pivot['Desv. Est. user_ids'] * 1000
    df_pivot['Desv. Est. user_screen_names (ms)'] = df_pivot['Desv. Est. user_screen_names'] * 1000
    
    # --- Formatear como string con 6 decimales fijos (Sin notación científica) ---
    df_pivot['Promedio user_ids (s)'] = df_pivot['Promedio user_ids (s)'].map(lambda x: f"{x:.6f}" if pd.notnull(x) else "0.000000")
    df_pivot['Promedio user_screen_names (s)'] = df_pivot['Promedio user_screen_names (s)'].map(lambda x: f"{x:.6f}" if pd.notnull(x) else "0.000000")
    df_pivot['Desv. Est. user_ids (ms)'] = df_pivot['Desv. Est. user_ids (ms)'].map(lambda x: f"{x:.6f}" if pd.notnull(x) else "0.000000")
    df_pivot['Desv. Est. user_screen_names (ms)'] = df_pivot['Desv. Est. user_screen_names (ms)'].map(lambda x: f"{x:.6f}" if pd.notnull(x) else "0.000000")
    # Formatear la columna N con separador de miles
    df_pivot['N'] = df_pivot['N'].apply(lambda x: f"{x:,}")
    
    # Seleccionar y ordenar las columnas finales
    columnas_finales = [
        'N', 
        'Promedio user_ids (s)', 'Desv. Est. user_ids (ms)', 
        'Promedio user_screen_names (s)', 'Desv. Est. user_screen_names (ms)'
    ]
    df_final = df_pivot[columnas_finales]
    

    # --- CREACIÓN DE LA IMAGEN DE LA TABLA ---
    
    # Calcular el tamaño dinámico ajustado
    num_filas = len(df_final)
    fig, ax = plt.subplots(figsize=(12, 0.22 * num_filas + 1.0)) 
    ax.axis('off') 
    
    tabla_img = ax.table(
        cellText=df_final.values, 
        colLabels=df_final.columns, 
        cellLoc='center', 
        loc='center'
    )
    
    # Ajustar el ancho al texto de cada columna
    tabla_img.auto_set_column_width(col=list(range(len(df_final.columns)))) 
    
    # Estilizar la tabla
    tabla_img.auto_set_font_size(False)
    tabla_img.set_fontsize(10)
    tabla_img.scale(1.0, 1.3) 
    
    for (fila, col), celda in tabla_img.get_celld().items():
        if fila == 0:
            celda.set_text_props(weight='bold', color='white')
            celda.set_facecolor('#404040') 
        elif fila % 2 == 0:
            celda.set_facecolor('#f9f9f9') 
            

    plt.title(f"Tabla de Rendimiento: {est_nombre}", fontsize=14, fontweight='bold', y=1.04)
    fig.subplots_adjust(left=0.05, right=0.95, top=0.90, bottom=0.05)
    
    nombre_archivo = f"tabla_{est_archivo}.png"
    plt.savefig(nombre_archivo, dpi=300)
    plt.close()