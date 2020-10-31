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
-h          this help
--solve=0/1
-s0/1       solve (default 1)
--debug=0/1
-d0/1       debug (default 1)
--max=int
-m int      max tops to alloc
--input=input
-i input    input tops (x y w)
--output=output
-o output   output svg file
```

    ./city-profile < city-profile.txt

