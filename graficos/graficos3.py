import matplotlib.pyplot as plt

data_sizes = ['10k datos', '50k datos', '100k datos']

avl_file_times = [384, 342, 940]
sequential_file_times = [87, 254, 123]
hash_file_times = [49372, 145496, 257794]

plt.plot(data_sizes, avl_file_times, marker='o', label='AVL File')
plt.plot(data_sizes, sequential_file_times, marker='o', label='Sequential File')
plt.plot(data_sizes, hash_file_times, marker='o', label='Hash File')

plt.xlabel('Tamaño de los Datos')
plt.ylabel('Tiempos de Ejecución (µs)')
plt.title('Comparación de Tiempos de Ejecución para busqueda')
plt.legend()
plt.grid(True)

plt.show()