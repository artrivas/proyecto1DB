import matplotlib.pyplot as plt

data_sizes = ['10k datos', '50k datos', '100k datos']

avl_file_accesses = [817644, 4807498, 10203070]
sequential_file_accesses = [8, 10, 9]
hash_file_accesses = [216, 1052,2098]

plt.plot(data_sizes, avl_file_accesses, marker='o', label='AVL File')
plt.plot(data_sizes, sequential_file_accesses, marker='o', label='Sequential File')
plt.plot(data_sizes, hash_file_accesses, marker='o', label='Hash File')

plt.xlabel('Tamaño de los Datos')
plt.ylabel('Accesos a Memoria Secundaria')
plt.title('Comparación de Accesos a Memoria Secundaria para busqueda')
plt.legend()
plt.grid(True)

plt.show()