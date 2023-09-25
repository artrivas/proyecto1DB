import matplotlib.pyplot as plt

data_sizes = ['10k datos', '50k datos', '100k datos']

avl_file_times = [2905,  19254, 37598]
sequential_file_times = [37448, 206006, 457048]
hash_file_times = [ 46324, 185251, 227971]

plt.plot(data_sizes, avl_file_times, marker='o', label='AVL File')
plt.plot(data_sizes, sequential_file_times, marker='o', label='Sequential File')
plt.plot(data_sizes, hash_file_times, marker='o', label='Hash File')

plt.xlabel('Tamaño de los Datos')
plt.ylabel('Tiempos de Ejecución (ms)')
plt.title('Comparación de Tiempos de Ejecución INSERTAR')
plt.legend()
plt.grid(True)
plt.show()
