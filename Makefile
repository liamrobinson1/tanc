.PHONY: clean pybind11 all

all: clean
	source bin/activate && pip install -e .

pybind11:
	rm -rf pybind11
	git clone https://github.com/pybind/pybind11
	cd pybind11 && mkdir build && cd build && cmake .. -DDOWNLOAD_EIGEN=ON

eigen:
	rm -rf eigen
	git clone https://gitlab.com/libeigen/eigen.git

clean:
	rm -rf build
	rm -rf *.so

test:
	export PYTHONPATH="./:${PYTHONPATH}" && pytest tests/*.py