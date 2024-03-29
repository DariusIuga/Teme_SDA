# Assignment 2 - Quadtree-based immage compression

## Usage

Clone the repository
```bash
git clone git@github.com:DariusIuga/Teme_SDA.git
```
Go to the project directory
```bash
cd Teme_SDA/Tema2/
```
Build the executable
```bash
make
```
Run the checker (You might have to grant it execution permissions)
```bash
./check.sh
```

On the local checker, I obtained a score of 54 out of 80 on tests and a bonus of 8 points on Valgrind.

## Implementation Description:

**Task 1:** The values written in the file containing the compression tree statistics are stored in an output structure.

- `stats:` The function calls the `calculate_mean` function to compute the mean values for the three colors and the similarity score. This mean is used as a criterion for the recursive calls of the function. If the mean is greater than the compression factor tolerance, the four quadrants of the image are analyzed. The three output values are calculated throughout the function.
    
- `write_text:` Used to write the output data to the "out" file with the name given in the command line.

**Task 2:**

- Used a structure to store a vector and retain its current length and capacity (allocated memory).
    
- `compression:` The function is similar to the one used in the first task. After the call to `calculate_mean`, the current node is added to the vector through the `add_cell` function. If the mean is greater than the given factor, the four children are generated through `generate_subtrees`, and the current function is recursively called for each of them.
    
- `add_cell:` This function represents all nodes in the quaternary compression tree in vector form. For each node, I store its type, and if it is a leaf, I assign the RGB values. When the vector's capacity is reached, the allocated memory is doubled through realloc. Since this function is called within a recursive function, nodes at the same level cannot be traversed consecutively. However, nodes at the same level in the tree are represented in order in the vector. For each element read in the vector, I also record the level of a node, and the vector can be correctly sorted through qsort based on the levels.
    
- `write_binary:` Writes the image's size, node type, and color values to the binary "out" file.

**Task 3:** I use the same structure as in task 2 for the vector.

- `read_binary:` Allocates memory and reads data from the binary "out" file into the vector.
    
- `vector_to_tree:` Aims to allocate memory for the quaternary tree and assign values from the vector: for leaves, I read the RGB values, and for internal nodes, I recursively call the function.
    
- `generate_image:` For all leaf nodes, I write the RGB values in their respective areas. For internal nodes, I recursively call the function.

- `write_ppm:` Writes the header and RGB values of pixels from the compressed image to the binary ".ppm" file.
