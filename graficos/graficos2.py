import matplotlib.pyplot as plt

data_sizes = ['10k datos', '50k datos', '100k datos']

avl_file_accesses = [1016771,  6821978, 13959706]
sequential_file_accesses = [103834, 183834, 283834]
hash_file_accesses = [46646, 233313, 466646]

plt.plot(data_sizes, avl_file_accesses, marker='o', label='AVL File')
plt.plot(data_sizes, sequential_file_accesses, marker='o', label='Sequential File')
plt.plot(data_sizes, hash_file_accesses, marker='o', label='Hash File')

plt.xlabel('Tamaño de los Datos')
plt.ylabel('Accesos a Memoria Secundaria')
plt.title('Comparación de Accesos a Memoria Secundaria INSERCCION')
plt.legend()
plt.grid(True)

plt.show()