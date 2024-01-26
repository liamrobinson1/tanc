.PHONY: clean pybind11 all

all: clean
	source bin/activate && python setup.py build_ext --inplace

pybind11:
	rm -rf pybind11
	git clone https://github.com/pybind/pybind11
	cd pybind11 && mkdir build && cd build && cmake .. -DDOWNLOAD_EIGEN=ON && make check -j 4

clean:
	rm -rf build
	rm -rf *.so

test:
	export PYTHONPATH="./:${PYTHONPATH}" && pytest tests/*.py