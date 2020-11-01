# city-profile
svg display of city profile problem and solved results

## problem

Given a series of 2d blocks representing buildings (can overlap), find out city profile.

## representation

Each building is a `top` with

- x: left position
- y: top position
- w: width

A series of this triple would be the input, also solution.

## display

svg is used to display buildins and profile.

## usage

```
city-profile [options...]

options:

--help
-h                  this help

--solve=0/1
-s0/1               solve (default 1)

--debug=0/1
-d0/1               debug (default 1)

--max=int
-m int              max tops to alloc (default 2048)

--input=input
-i input            input tops file (default <STDIN>)

--output=output
-o output           output svg file (defualt output.svg)

--profile=profile
-p profile          output profile file (default <STDOUT>)
```

## example
```
$ ./city-profile -d0 < city-profile.txt 
# 12 tops read
# 12 tops sorted
# 2036 tops left
# 12 tops copied
# 11 tops solved
5 15 5
10 20 5
15 30 10
25 15 5
30 40 10
40 80 10
50 50 22
72 20 18
100 40 5
105 20 5
110 10 5
# generated output.svg
```
![output.svg](output.svg)
