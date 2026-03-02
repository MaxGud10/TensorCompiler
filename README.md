# Tensor Compiler
- yet only Frontend

---
## Installation process

- **Clone the repository**
```bash
git clone git@github.com:MaxGud10/TensorCompiler.git
```
- **Navigate to the repository directory** 
- **Build the project using the following sequence of commands:**
```bash
cmake -B build
cmake --build build
```
----
## How to use (Frontend) 
### ONNX -> Neural Network Graph 

- **To run the project proceed the following command**
```bash
./build/onnx2dot <src.onnx> <dst.dot>
```
- **The output displays can be used to build the graph with the use of graphviz software, e.g. to generate png representation**

```
 dot -Tpng dst.dot -o dst.png
```

### Testing


----