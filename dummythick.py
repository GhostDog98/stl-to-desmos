import random

def format_vertex(value):
    return format(value, ".6e").replace('e+', 'e-') if random.choice([True, False]) else format(value, ".6e")

def generate_vertex():
    return tuple(format_vertex(random.uniform(0, 1e5)) for _ in range(3))


num_triangles_to_generate = 50
output_file_path = "dummy.stl"
stl_content = "solid DummySTL\n"

for _ in range(num_triangles_to_generate):
    vertices = [generate_vertex() for _ in range(3)]

    stl_content += ''.join([
        f"  facet normal 0 0 0\n"
        f"    outer loop\n"
        f"      vertex {' '.join(map(str, vertex))}\n" for vertex in vertices
    ])
    stl_content += "    endloop\n  endfacet\n"

# Repetition of stl_content (1000 times)
num_repititions = 40000
stl_content *= num_repititions

stl_content += f"endsolid DummySTL\n"

with open(output_file_path, 'w') as f:
    f.write(stl_content)

print(f"Dummy STL file '{output_file_path}' generated with {num_triangles_to_generate + num_repititions} triangles.")
