import random

def generate_dummy_stl(num_triangles=100):
    stl_content = f"solid DummySTL\n"

    for _ in range(num_triangles):
        def format_vertex(value):
            if random.choice([True, False]):  # 50/50 chance of having a negative exponent
                return format(value, ".6e").replace('e+', 'e-')
            else:
                return format(value, ".6e").replace('e-', 'e+')

        vertex1 = (
            format_vertex(random.uniform(0, 1e5)),
            format_vertex(random.uniform(0, 1e5)),
            format_vertex(random.uniform(0, 1e5))
        )
        vertex2 = (
            format_vertex(random.uniform(0, 1e5)),
            format_vertex(random.uniform(0, 1e5)),
            format_vertex(random.uniform(0, 1e5))
        )
        vertex3 = (
            format_vertex(random.uniform(0, 1e5)),
            format_vertex(random.uniform(0, 1e5)),
            format_vertex(random.uniform(0, 1e5))
        )

        stl_content += (
            f"  facet normal 0 0 0\n"
            f"    outer loop\n"
            f"      vertex {vertex1[0]} {vertex1[1]} {vertex1[2]}\n"
            f"      vertex {vertex2[0]} {vertex2[1]} {vertex2[2]}\n"
            f"      vertex {vertex3[0]} {vertex3[1]} {vertex3[2]}\n"
            f"    endloop\n"
            f"  endfacet\n"
        )

    stl_content += f"endsolid DummySTL\n"

    return stl_content

def save_stl_file(file_path, stl_content):
    with open(file_path, 'w') as f:
        f.write(stl_content)

if __name__ == "__main__":
    num_triangles_to_generate = 100
    output_file_path = "dummy.stl"

    dummy_stl_content = generate_dummy_stl(num_triangles_to_generate)
    save_stl_file(output_file_path, dummy_stl_content)

    print(f"Dummy STL file '{output_file_path}' generated with {num_triangles_to_generate} triangles.")
