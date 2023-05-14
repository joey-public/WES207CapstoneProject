# Using Eigen with CMAKE 

## Step 1: Download Eigen

download the `tar.gz` file with the eigen library from here: 

http://eigen.tuxfamily.org/index.php?title=Main_Page#Download

## Step 2: Install Eigen 

Use the following commands in your terminal to install the library.

```
cd ~
mkdir mypackages
cd mypackages
cp ~/Downloads/eigen-3.4.0.tar.gz ./
tar xvzf eigen-3.3.0.tar.gz 
cd eigen-3.4.0/
mkdir build
cd build
cmake ../
make install
```

## Step 3: Update CMakeLists.txt

add the following lines to your CmakeList.txt 

```
find_package (Eigen3 3.3 REQUIRED NO_MODULE)
target_link_libraries (main Eigen3::Eigen) 
```

make sure to add the `target_link_libraries()` call after the `add_executable()` line or the cmake call will fail.  

## Step 4: Add cmake flag when building project
When running cmake to builf your uhd project add the following flag

```
cmake ../ -DEigen3_DIR=$HOME/mypackages/
```
