# Python Setup Guide

We have few utiltiy python scripts we have been using to quickly analyze raw data as we collect it. We highly reccomend using a python venv with this project so you can easily run any of the scripts we created. 

## venev Creation 

To create a new python virtual enviornment you can use the following commands. 

```
cd ~
mkdir python_envs
cd python_envs
python 3 -m venv my_venv
```

Note `my_env` can be any name you want to choose for your enviornment. 

Depending on the version of python 3 on your operating system the last line here might fail. You may need to run the following command to install python virtual enviornmtns. 

```
command goes here
```

## venv Actiacation

To activate the newly created python enviornment you just need to use the following command

```
source ~/python_envs/my_env/bin/actiavate 
```

If the python env is activated you should see the `(my_env)` indicator on the far left of your terminal prompt. 

## Insalling python packages

To install all the python libraries we used for our scripts you can use the `requiremtns.txt` file at the root of the github repo. First navigate to the `WES207CapstoneProject` director where you cloned the repo. Then run the following command:

first check the currently installed librries with 

```
pip list
```

then you can install all the dependencies with 

```
python -m pip install --upgrade pip
python -m pip install -r requiremtns.txt
```

now you can check if the libraries were installed:
```
pip list
```



