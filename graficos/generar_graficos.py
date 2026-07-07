import pandas as pd
import numpy as np
import matplotlib.pyplot as plt

# Código hecho con asistencia de gemini para generar los gráficos en un formato similar a ayudantías.

# 1. Leer el archivo CSV especificando el delimitador de punto y coma
df = pd.read_csv('resultados.csv', delimiter=';')

# 2. Agrupar por cantidad de consultas, dataset y estructura para obtener el promedio
df_promedios = df.groupby(['cantidad_consultas', 'dataset', 'estructura_de_datos'], as_index=False)['tiempo_ejecucion'].mean()

# 3. Separar por dataset para graficar curvas distintas (user_ids y user_screen_names)
df_ids = df_promedios[df_promedios['dataset'] == 'user_ids']
df_names = df_promedios[df_promedios['dataset'] == 'user_screen_names']

# 4. Inicializar la figura con 5 subplots verticales
fig, axs = plt.subplots(5, 1, figsize=(10, 26))

# Se ajusta y=0.985 para darle una separación perfecta al título principal
fig.suptitle('Análisis de Rendimiento de Tablas Hash\n(Tiempo Promedio de Inserción vs. Cantidad de Datos)', 
             fontsize=16, fontweight='bold', y=0.985)

# Auxiliar para extraer los ejes X (N) y Y (Tiempo en segundos) filtrados
def obtener_datos(df_origen, est):
    sub_df = df_origen[df_origen['estructura_de_datos'] == est].sort_values('cantidad_consultas')
    return sub_df['cantidad_consultas'], sub_df['tiempo_ejecucion']

# --- CONFIGURACIÓN DE LOS GRÁFICOS ---

# # Hashing Abierto
N_ids, Time_ids = obtener_datos(df_ids, 'abierto')
N_names, Time_names = obtener_datos(df_names, 'abierto')
axs[0].plot(N_ids, Time_ids, marker='o', color='tab:brown', label='user_ids')
axs[0].plot(N_names, Time_names, marker='x', color='chocolate', label='user_screen_names')
axs[0].set_title('Análisis de Tiempo de Inserción (Hashing Abierto)', fontsize=12, fontweight='bold')
axs[0].set_ylabel('Tiempo T(N) en Segundos')
axs[0].legend()
axs[0].set_yscale('log')
axs[0].grid(True, which="both", ls="--", alpha=0.5)

# # Sondeo Lineal
N_ids, Time_ids = obtener_datos(df_ids, 'lineal')
N_names, Time_names = obtener_datos(df_names, 'lineal')
axs[1].plot(N_ids, Time_ids, marker='o', color='tab:red', label='user_ids')
axs[1].plot(N_names, Time_names, marker='x', color='tab:orange', label='user_screen_names')
axs[1].set_title('Análisis de Tiempo de Inserción (Lineal)', fontsize=12, fontweight='bold')
axs[1].set_ylabel('Tiempo T(N) en Segundos')
axs[1].legend()
axs[1].set_yscale('log')
axs[1].grid(True, which="both", ls="--", alpha=0.5)

# # Sondeo Cuadrático
N_ids, Time_ids = obtener_datos(df_ids, 'cuadratico')
N_names, Time_names = obtener_datos(df_names, 'cuadratico')
axs[2].plot(N_ids, Time_ids, marker='o', color='tab:blue', label='user_ids')
axs[2].plot(N_names, Time_names, marker='x', color='tab:cyan', label='user_screen_names')
axs[2].set_title('Análisis de Tiempo de Inserción (Cuadrático)', fontsize=12, fontweight='bold')
axs[2].set_ylabel('Tiempo T(N) en Segundos')
axs[2].legend()
axs[2].set_yscale('log')
axs[2].grid(True, which="both", ls="--", alpha=0.5)

# # Hashing Doble
N_ids, Time_ids = obtener_datos(df_ids, 'doble')
N_names, Time_names = obtener_datos(df_names, 'doble')
axs[3].plot(N_ids, Time_ids, marker='o', color='tab:purple', label='user_ids')
axs[3].plot(N_names, Time_names, marker='x', color='tab:pink', label='user_screen_names')
axs[3].set_title('Análisis de Tiempo de Inserción (Hashing Doble)', fontsize=12, fontweight='bold')
axs[3].set_ylabel('Tiempo T(N) en Segundos')
axs[3].legend()
axs[3].set_yscale('log')
axs[3].grid(True, which="both", ls="--", alpha=0.5)

# # Mapa No Ordenado (std::unordered_map)
N_ids, Time_ids = obtener_datos(df_ids, 'unordered_map')
N_names, Time_names = obtener_datos(df_names, 'unordered_map')
axs[4].plot(N_ids, Time_ids, marker='o', color='tab:green', label='user_ids')
axs[4].plot(N_names, Time_names, marker='x', color='tab:olive', label='user_screen_names')
axs[4].set_title('Análisis de Tiempo de Inserción (std::unordered_map)', fontsize=12, fontweight='bold')
axs[4].set_ylabel('Tiempo T(N) en Segundos')
axs[4].legend()
axs[4].set_yscale('log')
axs[4].grid(True, which="both", ls="--", alpha=0.5)


# 5. Configurar de forma individual las marcas del eje X para cada uno de los 5 subplots
max_n = int(df_promedios['cantidad_consultas'].max())
ticks_x = np.arange(10000, max_n + 10000, 10000)
etiquetas_x = [f"{x:,}" for x in ticks_x]

for ax in axs:
    ax.set_xticks(ticks_x)
    ax.set_xticklabels(etiquetas_x, rotation=45, ha='right')
    ax.set_xlabel('Cantidad de Elementos Insertados (N)')

plt.tight_layout()
plt.subplots_adjust(hspace=0.6, top=0.94) 

plt.savefig('grafico_rendimiento_segundos.png', dpi=300)
plt.show()