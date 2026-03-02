#!/bin/bash

mkdir ./output_dots

for file in test_models/*.onnx; do
    ./build/onnx2dot "$file" "output_dots/$(basename "${file%.onnx}").dot"
done

mkdir ./png_res

for file in output_dots/*.dot; do
    dot -Tpng "$file" -o "png_res/$(basename "${file%.dot}").png"
done